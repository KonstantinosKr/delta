// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_ABSTRACT_HEAP_H_
#define _PEANO_HEAP_ABSTRACT_HEAP_H_



namespace peano {
  namespace heap {
    class AbstractHeap;
  }
}


#include "tarch/logging/Log.h"

#include <set>


class peano::heap::AbstractHeap {
  protected:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    static std::set< AbstractHeap* >  _registeredHeaps;

    static void registerHeap( AbstractHeap* newHeap );
  public:
    /**
     * Start to send data
     *
     * This operation is typically called in beginIteration(). However, please
     * be aware that the data from the master is received and merged before.
     *
     * This operation is to be re-called in each traversal and should be
     * followed by a finish call in the each traversal as well.
     *
     * You are not allowed to send heap data from the master to the worker or
     * back before this operation has been called. This holds also for joins
     * and merges - if they exchange heap data, this operation has to be called
     * before. Synchronous means that you receive all sent data in the very
     * same iteration.
     *
     * !!! Communication specification
     *
     * If your communication specification makes the kernel responsible to
     * manage the heaps, do not call this operation yourself.
     */
    virtual void startToSendSynchronousData() = 0;

    /**
     * Counterpart of startToSendSynchronousData().
     *
     * If you exchange data along the boundaries, you have to call this variant
     * of start to send together with its finish operation later on. It opens
     * a kind of communication channel for boundary data exchange. Boundary
     * data is not received on the addressee side before the next iteration.
     * You even might decide to wait a couple of iterations before you receive
     * the boundary records.
     *
     * Please hand in the state's traversal bool that informs the heap about
     * the direction of the Peano space-filling curve.
     *
     * !!! Communication specification
     *
     * If your communication specification makes the kernel responsible to
     * manage the heaps, do not call this operation yourself.
     */
    virtual void startToSendBoundaryData(bool isTraversalInverted) = 0;

    /**
     * Stop to send data
     *
     * Counterpart of startToSendSynchronousData(). Should be called around in each
     * traversal where you've also called startToSendSynchronousData(). When this
     * operation is called, you are not allowed to send heap data anymore.
     *
     * This operation runs through all sent master-worker and join-fork
     * messages and waits for each sent
     * message until the corresponding non-blocking MPI request is freed, i.e.
     * until the message has left the system. As the underlying MPI_Test
     * modifies the MPI handles, the operation is not const. The method also
     * has some deadlock detection.
     *
     * !!! Frequently Done Bug
     *
     * Please note that the event prepareSendToMaster() is invoked by Peano
     * after endIteration() is called. Many heap users send data to the master
     * in prepareSendToMaster(). For such codes, you may not call
     * finishedToSendData() in endIteration(). Instead, you have to notify the
     * heap that you finished to send in prepareSendToMaster().
     *
     * This induces an error on the global master where prepareSendToMaster()
     * is never called. So, you have to call finish in endIteration() as well -
     * but if and only if you are on the global master.
     *
     * !!! Communication specification
     *
     * If your communication specification makes the kernel responsible to
     * manage the heaps, do not call this operation yourself.
     */
    virtual void finishedToSendSynchronousData() = 0;

    /**
     * Finish boundary data exchange
     *
     * Counterpart of start operation. Should be called in endIteration().
     *
     * !!! Parallelisation with MPI
     *
     * If you parallelise with MPI, this operation has to be called by
     * endIteration() at the end of the traversal of the whole grid and not
     * only the local data structure. As a consequence, you may not combine
     * this operation with the communication specification
     * SendDataAndStateAfterProcessingOfLocalSubtree or
     * MaskOutWorkerMasterDataAndStateExchange as these two guys call
     * endIteration() too early.
     *
     * !!! Communication specification
     *
     * If your communication specification makes the kernel responsible to
     * manage the heaps, do not call this operation yourself.
     */
    virtual void finishedToSendBoundaryData(bool isTraversalInverted) = 0;

    static void allHeapsStartToSendSynchronousData();

    static void allHeapsStartToSendBoundaryData(bool isTraversalInverted);

    static void allHeapsFinishedToSendSynchronousData();

    static void allHeapsFinishedToSendBoundaryData(bool isTraversalInverted);
};

#endif
