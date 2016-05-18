#include "multigrid/Vertex.h"
#include "peano/utils/Loop.h"
#include "peano/grid/Checkpoint.h"


multigrid::Vertex::Vertex():
  Base() { 
  // @todo Insert your code here
}


multigrid::Vertex::Vertex(const Base::DoNotCallStandardConstructor& value):
  Base(value) { 
  // Please do not insert anything here
}


multigrid::Vertex::Vertex(const Base::PersistentVertex& argument):
  Base(argument) {
  // @todo Insert your code here
}


void multigrid::Vertex::initInnerVertex(double f, const tarch::la::Vector<DIMENSIONS,double>&  fineGridH) {
  _vertexData.setVertexType( Records::Unknown );
  _vertexData.setU(0.0);
  _vertexData.setF(f * tarch::la::volume(fineGridH) );
}


void multigrid::Vertex::initDirichletVertex(double u) {
  _vertexData.setVertexType( Records::Dirichlet );
  _vertexData.setU(u);
}


double multigrid::Vertex::getF() const {
  return _vertexData.getF();
}


void multigrid::Vertex::clearF() {
  _vertexData.setF(0.0);
}


void multigrid::Vertex::clearUUpdate() {
  _vertexData.setUUpdate( 0.0 );
}


double multigrid::Vertex::getResidual() const {
  return _vertexData.getF() + _vertexData.getR();
}


double multigrid::Vertex::getHierarchicalResidual() const {
  return _vertexData.getF() + _vertexData.getHierarchicalR();
}


double multigrid::Vertex::getU() const {
  return _vertexData.getU();
}


double multigrid::Vertex::getHierarchicalU() const {
  return _vertexData.getHierarchicalU();
}


void multigrid::Vertex::clearAccumulatedAttributes() {
  _vertexData.setR(0.0);
  _vertexData.setD(0.0);

  _vertexData.setNumberOfFinerLevelsAtSamePosition( 1 );
}


void multigrid::Vertex::performJacobiSmoothingStep( double omega ) {
  assertion1( _vertexData.getVertexType()==Records::Unknown, toString() );
  assertion1( _vertexData.getD()>0.0, toString() );
  assertion2( omega>0.0, toString(), omega );
  const double update = omega / _vertexData.getD() * getResidual();
  _vertexData.setU( _vertexData.getU() + update );
  _vertexData.setUUpdate( _vertexData.getUUpdate() + update );
}


void multigrid::Vertex::correctU( double update ) {
  _vertexData.setU( _vertexData.getU() + update );
  _vertexData.setUUpdate( _vertexData.getUUpdate() + update );
}


void multigrid::Vertex::inject(const Vertex& fineGridVertex) {
  _vertexData.setU( fineGridVertex._vertexData.getU() );

  // can only be evaluated if we use the additive mg
  // assertion2(_vertexData.getNumberOfFinerLevelsAtSamePosition()>=1,toString(),fineGridVertex.toString());
  assertion2(fineGridVertex._vertexData.getNumberOfFinerLevelsAtSamePosition()>=1,toString(),fineGridVertex.toString());

  _vertexData.setNumberOfFinerLevelsAtSamePosition(
    std::max(
      _vertexData.getNumberOfFinerLevelsAtSamePosition(),
      fineGridVertex._vertexData.getNumberOfFinerLevelsAtSamePosition()+1
    )
  );
}


void multigrid::Vertex::setU( double u ) {
  _vertexData.setU( u );
}


void multigrid::Vertex::incF(double value) {
  _vertexData.setF( _vertexData.getF()+value );
}


void multigrid::Vertex::clearHierarchicalValues() {
  _vertexData.setHierarchicalU(0.0);
  _vertexData.setHierarchicalR(0.0);
}


void multigrid::Vertex::determineUHierarchical(double Pu_3h) {
  _vertexData.setHierarchicalU( _vertexData.getU()-Pu_3h );
}


double multigrid::Vertex::getDampingFactorForAdditiveCoarseGridCorrection(double omega) const {
  assertion(omega>0.0);
  assertion(omega<1.0);
  assertion1(_vertexData.getNumberOfFinerLevelsAtSamePosition()>=1,toString());
  return std::pow( omega, static_cast<double>(_vertexData.getNumberOfFinerLevelsAtSamePosition()) );
}
