// This file is part of the Peano project. For conditions of distribution and 
// use, please see the copyright notice at www.peano-framework.org
#ifndef _DEM_REPOSITORIES_REPOSITORY_ARRAY_STACK_H_ 
#define _DEM_REPOSITORIES_REPOSITORY_ARRAY_STACK_H_ 


#include "dem/repositories/Repository.h"
#include "dem/records/RepositoryState.h"

#include "dem/State.h"
#include "dem/Vertex.h"
#include "dem/Cell.h"

#include "peano/grid/Grid.h"
#include "peano/stacks/CellArrayStack.h"
#include "peano/stacks/VertexArrayStack.h"


 #include "dem/adapters/CreateGrid.h" 
 #include "dem/adapters/CreateGridAndPlot.h" 
 #include "dem/adapters/TimeStep.h" 
 #include "dem/adapters/TimeStepAndPlot.h" 
 #include "dem/adapters/Collision.h" 
 #include "dem/adapters/MoveParticles.h" 
 #include "dem/adapters/Adopt.h" 
 #include "dem/adapters/AdoptReluctantly.h" 
 #include "dem/adapters/AdoptFlop.h" 
 #include "dem/adapters/PlotData.h" 
 #include "dem/adapters/TimeStepOnDynamicGrid.h" 
 #include "dem/adapters/TimeStepAndPlotOnDynamicGrid.h" 
 #include "dem/adapters/TimeStepOnReluctantDynamicGrid.h" 
 #include "dem/adapters/TimeStepAndPlotOnReluctantDynamicGrid.h" 
 #include "dem/adapters/TimeStepOnFlopDynamicGrid.h" 
 #include "dem/adapters/TimeStepAndPlotOnFlopDynamicGrid.h" 



namespace dem {
      namespace repositories {
        class RepositoryArrayStack;  
      }
}


class dem::repositories::RepositoryArrayStack: public dem::repositories::Repository {
  private:
    static tarch::logging::Log _log;
  
    peano::geometry::Geometry& _geometry;
    
    typedef peano::stacks::CellArrayStack<dem::Cell>       CellStack;
    typedef peano::stacks::VertexArrayStack<dem::Vertex>   VertexStack;

    CellStack    _cellStack;
    VertexStack  _vertexStack;
    dem::State          _solverState;
    peano::grid::RegularGridContainer<dem::Vertex,dem::Cell>  _regularGridContainer;
    peano::grid::TraversalOrderOnTopLevel                                         _traversalOrderOnTopLevel;

    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::CreateGrid> _gridWithCreateGrid;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::CreateGridAndPlot> _gridWithCreateGridAndPlot;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::TimeStep> _gridWithTimeStep;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::TimeStepAndPlot> _gridWithTimeStepAndPlot;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::Collision> _gridWithCollision;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::MoveParticles> _gridWithMoveParticles;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::Adopt> _gridWithAdopt;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::AdoptReluctantly> _gridWithAdoptReluctantly;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::AdoptFlop> _gridWithAdoptFlop;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::PlotData> _gridWithPlotData;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::TimeStepOnDynamicGrid> _gridWithTimeStepOnDynamicGrid;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::TimeStepAndPlotOnDynamicGrid> _gridWithTimeStepAndPlotOnDynamicGrid;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::TimeStepOnReluctantDynamicGrid> _gridWithTimeStepOnReluctantDynamicGrid;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid> _gridWithTimeStepAndPlotOnReluctantDynamicGrid;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::TimeStepOnFlopDynamicGrid> _gridWithTimeStepOnFlopDynamicGrid;
    peano::grid::Grid<dem::Vertex,dem::Cell,dem::State,VertexStack,CellStack,dem::adapters::TimeStepAndPlotOnFlopDynamicGrid> _gridWithTimeStepAndPlotOnFlopDynamicGrid;

  
   dem::records::RepositoryState               _repositoryState;
   
    tarch::timing::Measurement _measureCreateGridCPUTime;
    tarch::timing::Measurement _measureCreateGridAndPlotCPUTime;
    tarch::timing::Measurement _measureTimeStepCPUTime;
    tarch::timing::Measurement _measureTimeStepAndPlotCPUTime;
    tarch::timing::Measurement _measureCollisionCPUTime;
    tarch::timing::Measurement _measureMoveParticlesCPUTime;
    tarch::timing::Measurement _measureAdoptCPUTime;
    tarch::timing::Measurement _measureAdoptReluctantlyCPUTime;
    tarch::timing::Measurement _measureAdoptFlopCPUTime;
    tarch::timing::Measurement _measurePlotDataCPUTime;
    tarch::timing::Measurement _measureTimeStepOnDynamicGridCPUTime;
    tarch::timing::Measurement _measureTimeStepAndPlotOnDynamicGridCPUTime;
    tarch::timing::Measurement _measureTimeStepOnReluctantDynamicGridCPUTime;
    tarch::timing::Measurement _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime;
    tarch::timing::Measurement _measureTimeStepOnFlopDynamicGridCPUTime;
    tarch::timing::Measurement _measureTimeStepAndPlotOnFlopDynamicGridCPUTime;

