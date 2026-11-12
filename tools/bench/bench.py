# STALE: targets the removed amrex_contact contact_dem binaries; port to
# `build/delta --backend amrex --mode <mode>` before trusting any output.
#!/usr/bin/env python3
"""Benchmark report for the amrex-contact app CSV output.

Reads one or more CSV files with the columns

    mode,narrowphase,step,time,dt,contacts,candidate_pairs,tri_pairs,
    contact_checks,kinetic,penetration,wall_time_ms

(extra columns are ignored, missing optional columns are reported as "-") and
prints a markdown summary: per (mode, narrowphase) group the number of steps,
final simulated time, mean/max dt, contact / candidate-pair / triangle-pair /
contact-check totals, wall time and the speedup relative to the
``nogrid``+``brute`` baseline.

PNG plots (dt, contacts, candidate pairs vs time, wall-time bars) are produced
only when matplotlib is importable; without it the report still works and the
plots are skipped.  numpy is never required directly (matplotlib may need it,
which is why the import is guarded).

``--verify`` compares the contact-count column across modes: the final contact
count and final simulated time of every group must match the reference group
within the given tolerances (dynamic-dt runs legitimately take different numbers
of steps, so step-by-step equality is only checked with ``--strict``).

Pure standard library.  Usage:  python3 tools/bench.py --help

Exit codes: 0 success, 1 verification failure, 2 bad input.
"""

from __future__ import annotations

import argparse
import csv
import glob
import os
import sys
from dataclasses import dataclass, field
from typing import Dict, Iterable, List, Optional, Sequence, Tuple

COLUMNS = [
    "mode",
    "narrowphase",
    "step",
    "time",
    "dt",
    "contacts",
    "candidate_pairs",
    "tri_pairs",
    "contact_checks",
    "kinetic",
    "penetration",
    "wall_time_ms",
]

# Columns that are summed over the run, and those that are averaged.
SUM_FIELDS = ("contacts", "candidate_pairs", "tri_pairs", "contact_checks", "wall_time_ms")


@dataclass
class Sample:
    mode: str
    narrowphase: str
    step: int
    time: float
    dt: float
    contacts: int
    candidate_pairs: int
    tri_pairs: int
    contact_checks: int
    kinetic: float
    penetration: float
    wall_time_ms: float


@dataclass
class Group:
    """All samples of one (mode, narrowphase) pair, from one or more files."""

    mode: str
    narrowphase: str
    samples: List[Sample] = field(default_factory=list)
    sources: List[str] = field(default_factory=list)

    @property
    def label(self) -> str:
        return self.mode if not self.narrowphase else f"{self.mode}/{self.narrowphase}"

    def sorted_samples(self) -> List[Sample]:
        return sorted(self.samples, key=lambda s: s.step)

    # -- aggregates ---------------------------------------------------------
    def steps(self) -> int:
        return len(self.samples)

    def final_time(self) -> float:
        return max((s.time for s in self.samples), default=float("nan"))

    def mean_dt(self) -> float:
        if not self.samples:
            return float("nan")
        return sum(s.dt for s in self.samples) / len(self.samples)

    def max_dt(self) -> float:
        return max((s.dt for s in self.samples), default=float("nan"))

    def total(self, name: str) -> float:
        return float(sum(getattr(s, name) for s in self.samples))

    def final_contacts(self) -> int:
        if not self.samples:
            return 0
        return self.sorted_samples()[-1].contacts

    def max_penetration(self) -> float:
        return max((s.penetration for s in self.samples), default=0.0)


# ---------------------------------------------------------------------------
# Parsing
# ---------------------------------------------------------------------------


def _to_float(text: Optional[str], default: float = 0.0) -> float:
    if text is None:
        return default
    t = text.strip()
    if not t:
        return default
    try:
        return float(t)
    except ValueError:
        return default


def _to_int(text: Optional[str], default: int = 0) -> int:
    if text is None:
        return default
    t = text.strip()
    if not t:
        return default
    try:
        return int(float(t))
    except ValueError:
        return default


