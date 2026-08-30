// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_AGGREGATION_BOUNDARY_DATA_EXCHANGER_H_
#define _PEANO_HEAP_AGGREGATION_BOUNDARY_DATA_EXCHANGER_H_

#include "peano/heap/BoundaryDataExchanger.h"


#include <vector>


namespace peano {
  namespace heap {
    template<class Data, class SendReceiveTaskType, class VectorContainer = std::vector<Data> >
    class AbstractAggregationBoundaryDataExchanger;

    /**
     * This is just a declaration. It does not actually exist.
     */
    //template<class Data, class SendReceiveTaskType, class VectorContainer = std::vector<Data> >
    template<class Data, class SendReceiveTaskType, class VectorContainer >
    class AggregationBoundaryDataExchanger;

    /**
     * I usually use the template alike
     *
     *   template<class SendReceiveTaskType, class VectorContainer = std::vector<char> >
     *
     * Most people use it that way, but others prefer to use aligned container variants.
     */
    template<class SendReceiveTaskType, class VectorContainer >
    class AggregationBoundaryDataExchanger<char, SendReceiveTaskType, VectorContainer>;

    /**
     * @todo The integer specialisation here is already written, but the
     *       SendReceiveTask specialisation is missing.
     */
    template<class SendReceiveTaskType, class VectorContainer >
    class AggregationBoundaryDataExchanger<int, SendReceiveTaskType, VectorContainer>;

    template<class SendReceiveTaskType, class VectorContainer >
    class AggregationBoundaryDataExchanger<double, SendReceiveTaskType, VectorContainer>;

    namespace tests {
      class AggregationBoundaryDataExchangerTest;
    }
  }
}


/**
 * The aggregation boundary data exchanger is usually only used for integers
 * and chars. It does not send out data step by step but collects all data in
 * one big buffer and sends it out at the end of the sweep. This way, we
 * eliminate the option for Peano to transfer data in the background. However,
 * we also eliminate any overhead created by lots of small messages.
 *
 * As we rely a lot on quite manual data modifications, this particular data
 * exchanger works only with the specialised heaps such as the CharHeap and the
 * DoubleHeap.
 *
 * <h2> Internal data format </h2>
 *
 * The very first entry in the array sent out is the number of (encoded)
 * messages. Per message we then have one entry which describes the number of
 * entries for this message, and then the actual message data.
 */
template<class Data, class SendReceiveTaskType, class VectorContainer>
class peano::heap::AbstractAggregationBoundaryDataExchanger: public peano::heap::BoundaryDataExchanger<Data,SendReceiveTaskType,VectorContainer> {
  protected:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    friend class tests::AggregationBoundaryDataExchangerTest;

    typedef BoundaryDataExchanger<Data, SendReceiveTaskType, VectorContainer> Base;

    int _numberOfSentMessages;

    std::vector<Data>    _aggregatedSendData;

    virtual void setAggregatedMessageHeader() = 0;
    virtual int  getNumberOfHeaderEntries() = 0;
    virtual int  getNumberOfMessages( const SendReceiveTaskType& receivedTask ) = 0;

    /**
     * As each message immediately is sent, the internal field
     * _sendTasks.size() holds this value. Each task there equals one sent
     * message.
     */
    virtual int getNumberOfSentMessages() const;

    /**
     * All messages now have gone out, so we have actually aggregated all
     * output data into our local buffer _aggregatedSendData. Now we can
     * wrap this data into a send task, enqueue it locally into the set
     * of send tasks (which always has at most cardinality one with this
     * boundary exchanger) and trigger the MPI commands.
     *
     * The routine sends out data if and only if the send buffer is not
     * empty. That means, the number of elements in the aggregated buffer
     * has to be bigger than NumberOfCharsToEncodeTotalMessageCount.
     * Otherwise, nothing is sent out. The first
     * NumberOfCharsToEncodeTotalMessageCount chars are used to encode the
     * number of messages. For this, we need a little bit of bit shuffling.
     * Originally, I used only the first element of the buffer to store the
     * number of messages. This is a poor idea if we work with chars. There are
     * obviously many situations, where a buffer holds more than 255 messages.
     */
    virtual void postprocessFinishedToSendData();