    tarch::timing::Measurement _measureCreateGridCalendarTime;
    tarch::timing::Measurement _measureCreateGridAndPlotCalendarTime;
    tarch::timing::Measurement _measureTimeStepCalendarTime;
    tarch::timing::Measurement _measureTimeStepAndPlotCalendarTime;
    tarch::timing::Measurement _measureCollisionCalendarTime;
    tarch::timing::Measurement _measureMoveParticlesCalendarTime;
    tarch::timing::Measurement _measureAdoptCalendarTime;
    tarch::timing::Measurement _measureAdoptReluctantlyCalendarTime;
    tarch::timing::Measurement _measureAdoptFlopCalendarTime;
    tarch::timing::Measurement _measurePlotDataCalendarTime;
    tarch::timing::Measurement _measureTimeStepOnDynamicGridCalendarTime;
    tarch::timing::Measurement _measureTimeStepAndPlotOnDynamicGridCalendarTime;
    tarch::timing::Measurement _measureTimeStepOnReluctantDynamicGridCalendarTime;
    tarch::timing::Measurement _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime;
    tarch::timing::Measurement _measureTimeStepOnFlopDynamicGridCalendarTime;
    tarch::timing::Measurement _measureTimeStepAndPlotOnFlopDynamicGridCalendarTime;


  public:
    RepositoryArrayStack(
      peano::geometry::Geometry&                   geometry,
      const tarch::la::Vector<DIMENSIONS,double>&  domainSize,
      const tarch::la::Vector<DIMENSIONS,double>&  computationalDomainOffset,
      int                                          maximumSizeOfCellInOutStack,
      int                                          maximumSizeOfVertexInOutStack,
      int                                          maximumSizeOfVertexTemporaryStack
    );
    
    /**
     * Parallel Constructor
     *
     * Used in parallel mode only where the size of the domain is not known 
     * when the type of repository is determined.  
     */
    RepositoryArrayStack(
      peano::geometry::Geometry&                   geometry,
      int                                          maximumSizeOfCellInOutStack,
      int                                          maximumSizeOfVertexInOutStack,
      int                                          maximumSizeOfVertexTemporaryStack
    );
    
    virtual ~RepositoryArrayStack();

    virtual void restart(
      const tarch::la::Vector<DIMENSIONS,double>&  domainSize,
      const tarch::la::Vector<DIMENSIONS,double>&  domainOffset,
      int                                          domainLevel,
      const tarch::la::Vector<DIMENSIONS,int>&     positionOfCentralElementWithRespectToCoarserRemoteLevel
    );
         
    virtual void terminate();
        
    virtual dem::State& getState();
    virtual const dem::State& getState() const;

    virtual void iterate(int numberOfIterations=1, bool exchangeBoundaryVertices=true);
    
    virtual void writeCheckpoint(peano::grid::Checkpoint<dem::Vertex, dem::Cell> * const checkpoint); 
    virtual void readCheckpoint( peano::grid::Checkpoint<dem::Vertex, dem::Cell> const * const checkpoint );
    virtual peano::grid::Checkpoint<dem::Vertex, dem::Cell>* createEmptyCheckpoint(); 

    virtual void switchToCreateGrid();    
    virtual void switchToCreateGridAndPlot();    
    virtual void switchToTimeStep();    
    virtual void switchToTimeStepAndPlot();    
    virtual void switchToCollision();    
    virtual void switchToMoveParticles();    
    virtual void switchToAdopt();    
    virtual void switchToAdoptReluctantly();    
    virtual void switchToAdoptFlop();    
    virtual void switchToPlotData();    
    virtual void switchToTimeStepOnDynamicGrid();    
    virtual void switchToTimeStepAndPlotOnDynamicGrid();    
    virtual void switchToTimeStepOnReluctantDynamicGrid();    
    virtual void switchToTimeStepAndPlotOnReluctantDynamicGrid();    
    virtual void switchToTimeStepOnFlopDynamicGrid();    
    virtual void switchToTimeStepAndPlotOnFlopDynamicGrid();    

    virtual bool isActiveAdapterCreateGrid() const;
    virtual bool isActiveAdapterCreateGridAndPlot() const;
    virtual bool isActiveAdapterTimeStep() const;
    virtual bool isActiveAdapterTimeStepAndPlot() const;
    virtual bool isActiveAdapterCollision() const;
    virtual bool isActiveAdapterMoveParticles() const;
    virtual bool isActiveAdapterAdopt() const;
    virtual bool isActiveAdapterAdoptReluctantly() const;
    virtual bool isActiveAdapterAdoptFlop() const;
    virtual bool isActiveAdapterPlotData() const;
    virtual bool isActiveAdapterTimeStepOnDynamicGrid() const;
    virtual bool isActiveAdapterTimeStepAndPlotOnDynamicGrid() const;
    virtual bool isActiveAdapterTimeStepOnReluctantDynamicGrid() const;
    virtual bool isActiveAdapterTimeStepAndPlotOnReluctantDynamicGrid() const;
    virtual bool isActiveAdapterTimeStepOnFlopDynamicGrid() const;
    virtual bool isActiveAdapterTimeStepAndPlotOnFlopDynamicGrid() const;

     
    #ifdef Parallel
    virtual ContinueCommand continueToIterate();
    virtual void runGlobalStep();
    #endif

    virtual void setMaximumMemoryFootprintForTemporaryRegularGrids(double value);
    virtual void logIterationStatistics(bool logAllAdapters) const;
    virtual void clearIterationStatistics();
};


#endif
