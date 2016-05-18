// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_RLE_BOUNDARY_DATA_EXCHANGER_H_
#define _PEANO_HEAP_RLE_BOUNDARY_DATA_EXCHANGER_H_

#include "peano/heap/BoundaryDataExchanger.h"


namespace peano {
  namespace heap {
    template<class Data, bool CreateCopiesOfSentData>
    class RLEBoundaryDataExchanger;
  }
}


/**
 * !!! CreateCopiesOfSentData
 *
 * This flag indicates how to handle the exchange of synchronous data. We
 * either can copy all data for each send and then free this data manually.
 * Or we can send away data directly from the heap and rely on the user that
 * this data remains persistent.
 */
template<class Data, bool CreateCopiesOfSentData>
class peano::heap::RLEBoundaryDataExchanger: public peano::heap::BoundaryDataExchanger<Data> {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    typedef BoundaryDataExchanger<Data> Base;

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

    virtual void handleAndQueueReceivedTask( const SendReceiveTask<Data>&  receivedTask );

    virtual void handleAndQueueSendTask( const SendReceiveTask<Data>&  sendTask, const std::vector<Data>& data );

    virtual bool dataExchangerCommunicatesInBackground() const;
  public:
    RLEBoundaryDataExchanger();

    RLEBoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);

    virtual ~RLEBoundaryDataExchanger();
};


#ifdef Parallel
#include "peano/heap/RLEBoundaryDataExchanger.cpph"
#endif

#endif
