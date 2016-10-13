#include "dem/Cell.h"


dem::Cell::Cell():
  Base() { 
  // @todo Insert your code here
}


dem::Cell::Cell(const Base::DoNotCallStandardConstructor& value):
  Base(value) { 
  // Please do not insert anything here
}

dem::Cell::Cell(const Base::PersistentCell& argument):
  Base(argument) {
  // @todo Insert your code here
}