def read_csv(path: str) -> Tuple[List[Sample], List[str]]:
    """Return (samples, warnings) for one CSV file."""
    warnings: List[str] = []
    samples: List[Sample] = []
    with open(path, newline="", encoding="utf-8", errors="replace") as fh:
        reader = csv.DictReader(fh)
        if reader.fieldnames is None:
            raise ValueError(f"{path}: empty file (no header)")
        header = [h.strip() for h in reader.fieldnames]
        missing = [c for c in ("mode", "step", "time", "dt") if c not in header]
        if missing:
            raise ValueError(f"{path}: missing required column(s): {', '.join(missing)}")
        optional = [c for c in COLUMNS if c not in header]
        if optional:
            warnings.append(f"{path}: missing optional column(s): {', '.join(optional)} "
                            "(reported as 0)")
        for row in reader:
            if not row:
                continue
            mode = (row.get("mode") or "").strip()
            if not mode:
                continue
            samples.append(
                Sample(
                    mode=mode,
                    narrowphase=(row.get("narrowphase") or "").strip(),
                    step=_to_int(row.get("step")),
                    time=_to_float(row.get("time")),
                    dt=_to_float(row.get("dt")),
                    contacts=_to_int(row.get("contacts")),
                    candidate_pairs=_to_int(row.get("candidate_pairs")),
                    tri_pairs=_to_int(row.get("tri_pairs")),
                    contact_checks=_to_int(row.get("contact_checks")),
                    kinetic=_to_float(row.get("kinetic")),
                    penetration=_to_float(row.get("penetration")),
                    wall_time_ms=_to_float(row.get("wall_time_ms")),
                )
            )
    if not samples:
        warnings.append(f"{path}: no data rows")
    return samples, warnings


def expand_paths(patterns: Sequence[str]) -> List[str]:
    paths: List[str] = []
    for pattern in patterns:
        if os.path.isdir(pattern):
            paths.extend(sorted(glob.glob(os.path.join(pattern, "*.csv"))))
        else:
            matches = sorted(glob.glob(pattern))
            paths.extend(matches if matches else [pattern])
    return paths


def load_groups(paths: Sequence[str], split_by_file: bool = False) -> Tuple[Dict[Tuple[str, str], Group], List[str]]:
    groups: Dict[Tuple[str, str], Group] = {}
    warnings: List[str] = []
    for path in paths:
        if not os.path.isfile(path):
            raise FileNotFoundError(path)
        samples, warns = read_csv(path)
        warnings.extend(warns)
        for s in samples:
            key = (s.mode, s.narrowphase)
            if split_by_file:
                key = (s.mode, f"{s.narrowphase}@{os.path.basename(path)}")
            g = groups.setdefault(key, Group(mode=key[0], narrowphase=key[1]))
            g.samples.append(s)
            if path not in g.sources:
                g.sources.append(path)
    return groups, warnings


# ---------------------------------------------------------------------------
# Reporting
# ---------------------------------------------------------------------------


def _fmt(value: float, digits: int = 4) -> str:
    if value is None:
        return "-"
    if value != value:  # NaN
        return "-"
    if value == float("inf"):
        return "inf"
    if value == 0:
        return "0"
    if abs(value) >= 1e5 or abs(value) < 1e-3:
        return f"{value:.{digits}e}"
    return f"{value:.{digits}f}"


def _fmt_int(value: float) -> str:
    return f"{int(round(value)):,}"


def find_baseline(groups: Iterable[Group]) -> Optional[Group]:
    """nogrid+brute is the reference; otherwise the slowest group."""
    groups = list(groups)
    for g in groups:
        if g.mode.lower().replace("-", "").replace("_", "") == "nogrid" and (
            g.narrowphase.lower() == "brute" or not g.narrowphase
        ):
            return g
    for g in groups:
        if g.mode.lower().replace("-", "").replace("_", "") == "nogrid":
            return g
    return max(groups, key=lambda g: g.total("wall_time_ms"), default=None)


