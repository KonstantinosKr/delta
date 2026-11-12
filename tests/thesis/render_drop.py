#!/usr/bin/env python3
"""Render the drop-test OBJ frames into PNGs, with the trajectory traced.

The point of the picture is the decay of the rebound: each bounce should be
visibly lower than the last, and the contact should look short.
"""
import os
import re
import sys
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


def parse(path):
    """Return (floor_points, ball_centre_and_radius).

    The driver writes floor vertices and their `l` lines first, then the `g
    ball` marker followed by the disk's vertices, so group membership has to be
    taken from the line ranges rather than from a running label.
    """
    verts, groups, cur = [], [], "default"
    with open(path) as fh:
        for line in fh:
            if line.startswith("g "):
                cur = line[2:].strip()
            elif line.startswith("v "):
                verts.append([float(x) for x in line.split()[1:4]])
                groups.append(cur)
    V = np.array(verts) if verts else np.zeros((0, 3))
    if len(V) == 0:
        return [], None

    # floor vertices were emitted before the first "g ball"; the rest are the disk
    ball_start = next((i for i, g in enumerate(groups) if g == "ball"), len(V))
    floor = V[:ball_start]
    ball_pts = V[ball_start:]
    ball = None
    if len(ball_pts) > 2:
        c = ball_pts.mean(axis=0)
        ball = (c, float(np.linalg.norm(ball_pts - c, axis=1).max()))
    return floor, ball


def main():
    srcdir, outdir = sys.argv[1], sys.argv[2]
    os.makedirs(outdir, exist_ok=True)
    files = sorted(f for f in os.listdir(srcdir) if f.endswith(".obj"))

    # pass 1: collect the centre trajectory so it can be drawn as a tail
    traj = []
    for f in files:
        _, ball = parse(os.path.join(srcdir, f))
        if ball:
            traj.append(ball[0][:2])
    traj = np.array(traj)

    for i, f in enumerate(files):
        floor, ball = parse(os.path.join(srcdir, f))

        fig, ax = plt.subplots(figsize=(7, 7), dpi=110)

        # trajectory so far
        if i > 1:
            ax.plot(traj[:i, 0], traj[:i, 1], color="#4a90d9", lw=1.0, alpha=0.55,
                    label="centre path")

        # floor
        ax.axhline(0.0, color="#333333", lw=3.0, solid_capstyle="butt")
        ax.axhspan(-0.35, 0.0, color="#cccccc", alpha=0.5)

        if ball:
            c, r = ball
            ax.add_patch(plt.Circle((c[0], c[1]), r, color="#d94f3d",
                                    zorder=5, alpha=0.95))
            ax.plot([c[0]], [c[1]], marker="+", color="white", ms=6, zorder=6)

        ax.set_xlim(-0.45, 0.45)
        ax.set_ylim(-0.12, 0.72)
        ax.set_aspect("equal")
        ax.set_xlabel("x  [m]")
        ax.set_ylabel("y  [m]")
        ax.grid(alpha=0.25)
        if i > 1:
            ax.legend(loc="upper right", fontsize=8)
        ax.set_title(
            "Delta 2D - disk dropped on a static floor, zeta=0.3\n"
            f"frame {i+1}/{len(files)}   height {ball[0][1] if ball else 0:.4f} m",
            fontsize=10)
        fig.tight_layout()
        fig.savefig(os.path.join(outdir, f"f{i:04d}.png"))
        plt.close(fig)

    print(f"wrote {len(files)} frames to {outdir}")


if __name__ == "__main__":
    main()
