"""Figures + PDF for the parallel/SIMD contact-kernel work.

Reads the measurements from output/bench.log (written by
tools/bench/parallel_bench), draws the figures and builds
output/report/parallel_report.pdf.

    ./build/parallel_bench > output/bench.log
    python3 tools/report/make_parallel_pdf.py
"""
import os
import re

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import FancyArrowPatch, FancyBboxPatch

from reportlab.lib import colors
from reportlab.lib.enums import TA_JUSTIFY
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import cm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import (BaseDocTemplate, Frame, Image, KeepTogether,
                                ListFlowable, ListItem, PageTemplate, Paragraph,
                                Preformatted, Spacer, Table, TableStyle)

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
R = os.environ.get('DELTA_REPORT_DIR', os.path.join(ROOT, 'output', 'report'))
BENCH = os.environ.get('DELTA_BENCH_LOG', os.path.join(ROOT, 'output', 'bench.log'))
OUT = os.path.join(R, 'parallel_report.pdf')
os.makedirs(R, exist_ok=True)

plt.rcParams.update({'font.size': 10, 'font.family': 'DejaVu Sans',
                     'axes.grid': True, 'grid.alpha': 0.3,
                     'axes.spines.top': False, 'axes.spines.right': False,
                     'mathtext.fontset': 'dejavusans'})

NAVY = '#2c3e50'
ACCENT = '#c0392b'
BLUE = '#2e86c1'

# --------------------------------------------------------------- parse bench
def parse_bench(path):
    """{section: ...} from parallel_bench's stdout."""
    hopper, pile, kernels = {}, {}, {}
    section = None
    model = None
    with open(path) as f:
        for line in f:
            line = line.rstrip()
            if line.startswith('1. hopper'):
                section = 'hopper'
            elif line.startswith('2. 36-box'):
                section = 'pile'
            elif line.startswith('3. single kernel'):
                section = 'kernel'
            m = re.match(r'\s+threads=(\d+)\s+([\d.]+) ms/step\s+speedup ([\d.]+)x', line)
            if m:
                t, ms, sp = int(m.group(1)), float(m.group(2)), float(m.group(3))
                if section == 'hopper':
                    hopper[t] = (ms, sp)
                elif section == 'pile':
                    pile.setdefault(model, {})[t] = (ms, sp)
                continue
            m = re.match(r'\s{2}([A-Za-z]+)\s*$', line)
            if m and section == 'pile':
                model = m.group(1)
                continue
            m = re.match(r'\s+(bf|penalty|hybrid)\s+\d+ triangles\s+([\d.]+) ms', line)
            if m and section == 'kernel':
                kernels.setdefault(m.group(1), {})['one'] = float(m.group(2))
                continue
            m = re.match(r'\s+(bf|penalty|hybrid)\s+speedup ([\d.]+)x', line)
            if m and section == 'kernel':
                kernels.setdefault(m.group(1), {})['speedup'] = float(m.group(2))
    return hopper, pile, kernels


hopper, pile, kernels = parse_bench(BENCH)
THREADS = sorted(hopper)

MODEL_ORDER = ['Sphere', 'BruteForce', 'Penalty', 'HybridOnBatches', 'HybridOnTrianglePairs']
PILE = {k: v for k, v in pile.items() if k in MODEL_ORDER}
KERNEL_ORDER = ['bf', 'penalty', 'hybrid']


