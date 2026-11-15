"""Render the granulate hopper run (hf_granulate) into an mp4.

Each body arrives as a list of world-space triangles per frame, so this is a
painter's-algorithm renderer over polygons: no 3D engine, correct transparency
and correct occlusion as long as the sort is by mean camera depth.
"""
import csv
import os
from multiprocessing import Pool

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.collections import PolyCollection

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
R = os.environ.get('DELTA_REPORT_DIR', os.path.join(ROOT, 'output', 'granulate'))
FRAMES = os.path.join(R, 'frames_gran')
os.makedirs(FRAMES, exist_ok=True)

AZ = np.radians(24.0)
EL = np.radians(12.0)
FIG_H = 9.0
BG = '#f8f9fb'
LIGHT = np.array([-0.4, 0.75, 0.5])
LIGHT = LIGHT / np.linalg.norm(LIGHT)

BODY_COLOURS = ['#2e86c1', '#27ae60', '#8e44ad', '#d35400', '#16a085',
                '#c0392b', '#2c3e50', '#f39c12', '#7f8c8d', '#00838f']


def rotation():
    """World Y is up in this scene (gravity is -y)."""
    ca, sa = np.cos(AZ), np.sin(AZ)
    ce, se = np.cos(EL), np.sin(EL)
    rz = np.array([[ca, -sa, 0.0], [sa, ca, 0.0], [0.0, 0.0, 1.0]])
    rx = np.array([[1.0, 0.0, 0.0], [0.0, ce, se], [0.0, -se, ce]])
    return rz @ rx.T


ROT = rotation()


def project(points):
    q = np.asarray(points, dtype=float) @ ROT.T
    return np.stack([q[:, 0], q[:, 1]], axis=1), q[:, 2]


def load():
    walls = []
    with open(os.path.join(R, 'gran_walls.csv')) as f:
        next(f)                                    # x,y,z header
        for line in f:
            if not line.strip():
                continue
            walls.append([float(x) for x in line.split(',')])
    walls = np.array(walls, dtype=float).reshape(-1, 3, 3)

    frames = []
    with open(os.path.join(R, 'gran_frames.csv')) as f:
        cur = None
        for line in f:
            if line.startswith('#'):
                parts = line.split()
                if len(parts) >= 5 and parts[1] == 'frame':
                    cur = {'time': float(parts[3]), 'contacts': int(parts[4]), 'bodies': []}
                    frames.append(cur)
                continue
            if line.startswith('body'):
                _, bid, ntri = line.split()
                cur['bodies'].append({'id': int(bid), 'ntri': int(ntri), 'tri': []})
                continue
            if cur is None or not cur['bodies']:
                continue
            cur['bodies'][-1]['tri'].append([float(x) for x in line.split(',')])
    for fr in frames:
        for b in fr['bodies']:
            b['tri'] = np.array(b['tri'], dtype=float).reshape(-1, 3, 3)
    return walls, frames


DATA = None


def init(shared):
    global DATA
    DATA = shared


def render(i):
    walls, frames, lim, size = DATA
    fr = frames[i]

    fig = plt.figure(figsize=size, dpi=100)
    ax = fig.add_axes([0, 0, 1, 1])
    ax.set_xlim(*lim[0]); ax.set_ylim(*lim[1])
    ax.set_aspect('equal'); ax.axis('off')
    fig.patch.set_facecolor(BG); ax.set_facecolor(BG)

    polys, colours, depths = [], [], []

    for tri in walls:
        xy, d = project(tri)
        polys.append(xy)
        colours.append((0.55, 0.60, 0.66, 0.16))
        depths.append(float(d.mean()))
    wall_count = len(polys)

    for b in fr['bodies']:
        base = BODY_COLOURS[(b['id'] - 1) % len(BODY_COLOURS)]
        rgb = matplotlib.colors.to_rgb(base)
        for tri in b['tri']:
            u, v = tri[1] - tri[0], tri[2] - tri[0]
            n = np.cross(u, v)
            ln = np.linalg.norm(n)
            if ln <= 0:
                continue
            n /= ln
            shade = 0.45 + 0.55 * abs(float(n @ LIGHT))
            xy, d = project(tri)
            polys.append(xy)
            colours.append((rgb[0] * shade, rgb[1] * shade, rgb[2] * shade, 1.0))
            depths.append(float(d.mean()))

    order = np.argsort(depths)
    # walls first (they are the far half of the scene), then bodies back-to-front
    wall_idx = [k for k in order if k < wall_count]
    body_idx = [k for k in order if k >= wall_count]
    if wall_idx:
        ax.add_collection(PolyCollection([polys[k] for k in wall_idx],
                                         facecolors=[colours[k] for k in wall_idx],
                                         edgecolors=[(0.25, 0.30, 0.36, 0.45)] * len(wall_idx),
                                         linewidths=0.6, zorder=1))
    if body_idx:
        ax.add_collection(PolyCollection([polys[k] for k in body_idx],
                                         facecolors=[colours[k] for k in body_idx],
                                         edgecolors=[(0.15, 0.18, 0.22, 0.55)] * len(body_idx),
                                         linewidths=0.4, zorder=2))

    box = dict(boxstyle='round,pad=0.5', facecolor='white', edgecolor='none', alpha=0.82)
    ax.text(0.012, 0.988, 'Delta \u2014 10 granulates dropping into the hopper',
            transform=ax.transAxes, fontsize=15, color='#1d2a35', va='top',
            bbox=box, zorder=5)
    ax.text(0.012, 0.938,
            f't = {fr["time"]:.4f} s     frame {i + 1}/{len(frames)}     '
            f'contacts {fr["contacts"]}',
            transform=ax.transAxes, fontsize=11.5, color='#4a5a68', va='top',
            bbox=box, zorder=5)
    ax.text(0.985, 0.02, 'convex hulls, 24 points each   |   BruteForce contact model',
            transform=ax.transAxes, fontsize=9.5, color='#8b98a5', ha='right')

    path = f'{FRAMES}/frame_{i:04d}.png'
    fig.savefig(path, facecolor=BG)
    plt.close(fig)
    return path


if __name__ == '__main__':
    walls, frames = load()
    print(f'{len(frames)} frames, {len(frames[0]["bodies"])} bodies')

    pts = [walls.reshape(-1, 3)]
    for fr in (frames[0], frames[-1]):
        for b in fr['bodies']:
            pts.append(b['tri'].reshape(-1, 3))
    xy, _ = project(np.vstack(pts))
    x0, x1 = xy[:, 0].min() - 0.04, xy[:, 0].max() + 0.04
    y0, y1 = xy[:, 1].min() - 0.04, xy[:, 1].max() + 0.04
    y1 += 0.10
    w, h = x1 - x0, y1 - y0
    size = (FIG_H * w / h, FIG_H)

    DATA = (walls, frames, ((x0, x1), (y0, y1)), size)
    with Pool(8, initializer=init, initargs=(DATA,)) as pool:
        for k, _ in enumerate(pool.imap_unordered(render, range(len(frames)), chunksize=4)):
            if k % 50 == 0:
                print(f'  {k}/{len(frames)}', flush=True)
    print('frames done')
