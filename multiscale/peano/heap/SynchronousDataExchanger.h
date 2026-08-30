// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_SYNCHRONOUS_DATA_EXCHANGER_H_
#define _PEANO_HEAP_SYNCHRONOUS_DATA_EXCHANGER_H_


#include <list>

namespace peano {
  namespace heap {
    template<class Data, bool CreateCopiesOfSentData, class SendReceiveTaskType, class VectorContainer=std::vector<Data> >
    class SynchronousDataExchanger;
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
template<class Data, bool CreateCopiesOfSentData, class SendReceiveTaskType, class VectorContainer >
class peano::heap::SynchronousDataExchanger {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    const std::string    _identifier;
    const int            _metaDataTag;
    const int            _dataTag;

    std::list<SendReceiveTaskType >   _sendTasks;
    std::list<SendReceiveTaskType >   _receiveTasks;

    int  _numberOfSentMessages;
    int  _numberOfSentRecords;

    int  _numberOfReceivedMessages;
    int  _numberOfReceivedRecords;

    #ifdef Asserts
    bool _isCurrentlySending;
    #endif

    /**
     * Find Message in Buffer
     *
     * !!! Realisation Details
     *
     * If we find a message from the right sender, we have to check whether the
     * corresponding message exchange already has finished. If it has not
     * finished, we may not continue to search in the queue for fitting tasks.
     * We we searched, we might find a newer message from the same rank that
     * already has been completed. If we returned this message's index, we
     * would invalidate the message order.
     *
     * !!! Const
     *
     * This operation is not const as it has to test whether an assynchronous
     * message exchange has already finished. For this, it uses MPI_Test.
     * MPI_Test in turn is not const.
     *
     * @return -1 if no message found
     */
    typename std::list< SendReceiveTaskType >::iterator findMessageFromRankInReceiveBuffer(int ofRank);

    VectorContainer extractMessageFromReceiveBuffer(
      typename std::list< SendReceiveTaskType >::iterator   messageTask,
      const tarch::la::Vector<DIMENSIONS, double>&          position,
      int                                                   level
    );
  public:
    /**
     * @param identifier Only required by the plot
     */
    SynchronousDataExchanger(const std::string& identifier, int metaDataTag, int dataTag);

    void startToSendData();

    /**
     * Release the sent messages
     *
     * Wait until test holds for all messages in the send queue and then erase
     * that buffer. Please note that all meta data is sent out immediately
     * anyway, i.e. as long as only meta data is transferred, this operation
     * returns immediately. What it does is that it waits for the real data to
     * be transferred.
     */
    void finishedToSendData();

    /**
     * Check for data in the mpi queue
     *
     * If there are new messages, add a new task to the receive queue, trigger
     * the corresponding ireceive if the message comes along with data, and
     * return.
     */
    void receiveDanglingMessages();

    /**
     * Send data in a synchronous way
     *
     * Sends away the meta data immediately and (if necessary) triggers an
     * asynchronous send call for the actual data afterwards.
     *
     * !!! Implementation remarks
     *
     * It is important to insert the task first and then to trigger the send.
     * The mpi command uses the exact memory location for the isend request
     * handle. Thus, triggering the send first and then queuing the task object
     * messes up the memory.
     */
    void sendData(
      const Data* const                             data,
      int                                           count,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level
    );

    VectorContainer receiveData(
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level
    );

    void plotStatistics() const;
    void clearStatistics();

    bool validateThatIncomingJoinBuffersAreEmpty();
};


#ifdef Parallel
#include "peano/heap/SynchronousDataExchanger.cpph"
#endif

#endif