# ------------------------------------------------------------------- figure 1
def figure_architecture():
    fig, ax = plt.subplots(figsize=(9.0, 4.2))
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 4.6)
    ax.axis('off')
    ax.grid(False)

    def box(x, y, w, h, title, body, colour):
        ax.add_patch(FancyBboxPatch((x, y), w, h, boxstyle='round,pad=0.06',
                                    fc=colour, ec=NAVY, lw=1.2, alpha=0.16))
        ax.text(x + w / 2, y + h - 0.22, title, ha='center', va='top',
                fontsize=9.5, fontweight='bold', color=NAVY)
        ax.text(x + w / 2, y + h - 0.55, body, ha='center', va='top',
                fontsize=8.2, color='#33475b', linespacing=1.35)

    box(0.15, 2.85, 4.5, 1.5, '1  driver — Engine::evaluateCandidates',
        'parallel for over candidate pairs\n(one result slot per pair, no locks)', '#2e86c1')
    box(0.15, 1.25, 4.5, 1.3, '2  driver — Engine::deriveForces',
        'parallel for over particles\n(partner data is a read-only snapshot)', '#2e86c1')
    box(0.15, 0.15, 4.5, 0.9, 'serial replay, in candidate order',
        'addCollision() → same map, same contact order', '#7f8c8d')

    box(5.35, 2.85, 4.5, 1.5, '3  kernels — triangles of A',
        'parallel for over A\'s triangles\n(only for meshes ≥ 512 triangles)', '#27ae60')
    box(5.35, 1.55, 4.5, 1.1, '4  kernels — triangles of B',
        'omp simd, one lane per triangle\n(declare simd, linear :3 on B)', '#27ae60')
    box(5.35, 0.15, 4.5, 1.1, '5  kernels — per-triangle slots',
        'slot[iA] written by its own iteration,\nthen swept in order', '#7f8c8d')

    for (x0, y0, x1, y1) in [(4.65, 3.6, 5.35, 3.6), (4.65, 1.9, 5.35, 2.1),
                             (4.65, 0.6, 5.35, 0.7), (5.35 + 2.25, 2.85, 5.35 + 2.25, 2.65)]:
        ax.add_patch(FancyArrowPatch((x0, y0), (x1, y1), arrowstyle='-|>',
                                     mutation_scale=13, color=NAVY, lw=1.3))

    ax.text(2.4, 4.45, 'every CollisionModel', ha='center', fontsize=9,
            style='italic', color='#5b6b79')
    ax.text(7.6, 4.45, 'each detection method', ha='center', fontsize=9,
            style='italic', color='#5b6b79')
    fig.tight_layout()
    fig.savefig(f'{R}/par_architecture.png', dpi=190, bbox_inches='tight', pad_inches=0.05)
    plt.close(fig)


# ------------------------------------------------------------------- figure 2
def figure_speedup_bars():
    labels, values, colours = [], [], []

    labels.append('hopper\n15 651 bodies')
    values.append(hopper[max(THREADS)][1])
    colours.append(NAVY)

    for m in ['BruteForce', 'Penalty', 'HybridOnBatches', 'HybridOnTrianglePairs']:
        labels.append(m.replace('HybridOn', 'Hybrid\n'))
        values.append(PILE[m][max(THREADS)][1])
        colours.append(BLUE)

    for k in KERNEL_ORDER:
        labels.append(f'kernel\n{k}')
        values.append(kernels[k]['speedup'])
        colours.append('#27ae60')

    fig, ax = plt.subplots(figsize=(9.0, 3.4))
    bars = ax.bar(range(len(values)), values, color=colours, width=0.62)
    ax.axhline(1.0, color='0.35', lw=0.8, ls='--')
    ax.axhline(max(THREADS), color=ACCENT, lw=1.0, ls=':')
    ax.text(len(values) - 0.4, max(THREADS) + 0.12, f'ideal {max(THREADS)}×',
            ha='right', fontsize=8.5, color=ACCENT)
    ax.set_xticks(range(len(values)))
    ax.set_xticklabels(labels, fontsize=8)
    ax.set_ylabel(f'speedup at {max(THREADS)} threads')
    ax.set_ylim(0, max(THREADS) + 1.2)
    for b, v in zip(bars, values):
        ax.text(b.get_x() + b.get_width() / 2, v + 0.15, f'{v:.2f}×',
                ha='center', fontsize=9, fontweight='bold')
    ax.set_title('Measured speedup, 8 threads vs 1')
    ax.grid(axis='x', visible=False)
    fig.tight_layout()
    fig.savefig(f'{R}/par_speedup.png', dpi=190, bbox_inches='tight', pad_inches=0.05)
    plt.close(fig)


