#include "peano/grid/CellFlags.h"

#include <sstream>


std::string toString( const peano::grid::CellFlags& cellFlags ) {
  switch (cellFlags) {
    case peano::grid::Undefined:
      return "undefined";
    case peano::grid::Leaf:
      return "leaf";
    case peano::grid::StationaryButIrregular:
      return "stationary-but-irregular";
    case peano::grid::NotStationary:
      return "not-stationary";
    case peano::grid::NotStationaryDueToInvalidation:
      return "not-stationary-due-to-invalidation";
    default:
      {
        std::ostringstream msg;
        msg << "regular subtree of height " << cellFlags;
        return msg.str();
      }
  }
  return "<error>";
}


peano::grid::CellFlags peano::grid::min( const peano::grid::CellFlags& lhs, const peano::grid::CellFlags& rhs) {
  peano::grid::CellFlags result =
    static_cast<int>(lhs) < static_cast<int>(rhs) ? lhs : rhs;
  return result;
}


void peano::grid::inc( peano::grid::CellFlags& value) {
  value = static_cast<peano::grid::CellFlags>( static_cast<int>(value)+1 );
}




std::string peano::grid::getCellFlagsLegend() {
  std::ostringstream msg;
  msg << "adjacent-trees("

      << static_cast<int>(NotStationaryDueToInvalidation)
      <<  "="
      << toString(NotStationaryDueToInvalidation)

      << ","
      << static_cast<int>(NotStationary)
      << "="
      << toString(NotStationary)

      << ","
      << static_cast<int>(StationaryButIrregular)
      << "="
      << toString(StationaryButIrregular)

      << ","
      << static_cast<int>(Leaf)
      << "="
      << toString(Leaf)

      << ",>"
      << static_cast<int>(Leaf)
      << "=regular-subtree)";
  return msg.str();
}

