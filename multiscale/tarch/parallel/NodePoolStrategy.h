// Copyright (C) 2009 Technische Universitaet Muenchen
// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www5.in.tum.de/peano
#ifndef _TARCH_PARALLEL_NODE_POOL_STRATEGY_H_
#define _TARCH_PARALLEL_NODE_POOL_STRATEGY_H_

#ifdef Parallel
#include <mpi.h>
#endif
#include "tarch/parallel/messages/WorkerRequestMessage.h"
#include "tarch/parallel/messages/RegisterAtNodePoolMessage.h"


#include <list>


namespace tarch {
  namespace parallel {
    class NodePoolStrategy;
  }
}



/**
 * Node Pool Strategy
 *
 * Represents the strategy how the node pool handles worker requests.
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.2 $
 */
class tarch::parallel::NodePoolStrategy {
  public:
    static const int AnyMaster = -1;

    typedef std::list<tarch::parallel::messages::WorkerRequestMessage>  RequestQueue;

    virtual ~NodePoolStrategy() {}

    /**
     * Set the tag on which the node pool shall work. It is solely use to poll
     * for instances of WorkerRequestMessage.
     */
    virtual void setNodePoolTag(int tag) = 0;

    /**
     * Extract Free Element From Request Queue
     *
     * It is up to the strategy to decide which element of the queue to answer
     * first. The result is to be removed from the input parameter.
     *
     * @return Request message from queue. Element also is removed from queue.
     */
    virtual tarch::parallel::messages::WorkerRequestMessage extractElementFromRequestQueue(RequestQueue& queue) = 0;

    /**
     * Fill the Queue With Messages
     *
     * It is up to the strategy to decide how to fill the queue. One might think
     * that such a behaviour is just a poll on the MPI queues. However, there
     * are strategies that, just an example, wait for a certain time for
     * messages to give all ranks the opportunity to place their worker request.
     */
    virtual void fillWorkerRequestQueue(RequestQueue& queue) = 0;

    /**
     * Register a node.
     *
     * Adds a node as it has registered. The node has to be set working, as the
     * idle flag is to be set on the first setNodeIdle() call.
     */
    virtual void addNode(const tarch::parallel::messages::RegisterAtNodePoolMessage& node ) = 0;

    /**
     * Counterpart of addNode().
     *
     * Removed nodes always should be idle.
     */
    virtual void removeNode( int rank ) = 0;

    /**
     * This operation one is allowed to call only for the job server. It returns
     * the number of idle processors.
     */
    virtual int getNumberOfIdleNodes() const = 0;

    /**
     * Marks node rank as idle node.
     *
     * The next time another node asks for a new worker, the node pool strategy
     * then might return rank.
     */
    virtual void setNodeIdle( int rank ) = 0;

    /**
     * Counterpart of setNodeIdle().
     *
     * Makes a node active, and returns its rank. Should be called if and only
     * if there are still idle nodes. Yet, the operation may still return NoFreeNodesMessage
     * in which case it means that the reserve failed. It might still happen
     * that there are idle nodes, but the underlying domain decomposition
     * strategy decides that there are no idle nodes for this particular
     * rank.
     *
     * @param  forMaster Rank of the node that tries to reserve this node.
     * @return Rank of new active node
     */
    virtual int reserveNode(int forMaster) = 0;

    /**
     * Only used by the node pool to wake up particular ranks for global
     * communication. They will be sent idle afterwards immediately. Hence,
     * this operation doesn't really take part in the mpi balancing.
     */
    virtual void reserveParticularNode(int rank) = 0;

    /**
     * Return whether node rank is registered via addNode().
     */
    virtual bool isRegisteredNode(int rank) const = 0;

    virtual bool isIdleNode(int rank) const = 0;

    /**
     * @return Number of registered nodes.
     */
    virtual int getNumberOfRegisteredNodes() const = 0;

    /**
     * @return String representation of strategy.
     */
    virtual std::string toString() const = 0;

    /**
     * Are idle workers available.
     *
     * This operation is used in two different settings. Either we search for
     * an idle node for a given master forMaster. Or the node pool runs through
     * the sequence and wants to send all idle nodes a terminate signal. In the
     * latter case, it passes not a valid forMaster number but the flag
     * AnyMaster.
     */
    virtual bool hasIdleNode(int forMaster) const = 0;
};

#endif
