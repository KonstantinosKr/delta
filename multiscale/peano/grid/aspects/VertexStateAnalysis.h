// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_ASPECTS_VERTEX_STATE_ANALYSIS_H_
#define _PEANO_GRID_ASPECTS_VERTEX_STATE_ANALYSIS_H_


#include "tarch/logging/Log.h"

#include "peano/utils/Globals.h"
#include "peano/utils/Loop.h"

#include "peano/grid/VertexEnumerator.h"


namespace peano {
  namespace grid {
    namespace aspects {
      class VertexStateAnalysis;
    }
  }
}



class peano::grid::aspects::VertexStateAnalysis {
  private:
    static tarch::logging::Log  _log;
  public:
    template <class Vertex>
    static bool doesOneVertexCarryRefinementFlag(
      Vertex                                       fineGridVertices[FOUR_POWER_D],
      const peano::grid::VertexEnumerator&         fineGridVerticesEnumerator,
      typename Vertex::Records::RefinementControl  whichFlag
    );

    template <class Vertex>
    static bool doesNoVertexCarryRefinementFlag(
      Vertex                                       fineGridVertices[FOUR_POWER_D],
      const peano::grid::VertexEnumerator&         fineGridVerticesEnumerator,
      typename Vertex::Records::RefinementControl  whichFlag
    );

    template <class Vertex>
    static bool doAllVerticesCarryRefinementFlag(
      Vertex                                       fineGridVertices[FOUR_POWER_D],
      const peano::grid::VertexEnumerator&         fineGridVerticesEnumerator,
      typename Vertex::Records::RefinementControl  whichFlag
    );

    template <class Vertex>
    static bool doAllNonHangingVerticesCarryRefinementFlag(
      Vertex                                       fineGridVertices[FOUR_POWER_D],
      const peano::grid::VertexEnumerator&         fineGridVerticesEnumerator,
      typename Vertex::Records::RefinementControl  whichFlag
    );

    /**
     * Analyse vertices
     *
     * The operation analyses the vertices of one cell whether one of them
     * is outside the domain.
     */
    template <class Vertex>
    static bool isOneVertexOutsideDomain(
      Vertex*                               vertices,
      const peano::grid::VertexEnumerator&  verticesEnumerator
    );

    template <class Vertex>
    static bool areAllVerticesOutsideDomain(
      Vertex*                               vertices,
      const peano::grid::VertexEnumerator&  verticesEnumerator
    );

    template <class Vertex>
    static bool isOneVertexBoundary(
      Vertex*                               vertices,
      const peano::grid::VertexEnumerator&  verticesEnumerator
    );


    template <class Vertex>
    static bool areAllVerticesInsideDomain(
      Vertex*                               vertices,
      const peano::grid::VertexEnumerator&  verticesEnumerator
    );


    #ifdef Parallel
    /**
     * Analyse adjacency information of vertices
     *
     * This operation runs through the adjacent vertices of one cell and checks
     * whether they are adjacent to the passed rank. If they are, a bit is set
     * in the result bitset. With the result you can check whether a vertex is
     * adjacent to a cell (the result then is unequal zero) or where this vertex
     * is. The latter information is used throughout the fork to inform the
     * receiving class how the curve runs along a forked cell.
     * Node::updateCellsParallelStateAfterLoad() is the method using this
     * operation.
     */
    template <class Vertex>
    static std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT> whichPersistentVerticesAreAdjacentToRank(
      int                                   rank,
      Vertex*                               vertices,
      const peano::grid::VertexEnumerator&  verticesEnumerator
    );
    #endif

    template <class Vertex>
    static std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT> whichVerticesArePersistent(
      int                                   rank,
      Vertex*                               vertices,
      const peano::grid::VertexEnumerator&  verticesEnumerator
    );

    template <class Vertex>
    static bool isOneVertexHanging(
      Vertex*                               vertices,
      const peano::grid::VertexEnumerator&  verticesEnumerator
    );
};


#include "peano/grid/aspects/VertexStateAnalysis.cpph"

#endif
