#include <stdio.h>
#include <cmath>
#include <random>
#include <vector>
#include <array>
#include <iostream>
#include "core/delta.h"
#include "core/Engine.h"
#include "core/io/read.h"
#include "core/io/write.h"
#include "core/data/Meta.h"
#include "geometry/material.h"
#include "world/scenario/scenarios.h"

using namespace std;

/*
 * Small serial DEM scenario: a dense lattice of jittered spheres whose centre
 * spacing is below the (2r + 2*epsilon) contact reach, so every particle has
 * neighbours and the contact loop actually has work to do.
 *
 * The 15625-sphere hopper file is still loadable (readmbfcp below) but it is
 * far too large to drive the serial broad phase for a few steps, so the demo
 * defaults to a 1000-sphere pile.
 */
static std::vector<delta::world::structure::Object> makeSpherePile(
	int side, iREAL radius, iREAL epsilon)
{
	std::vector<delta::world::structure::Object> particles;
	std::mt19937 rng(42);
	std::uniform_real_distribution<iREAL> jitter(-0.2*radius, 0.2*radius);

	// spacing < 2r + 2*epsilon -> touching; the residual overlap is the depth
	// the spring/dashpot contact model reacts to.
	iREAL spacing = 1.9 * (radius + epsilon);
	iREAL density = 20000.0; // matches geometry::material WOOD
	iREAL mass = density * (4.0/3.0) * 3.14159265358979323846 * radius*radius*radius;

	int id = 0;
	for(int x=0; x<side; x++)
	for(int y=0; y<side; y++)
	for(int z=0; z<side; z++)
	{
		std::array<iREAL,3> centre = {
			0.2 + x*spacing + jitter(rng),
			0.2 + y*spacing + jitter(rng),
			0.2 + z*spacing + jitter(rng)};
		std::array<iREAL,3> linear  = {0.0, 0.0, 0.0};
		std::array<iREAL,3> angular = {0.0, 0.0, 0.0};

		delta::world::structure::Object o(
			"sphere", radius, id++, centre,
			delta::geometry::material::MaterialType::WOOD,
			false, true, true, epsilon, linear, angular);
		o.setMass(mass);
		particles.push_back(o);
	}
	return particles;
}

int main(int argc, const char *argv[]) {

	delta::core::Delta delta;
	std::vector<delta::world::structure::Object> particles;

	bool isSphere = true;
	int meshDensity = 20;
	iREAL epsilon = 0.01;

	//delta::core::io::readmbfcp("../input/hopper.mbfcp", particles, epsilon);
	//delta::world::scenarios::twoParticlesCrashDiagonal(particles, isSphere, meshDensity, epsilon);
	particles = makeSpherePile(10, 0.02, epsilon);

	/////////////////////////////////////////////////////////////////////////
	delta::core::data::Meta::Simulation simMeta;

	simMeta.plotScheme = delta::core::data::Meta::Plot::Never;
	simMeta.modelScheme = delta::core::data::Meta::CollisionModel::Sphere;
	simMeta.overlapPreCheck = false;
	simMeta.dt = 0.0001;
	simMeta.gravity = true;
	simMeta.maxPrescribedRefinement = -1.0;
	simMeta.resolveContacts = true;

	std::array<iREAL, 6> boundary = {{ 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 }};

	auto _deltaEngine = delta::core::Engine(particles, boundary, simMeta);

	cout << "particles=" << particles.size() << endl;

	const int steps = 20;
	for (int ii = 0; ii < steps; ii++) {
		_deltaEngine.iterate();
		cout << "step " << ii
			 << " contacts=" << _deltaEngine.getContactPairs().size()
			 << " max|force|=" << _deltaEngine.getMaxForceMagnitude() << endl;
	}

	cout << "hello master" << endl;
}
