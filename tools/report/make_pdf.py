"""Build the hopper bug report PDF.

Fixes over the first attempt:
  * DejaVu TTF fonts are registered, so superscripts (10^8) render as glyphs
    instead of black boxes.
  * Table cells are Paragraphs, so long text wraps inside its column.
  * No explicit page breaks; figure+caption pairs are kept together instead,
    which removes the near-empty page.
  * Bullets are round, not the tofu square.
"""
import os

from reportlab.lib import colors
from reportlab.lib.enums import TA_JUSTIFY
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import cm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import (BaseDocTemplate, Frame, Image, KeepTogether,
                                ListFlowable, ListItem, PageBreak, PageTemplate,
                                Paragraph, Preformatted, Spacer, Table, TableStyle)

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
R = os.environ.get('DELTA_REPORT_DIR', os.path.join(ROOT, 'output', 'report'))
OUT = os.environ.get('DELTA_REPORT_PDF', f'{R}/hopper_bug_report.pdf')

# ------------------------------------------------------------------ fonts
import matplotlib

TTF = os.path.join(matplotlib.get_data_path(), 'fonts', 'ttf')
pdfmetrics.registerFont(TTFont('DejaVu', f'{TTF}/DejaVuSans.ttf'))
pdfmetrics.registerFont(TTFont('DejaVu-Bold', f'{TTF}/DejaVuSans-Bold.ttf'))
pdfmetrics.registerFont(TTFont('DejaVu-Oblique', f'{TTF}/DejaVuSans-Oblique.ttf'))
pdfmetrics.registerFont(TTFont('DejaVu-Mono', f'{TTF}/DejaVuSansMono.ttf'))
pdfmetrics.registerFontFamily('DejaVu', normal='DejaVu', bold='DejaVu-Bold',
                              italic='DejaVu-Oblique', boldItalic='DejaVu-Bold')

styles = getSampleStyleSheet()
H1 = ParagraphStyle('H1', parent=styles['Heading1'], fontName='DejaVu-Bold',
                    fontSize=18, leading=22, spaceAfter=4,
                    textColor=colors.HexColor('#12222f'))
SUB = ParagraphStyle('SUB', parent=styles['BodyText'], fontName='DejaVu-Oblique',
                     fontSize=11.5, leading=15, spaceAfter=14,
                     textColor=colors.HexColor('#4a5a68'))
H2 = ParagraphStyle('H2', parent=styles['Heading2'], fontName='DejaVu-Bold',
                    fontSize=13.5, leading=17, spaceBefore=13, spaceAfter=6,
                    keepWithNext=1, textColor=colors.HexColor('#12222f'))
BODY = ParagraphStyle('BODY', parent=styles['BodyText'], fontName='DejaVu',
                      fontSize=10.2, leading=15.2, alignment=TA_JUSTIFY,
                      spaceAfter=8)
CAP = ParagraphStyle('CAP', parent=styles['BodyText'], fontName='DejaVu',
                     fontSize=8.8, leading=12, textColor=colors.HexColor('#55636f'),
                     spaceBefore=4, spaceAfter=14)
CELL = ParagraphStyle('CELL', parent=styles['BodyText'], fontName='DejaVu',
                      fontSize=8.6, leading=11.5)
CELL_H = ParagraphStyle('CELL_H', parent=CELL, fontName='DejaVu-Bold',
                        textColor=colors.white)
CODE = ParagraphStyle('CODE', parent=CELL, fontName='DejaVu-Mono', fontSize=7.8,
                      leading=10.5)

NAVY = colors.HexColor('#2c3e50')
ZEBRA = colors.HexColor('#f4f6f8')
GRID = colors.HexColor('#c9d2da')


def P(text, style=BODY):
    return Paragraph(text, style)


