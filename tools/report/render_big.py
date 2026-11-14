"""Render the 15 651-body hopper run (hf_big) into an mp4.

Reads <DELTA_REPORT_DIR>/big_particles.csv and big_walls.csv. Frames are drawn
with a single scatter call per frame (15 625 markers), ordered back to front, so
a frame costs tens of milliseconds instead of seconds.
"""
import glob
import os
from multiprocessing import Pool

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.cm import ScalarMappable
from matplotlib.collections import PolyCollection
from matplotlib.colors import Normalize

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
R = os.environ.get('DELTA_REPORT_DIR', os.path.join(ROOT, 'output', 'big'))
FRAMES = os.path.join(R, 'frames_big')
os.makedirs(FRAMES, exist_ok=True)

AZ = np.radians(22.0)
EL = np.radians(8.0)
FIG_H = 10.0
BG = '#f8f9fb'
RADIUS = 0.02

Z_NORM = Normalize(vmin=0.0, vmax=1.0)   # world height of the bed
CMAP = plt.get_cmap('viridis')
MARKER_SCALE = 0.55                      # fraction of the true sphere radius


def load_walls():
    tris = []
    with open(os.path.join(R, 'big_walls.csv')) as f:
        for line in f:
            tris.append([float(x) for x in line.split(',')])
    tris = np.array(tris, dtype=float).reshape(-1, 3, 3)
    # Keep the container around the bed: the four side walls and the four
    # funnel ramps. The file also carries four far outer slabs and a catch floor
    # 2 m down; they only add lines across the picture.
    keep = []
    for t in tris:
        if t[:, 2].max() < -1.2:
            continue
        if t[:, 0].max() > 1.2 or t[:, 1].max() > 1.2:
            continue
        if t[:, 0].min() < -0.2 or t[:, 1].min() < -0.2:
            continue
        keep.append(t)
    return np.array(keep)


def load_frames():
    raw, times, contacts = [], [], []
    pts = None
    with open(os.path.join(R, 'big_particles.csv')) as f:
        for line in f:
            if line.startswith('#'):
                parts = line.split()
                if len(parts) == 4:
                    if pts is not None:
                        raw.append(pts)
                    pts = []
                    times.append(float(parts[2]))
                    contacts.append(int(parts[3]))
                continue
            pts.append([float(x) for x in line.split(',')])
    if pts:
        raw.append(pts)

    # A run that is still writing leaves a short final block; drop incomplete
    # frames rather than failing on a ragged array.
    n = max((len(p) for p in raw), default=0)
    keep = [i for i, p in enumerate(raw) if len(p) == n and n > 0]
    if len(keep) != len(raw):
        print(f'ignoring {len(raw) - len(keep)} incomplete frame(s)')
    return (np.array([raw[i] for i in keep]),
            np.array([times[i] for i in keep]),
            np.array([contacts[i] for i in keep]))


def rotation():
    """World Z is up here (gravity is -z), unlike the funnel test. Rows are
    (screen x, screen y, depth): azimuth spins about the vertical, elevation
    tilts the camera down onto the scene."""
    ca, sa = np.cos(AZ), np.sin(AZ)
    ce, se = np.cos(EL), np.sin(EL)
    return np.array([[ca, -sa, 0.0],
                     [-sa * se, -ca * se, ce],
                     [sa * ce, ca * ce, se]])


ROT = rotation()
EYE = ROT.T @ np.array([0.0, 0.0, 1.0])
DATA = None


def project(points):
    q = np.asarray(points, dtype=float) @ ROT.T
    return np.stack([q[:, 0], q[:, 1]], axis=1), q[:, 2]


def init(shared):
    global DATA
    DATA = shared


