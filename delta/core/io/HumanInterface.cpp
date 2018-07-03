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

#include "delta/core/io/HumanInterface.h"

delta::core::io::HumanInterface::HumanInterface(int argc, const char* argv[]) {
  int NumberOfArguments = 7;
  if(argc!=NumberOfArguments)
  {
    printManualDefault();
    //printManualPeano();
  }

  _scenario        	= argv[1];
  _steps           	= atoi(argv[2]);
  _dt           		= atof(argv[3]);
  const std::string plotID        	= argv[4];
  const std::string collisionModelID	= argv[5];
  _meshDensity   	= atof(argv[6]);

  setPlotScheme(plotID);
  setCollisionScheme(collisionModelID);

  _engineMeta.plotScheme = _plotScheme;
  _engineMeta.modelScheme = _collisionModel;
  _engineMeta.dt = _dt;
  _engineMeta.overlapPreCheck = _dt;
}

void delta::core::io::HumanInterface::setPlotScheme(std::string plotID)
{
  if (plotID=="never") {
	_plotScheme = delta::core::data::Meta::Plot::Never;
  }
  else if(plotID=="every-iteration") {
	_plotScheme = delta::core::data::Meta::Plot::EveryIteration;
  }
  else if(plotID=="upon-change") {
	_plotScheme = delta::core::data::Meta::Plot::UponChange;
  }
  else if(plotID=="every-batch") {
	_plotScheme = delta::core::data::Meta::Plot::EveryBatch;
  }
  else if(plotID=="adaptive") {
	_plotScheme = delta::core::data::Meta::Plot::Adaptive;
  }
  else if(plotID=="range") {
	_plotScheme = delta::core::data::Meta::Plot::Range;
  }else {
    std::cerr << "invalid plot option: " << "Please run without arguments to see list of valid plot variants" << std::endl;
  }
}

delta::core::data::Meta::Plot delta::core::io::HumanInterface::getPlotScheme()
{
  return _plotScheme;
}

void delta::core::io::HumanInterface::setCollisionScheme(std::string collisionModelID)
{
  _overlapPreCheck = false;
  if (collisionModelID=="sphere" || collisionModelID=="sphere-sphere") {
	_collisionModel = delta::core::data::Meta::CollisionModel::Sphere;
    if(collisionModelID=="sphere-sphere") {_overlapPreCheck = true;}
  }
  else if((collisionModelID=="bf") || (collisionModelID=="sphere-bf"))
  {
	_collisionModel = delta::core::data::Meta::CollisionModel::BruteForce;
    if(collisionModelID=="sphere-bf") {_overlapPreCheck = true;}
  }
  else if((collisionModelID=="penalty") || (collisionModelID=="sphere-penalty"))
  {
	_collisionModel = delta::core::data::Meta::CollisionModel::Penalty;
    if(collisionModelID=="sphere-penalty") {_overlapPreCheck = true;}
  }
  //penalty iterations histogram
  //penalty fail rate - maybe no need for this one
  else if (collisionModelID=="penalty-stat") {
	_collisionModel = delta::core::data::Meta::CollisionModel::PenaltyStat;
  }
  else if (collisionModelID=="penalty-tune") {
	_collisionModel = delta::core::data::Meta::CollisionModel::PenaltyTune;
  }
  else if((collisionModelID=="hybrid-on-triangle-pairs") || (collisionModelID=="sphere-hybrid-on-triangle-pairs"))
  {
	_collisionModel = delta::core::data::Meta::CollisionModel::HybridOnTrianglePairs;
    if(collisionModelID=="sphere-hybrid-on-triangle-pairs") {_overlapPreCheck = true;}
  }
  else if ((collisionModelID=="hybrid-on-batches") || (collisionModelID=="sphere-hybrid-on-batches"))
  {
	_collisionModel = delta::core::data::Meta::CollisionModel::HybridOnBatches;
    if(collisionModelID=="sphere-hybrid-on-batches") {_overlapPreCheck = true;}
  }
  //hybrid on triangle pairs fail rate histogram
  //hybrid on batches fail rate histogram
  else if (collisionModelID=="hybrid-on-triangle-pairs-stat")
  {
	_collisionModel = delta::core::data::Meta::CollisionModel::HybridTriangleStat;
  }
  else if (collisionModelID=="hybrid-on-batches-stat")
  {
	_collisionModel = delta::core::data::Meta::CollisionModel::HybridBatchStat;
  }
  else if (collisionModelID=="gjk") {
	_collisionModel = delta::core::data::Meta::CollisionModel::GJK;
  }
  else if (collisionModelID=="none") {
	_collisionModel = delta::core::data::Meta::CollisionModel::none;
  } else {
    std::cerr << "invalid collision model option. Please run without arguments to see list of valid variants" << std::endl;
  }
}

