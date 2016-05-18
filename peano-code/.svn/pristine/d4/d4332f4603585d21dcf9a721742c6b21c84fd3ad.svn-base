#include "multigrid/Cell.h"
#include "matrixfree/stencil/StencilFactory.h"
#include "matrixfree/stencil/ElementMatrix.h"


multigrid::Cell::Cell():
  Base() { 
  // @todo Insert your code here
}


multigrid::Cell::Cell(const Base::DoNotCallStandardConstructor& value):
  Base(value) { 
  // Please do not insert anything here
}

multigrid::Cell::Cell(const Base::PersistentCell& argument):
  Base(argument) {
  // @todo Insert your code here
}


tarch::la::Vector<DIMENSIONS,double> multigrid::Cell::getEpsilon() const {
  return _cellData.getEpsilon();
}


tarch::la::Vector<DIMENSIONS,double> multigrid::Cell::getV() const {
  return _cellData.getV();
}


void multigrid::Cell::init(const tarch::la::Vector<DIMENSIONS,double>& epsilon, const tarch::la::Vector<DIMENSIONS,double>& v) {
  _cellData.setEpsilon(epsilon);
  _cellData.setV(v);
}


matrixfree::stencil::ElementWiseAssemblyMatrix multigrid::Cell::getElementsAssemblyMatrix(const tarch::la::Vector<DIMENSIONS,double>&  h) const {
  matrixfree::stencil::ElementWiseAssemblyMatrix result;

  const matrixfree::stencil::Stencil laplacianStencil = matrixfree::stencil::getLaplacian(h,_cellData.getEpsilon());

  return matrixfree::stencil::getElementWiseAssemblyMatrix(laplacianStencil);
}
