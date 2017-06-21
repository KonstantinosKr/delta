/*
 The MIT License (MIT)

 Copyright (c) 2015 Konstantinos Krestenitis

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

#include <vector>
#include <array>
#include "delta/geometry/material.h"
#include "delta/geometry/properties.h"
#include "delta/geometry/granulates.h"
#include "delta/geometry/graphite.h"
#include "stdlib.h"
#include <string>

namespace delta {
  namespace world {
    namespace assembly {
  	  iREAL getDiscritization(iREAL length, int number);

  	  std::vector<std::array<iREAL, 3>> array1d(iREAL position[3], iREAL xAxisLength, int partsNo);

  	  std::vector<std::array<iREAL, 3>> array2d(iREAL position[3], iREAL xyAxisLength, int partsNo);

  	  std::vector<std::array<iREAL, 3>> array3d(iREAL position[3], iREAL xyzAxisLength, int partsNo);

  	  std::vector<std::array<iREAL, 3>> array3d(iREAL position[3], iREAL xyzAxisLength, int partsXYZNo, iREAL yAxisLength, int partsYNo);

  	  std::vector<std::array<iREAL, 3>> getGridArrayList(iREAL position[3], int xzcuts, int ycuts, iREAL width);

  	  void collapseUniformGrid(iREAL position[3], std::vector<std::array<iREAL, 3>>& grid, int xzcuts, int ycuts, iREAL elementWidth, iREAL elementHeight, iREAL epsilon);

  	  void uniform(
  	      iREAL totalMass,
  	      delta::geometry::material::MaterialType material,
  	      bool isSphereOrNone,
  	      int noPointsPerParticle,
  	      std::vector<iREAL>  &rad,
  	      std::vector<std::array<iREAL, 3>> &particleGrid,
  	      std::vector<std::string> &componentGrid,
  	      iREAL &minParticleDiam,
  	      iREAL &maxParticleDiam,
  	      std::vector<std::vector<iREAL>>  &xCoordinatesArray,
  	      std::vector<std::vector<iREAL>>  &yCoordinatesArray,
  	      std::vector<std::vector<iREAL>>  &zCoordinatesArray);

  	  void nonuniform (
  	      iREAL totalMass,
  	      delta::geometry::material::MaterialType material,
  	      bool isSphereOrNone,
  	      iREAL subcellx,
  	      int noPointsPerParticle,
  	      std::vector<iREAL>  &rad,
  	      std::vector<std::array<iREAL, 3>> &particleGrid,
  	      std::vector<std::string> &componentGrid,
  	      iREAL &minParticleDiam, iREAL &maxParticleDiam,
  	      std::vector<std::vector<iREAL>>  &xCoordinatesArray,
  	      std::vector<std::vector<iREAL>>  &yCoordinatesArray,
  	      std::vector<std::vector<iREAL>>  &zCoordinatesArray);

  	  void uniSphereRadius(
  	      iREAL totalMass,
          delta::geometry::material::MaterialType material,
          std::vector<iREAL>  &rad,
          std::vector<std::array<iREAL, 3>> &particleGrid,
          std::vector<std::string> &componentGrid,
          iREAL &minParticleDiam, iREAL &maxParticleDiam);

  	  void uniMeshGeometry(
  	      iREAL totalMass,
          delta::geometry::material::MaterialType material,
          int noPointsPerParticle,
          std::vector<iREAL>  &rad,
          std::vector<std::array<iREAL, 3>> &particleGrid,
          std::vector<std::string> &componentGrid,
          iREAL &minParticleDiam, iREAL &maxParticleDiam,
          std::vector<std::vector<iREAL>>  &xCoordinatesArray,
          std::vector<std::vector<iREAL>>  &yCoordinatesArray,
          std::vector<std::vector<iREAL>>  &zCoordinatesArray);

  	  void nonUniSphereRadius(
  	      iREAL totalMass,
          delta::geometry::material::MaterialType material,
          iREAL subcellx,
          std::vector<iREAL>  &rad,
          std::vector<std::array<iREAL, 3>> &particleGrid,
          std::vector<std::string> &componentGrid,
          iREAL &minParticleDiam, iREAL &maxParticleDiam);


  	  void nonUniMeshGeometry(
  	      iREAL totalMass,
          delta::geometry::material::MaterialType material,
          iREAL subcellx,
          int noPointsPerParticle,
          std::vector<iREAL>  &rad,
          std::vector<std::array<iREAL, 3>> &particleGrid,
          std::vector<std::string> &componentGrid,
          iREAL &minParticleDiam, iREAL &maxParticleDiam,
          std::vector<std::vector<iREAL>>  &xCoordinatesArray,
          std::vector<std::vector<iREAL>>  &yCoordinatesArray,
          std::vector<std::vector<iREAL>>  &zCoordinatesArray);

  	  void loadNuclearGeometry(iREAL position[3],
  	      iREAL width,
  	      int layers,
  	      std::vector<std::array<iREAL, 3>> &particleGrid,
  	      std::vector<std::string> &componentGrid,
  	      std::vector<iREAL> &radius,
  	      iREAL &minParticleDiam, iREAL &maxParticleDiam);

  	  void makeBrickGrid(
  	      iREAL position[3],
  	      iREAL arrayXZlength,
  	      int   xzElements,
  	      iREAL arrayYlength,
  	      int   yElements,
  	      std::vector<std::array<iREAL, 3>> &particleGrid,
  	      std::vector<std::string> &componentGrid,
  	      std::vector<iREAL> &radius,
  	      iREAL &minParticleDiam,
  	      iREAL &maxParticleDiam);
    }
  }
}