def summary_table(groups: Sequence[Group], baseline: Optional[Group]) -> str:
    base_wall = baseline.total("wall_time_ms") if baseline is not None else 0.0
    header = (
        "| mode | narrowphase | steps | final time | mean dt | max dt | contacts "
        "| candidate pairs | tri pairs | contact checks | wall [ms] | speedup |"
    )
    sep = "|" + "---|" * 12
    lines = [header, sep]
    for g in groups:
        wall = g.total("wall_time_ms")
        if base_wall > 0 and wall > 0:
            speed = f"{base_wall / wall:.2f}x"
        else:
            speed = "-"
        lines.append(
            "| {mode} | {np} | {steps} | {tend} | {mdt} | {xdt} | {c} | {cp} | {tp} | {cc} | {w} | {sp} |".format(
                mode=g.mode,
                np=g.narrowphase or "-",
                steps=g.steps(),
                tend=_fmt(g.final_time()),
                mdt=_fmt(g.mean_dt(), 6),
                xdt=_fmt(g.max_dt(), 6),
                c=_fmt_int(g.total("contacts")),
                cp=_fmt_int(g.total("candidate_pairs")),
                tp=_fmt_int(g.total("tri_pairs")),
                cc=_fmt_int(g.total("contact_checks")),
                w=_fmt(wall, 1),
                sp=speed,
            )
        )
    if baseline is not None:
        lines.append("")
        lines.append(f"Baseline for the speedup column: **{baseline.label}**.")
    return "\n".join(lines)


def efficiency_table(groups: Sequence[Group]) -> str:
    lines = [
        "| mode | cand. pairs/step | tri pairs/step | checks/contact | contacts/step | max penetration |",
        "|---|---|---|---|---|---|",
    ]
    for g in groups:
        n = max(1, g.steps())
        contacts = g.total("contacts")
        checks = g.total("contact_checks")
        lines.append(
            f"| {g.mode} | {_fmt(g.total('candidate_pairs') / n, 1)} | "
            f"{_fmt(g.total('tri_pairs') / n, 1)} | "
            f"{_fmt(checks / contacts if contacts else 0.0, 2)} | "
            f"{_fmt(contacts / n, 2)} | {_fmt(g.max_penetration(), 3)} |"
        )
    return "\n".join(lines)


def dt_histogram(groups: Sequence[Group], bins: int = 10, width: int = 32) -> str:
    """Coarse ASCII histogram of dt per group (useful without matplotlib)."""
    out: List[str] = []
    for g in groups:
        dts = [s.dt for s in g.samples if s.dt > 0]
        if not dts:
            continue
        lo, hi = min(dts), max(dts)
        if hi <= lo:
            hi = lo * (1.0 + 1e-12) + 1e-30
        counts = [0] * bins
        for d in dts:
            idx = min(bins - 1, int((d - lo) / (hi - lo) * bins))
            counts[idx] += 1
        peak = max(counts) or 1
        out.append(f"`{g.label}` dt in [{_fmt(lo, 6)}, {_fmt(hi, 6)}], {len(dts)} steps")
        out.append("```")
        for i, c in enumerate(counts):
            edge_lo = lo + (hi - lo) * i / bins
            bar = "#" * max(0, int(round(width * c / peak)))
            out.append(f"{_fmt(edge_lo, 6):>12} | {bar} {c}")
        out.append("```")
        out.append("")
    return "\n".join(out).rstrip()


# ---------------------------------------------------------------------------
# Verification
# ---------------------------------------------------------------------------


