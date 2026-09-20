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
 * Checks that the 2D writer emits real filled cells (a tessellated disk and
 * actual triangles), the per-particle fields, and a ParaView time-series
 * collection that orders frames by step.
 *
 * Run: ctest --test-dir build  (or ./build/delta_dem2d_viz_test)
 */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "dem2d/Dem2D.h"
#include "dem2d/Dem2DWrite.h"

using namespace delta::dem2d;

namespace {

  std::string slurp(const std::string& file)
  {
    std::ifstream in(file.c_str());
    return std::string((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
  }

  /* Value of an XML attribute, e.g. attr(xml, "NumberOfCells=\"") . */
  int attr(const std::string& xml, const std::string& tag)
  {
    std::size_t pos = xml.find(tag);
    assert(pos != std::string::npos && "attribute missing from the vtu");
    return std::atoi(xml.c_str() + pos + tag.size());
  }

  int count(const std::string& haystack, const std::string& needle)
  {
    int n = 0;
    for (std::size_t pos = haystack.find(needle); pos != std::string::npos;
         pos = haystack.find(needle, pos + needle.size()))
      ++n;
    return n;
  }

}  // namespace

int main()
{
  const std::string dir = "dem2dviz/";
  assert(std::system(("rm -rf " + dir + " && mkdir -p " + dir).c_str()) == 0);

  /* one disk, one triangle, and a wall built from two triangles */
  const int segments = 24;

  std::vector<Particle2D> ps;
  ps.push_back(makeCircle(0, Vec2(1.0, 2.0), 0.5, 1000.0, 0.3));

  std::array<Vec2, 3> tri = { Vec2(3.0, 3.0), Vec2(4.0, 3.0), Vec2(3.5, 4.0) };
  ps.push_back(makeTriangle(1, tri, 1000.0, 0.3));

  std::array<Vec2, 3> w0 = { Vec2(0.0, 0.0), Vec2(10.0, 0.0), Vec2(10.0, 0.5) };
  std::array<Vec2, 3> w1 = { Vec2(0.0, 0.0), Vec2(10.0, 0.5), Vec2(0.0, 0.5) };
  Particle2D wallA = makeTriangle(2, w0, 1000.0, 0.0);
  Particle2D wallB = makeTriangle(3, w1, 1000.0, 0.0);
  makeStatic(wallA);
  makeStatic(wallB);
  ps.push_back(wallA);
  ps.push_back(wallB);

  Engine2D engine(ps);
  writeDem2DToVTK(dir, 0, engine.particles());
  engine.step(1e-3);
  writeDem2DToVTK(dir, 1, engine.particles());

  const std::string vtu = slurp(dir + "dem2d_0.vtu");
  assert(!vtu.empty() && "vtu must exist");
  assert(vtu.find("<VTKFile") != std::string::npos &&
         "must be an XML vtu, not the legacy vtk format");

  /* every particle is one cell: the disk is a filled polygon, not a point */
  assert(attr(vtu, "NumberOfCells=\"") == 4);
  /* 24 ring points for the disk, 3 per triangle */
  assert(attr(vtu, "NumberOfPoints=\"") == segments + 3 * 3);

  assert(vtu.find("Name=\"velocity\"") != std::string::npos);
  assert(vtu.find("Name=\"radius\"") != std::string::npos);
  assert(vtu.find("Name=\"global_id\"") != std::string::npos);
  assert(vtu.find("Name=\"shape\"") != std::string::npos);
  assert(vtu.find("Name=\"angular_velocity\"") != std::string::npos);
  assert(vtu.find("Name=\"obstacle\"") != std::string::npos);

  const std::string pvd = slurp(dir + "dem2d.pvd");
  assert(count(pvd, "<DataSet") == 2 && "both frames must be in the collection");
  assert(pvd.find("dem2d_0.vtu") != std::string::npos);
  assert(pvd.find("dem2d_1.vtu") != std::string::npos);
  assert(pvd.find("dem2d_0.vtu") < pvd.find("dem2d_1.vtu") &&
         "frames must be ordered by step");

  std::printf("dem2d_viz_test OK: %d cells, %d points, %d frames\n",
              4, segments + 9, count(pvd, "<DataSet"));
  return 0;
}