def table(rows, widths, header=True, pad=5, font=8.6, leading=11.5):
    cell = ParagraphStyle('c', parent=CELL, fontSize=font, leading=leading)
    cellh = ParagraphStyle('ch', parent=CELL_H, fontSize=font, leading=leading)
    data = []
    for r, row in enumerate(rows):
        style = cellh if (header and r == 0) else cell
        data.append([Paragraph(c, style) for c in row])
    t = Table(data, colWidths=widths, repeatRows=1 if header else 0)
    cmds = [
        ('GRID', (0, 0), (-1, -1), 0.4, GRID),
        ('VALIGN', (0, 0), (-1, -1), 'TOP'),
        ('LEFTPADDING', (0, 0), (-1, -1), 6),
        ('RIGHTPADDING', (0, 0), (-1, -1), 6),
        ('TOPPADDING', (0, 0), (-1, -1), pad),
        ('BOTTOMPADDING', (0, 0), (-1, -1), pad),
    ]
    if header:
        cmds += [('BACKGROUND', (0, 0), (-1, 0), NAVY),
                 ('ROWBACKGROUNDS', (0, 1), (-1, -1), [colors.white, ZEBRA])]
    else:
        cmds += [('ROWBACKGROUNDS', (0, 0), (-1, -1), [colors.white, ZEBRA])]
    t.setStyle(TableStyle(cmds))
    return t



def codeblock(text, size=7.4, leading=10.2):
    """Monospace block on a light card, kept on one page."""
    pre = Preformatted(text, ParagraphStyle('PRE', fontName='DejaVu-Mono',
                                            fontSize=size, leading=leading))
    box = Table([[pre]], colWidths=[16.4 * cm])
    box.setStyle(TableStyle([
        ('BACKGROUND', (0, 0), (-1, -1), colors.HexColor('#f6f8fa')),
        ('BOX', (0, 0), (-1, -1), 0.5, colors.HexColor('#dbe1e7')),
        ('LEFTPADDING', (0, 0), (-1, -1), 8),
        ('RIGHTPADDING', (0, 0), (-1, -1), 8),
        ('TOPPADDING', (0, 0), (-1, -1), 5),
        ('BOTTOMPADDING', (0, 0), (-1, -1), 5),
    ]))
    return box


def figure(path, width_cm, caption):
    from PIL import Image as PILImage
    w, h = PILImage.open(path).size
    width = width_cm * cm
    height = width * h / w
    return KeepTogether([Image(path, width=width, height=height),
                         Paragraph(caption, CAP)])


# ------------------------------------------------------------------ content
story = []

story.append(P('Hopper Contact Bug: Root Cause and Fix', H1))
story.append(P('Sphere-vs-mesh and sphere-sphere contact in the Delta DEM solver', SUB))

story.append(table([
    ['Test', 'Before the fix', 'After the fix'],
    ['<font face="DejaVu-Mono">hopper_test</font> — 15 651 bodies, '
     '<font face="DejaVu-Mono">SPRING = 1e6</font>',
     'FAIL — max |v| ≈ 1.9 × 10<super>8</super> m/s after 5 steps',
     'PASS — max |v| = 5 × 10<super>−4</super> m/s'],
    ['<font face="DejaVu-Mono">hopper_fall_test</font> — 10 spheres into a funnel',
     'FAIL — spheres ejected sideways through the front/back walls',
     'PASS — no escapes; the pile settles on the throat'],
    ['<font face="DejaVu-Mono">ctest</font> suite', '10 / 11',
     '<b>11 / 11</b>'],
], [6.4 * cm, 5.6 * cm, 4.8 * cm]))
story.append(Spacer(1, 12))

# ---------------------------------------------------------------- section 1
story.append(P('1. Background', H2))
story.append(P(
    'Delta is an explicit discrete-element solver for non-spherical particles. Each iteration runs contact '
    'detection, turns every contact into a spring-dashpot force, then integrates positions. The hopper is the '
    'repository\'s end-to-end scenario: a 25<super>3</super> lattice of 15 625 spheres inside 26 triangulated '
    'wall meshes, plus a smaller 10-sphere funnel test used to exercise the whole contact path. Both were '
    'failing until three independent defects in the contact law were corrected.'))
story.append(P(
    'The failure was spectacular rather than subtle. Three symptoms were measured before anything was changed:'))