delta::core::data::Meta::CollisionModel delta::core::io::HumanInterface::getCollisionScheme()
{
  return _collisionModel;
}

void delta::core::io::HumanInterface::printManualDefault()
{
  std::cerr
    << "##################################################################################################" << std::endl
    << "#  Delta Library - Konstantinos Krestenitis" << std::endl
    << "##################################################################################################" << std::endl
    << "#  A Grid-based Non-Spherical Particle Dynamics Library" << std::endl
    << "##################################################################################################" << std::endl
    << "Option Type---------Short Description" << std::endl
    << "scenario            which scenario to use - see list below for valid configurations " << std::endl
    << "steps		    		time steps" << std::endl
    << "step-size           triggers static step size" << std::endl
    << "plot                see plot variants below" << std::endl
    << "collision-model     choose: sphere, bf, penalty, hybrid-on-triangle-pairs, hybrid-on-batches, gjk, add sphere- for sphere check" << std::endl
    << "mesh-density        triangles used to represent one particle" << std::endl
    << std::endl << std::endl << std::endl << std::endl
    << "Usage: ./delta-xxx scenario(string) steps(int) step-size(int) plot(string) collision-model(string) mesh-density(int)" << std::endl  << std::endl
    << "eg: ./delta-icc-release two-particles-crash 10 0.00001 never sphere 20" << std::endl;
}

