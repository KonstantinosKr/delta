// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_PLAIN_BOUNDARY_DATA_EXCHANGER_H_
#define _PEANO_HEAP_PLAIN_BOUNDARY_DATA_EXCHANGER_H_

#include "peano/heap/BoundaryDataExchanger.h"


namespace peano {
  namespace heap {
    template<class Data, bool CreateCopiesOfSentData, class SendReceiveTaskType, class VectorContainer = std::vector<Data> >
    class PlainBoundaryDataExchanger;
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
template<class Data, bool CreateCopiesOfSentData, class SendReceiveTaskType, class VectorContainer>
class peano::heap::PlainBoundaryDataExchanger: public peano::heap::BoundaryDataExchanger<Data,SendReceiveTaskType,VectorContainer> {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    typedef BoundaryDataExchanger<Data, SendReceiveTaskType, VectorContainer> Base;
  protected:
    /**
     * As each message immediately is sent, the internal field
     * _sendTasks.size() holds this value. Each task there equals one sent
     * message.
     */
    virtual int getNumberOfSentMessages() const;

    /**
     * nop
     */
    virtual void postprocessFinishedToSendData();

    /**
     * nop
     */
    virtual void postprocessStartToSendData();

    /**
     * A meta data message came in. Insert into local queue and then trigger
     * receive is necessary.
     *
     * @see BoundaryDataExchanger::handleAndQueueReceivedTask()
     */
    virtual void handleAndQueueReceivedTask( const SendReceiveTaskType& receivedTask );

    /**
     * Insert send task into local data structure and wrap and send out data
     * if necessary.
     */
    virtual void handleAndQueueSendTask( const SendReceiveTaskType& sendTask, const Data* const data );

    virtual bool dataExchangerCommunicatesInBackground() const;
  public:
    PlainBoundaryDataExchanger();

    PlainBoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);

    virtual ~PlainBoundaryDataExchanger();

    bool probeMPIQueues() const override {
      return true;
    };
};


#ifdef Parallel
#include "peano/heap/PlainBoundaryDataExchanger.cpph"
#endif

#endif
