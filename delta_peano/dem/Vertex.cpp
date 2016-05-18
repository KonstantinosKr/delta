#include "dem/Vertex.h"
#include "peano/utils/Loop.h"
#include "peano/grid/Checkpoint.h"


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


int  dem::Vertex::createNewParticle(
  const tarch::la::Vector<DIMENSIONS,double>&   centre
) {
  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back( records::Particle() );

  records::Particle& newParticle = ParticleHeap::getInstance().getData( _vertexData.getParticles() ).back();

  newParticle._persistentRecords._centre            = centre;
  newParticle._persistentRecords._velocity          = tarch::la::Vector<DIMENSIONS,double>(0.0);
  newParticle._persistentRecords._numberOfTriangles = 0;

  #ifdef Dim2
  newParticle._persistentRecords._angularVelocity   = 0.0;
  #endif

  #ifdef Dim3
  newParticle._persistentRecords._angularVelocity   = tarch::la::Vector<DIMENSIONS,double>(0.0);
  #endif

  #ifdef Dim2
  newParticle._persistentRecords._vertices(0) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMDoubleHeap::getInstance().createData();
  #else
  newParticle._persistentRecords._vertices(0) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(2) = DEMDoubleHeap::getInstance().createData();
  #endif

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

  const int result = static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size())
                   + static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size());

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


void dem::Vertex::clearGridRefinementAnalysisData() {
  _vertexData.setVetoCoarsening(false);
}


void dem::Vertex::restrictParticleResponsibilityData(const Vertex& fineGridVertex) {
  _vertexData.setVetoCoarsening( _vertexData.getVetoCoarsening() || fineGridVertex._vertexData.getVetoCoarsening() ||  fineGridVertex.getNumberOfParticles()>0);
}


void dem::Vertex::eraseIfParticleDistributionPermits() {
  if (
    !_vertexData.getVetoCoarsening() &&
    getRefinementControl()==Records::Refined
  ) {
    erase();
  }
}


void dem::Vertex::clearInheritedCoarseGridParticles() {
  ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).clear();
}


void dem::Vertex::inheritCoarseGridParticles( const Vertex&  vertex ) {
  if (
    !vertex.isOutside()
    &&
    vertex.getRefinementControl()==Vertex::Records::Refined
    &&
    (getRefinementControl()!=Vertex::Records::Unrefined || getNumberOfParticles()>0)
  ) {
    ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).insert(
      ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).end(),
      ParticleHeap::getInstance().getData( vertex._vertexData.getParticlesOnCoarserLevels() ).begin(),
      ParticleHeap::getInstance().getData( vertex._vertexData.getParticlesOnCoarserLevels() ).end()
    );

    ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).insert(
      ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).end(),
      ParticleHeap::getInstance().getData( vertex._vertexData.getParticles() ).begin(),
      ParticleHeap::getInstance().getData( vertex._vertexData.getParticles() ).end()
    );
  }
}
