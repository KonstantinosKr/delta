// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GEOMETRY_GEOMETRY_HELPER_H_
#define _PEANO_GEOMETRY_GEOMETRY_HELPER_H_


#include "tarch/la/Vector.h"
#include "tarch/logging/Log.h"
#include "peano/utils/Dimensions.h"


namespace peano {
  namespace geometry {
    class GeometryHelper;
  }
}


/**
 * Helper for geometry queries
 *
 * This class is a collection of some static operations that are used by the
 * different grid realisations. It encapsulates knowledge how to deal with
 * different geometry information.
 *
 * @author Tobias Weinzierl
 */
class peano::geometry::GeometryHelper {
  private:
    GeometryHelper();

    static tarch::logging::Log _log;

  public:
    enum CurrentVertexState {
      Inside,
      Boundary,
      Outside
    };

    enum VertexAction {
      LeaveVertexUnaltered,
      LeaveOuterVertexUnaltered,
      CreateInnerVertex,
      CreateBoundaryVertex,
      EraseOutsideVertex,
      DestroyVertexAndSwitchToOutside,
      DestroyVertexAndEraseItAndSwitchToOutside
    };

    enum CellAction {
      LeaveCellUnaltered,
      CreateInnerCell,
      CreateOuterCell,
      CreateOuterCellAndDoNotAnalyseItFurtherIfItsRefined
    };

    ~GeometryHelper();

    static VertexAction getVertexCommand(
      bool  pointWithHEnvironmentIsInside,
      bool  pointWithHEnvironmentIsOutside,
      bool  pointIsOutsideOfDomainClosure,
      const CurrentVertexState& currentVertexState
    );

    static CellAction getCellCommand(
      bool centerOfCellWithH2EnvironmentIsInside,
      bool centerOfCellWithH2EnvironmentIsOutside,
      bool currentCellIsInside
    );

    /**
     * Determine center of a cell.
     *
     * @param  bottomVertex Position of left bottom vertex.
     * @param  h            Cell size
     *
     * @return @f$ bottomVertex + h/2 @f$
     */
    static tarch::la::Vector<DIMENSIONS,double> getCellCenter(
      const tarch::la::Vector<DIMENSIONS,double>& bottomVertex,
      const tarch::la::Vector<DIMENSIONS,double>& h
    );
};

#endif
