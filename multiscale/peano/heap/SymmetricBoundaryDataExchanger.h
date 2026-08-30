// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_SYMMETRIC_BOUNDARY_DATA_EXCHANGER_H_
#define _PEANO_HEAP_SYMMETRIC_BOUNDARY_DATA_EXCHANGER_H_

#include "peano/heap/BoundaryDataExchanger.h"


namespace peano {
  namespace heap {
    template<class Data, bool CreateCopiesOfSentData, class SendReceiveTaskType, class VectorContainer = std::vector<Data> >
    class SymmetricBoundaryDataExchanger;
  }
}


/**
 * A specialised version of the boundary data exchanger assuming absolute
 * symmetry of the data exchanges
 *
 * The class fuses ideas of the RLEBoundaryDataExchanger and a class proposed
 * by Dominic Charrier called NonBlockingExchanger. Its fundamental assumption
 * is that
 *
 * All data exchange is totally symmetric, i.e. if rank a sends a message of
 * size k for one vertex to rank b, then rank b sends also a message of size
 * k to a.
 *
 * That having said, we can simplify/skip some data communication:
 *
 * - We count the number of empty messages in-between two sends with data. The
 *   very moment the second send is triggered, we know that there would also be
 *   the same number of empty messages from the neighbour side and we insert
 *   those guys into the receiver queue. See handleEmptyMessagesInBetweenTwoRealSends().
 * - Whenever handleAndQueueSendTask() is called, we don't send out any meta
 *   data. As everything is symmetric, the neighbour already knows that some
 *   data is dropping in. That implies, we should immediately queue a receive
 *   task for the respective data.
 *
 * @see RLEBoundaryDataExchanger
 */
template<class Data, bool CreateCopiesOfSentData, class SendReceiveTaskType, class VectorContainer>
class peano::heap::SymmetricBoundaryDataExchanger: public peano::heap::BoundaryDataExchanger<Data, SendReceiveTaskType, VectorContainer> {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    typedef BoundaryDataExchanger<Data, SendReceiveTaskType, VectorContainer> Base;

    int _numberOfEmptyMessagesSinceLastNonEmptySendCall;
    int _totalNumberOfSentMessagesThisTraversal;

    /**
     * We know that _numberOfEmptyMessagesSinceLastNonEthamptySendCall have been
     * triggered since the last real send of data and we know that the next real
     * send will be a non-empty one. So we reset _numberOfEmptyMessagesSinceLastNonEmptySendCall
     * to zero but before insert _numberOfEmptyMessagesSinceLastNonEmptySendCall
     * empty receive tasks into the receive queue.
     */
    void handleEmptyMessagesInBetweenTwoRealSends();
  protected:
    int getNumberOfSentMessages() const override;

    /**
     * It might have happened that a few empty sends had been triggered
     * since the last send out of real data. So we call
     * handleEmptyMessagesInBetweenTwoRealSends() to handle the remaining
     * empty messages.
     */
    void postprocessFinishedToSendData() override;

    void postprocessStartToSendData() override;

    /**
     * According to our fundamental assumption, every send operation has a
     * corresponding receive operation. So there's no need to send out any
     * meta data ever and therefore this operation never should be called.
     */
    void handleAndQueueReceivedTask( const SendReceiveTaskType&  receivedTask ) override;

    /**
     * @see Class description
     */
    void handleAndQueueSendTask( const SendReceiveTaskType&  sendTask, const Data* const data ) override;

    bool dataExchangerCommunicatesInBackground() const override;
  public:
    SymmetricBoundaryDataExchanger();

    SymmetricBoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);

    virtual ~SymmetricBoundaryDataExchanger();

    bool probeMPIQueues() const override {
      return false;
    };
};


#ifdef Parallel
#include "peano/heap/SymmetricBoundaryDataExchanger.cpph"
#endif

#endif
