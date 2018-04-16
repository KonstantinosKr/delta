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

tarch::multicore::BooleanSemaphore                                  dem::Vertex::_VertexSemaphore;

void dem::Vertex::init() {
#if defined(SharedMemoryParallelisation)

  tarch::multicore::Lock lock(_VertexSemaphore);
  int returnedHeapIndex = ParticleHeap::getInstance().createData(0,0,peano::heap::Allocation::UseRecycledEntriesIfPossibleCreateNewEntriesIfRequired);
  _vertexData.setParticles(returnedHeapIndex);

  returnedHeapIndex = ParticleHeap::getInstance().createData(0,0,peano::heap::Allocation::UseRecycledEntriesIfPossibleCreateNewEntriesIfRequired);
  _vertexData.setParticlesOnCoarserLevels(returnedHeapIndex);

  _vertexData.setNumberOfParticlesInUnrefinedVertex(0.0);
  lock.free();
#else
  _vertexData.setParticles( ParticleHeap::getInstance().createData() );
  _vertexData.setParticlesOnCoarserLevels( ParticleHeap::getInstance().createData() );
  _vertexData.setNumberOfParticlesInUnrefinedVertex(0.0);
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

int dem::Vertex::createParticle(
	delta::geometry::Object Object,
    int particleId,
    int localparticleId)
{
  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back( records::Particle() );

  records::Particle& newParticle = ParticleHeap::getInstance().getData( _vertexData.getParticles() ).back();

  newParticle._persistentRecords._diameter = Object.getMesh().computeDiagonal();

  iREAL centerOfMass[3], inertia[9], inverse[9], mass;

  Object.computeInertia(Object.getMaterial(), mass, centerOfMass, inertia);

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

  newParticle._persistentRecords._centre(0) = centerOfMass[0];
  newParticle._persistentRecords._centre(1) = centerOfMass[1];
  newParticle._persistentRecords._centre(2) = centerOfMass[2];

  Object.computeInverseInertia(inertia, inverse, Object.getIsObstacle());

  newParticle._persistentRecords._inverse(0) = inverse[0];
  newParticle._persistentRecords._inverse(1) = inverse[1];
  newParticle._persistentRecords._inverse(2) = inverse[2];
  newParticle._persistentRecords._inverse(3) = inverse[3];
  newParticle._persistentRecords._inverse(4) = inverse[4];
  newParticle._persistentRecords._inverse(5) = inverse[5];
  newParticle._persistentRecords._inverse(6) = inverse[6];
  newParticle._persistentRecords._inverse(7) = inverse[7];
  newParticle._persistentRecords._inverse(8) = inverse[8];

  newParticle._persistentRecords._orientation(0) = 1.0;
  newParticle._persistentRecords._orientation(1) = 0;
  newParticle._persistentRecords._orientation(2) = 0;
  newParticle._persistentRecords._orientation(3) = 0;
  newParticle._persistentRecords._orientation(4) = 1.0;
  newParticle._persistentRecords._orientation(5) = 0;
  newParticle._persistentRecords._orientation(6) = 0;
  newParticle._persistentRecords._orientation(7) = 0;
  newParticle._persistentRecords._orientation(8) = 1.0;

  newParticle._persistentRecords._mass           = mass;
  newParticle._persistentRecords._friction		= Object.getIsFriction();
  newParticle._persistentRecords._haloDiameter 	= (newParticle.getDiameter()+Object.getEpsilon()*2) * 1.1;
  newParticle._persistentRecords._epsilon		= Object.getEpsilon();
  newParticle._persistentRecords._hMin 			= Object.getMesh().getHMin();

  std::vector<iREAL> xCoordinates = Object.getMesh().getxCoordinates();
  std::vector<iREAL> yCoordinates = Object.getMesh().getyCoordinates();
  std::vector<iREAL> zCoordinates = Object.getMesh().getzCoordinates();

  newParticle._persistentRecords._numberOfTriangles 	= xCoordinates.size()/DIMENSIONS;
  newParticle._persistentRecords._isObstacle 		= Object.getIsObstacle();
  newParticle._persistentRecords._material 			= int(Object.getMaterial());
  newParticle._persistentRecords._globalParticleId 	= particleId;
  newParticle._persistentRecords._localParticleId   	= localparticleId;

  newParticle._persistentRecords._velocity    		= tarch::la::Vector<DIMENSIONS,iREAL>(0.0);
  newParticle._persistentRecords._angular		 	= tarch::la::Vector<DIMENSIONS,iREAL>(0.0);
  newParticle._persistentRecords._referentialAngular	= tarch::la::Vector<DIMENSIONS,iREAL>(0.0);

  newParticle._persistentRecords._vertices(0) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(2) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(3) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(4) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(5) = DEMdoubleHeap::getInstance().createData();

  for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
  {
    getXCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( xCoordinates[i] );
    getYCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( yCoordinates[i] );
    getZCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( zCoordinates[i] );

    getXRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(xCoordinates[i]);
    getYRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(yCoordinates[i]);
    getZRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(zCoordinates[i]);
  }

  #if defined(PARTICLESTATSMIN) || defined(PARTICLESTATSFULL)
    std::cout << "#####PARTICLE-INIT-PROPERTIES-DATA#####" << std::endl
              << "partiId=" << std::fixed << std::setprecision(10) << newParticle.getGlobalParticleId()  <<", mass=" << std::fixed << std::setprecision(10) << newParticle.getMass() << ", diameter=" << std::fixed << std::setprecision(10) << newParticle.getDiameter() << std::endl
              << "influRa=" << std::fixed << std::setprecision(10) << newParticle.getInfluenceRadius() <<", epsilon=" << std::fixed << std::setprecision(10) << newParticle.getEpsilon() << ", hMin=" << std::fixed << std::setprecision(10) << newParticle.getHMin() << std::endl;
  #endif

  if(particleId > -1)
  {
  newParticle._persistentRecords._velocity(0) = Object.getLinearVelocity()[0];
  newParticle._persistentRecords._velocity(1) = Object.getLinearVelocity()[1];
  newParticle._persistentRecords._velocity(2) = Object.getLinearVelocity()[2];

  newParticle._persistentRecords._angular(0) = Object.getAngularVelocity()[0];
  newParticle._persistentRecords._angular(1) = Object.getAngularVelocity()[1];
  newParticle._persistentRecords._angular(2) = Object.getAngularVelocity()[2];

  newParticle._persistentRecords._referentialAngular(0) = Object.getAngularVelocity()[0];
  newParticle._persistentRecords._referentialAngular(1) = Object.getAngularVelocity()[1];
  newParticle._persistentRecords._referentialAngular(2) = Object.getAngularVelocity()[2];
  }

  return ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1;
}

int dem::Vertex::createSubParticle(
	delta::geometry::Object Object,
    int particleId,
    int localparticleId)
{
  std::vector<iREAL> xCoordinates = Object.getMesh().getxCoordinates();
  std::vector<iREAL> yCoordinates = Object.getMesh().getyCoordinates();
  std::vector<iREAL> zCoordinates = Object.getMesh().getzCoordinates();

  const tarch::la::Vector<DIMENSIONS,iREAL>& center = {Object.getCentre()[0], Object.getCentre()[1], Object.getCentre()[2]};
  iREAL centerOfMass[3] = {Object.getCentreOfMass()[0], Object.getCentreOfMass()[1], Object.getCentreOfMass()[2]};

  iREAL inertia[9] = {
      Object.getInertia()[0],
      Object.getInertia()[1],
      Object.getInertia()[2],
      Object.getInertia()[3],
      Object.getInertia()[4],
      Object.getInertia()[5],
      Object.getInertia()[6],
      Object.getInertia()[7],
      Object.getInertia()[8]};

  iREAL inverse[9] = {
      Object.getInverse()[0],
      Object.getInverse()[1],
      Object.getInverse()[2],
      Object.getInverse()[3],
      Object.getInverse()[4],
      Object.getInverse()[5],
      Object.getInverse()[6],
      Object.getInverse()[7],
      Object.getInverse()[8]};

  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back( records::Particle() );

  records::Particle& newParticle = ParticleHeap::getInstance().getData( _vertexData.getParticles() ).back();

  newParticle._persistentRecords._diameter  = Object.getMesh().computeDiagonal();

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

  newParticle._persistentRecords._mass            	= Object.getMass();
  newParticle._persistentRecords._friction        	= Object.getIsFriction();
  newParticle._persistentRecords._haloDiameter 		= (newParticle.getDiameter()+Object.getEpsilon()*2) * 1.1;
  newParticle._persistentRecords._epsilon        	 	= Object.getEpsilon();
  newParticle._persistentRecords._hMin            	= Object.getMesh().getHMin();;

  newParticle._persistentRecords._numberOfTriangles 	= xCoordinates.size()/DIMENSIONS;
  newParticle._persistentRecords._isObstacle        	= Object.getIsObstacle();
  newParticle._persistentRecords._material          	= int(Object.getMaterial());
  newParticle._persistentRecords._globalParticleId  	= particleId;
  newParticle._persistentRecords._localParticleId  	= localparticleId;

  newParticle._persistentRecords._velocity    		= tarch::la::Vector<DIMENSIONS,iREAL>(0.0);
  newParticle._persistentRecords._angular     		= tarch::la::Vector<DIMENSIONS,iREAL>(0.0);
  newParticle._persistentRecords._referentialAngular	= tarch::la::Vector<DIMENSIONS,iREAL>(0.0);

  newParticle._persistentRecords._vertices(0) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(2) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(3) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(4) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(5) = DEMdoubleHeap::getInstance().createData();

  for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
  {
	/*
    if(i%3 == 0 && i > 0) //padding for vectorisation
    {
      getXCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( 0.0 );
      getYCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( 0.0 );
      getZCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( 0.0 );

      getXRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( 0.0 );
      getYRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( 0.0 );
      getZRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( 0.0 );
    }*/

    getXCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( xCoordinates[i] );
    getYCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( yCoordinates[i] );
    getZCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back( zCoordinates[i] );

    getXRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(xCoordinates[i]);
    getYRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(yCoordinates[i]);
    getZRefCoordinatesAsVector(ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1).push_back(zCoordinates[i]);
  }

  if(particleId > -1)
  {
	newParticle._persistentRecords._velocity(0) = Object.getLinearVelocity()[0];
	newParticle._persistentRecords._velocity(1) = Object.getLinearVelocity()[1];
	newParticle._persistentRecords._velocity(2) = Object.getLinearVelocity()[2];

	newParticle._persistentRecords._angular(0) = Object.getAngularVelocity()[0];
	newParticle._persistentRecords._angular(1) = Object.getAngularVelocity()[1];
	newParticle._persistentRecords._angular(2) = Object.getAngularVelocity()[2];

	newParticle._persistentRecords._referentialAngular(0) = Object.getAngularVelocity()[0];
	newParticle._persistentRecords._referentialAngular(1) = Object.getAngularVelocity()[1];
	newParticle._persistentRecords._referentialAngular(2) = Object.getAngularVelocity()[2];
  }

  return ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1;
}

int dem::Vertex::createSphereParticle(
	delta::geometry::Object Object,
	int particleId)
{
  const tarch::la::Vector<DIMENSIONS,iREAL>& center = {Object.getCentre()[0], Object.getCentre()[1], Object.getCentre()[2]};

  ParticleHeap::getInstance().getData( _vertexData.getParticles() ).push_back( records::Particle() );
  records::Particle& newParticle = ParticleHeap::getInstance().getData( _vertexData.getParticles() ).back();

  iREAL inertia[9], inverse[9];

  iREAL density = int(delta::geometry::material::materialToDensitymap.find(Object.getMaterial())->second);
  iREAL volume = (4.0/3.0) * 3.14 * Object.getRad() * Object.getRad() * Object.getRad();
  iREAL mass = volume * density;

  newParticle._persistentRecords._inertia(0) = 0.4 * mass * Object.getRad() * Object.getRad();
  newParticle._persistentRecords._inertia(1) = 0.0;
  newParticle._persistentRecords._inertia(2) = 0.0;
  newParticle._persistentRecords._inertia(3) = 0.0;
  newParticle._persistentRecords._inertia(4) = 0.4 * mass * Object.getRad() * Object.getRad();
  newParticle._persistentRecords._inertia(5) = 0.0;
  newParticle._persistentRecords._inertia(6) = 0.0;
  newParticle._persistentRecords._inertia(7) = 0.0;
  newParticle._persistentRecords._inertia(8) = 0.4 * mass * Object.getRad() * Object.getRad();

  inertia[0] = newParticle._persistentRecords._inertia(0);
  inertia[1] = newParticle._persistentRecords._inertia(1);
  inertia[2] = newParticle._persistentRecords._inertia(2);

  inertia[3] = newParticle._persistentRecords._inertia(3);
  inertia[4] = newParticle._persistentRecords._inertia(4);
  inertia[5] = newParticle._persistentRecords._inertia(5);

  inertia[6] = newParticle._persistentRecords._inertia(6);
  inertia[7] = newParticle._persistentRecords._inertia(7);
  inertia[8] = newParticle._persistentRecords._inertia(8);

  newParticle._persistentRecords._centreOfMass(0) = center(0);
  newParticle._persistentRecords._centreOfMass(1) = center(1);
  newParticle._persistentRecords._centreOfMass(2) = center(2);

  newParticle._persistentRecords._referentialCentreOfMass(0) = center(0);
  newParticle._persistentRecords._referentialCentreOfMass(1) = center(1);
  newParticle._persistentRecords._referentialCentreOfMass(2) = center(2);

  newParticle._persistentRecords._centre(0) = center(0);
  newParticle._persistentRecords._centre(1) = center(1);
  newParticle._persistentRecords._centre(2) = center(2);

  Object.computeInverseInertia(inertia, inverse, Object.getIsObstacle());

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

  newParticle._persistentRecords._mass				= mass;
  newParticle._persistentRecords._friction			= Object.getIsFriction();
  newParticle._persistentRecords._diameter			= Object.getRad()*2;
  newParticle._persistentRecords._haloDiameter 		= (Object.getRad()*2+Object.getEpsilon()*2) * 1.1;
  newParticle._persistentRecords._epsilon			= Object.getEpsilon();
  newParticle._persistentRecords._hMin 				= 0;

  newParticle._persistentRecords._numberOfTriangles 	= 0;
  newParticle._persistentRecords._isObstacle 		= Object.getIsObstacle();
  newParticle._persistentRecords._material 			= int(Object.getMaterial());
  newParticle._persistentRecords._globalParticleId  	= particleId;
  newParticle._persistentRecords._localParticleId   	= 0;

  newParticle._persistentRecords._velocity           = tarch::la::Vector<DIMENSIONS,iREAL>(0.0);
  newParticle._persistentRecords._angular		 	= tarch::la::Vector<DIMENSIONS,iREAL>(0.0);
  newParticle._persistentRecords._referentialAngular	= tarch::la::Vector<DIMENSIONS,iREAL>(0.0);

  newParticle._persistentRecords._vertices(0) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(1) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(2) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(3) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(4) = DEMdoubleHeap::getInstance().createData();
  newParticle._persistentRecords._vertices(5) = DEMdoubleHeap::getInstance().createData();

  if(particleId > -1)
  {
	newParticle._persistentRecords._velocity(0) = Object.getLinearVelocity()[0];
	newParticle._persistentRecords._velocity(1) = Object.getLinearVelocity()[1];
	newParticle._persistentRecords._velocity(2) = Object.getLinearVelocity()[2];

	newParticle._persistentRecords._angular(0) = Object.getAngularVelocity()[0];
	newParticle._persistentRecords._angular(1) = Object.getAngularVelocity()[1];
	newParticle._persistentRecords._angular(2) = Object.getAngularVelocity()[2];

	newParticle._persistentRecords._referentialAngular(0) = Object.getAngularVelocity()[0];
	newParticle._persistentRecords._referentialAngular(1) = Object.getAngularVelocity()[1];
	newParticle._persistentRecords._referentialAngular(2) = Object.getAngularVelocity()[2];
  }

  #if defined(PARTICLESTATSMIN) || defined(PARTICLESTATSFULL)
  std::cout   << "#####PARTICLE-INIT-PROPERTIES-DATA#####" << std::endl
              << "partiId=" << std::fixed << std::setprecision(10) << newParticle.getGlobalParticleId()  <<", mass=" << std::fixed << std::setprecision(10) << newParticle.getMass() << ", diameter=" << std::fixed << std::setprecision(10) << newParticle.getDiameter() << std::endl
              << "influRa=" << std::fixed << std::setprecision(10) << newParticle.getInfluenceRadius() <<", epsilon=" << std::fixed << std::setprecision(10) << newParticle.getEpsilon() << ", hMin=" << std::fixed << std::setprecision(10) << newParticle.getHMin() << std::endl;
  #endif

  return ParticleHeap::getInstance().getData( _vertexData.getParticles() ).size()-1;
}

int dem::Vertex::getNumberOfParticles() const {
  assertion1( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), toString() );
  return static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size());
}

