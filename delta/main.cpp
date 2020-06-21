#include <stdio.h>
#include "core/delta.h"
#include "core/Engine.h"
#include "core/io/read.h"
#include "core/data/Meta.h"
#include "world/scenario/scenarios.h"

using namespace std;

int main(int argc, const char *argv[]) {

	delta::core::Delta delta;
	delta::core::io::readmbfcp("../inp/hopper.mbfcp");

	std::vector<delta::world::structure::Object> particles;
	bool isSphere = true;
	int meshDensity = 20;
	iREAL epsilon = 0.038;

	delta::world::scenarios::twoParticlesCrashDiagonal(
			particles, isSphere,
			meshDensity, epsilon);

	delta::core::data::Meta::EngineMeta engineMeta;

	engineMeta.plotScheme = delta::core::data::Meta::Plot::EveryIteration;
	engineMeta.modelScheme = delta::core::data::Meta::CollisionModel::Sphere;
	engineMeta.overlapPreCheck = false;
	engineMeta.dt = 0.11;
	engineMeta.gravity = false;
	engineMeta.maxPrescribedRefinement = -1.0;

	std::array<iREAL, 6> boundary = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 };

	auto _deltaEngine = delta::core::Engine(particles, boundary, engineMeta);

	for (int ii = 0; ii < 10; ii++) {
		_deltaEngine.iterate();
	}

	cout << "hello master" << endl;
}
