// Copyright (C) 2009 Technische Universitaet Muenchen
// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www5.in.tum.de/peano
#ifndef _PEANO_KERNEL_PARALLEL_ADJACENCY_LIST_H_
#define _PEANO_KERNEL_PARALLEL_ADJACENCY_LIST_H_


#include "tarch/la/Vector.h"
#include "peano/utils/Globals.h"
#include "peano/grid/VertexEnumerator.h"


#include <set>


namespace peano {
    namespace parallel {
      /**
       * I use this flag to identify neighbours that are unknown
       */
      const int UndefinedNeighbour = -1;

      /**
       * Merge adjacency list of local vertex with neighbour's adjacency list
       *
       * This operation merges the adjacency list of a local vertex with the
       * one of the neighbour. Usually, the two of them are the same, as the
       * vertex is the same due to the non-overlapping domain decomposition.
       * However several special cases can occur implying a temporary data
       * inconsistency where the adjacency lists are not the same anymore.
       * This operation has to resolve the inconsistencies
       *
       * @image html peano/parallel/AdjacencyListsEnumeration.png
       *
       * Definition:
       * - A local node is responsible for an entry i of the adjacency list, if the
       *   neighbour's adjacency list's entry i equals the node's rank.
       * - The neighbour is responsible for an entry i of the adjacency list, if
       *   the original local adjacency list's entry i equals the neighbour's rank.
       *
       * Several merge scenarios could occur. They are discussed below. In this
       * discussion, we always have to study two neighbouring processes, as
       * only two ranks are taken into account by the operation. Vertices
       * adjacent to several ranks are mapped to a sequence of local node -
       * remote node merge calls, as one vertex copy (and thus one adjacency
       * list) is received per neighbour.
       *
       * !!! The Neighbour or the Local Node Fork
       *
       * - A vertex is hold on a node 1 and a node 5. The merge operation
       *   is invoked on node 1. Node 1 has forked in to 1 and 2. Now, an
       *   entry of the local adjacency list holds 2 whereas the corresponding
       *   entry of the remote node still holds 1. The local adjacency list
       *   has the up-to-date information and is preserved.
       * - The other way round: The same scenario is running on node 5. The
       *   local entry assumes that node 1 is responsible for the adjacent
       *   cell. However, the copy received from the neighbour indicates that
       *   already node 2 has taken over control. As node 1 has been
       *   responsible for the cell before, node 5 has nothing to veto and
       *   takes over the remote information.
       *
       * !!! A Third Node Has Forked
       *
       * Another is more complicated: Let a vertex have four different
       * adjacent ranks: 1,2,3,5. We study the behaviour on node 1. Node 3
       * forks its partition into 3 and 4, i.e.
       *
       * - Node 1 holds 1,2,3,5.
       * - Node 5 also holds 1,2,3,5. This adjacency list is sent to node 1.
       * - Node 3 has forked and holds 1,2,4,5. This updated adjacency list
       *   is sent to both 5 and 1.
       *
       * Two cases can occur:
       * - Node 1 merges with the vertex from node 5 before it merges with
       *   the vertex from node 3. The old adjacency list from node 1 and the
       *   list received from 5 are the same. The merge does not update any
       *   entry. Then, node 1 merges with the list from node 3, identifies
       *   the fork of node 3, and updates its local list.
       * - Node 1 merges with the vertex from node 3 first. It identifies that
       *   node 3 has updated its list, and updates the local adjacency list
       *   as well. Now it merges with node 5 and sees that the two lists
       *   have different entries even though both node 5 and 1 are not
       *   responsible for this entry.
       *
       * In the latter case, there is no way to validate the list entries if
       * neither the local node nor the remote node are or have been
       * responsible for an entry.
       *
       * !!! The Neighbour or the Local Node Join
       *
       * We have node 0 and 2 sharing a vertex. 2 has forked further, i.e.
       * there's also a node 1 involved. The vertex's adjacency list is
       * 0,2,0,1 with 1 being a worker of 2. Node 2 and 1 decide to rejoin
       * their partitions. We are studying the merge operation on node 0.
       *
       * - Case 1: Vertex from node 2 arrives before the vertex from node 1.
       *   The vertex's adjacency list still is 0,2,0,1. So is the local one.
       *   The merge results in 0,2,0,1. Now, the 0,2,0,2 from the neighbour
       *   arrives. Node 0 changes its list into 0,2,0,2.
       * - Case 2: Vertex from node 1 arrives before the vertex from node 2.
       *   The vertex's adjacency list is already 0,2,0,2, as the worker is
       *   the one who informs everybody else about the join. The neighbour
       *   is responsible. We make the local list 0,2,0,2. Now, the vertex
       *   from node 2 with 0,2,0,1 arrives. If we now used the current local
       *   list, we would assume that 2 is responsible for this entry. However,
       *   node 2 just became responsible - in fact, it will become reponsible
       *   in the next iteration.
       *
       * For the latter case, it is important to have the local list available
       * twice.
       *
       * @param ownListBeforeMerge Own list at the begin of the iteration
       * @param ownListAfterMerge  This field holds the merged list. At the
       *                           beginning, i.e. before the first merge, it
       *                           should be a copy of ownListBeforeMerge.
       * @param neighbourList      Neighbour's list.
       * @param neighbourRank      Neighbour's rank.
       */
      void getMergedAdjacencyLists(
        const tarch::la::Vector<TWO_POWER_D,int>&   ownListBeforeMerge,
        tarch::la::Vector<TWO_POWER_D,int>&         ownListAfterMerge,
        const tarch::la::Vector<TWO_POWER_D,int>&   neighbourList,
        int                                         neighbourRank
      );


