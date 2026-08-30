// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PARALLEL_FCFS_NODE_POOL_STRATEGY_H_
#define _TARCH_PARALLEL_FCFS_NODE_POOL_STRATEGY_H_


#ifdef Parallel
#include <mpi.h>
#endif

#include "tarch/parallel/NodePoolStrategy.h"
#include "tarch/logging/Log.h"

#include <list>


namespace tarch {
  namespace parallel {
    class FCFSNodePoolStrategy;
  }
}


/**
 * FCFS Node Pool Strategy
 *
 * This is a very simple node pool strategy. It was written to enable Peano
 * users to come up with a running parallel prototype fast, but it is not
 * tuned to any pde-specific properties. It thus makes sense to implement
 * NodePoolStrategy independently for a given project. This one answers any
 * request on a FCFS basis.
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.6 $
 */
class tarch::parallel::FCFSNodePoolStrategy: public tarch::parallel::NodePoolStrategy {
  protected:
    /**
     * Node Pool List Entry Storing Status of Node
     *
     * One instance in the whole application has to keep track of the ranks
     * available, which of them are idle, and whether some of them belong to others,
     * i.e. have to communicate a lot/very fast with special other ranks. These
     * relations are represented by a node pool list entry. By this class.
     *
     * In the application's realisation, the node pool does not administer the
     * collection of ranks and their state itself. Instead, it delegates this job to
     * the node pool strategy. This strategy can be adopted to the concrete problem
     * and the concrete hardware, in particular supercomputer topology.
     *
     * @author Tobias Weinzierl
     * @version $Revision: 1.4 $
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
         * Holds the rank of the process represented by this object.
         */
        int         _rank;

        /**
         * Holds the state of the process.
         */
        State       _state;

        /**
         * Machine name
         */
        std::string _name;

      public:
        /**
         * Construct one entry. By default this entry corresponds to an idle worker.
         */
        NodePoolListEntry( int rank, const std::string& name );

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
         * An element is smaller if and only if it is idle and the subsequent node
         * than is not idle.
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

    void logQueue( const RequestQueue& queue ) const;

  public:
	/**
	 * Constructor
	 *
	 * Construct all the attributes.
	 */
    FCFSNodePoolStrategy();
    virtual ~FCFSNodePoolStrategy();

    virtual void setNodePoolTag(int tag);
    virtual tarch::parallel::messages::WorkerRequestMessage extractElementFromRequestQueue(RequestQueue& queue);
    virtual void fillWorkerRequestQueue(RequestQueue& queue);
    virtual void addNode(const tarch::parallel::messages::RegisterAtNodePoolMessage& node );
    virtual int getNumberOfIdleNodes() const;
    virtual void setNodeIdle( int rank );
    virtual int reserveNode(int forMaster);
    virtual void reserveParticularNode(int rank);
    virtual bool isRegisteredNode(int rank) const;
    virtual bool isIdleNode(int rank) const;
    virtual int getNumberOfRegisteredNodes() const;
    virtual std::string toString() const;
    virtual bool hasIdleNode(int forMaster) const;
    void removeNode( int rank ) override;

};

#endif