# ------------------------------------------------------------------- figure 3
def figure_scaling():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(9.4, 3.6))

    ts = np.linspace(1, max(THREADS), 50)
    ax1.plot(ts, ts, ':', color='0.55', label='ideal')
    for m, style in [('BruteForce', '-o'), ('Penalty', '-s'),
                     ('HybridOnBatches', '-^'), ('HybridOnTrianglePairs', '-d')]:
        sp = [PILE[m][t][1] for t in THREADS]
        ax1.plot(THREADS, sp, style, ms=4, lw=1.4, label=m.replace('HybridOn', 'Hybrid '))
    sp = [hopper[t][1] for t in THREADS]
    ax1.plot(THREADS, sp, '-o', color=NAVY, lw=2.2, ms=5, label='hopper (Sphere)')
    ax1.set_xlabel('threads')
    ax1.set_ylabel('speedup')
    ax1.set_xticks(THREADS)
    ax1.set_title('(a) strong scaling, engine')
    ax1.legend(fontsize=7.4, loc='upper left')
    ax1.set_ylim(0, max(THREADS) + 0.6)

    # What is left of the hopper step: ideal vs measured at 8 threads.
    total = hopper[max(THREADS)][0]
    serial_fraction = (1 / hopper[max(THREADS)][1] - 1 / max(THREADS)) / (1 - 1 / max(THREADS))
    serial = serial_fraction * total
    parallel = total - serial
    ax2.barh(['1 thread'], [total], color='#aab7c4')
    ax2.barh([f'{max(THREADS)} threads'], [serial], color=ACCENT,
             label=f'serial work ({serial_fraction * 100:.0f} %)')
    ax2.barh([f'{max(THREADS)} threads'], [parallel], left=[serial], color=BLUE,
             label='parallel work')
    ax2.set_xlabel('ms per step')
    ax2.set_title('(b) where the hopper step goes')
    ax2.legend(fontsize=8, loc='lower right')
    ax2.grid(axis='y', visible=False)
    ax2.text(total * 0.99, 0, f'{total:.0f} ms', va='center', ha='right', fontsize=8.5)
    ax2.text((serial + parallel / 2), 1, f'{parallel:.0f} ms', va='center',
             ha='center', fontsize=8.5, color='white')
    ax2.text(serial / 2, 1, f'{serial:.0f} ms', va='center', ha='center',
             fontsize=8.5, color='white')

    fig.tight_layout()
    fig.savefig(f'{R}/par_scaling.png', dpi=190, bbox_inches='tight', pad_inches=0.05)
    plt.close(fig)


# ---------------------------------------------------------------------- PDF
TTF = os.path.join(matplotlib.get_data_path(), 'fonts', 'ttf')
pdfmetrics.registerFont(TTFont('DejaVu', f'{TTF}/DejaVuSans.ttf'))
pdfmetrics.registerFont(TTFont('DejaVu-Bold', f'{TTF}/DejaVuSans-Bold.ttf'))
pdfmetrics.registerFont(TTFont('DejaVu-Oblique', f'{TTF}/DejaVuSans-Oblique.ttf'))
pdfmetrics.registerFont(TTFont('DejaVu-Mono', f'{TTF}/DejaVuSansMono.ttf'))
pdfmetrics.registerFontFamily('DejaVu', normal='DejaVu', bold='DejaVu-Bold',
                              italic='DejaVu-Oblique', boldItalic='DejaVu-Bold')

styles = getSampleStyleSheet()
H1 = ParagraphStyle('H1', parent=styles['Heading1'], fontName='DejaVu-Bold',
                    fontSize=18, leading=22, spaceAfter=4, textColor=colors.HexColor('#12222f'))
SUB = ParagraphStyle('SUB', parent=styles['BodyText'], fontName='DejaVu-Oblique',
                     fontSize=11.5, leading=15, spaceAfter=14,
                     textColor=colors.HexColor('#4a5a68'))
H2 = ParagraphStyle('H2', parent=styles['Heading2'], fontName='DejaVu-Bold',
                    fontSize=13.5, leading=17, spaceBefore=13, spaceAfter=6,
                    keepWithNext=1, textColor=colors.HexColor('#12222f'))
BODY = ParagraphStyle('BODY', parent=styles['BodyText'], fontName='DejaVu',
                      fontSize=10.2, leading=15.2, alignment=TA_JUSTIFY, spaceAfter=8)
CAP = ParagraphStyle('CAP', parent=styles['BodyText'], fontName='DejaVu',
                     fontSize=8.8, leading=12, textColor=colors.HexColor('#55636f'),
                     spaceBefore=4, spaceAfter=14)