int dem::Vertex::getNumberOfVirtualParticles() const {
  return static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size());
}

int dem::Vertex::getNumberOfRealAndVirtualParticles() const {
  assertion1( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), toString() );
  assertion1( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticlesOnCoarserLevels()), toString() );
  return static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size()) +
         static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size());
}

dem::records::Particle& dem::Vertex::getParticle( int particleNumber ) {
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), particleNumber, toString() );
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticlesOnCoarserLevels()), particleNumber, toString() );
  assertion2( particleNumber>=0, particleNumber, toString() );

  if (particleNumber >= static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size())) {
    particleNumber -= static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size());
    assertion2( particleNumber<static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size()), particleNumber, toString() );
    return ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() )[particleNumber];
  } else {
    assertion2( particleNumber<static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size()), particleNumber, toString() );
    return ParticleHeap::getInstance().getData( _vertexData.getParticles() )[particleNumber];
  }
}

const dem::records::Particle& dem::Vertex::getParticle( int particleNumber ) const {
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticles()), particleNumber, toString() );
  assertion2( ParticleHeap::getInstance().isValidIndex(_vertexData.getParticlesOnCoarserLevels()), particleNumber, toString() );
  assertion2( particleNumber>=0, particleNumber, toString() );

  if (particleNumber >= static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size())) {
    particleNumber -= static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticles()).size());
    assertion2( particleNumber<static_cast<int>(ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).size()), particleNumber, toString() );
    return ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() )[particleNumber];
  } else {
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

void dem::Vertex::releaseCoarseParticle(int particleNumber) {
  ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).erase( ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).begin()+particleNumber );
}

