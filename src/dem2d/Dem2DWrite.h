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

#ifndef DELTA_DEM2D_WRITE_H_
#define DELTA_DEM2D_WRITE_H_

#include <string>
#include <vector>

#include "Dem2D.h"

namespace delta {
  namespace dem2d {

/*
 * Write one frame of the 2D scene as a ParaView XML unstructured grid, and
 * rewrite the time series collection beside it.
 *
 * Mirrors the 3D convention by filling a delta::core::io::MeshBuffer and
 * calling delta::core::io::writeVTU, so the file format lives in one place:
 * every particle becomes real filled geometry, not a point cloud.
 *
 *   disk      -> VTK_POLYGON (tessellated ring, in the z=0 plane)
 *   triangle  -> VTK_TRIANGLE, taken from worldVertices
 *
 * Per-point: velocity (z=0), radius.  Per-cell: global_id, shape,
 * angular_velocity, mass, obstacle.  worldVertices must be current, which
 * the engine guarantees after a step.
 */
void writeDem2DToVTK(const std::string& path, int step,
                     const std::vector<Particle2D>& particles);

    }
  }

#endif /* DELTA_DEM2D_WRITE_H_ */