CELL = ParagraphStyle('CELL', parent=styles['BodyText'], fontName='DejaVu',
                      fontSize=8.6, leading=11.5)
CELL_H = ParagraphStyle('CELL_H', parent=CELL, fontName='DejaVu-Bold', textColor=colors.white)

ZEBRA = colors.HexColor('#f4f6f8')
GRID = colors.HexColor('#c9d2da')


def P(t, s=BODY):
    return Paragraph(t, s)


def table(rows, widths, pad=4, font=8.4, leading=11.0):
    cell = ParagraphStyle('c', parent=CELL, fontSize=font, leading=leading)
    cellh = ParagraphStyle('ch', parent=CELL_H, fontSize=font, leading=leading)
    data = [[Paragraph(c, cellh if r == 0 else cell) for c in row]
            for r, row in enumerate(rows)]
    t = Table(data, colWidths=widths, repeatRows=1)
    t.setStyle(TableStyle([
        ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor(NAVY)),
        ('ROWBACKGROUNDS', (0, 1), (-1, -1), [colors.white, ZEBRA]),
        ('GRID', (0, 0), (-1, -1), 0.4, GRID),
        ('VALIGN', (0, 0), (-1, -1), 'TOP'),
        ('LEFTPADDING', (0, 0), (-1, -1), 6),
        ('RIGHTPADDING', (0, 0), (-1, -1), 6),
        ('TOPPADDING', (0, 0), (-1, -1), pad),
        ('BOTTOMPADDING', (0, 0), (-1, -1), pad),
    ]))
    return t


def figure(path, width_cm, caption):
    from PIL import Image as PILImage
    w, h = PILImage.open(path).size
    width = width_cm * cm
    return KeepTogether([Image(path, width=width, height=width * h / w),
                         Paragraph(caption, CAP)])


def codeblock(text):
    pre = Preformatted(text, ParagraphStyle('PRE', fontName='DejaVu-Mono',
                                            fontSize=7.6, leading=10.6))
    box = Table([[pre]], colWidths=[16.4 * cm])
    box.setStyle(TableStyle([
        ('BACKGROUND', (0, 0), (-1, -1), colors.HexColor('#f6f8fa')),
        ('BOX', (0, 0), (-1, -1), 0.5, colors.HexColor('#dbe1e7')),
        ('LEFTPADDING', (0, 0), (-1, -1), 8),
        ('RIGHTPADDING', (0, 0), (-1, -1), 8),
        ('TOPPADDING', (0, 0), (-1, -1), 5),
        ('BOTTOMPADDING', (0, 0), (-1, -1), 5),
    ]))
    return KeepTogether([box])


figure_architecture()
figure_speedup_bars()
figure_scaling()

story = []
story.append(P('Multicore and SIMD Contact Kernels', H1))
story.append(P('Thesis chapters 5.4–5.8 and 8.1–8.2 in the Delta DEM solver', SUB))

top = max(THREADS)
story.append(table([
    ['', 'Before', 'After'],
    ['Contact parallelism', 'serial C++; dead <font face="DejaVu-Mono">#ifdef</font> '
     'scaffolding for OMP and ISPC that never compiled',
     f'OpenMP over pairs, particles and triangles; SIMD inner loops'],
    ['<font face="DejaVu-Mono">hopper.mbfcp</font>, 15 651 bodies',
     f'{hopper[1][0]:.0f} ms/step', f'{hopper[top][0]:.0f} ms/step ({hopper[top][1]:.2f}×)'],
    ['Mesh models, 36-box pile', f'{PILE["Penalty"][1][0]:.1f} ms/step',
     f'{PILE["Penalty"][top][0]:.1f} ms/step ({PILE["Penalty"][top][1]:.2f}×)'],
    ['Kernels, 4050-triangle pair', f'{kernels["penalty"]["one"]:.0f} ms',
     f'{kernels["penalty"]["one"] / kernels["penalty"]["speedup"]:.0f} ms '
     f'({kernels["penalty"]["speedup"]:.2f}×)'],
    ['<font face="DejaVu-Mono">ctest</font>', '9 / 9', '<b>10 / 10</b> with OpenMP on and off'],
], [4.6 * cm, 6.0 * cm, 6.2 * cm]))
story.append(Spacer(1, 10))

