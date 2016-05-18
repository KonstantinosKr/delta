// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_BOUNDARY_DATA_EXCHANGER_H_
#define _PEANO_HEAP_BOUNDARY_DATA_EXCHANGER_H_


namespace peano {
  namespace heap {
    template<class Data>
    class BoundaryDataExchanger;
  }
}


/**
 * Abstract superclass of all boundary data exchanges
 *
 * The important plugin points are
 *
 * - getNumberOfSentMessages()
 * - postprocessFinishedToSendData()
 * - postprocessStartToSendData()
 * - handleAndQueueReceivedTask()
 * - handleAndQueueSendTask()
 *
 * i.e. the abstract protected members.
 *
 * @author Tobias Weinzierl
 */
template<class Data>
class peano::heap::BoundaryDataExchanger {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

  protected:
    const std::string    _identifier;
    const int            _metaDataTag;
    const int            _dataTag;
    const int            _rank;

    std::list<SendReceiveTask<Data> >   _sendTasks;
    std::list<SendReceiveTask<Data> >   _receiveTasks[2];

    int  _numberOfSentMessages;
    int  _numberOfSentRecords;

    /**
     * Purely statistical purposes, i.e. is not used for any real algorithmics.
     */
    int  _numberOfReceivedMessages;

    /**
     * Purely statistical purposes, i.e. is not used for any real algorithmics.
     */
    int  _numberOfReceivedRecords;

    /**
     * Is either 0 or 1 and identifies which element of _receiveDeployTasks
     * currently is the receive buffer and which one is the deploy buffer.
     */
    int _currentReceiveBuffer;


    /**
     * Shall the deploy buffer data be read in reverse order?
     *
     * Usually, the elements of the deploy buffer are delivered in reverse
     * order compared to the order they are received. See the class
     * documentation for the rationale and a reasoning. That means that this
     * flag usually is set. However, if you send out data in an adapter, then
     * do an iteration without communication, and then receive the data from
     * the heap, this flag has to be set to false.
     *
     * As a consequence, this flag is by default true. If you finishedToSendOrReceiveHeapData()
     * and the content of the receive buffer is moved to the buffer (due
     * to a transition of _currentReceiveBuffer to 1 or 0, respectively), this
     * flag also is set true. If no switching is performed in
     * relesaseMessages() as the buffer still was filled but the receive
     * buffer was empty, solely this flag is inverted.
     */
    bool  _readDeployBufferInReverseOrder;

    bool  _wasTraversalInvertedThroughoutLastSendReceiveTraversal;

    #ifdef Asserts
    bool _isCurrentlySending;
    #endif

    /**
     * Wait until number of received messages equals sent messages
     *
     * The operation is a big while loop around receiveDanglingMessages() with
     * some deadlocking aspects added, i.e. it can time out. It is not const as
     * receiveDanglingMessages() cannot be const.
     *
     * Besides waiting for MPI to release some handles, the operation also
     * invokes all the services to receive any dangling messages.
     */
    void waitUntilNumberOfReceivedNeighbourMessagesEqualsNumberOfSentMessages(int numberOfMessagesSentThisIteration);

    /**
     * Switches receive and deploy buffer between two iterations.
     *
     * This method switches receive and deploy buffer for heap data
     * exchanged between neighbours. This happens between two iterations
     * in which heap data is exchanged and results in a situation where
     * the former receive buffer is the deploy buffer afterwards and vice
     * versa.
     *
     * Due to the polling mechanism in receiveDanglingMessages(...) it
     * might happen that the receive buffer before the switch contains
     * messages which belong to the following iteration, since a
     * neighbouring rank may already be an iteration ahead. These
     * messages need to reside in the new receive buffer (i.e. the
     * former deploy buffer). Thus, after switching both buffers the
     * information of the statistics map is used to copy all messages
     * that do not belong to the previous iteration to the new receive
     * buffer.
     *
     * !!! Realisation
     *
     * We assume that all message in the receive buffer are already unpacked/in
     * a state that we can directly extract them from the receive buffer. If
     * you use sophisticated packing/coding mechanisms, all data in the receive
     * buffer is assumed to be uncompressed. See receiveDanglingMessages() for
     * details. This operation is really based upon the actual value of the
     * field _receiveTasks[1-_currentReceiveBuffer].size().
     *
     * !!! Overtaking messages
     *
     * If Peano manages to break tight synchronisation, i.e. to run multiple
     * sweeps of different traversals in parallel, it can happen that messages
     * arrive in the buffer that belong to already the subsequent traversal.
     * These may not be delivered in the current traversal, i.e. we may switch
     * the buffers, but then we have to return those additional messages to the
     * receive buffer again.
     *
     * This return mechanism also has to take into account that we need the
     * _readDeployBufferInReverseOrder flag to
     *
     * @param numberOfMessagesSentThisIteration The switch mechanism has to be
     *   know how many data have to be in the new receive buffer. If there are
     *   more messages available, those additional ones belong to next
     *   traversal already and have to be places back.
     */
    void switchReceiveAndDeployBuffer(int numberOfMessagesSentThisIteration);

