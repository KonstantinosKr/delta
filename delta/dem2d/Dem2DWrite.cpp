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

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <glob.h>
#include <utility>
#include <vector>

#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>

namespace delta {
  namespace dem2d {

    namespace {

      const iREAL TWO_PI = 6.28318530717958647692;

      /*
       * Rewrite the ParaView collection so every dem2d_<step>.vtu on disk
       * loads as one animated time series. Stateless: it rescans the
       * directory on every call, same as the 3D writeGeometryPVD.
       */
      void writeDem2DPVD(const std::string& path)
      {
        const std::string prefix = "dem2d_";
        const std::string suffix = ".vtu";

        std::vector<std::pair<int, std::string> > frames;

        glob_t matches;
        if (glob((path + prefix + "*" + suffix).c_str(), 0, nullptr, &matches) == 0) {
          for (size_t i = 0; i < matches.gl_pathc; ++i) {
            std::string full(matches.gl_pathv[i]);
            if (full.size() < path.size() + prefix.size() + suffix.size()) continue;
            std::string base   = full.substr(path.size());
            std::string digits = base.substr(prefix.size(),
                                             base.size() - prefix.size() - suffix.size());
            frames.push_back(std::make_pair(std::atoi(digits.c_str()), base));
          }
        }
        globfree(&matches);

        std::sort(frames.begin(), frames.end());

        std::ofstream pvd(path + "dem2d.pvd");
        if (!pvd) return;
        pvd << "<?xml version=\"1.0\"?>\n"
               "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
               "  <Collection>\n";
        char timestep[64];
        for (size_t i = 0; i < frames.size(); ++i) {
          std::snprintf(timestep, sizeof(timestep), "%.6g", double(frames[i].first));
          pvd << "    <DataSet timestep=\"" << timestep
              << "\" group=\"\" part=\"0\" file=\"" << frames[i].second << "\"/>\n";
        }
        pvd << "  </Collection>\n"
               "</VTKFile>\n";
      }

    }  // namespace

    void writeDem2DToVTK(const std::string& path, int step,
                         const std::vector<Particle2D>& particles)
    {
      const std::string filename = path + "dem2d_" + std::to_string(step) + ".vtu";

      auto grid   = vtkSmartPointer<vtkUnstructuredGrid>::New();
      auto points = vtkSmartPointer<vtkPoints>::New();
      points->SetDataTypeToDouble();

      /* Per-point fields: velocity keeps glyph rendering working, radius is
       * the glyph scale for a disk. */
      auto velocity = vtkSmartPointer<vtkDoubleArray>::New();
      velocity->SetNumberOfComponents(3);
      velocity->SetName("velocity");

      auto radius = vtkSmartPointer<vtkDoubleArray>::New();
      radius->SetNumberOfComponents(1);
      radius->SetName("radius");

      /* Per-particle fields. */
      auto globalId = vtkSmartPointer<vtkIntArray>::New();
      globalId->SetNumberOfComponents(1);
      globalId->SetName("global_id");

      auto shape = vtkSmartPointer<vtkIntArray>::New();
      shape->SetNumberOfComponents(1);
      shape->SetName("shape");           /* 0 = circle, 1 = triangle */

      auto angularVelocity = vtkSmartPointer<vtkDoubleArray>::New();
      angularVelocity->SetNumberOfComponents(1);
      angularVelocity->SetName("angular_velocity");

      auto mass = vtkSmartPointer<vtkDoubleArray>::New();
      mass->SetNumberOfComponents(1);
      mass->SetName("mass");

      auto obstacle = vtkSmartPointer<vtkIntArray>::New();
      obstacle->SetNumberOfComponents(1);
      obstacle->SetName("obstacle");

      const int circleSegments = 24;

      for (std::vector<Particle2D>::const_iterator it = particles.begin();
           it != particles.end(); ++it) {
        const Particle2D& p = *it;
        const vtkIdType offset = points->GetNumberOfPoints();

        if (p.shape == ShapeType::Circle) {
          std::vector<vtkIdType> ids;
          ids.reserve(circleSegments);
          for (int s = 0; s < circleSegments; ++s) {
            const iREAL a = TWO_PI * s / (iREAL)circleSegments;
            points->InsertNextPoint(p.position.x + p.radius * std::cos(a),
                                    p.position.y + p.radius * std::sin(a),
                                    0.0);
            velocity->InsertNextTuple3(p.velocity.x, p.velocity.y, 0.0);
            radius->InsertNextTuple1(p.radius);
            ids.push_back(offset + s);
          }
          grid->InsertNextCell(VTK_POLYGON, circleSegments, ids.data());
        } else {
          for (int v = 0; v < 3; ++v) {
            points->InsertNextPoint(p.worldVertices[v].x, p.worldVertices[v].y, 0.0);
            velocity->InsertNextTuple3(p.velocity.x, p.velocity.y, 0.0);
            radius->InsertNextTuple1(p.boundingRadius);
          }
          vtkIdType ids[3] = { offset, offset + 1, offset + 2 };
          grid->InsertNextCell(VTK_TRIANGLE, 3, ids);
        }

        globalId->InsertNextTuple1(p.id);
        shape->InsertNextTuple1(p.shape == ShapeType::Circle ? 0 : 1);
        angularVelocity->InsertNextTuple1(p.angularVelocity);
        mass->InsertNextTuple1(p.mass);
        obstacle->InsertNextTuple1(p.isObstacle ? 1 : 0);
      }

      grid->SetPoints(points);
      grid->GetPointData()->AddArray(velocity);
      grid->GetPointData()->AddArray(radius);
      grid->GetPointData()->SetActiveScalars("radius");
      grid->GetCellData()->AddArray(globalId);
      grid->GetCellData()->AddArray(shape);
      grid->GetCellData()->AddArray(angularVelocity);
      grid->GetCellData()->AddArray(mass);
      grid->GetCellData()->AddArray(obstacle);

      auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
      writer->SetFileName(filename.c_str());
      writer->SetInputData(grid);
      writer->Write();

      writeDem2DPVD(path);
    }

  }
}
