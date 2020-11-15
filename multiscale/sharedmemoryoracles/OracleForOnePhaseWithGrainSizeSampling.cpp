#include "sharedmemoryoracles/OracleForOnePhaseWithGrainSizeSampling.h"
#include "tarch/Assertions.h"
#include "peano/utils/Globals.h"

#include <cstdlib>
#include <math.h>
#include <fstream>


tarch::logging::Log  sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::_log( "sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling" );
const double         sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::ExactValue( 1e-8 );


sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::OracleForOnePhaseWithGrainSizeSampling(
  int numberOfSamples, bool logarithmicDistribution, int adapterNumber
):
  _executionTimes(),
  _currentBin(0),
  _numberOfSamples(numberOfSamples),
  _logarithmicDistribution(logarithmicDistribution) {
  assertion( _numberOfSamples>0 );
}


sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::~OracleForOnePhaseWithGrainSizeSampling() {
}


peano::datatraversal::autotuning::GrainSize sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::parallelise(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod) {
  logTraceInWith1Argument( "parallelise(...)", problemSize );

  if (_executionTimes.count(askingMethod)==0) {
    _executionTimes.insert( std::pair<peano::datatraversal::autotuning::MethodTrace, DataBaseEntry>(askingMethod,DataBaseEntry(_numberOfSamples,_logarithmicDistribution,problemSize)) );
  }

  _executionTimes[askingMethod].ensureThatRequestIsCoveredBySamplingRange(problemSize);

  return peano::datatraversal::autotuning::GrainSize(
    _executionTimes[askingMethod].getGrainSize(problemSize,_currentBin),
    _executionTimes[askingMethod].requiresMoreData(problemSize,_currentBin),
    problemSize,
    askingMethod,
    this
  );
}


void sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::parallelSectionHasTerminated(int problemSize, int grainSize, peano::datatraversal::autotuning::MethodTrace askingMethod, double costPerProblemElement) {
  assertion(_executionTimes.count(askingMethod)==1);
  _executionTimes[askingMethod].updateMeasurement(problemSize,_currentBin,costPerProblemElement);
}



void sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::loadStatistics(const std::string& filename, int oracleNumber) {
  logWarning( "loadStatistics(std::string)", "reload of statistics is not supported. Statistics file will be overwritten" );
}


void sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::plotStatistics(std::ostream& out, int oracleNumber) const {
  out << "begin OracleForOnePhaseWithGrainSizeSampling" << std::endl;
  out << "adapter-number=" << oracleNumber << std::endl
      << "number-of-samples=" << _numberOfSamples << std::endl
      << "logarithmic-distribution=" << _logarithmicDistribution << std::endl;

  for (auto measurement: _executionTimes) {
    out << peano::datatraversal::autotuning::toString(measurement.first)
        << "=" << measurement.second.toString() << std::endl;
  }
  out << "end OracleForOnePhaseWithGrainSizeSampling" << std::endl;
}


peano::datatraversal::autotuning::OracleForOnePhase* sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::createNewOracle() const {
  return new OracleForOnePhaseWithGrainSizeSampling(_numberOfSamples, _logarithmicDistribution);
}


sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::DataBaseEntry(int numberOfSamples, bool logarithmicDistribution, int problemSizeForFirstRequest):
  _numberOfSamples(numberOfSamples),
  _logarithmicDistribution(logarithmicDistribution),
  _measurements(std::min(_numberOfSamples,problemSizeForFirstRequest)),
  _maxProblemSize(problemSizeForFirstRequest) {
}


sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::DataBaseEntry():
  _numberOfSamples(-1),
  _logarithmicDistribution(false),
  _measurements(),
  _maxProblemSize(-1) {
}


void sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::ensureThatRequestIsCoveredBySamplingRange(int problemSize) {
  if (
    static_cast<int>(_measurements.size())<_numberOfSamples &&
    problemSize > static_cast<int>(_measurements.size())
  ) {
    assertion(_maxProblemSize<problemSize);
    _maxProblemSize = problemSize;
    _measurements.resize(_maxProblemSize);
    for (auto measurement: _measurements) {
      measurement = tarch::timing::Measurement(ExactValue);
    }
  }
}


int sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::getBin( int problemSize, int currentBin ) const {
  int bin = std::min( currentBin, static_cast<int>(_measurements.size()-1) );

  if ( getGrainSize(bin)>=problemSize ) {
    bin = 0;
  }

  return bin;
}


int sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::getGrainSize( int bin ) const {
  if (_logarithmicDistribution) {
    if (bin==0) return 0;
    int lastBinTried  = _numberOfSamples-1;
    int lastGrainSize = _maxProblemSize-1;
    while (lastBinTried>bin) {
      lastBinTried--;
      lastGrainSize /= 2;
    }
    return lastGrainSize;
  }
  else {
    return _maxProblemSize * bin / _numberOfSamples;
  }
}


int sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::getGrainSize(int problemSize,int currentBin) const {
  return getGrainSize( getBin(problemSize,currentBin) );
}


bool sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::requiresMoreData(int problemSize,int currentBin) const {
  return !_measurements[ getBin(problemSize,currentBin) ].isAccurateValue();
}


void sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::updateMeasurement(int problemSize, int currentBin, double value) {
  _measurements[getBin(problemSize,currentBin)].setValue(value);
}


std::string sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::DataBaseEntry::toString() const {
  std::ostringstream msg;
  msg << "(max-problem-size=" << _maxProblemSize;
  for (int i=0; i<static_cast<int>(_measurements.size()); i++) {
    msg << ",bin(" << i << ","
        << getGrainSize(i) << ")="
        << _measurements[i].toString();
  }
  msg << ")";
  return msg.str();
}


void sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::deactivateOracle() {
}


void sharedmemoryoracles::OracleForOnePhaseWithGrainSizeSampling::activateOracle() {
  _currentBin++;
  if (_currentBin>=_numberOfSamples) _currentBin = 0;
}
