#include "dem/repositories/Repository.h"
#include "dem/records/RepositoryState.h"

#include "dem/State.h"
#include "dem/Vertex.h"
#include "dem/Cell.h"

#include "peano/grid/Grid.h"

#include "peano/stacks/CellArrayStack.h"
#include "peano/stacks/CellSTDStack.h"

#include "peano/stacks/VertexArrayStack.h"
#include "peano/stacks/VertexSTDStack.h"

 #include "dem/adapters/CreateGrid.h" 
 #include "dem/adapters/CreateGridAndPlot.h" 
 #include "dem/adapters/TimeStep.h" 
 #include "dem/adapters/TimeStepAndPlot.h" 
 #include "dem/adapters/Collision.h" 
 #include "dem/adapters/MoveParticles.h" 
 #include "dem/adapters/Adopt.h" 
 #include "dem/adapters/AdoptReluctantly.h" 
 #include "dem/adapters/FlopAdopt.h" 
 #include "dem/adapters/PlotData.h" 
 #include "dem/adapters/TimeStepOnDynamicGrid.h" 
 #include "dem/adapters/TimeStepAndPlotOnDynamicGrid.h" 
 #include "dem/adapters/TimeStepOnReluctantDynamicGrid.h" 
 #include "dem/adapters/TimeStepAndPlotOnReluctantDynamicGrid.h" 
 #include "dem/adapters/TimeStepOnFlopDynamicGrid.h" 
 #include "dem/adapters/TimeStepAndPlotOnFlopDynamicGrid.h" 


namespace peano {
  namespace grid {
    template class Grid<dem::Vertex,dem::Cell,dem::State, peano::stacks::VertexArrayStack<dem::Vertex> ,peano::stacks::CellArrayStack<dem::Cell> ,dem::adapters::CreateGrid>;
    template class Grid<dem::Vertex,dem::Cell,dem::State, peano::stacks::VertexSTDStack<  dem::Vertex> ,peano::stacks::CellSTDStack<  dem::Cell> ,dem::adapters::CreateGrid>;
  }
}

#include "peano/grid/Grid.cpph"
