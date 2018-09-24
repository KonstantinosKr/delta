// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _MPIBALANCING_FAIR_NODE_POOL_STRATEGY_H_
#define _MPIBALANCING_FAIR_NODE_POOL_STRATEGY_H_


#ifdef Parallel
#include <mpi.h>
#endif

#include "tarch/parallel/NodePoolStrategy.h"
#include "tarch/logging/Log.h"

#include <list>


namespace mpibalancing {
  class FairNodePoolStrategy;
}


/**
 * Fair Node Pool Strategy
 *
 * Also a very simple node pool strategy. Different to the default strategy
 * answering node requests on a fcfs basis, this class collects multiple
 * requests and does some bookkeeping how many classes have already got
 * additional workers. It than answers those nodes first that got the smallest
 * number of additional workers so far.
 *
 *
 * !!! Topology-aware load balancing
 *
 * This is the prototype of a very simple topology-aware load balancer that
 * was originally written for SuperMUC's SandyBridge processors. It allows you
 * to specify how many mpi ranks you have per node. In our examples, 6 for
 * example was always a good choice which means that always 3 MPI ranks are
 * sharing 6 logical multithreaded cores.
 *
 * Given k mpi ranks per node, the strategy assumes that rank 0 is the node
 * pool server. The next k-1 ranks are left idle to avoid that ranks on the
 * first node compete for ressources and to ensure that the node pool always
 * answers promptly. Then, we try first to fill all nodes before we put multiple
 * jobs on one rank.
 *
 * @see operator< for the ordering.
 *
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.1 $
 */
class mpibalancing::FairNodePoolStrategy: public tarch::parallel::NodePoolStrategy {
  protected:
    /**
     * Copy from FCFS but enriched by counter how many rank have already
     * requested an update.
     *
     * @author Tobias Weinzierl
     * @version $Revision: 1.1 $
     */
    class NodePoolListEntry {
      public:
        /**
         * Represents the state of the worker, i.e. whether it is idle or busy.
         */
        enum State {
          IDLE,
          WORKING
        };

      private:
        /**
         * Is a copy of the encapsulating class' field.
         *
         * Should be const but then using the operator= becomes a mess.
         */
        int         _ranksPerNode;

        int         _ranksToSpareFromWork;

        /**
         * Holds the rank of the process represented by this object.
         *
         * Should be const but then using the operator= becomes a mess.
         */
        int               _rank;

        /**
         * Keep track how many workers are booked by this node.
         */
        double            _bookedWorkers;

        /**
         * Holds the state of the process.
         */
        State             _state;

        /**
         * Machine name
         *
         * Should be const but then using the operator= becomes a mess.
         */
        std::string       _name;

      public:
        /**
         * Construct one entry. By default this entry corresponds to an idle worker.
         */
        NodePoolListEntry( const int ranksPerNode, const int ranksToSpareFromWork, int rank, const std::string& name );

        /**
         * I need a default constructor for some resorting, but it is not
         * available from outside.
         */
        NodePoolListEntry();

        virtual ~NodePoolListEntry();

        /**
         * Activates the node. Precondition: Node is idle. Thus, the local min level
         * is overwritten by the argument level and the state is set to working.
         */
        void activate();

        /**
         * The local rank is set to 0 and the state is switched to idle.
         */
        void deActivate();

        /**
         * @return Rank of process.
         */
        int getRank() const;

        /**
         * @return Name of the node the process is running on.
         */
        std::string getNodeName() const;

        /**
         * @return Is the node idle?
         */
        bool isIdle() const;

        /**
         * Order elements
         *
         * The ranks on the node pool server always are held in ascending
         * order. Furthermore, the rank sequence consists of up to three
         * different segments. First, there are all the idle ranks. Afterwards,
         * the sequence holds all the busy ranks. Finally, it holds those ranks
         * that are idling but shall not be assigned a working job. The latter
         * ones are those started on the node that shall act as node pool server.
         *
         * We first check whether the current element does not belong to the
         * latter category while than belongs to those ranks that shall never
         * be assigned work. If that is the case, we can return immediately.
         * This way, we shift all ranks of the third category to the end of the
         * sequence.
         *
         * Afterwards, an element can be smaller than than if and only if the
         * current rank is idle.
         *
         * Finally, we order the ranks along their rank and their modulo rank.
         *
         * @return Object is smaller
         */
        bool operator<( const NodePoolListEntry& than ) const;

        /**
         * Two entries are equal if and only if their rank equals.
         */
        bool operator==( const NodePoolListEntry& than ) const;

        /**
         * Create string representation.
         */
        void toString(std::ostream& out) const;

        /**
         * Return string representation.
         */
        std::string toString() const;

        /**
         * We could increment the worker counter, but we actually add the level.
         * This way, ranks that already deployed very fine grid levels are not
         * as important as workers trying to fork rather coarse areas.
         */
        void addNewWorker();

        /**
         * Halves all the entries
         */
        void reduceNumberOfBookedWorkers();

        int getNumberOfBookedWorkers() const;
    };

    typedef std::list<NodePoolListEntry>   NodeContainer;

    /**
     * Logging Device
     */
    static tarch::logging::Log _log;

    /**
     * Tag on which the node pool works
     */
    int _tag;

    /**
     * The ist the list of active nodes. Every entry corresponds to one node.
     * If the entry is set, the node is working already and the server is not
     * allowed to deploy another job on this node. If the entry isn't set, there
     * is a job request message in the queue and the server is allowed to send
     * a job. Therefore, in the beginning, all the entries are set. For the very
     * first entry, corresponding to the server node, the invariant holds, that
     * this entry is set always.
     */
    NodeContainer _nodes;

    double        _waitTimeOut;

    const int     _ranksPerNode;

    const int     _ranksToSpareFromWork;

    void logQueue( const RequestQueue& queue ) const;

    int getWorkersOfNode( int rank ) const;

    void updateNodeWeights();

    bool continueToFillRequestQueue(int queueSize) const;
  public:
  /**
   * Constructor
   *
   * @param mpiRanksPerNode       Number of ranks per node.
   * @param ranksToSpareFromWork  This is the number of ranks to spare. We
   *   always spare at least the global master (rank 0) from work. However you
   *   might want to choose a bigger number of ranks such as all ranks on the
   *   first node.
   * @param waitTimeOutSec        How long shall the node wait for more
   *   messages dropping in before it starts to answer them.
   */
    FairNodePoolStrategy(int mpiRanksPerNode = 1, int ranksToSpareFromWork = 1, double waitTimeOutSec = 1e-5);
    virtual ~FairNodePoolStrategy();

    virtual void setNodePoolTag(int tag);
    virtual tarch::parallel::messages::WorkerRequestMessage extractElementFromRequestQueue(RequestQueue& queue);
    virtual void fillWorkerRequestQueue(RequestQueue& queue);
    virtual void addNode(const tarch::parallel::messages::RegisterAtNodePoolMessage& node );
    virtual void removeNode( int rank );
    virtual int getNumberOfIdleNodes() const;
    virtual void setNodeIdle( int rank );
    virtual int reserveNode(int forMaster);
    virtual void reserveParticularNode(int rank);
    virtual bool isRegisteredNode(int rank) const;
    virtual bool isIdleNode(int rank) const;
    virtual int getNumberOfRegisteredNodes() const;
    virtual std::string toString() const;

    /**
     * Are there idle nodes
     *
     * Depending on the usage context, we have to study the first element in the
     * node set or both the tail and the first element.
     *
     * @see super class
     */
    virtual bool hasIdleNode(int forMaster) const;
};

#endif
