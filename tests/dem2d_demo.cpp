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
 * Runs a mixed circle/triangle scene: static box walls, two shelves, and a
 * stack of disks and triangles falling under gravity, then prints per-step
 * statistics.
 *
 * Writes a ParaView time series into the output directory:
 *
 *   <dir>/dem2d_<step>.vtu   one frame, real filled geometry + fields
 *   <dir>/dem2d.pvd          the collection, open this one in ParaView
 *
 *   ./delta_dem2d_demo [steps] [bodies] [saveEvery] [outDir]
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "dem2d/Dem2D.h"
#include "dem2d/Dem2DWrite.h"

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

}  // namespace

int main(int argc, char** argv)
{
  const int         steps     = argc > 1 ? std::atoi(argv[1]) : 3000;
  const int         bodies    = argc > 2 ? std::atoi(argv[2]) : 60;
  const int         saveEvery = argc > 3 ? std::atoi(argv[3]) : 25;
  const std::string outDir    = argc > 4 ? std::string(argv[4]) : std::string("dem2d_out/");
  const iREAL       dt        = 1e-3;

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

  Engine2D engine(ps);

  if (std::system(("mkdir -p " + outDir).c_str()) != 0) {
    std::printf("cannot create %s\n", outDir.c_str());
    return 1;
  }

  std::printf("dem2d demo: %d bodies (%d static), dt=%g, %d steps\n",
              (int)engine.particles().size(),
              (int)std::count_if(engine.particles().begin(), engine.particles().end(),
                                 [](const Particle2D& p) { return p.isObstacle; }),
              dt, steps);

  int frames = 0;
  writeDem2DToVTK(outDir, 0, engine.particles());
  ++frames;

  for (int s = 1; s <= steps; ++s) {
    engine.step(dt);
    if (s % saveEvery == 0) {
      writeDem2DToVTK(outDir, s, engine.particles());
      ++frames;
    }
    if (s % (steps / 10 == 0 ? 1 : steps / 10) == 0) {
      int contacts = (int)engine.contacts().size();
      iREAL maxSpeed = 0.0;
      for (size_t i = 0; i < engine.particles().size(); ++i)
        if (!engine.particles()[i].isObstacle)
          maxSpeed = std::max(maxSpeed, length(engine.particles()[i].velocity));
      std::printf("  step %5d  contacts=%5d  max|v|=%8.4g\n", s, contacts, maxSpeed);
    }
  }

  std::printf("wrote %sdem2d_<step>.vtu + %sdem2d.pvd (%d frames, open the .pvd)\n",
              outDir.c_str(), outDir.c_str(), frames);
  return 0;
}
