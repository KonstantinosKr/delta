/*
 * write.cpp
 *
 *  Created on: 20 Oct 2022
 *      Author: konstantinos
 *
 * The two model-specific writers below do nothing but turn a model into a
 * MeshBuffer; emission is shared in core/io/MeshBuffer.cpp.
 */

#include "write.h"

#include <vtkCellType.h>

void delta::core::io::writeGeometryToVTKVTK(
    std::string                                     path,
    int                                             step,
    std::vector<delta::core::data::ParticleRecord>& geometries) {

  MeshBuffer buffer;
  buffer.pointFields.push_back({"velocity", 3, false, {}});
  buffer.pointFields.push_back({"radius", 1, false, {}});
  buffer.pointFields.push_back({"epsilon", 1, false, {}});
  buffer.cellFields.push_back({"global_id", 1, true, {}});
  buffer.cellFields.push_back({"material", 1, true, {}});

  std::vector<double>& velocity = buffer.pointFields[0].data;
  std::vector<double>& radius = buffer.pointFields[1].data;
  std::vector<double>& epsilon = buffer.pointFields[2].data;
  std::vector<double>& globalId = buffer.cellFields[0].data;
  std::vector<double>& material = buffer.cellFields[1].data;

  long long offset = 0;

  for (auto& geometry : geometries) {
    const int   numberOfTriangles = geometry.getNumberOfTriangles();
    const iREAL rad = geometry.getRad();
    const iREAL eps = geometry.getEpsilon();

    auto addVertex = [&](iREAL x, iREAL y, iREAL z) {
      buffer.points.push_back({{x, y, z}});
      velocity.push_back(geometry._linearVelocity[0]);
      velocity.push_back(geometry._linearVelocity[1]);
      velocity.push_back(geometry._linearVelocity[2]);
      radius.push_back(rad);
      epsilon.push_back(eps);
    };

    if (numberOfTriangles == 0) {
      //Non-mesh particle (sphere): keep a point so glyph rendering still works.
      addVertex(geometry._centre[0], geometry._centre[1], geometry._centre[2]);
      ++offset;
      continue;
    }

    const int numberOfVertices = numberOfTriangles * 3;
    for (int v = 0; v < numberOfVertices; ++v) {
      addVertex(geometry._xCoordinates[v],
                geometry._yCoordinates[v],
                geometry._zCoordinates[v]);
    }

    for (int t = 0; t < numberOfTriangles; ++t) {
      buffer.cells.push_back({VTK_TRIANGLE,
                              {offset + t * 3 + 0,
                               offset + t * 3 + 1,
                               offset + t * 3 + 2}});
      globalId.push_back(geometry.getGlobalParticleID());
      material.push_back(int(geometry.getMaterial()));
    }

    offset += numberOfVertices;
  }

  buffer.activeScalars = "radius";
  writeVTU(path, "geometry", step, buffer);
}

void delta::core::io::writeGridGeometryToVTKVTK(
    std::string 						path,
    int 								step,
    std::vector<std::array<iREAL, 6>> 	boundary)
{
  MeshBuffer buffer;
  buffer.pointFields.push_back({"vector123", 3, false, {}});
  std::vector<double>& normals = buffer.pointFields[0].data;

  // The twelve edges of a box, in the point order pushed below.
  static const int edges[12][2] = {
      {0, 1}, {0, 3}, {0, 6}, {1, 2}, {1, 7}, {2, 3},
      {3, 5}, {6, 7}, {6, 5}, {4, 2}, {4, 7}, {4, 5}};

  for (const auto& value : boundary) {
    const iREAL lo[3] = {value[0], value[1], value[2]};
    const iREAL hi[3] = {value[3], value[4], value[5]};

    const double corners[8][3] = {
        {lo[0], lo[1], lo[2]},  //0: A
        {lo[0], hi[1], lo[2]},  //1: B
        {lo[0], hi[1], hi[2]},  //2: E
        {lo[0], lo[1], hi[2]},  //3: F
        {hi[0], hi[1], hi[2]},  //4: H
        {hi[0], lo[1], hi[2]},  //5: G
        {hi[0], lo[1], lo[2]},  //6: D
        {hi[0], hi[1], lo[2]}}; //7: C

    // Points, and therefore cell ids, accumulate across boxes; each box's ids
    // are offset by the running point count.
    const long long base = static_cast<long long>(buffer.points.size());

    for (int c = 0; c < 8; ++c) {
      buffer.points.push_back({{corners[c][0], corners[c][1], corners[c][2]}});
      normals.push_back(-1.0);
      normals.push_back(0.0);
      normals.push_back(0.0);
    }

    for (int e = 0; e < 12; ++e)
      buffer.cells.push_back({VTK_LINE, {base + edges[e][0], base + edges[e][1]}});
  }

  buffer.activeVectors = "vector123";
  writeVTU(path, "grid", step, buffer);
}
