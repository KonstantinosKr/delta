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
#include "delta/collision/material.h"
#include "delta/geometry/properties.h"
#include "delta/geometry/granulates.h"
#include "delta/geometry/graphite.h"
#include "stdlib.h"
#include <string>

namespace delta {
  namespace world {
    namespace assembly {
  	  double getxDiscritizationLength(double length, int number);

  	  std::vector<std::array<double, 3>> array1d(double position[3], double xAxisLength, int partsNo);

  	  std::vector<std::array<double, 3>> array2d(double position[3], double xyAxisLength, int partsNo);

  	  std::vector<std::array<double, 3>> array3d(double position[3], double xyzAxisLength, int partsNo);

  	  std::vector<std::array<double, 3>> array3d(double position[3], double xyzAxisLength, int partsXYZNo, double yAxisLength, int partsYNo);

  	  std::vector<std::array<double, 3>> getGridArrayList(double position[3], int xzcuts, int ycuts, double width);

  	  void uniSphereRadius(double totalMass,
  	                        delta::collision::material::MaterialType material,
  	                        std::vector<double>  &rad,
  	                        std::vector<std::array<double, 3>> &particleGrid,
  	                        std::vector<std::string> &componentGrid,
  	                        double &minParticleDiam, double &maxParticleDiam);

  	  void uniMeshGeometry(double totalMass,
  	                        delta::collision::material::MaterialType material,
  	                        int noPointsPerParticle,
                            std::vector<double>  &rad,
                            std::vector<std::array<double, 3>> &particleGrid,
                            std::vector<std::string> &componentGrid,
                            double &minParticleDiam, double &maxParticleDiam,
                            std::vector<std::vector<double>>  &xCoordinatesArray,
                            std::vector<std::vector<double>>  &yCoordinatesArray,
                            std::vector<std::vector<double>>  &zCoordinatesArray);

  	  void nonUniSphereRadius(double totalMass,
                              delta::collision::material::MaterialType material,
                              double subcellx,
                              std::vector<double>  &rad,
                              std::vector<std::array<double, 3>> &particleGrid,
                              std::vector<std::string> &componentGrid,
                              double &minParticleDiam, double &maxParticleDiam);


  	  void nonUniMeshGeometry(double totalMass,
  	                          delta::collision::material::MaterialType material,
                              double subcellx,
                              int noPointsPerParticle,
                              std::vector<double>  &rad,
                              std::vector<std::array<double, 3>> &particleGrid,
                              std::vector<std::string> &componentGrid,
                              double &minParticleDiam, double &maxParticleDiam,
                              std::vector<std::vector<double>>  &xCoordinatesArray,
                              std::vector<std::vector<double>>  &yCoordinatesArray,
                              std::vector<std::vector<double>>  &zCoordinatesArray);

  	  void makeLoadNuclearGeometry(double position[3],
  	      std::vector<std::array<double, 3>> &particleGrid,
  	      std::vector<std::string> &componentGrid,
  	      std::vector<double> &radius,
  	      double &minParticleDiam, double &maxParticleDiam);

  	  void makeFullBrickFBGrid(double position[3],
  	      double length,
  	      double elements,
  	      std::vector<std::array<double, 3>> &particleGrid,
  	      std::vector<std::string> &componentGrid,
  	      std::vector<double> &radius,
  	      double &minParticleDiam, double &maxParticleDiam);
    }
  }
 }
