/*
 The MIT License (MIT)

 Copyright (c) 10 Mar 2017 Konstantinos Krestenitis

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

#include <delta/core/delta.h>
#include <delta/core/io/write.h>
#include <delta/core/io/read.h>

delta::core::Delta::Delta()
{
  printf("init material\n");
  delta::geometry::material::materialInit();
/*
  delta::geometry::mesh::Mesh *mesh = delta::core::io::readVTKGeometry("input/turbine.stl");

  std::vector<delta::geometry::Object> objects;
  std::array<iREAL, 3> 					centre 	= {0.0, 0.0, 0.0};
  std::array<iREAL, 3> 					linear 	= {0.0, 0.0, 0.0};
  std::array<iREAL, 3> 					angular 	= {0.0, 0.0, 0.0};

  delta::geometry::Object object("mesh",
								0,
								mesh,
								centre,
								delta::geometry::material::MaterialType::WOOD,
								false,
								false,
								true,
								0.0001,
								linear,
								angular);

  objects.push_back(object);
  //printf("mesh:%i\n",object->getMesh().getTriangleFaces().size());
  //printf("mesh:%i\n", meshes[0].getTriangleFaces().size());

  //printf("mesh:%i\n", meshes[0].getTriangleFaces().size());
  std::array<iREAL, 6> boundary = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
  delta::core::io::writeGeometryToVTK(0, boundary, objects);
  */
}

delta::core::Delta::~Delta()
{

}
