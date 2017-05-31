#include "dem/Vertex.h"
#include "peano/utils/Loop.h"
#include "peano/grid/Checkpoint.h"
#include "delta/primitives/properties.h"

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

tarch::multicore::BooleanSemaphore                                  dem::Vertex::_VertexSemaphore;

void dem::Vertex::init() {
#if defined(SharedMemoryParallelisation)

  tarch::multicore::Lock lock(_VertexSemaphore);
  int returnedHeapIndex = ParticleHeap::getInstance().createData(0,0,peano::heap::Allocation::UseOnlyRecycledEntries);

  if (returnedHeapIndex<0) {
    returnedHeapIndex = ParticleHeap::getInstance().createData(0,0,peano::heap::Allocation::UseRecycledEntriesIfPossibleCreateNewEntriesIfRequired);
  }
  _vertexData.setParticles(returnedHeapIndex);

  returnedHeapIndex = ParticleHeap::getInstance().createData(0,0,peano::heap::Allocation::UseOnlyRecycledEntries);

  if (returnedHeapIndex<0) {
    returnedHeapIndex = ParticleHeap::getInstance().createData(0,0,peano::heap::Allocation::UseRecycledEntriesIfPossibleCreateNewEntriesIfRequired);
  }

  _vertexData.setParticlesOnCoarserLevels(returnedHeapIndex);
  _vertexData.setVetoCoarsening(false);
  lock.free();
#else
  _vertexData.setParticles( ParticleHeap::getInstance().createData() );
  _vertexData.setParticlesOnCoarserLevels( ParticleHeap::getInstance().createData() );
  _vertexData.setVetoCoarsening(false);
#endif
}

void dem::Vertex::destroy() const {
#if defined(SharedMemoryParallelisation)
  tarch::multicore::Lock lock(_VertexSemaphore);
  ParticleHeap::getInstance().deleteData( _vertexData.getParticles(), true);
  ParticleHeap::getInstance().deleteData( _vertexData.getParticlesOnCoarserLevels(), true);
  lock.free();
#else
  ParticleHeap::getInstance().deleteData( _vertexData.getParticles());
  ParticleHeap::getInstance().deleteData( _vertexData.getParticlesOnCoarserLevels());
#endif
}

