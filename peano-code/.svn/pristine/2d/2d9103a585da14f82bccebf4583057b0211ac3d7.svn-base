#include "peano/MappingSpecification.h"
#include "tarch/Assertions.h"
#include "tarch/logging/Log.h"


#include <sstream>


peano::MappingSpecification::MappingSpecification(peano::MappingSpecification::Manipulates manipulates_, peano::MappingSpecification::Multithreading multithreading_):
  manipulates(manipulates_),
  multithreading(multithreading_) {
}


std::string peano::MappingSpecification::toString() const {
  std::ostringstream msg;

  msg << "(manipulates=";
  switch (manipulates) {
    case Nop:
      msg << "nop";
      break;
    case OnlyLeaves:
      msg << "only leaves";
      break;
    case WholeTree:
      msg << "whole tree";
      break;
  }
  msg << ",multithreading=";
  switch (multithreading) {
    case Serial:
      msg << "serial";
      break;
    case AvoidFineGridRaces:
      msg << "avoid-fine-grid-races";
      break;
    case AvoidCoarseGridRaces:
      msg << "avoid-coarse-grid-races";
      break;
    case RunConcurrentlyOnFineGrid:
      msg << "concurrently on fine grid";
      break;
  }
  msg << ")";
  return msg.str();
}


peano::MappingSpecification operator&(const peano::MappingSpecification& lhs, const peano::MappingSpecification& rhs) {
  static tarch::logging::Log _log( "peano::MappingSpecification" );
  logTraceInWith2Arguments("operator&(...)", lhs.toString(), rhs.toString() );

  if (lhs.manipulates==peano::MappingSpecification::Nop ) {
    logTraceOutWith1Argument("operator&(...)","take rhs");
    return rhs;
  }
  if (rhs.manipulates==peano::MappingSpecification::Nop ) {
    logTraceOutWith1Argument("operator&(...)","take lhs");
    return lhs;
  }

  peano::MappingSpecification::Manipulates manipulates =
    (rhs.manipulates==peano::MappingSpecification::WholeTree  || lhs.manipulates==peano::MappingSpecification::WholeTree)  ? peano::MappingSpecification::WholeTree  :
    (rhs.manipulates==peano::MappingSpecification::OnlyLeaves || lhs.manipulates==peano::MappingSpecification::OnlyLeaves) ? peano::MappingSpecification::OnlyLeaves :
    peano::MappingSpecification::Nop;

  peano::MappingSpecification::Multithreading multithreading;

  if (rhs.multithreading==peano::MappingSpecification::Serial || lhs.multithreading==peano::MappingSpecification::Serial) {
    multithreading = peano::MappingSpecification::Serial;
  }
  else if (
    rhs.multithreading==peano::MappingSpecification::AvoidCoarseGridRaces || lhs.multithreading==peano::MappingSpecification::AvoidCoarseGridRaces
  ) {
    multithreading = peano::MappingSpecification::AvoidCoarseGridRaces;
  }
  else if (
    rhs.multithreading==peano::MappingSpecification::AvoidFineGridRaces || lhs.multithreading==peano::MappingSpecification::AvoidFineGridRaces
  ) {
    multithreading = peano::MappingSpecification::AvoidFineGridRaces;
  }
  else if (
    rhs.multithreading==peano::MappingSpecification::RunConcurrentlyOnFineGrid || lhs.multithreading==peano::MappingSpecification::RunConcurrentlyOnFineGrid
  ) {
    multithreading = peano::MappingSpecification::RunConcurrentlyOnFineGrid;
  }
  else {
    logError( "operator&(...)", "trying to combine " << rhs.toString() << " with " << lhs.toString() << ". Fall back to serial code" );
    multithreading = peano::MappingSpecification::Serial;
  }

  const peano::MappingSpecification result(manipulates,multithreading);
  logTraceOutWith1Argument("operator&(...)",result.toString());
  return result;
}


peano::MappingSpecification peano::MappingSpecification::getMinimalSpecification() {
  return MappingSpecification(Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