def verify(groups: Sequence[Group], reference: Optional[Group], tol: int, time_tol: float,
           strict: bool) -> Tuple[bool, str]:
    if reference is None:
        return False, "FAIL: no reference group to compare against"
    lines: List[str] = []
    ok = True
    ref_end = reference.final_time()
    ref_contacts = reference.final_contacts()

    lines.append(f"Reference: **{reference.label}** — {reference.steps()} steps, "
                 f"t_final={_fmt(ref_end, 6)}, contacts={ref_contacts}")
    lines.append("")
    lines.append("| mode | steps | t_final | final contacts | d(contacts) | result |")
    lines.append("|---|---|---|---|---|---|")

    for g in groups:
        d_contacts = g.final_contacts() - ref_contacts
        same_time = abs(g.final_time() - ref_end) <= time_tol * max(1.0, abs(ref_end))
        # Contact counts are the hard criterion.  A differing final time only
        # matters with --strict: dynamic-dt runs may legitimately overshoot t_end
        # by up to one step, which makes the raw contact counts hard to compare.
        good = abs(d_contacts) <= tol
        if strict and not same_time:
            good = False
        if g is reference:
            good = True
        if strict:
            ref_map = {s.step: s.contacts for s in reference.samples}
            mismatched = [
                s.step for s in g.samples if s.step in ref_map and abs(s.contacts - ref_map[s.step]) > tol
            ]
            if mismatched:
                good = False
                lines.append(
                    f"| {g.label} | {g.steps()} | {_fmt(g.final_time(), 6)} | "
                    f"{g.final_contacts()} | {d_contacts:+d} | FAIL (steps {mismatched[:5]}) |"
                )
                ok = False
                continue
        ok = ok and good
        note = "" if same_time else f" (t_final differs by {g.final_time() - ref_end:+.3e})"
        lines.append(
            f"| {g.label} | {g.steps()} | {_fmt(g.final_time(), 6)} | {g.final_contacts()} | "
            f"{d_contacts:+d} | {'PASS' if good else 'FAIL'}{note} |"
        )

    lines.append("")
    if ok:
        lines.append(f"**RESULT: PASS** (all modes agree on the contact count at the final time, "
                     f"tolerance {tol})")
    else:
        lines.append("**RESULT: FAIL** (a mode disagrees on the contact count"
                     + (", or on the final time / per-step counts (--strict)" if strict else "")
                     + ")")
    return ok, "\n".join(lines)


# ---------------------------------------------------------------------------
# Plots (optional)
# ---------------------------------------------------------------------------


def make_plots(groups: Sequence[Group], outdir: str, title: str) -> List[str]:
    try:  # pragma: no cover - depends on the environment
        import matplotlib

        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
    except Exception as exc:  # noqa: BLE001 - any failure must degrade gracefully
        print(f"[plots] matplotlib unavailable ({exc.__class__.__name__}: {exc}); skipping PNGs",
              file=sys.stderr)
        return []

    os.makedirs(outdir, exist_ok=True)
    written: List[str] = []

    def series(g: Group):
        ss = g.sorted_samples()
        return [s.time for s in ss], ss

    specs = [
        ("dt", "time step dt", lambda s: s.dt),
        ("contacts", "contacts", lambda s: s.contacts),
        ("candidate_pairs", "candidate pairs", lambda s: s.candidate_pairs),
        ("tri_pairs", "triangle pairs", lambda s: s.tri_pairs),
    ]
    for name, ylabel, getter in specs:
        fig, ax = plt.subplots(figsize=(7.0, 4.0))
        for g in groups:
            t, ss = series(g)
            ax.plot(t, [getter(s) for s in ss], label=g.label, linewidth=1.2)
        ax.set_xlabel("simulated time")
        ax.set_ylabel(ylabel)
        ax.set_title(f"{title}: {ylabel}")
        if name == "dt":
            ax.set_yscale("log")
        ax.grid(True, alpha=0.3)
        ax.legend(fontsize=8)
        fig.tight_layout()
        path = os.path.join(outdir, f"{name}.png")
        fig.savefig(path, dpi=140)
        plt.close(fig)
        written.append(path)

    fig, ax = plt.subplots(figsize=(6.0, 3.6))
    labels = [g.label for g in groups]
    values = [g.total("wall_time_ms") for g in groups]
    ax.bar(range(len(groups)), values, color="#4C78A8")
    ax.set_xticks(range(len(groups)))
    ax.set_xticklabels(labels, rotation=20, ha="right", fontsize=8)
    ax.set_ylabel("wall time [ms]")
    ax.set_title(f"{title}: wall time")
    fig.tight_layout()
    path = os.path.join(outdir, "wall_time.png")
    fig.savefig(path, dpi=140)
    plt.close(fig)
    written.append(path)
    return written


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------


