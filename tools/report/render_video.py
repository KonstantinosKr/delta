"""Render the hopper_fall scenario frames into an mp4.

Reads <DELTA_REPORT_DIR>/video_{particles,walls}.csv (written by hf_video.cpp) and
draws an orthographic view with a painter's algorithm: walls and spheres are
sorted back-to-front so transparency looks right without a 3D engine.
"""
import csv
import os
from collections import defaultdict
from multiprocessing import Pool

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.cm import ScalarMappable
from matplotlib.collections import PolyCollection
from matplotlib.colors import Normalize
from matplotlib.patches import Circle

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
R = os.environ.get('DELTA_REPORT_DIR', os.path.join(ROOT, 'output', 'report'))
os.makedirs(R, exist_ok=True)
FRAMES = f'{R}/frames'
os.makedirs(FRAMES, exist_ok=True)

AZ = np.radians(18.0)
EL = np.radians(10.0)
FIG_H = 10.0            # inches
RADIUS = 0.04
BG = '#f8f9fb'

# turbo at this range puts a resting sphere (~0 m/s) in blue, a free-falling one
# (~2.5 m/s) in orange-red.
NORM = Normalize(vmin=-0.9, vmax=3.5)
CMAP = plt.get_cmap('turbo')


def load():
    tris = []
    with open(f'{R}/video_walls.csv') as f:
        for row in csv.DictReader(f):
            tris.append([float(row['x']), float(row['y']), float(row['z'])])
    tris = np.array(tris, dtype=float).reshape(-1, 3, 3)

    frames = defaultdict(list)
    with open(f'{R}/video_particles.csv') as f:
        for row in csv.DictReader(f):
            frames[int(row['frame'])].append(
                (float(row['time']), int(row['id']),
                 float(row['x']), float(row['y']), float(row['z']),
                 float(row['speed'])))
    return tris, frames


def rotation():
    cz, sz = np.cos(AZ), np.sin(AZ)
    cx, sx = np.cos(EL), np.sin(EL)
    rz = np.array([[cz, -sz, 0.0], [sz, cz, 0.0], [0.0, 0.0, 1.0]])
    rx = np.array([[1.0, 0.0, 0.0], [0.0, cx, -sx], [0.0, sx, cx]])
    return rx @ rz


ROT = rotation()
# Camera sits along +eye in world space. Hopper faces are wound so their normals
# point INTO the funnel, so a face with normal dot eye > 0 is seen from inside:
# draw those and cull the rest. That is the standard cutaway view of a container.
EYE = ROT.T @ np.array([0.0, 0.0, 1.0])


def face_normal(tri):
    u = tri[1] - tri[0]
    v = tri[2] - tri[0]
    n = np.cross(u, v)
    ln = np.linalg.norm(n)
    return n / ln if ln > 0 else None


def project(points):
    q = np.asarray(points, dtype=float) @ ROT.T
    return np.stack([q[:, 0], q[:, 1]], axis=1), q[:, 2]


def world_bounds(tris, frames):
    pts = [tris.reshape(-1, 3)]
    for rows in frames.values():
        pts.append(np.array([[r[2], r[3], r[4]] for r in rows]))
    xy, _ = project(np.vstack(pts))
    return xy[:, 0].min(), xy[:, 0].max(), xy[:, 1].min(), xy[:, 1].max()


DATA = None


def init(shared):
    global DATA
    DATA = shared