    /**
     * Release Requests for Received Messages
     *
     * This operation runs through all received messages. For each one it
     * waits until the receive operation has been finished. The operation
     * basically should be const. However, it calls MPI_Test/MPI_Wait on the
     * request objects associated to the heap. This test modifies the request
     * object which makes the operation (basically via a callback) non-const.
     *
     * Besides waiting for MPI to release some handles, the operation also
     * invokes all the services to receive any dangling messages.
     */
    void releaseReceivedNeighbourMessagesRequests();

    /**
     * Release all sent messages
     *
     * This operation waits until all messages are successfully sent. Then, it
     * clears the send buffer.
     */
    void releaseSentMessages();

    /**
     * Exchanger has to know how many messages should be in the receive buffer at least.
     *
     * Keeps track of the logical number of sent messages. If you use fancy
     * compression schemes, ensure that this number is the real number of
     * messages prior to any pre-/postprocessing and that your receive
     * operations decode all received data immediately. The result afterward is used
     * by the abstract class to tell the actual implementation in
     * waitUntilNumberOfReceivedNeighbourMessagesEqualsNumberOfSentMessages()
     * for how many messages it has to wait.
     */
    virtual int getNumberOfSentMessages() const = 0;

    /**
     * Hook in for finishToSendData(). Is called when the abstract superclass
     * has finished its operations. If you use
     */
    virtual void postprocessFinishedToSendData() = 0;

    virtual void postprocessStartToSendData() = 0;

    /**
     * There has been a receive task in the mpi queue. This one is received and
     * then passed to this operation. It is the responsibility of the
     * implementation ot trigger also receives for the corresponding data (if
     * there is any), and to insert the resulting data structures into the
     * queue _receiveTasks[Base::_currentReceiveBuffer].
     */
    virtual void handleAndQueueReceivedTask( const SendReceiveTask<Data>& receivedTask ) = 0;

    /**
     * No mpi operation done yet. Ensure that data is wrapped and sent out (if
     * necessary), but first insert sendTask into the send data structure if
     * the buffer shall later check for completition. Each call increases the
     * result of getNumberOfSentMessages().
     */
    virtual void handleAndQueueSendTask( const SendReceiveTask<Data>& sendTask, const std::vector<Data>& data ) = 0;

    /**
     * Just identifies whether there are background receives possible. If not, the
     * exchanger does not use MPI_Test. This is important, as an MPI_Test on the
     * request handles otherwise yields a seg fault.
     */
    virtual bool dataExchangerCommunicatesInBackground() const = 0;
  public:
    BoundaryDataExchanger();

    /**
     *
     * !!! The parameter _isCurrentlySending
     *
     * _isCurrentlySending primarily acts as assertion. If we try to send
     * something while the flag is not set, Peano quits with an assertion.
     * This however is only half of the story.
     *
     * We have to know when a new traversal starts to be able to take all
     * data received, remove those that belong to the last iteration (while
     * we might already have received data from the current iteration), and
     * move them to the deploy buffer. For the deploy buffer, we have to know
     * whether to read it FIFO or LIFO. For this, we analyse the traversal
     * direction.
     *
     * The constructor sets the flag to false, but the Heap implementation will
     * trigger startToSend() immediately afterwards.
     *
     * @param identifier Only required by the plot
     */
    BoundaryDataExchanger(const std::string& identifier, int metaDataTag, int dataTag, int rank);

    virtual ~BoundaryDataExchanger();

    void startToSendData(bool isTraversalInverted);
    void finishedToSendData(bool isTraversalInverted);

    void receiveDanglingMessages();

    void sendData(
      const std::vector<Data>&                      data,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level
    );

    /**
     * Receive data
     *
     * i.e. take data from the received buffer (it should be called deploy
     * buffer), copy it into the result data structure and send it back. The
     * copying might also induce some type conversion if only subsets of data
     * are actually exchanged via mpi.
     *
     * !!! Validation
     *
     * If you are in assert mode, the receive operation also validates the
     * received data with the arguments you hand in, i.e. it looks whether the
     * data in the receive buffer fits to the specification. These checks
     * unfortunately have to fail with the buffered boundary exchanger as this
     * one does remove all the additional meta data from the records that
     * encode position and level. I hence make a kind of nasty assertion that
     * is disabled if data is sent via the buffered exchanger, i.e. without any
     * communication in the background.
     *
     * The buffered exchanger can be distinguished from the other exchangers as
     * dataExchangerCommunicatesInBackground() holds not: it does all the exchange
     * in one sweep.
     *
     * @param position Used for validation, i.e. to ensure that the right
     *                 record is sent back
     * @param level    Used for validation, i.e. to ensure that the right
     *                 record is sent back
     */
    std::vector< Data > receiveData(
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level
    );

    void plotStatistics() const;
    void clearStatistics();
};


#ifdef Parallel
#include "peano/heap/BoundaryDataExchanger.cpph"
#endif

#endif
