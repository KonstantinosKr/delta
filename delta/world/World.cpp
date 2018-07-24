/*
 The MIT License (MIT)

 Copyright (c) 2018 Konstantinos Krestenitis

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

#include <delta/world/World.h>
#include <delta/core/Engine.h>

#define epsilon 1E-3

delta::world::World::World(std::string scenario, bool gravity, delta::core::data::Meta::CollisionModel collisionModelID, iREAL meshDensity) {

  _boundary = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};

  _gravity = gravity;
  _epsilon = epsilon;
  _meshDensity = meshDensity;
  _isSphere = false;

  if(scenario == "two-particles-crash")
  {
    delta::world::scenarios::twoParticlesCrash(_particles, _isSphere, _meshDensity, epsilon);
  }
  else if(scenario == "turbine")
  {
    delta::world::scenarios::turbine(_particles, epsilon);
  }
  else if(scenario == "kaikoura")
  {
    delta::world::scenarios::kaikoura(_particles, epsilon);
  }
  else if(scenario == "helicopter")
  {
	delta::world::scenarios::helicopter(_particles, epsilon);
  }

  _triangles = 0;

  if(!_isSphere)
  for(int i=0; i<_particles.size(); i++)
  {
	_triangles += _particles[i].getNumberOfTriangles();
  }
}

bool delta::world::World::hasGravity()
{
  return _gravity;
}

std::array<iREAL, 6> delta::world::World::getBoundary()
{
  return _boundary;
}

int delta::world::World::getNumberOfTriangles()
{
  return _triangles;
}

std::vector<delta::geometry::Object> delta::world::World::getObjects()
{
  return _particles;
}

bool delta::world::World::getIsSphere()
{
  return _isSphere;
}

int delta::world::World::getMeshDensity()
{
  return _meshDensity;
}

iREAL delta::world::World::getEpsilon()
{
  return _epsilon;
}

delta::world::World::~World() {
  // TODO Auto-generated destructor stub
}
