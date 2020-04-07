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

#include <world/World.h>
#include <core/Engine.h>

#define epsilon 1E-3

delta::world::World::World(
	std::string								scenario,
	bool 									gravity,
	delta::core::data::Meta::CollisionModel 	collisionModelID,
	iREAL 									meshDensity)
{
  _boundary 						= {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
  _gravity 						= gravity;
  _globalPrescribedEpsilon 		= epsilon;
  _globalPrescribedMeshDensity 	= meshDensity;
  _scenario 						= scenario;
  _isSphere 						= false;
  _triangles	 					= 0;
  _maxParticleDiam 				= 0;
  _minParticleDiam 				= 1E99;

  if(collisionModelID == delta::core::data::Meta::CollisionModel::Sphere)
  {
	_isSphere = true;
  }

  delta::world::World::createWorld();

  std::array<iREAL,3> globalMin = {1E99, 1E99, 1E99};
  std::array<iREAL,3> globalMax = {0.0, 0.0, 0.0};
  std::array<iREAL,3> min, max;

  if(!_isSphere)
  {
	for(int i=0; i<_particles.size(); i++)
	{
	  _triangles += _particles[i].getNumberOfTriangles();

	  min = _particles[i].getMesh().getBoundaryMinVertex();
	  max = _particles[i].getMesh().getBoundaryMaxVertex();

	  if(min[0] < globalMin[0]) globalMin[0] = min[0];
	  if(min[1] < globalMin[1]) globalMin[1] = min[1];
	  if(min[2] < globalMin[2]) globalMin[2] = min[2];

	  if(max[0] > globalMax[0]) globalMin[0] = max[0];
	  if(max[1] > globalMax[1]) globalMin[1] = max[1];
	  if(max[2] > globalMax[2]) globalMin[2] = max[2];

	  if(_particles[i].getRad() * 2.0 < _minParticleDiam)
		_minParticleDiam = _particles[i].getRad() * 2.0;
	  if(_particles[i].getRad() * 2.0 > _maxParticleDiam)
		_maxParticleDiam = _particles[i].getRad() * 2.0;
	}
  } else
  {
	for(int i=0; i<_particles.size(); i++)
	{
	  min = {	_particles[i].getCentre()[0] - _particles[i].getRad(),
				_particles[i].getCentre()[1] - _particles[i].getRad(),
				_particles[i].getCentre()[2] - _particles[i].getRad()};

	  max = {	_particles[i].getCentre()[0] + _particles[i].getRad(),
				_particles[i].getCentre()[1] + _particles[i].getRad(),
				_particles[i].getCentre()[2] + _particles[i].getRad()};

	  if(min[0] < globalMin[0]) globalMin[0] = min[0];
	  if(min[1] < globalMin[1]) globalMin[1] = min[1];
	  if(min[2] < globalMin[2]) globalMin[2] = min[2];

	  if(max[0] > globalMax[0]) globalMin[0] = max[0];
	  if(max[1] > globalMax[1]) globalMin[1] = max[1];
	  if(max[2] > globalMax[2]) globalMin[2] = max[2];

	  if(_particles[i].getRad() * 2.0 < _minParticleDiam)
		_minParticleDiam = _particles[i].getRad() * 2.0;
	  if(_particles[i].getRad() * 2.0 > _maxParticleDiam)
		_maxParticleDiam = _particles[i].getRad() * 2.0;
	}
  }
  _boundary = {min[0], min[1], min[2], max[0], max[1], max[2]};
}

void delta::world::World::createWorld()
{
  if(_scenario == "two-particles-crash")
  {
    delta::world::scenarios::twoParticlesCrash(
    	_particles, _isSphere, _globalPrescribedMeshDensity, _globalPrescribedEpsilon);
  }
  else if(_scenario == "turbine")
  {
    delta::world::scenarios::turbine(
    	_particles, _globalPrescribedEpsilon);
  }
  else if(_scenario == "kaikoura")
  {
    delta::world::scenarios::kaikoura(
    	_particles, _globalPrescribedEpsilon);
  }
  else if(_scenario == "helicopter")
  {
	delta::world::scenarios::helicopter(
	_particles, _globalPrescribedEpsilon);
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

std::vector<delta::world::structure::Object> delta::world::World::getObjects()
{
  return _particles;
}

bool delta::world::World::getIsSphere()
{
  return _isSphere;
}

int delta::world::World::getGlobalPrescribedMeshDensity()
{
  return _globalPrescribedMeshDensity;
}

iREAL delta::world::World::getGlobalPrescribedEpsilon()
{
  return _globalPrescribedEpsilon;
}

delta::world::World::~World() {
  // TODO Auto-generated destructor stub
}