iREAL* dem::Vertex::getXCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(0) ).data();
}

iREAL* dem::Vertex::getYCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(1) ).data();
}

iREAL* dem::Vertex::getZCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(2) ).data();
}

const iREAL * dem::Vertex::getXCoordinates( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(0) ).data();
}

const iREAL * dem::Vertex::getYCoordinates( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(1) ).data();
}

const iREAL * dem::Vertex::getZCoordinates( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(2) ).data();
}

iREAL* dem::Vertex::getXRefCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(3) ).data();
}

iREAL* dem::Vertex::getYRefCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(4) ).data();
}

iREAL* dem::Vertex::getZRefCoordinates( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(5) ).data();
}

int dem::Vertex::getNumberOfTriangles( int particleNumber ) const {
  return getXCoordinatesAsVector( particleNumber ).size() / DIMENSIONS;
}

dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getXCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(0) );
}

dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getYCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(1) );
}

dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getZCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(2) );
}

dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getXRefCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(3) );
}

dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getYRefCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(4) );
}

dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getZRefCoordinatesAsVector( int particleNumber ) {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(5) );
}

const dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getXCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(0) );
}

const dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getYCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(1) );
}

const dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getZCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(2) );
}

const dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getXRefCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(3) );
}

const dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getYRefCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(4) );
}