story.append(ListFlowable([
    ListItem(Paragraph('<b>Divergence without NaNs</b> — the 15 651-body case stayed finite but reached '
                       'max |v| = 1.9 × 10<super>8</super> m/s inside five steps, with ~42 000 simultaneous '
                       'contacts and the file\'s SPRING = 1e6.', BODY), leftIndent=14),
    ListItem(Paragraph('<b>Bodies outside the container</b> — in the funnel test, bodies were found at '
                       'z = ±0.25 m in a box whose walls are at z = ±0.10 m.', BODY), leftIndent=14),
    ListItem(Paragraph('<b>Drift against a resting contact</b> — a single sphere placed at rest next to a '
                       'wall slowly moved into it, and a sphere that overlapped a neighbour was pushed out '
                       'with more energy than it went in with.', BODY), leftIndent=14),
], bulletType='bullet', start='•', bulletFontName='DejaVu', bulletFontSize=9))
story.append(P(
    'The sections below give the mechanism, the measured evidence and the fix for each defect.'))

# ---------------------------------------------------------------- section 2
story.append(P('2. Bug 1 — the spring used the acceptance quantity, not the overlap', H2))
story.append(P(
    'For sphere-vs-mesh contact the signed gap is <font face="DejaVu-Mono">gap = planeDist − radius</font>. '
    'Detection needs the acceptance quantity <font face="DejaVu-Mono">depth = (ε<sub>A</sub> + ε<sub>B</sub>) − '
    'gap ≥ 0</font> so a sphere is caught slightly before it touches. The bug stored that same acceptance value '
    'as the spring depth. Whenever a sphere sat inside the ε band but was not yet touching '
    '(<font face="DejaVu-Mono">gap &gt; 0</font>), the stored depth became <b>negative</b> and the spring '
    '<i>pulled the sphere into the wall</i> instead of pushing it away. The fix stores the physical overlap, '
    '<font face="DejaVu-Mono">max(0, −gap)</font>, which is exactly zero at first touch.'))
story.append(figure(f'{R}/depth_gap.png', 13.6,
    'Figure 1. Stored spring depth against the true geometry. (a) Sphere vs mesh wall: the old depth is '
    'negative inside the 4 mm detection band, so the spring attracts. (b) Sphere vs sphere: the old depth '
    'falls as the bodies penetrate.'))

# ---------------------------------------------------------------- section 3
story.append(P('3. Bug 2 — a plane test turned every triangle into an infinite plane', H2))
story.append(P(
    'An earlier fix measured the gap against the triangle\'s <i>plane</i> and used the face normal for the '
    'force. That is only correct while the sphere centre projects onto the interior of the triangle. Near the '
    'top or side edge of a slanted hopper wall the projection falls outside the triangle, yet the plane test '
    'still reported a face contact. The sphere was therefore pushed by a wall face it was not touching, which '
    'is what ejected bodies sideways through the front and back walls in '
    '<font face="DejaVu-Mono">hopper_fall_test</font>.'))
story.append(P(
    'The fix introduces <font face="DejaVu-Mono">sphereTriangleContact()</font>: the plane distance is used '
    'when the centre projects inside the triangle, otherwise the gap is measured against the closest edge or '
    'vertex returned by the existing closest-point routine. Back faces are still rejected by winding.'))
story.append(figure(f'{R}/finite_triangle.png', 13.2,
    'Figure 2. A sphere beyond the end of a wall triangle. The old test compared it to the infinite plane '
    '(dashed), found a 4 mm gap and applied the plane normal; the nearest point on the real triangle is the '
    'edge corner, 22 mm away, so there is no contact at all.'))

# ---------------------------------------------------------------- section 4
story.append(P('4. Bug 3 — sphere-sphere depth and normal were inconsistent', H2))
story.append(P(
    'Sphere-sphere contact still used the old acceptance depth, '
    '<font face="DejaVu-Mono">ε<sub>total</sub> − overlap</font>. That value is largest at first touch and '
    'falls to zero as the overlap grows — the opposite of a spring. The stored normal also used the opposite '
    'convention to the sphere-vs-mesh path, so the shared dashpot <i>added</i> energy at shallow overlap '
    'instead of removing it. The fix stores the true overlap and the same B→A normal convention as the wall '
    'path, so one spring-dashpot law is now correct and dissipative for both contact kinds. Panel (b) of '
    'Figure 1 shows the two depth definitions side by side.'))

