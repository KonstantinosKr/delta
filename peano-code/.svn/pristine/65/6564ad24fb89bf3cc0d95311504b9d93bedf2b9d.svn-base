#include "peano/geometry/GeometryHelper.h"



tarch::logging::Log peano::geometry::GeometryHelper::_log( "peano::geometry::GeometryHelper" );


peano::geometry::GeometryHelper::GeometryHelper() {
}


peano::geometry::GeometryHelper::~GeometryHelper() {
}


tarch::la::Vector<DIMENSIONS,double> peano::geometry::GeometryHelper::getCellCenter(
  const tarch::la::Vector<DIMENSIONS,double>& bottomVertex,
  const tarch::la::Vector<DIMENSIONS,double>& h
) {
  logTraceInWith2Arguments( "getCellCenter(...)", bottomVertex, h );

  tarch::la::Vector<DIMENSIONS,double> result( bottomVertex + 0.5 * h );

  logTraceOutWith1Argument( "getCellCenter(...)", result );

  return result;
}


peano::geometry::GeometryHelper::VertexAction peano::geometry::GeometryHelper::getVertexCommand(
  bool  pointWithHEnvironmentIsInside,
  bool  pointWithHEnvironmentIsOutside,
  bool  pointIsOutsideOfDomainClosure,
  const CurrentVertexState& currentVertexState
) {
  logTraceInWith4Arguments( "getVertexCommand(...)", pointWithHEnvironmentIsInside, pointWithHEnvironmentIsOutside, pointIsOutsideOfDomainClosure, currentVertexState );

  if (pointWithHEnvironmentIsInside && (currentVertexState != Inside)) {
    logTraceOutWith1Argument( "getVertexCommand(...)", "CreateInnerVertex" );
    return CreateInnerVertex;
  }
  else if (pointWithHEnvironmentIsOutside && (currentVertexState != Outside)) {
    logTraceOutWith1Argument( "getVertexCommand(...)", "DestroyVertexAndCoarseItAndSwitchToOutside" );
    return DestroyVertexAndEraseItAndSwitchToOutside;
  }
  else if (pointWithHEnvironmentIsOutside ) {
    logTraceOutWith1Argument( "getVertexCommand(...)", "CoarseOutsideVertex" );
    return EraseOutsideVertex;
  }
  else if (pointIsOutsideOfDomainClosure && (currentVertexState != Outside)) {
    logTraceOutWith1Argument( "getVertexCommand(...)", "DestroyVertexAndSwitchToOutside" );
    return DestroyVertexAndSwitchToOutside;
  }
  else if (pointIsOutsideOfDomainClosure && (currentVertexState == Outside)) {
    logTraceOutWith1Argument( "getVertexCommand(...)", "LeaveOuterVertexUnaltered" );
    return LeaveOuterVertexUnaltered;
  }
  else if (!pointWithHEnvironmentIsInside && !pointIsOutsideOfDomainClosure && (currentVertexState != Boundary)) {
    logTraceOutWith1Argument( "getVertexCommand(...)", "CreateBoundaryVertex" );
    return CreateBoundaryVertex;
  }

  logTraceOutWith1Argument( "getVertexCommand(...)", "LeaveVertexUnaltered" );
  return LeaveVertexUnaltered;
}


peano::geometry::GeometryHelper::CellAction peano::geometry::GeometryHelper::getCellCommand(
  bool centerOfCellWithH2EnvironmentIsInside,
  bool centerOfCellWithH2EnvironmentIsOutside,
  bool currentCellIsInside
) {
  logTraceInWith3Arguments( "getCellCommand(...)", centerOfCellWithH2EnvironmentIsInside, centerOfCellWithH2EnvironmentIsOutside, currentCellIsInside );

  if ( centerOfCellWithH2EnvironmentIsInside && !currentCellIsInside) {
    logTraceOutWith1Argument( "getCellCommand(...)", "CreateInnerCell" );
    return CreateInnerCell;
  }
  else if ( centerOfCellWithH2EnvironmentIsOutside && currentCellIsInside ) {
    logTraceOutWith1Argument( "getCellCommand(...)", "CreateOuterCellAndDoNotAnalyseItFurtherIfItsRefined" );
    return CreateOuterCellAndDoNotAnalyseItFurtherIfItsRefined;
  }
  else if ( !centerOfCellWithH2EnvironmentIsOutside && !centerOfCellWithH2EnvironmentIsInside && currentCellIsInside ) {
    logTraceOutWith1Argument( "getCellCommand(...)", "CreateOuterCell" );
    return CreateOuterCell;
  }

  logTraceOutWith1Argument( "getCellCommand(...)", "LeaveCellUnaltered" );
  return LeaveCellUnaltered;
}