const dem::DEMdoubleHeap::HeapEntries& dem::Vertex::getZRefCoordinatesAsVector( int particleNumber ) const {
  const records::Particle& particle = getParticle(particleNumber);
  return DEMdoubleHeap::getInstance().getData( particle._persistentRecords._vertices(5) );
}

void dem::Vertex::clearGridRefinementAnalysisData() {
  _vertexData.setNumberOfParticlesInUnrefinedVertex(0.0);
}

void dem::Vertex::propagageCoarseningFlagToCoarseGrid(const Vertex& fineGridVertex) {
  _vertexData.setNumberOfParticlesInUnrefinedVertex(_vertexData.getNumberOfParticlesInUnrefinedVertex() + fineGridVertex._vertexData.getNumberOfParticlesInUnrefinedVertex());
}

void dem::Vertex::setNumberOfParticlesInUnrefinedVertex(iREAL number)
{
  _vertexData.setNumberOfParticlesInUnrefinedVertex(number);
}

iREAL dem::Vertex::getNumberOfParticlesInUnrefinedVertex()
{
  return _vertexData.getNumberOfParticlesInUnrefinedVertex();
}

void dem::Vertex::eraseIfParticleDistributionPermits(bool realiseAggressiveCoarsening, int particles) {
  if(realiseAggressiveCoarsening)
  {
    if (_vertexData.getNumberOfParticlesInUnrefinedVertex() <= 1.0 && getRefinementControl()==Records::Refined)
    {
      erase();
    }
  } else {
    if (_vertexData.getNumberOfParticlesInUnrefinedVertex() <= 1.0 && getRefinementControl()==Records::Refined)
    {
      erase();
    }
  }
}