# ---------------------------------------------------------------- section 5
story.append(P('5. Result — the hopper now stays finite and settles', H2))
story.append(P(
    'With the three fixes, <font face="DejaVu-Mono">hopper_test</font> applies the scenario file\'s '
    '<font face="DejaVu-Mono">SPRING = 1e6</font> again and remains stable: the maximum speed after five steps '
    'is 5 × 10<super>−4</super> m/s, against 1.9 × 10<super>8</super> m/s before. '
    '<font face="DejaVu-Mono">hopper_fall_test</font> now completes all 120 000 steps with no escapes; the ten '
    'spheres fall into the funnel, stay inside the walls and come to rest on the throat.'))
story.append(figure(f'{R}/trajectory.png', 11.6,
    'Figure 3. z trajectory of body 1 in the funnel test. Before the fix the body is ejected through the back '
    'wall (z = −0.25 m); afterwards it stays in the z = 0 symmetry plane for the whole run.'))
story.append(figure(f'{R}/hopper_speed.png', 7.6,
    'Figure 4. Maximum speed over the first five steps of the 15 651-body hopper scenario, before and after '
    'the fix (logarithmic scale).'))
story.append(figure(f'{R}/montage.png', 14.6,
    'Figure 5. The fixed funnel test, rendered from the recorded trajectory (colour = sphere speed). The '
    'companion video is <font face="DejaVu-Mono">output/report/hopper-fall.mp4</font>: 600 frames, 40 fps, 15 s.'))

# ---------------------------------------------------------------- section 6
story.append(P('6. Verification', H2))
story.append(P('The full CMake/ctest suite passes 11/11 on macOS (AppleClang, Release build):'))
story.append(ListFlowable([
    ListItem(Paragraph('<font face="DejaVu-Mono">hopper</font> — 15 651 bodies load, gravity and the contact '
                       'law come from the file, five steps stay finite', BODY), leftIndent=14),
    ListItem(Paragraph('<font face="DejaVu-Mono">hopper_fall</font> — 10 spheres fall into the funnel, stay '
                       'inside and settle', BODY), leftIndent=14),
    ListItem(Paragraph('<font face="DejaVu-Mono">regression</font>, <font face="DejaVu-Mono">grid_modes</font>, '
                       '<font face="DejaVu-Mono">dt_controller</font>, <font face="DejaVu-Mono">stl_read</font>, '
                       '<font face="DejaVu-Mono">frame_staging</font>, <font face="DejaVu-Mono">viz_write</font>, '
                       '<font face="DejaVu-Mono">dem_phase0</font>, <font face="DejaVu-Mono">dem2d</font>, '
                       '<font face="DejaVu-Mono">dem2d_viz</font> — unchanged and passing',
                       BODY), leftIndent=14),
], bulletType='bullet', start='•', bulletFontName='DejaVu', bulletFontSize=9))

# ---------------------------------------------------------------- section 7
measurements = table([
    ['Measurement', 'Before', 'After'],
    ['<font face="DejaVu-Mono">hopper_test</font>: max |v| after 5 steps (k = 1e6)',
     '1.9 × 10<super>8</super> m/s', '5 × 10<super>−4</super> m/s'],
    ['<font face="DejaVu-Mono">hopper_test</font>: contacts resolved on step 5',
     '41 988', '47 600'],
    ['<font face="DejaVu-Mono">hopper_test</font>: non-finite values',
     'none', 'none'],
    ['<font face="DejaVu-Mono">hopper_fall_test</font>: bodies outside the walls',
     'yes, up to z = ±0.25 m', 'none'],
    ['<font face="DejaVu-Mono">hopper_fall_test</font>: max |v| over the run',
     'diverged before settling', '3.25 m/s'],
    ['<font face="DejaVu-Mono">hopper_fall_test</font>: max |v| at t = 1.2 s',
     '—', '2.8 × 10<super>−3</super> m/s'],
    ['<font face="DejaVu-Mono">ctest</font>', '10 / 11', '11 / 11'],
], [8.4 * cm, 4.6 * cm, 3.8 * cm], pad=3, font=8.2, leading=10.6)
story.append(Spacer(1, 4))
story.append(KeepTogether([P('The numbers behind the two FAILs:', BODY), measurements]))

