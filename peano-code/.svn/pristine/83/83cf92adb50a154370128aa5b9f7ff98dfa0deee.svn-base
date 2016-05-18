// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_INITIALISE_VERTEX_ENUMERATORS_ON_REGULAR_REFINED_PATCH_H_
#define _PEANO_GRID_NODES_TASKS_INITIALISE_VERTEX_ENUMERATORS_ON_REGULAR_REFINED_PATCH_H_


namespace peano {
  namespace grid {
    namespace nodes {
      namespace tasks {
        template <class Vertex, class Cell>
        class InitialiseVertexEnumeratorsOnRegularRefinedPatch;
      }
    }
  }
}


template <class Vertex, class Cell>
class peano::grid::nodes::tasks::InitialiseVertexEnumeratorsOnRegularRefinedPatch {
  private:
    static tarch::logging::Log  _log;

    const SingleLevelEnumerator&                                      _fineGridVerticesEnumerator;
    peano::grid::RegularGridContainer<Vertex,Cell>&                   _regularGridContainer;
  public:
    InitialiseVertexEnumeratorsOnRegularRefinedPatch(
      const SingleLevelEnumerator&                     fineGridVerticesEnumerator,
      peano::grid::RegularGridContainer<Vertex,Cell>&  regularGridContainer
    );

    void operator()();
};


#include "peano/grid/nodes/tasks/InitialiseVertexEnumeratorsOnRegularRefinedPatch.cpph"


#endif