int  dem::Vertex::createNewParticle(const tarch::la::Vector<DIMENSIONS,double>&   center,
		  std::vector<double>&  xCoordinates,
		  std::vector<double>&  yCoordinates,
		  std::vector<double>&  zCoordinates,
		  double epsilon, bool friction, delta::collision::material::MaterialType material, bool isObstacle, int particleId)
{
  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back( records::Particle() );

  records::Particle& newParticle = ParticleHeap::getInstance().getData( _vertexData.getParticles() ).back();

  double centerOfMass[3], inertia[9], inverse[9], mass, hMin;

  newParticle._persistentRecords._diameter	= delta::primitives::properties::computeDiagonal(xCoordinates, yCoordinates, zCoordinates)*1.2;

  //printf("DIAGONAL:%f\n", newParticle.getDiameter());
  hMin = delta::primitives::properties::computeHMin(xCoordinates, yCoordinates, zCoordinates);

  delta::primitives::properties::computeInertia(xCoordinates, yCoordinates, zCoordinates, material, mass, centerOfMass, inertia);

  //printf("Inertia: %f %f %f %f %f %f %f %f %f\n", inertia[0], inertia[1], inertia[2], inertia[3], inertia[4], inertia[5], inertia[6], inertia[7], inertia[8]);
  //printf("mass: %f\n", mass);

  newParticle._persistentRecords._inertia(0) = inertia[0];
  newParticle._persistentRecords._inertia(1) = inertia[1];
  newParticle._persistentRecords._inertia(2) = inertia[2];
  newParticle._persistentRecords._inertia(3) = inertia[3];
  newParticle._persistentRecords._inertia(4) = inertia[4];
  newParticle._persistentRecords._inertia(5) = inertia[5];
  newParticle._persistentRecords._inertia(6) = inertia[6];
  newParticle._persistentRecords._inertia(7) = inertia[7];
  newParticle._persistentRecords._inertia(8) = inertia[8];

  newParticle._persistentRecords._centreOfMass(0) = centerOfMass[0];
  newParticle._persistentRecords._centreOfMass(1) = centerOfMass[1];
  newParticle._persistentRecords._centreOfMass(2) = centerOfMass[2];

  newParticle._persistentRecords._referentialCentreOfMass(0) = centerOfMass[0];
  newParticle._persistentRecords._referentialCentreOfMass(1) = centerOfMass[1];
  newParticle._persistentRecords._referentialCentreOfMass(2) = centerOfMass[2];

  newParticle._persistentRecords._centre(0) = center(0);
  newParticle._persistentRecords._centre(1) = center(1);
  newParticle._persistentRecords._centre(2) = center(2);

  delta::primitives::properties::computeInverseInertia(inertia, inverse, isObstacle);

  newParticle._persistentRecords._inverse(0) = inverse[0];
  newParticle._persistentRecords._inverse(1) = inverse[1];
  newParticle._persistentRecords._inverse(2) = inverse[2];
  newParticle._persistentRecords._inverse(3) = inverse[3];
  newParticle._persistentRecords._inverse(4) = inverse[4];
  newParticle._persistentRecords._inverse(5) = inverse[5];
  newParticle._persistentRecords._inverse(6) = inverse[6];
  newParticle._persistentRecords._inverse(7) = inverse[7];
  newParticle._persistentRecords._inverse(8) = inverse[8];

  newParticle._persistentRecords._orientation(0) = 1;
  newParticle._persistentRecords._orientation(1) = 0;
  newParticle._persistentRecords._orientation(2) = 0;
  newParticle._persistentRecords._orientation(3) = 0;
  newParticle._persistentRecords._orientation(4) = 1;
  newParticle._persistentRecords._orientation(5) = 0;
  newParticle._persistentRecords._orientation(6) = 0;
  newParticle._persistentRecords._orientation(7) = 0;
  newParticle._persistentRecords._orientation(8) = 1;

  newParticle._persistentRecords._mass            = mass;
  newParticle._persistentRecords._friction			  = friction;
  newParticle._persistentRecords._influenceRadius = (newParticle.getDiameter()+epsilon) * 1.1;
  newParticle._persistentRecords._epsilon			    = epsilon;
  newParticle._persistentRecords._hMin 				    = hMin;

  newParticle._persistentRecords._numberOfTriangles = xCoordinates.size()/DIMENSIONS;
  newParticle._persistentRecords._isObstacle 		    = isObstacle;
  newParticle._persistentRecords._material 			    = int(material);
  newParticle._persistentRecords._globalParticleId  = particleId;

  newParticle._persistentRecords._velocity    = tarch::la::Vector<DIMENSIONS,double>(0.0);
  newParticle._persistentRecords._angular		 	= tarch::la::Vector<DIMENSIONS,double>(0.0);
  newParticle._persistentRecords._referentialAngular= tarch::la::Vector<DIMENSIONS,double>(0.0);

  newParticle._persistentRecords._vertices(0) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(2) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(3) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(4) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(5) = DEMDoubleHeap::getInstance().createData();


  for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
  {
    getXCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( xCoordinates[i] );
    getYCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( yCoordinates[i] );
    getZCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( zCoordinates[i] );

    getXRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(xCoordinates[i]);
    getYRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(yCoordinates[i]);
    getZRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(zCoordinates[i]);
  }

  return ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1;
}