def render(i):
    walls, frames, times, contacts, lim, size, marker = DATA
    pts = frames[i]
    speed = np.zeros(len(pts))
    if i > 0:
        speed = np.linalg.norm(pts - frames[i - 1], axis=1) / max(times[i] - times[i - 1], 1e-12)
    maxspeed = float(speed.max())

    xy, depth = project(pts)
    order = np.argsort(depth)

    fig = plt.figure(figsize=size, dpi=100)
    ax = fig.add_axes([0, 0, 1, 1])
    ax.set_xlim(*lim[0]); ax.set_ylim(*lim[1])
    ax.set_aspect('equal'); ax.axis('off')
    fig.patch.set_facecolor(BG); ax.set_facecolor(BG)

    # container: only faces seen from inside (normals are wound inwards)
    polys, shades, pdepth = [], [], []
    for tri in walls:
        u, v = tri[1] - tri[0], tri[2] - tri[0]
        n = np.cross(u, v)
        ln = np.linalg.norm(n)
        if ln <= 0:
            continue
        n /= ln
        if float(n @ EYE) <= 0.0:
            continue
        p, d = project(tri)
        polys.append(p)
        shades.append(0.5 + 0.5 * float(np.tanh(d.mean() * 3.0)))
        pdepth.append(float(d.mean()))
    if polys:
        o = np.argsort(pdepth)
        ax.add_collection(PolyCollection(
            [polys[k] for k in o],
            facecolors=[(0.55, 0.60, 0.67, 0.10) for _ in o],
            edgecolors=[(0.20, 0.25, 0.32, 0.35)] * len(o),
            linewidths=0.35, zorder=1))

    ax.scatter(xy[order, 0], xy[order, 1], s=marker, c=pts[order, 2],
               cmap=CMAP, norm=Z_NORM, linewidths=0, rasterized=True, zorder=2)

    box = dict(boxstyle='round,pad=0.5', facecolor='white', edgecolor='none', alpha=0.82)
    ax.text(0.012, 0.988, 'Delta \u2014 hopper.mbfcp: 15 651 bodies (15 625 spheres)',
            transform=ax.transAxes, fontsize=15, color='#1d2a35', va='top', bbox=box, zorder=5)
    ax.text(0.012, 0.941,
            f't = {times[i]:.4f} s     frame {i + 1}/{len(frames)}     '
            f'contacts {contacts[i]}     max|v| = {maxspeed:.3f} m/s',
            transform=ax.transAxes, fontsize=11.5, color='#4a5a68', va='top',
            bbox=box, zorder=5)

    cax = fig.add_axes([0.035, 0.055, 0.24, 0.012])
    cb = fig.colorbar(ScalarMappable(norm=Z_NORM, cmap=CMAP), cax=cax,
                      orientation='horizontal')
    cb.set_label('sphere height  z  (m)', fontsize=8.5, color='#5b6b79', labelpad=2)
    cb.ax.tick_params(labelsize=7.5, colors='#5b6b79', length=2, pad=1)
    cb.outline.set_visible(False)

    path = f'{FRAMES}/frame_{i:04d}.png'
    fig.savefig(path, facecolor=BG)
    plt.close(fig)
    return path


if __name__ == '__main__':
    walls = load_walls()
    frames, times, contacts = load_frames()
    print(f'{len(frames)} frames, {frames.shape[1]} spheres, '
          f't = {times[0]:.4g} .. {times[-1]:.4g} s')

    allpts = np.vstack([frames[0], frames[-1], walls.reshape(-1, 3)])
    xy, _ = project(allpts)
    x0, x1 = xy[:, 0].min() - 0.03, xy[:, 0].max() + 0.03
    y0, y1 = xy[:, 1].min() - 0.03, xy[:, 1].max() + 0.03
    y1 += 0.12
    w, h = x1 - x0, y1 - y0
    size = (FIG_H * w / h, FIG_H)

    # One screen unit is FIG_H*100/h pixels; a point is 1/72 in = 100/72 px at
    # dpi 100, so a sphere of radius RADIUS spans this many points.
    px_per_unit = FIG_H * 100.0 / h
    r_pt = RADIUS * MARKER_SCALE * px_per_unit * 72.0 / 100.0
    marker = (2.0 * r_pt) ** 2

    DATA = (walls, frames, times, contacts, ((x0, x1), (y0, y1)), size, marker)
    with Pool(8, initializer=init, initargs=(DATA,)) as pool:
        for k, _ in enumerate(pool.imap_unordered(render, range(len(frames)), chunksize=4)):
            if k % 50 == 0:
                print(f'  {k}/{len(frames)}', flush=True)
    print('frames done')