      /**
       * Derive adjacency information from coarse grid
       *
       * This operation is given the @f$ 2^d @f$ coarse grid vertices and the
       * coarse grid vertex enumerator. Furthermore, the operation accepts the
       * position of a fine grid vertex within a @f$ 4^d @f$ patch. Given these
       * data, it yields the adjacency information of the fine grid vertex.
       *
       * The operation is needed at two places within the load processes.
       * It obviously has to be called if we create a new vertex to find out
       * what the adjacency information of the new vertex looks like. For this
       * invocation, see LoadVertexLoopBody(). It also is needed if we enter a
       * cell and recognise that the parent cell is involved in a fork, i.e. for
       * this cell, a fork is triggered. If this is the case, we have to update
       * all the @f$ 2^d @f$ vertices, as their entries might have changed. See
       * the Node's updateCellAfterLoad() operation.
       *
       * !! Example
       *
       * @image html peano/parallel/parallel_AdjacencyList.png
       *
       * This stuff all has to be rewritten asap as the algorithm works.
       */
      tarch::la::Vector<TWO_POWER_D,int> getAdjacencyListFromCoarseGrid(
        const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>&  coarseGridVerticesAdjacencyLists,
        const tarch::la::Vector<DIMENSIONS,int>&                     coordinates
      );


      template <class Vertex>
      class AdjacencyListAspect {
        public:
          /**
           * Replace the rank of one cell with a different rank
           *
           * This operation runs over all the @f$ 2^d @f$ vertices adjacent to
           * the cell and sets a new rank. There's no need to invalidate these
           * vertices manually, as setAdjacentRank() already does so.
           *
           * @image html peano/parallel/AdjacencyListsEnumeration.png
           */
          static void replaceAdjancyEntriesOfVerticesOfOneCellWithDifferentRank(
            int                                   newRank,
            Vertex*                               vertices,
            const peano::grid::VertexEnumerator&  enumerator
          );

          template <class State>
          static bool isRemote(
            const Vertex&              vertex,
            const State&               state,
            bool                       considerForkingNodesToBeRemote,
            bool                       considerJoiningNodesToBeRemote
          );

          static std::set<int> getAdjacentRemoteRanks(const Vertex& vertex);
      };
    }
}


#include "peano/parallel/AdjacencyList.cpph"

#endif
