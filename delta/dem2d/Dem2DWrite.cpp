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

#include "Dem2DWrite.h"

#include <cmath>

#include <vtkCellType.h>

#include "../core/io/MeshBuffer.h"

namespace delta {
  namespace dem2d {

    void writeDem2DToVTK(const std::string& path, int step,
                         const std::vector<Particle2D>& particles)
    {
      const iREAL TWO_PI = 6.28318530717958647692;
      const int circleSegments = 24;

      delta::core::io::MeshBuffer buffer;
      buffer.pointFields.push_back({"velocity", 3, false, {}});
      buffer.pointFields.push_back({"radius", 1, false, {}});
      buffer.cellFields.push_back({"global_id", 1, true, {}});
      buffer.cellFields.push_back({"shape", 1, true, {}});
      buffer.cellFields.push_back({"angular_velocity", 1, false, {}});
      buffer.cellFields.push_back({"mass", 1, false, {}});
      buffer.cellFields.push_back({"obstacle", 1, true, {}});

      std::vector<double>& velocity = buffer.pointFields[0].data;
      std::vector<double>& radius = buffer.pointFields[1].data;
      std::vector<double>& globalId = buffer.cellFields[0].data;
      std::vector<double>& shape = buffer.cellFields[1].data;
      std::vector<double>& angularVelocity = buffer.cellFields[2].data;
      std::vector<double>& mass = buffer.cellFields[3].data;
      std::vector<double>& obstacle = buffer.cellFields[4].data;

      for (std::vector<Particle2D>::const_iterator it = particles.begin();
           it != particles.end(); ++it) {
        const Particle2D& p = *it;
        const long long offset = static_cast<long long>(buffer.points.size());

        std::vector<long long> ids;
        if (p.shape == ShapeType::Circle) {
          ids.reserve(circleSegments);
          for (int s = 0; s < circleSegments; ++s) {
            const iREAL a = TWO_PI * s / (iREAL)circleSegments;
            // Plane geometry sits in z = 0; VTK points are always 3D.
            buffer.points.push_back({{p.position.x + p.radius * std::cos(a),
                                      p.position.y + p.radius * std::sin(a), 0.0}});
            velocity.push_back(p.velocity.x);
            velocity.push_back(p.velocity.y);
            velocity.push_back(0.0);
            radius.push_back(p.radius);
            ids.push_back(offset + s);
          }
          buffer.cells.push_back({VTK_POLYGON, ids});
        } else {
          for (int v = 0; v < 3; ++v) {
            buffer.points.push_back({{p.worldVertices[v].x, p.worldVertices[v].y, 0.0}});
            velocity.push_back(p.velocity.x);
            velocity.push_back(p.velocity.y);
            velocity.push_back(0.0);
            radius.push_back(p.boundingRadius);
            ids.push_back(offset + v);
          }
          buffer.cells.push_back({VTK_TRIANGLE, ids});
        }

        globalId.push_back(p.id);
        shape.push_back(p.shape == ShapeType::Circle ? 0 : 1);
        angularVelocity.push_back(p.angularVelocity);
        mass.push_back(p.mass);
        obstacle.push_back(p.isObstacle ? 1 : 0);
      }

      buffer.activeScalars = "radius";
      delta::core::io::writeVTU(path, "dem2d", step, buffer);
    }

  }
}
