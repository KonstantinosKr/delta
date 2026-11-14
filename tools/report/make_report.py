"""Plots for the hopper bug report (docs/hopper-bug-report.pdf).

Reads output/report/*.csv (written by tools/report/hf_video.cpp and
render_video.py) and writes the figure PNGs next to them.

    cmake --build build -j
    ./build/hf_video output/report       # or the standalone build in tools/report
    python3 tools/report/make_report.py
    python3 tools/report/make_pdf.py
"""
import csv
import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import Circle, FancyArrowPatch, Polygon

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
R = os.environ.get('DELTA_REPORT_DIR', os.path.join(ROOT, 'output', 'report'))
os.makedirs(R, exist_ok=True)

plt.rcParams.update({'font.size': 11, 'font.family': 'DejaVu Sans',
                     'axes.grid': True, 'grid.alpha': 0.3,
                     'axes.spines.top': False, 'axes.spines.right': False,
                     'mathtext.fontset': 'dejavusans'})

# ---------------------------------------------------------------- figure 1
# Two panels, one for the mesh gap and one for the sphere-sphere overlap.
eps = 0.004
gap = np.linspace(-0.010, 0.010, 400)

fig, (ax, ax2) = plt.subplots(1, 2, figsize=(9.2, 3.5))

ax.axvspan(-0.010, eps, color='tab:green', alpha=0.08,
           label='detected (gap $\\leq \\varepsilon$)')
ax.plot(gap, -gap, color='tab:red', lw=2.0, label='old: depth $=-$gap')
ax.plot(gap, np.maximum(0.0, -gap), color='tab:blue', lw=2.0,
        label='new: depth $=\\max(0,-$gap$)$')
ax.axvline(0, color='k', lw=0.7, ls='--')
ax.axhline(0, color='k', lw=0.7, ls='--')
ax.set_xticks(np.arange(-0.010, 0.0101, 0.005))
ax.set_xlabel('signed gap = planeDist $-$ radius  (m)')
ax.set_ylabel('spring depth  (m)')
ax.set_ylim(-0.0115, 0.0115)
ax.set_title('(a) sphere vs mesh wall', fontsize=10.5)
ax.annotate('negative depth:\nspring pulls the sphere\nINTO the wall',
            xy=(-0.0062, 0.0062), xytext=(-0.0097, -0.0092), color='tab:red',
            fontsize=8.5, va='bottom',
            arrowprops=dict(arrowstyle='->', color='tab:red'))
ax.legend(loc='upper right', fontsize=7.6, framealpha=0.95)

overlap = np.linspace(0, eps, 300)
ax2.plot(overlap, eps - overlap, color='tab:red', lw=2.0,
         label='old: depth $=\\varepsilon_{tot}-$overlap')
ax2.plot(overlap, overlap, color='tab:blue', lw=2.0, label='new: depth = overlap')
ax2.set_xticks(np.arange(0, 0.0041, 0.001))
ax2.set_xlabel('true overlap  $2r-d$  (m)')
ax2.set_ylabel('spring depth  (m)')
ax2.set_ylim(-0.0004, 0.0048)
ax2.set_title('(b) sphere vs sphere', fontsize=10.5)
ax2.annotate('force vanishes at the\nmaximum accepted overlap',
             xy=(0.00385, 0.00012), xytext=(0.00055, 0.00055), color='tab:red',
             fontsize=8.5, va='bottom',
             arrowprops=dict(arrowstyle='->', color='tab:red'))
ax2.legend(loc='upper left', fontsize=7.6, framealpha=0.95)

fig.tight_layout()
fig.savefig(f'{R}/depth_gap.png', dpi=190, bbox_inches='tight', pad_inches=0.06)
plt.close(fig)

# ---------------------------------------------------------------- figure 2
fig, ax = plt.subplots(figsize=(6.8, 3.7))

r_s = 0.16
C = np.array([-0.12, 0.20])
E = np.array([0.0, 0.0])
edge_x = 1.2

ax.add_patch(Polygon([[0, 0], [edge_x, 0], [edge_x, -0.20], [0, -0.20]],
                     closed=True, fc='#cfd6dd', ec='0.25', lw=1.5, zorder=2))
ax.text(edge_x/2, -0.10, 'wall face (one triangle)', ha='center', va='center',
        fontsize=9.5, color='0.2', zorder=3)

ax.plot([-1.25, 1.85], [0, 0], ls='--', lw=1.6, color='tab:red', zorder=1)
ax.text(1.00, 0.06, 'infinite plane (old test)', color='tab:red', fontsize=9.5,
        ha='center', va='bottom')

ax.add_patch(Circle(C, r_s, fc='#4c8fd6', ec='0.15', lw=1.4, alpha=0.95, zorder=4))
ax.plot([C[0]], [C[1]], 'k.', ms=4, zorder=6)

ax.add_patch(FancyArrowPatch((-0.40, C[1]), (-0.40, 0.0),
                             arrowstyle='<->', color='tab:red', lw=1.8,
                             mutation_scale=11, zorder=5))
ax.text(-0.45, 0.10, '4 mm\n$<\\varepsilon$', color='tab:red', fontsize=9.5,
        ha='right', va='center')