# ---------------------------------------------------------------- section 7
story.append(P('7. Files changed', H2))
story.append(table([
    ['File', 'Change'],
    ['<font face="DejaVu-Mono" size="7.8">src/dim3/contact/detection/sphere.cpp</font>',
     'Finite-triangle sphere contact; spring depth is the real overlap; sphere-sphere normal and depth '
     'corrected; dead surfaceNormal() removed'],
    ['<font face="DejaVu-Mono" size="7.8">tests/hopper_test.cpp</font>',
     'Re-applies the file\'s SPRING = 1e6 and removes the DELTA_SKIP_HOPPER_RUN escape hatch'],
    ['<font face="DejaVu-Mono" size="7.8">docs/CONTACT-DETECTION-NOTES.md</font>',
     'Defects 5c and 6 recorded as fixed, with their root causes'],
    ['<font face="DejaVu-Mono" size="7.8">output/report/hopper-fall.mp4</font>',
     'Rendered simulation video of the fixed funnel test'],
], [5.6 * cm, 11.2 * cm]))

# ---------------------------------------------------------------- section 8
story.append(P('8. What to keep in mind', H2))
story.append(P(
    'Three conventions in this contact path are easy to break, and each of them caused one of the defects '
    'above:'))
story.append(ListFlowable([
    ListItem(Paragraph('<b>Depth is the overlap, not the detection tolerance.</b> The acceptance quantity may '
                       'be positive before contact; the spring force must not be.', BODY), leftIndent=14),
    ListItem(Paragraph('<b>Measure against the finite triangle.</b> A plane test silently inflates every wall '
                       'to an infinite sheet, and the error is invisible until a body is near an edge.',
                       BODY), leftIndent=14),
    ListItem(Paragraph('<b>One normal convention per contact kind.</b> Both sides of a pair store the same '
                       'contact with the second copy negated; mixing conventions flips the dashpot from '
                       'dissipative to energy-adding.', BODY), leftIndent=14),
], bulletType='bullet', start='•', bulletFontName='DejaVu', bulletFontSize=9))
story.append(Spacer(1, 6))
story.append(P(
    'These changes alter the contact law for every simulation, not only the hopper. Any scenario or tuning '
    'that implicitly relied on the old pre-load, or on plane-shaped walls, will now behave differently — in '
    'both cases more physically.'))


# ---------------------------------------------------------------- appendix
appendix_intro = P('The behavioural change is these lines in '
               '<font face="DejaVu-Mono">sphereTriangleContact()</font>.')
code = (
"""const iREAL planeDist = planeDistance(TP1, n, P);
if (planeDist < 0.0) return false;              // back face, not a contact

const iREAL qDist = pt(TP1, TP2, TP3, P, Q);    // closest point on the triangle
const iREAL tol   = 1e-12 * std::max(1.0, std::fabs(planeDist));
if (qDist <= planeDist + tol) {                 // centre is over the interior
  cn[0] = n[0]; cn[1] = n[1]; cn[2] = n[2];
  gap   = planeDist - radius;
} else {                                        // nearest feature: an edge
  cn[0] = (P[0]-Q[0]) / qDist;                  //  or a vertex - use it
  cn[1] = (P[1]-Q[1]) / qDist;  cn[2] = (P[2]-Q[2]) / qDist;
  gap   = qDist - radius;
}
return (epsTotal - gap) >= 0.0;                 // acceptance, never the force""")
story.append(KeepTogether([P('Appendix A — the fix in code', H2), appendix_intro, codeblock(code)]))
repro = (
"""cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
(cd build && ctest)             # 11/11

./build/delta_hopper_test       # 15 651 bodies, 5 steps, |v| bounded
./build/delta_hopper_fall_test  # 10 spheres into the funnel, 120 000 steps
python3 tools/report/make_report.py   # figures, then make_pdf.py""")
story.append(KeepTogether([P('Reproducing', H2), codeblock(repro)]))

story.append(P('Appendix B — how the video was produced', H2))
story.append(P('The video is not a screen recording: it is rendered from the trajectory the solver actually '
               'produced, so it can be regenerated for any scenario.'))