story.append(P('1. What the thesis asked for, and what was missing', H2))
story.append(P(
    'The thesis devotes chapter 5.4, 5.6 and 5.8 to vectorising the narrowphase and cheapening its loop '
    'footprint, and chapter 8 to concurrency: 8.1 triangle-mesh parallelisation, 8.2 particle-to-particle '
    'parallelisation, 8.3 producer–consumer grids, with algorithms 13, 15 and 16. None of it was in this '
    'solver. The mapping document says so explicitly — “No explicit SIMD; compiler auto-vectorisation only” '
    'and “out of scope for this replication” — and the code agreed: the <font face="DejaVu-Mono">OMPProcess'
    '</font> and <font face="DejaVu-Mono">OMPTriangle</font> guards around the pragmas were never defined by '
    'the build, the ISPC kernels <font face="DejaVu-Mono">bf.ispc</font> and '
    '<font face="DejaVu-Mono">penalty.ispc</font> had no CMake rule, no call site and no compiler on this '
    'machine, and the build had no <font face="DejaVu-Mono">-fopenmp</font>.'))
story.append(P(
    'So the scaffolding was dead, not dormant. This work replaces it with two real layers that can be switched '
    'off at configure time, keeping the scalar loop as the reference.'))

story.append(P('2. Two layers, because they cover different things', H2))
story.append(figure(f'{R}/par_architecture.png', 16.2,
    'Figure 1. The pair loop (1) and the force pass (2) are parallel for every collision model, because every '
    'model flows through the same candidate loop. The detection kernels add a second layer (3–5) that only '
    'pays off for meshes large enough to clear a fork/join grain.'))
story.append(P(
    'Parallelising only the kernels would have helped almost nothing in practice: the hopper’s walls are 12 '
    'triangles each, which is far below the fork/join break-even. Parallelising only the driver would have '
    'left the triangle-vs-triangle kernels serial for the big meshes that need them. Both layers exist, and '
    'each is gated.'))
story.append(table([
    ['model', 'function', 'parallel over', 'SIMD over'],
    ['<font face="DejaVu-Mono">Sphere</font>',
     '<font face="DejaVu-Mono">sphere()</font>, '
     '<font face="DejaVu-Mono">sphereWithBarrierAB/BA()</font>',
     'pairs (driver); triangles of the mesh',
     'batched pairs; one lane per triangle'],
    ['<font face="DejaVu-Mono">BruteForce</font>', '<font face="DejaVu-Mono">bf()</font>',
     'triangles of A', 'triangles of B <font face="DejaVu-Mono">(declare simd, linear :3)</font>'],
    ['<font face="DejaVu-Mono">Penalty</font>', '<font face="DejaVu-Mono">penalty()</font>',
     'triangles of A', 'triangles of B'],
    ['<font face="DejaVu-Mono">HybridOnBatches</font>',
     '<font face="DejaVu-Mono">hybridWithPerBatchFallBack()</font>', 'batches', 'batch lanes'],
    ['<font face="DejaVu-Mono">HybridOnTrianglePairs</font>',
     '<font face="DejaVu-Mono">hybridWithPerTriangleFallBack()</font>', 'triangles of A', 'triangles of B'],
], [3.6 * cm, 5.4 * cm, 3.7 * cm, 4.1 * cm], font=8.0))
story.append(Spacer(1, 8))

story.append(P('3. Measured', H2))
story.append(figure(f'{R}/par_speedup.png', 15.6,
    'Figure 2. Speedup at 8 threads. The two engine rows on the left are the real scenario and the '
    'triangle-vs-triangle models on a pile; the three on the right are a single kernel call on a '
    '4050-triangle pair, where the kernels’ own parallel region is above its grain.'))
story.append(figure(f'{R}/par_scaling.png', 16.2,
    'Figure 3. (a) Strong scaling for the engine scenarios. (b) The hopper step at 8 threads, split into the '
    'part that scales and the part that cannot: broad-phase binning, the serial merge into the collision map '
    'and <font face="DejaVu-Mono">addCollision()</font> are still one thread.'))
