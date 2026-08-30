// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_RLE_BOUNDARY_DATA_EXCHANGER_H_
#define _PEANO_HEAP_RLE_BOUNDARY_DATA_EXCHANGER_H_

#include "peano/heap/BoundaryDataExchanger.h"


namespace peano {
  namespace heap {
    template<class Data, bool CreateCopiesOfSentData, class SendReceiveTaskType, class VectorContainer = std::vector<Data> >
    class RLEBoundaryDataExchanger;
  }
}


/**
 * This class is one realisation how to exchange boundary data with neighbours.
 * Its characteristic property is that it does not send out messages of lenght
 * zero. In Peano, the number of outgoing and incoming messages sent through
 * the heap along the domain boundaries to any other rank have to match. This
 * leads to situations that some ranks often send lots of empty messages, i.e.
 * no data is transferred but we have to inform neighbours about this.
 *
 * Every message in the boundary exchangers is split up into two MPI messages.
 * The first one is a simple integer code that informs the neighbour about how
 * many entries the next message will contain. Afterwards, the exchanger pipes
 * out this number of doubles, chars, whatever you exchange ... but if and only
 * if there is data to be exchanged. If one rank sends out a lot of empty
 * messages, then mainly a bunch of meta messages dribbles through the system
 * that all cary 0 as data. They inform the other rank that no actual data is
 * to be transferred.
 *
 * If you use the RLE data exchanger, we can eliminate this dribbling and
 * instead send out fewer messages. The messager (almost) never sends out a
 * zero message. Whenever your code wants to send out a message of size zero,
 * it bookkeeps this request. If N empty messages are followed by a message
 * with real content, first a meta messaage is sent to the neighbour that
 * informs it that there had been N empty messages (the exchanger sends out a
 * -N integer to communicate this information) and then the real message leaves
 * the system. A sequence of N empty messages is compressed. This is runtime
 * lenght encoding.
 *
 *
 * <h2> Using RLE to encode particular message patterns </h2>
 *
 * In some applications, most vertices do send out one particular pattern while
 * no vertex/face sends out a message of lenght zero. To benefit from RLE in
 * this case, the user can identify those patterns herself and replace them
 * with an empty message before it is handed over to the RLE boundary
 * exchanger. If the latter returns an empty message, the user in return has to
 * replace this one by the prescribed pattern.
 *
 * This way, one can benefit from RLE but now not empty messages but a certain
 * message pattern is replaced and compressed. Such a technique has been
 * successfully developed and implemented in the ExaHyPE project by Dominic
 * Charrier.
 *
 *
 * <h2> CreateCopiesOfSentData </h2>
 *
 * This flag indicates how to handle the exchange of synchronous data. We
 * either can copy all data for each send and then free this data manually.
 * Or we can send away data directly from the heap and rely on the user that
 * this data remains persistent.
 */
template<class Data, bool CreateCopiesOfSentData, class SendReceiveTaskType, class VectorContainer>
class peano::heap::RLEBoundaryDataExchanger: public peano::heap::BoundaryDataExchanger<Data, SendReceiveTaskType, VectorContainer> {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    typedef BoundaryDataExchanger<Data, SendReceiveTaskType, VectorContainer> Base;

    int _numberOfEmptyMessagesSinceLastNonEmptySendCall;
    int _totalNumberOfSentMessagesThisTraversal;

    /**
     * Sends all zero-length messages that have not been sent yet and resets
     * their counter to zero.
     */
    void sendAllCompressedEmptyMessages();
  protected:
    virtual int getNumberOfSentMessages() const;

    virtual void postprocessFinishedToSendData();

    virtual void postprocessStartToSendData();

    /**
     * @see BoundaryDataExchanger::handleAndQueueReceivedTask()
     */
    virtual void handleAndQueueReceivedTask( const SendReceiveTaskType&  receivedTask );

    virtual void handleAndQueueSendTask( const SendReceiveTaskType&  sendTask, const Data* const data );

    virtual bool dataExchangerCommunicatesInBackground() const;
  public:
    RLEBoundaryDataExchanger();

    RLEBoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);

    virtual ~RLEBoundaryDataExchanger();

    bool probeMPIQueues() const override {
      return true;
    };
};


#ifdef Parallel
#include "peano/heap/RLEBoundaryDataExchanger.cpph"
#endif

#endif