steps = ListFlowable([
    ListItem(Paragraph('<b>Dump</b> — a small driver runs the same scene as '
                       '<font face="DejaVu-Mono">hopper_fall_test</font> and writes every free particle '
                       'position and speed to CSV every 200 steps (600 frames).', BODY), leftIndent=14),
    ListItem(Paragraph('<b>Project</b> — a Python renderer applies a fixed orthographic camera, sorts walls '
                       'and spheres back-to-front and draws circles for the spheres, so transparency and '
                       'overlap look right without a 3D engine. Container faces are culled by their winding, '
                       'which gives the cutaway view into the funnel.', BODY), leftIndent=14),
    ListItem(Paragraph('<b>Encode</b> — ffmpeg composes the PNG sequence at 40 fps into H.264 '
                       '(<font face="DejaVu-Mono">libx264</font>, CRF 20, yuv420p, faststart).', BODY),
             leftIndent=14),
], bulletType='bullet', start='•', bulletFontName='DejaVu', bulletFontSize=9)
story.append(steps)
story.append(Spacer(1, 6))
pipe = (
"""# 1. dump the trajectory (driver links libdelta3d_core.a)
./build/hf_video output/report         # -> video_particles.csv, video_walls.csv
# 2. render frames
python3 tools/report/render_video.py   # -> frames/frame_0000.png ... frame_0599.png
# 3. encode
ffmpeg -framerate 40 -i output/report/frames/frame_%04d.png \\
       -c:v libx264 -crf 20 -pix_fmt yuv420p \\
       -movflags +faststart hopper_fall.mp4""")
story.append(KeepTogether([codeblock(pipe, size=7.8, leading=11.0)]))
story.append(Spacer(1, 8))

story.append(P('Status and where to look', H2))
story.append(P('The changes are uncommitted on branch '
               '<font face="DejaVu-Mono">amrex-integration</font>. The investigation behind them — including '
               'the measurements that were wrong on the first attempt and how they were corrected — is '
               'recorded in <font face="DejaVu-Mono">docs/CONTACT-DETECTION-NOTES.md</font>, which is also the '
               'place to extend if a new shape or wall mesh misbehaves. The two artefacts next to this report '
               'are the rendered video (<font face="DejaVu-Mono">output/report/hopper-fall.mp4</font>) and the scripts that '
               'produced it under <font face="DejaVu-Mono">tools/report</font>.'))

story.append(Spacer(1, 6))
story.append(table([
    ['Where', 'What'],
    ['<font face="DejaVu-Mono" size="7.8">src/dim3/contact/detection/sphere.cpp</font>',
     'Sphere-vs-triangle and sphere-vs-sphere detection, and the depth/normal stored for the force law'],
    ['<font face="DejaVu-Mono" size="7.8">src/dim3/contact/forces/forces.cpp</font>',
     'The spring-dashpot law itself — unchanged, but it now receives the right depth'],
    ['<font face="DejaVu-Mono" size="7.8">tests/hopper_test.cpp</font>',
     'The assertions that pin the scenario file to the engine'],
    ['<font face="DejaVu-Mono" size="7.8">tests/hopper_fall_test.cpp</font>',
     'The end-to-end funnel test, including the winding and obstacle-centre notes'],
], [6.2 * cm, 10.6 * cm], pad=3, font=8.2, leading=10.6))

# ------------------------------------------------------------------ build
class Doc(BaseDocTemplate):
    def __init__(self, filename, **kw):
        BaseDocTemplate.__init__(self, filename, **kw)
        frame = Frame(self.leftMargin, self.bottomMargin, self.width, self.height,
                      id='body')
        self.addPageTemplates([PageTemplate(id='main', frames=[frame],
                                            onPage=self.footer)])

    def footer(self, canvas, doc):
        canvas.saveState()
        canvas.setFont('DejaVu', 8)
        canvas.setFillColor(colors.HexColor('#8b98a5'))
        canvas.drawString(doc.leftMargin, 1.15 * cm,
                          'Delta — hopper contact bug report')
        canvas.drawRightString(A4[0] - doc.rightMargin, 1.15 * cm,
                               f'{doc.page}')
        canvas.setStrokeColor(colors.HexColor('#dde3e8'))
        canvas.setLineWidth(0.5)
        canvas.line(doc.leftMargin, 1.55 * cm, A4[0] - doc.rightMargin, 1.55 * cm)
        canvas.restoreState()


doc = Doc(OUT, pagesize=A4, leftMargin=2.1 * cm, rightMargin=2.1 * cm,
          topMargin=1.9 * cm, bottomMargin=2.0 * cm,
          title='Hopper Contact Bug: Root Cause and Fix',
          author='Delta')
doc.build(story)
print('PDF written:', OUT)