ax.plot([E[0]], [E[1]], 'o', color='tab:green', ms=7, zorder=7)
ax.plot([C[0], E[0]], [C[1], E[1]], color='tab:green', lw=2.0, zorder=5)
ax.text(-0.275, 0.235, '22 mm\n$>\\varepsilon$', color='tab:green', fontsize=9.5,
        ha='right', va='bottom', rotation=58)

ax.text(-1.22, 0.76, 'old test: measures to the plane \u2192 contact,\n'
                     'pushes the sphere along the plane normal',
        color='tab:red', fontsize=9.5, va='top')
ax.text(-1.22, -0.42, 'new test: measures to the finite triangle \u2192\n'
                      'nearest point is the edge corner, no contact',
        color='tab:green', fontsize=9.5, va='top')

ax.set_xlim(-1.28, 1.92)
ax.set_ylim(-0.55, 0.80)
ax.set_aspect('equal')
ax.axis('off')
ax.grid(False)
ax.set_title('Bug 2: a plane-only test turns a finite wall into an infinite plane',
             fontsize=11.5, pad=6)
fig.tight_layout()
fig.savefig(f'{R}/finite_triangle.png', dpi=170, bbox_inches='tight', pad_inches=0.06)
plt.close(fig)

# ---------------------------------------------------------------- figure 4
before_steps = [0, 14300, 14400, 14450, 14500, 14550, 14600, 14650,
                14700, 14750, 14800, 14918]
before_z = [0.0, 0.0, 0.000022, 0.004165, 0.030983, 0.062835, 0.069384,
            0.069044, 0.068720, 0.063628, 0.012628, -0.253]
before_t = np.array(before_steps) * 1e-5

after_t, after_z = [], []
with open(f'{R}/hopper_after.csv') as f:
    for row in csv.DictReader(f):
        after_t.append(int(row['step']) * 1e-5)
        after_z.append(float(row['z']))

fig, ax = plt.subplots(figsize=(6.8, 4.3))
ax.plot(after_t, after_z, color='tab:blue', lw=1.4,
        label='after fix: body 1 stays in the z = 0 plane')
ax.plot(before_t, before_z, marker='o', ms=4, color='tab:red', lw=1.6,
        label='before fix: body 1 is ejected through the back wall')
ax.axhline(0.10, color='k', ls=':', lw=1)
ax.axhline(-0.10, color='k', ls=':', lw=1)
ax.text(0.62, 0.108, 'front wall  z = +0.10', fontsize=9, color='0.25')
ax.text(0.62, -0.152, 'back wall  z = −0.10', fontsize=9, color='0.25')
ax.annotate('escape', xy=(0.1492, -0.253), xytext=(0.20, -0.24),
            color='tab:red', fontsize=10,
            arrowprops=dict(arrowstyle='->', color='tab:red'))
ax.set_xlabel('time (s)')
ax.set_ylabel('z of body 1  (m)')
ax.set_title('hopper_fall_test: body 1 across the walls')
ax.set_ylim(-0.30, 0.30)
ax.set_xlim(-0.02, 1.22)
ax.legend(loc='upper right', fontsize=9, framealpha=0.95)
fig.tight_layout()
fig.savefig(f'{R}/trajectory.png', dpi=170, bbox_inches='tight', pad_inches=0.06)
plt.close(fig)

# ---------------------------------------------------------------- figure 5
fig, ax = plt.subplots(figsize=(5.6, 3.6))
labels = ['before fix', 'after fix']
values = [1.896e8, 5e-4]
bars = ax.bar(labels, values, color=['tab:red', 'tab:blue'], width=0.55)
ax.set_yscale('log')
ax.set_ylim(1e-6, 1e12)
ax.set_ylabel('max |v| after 5 steps  (m/s, log)')
ax.set_title('hopper_test: 15 651 bodies, SPRING = 1e6')
for b, v in zip(bars, values):
    txt = f'$1.9\\times10^{{8}}$' if v > 1 else f'$5\\times10^{{-4}}$'
    ax.text(b.get_x() + b.get_width() / 2, v * 4, txt, ha='center',
            va='bottom', fontsize=11)
ax.grid(axis='x', visible=False)
fig.tight_layout()
fig.savefig(f'{R}/hopper_speed.png', dpi=170, bbox_inches='tight', pad_inches=0.06)
plt.close(fig)

# ---------------------------------------------------------------- figure 6
# montage of six video frames
frames = [0, 80, 140, 200, 320, 599]
titles = ['t = 0.000 s', 't = 0.200 s', 't = 0.350 s',
          't = 0.500 s', 't = 0.800 s', 't = 1.498 s']
fig, axes = plt.subplots(2, 3, figsize=(9.0, 7.4))
for ax, fr, title in zip(axes.ravel(), frames, titles):
    img = plt.imread(f'{R}/frames/frame_{fr:04d}.png')
    h, w = img.shape[:2]
    # crop the empty upper area away
    ax.imshow(img[int(0.095 * h):int(0.99 * h)])
    ax.set_xticks([]); ax.set_yticks([]); ax.grid(False)
    for s in ax.spines.values():
        s.set_visible(False)
    ax.set_title(title, fontsize=10)
fig.suptitle('hopper_fall_test after the fix: 10 spheres drop in and settle',
             fontsize=12, y=0.995)
fig.tight_layout()
fig.savefig(f'{R}/montage.png', dpi=150, bbox_inches='tight', pad_inches=0.06)
plt.close(fig)

print('plots written')