story.append(P(
    'The hopper stops at 2.1× because about 40 % of its step is outside the parallel regions — not because '
    'the parallel part is inefficient. Panel (b) is that split, derived from the measured scaling curve. '
    'The mesh models reach 4.8–5.6× and the kernels 3.8–6.1×, which is the shape a memory-bound kernel '
    'should have on four performance cores.'))

story.append(table([
    ['threads', 'hopper ms/step', 'speedup'] +
    [f'{m.replace("HybridOn", "Hybrid<br/>")} ms/step' for m in MODEL_ORDER if m != 'Sphere'],
    ['1'] + [f'{hopper[1][0]:.1f}', '1.00×'] +
    [f'{PILE[m][1][0]:.2f}' for m in MODEL_ORDER if m != 'Sphere'],
    [f'<b>{top}</b>', f'<b>{hopper[top][0]:.1f}</b>', f'<b>{hopper[top][1]:.2f}×</b>'] +
    [f'<b>{PILE[m][top][0]:.2f}</b>' for m in MODEL_ORDER if m != 'Sphere'],
], [1.6 * cm, 2.6 * cm, 1.7 * cm] + [2.7 * cm] * 4, font=8.2))
story.append(Spacer(1, 6))

story.append(P('4. How the result stays deterministic', H2))
story.append(P(
    'Parallelising a loop that appends to a shared <font face="DejaVu-Mono">std::vector</font> changes '
    'floating-point accumulation order, and the old code did exactly that behind <font face="DejaVu-Mono">'
    '#pragma omp critical</font>. Both layers now write one slot per unit of work and sweep the slots in the '
    'original order afterwards:'))
story.append(ListFlowable([
    ListItem(Paragraph('<b>Driver.</b> One <font face="DejaVu-Mono">ResolvedPair</font> per candidate; a '
                       'serial replay calls <font face="DejaVu-Mono">addCollision()</font> in candidate '
                       'order, so the collision map, the per-particle lists and the counters are the serial '
                       'ones. The unsupported-model throw moved into the replay, where it fires at the same '
                       'candidate index as before.', BODY), leftIndent=14),
    ListItem(Paragraph('<b>Forces.</b> Parallel over particles; partner data comes from the snapshot stored '
                       'in <font face="DejaVu-Mono">Collisions</font>, so no thread reads a velocity another '
                       'thread is writing. The max-force reduction is redone serially and is exact.', BODY),
             leftIndent=14),
    ListItem(Paragraph('<b>Kernels.</b> <font face="DejaVu-Mono">slot[iA]</font> belongs to one iteration; '
                       'the sweep afterwards reproduces <font face="DejaVu-Mono">push_back</font> order.',
                       BODY), leftIndent=14),
], bulletType='bullet', start='•', bulletFontName='DejaVu', bulletFontSize=9))
story.append(P(
    '<font face="DejaVu-Mono">tests/parallel_test.cpp</font> checks this the hard way: every model is run at '
    '1 thread and at all threads and the state must be <b>bit-identical</b>, not merely close. The scenes are '
    'sized above the grain constants (730 bodies / 2025 contacts for the sphere case, 110 contacts for the '
    'mesh models), and a second section calls the three triangle kernels directly on 1352-triangle walls so '
    'their parallel region actually executes.'))

story.append(P('5. Two bugs this work uncovered', H2))
story.append(P(
    '<b>560 KB of stack per loop iteration.</b> <font face="DejaVu-Mono">bf</font>, '
    '<font face="DejaVu-Mono">penalty</font> and <font face="DejaVu-Mono">hybrid</font> kept '
    '<font face="DejaVu-Mono">xPA[10000] … d[10000]</font> on the stack. That fits one thread, but not eight: '
    'the OpenMP thread stack is 512 KB, so the first 4050-triangle mesh at 8 threads died with a bus error. '
    'The buffers are heap now, and sized from the partner mesh instead of a hard-coded 10 000 that was never '
    'checked against it.'))
story.append(P(
    '<b>15 651 fork/joins per step.</b> <font face="DejaVu-Mono">updatePosition</font> had a bare '
    '<font face="DejaVu-Mono">#pragma omp parallel for</font> over a wall’s vertices, inside the per-particle '
    'loop. It was harmless while OpenMP was off and became 353 ms/step at 8 threads versus 2 ms at 1 once it '
    'was on — the whole driver was slower than serial. It is gated on the vertex count now, which is also why '
    'the test suite’s wall time fell from 44.9 s to 15.1 s.'))