void dem::Vertex::clearInheritedCoarseGridParticles() {
  ParticleHeap::getInstance().getData( _vertexData.getParticlesOnCoarserLevels() ).clear();
}

void dem::Vertex::inheritCoarseGridParticles(
    const Vertex&  coarseVertex,
    const tarch::la::Vector<DIMENSIONS,iREAL>& fineGridX,
    iREAL fineGridH)
{

  if(!coarseVertex.isOutside())
  {
    iREAL v[3] = {fineGridX(0), fineGridX(1), fineGridX(2)};

    /* This bounding sphere has to have the radius =
    particle sphere radius + 2*epsilon + sqrt(diameter*h)*/

    //////////////////////////////////////////////////////////////////////
    ////////////////VIRTUAL PARTICLE INHERITANCE//////////////////////////
    //////////////////////////////////////////////////////////////////////

	  //inherit unique virtual particles from coarse level
    //loop coarse grid vertex virtual particles
    for(auto &virtualParticleCoarse: ParticleHeap::getInstance().getData(coarseVertex._vertexData.getParticlesOnCoarserLevels()))
    {
      bool found = false;

      //loop local grid vertex virtual particles
      for(auto &virtualParticleLocal: ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()))
      {
        if(virtualParticleCoarse.getGlobalParticleId() == virtualParticleLocal.getGlobalParticleId() &&
           virtualParticleCoarse.getLocalParticleId() == virtualParticleLocal.getLocalParticleId())
        {
          found = true; //if found ignore and look up next coarse particle
          break;
        }
      }

      if(!found) //add to local list
      {
        iREAL p[3] = {virtualParticleCoarse.getCentre(0), virtualParticleCoarse.getCentre(1), virtualParticleCoarse.getCentre(2)};

        iREAL dd[3];

        dd[0] = p[0] - v[0];
        dd[1] = p[1] - v[1];
        dd[2] = p[2] - v[2];

        iREAL d = sqrt(dd[0]*dd[0] + dd[1]*dd[1] + dd[2]*dd[2]);

        iREAL radius = virtualParticleCoarse.getDiameter()/2.0 + 2.0*virtualParticleCoarse.getEpsilon() + sqrt(fineGridH * fineGridH * d);

        if(d <= radius)
        {
          ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).push_back(virtualParticleCoarse);
        }
      }
    }
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////



    //////////////////////////////////////////////////////////////////////
    ////////////////REAL PARTICLE INHERITANCE/////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //inherit real particles from coarse level
    for(auto &realParticleCoarse: ParticleHeap::getInstance().getData(coarseVertex._vertexData.getParticles()))
    {
      bool found = false;

      //loop local grid vertex virtual particles
      for(auto &realParticleLocal: ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()))
      {
        if(realParticleCoarse.getGlobalParticleId() == realParticleLocal.getGlobalParticleId() &&
           realParticleCoarse.getLocalParticleId() == realParticleLocal.getLocalParticleId())
        {
          found = true; //if found ignore and look up next coarse particle
          break;
        }
      }

      if(!found) //add to local list
      {
        iREAL p[3] = {realParticleCoarse.getCentre(0), realParticleCoarse.getCentre(1), realParticleCoarse.getCentre(2)};

        iREAL dd[3];

        dd[0] = p[0] - v[0];
        dd[1] = p[1] - v[1];
        dd[2] = p[2] - v[2];

        iREAL d = sqrt(dd[0]*dd[0] + dd[1]*dd[1] + dd[2]*dd[2]);

        iREAL radius = realParticleCoarse.getDiameter()/2.0 + 2.0*realParticleCoarse.getEpsilon() + sqrt(fineGridH * fineGridH * d);

        if(d <= radius)
        {
          ParticleHeap::getInstance().getData(_vertexData.getParticlesOnCoarserLevels()).push_back(realParticleCoarse);
        }
      }
    }
    //////////////////////////////////////////////////////////////////////
  }
}

