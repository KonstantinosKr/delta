/*
 The MIT License (MIT)

 Copyright (c) 2022 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

/*
 * Runs a mixed circle/triangle scene: static box walls, a stack of disks and
 * triangles falling under gravity, then prints per-step statistics. Writes
 * dem2d_frames.csv for plotting, one row per body per saved frame:
 *
 *   frame,time,id,shape,x,y,angle,radius,v0x,v0y,v1x,v1y,v2x,v2y,vx,vy,omega
 *
 * Circles carry a radius and leave the vertex columns at 0; triangles carry
 * the three body-frame vertex offsets and a radius of 0.
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "dem2d/Dem2D.h"

using namespace delta::dem2d;

namespace {

  void addRect(std::vector<Particle2D>& ps, double x0, double y0, double x1, double y1, int baseId)
  {
    std::array<Vec2, 3> t0 = { Vec2(x0, y0), Vec2(x1, y0), Vec2(x1, y1) };
    std::array<Vec2, 3> t1 = { Vec2(x0, y0), Vec2(x1, y1), Vec2(x0, y1) };
    Particle2D a = makeTriangle(baseId, t0, 1000.0, 0.0);
    Particle2D b = makeTriangle(baseId + 1, t1, 1000.0, 0.0);
    makeStatic(a);
    makeStatic(b);
    ps.push_back(a);
    ps.push_back(b);
  }

  void writeFrame(std::FILE* f, int frame, double time, const std::vector<Particle2D>& ps)
  {
    for (size_t i = 0; i < ps.size(); ++i) {
      const Particle2D& p = ps[i];
      const bool circle = (p.shape == ShapeType::Circle);
      std::fprintf(f, "%d,%.6g,%d,%s,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g\n",
                   frame, time, p.id, circle ? "circle" : "triangle",
                   p.position.x, p.position.y, p.angle,
                   circle ? p.radius : 0.0,
                   circle ? 0.0 : p.localVertices[0].x, circle ? 0.0 : p.localVertices[0].y,
                   circle ? 0.0 : p.localVertices[1].x, circle ? 0.0 : p.localVertices[1].y,
                   circle ? 0.0 : p.localVertices[2].x, circle ? 0.0 : p.localVertices[2].y,
                   p.velocity.x, p.velocity.y, p.angularVelocity);
    }
  }

}  // namespace

int main(int argc, char** argv)
{
  const int   steps    = argc > 1 ? std::atoi(argv[1]) : 3000;
  const int   bodies   = argc > 2 ? std::atoi(argv[2]) : 60;
  const int   saveEvery = argc > 3 ? std::atoi(argv[3]) : 25;
  const iREAL dt       = 1e-3;

  std::vector<Particle2D> ps;

  /* static box, open at the top */
  addRect(ps, -1.0, -1.0,  0.0, 21.0, 0);     /* left   */
  addRect(ps, 20.0, -1.0, 21.0, 21.0, 10);    /* right  */
  addRect(ps, -1.0, -1.0, 21.0,  0.0, 20);    /* bottom */

  /* two stacked static shelves to make the pile spread */
  addRect(ps,  4.0,  6.0,  9.0,  6.4, 40);
  addRect(ps, 12.0, 11.0, 17.0, 11.4, 50);

  int nextId = 100;
  for (int i = 0; i < bodies; ++i) {
    const double x = 2.0 + (i % 8) * 1.6;
    const double y = 14.0 + (i / 8) * 1.2;
    if (i % 2 == 0) {
      ps.push_back(makeCircle(nextId++, Vec2(x, y), 0.3, 1000.0, 0.4));
    } else {
      std::array<Vec2, 3> v = { Vec2(x - 0.35, y - 0.35), Vec2(x + 0.35, y - 0.35), Vec2(x, y + 0.35) };
      ps.push_back(makeTriangle(nextId++, v, 1000.0, 0.4));
    }
  }

  std::FILE* out = std::fopen("dem2d_frames.csv", "w");
  if (out == nullptr) {
    std::printf("cannot open dem2d_frames.csv\n");
    return 1;
  }
  std::fprintf(out, "frame,time,id,shape,x,y,angle,radius,v0x,v0y,v1x,v1y,v2x,v2y,vx,vy,omega\n");
  writeFrame(out, 0, 0.0, ps);

  Engine2D engine(ps);
  std::printf("dem2d demo: %d bodies (%d static), dt=%g, %d steps\n",
              (int)engine.particles().size(),
              (int)std::count_if(engine.particles().begin(), engine.particles().end(),
                                 [](const Particle2D& p) { return p.isObstacle; }),
              dt, steps);

  for (int s = 1; s <= steps; ++s) {
    engine.step(dt);
    if (s % saveEvery == 0) writeFrame(out, s, s * dt, engine.particles());
    if (s % (steps / 10 == 0 ? 1 : steps / 10) == 0) {
      int contacts = (int)engine.contacts().size();
      iREAL maxSpeed = 0.0;
      for (size_t i = 0; i < engine.particles().size(); ++i)
        if (!engine.particles()[i].isObstacle)
          maxSpeed = std::max(maxSpeed, length(engine.particles()[i].velocity));
      std::printf("  step %5d  contacts=%5d  max|v|=%8.4g\n", s, contacts, maxSpeed);
    }
  }

  std::fclose(out);
  std::printf("wrote dem2d_frames.csv (%d frames)\n", steps / saveEvery + 1);
  return 0;
}