story.append(P(
    'And one that a reviewer caught: the first version of the equivalence test had 5 bodies and 12-triangle '
    'meshes, so every grain guard was false and it compared the scalar loop with itself. It now proves the '
    'scenes clear the grains before comparing.'))

story.append(P('6. Building and checking it', H2))
story.append(codeblock('''export DEVELOPER_DIR=/Library/Developer/CommandLineTools
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release      # -DDELTA_ENABLE_OPENMP=OFF for the scalar build
cmake --build build -j 8
(cd build && ctest)                                 # 10/10 both ways
./build/parallel_bench > output/bench.log           # the numbers in this report'''))
story.append(Spacer(1, 8))
story.append(P(
    'OpenMP is optional, like VTK and assimp. <font face="DejaVu-Mono">find_package(OpenMP)</font> is tried '
    'first; on AppleClang, where CMake cannot see the MacPorts/Homebrew runtime, the build falls back to '
    '<font face="DejaVu-Mono">/opt/local/include/libomp</font> and '
    '<font face="DejaVu-Mono">libomp.dylib</font>. Without either, the build prints one status line and every '
    'pragma compiles out.'))

story.append(P('7. Still open', H2))
story.append(ListFlowable([
    ListItem(Paragraph('<b>The AMReX backend is not buildable here.</b> '
                       '<font face="DejaVu-Mono">third_party/amrex-install</font> is gone and the vendored '
                       'source tree is missing <font face="DejaVu-Mono">Tools/CMake</font>, so '
                       '<font face="DejaVu-Mono">tools/build_amrex.sh</font> fails before it configures. '
                       'Everything in this report was built with '
                       '<font face="DejaVu-Mono">-DDELTA_USE_AMREX=OFF</font>.', BODY), leftIndent=14),
    ListItem(Paragraph('<b>Kernel scratch is per-iteration heap.</b> Correct and fast enough for the measured '
                       'cases, but a thread-local block hoisted above the loop would remove ~35 k small '
                       'allocations in the worst call.', BODY), leftIndent=14),
    ListItem(Paragraph('<b>A pre-existing bound bug in '
                       '<font face="DejaVu-Mono">hybridWithPerTriangleFallBack</font></b> walks A’s triangles '
                       'using B’s count: it reads past A when |B| &gt; |A| and skips contacts when '
                       '|A| &gt; |B|. Flagged with a <font face="DejaVu-Mono">shortcut:</font> comment rather '
                       'than silently fixed, because correcting it changes results.', BODY), leftIndent=14),
], bulletType='bullet', start='•', bulletFontName='DejaVu', bulletFontSize=9))


class Doc(BaseDocTemplate):
    def __init__(self, filename, **kw):
        BaseDocTemplate.__init__(self, filename, **kw)
        frame = Frame(self.leftMargin, self.bottomMargin, self.width, self.height, id='body')
        self.addPageTemplates([PageTemplate(id='main', frames=[frame], onPage=self.footer)])

    def footer(self, canvas, doc):
        canvas.saveState()
        canvas.setFont('DejaVu', 8)
        canvas.setFillColor(colors.HexColor('#8b98a5'))
        canvas.drawString(doc.leftMargin, 1.15 * cm, 'Delta — parallel and SIMD contact kernels')
        canvas.drawRightString(A4[0] - doc.rightMargin, 1.15 * cm, f'{doc.page}')
        canvas.setStrokeColor(colors.HexColor('#dde3e8'))
        canvas.setLineWidth(0.5)
        canvas.line(doc.leftMargin, 1.55 * cm, A4[0] - doc.rightMargin, 1.55 * cm)
        canvas.restoreState()


doc = Doc(OUT, pagesize=A4, leftMargin=2.1 * cm, rightMargin=2.1 * cm,
          topMargin=1.9 * cm, bottomMargin=2.0 * cm,
          title='Multicore and SIMD Contact Kernels', author='Delta')
doc.build(story)
print('PDF written:', OUT)
