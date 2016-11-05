#include "dem/Vertex.h"
#include "peano/utils/Loop.h"
#include "peano/grid/Checkpoint.h"

#define INVERT(F, INV, DET)\
if (((DET) =\
 ((F)[0]*((F)[4]*(F)[8]-(F)[5]*(F)[7])+\
  (F)[3]*((F)[2]*(F)[7]-(F)[1]*(F)[8])+\
 ((F)[1]*(F)[5]-(F)[2]*(F)[4])*(F)[6])) != 0.0)\
{\
  (DET) = 1.0 / (DET);\
  (INV) [0] = ((F)[4]*(F)[8]-(F)[5]*(F)[7])*(DET);\
  (INV) [1] = ((F)[2]*(F)[7]-(F)[1]*(F)[8])*(DET);\
  (INV) [2] = ((F)[1]*(F)[5]-(F)[2]*(F)[4])*(DET);\
  (INV) [3] = ((F)[5]*(F)[6]-(F)[3]*(F)[8])*(DET);\
  (INV) [4] = ((F)[0]*(F)[8]-(F)[2]*(F)[6])*(DET);\
  (INV) [5] = ((F)[2]*(F)[3]-(F)[0]*(F)[5])*(DET);\
  (INV) [6] = ((F)[3]*(F)[7]-(F)[4]*(F)[6])*(DET);\
  (INV) [7] = ((F)[1]*(F)[6]-(F)[0]*(F)[7])*(DET);\
  (INV) [8] = ((F)[0]*(F)[4]-(F)[1]*(F)[3])*(DET);\
  (DET) = 1.0 / (DET);\
}

dem::Vertex::Vertex():
  Base() { 
  _vertexData.setParticles( -1 );
}


dem::Vertex::Vertex(const Base::DoNotCallStandardConstructor& value):
  Base(value) { 
  // Please do not insert anything here
}


dem::Vertex::Vertex(const Base::PersistentVertex& argument):
  Base(argument) {
  // @todo Insert your code here
}


void dem::Vertex::init() {
  _vertexData.setParticles( ParticleHeap::getInstance().createData() );
  _vertexData.setParticlesOnCoarserLevels( ParticleHeap::getInstance().createData() );
  _vertexData.setVetoCoarsening(false);
}


void dem::Vertex::destroy() const {
  ParticleHeap::getInstance().deleteData( _vertexData.getParticles() );
  ParticleHeap::getInstance().deleteData( _vertexData.getParticlesOnCoarserLevels() );
}

int  dem::Vertex::createNewParticle(const tarch::la::Vector<DIMENSIONS,double>&   centre)
{
  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back( records::Particle() );

  records::Particle& newParticle = ParticleHeap::getInstance().getData( _vertexData.getParticles() ).back();

  newParticle._persistentRecords._centre 			= centre;
  newParticle._persistentRecords._centreOfMass      = centre;
  newParticle._persistentRecords._referentialCentreOfMass = centre;
  newParticle._persistentRecords._epsilon			= 0;
  newParticle._persistentRecords._mass				= 1;
  newParticle._persistentRecords._velocity          = tarch::la::Vector<DIMENSIONS,double>(0.0);
  newParticle._persistentRecords._angular		 	= tarch::la::Vector<DIMENSIONS,double>(0.0);

  newParticle._persistentRecords._hMin 				= 0;
  newParticle._persistentRecords._influenceRadius	= 0;
  newParticle._persistentRecords._diameter			= 0;
  newParticle._persistentRecords._numberOfTriangles = 0;

  #ifdef Dim2
  newParticle._persistentRecords._vertices(0) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(2) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(3) = DEMDoubleHeap::getInstance().createData();

  newParticle._persistentRecords._orientation = tarch::la::Vector<4,double>(0.0);
  newParticle._persistentRecords._inertia = tarch::la::Vector<4,double>(0.0);
  newParticle._persistentRecords._inverse = tarch::la::Vector<4,double>(0.0);
  #endif

  newParticle._persistentRecords._vertices(0) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(2) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(3) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(4) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(5) = DEMDoubleHeap::getInstance().createData();

  newParticle._persistentRecords._orientation(0) = 1;
  newParticle._persistentRecords._orientation(1) = 0;
  newParticle._persistentRecords._orientation(2) = 0;
  newParticle._persistentRecords._orientation(3) = 0;
  newParticle._persistentRecords._orientation(4) = 1;
  newParticle._persistentRecords._orientation(5) = 0;
  newParticle._persistentRecords._orientation(6) = 0;
  newParticle._persistentRecords._orientation(7) = 0;
  newParticle._persistentRecords._orientation(8) = 1;

  newParticle._persistentRecords._inertia = tarch::la::Vector<9,double>(0.0);
  newParticle._persistentRecords._inverse = tarch::la::Vector<9,double>(0.0);

  return ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1;
}

int dem::Vertex::getNumberOfParticles() const {
  assertion1( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), toString() );

  const int result = static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size());

  return result;
}

