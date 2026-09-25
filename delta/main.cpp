#include <stdio.h>
#include <cmath>
#include <fstream>
#include <random>
#include <string>
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
 * Target scenario is the real hopper file (input/hopper.mbfcp): 15625 spheres
 * in a 25^3 lattice plus 26 obstacle walls. makeSpherePile() stays as the
 * fallback when the file is not where the binary expects it.
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
	std::array<iREAL, 3> gravity = {0.0, 0.0, 0.0};

	const std::string hopperFile = delta::core::io::inputPath("hopper.mbfcp");
	std::ifstream hopper(hopperFile);
	if(hopper.good())
	{
		hopper.close();
		//The mbfcp file carries no contact epsilon. The contact model's rest gap
		//is epsilonA+epsilonB, so epsilon must stay well below the 0.02 radius:
		//the hopper is a 0.04-spaced (tangent) lattice, and a large epsilon makes
		//that lattice start 2*epsilon compressed, releasing the springs in one
		//step. 0.002 keeps the reach (0.044) above the 0.04 neighbour spacing
		//while staying a tenth of a radius (6 face-neighbours per sphere).
		epsilon = 0.002;
		gravity = delta::core::io::readmbfcp(hopperFile, particles, epsilon);
	}
	else
	{
		particles = makeSpherePile(10, 0.02, epsilon);
	}

	//A zero mass makes deriveForces() divide by zero, so fail loudly here.
	int obstacles = 0;
	iREAL minMass = 1E99;
	for(size_t i=0; i<particles.size(); i++)
	{
		if(particles[i].getIsObstacle()) {obstacles++;}
		iREAL mass = particles[i].getMass();
		if(!(mass > 0.0) || !std::isfinite(mass))
		{
			std::cerr << "particle " << i << " has mass " << mass << std::endl;
			return 1;
		}
		if(mass < minMass) {minMass = mass;}
	}

	std::cout << "loaded particles=" << particles.size()
			  << " spheres=" << (particles.size()-obstacles)
			  << " obstacles=" << obstacles
			  << " minMass=" << minMass << std::endl;

	/////////////////////////////////////////////////////////////////////////
	delta::core::data::Meta::Simulation simMeta;

	simMeta.plotScheme = delta::core::data::Meta::Plot::Never;
	simMeta.modelScheme = delta::core::data::Meta::CollisionModel::Sphere;
	simMeta.overlapPreCheck = false;
	//Explicit Euler with SSPRING = 2e5 and ~100 contacts per sphere: the
	//per-particle stiffness sets dt <~ 1e-5, above that the pile blows up (still
	//finite, just wrong).
	simMeta.dt = 0.00001;
	simMeta.gravity = (gravity[0] != 0.0 || gravity[1] != 0.0 || gravity[2] != 0.0);
	simMeta.gravityVector = gravity;
	simMeta.maxPrescribedRefinement = -1.0;
	simMeta.resolveContacts = true;

	std::array<iREAL, 6> boundary = {{ 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 }};

	auto _deltaEngine = delta::core::Engine(particles, boundary, simMeta);

	std::cout << "particles=" << particles.size() << std::endl;
	std::cout << "gravity=" << gravity[0] << " " << gravity[1] << " " << gravity[2] << std::endl;

	const int steps = 10;
	for (int ii = 0; ii < steps; ii++) {
		_deltaEngine.iterate();

		//Positions/velocities must stay finite; getMaxForceMagnitude() is the
		//max|force| is the per-step peak reported by deriveForces().
		bool finite = true;
		int badParticle = -1;
		const char* badField = "";
		iREAL maxSpeed = 0.0;
		std::vector<delta::core::data::ParticleRecord>& records = _deltaEngine.getParticleRecords();
		for(unsigned p=0; p<records.size(); p++)
		{
			delta::core::data::ParticleRecord& record = records[p];
			const char* field = "";
			for(int d=0; d<3; d++)
			{
				if(!std::isfinite(record._centre[d])) {field = "centre";}
				else if(!std::isfinite(record._linearVelocity[d])) {field = "linearVelocity";}
				else if(!std::isfinite(record._angularVelocity[d])) {field = "angularVelocity";}
			}
			if(field[0] != '\0' && badParticle < 0) {finite = false; badParticle = p; badField = field;}
			iREAL speed = std::sqrt(record._linearVelocity[0]*record._linearVelocity[0]+record._linearVelocity[1]*record._linearVelocity[1]+record._linearVelocity[2]*record._linearVelocity[2]);
			if(std::isfinite(speed) && speed > maxSpeed) {maxSpeed = speed;}
		}

		std::cout << "step " << ii
			 << " contacts=" << _deltaEngine.getContactPairs().size()
			 << " max|v|=" << maxSpeed
			 << " max|force|(per-step)=" << _deltaEngine.getMaxForceMagnitude()
			 << " finite=" << (finite ? "yes" : "no");
		if(badParticle >= 0)
		{
			delta::core::data::ParticleRecord& record = records[badParticle];
			std::cout << " firstNonFinite=" << badParticle << ":" << badField
					  << " obstacle=" << record.getIsObstacle()
					  << " centre=" << record._centre[0] << "," << record._centre[1] << "," << record._centre[2]
					  << " linear=" << record._linearVelocity[0] << "," << record._linearVelocity[1] << "," << record._linearVelocity[2]
					  << " angular=" << record._angularVelocity[0] << "," << record._angularVelocity[1] << "," << record._angularVelocity[2];
		}
		std::cout << std::endl;

		if(!finite) {return 1;}
	}

	cout << "hello master" << endl;
}
