#include <stdio.h>
#include "core/delta.h"
#include "core/io/read.h"
#include "world/scenario/scenarios.h"

using namespace std;

int main(int argc, const char* argv[]) {
	
	delta::core::Delta delta;
	delta::core::io::readmbfcp("../inp/hopper.mbfcp");


	std::vector<delta::world::structure::Object>	coarseObjects;
	bool isSphere = true;
	int meshDensity = 20;
	iREAL epsilon = 1E-04;

	delta::world::scenarios::twoParticlesCrashDiagonal(coarseObjects,
																										isSphere,
																										meshDensity,
																										epsilon);
	
	delta.run()
		
	cout<< "hello master" << endl;
	
}