int  dem::Vertex::createNewParticleSphere(const tarch::la::Vector<DIMENSIONS,double>&   center,
		  double radius, double epsilon, bool friction, delta::collision::material::MaterialType material, bool isObstacle, int particleId)
{
  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back( records::Particle() );

  records::Particle& newParticle = ParticleHeap::getInstance().getData( _vertexData.getParticles() ).back();

  double inertia[9], inverse[9], mass, hMin;

  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  iREAL volume = (4.0/3.0) * 3.14 * pow(radius,3);
  iREAL density;

  switch(material)
  {
    case delta::collision::material::MaterialType::WOOD:
      density = int(delta::collision::material::MaterialDensity::WOOD);
      break;
    case delta::collision::material::MaterialType::GOLD:
      density = int(delta::collision::material::MaterialDensity::GOLD);
      break;
    case delta::collision::material::MaterialType::GRAPHITE:
      density = int(delta::collision::material::MaterialDensity::GRAPHITE);
  }
  mass = volume * density;

  newParticle._persistentRecords._inertia(0) = 0.4 * mass * radius * radius;
  newParticle._persistentRecords._inertia(1) = 0;
  newParticle._persistentRecords._inertia(2) = 0;
  newParticle._persistentRecords._inertia(3) = 0;
  newParticle._persistentRecords._inertia(4) = 0.4 * mass * radius * radius;
  newParticle._persistentRecords._inertia(5) = 0;
  newParticle._persistentRecords._inertia(6) = 0;
  newParticle._persistentRecords._inertia(7) = 0;
  newParticle._persistentRecords._inertia(8) = 0.4 * mass * radius * radius;

  newParticle._persistentRecords._centreOfMass(0) = center(0);
  newParticle._persistentRecords._centreOfMass(1) = center(1);
  newParticle._persistentRecords._centreOfMass(2) = center(2);

  newParticle._persistentRecords._referentialCentreOfMass(0) = center(0);
  newParticle._persistentRecords._referentialCentreOfMass(1) = center(1);
  newParticle._persistentRecords._referentialCentreOfMass(2) = center(2);

  newParticle._persistentRecords._centre(0) = center(0);
  newParticle._persistentRecords._centre(1) = center(1);
  newParticle._persistentRecords._centre(2) = center(2);

  delta::primitives::properties::computeInverseInertia(inertia, inverse, isObstacle);

  newParticle._persistentRecords._inverse(0) = inverse[0];
  newParticle._persistentRecords._inverse(1) = inverse[1];
  newParticle._persistentRecords._inverse(2) = inverse[2];
  newParticle._persistentRecords._inverse(3) = inverse[3];
  newParticle._persistentRecords._inverse(4) = inverse[4];
  newParticle._persistentRecords._inverse(5) = inverse[5];
  newParticle._persistentRecords._inverse(6) = inverse[6];
  newParticle._persistentRecords._inverse(7) = inverse[7];
  newParticle._persistentRecords._inverse(8) = inverse[8];

  newParticle._persistentRecords._orientation(0) = 1;
  newParticle._persistentRecords._orientation(1) = 0;
  newParticle._persistentRecords._orientation(2) = 0;
  newParticle._persistentRecords._orientation(3) = 0;
  newParticle._persistentRecords._orientation(4) = 1;
  newParticle._persistentRecords._orientation(5) = 0;
  newParticle._persistentRecords._orientation(6) = 0;
  newParticle._persistentRecords._orientation(7) = 0;
  newParticle._persistentRecords._orientation(8) = 1;

  newParticle._persistentRecords._mass				      = mass;
  newParticle._persistentRecords._friction			    = friction;
  newParticle._persistentRecords._diameter			    = radius*2;
  newParticle._persistentRecords._influenceRadius 	= (radius*2+epsilon) * 1.1;
  newParticle._persistentRecords._epsilon			      = epsilon;
  newParticle._persistentRecords._hMin 				      = 0;

  newParticle._persistentRecords._numberOfTriangles = xCoordinates.size()/DIMENSIONS;
  newParticle._persistentRecords._isObstacle 		    = isObstacle;
  newParticle._persistentRecords._material 			    = int(material);
  newParticle._persistentRecords._globalParticleId  = particleId;

  newParticle._persistentRecords._velocity          = tarch::la::Vector<DIMENSIONS,double>(0.0);
  newParticle._persistentRecords._angular		 	      = tarch::la::Vector<DIMENSIONS,double>(0.0);
  newParticle._persistentRecords._referentialAngular= tarch::la::Vector<DIMENSIONS,double>(0.0);

  newParticle._persistentRecords._vertices(0) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(2) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(3) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(4) = DEMDoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(5) = DEMDoubleHeap::getInstance().createData();

  for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
  {
    getXCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( xCoordinates[i] );
    getYCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( yCoordinates[i] );
    getZCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( zCoordinates[i] );

    getXRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(xCoordinates[i]);
    getYRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(yCoordinates[i]);
    getZRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(zCoordinates[i]);
  }

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
  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back(particle);
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
  if (!_vertexData.getVetoCoarsening() && getRefinementControl()==Records::Refined)
  {
    erase();
  }
}

void dem::Vertex::clearInheritedCoarseGridParticles() {
  ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).clear();
}

void dem::Vertex::inheritCoarseGridParticles( const Vertex&  vertex )
{
  if (!vertex.isOutside() && vertex.getRefinementControl()==Vertex::Records::Refined && (getRefinementControl()!=Vertex::Records::Unrefined || getNumberOfParticles()>0))
  {
	  //inherit virtual particles from coarse level
    for(auto &particleCoarse: ParticleHeap::getInstance().getData(vertex._vertexData.getParticlesOnCoarserLevels())) //loop coarse grid vertex real and virtual particles
    {
      bool found = false;
      for(auto &particleLocal: ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()))//loop local grid vertex real and virtual particles
      {
        if(particleCoarse.getGlobalParticleId()==particleLocal.getGlobalParticleId())
        {
          found = true;
          break;
        }
      }
      if(!found)
      {
          ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).insert(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).end(), particleCoarse);
      }
    }

    /*ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).insert(
      ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).end(),
      ParticleHeap::getInstance().getData(vertex._vertexData.getParticlesOnCoarserLevels()).begin(), ParticleHeap::getInstance().getData(vertex._vertexData.getParticlesOnCoarserLevels()).end());*/

      //inherit real particles from coarse level
      ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).insert(
      ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).end(),
      ParticleHeap::getInstance().getData(vertex._vertexData.getParticles()).begin(), ParticleHeap::getInstance().getData(vertex._vertexData.getParticles()).end());
  }
}
