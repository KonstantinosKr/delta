#include "dem/repositories/RepositoryFactory.h"

#include "dem/repositories/RepositoryArrayStack.h"
#include "dem/repositories/RepositorySTDStack.h"

#include "dem/records/RepositoryState.h"

#ifdef Parallel
#include "tarch/parallel/NodePool.h"
#include "peano/parallel/Partitioner.h"
#endif


dem::repositories::RepositoryFactory::RepositoryFactory() {
  #ifdef Parallel
  peano::parallel::Partitioner::initDatatypes();

  dem::State::initDatatype();
  dem::Vertex::initDatatype();
  dem::Cell::initDatatype();

  if (dem::records::RepositoryState::Datatype==0) {
    dem::records::RepositoryState::initDatatype();
  }
  #endif
}


dem::repositories::RepositoryFactory::~RepositoryFactory() {
}


void dem::repositories::RepositoryFactory::shutdownAllParallelDatatypes() {
  #ifdef Parallel
  peano::parallel::Partitioner::shutdownDatatypes();

  dem::State::shutdownDatatype();
  dem::Vertex::shutdownDatatype();
  dem::Cell::shutdownDatatype();

  if (dem::records::RepositoryState::Datatype!=0) {
    dem::records::RepositoryState::shutdownDatatype();
    dem::records::RepositoryState::Datatype = 0;
  }
  #endif
}


dem::repositories::RepositoryFactory& dem::repositories::RepositoryFactory::getInstance() {
  static dem::repositories::RepositoryFactory singleton;
  return singleton;
}

    
dem::repositories::Repository* 
dem::repositories::RepositoryFactory::createWithArrayStackImplementation(
  peano::geometry::Geometry&                   geometry,
  const tarch::la::Vector<DIMENSIONS,double>&  domainSize,
  const tarch::la::Vector<DIMENSIONS,double>&  computationalDomainOffset,
  int                                          maxCellStackSize,    
  int                                          maxVertexStackSize,    
  int                                          maxTemporaryVertexStackSize    
) {
  #ifdef Parallel
  if (!tarch::parallel::Node::getInstance().isGlobalMaster()) {
    return new dem::repositories::RepositoryArrayStack(geometry, domainSize, computationalDomainOffset,maxCellStackSize,maxVertexStackSize,maxTemporaryVertexStackSize);
  }
  else
  #endif
  return new dem::repositories::RepositoryArrayStack(geometry, domainSize, computationalDomainOffset,maxCellStackSize,maxVertexStackSize,maxTemporaryVertexStackSize);
}    


dem::repositories::Repository* 
dem::repositories::RepositoryFactory::createWithSTDStackImplementation(
  peano::geometry::Geometry&                   geometry,
  const tarch::la::Vector<DIMENSIONS,double>&  domainSize,
  const tarch::la::Vector<DIMENSIONS,double>&  computationalDomainOffset
) {
  #ifdef Parallel
  if (!tarch::parallel::Node::getInstance().isGlobalMaster()) {
    return new dem::repositories::RepositorySTDStack(geometry);
  }
  else
  #endif
  return new dem::repositories::RepositorySTDStack(geometry, domainSize, computationalDomainOffset);
}