def render(frame):
    tris, frames, xlim, ylim, size = DATA
    rows = frames[frame]

    fig = plt.figure(figsize=size, dpi=100)
    ax = fig.add_axes([0, 0, 1, 1])
    ax.set_xlim(*xlim)
    ax.set_ylim(*ylim)
    ax.set_aspect('equal')
    ax.axis('off')
    fig.patch.set_facecolor(BG)
    ax.set_facecolor(BG)

    # ---- walls ---------------------------------------------------------
    polys, facecolors, depths = [], [], []
    for tri in tris:
        n = face_normal(tri)
        if n is None or float(n @ EYE) <= 0.0:
            continue                       # near-side wall: cut away
        xy, d = project(tri)
        d = float(d.mean())
        t = 0.5 + 0.5 * np.tanh(d * 5.0)
        polys.append(xy)
        facecolors.append((0.60 - 0.10 * t, 0.65 - 0.10 * t, 0.72 - 0.08 * t, 0.22))
        depths.append(d)

    order = np.argsort(depths)
    ax.add_collection(PolyCollection(
        [polys[i] for i in order],
        facecolors=[facecolors[i] for i in order],
        edgecolors=[(0.22, 0.27, 0.33, 0.55)] * len(order),
        linewidths=0.8, zorder=1))

    # ---- spheres -------------------------------------------------------
    spheres = []
    for (_, pid, x, y, z, speed) in rows:
        xy, d = project(np.array([[x, y, z]]))
        spheres.append((float(d[0]), xy[0][0], xy[0][1], speed))
    spheres.sort()

    for _, sx, sy, speed in spheres:
        ax.add_patch(Circle((sx, sy), RADIUS, facecolor=CMAP(NORM(speed)),
                            edgecolor='white', linewidth=1.1, alpha=0.97, zorder=2))

    # ---- overlays ------------------------------------------------------
    t = rows[0][0] if rows else 0.0
    box = dict(boxstyle='round,pad=0.5', facecolor='white', edgecolor='none', alpha=0.80)
    ax.text(0.012, 0.988, 'Delta \u2014 hopper_fall: 10 spheres dropped into a funnel',
            transform=ax.transAxes, fontsize=16, color='#1d2a35',
            va='top', ha='left', bbox=box, zorder=5)
    ax.text(0.012, 0.938, f't = {t:.3f} s      frame {frame:03d} / 599',
            transform=ax.transAxes, fontsize=13, color='#4a5a68',
            va='top', ha='left', bbox=box, zorder=5)
    ax.text(0.988, 0.040, 'g = (0, \u22129.81, 0) m/s\u00b2     dt = 1e\u207b\u2075 s',
            transform=ax.transAxes, fontsize=10, color='#7b8896',
            va='bottom', ha='right', zorder=5)

    cax = fig.add_axes([0.035, 0.055, 0.26, 0.013])
    cb = fig.colorbar(ScalarMappable(norm=NORM, cmap=CMAP), cax=cax,
                      orientation='horizontal')
    cb.set_label('|v|  (m/s)', fontsize=9, color='#5b6b79', labelpad=2)
    cb.ax.tick_params(labelsize=8, colors='#5b6b79', length=2, pad=1)
    cb.outline.set_visible(False)

    path = f'{FRAMES}/frame_{frame:04d}.png'
    fig.savefig(path, facecolor=BG)
    plt.close(fig)
    return path


def add_colourbar_strip():
    """One static colour bar PNG, overlaid on every frame by ffmpeg."""
    fig = plt.figure(figsize=(4.0, 0.55), dpi=100)
    cax = fig.add_axes([0.02, 0.45, 0.96, 0.4])
    cb = fig.colorbar(ScalarMappable(norm=NORM, cmap=CMAP), cax=cax,
                      orientation='horizontal')
    cb.set_label('sphere speed  |v|  (m/s)', fontsize=9, color='#4a5a68')
    cb.ax.tick_params(labelsize=8, colors='#4a5a68', length=2)
    cb.outline.set_visible(False)
    fig.patch.set_facecolor(BG)
    fig.savefig(f'{R}/colorbar.png', facecolor=BG, transparent=False)
    plt.close(fig)


if __name__ == '__main__':
    tris, frames = load()
    x0, x1, y0, y1 = world_bounds(tris, frames)

    # pad, and reserve headroom for the two text lines
    padx, pady = 0.05, 0.04
    x0 -= padx; x1 += padx; y0 -= pady; y1 += pady
    y1 += 0.09

    w, h = x1 - x0, y1 - y0
    fig_w = FIG_H * w / h
    size = (fig_w, FIG_H)

    DATA = (tris, frames, (x0, x1), (y0, y1), size)
    print(f'{len(frames)} frames, window x[{x0:.2f},{x1:.2f}] y[{y0:.2f},{y1:.2f}]')
    with Pool(8, initializer=init, initargs=(DATA,)) as pool:
        for i, _ in enumerate(pool.imap_unordered(render, sorted(frames), chunksize=8)):
            if i % 200 == 0:
                print(f'  {i}/{len(frames)}', flush=True)
    add_colourbar_strip()
    print('frames done')