    /**
     * If data had dropped in, we can now decompose it into the many messages it did
     * stem from in the first place. While we do so, we have to ensure that all
     * these reconstructed messages are marked to have length 0. Otherwise,
     * receiveDanglingMessages() would try to invoke MPI_Test on them.
     *
     * @see BoundaryDataExchanger::handleAndQueueReceivedTask()
     */
    virtual void postprocessStartToSendData();

    /**
     * Well, just enqueue the task and trigger the receive. Nothing more to be done
     * in this very moment.
     */
    virtual void handleAndQueueReceivedTask( const SendReceiveTaskType& receivedTask );

    /**
     * Append the size of the message (from sendTask) to the local data and
     * then append also all the data.
     */
    virtual void handleAndQueueSendTask( const SendReceiveTaskType& sendTask, const Data* const data );

    virtual bool dataExchangerCommunicatesInBackground() const;
  public:
    AbstractAggregationBoundaryDataExchanger();

    AbstractAggregationBoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);

    virtual ~AbstractAggregationBoundaryDataExchanger();

    #ifdef Asserts
    /**
     * We concatenate all data in one buffer but kick out any validation data
     * such as the level from which the data comes from. So the abstract
     * superclass cannot validate whether data matches to grid entities.
     */
    bool heapStoresVertexSpatialDataForValidation() const override {
      return false;
    }
    #endif

    bool probeMPIQueues() const override {
      return true;
    };
};


template<class SendReceiveTaskType, class VectorContainer >
class peano::heap::AggregationBoundaryDataExchanger<char, SendReceiveTaskType, VectorContainer>: public peano::heap::AbstractAggregationBoundaryDataExchanger<char,SendReceiveTaskType,VectorContainer> {
  private:
	typedef peano::heap::AbstractAggregationBoundaryDataExchanger<char,SendReceiveTaskType,VectorContainer>  Base;
    friend class tests::AggregationBoundaryDataExchangerTest;

    int  getNumberOfHeaderEntries() override;
    void setAggregatedMessageHeader() override;
    int  getNumberOfMessages( const SendReceiveTaskType& receivedTask ) override;
  public:
    AggregationBoundaryDataExchanger();
    AggregationBoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);
};


template<class SendReceiveTaskType, class VectorContainer >
class peano::heap::AggregationBoundaryDataExchanger<int, SendReceiveTaskType, VectorContainer>: public peano::heap::AbstractAggregationBoundaryDataExchanger<int,SendReceiveTaskType,VectorContainer> {
  private:
	typedef peano::heap::AbstractAggregationBoundaryDataExchanger<int,SendReceiveTaskType,VectorContainer>  Base;
    friend class tests::AggregationBoundaryDataExchangerTest;

    int  getNumberOfHeaderEntries() override;
    void setAggregatedMessageHeader() override;
    int  getNumberOfMessages( const SendReceiveTaskType& receivedTask ) override;
  public:
    AggregationBoundaryDataExchanger();
    AggregationBoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);
};


template<class SendReceiveTaskType, class VectorContainer >
class peano::heap::AggregationBoundaryDataExchanger<double, SendReceiveTaskType, VectorContainer>: public peano::heap::AbstractAggregationBoundaryDataExchanger<double,SendReceiveTaskType,VectorContainer> {
  private:
	typedef peano::heap::AbstractAggregationBoundaryDataExchanger<double,SendReceiveTaskType,VectorContainer>  Base;
    friend class tests::AggregationBoundaryDataExchangerTest;

    int  getNumberOfHeaderEntries() override;
    void setAggregatedMessageHeader() override;
    int  getNumberOfMessages( const SendReceiveTaskType& receivedTask ) override;
  public:
    AggregationBoundaryDataExchanger();
    AggregationBoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);
};


#include "peano/heap/AggregationBoundaryDataExchanger.cpph"

#endif