int dem::Vertex::getNumberOfRealAndVirtualParticles() const {
  assertion1( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), toString() );
  assertion1( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticlesOnCoarserLevels()), toString() );

  int result = static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size())
                   + static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size());

  //printf("VIRTUAL:%d\n", static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size()));

  return result;
}

dem::records::Particle& dem::Vertex::getParticle( int particleNumber ) {
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), particleNumber, toString() );
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticlesOnCoarserLevels()), particleNumber, toString() );
  assertion2( particleNumber>=0, particleNumber, toString() );

  if (particleNumber>=static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size())) {
    particleNumber -= static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size());
    assertion2( particleNumber<static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size()), particleNumber, toString() );
    return ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() )[particleNumber];
  }
  else {
    assertion2( particleNumber<static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size()), particleNumber, toString() );
    return ParticleHeap::getInstance().getData( _vertexData.getParticles() )[particleNumber];
  }
}

const dem::records::Particle& dem::Vertex::getParticle( int particleNumber ) const {
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), particleNumber, toString() );
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticlesOnCoarserLevels()), particleNumber, toString() );
  assertion2( particleNumber>=0, particleNumber, toString() );

  if (particleNumber>=static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size())) {
    particleNumber -= static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size());
    assertion2( particleNumber<static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size()), particleNumber, toString() );
    return ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() )[particleNumber];
  }
  else {
    assertion2( particleNumber<static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size()), particleNumber, toString() );
    return ParticleHeap::getInstance().getData( _vertexData.getParticles() )[particleNumber];
  }
}


void dem::Vertex::appendParticle(const records::Particle& particle) {
  return ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back(particle);
}


void dem::Vertex::releaseParticle(int particleNumber) {
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), particleNumber, toString() );
  assertion2( particleNumber>=0, particleNumber, toString() );
  assertion2( particleNumber<static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size()), particleNumber, toString() );
  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).erase( ParticleHeap::getInstance().getData( _vertexData.getParticles() ).begin()+particleNumber );
}


double* dem::Vertex::getXCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(0) ).data();
}


double* dem::Vertex::getYCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(1) ).data();
}


double* dem::Vertex::getZCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(2) ).data();
}

double* dem::Vertex::getXRefCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(3) ).data();
}


double* dem::Vertex::getYRefCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(4) ).data();
}


double* dem::Vertex::getZRefCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(5) ).data();
}


int dem::Vertex::getNumberOfTriangles( int particleNumber ) const {
  return getXCoordinatesAsVector( particleNumber ).size() / DIMENSIONS;
}


dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getXCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(0) );
}


dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getYCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(1) );
}


dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getZCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(2) );
}

dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getXRefCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(3) );
}


dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getYRefCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(4) );
}


dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getZRefCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(5) );
}

const dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getXCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(0) );
}


const dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getYCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(1) );
}


const dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getZCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(2) );
}

const dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getXRefCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(3) );
}


const dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getYRefCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(4) );
}


const dem::DEMDoubleHeap::HeapEntries& dem::Vertex::getZRefCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMDoubleHeap::getInstance().getData( particle._persistentRecords._vertices(5) );
}

void dem::Vertex::clearGridRefinementAnalysisData() {
  _vertexData.setVetoCoarsening(false);
}


void dem::Vertex::restrictParticleResponsibilityData(const Vertex& fineGridVertex) {
  _vertexData.setVetoCoarsening( _vertexData.getVetoCoarsening() || fineGridVertex._vertexData.getVetoCoarsening() ||  fineGridVertex.getNumberOfParticles()>0);
}


void dem::Vertex::eraseIfParticleDistributionPermits() {
  if (!_vertexData.getVetoCoarsening() &&
    getRefinementControl()==Records::Refined)
  {
    erase();
  }
}


void dem::Vertex::clearInheritedCoarseGridParticles() {
  ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).clear();
}


void dem::Vertex::inheritCoarseGridParticles( const Vertex&  vertex )
{
  if (!vertex.isOutside() && vertex.getRefinementControl()==Vertex::Records::Refined &&
    (getRefinementControl()!=Vertex::Records::Unrefined || getNumberOfParticles()>0))
  {
	for(auto & particleCoarse:
			ParticleHeap::getInstance().getData(vertex._vertexData.getParticlesOnCoarserLevels())) //loop coarse grid vertex real and virtual particles
	{
		bool found = false;
		for(auto & particleLocal:
				ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()))//loop local grid vertex real and virtual particles
		{
			if(particleCoarse._persistentRecords.getGlobalParticleNumber()==particleLocal._persistentRecords.getGlobalParticleNumber()){
				found = true;
				break;
			}
		}
		if(!found)
			ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).insert(
			      ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).end(), particleCoarse);
	}

    /*ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).insert(
      ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).end(),
      ParticleHeap::getInstance().getData(vertex._vertexData.getParticlesOnCoarserLevels()).begin(),
      ParticleHeap::getInstance().getData(vertex._vertexData.getParticlesOnCoarserLevels()).end());*/

    ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).insert(
      ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).end(),
      ParticleHeap::getInstance().getData(vertex._vertexData.getParticles()).begin(),
      ParticleHeap::getInstance().getData(vertex._vertexData.getParticles()).end());
  }
}