void printManualPeano()
{
	std::cerr
	  << "##################################################################################################" << std::endl
	  << "#  Delta Library - Peano" << std::endl
	  << "##################################################################################################" << std::endl
	  << "#  A Grid-based Non-Spherical Particle Dynamics Library" << std::endl
	  << "##################################################################################################" << std::endl
	  << " Usage: ./dem-xxx grid_h_max(iREAL) scenario(string) iterations(int) grid-type(string) step-size(int) plot(string) snapshot-frequency(iREAL in seconds) collision-model(string) mesh-density-per-particle(int) [tbb-core-count](int)" << std::endl  << std::endl
	  << " Option Type---------Short Description" << std::endl
	  << " grid_h_max          maximum mesh width of grid" << std::endl
	  << " scenario            which scenario to use. See list below for valid configurations " << std::endl
	  << " iterations          number of iterations or time steps depending on scheme" << std::endl
	  << " grid-type           which grid type to use. See list below for valid configurations " << std::endl
	  << " step-size           negative value trigger adaptive step size with a positive initial step size | positive value triggers static step size" << std::endl
	  << " plot                see plot variants below" << std::endl
	  << " collision-model     choose from none, sphere, bf, penalty, hybrid-on-triangle-pairs, hybrid-on-batches, gjk, add sphere- for sphere check" << std::endl
	  << " max-step-size       adaptive max step" << std::endl
	  << " triangles-per-particle    triangles used to represent one particle" << std::endl
	  << " [core-count]        only required in TBB shared memory" << std::endl
    << " [run-grid-parallel] only required in TBB shared memory" << std::endl
    << " [run-particles-parallel] only required in TBB shared memory" << std::endl
    << " [run-comparisons-in-background] only required in TBB shared memory. Either off or a positive number" << std::endl
	  << std::endl << std::endl << std::endl << std::endl

	  << " OPTIONS: "  << std::endl << std::endl
	  << " Grid H max (iREAL)" << std::endl
	  << " =========" << std::endl
	  << " 0.0 - 0.5" << std::endl

	  << " Scenarios (string)" << std::endl
	  << " =========" << std::endl
	  << "  none" << std::endl
	  << "  blackholeWithRandomlyOrientedCubes" << std::endl
	  << "  blackholeWithCubes" << std::endl
	  << "  blackholeWithGranulates" << std::endl

	  << "  freefallWithRandomlyOrientedCubes" << std::endl
	  << "  freefallWithCubes" << std::endl
	  << "  freefallWiithGranulates" << std::endl

	  << "  two-particles-crash" << std::endl

	  << "  SLA"<< std::endl
	  << "  DLA"<< std::endl
	  << "  nuclearDeck4" << std::endl
	  << "  nuclearDeck32" << std::endl
	  << "  nuclearDeck64" << std::endl
	  << "  nuclearDeck256" << std::endl

	  << "  hopperUniform" << std::endl
	  << "  hopperUniform1k" << std::endl
	  << "  hopperUniform10k" << std::endl
	  << "  hopperUniform100k" << std::endl
	  << "  hopperUniform500k" << std::endl

	  << "  hopperNonUniform" << std::endl
	  << "  hopperNonUniform1k" << std::endl
	  << "  hopperNonUniform10k" << std::endl
	  << "  hopperNonUniform100k" << std::endl
	  << "  hopperNonUniform500k" << std::endl

	  << "  frictionStatic" << std::endl
	  << "  frictionSlide" << std::endl
	  << "  frictionRoll" << std::endl << std::endl

    << " Iterations (int)" << std::endl
    << " =========" << std::endl
    << " 0 - infinity" << std::endl << std::endl

	  << " Grid types (string)" << std::endl
	  << " ==========" << std::endl
	  << "  no-grid" << std::endl
	  << "  regular-grid" << std::endl
	  << "  adaptive-grid" << std::endl
	  << "  reluctant-grid" << std::endl << std::endl

    << " Step-size (iREAL)" << std::endl
    << " =========" << std::endl
    << " -infinity - infinity" << std::endl << std::endl

	  << " Plot variants (string)" << std::endl
	  << " =============" << std::endl
	  << "  never" << std::endl
	  << "  every-iteration" << std::endl
	  << "  upon-change" << std::endl
	  << "  every-batch" << std::endl
	  << "  every-checkpoint" << std::endl
	  << "  range" << std::endl << std::endl

    << " snapshot-frequency (iREAL)" << std::endl
    << " =========" << std::endl
    << " 0.0 - infinity" << std::endl << std::endl

    << " Collision-model (string)" << std::endl
    << " =========" << std::endl
    << " sphere" << std::endl
    << " bf" << std::endl
    << " penalty" << std::endl
    << " penaltyStat" << std::endl
    << " hybrid-on-triangle-pairs" << std::endl
    << " hybrid-on-batches" << std::endl
    << " hybridStat" << std::endl
    << " gjk" << std::endl
    << " none" << std::endl << std::endl

    << " Mesh-density [if mesh is used] (int)" << std::endl
    << " =========" << std::endl
    << " 0 - infinity" << std::endl << std::endl

    << " tbb-core-count (int)" << std::endl
    << " =========" << std::endl
    << " 0 - infinity" << std::endl << std::endl

	<< " Usage: ./dem-xxx grid_h_max(iREAL) scenario(string) iterations(int) grid-type(string) step-size(int) plot(string) snapshot-frequency(iREAL in seconds) collision-model(string) mesh-density-per-particle(int) [tbb-core-count](int)" << std::endl
	<< " eg: ./delta-icc-release-tbb 0.1 [grid] hopperUniform [scenario] 10 [steps] regular-grid [grid] 0.0001 [step-size] never [plot] 1 [snap] sphere [collision] 10 [mesh] 3 [cores] true [grid-para] true [particle-para] auto [tbb number] false [autotune]" << std::endl
	<< " eg: ./delta-icc-release-tbb 0.1 hopperUniform 10 regular-grid 0.0001 never 1 sphere 10 3 true true auto false" << std::endl;
}

delta::core::data::Meta::EngineMeta delta::core::io::HumanInterface::getEngineMeta()
{
  return _engineMeta;
}

std::string delta::core::io::HumanInterface::getScenario()
{
  return _scenario;
}

int delta::core::io::HumanInterface::getSteps()
{
  return _steps;
}

iREAL delta::core::io::HumanInterface::getStepSize()
{
  return _dt;
}

int delta::core::io::HumanInterface::getMeshDensity()
{
  return _meshDensity;
}

delta::core::io::HumanInterface::~HumanInterface() {
  // TODO Auto-generated destructor stub
}