def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="bench.py",
        description="Summarise amrex-contact benchmark CSVs as a markdown report.",
        epilog=(
            "Expected columns: " + ",".join(COLUMNS) + ". "
            "Plots are written only if matplotlib is importable."
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    p.add_argument("csv", nargs="+", help="CSV file(s) or glob(s); a directory reads its *.csv")
    p.add_argument("--out", metavar="FILE", help="also write the markdown report to FILE")
    p.add_argument("--title", default="contact benchmark", help="report title")
    p.add_argument("--plots", metavar="DIR", default=None,
                   help="directory for PNGs (default: <first csv dir>/plots, skipped with --no-plots)")
    p.add_argument("--no-plots", action="store_true", help="never write PNGs")
    p.add_argument("--split-files", action="store_true",
                   help="keep groups from different CSV files separate (label mode/narrowphase@file)")
    p.add_argument("--histogram", action="store_true", help="add ASCII dt histograms")
    p.add_argument("--verify", action="store_true",
                   help="compare contact counts across modes and print PASS/FAIL (exit 1 on FAIL)")
    p.add_argument("--reference", metavar="MODE[:NARROWPHASE]",
                   help="verification reference group (default: nogrid/brute, else first)")
    p.add_argument("--tolerance", type=int, default=0, help="allowed |contact| difference (default 0)")
    p.add_argument("--time-tolerance", type=float, default=1e-6,
                   help="relative tolerance on the final simulated time (default 1e-6)")
    p.add_argument("--strict", action="store_true",
                   help="also require identical contact counts at equal step indices")
    return p


def select_reference(groups: Sequence[Group], spec: Optional[str]) -> Optional[Group]:
    if spec is None:
        return find_baseline(groups)
    want_mode, _, want_np = spec.partition(":")
    want_mode = want_mode.strip().lower()
    want_np = want_np.strip().lower()
    for g in groups:
        if g.mode.lower() == want_mode and (not want_np or g.narrowphase.lower() == want_np):
            return g
    return None


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_parser().parse_args(argv)

    try:
        paths = expand_paths(args.csv)
        groups_map, warnings = load_groups(paths, split_by_file=args.split_files)
    except (OSError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    for w in warnings:
        print(f"warning: {w}", file=sys.stderr)
    if not groups_map:
        print("error: no data rows found", file=sys.stderr)
        return 2

    groups = sorted(groups_map.values(), key=lambda g: (g.mode.lower(), g.narrowphase.lower()))
    baseline = find_baseline(groups)

    out: List[str] = [f"# {args.title}", ""]
    out.append("Files: " + ", ".join(f"`{p}`" for p in paths))
    out.append("")
    out.append("## Summary")
    out.append("")
    out.append(summary_table(groups, baseline))
    out.append("")
    out.append("## Efficiency")
    out.append("")
    out.append(efficiency_table(groups))
    if args.histogram:
        out.append("")
        out.append("## Step-size distribution")
        out.append("")
        out.append(dt_histogram(groups))

    exit_code = 0
    if args.verify:
        reference = select_reference(groups, args.reference)
        ok, text = verify(groups, reference, args.tolerance, args.time_tolerance, args.strict)
        out.append("")
        out.append("## Verification")
        out.append("")
        out.append(text)
        if not ok:
            exit_code = 1

    report = "\n".join(out).rstrip() + "\n"
    sys.stdout.write(report)
    if args.out:
        with open(args.out, "w", encoding="utf-8") as fh:
            fh.write(report)
        print(f"\n[report] wrote {args.out}", file=sys.stderr)

    if not args.no_plots:
        outdir = args.plots
        if outdir is None:
            first = paths[0] if paths else "."
            outdir = os.path.join(os.path.dirname(os.path.abspath(first)) or ".", "plots")
        written = make_plots(groups, outdir, args.title)
        if written:
            print("[plots] " + ", ".join(written), file=sys.stderr)

    return exit_code


if __name__ == "__main__":
    sys.exit(main())
