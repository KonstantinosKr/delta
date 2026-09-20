/*
 * Checks that the 3D writer emits real triangle cells and a ParaView
 * time-series collection, not just a point cloud.
 *
 * Run: ctest --test-dir build  (or ./build/delta_viz_test)
 */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "core/data/Structure.h"
#include "core/io/write.h"
#include "geometry/shape/cube.h"
#include "world/structure/Object.h"

namespace {

std::string slurp(const std::string& file) {
  std::ifstream in(file);
  return std::string((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());
}

}  // namespace

int main() {
  std::system("mkdir -p viztest");

  iREAL centre[3] = {0.0, 0.0, 0.0};
  delta::geometry::mesh::Mesh* mesh =
      delta::geometry::primitive::cube::generateCube(centre, 0.5, 0.0, 0.0, 0.0);

  delta::world::structure::Object object(
      "cube", 7, mesh, {{0.0, 0.0, 0.0}},
      delta::geometry::material::MaterialType::WOOD,
      false, false, true, 0.01, {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});

  std::vector<delta::world::structure::Object> objects{object};
  delta::core::data::Structure structure(objects);
  std::vector<delta::core::data::ParticleRecord>& records = structure.getAll();

  int triangles = 0;
  for (auto& record : records) {
    triangles += record.getNumberOfTriangles();
  }
  assert(triangles > 0 && "cube mesh must expose triangles");

  delta::core::io::writeGeometryToVTKVTK("./viztest/", 0, records);
  delta::core::io::writeGeometryToVTKVTK("./viztest/", 1, records);

  std::string vtu = slurp("./viztest/geometry_0.vtu");
  const std::string cellTag = "NumberOfCells=\"";
  std::size_t pos = vtu.find(cellTag);
  assert(pos != std::string::npos && "vtu must declare NumberOfCells");
  int cells = std::atoi(vtu.c_str() + pos + cellTag.size());
  assert(cells == triangles && "every triangle must become one VTK cell");

  std::string pvd = slurp("./viztest/geometry.pvd");
  assert(pvd.find("geometry_0.vtu") != std::string::npos);
  assert(pvd.find("geometry_1.vtu") != std::string::npos);

  std::printf("viz_write_test OK: %d triangles, %d cells\n", triangles, cells);
  return 0;
}
