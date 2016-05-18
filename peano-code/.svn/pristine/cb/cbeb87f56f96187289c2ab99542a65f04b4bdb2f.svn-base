// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_PARTITIONER_H_
#define _PEANO_PARALLEL_PARTITIONER_H_


#include "tarch/la/Vector.h"
#include "peano/utils/Globals.h"
#include "peano/utils/Loop.h"
#include "tarch/logging/Log.h"

#include <bitset>
#include <vector>


namespace peano {
  namespace parallel {
    class Partitioner;
    namespace messages {
      /**
       * Forward declaration
       */
      class ForkMessage;
    }
  }
}


class peano::parallel::Partitioner {
  protected:
    static tarch::logging::Log _log;

    /**
     * Holds list of ranks reserved. The first entry should be the local node.
     *
     * Is filled by reserveNodes().
     */
    std::vector<int>                   _ranks;

    const std::bitset<THREE_POWER_D>  _localCellsOfPatch;

    std::vector<int>                  _assignedNodes;

  public:
    /**
     * Init MPI Datatypes
     *
     * The static partitioner needs MPI messages to set up the initial workload
     * distribution. The corresponding messages have to be initialised by this
     * operation.
     *
     * I made this operation protected by -DParallel guards. This way, I can
     * translate regulargrid's parallel directory even if MPI ain't included.
     */
    static void initDatatypes();
    static void shutdownDatatypes();

    /**
     * Construct partitioner
     *
     * !!! Difference to P1
     *
     * In the old Peano version, only inner cells were deployed to other ranks.
     * In the new code, I also deploy outer cells, as they also induce load
     * (memory movements), and they might become inner cells due to domain
     * changes.
     *
     * @param numberOfInnerLocalCellsOfPath Partitioner has to know how many
     *     cells of the @f$ 3^d @f$ are not already assigned to
     *     another MPI rank.
     */
    Partitioner( const std::bitset<THREE_POWER_D>&  localCellsOfPatch );

    virtual ~Partitioner();

    /**
     * Only to be called after reserveNodesForRegularGrid() or
     * reserveNodesForSpacetreeGrid() have termined successfully.
     */
    bool hasSuccessfullyReservedAdditionalWorkers() const;

    int getNumberOfSuccessfullyReservedAdditionalWorkers() const;

    /**
     * Reserve nodes
     *
     * !!! Behaviour
     *
     * This operation asks for new workers at the node pool, and it also adds
     * these new workers to the loadbalancing's oracle. It does not communicate
     * with the new workers directly.
     *
     * If the local command does not equal ForkAllChildrenAndBecomeAdministrativeRank
     * but wants to fork, we do not fork all potential candidates but leave one
     * rank local. Otherwise, nodes soon becomes pure administrative nodes not
     * computing anything anymore.
     */
    void reserveNodes();

    /**
     * Send Out Fork Messages
     *
     * Sends out all the fork messages. The partitioning takes the meander
     * direction of the Peano curve into account.
     *
     * !!! Send protocol
     *
     * The partitioner could send its partitioning message either blocking or
     * non-blocking. As the message is a direct follow-up to a job message
     * resulting from re-balancing, I make the flag
     * SendAndReceiveLoadBalancingMessagesBlocking determine the send protocol.
     *
     * @param domainOffset Offset of original computational domain, i.e. the
     *                     @f$ 3^d @f$ patch, that is now split.
     * @param h            Mesh width. That is the fine grid mesh width, i.e.
     *                     the mesh width within the patch.
     * @param loopDirection Loop direction describing how to sfc runs through
     *                     the patch.
     */
    void sendForkMessages(
      const tarch::la::Vector<DIMENSIONS,double>&  domainOffset,
      const tarch::la::Vector<DIMENSIONS,double>&  h,
      peano::utils::LoopDirection                  loopDirection,
      int                                          levelOfPatch,
      const std::bitset<DIMENSIONS>&               bitfieldOfCoarseLevelLevel
    );

    int getRankOfWorkerReponsibleForCell( tarch::la::Vector<DIMENSIONS,int> cellIndex ) const;

    std::string getPartitiongDescription() const;

    int getNumberOfReservedWorkers() const;
};


#endif
