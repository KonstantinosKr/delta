#include "myproject/Vertex.h"
#include "peano/utils/Loop.h"
#include "peano/grid/Checkpoint.h"


myproject::Vertex::Vertex():
  Base() { 
  _vertexData.setU(0.0);
}


myproject::Vertex::Vertex(const Base::DoNotCallStandardConstructor& value):
  Base(value) { 
  // Please do not insert anything here
}


myproject::Vertex::Vertex(const Base::PersistentVertex& argument):
  Base(argument) {
  // @todo Insert your code here
}

double myproject::Vertex::getU() const {
  return _vertexData.getU();
}


void myproject::Vertex::copyCurrentSolutionIntoOldSolution() {
  _vertexData.setOldU( _vertexData.getU() );
}

void myproject::Vertex::inject(const Vertex& fromVertex) {
  _vertexData.setU( fromVertex._vertexData.getU() );
}
