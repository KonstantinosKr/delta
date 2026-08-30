// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_BOUNDARY_DATA_EXCHANGER_H_
#define _PEANO_HEAP_BOUNDARY_DATA_EXCHANGER_H_



#include "tarch/compiler/CompilerSpecificSettings.h"
#include "peano/utils/PeanoOptimisations.h"
#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "tarch/la/Vector.h"


#include "peano/utils/Globals.h"


#include <list>


namespace peano {
  namespace heap {
    template<class Data, class SendReceiveTaskType, class VectorContainer>
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
template<class Data, class SendReceiveTaskType, class VectorContainer>
class peano::heap::BoundaryDataExchanger {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    struct BackgroundThread {
      public:
        enum class State {
          Running,
		  Terminate
        };

        static std::string toString(State state);
      private:
        BoundaryDataExchanger*               _boundaryDataExchanger;
        State                                _state;

        /**
         * We have to protect the state. The main thread might ask the
         * background thread to go down, but the background thread might be
         * busy polling MPI messages at the same time. As a result, as long as
         * the background thread is reacting to one of its states, the main
         * thread is not allowed to alter this very state.
         */
        tarch::multicore::BooleanSemaphore   _semaphore;

        BackgroundThread(const BackgroundThread&) = delete;
      public:
        BackgroundThread(BoundaryDataExchanger*  boundaryDataExchanger);
        virtual ~BackgroundThread();
        /**
         * @see peano::parallel::SendReceiveBufferPool::BackgroundThread::operator()
         *
         * This implementation explains most of the details. Different to the plain
         * boundary data receiver (which can't have too much overlap anyway), we do
         * terminate the task here also if we have already received more data than
         * we have sent out.
         */
        bool operator()();
        std::string toString() const;

        /**
         * @see _semaphores
         */
        void terminate();
    };

    #if defined(MPIHeapUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
    BackgroundThread*                    _backgroundThread;
    tarch::multicore::BooleanSemaphore   _backgroundThreadSemaphore;
    #endif

    void terminateBackgroundThread();
    void disconnectBackgroundThread();

  protected:
    const std::string    _identifier;
    const int            _metaDataTag;
    const int            _dataTag;
    const int            _rank;

    std::list<SendReceiveTaskType >   _sendTasks;
    std::list<SendReceiveTaskType >   _receiveTasks[2];

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
     *
     * The routine is invoked by startToSendData(), i.e. whenever one starts to
     * send new stuff, I first validate whether the incoming data already matches
     * the number of records sent out in the last traversal. One might argue that
     * such a check should be done in finishedToSendData(), but I want to give
     * MPI more time to complete all data transfer, so I move it into the subsequent
     * start call. Once this operation terminates, the calling routine invokes
     * releaseReceivedNeighbourMessagesRequests() next.
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
     * <h2> Realisation </h2>
     *
     * We assume that all message in the receive buffer are already unpacked/in
     * a state that we can directly extract them from the receive buffer. If
     * you use sophisticated packing/coding mechanisms, all data in the receive
     * buffer is assumed to be uncompressed. See receiveDanglingMessages() for
     * details. This operation is really based upon the actual value of the
     * field _receiveTasks[1-_currentReceiveBuffer].size().
     *
     * <h2> Overtaking messages </h2>
     *
     * If Peano manages to break tight synchronisation, i.e. to run multiple
     * sweeps of different traversals in parallel, it can happen that messages
     * arrive in the buffer that belong to already the subsequent traversal.
     * These may not be delivered in the current traversal, i.e. we may switch
     * the buffers, but then we have to return those additional messages to the
     * receive buffer again.
     *
     * releaseReceivedNeighbourMessagesRequests() is called directly before we
     * switch and we are not allowed to receive any dangling stuff in-between.
     * We therefore may copy around message meta data - we are sure that we do
     * not move around MPI handles in memory where MPI still might alter stuff.
     *
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
     * object which renders the operation non-const.
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
     * Hook in for finishToSendData(). The heaps typically wait for all
     * messages to go out until the next traversal starts again.
     */
    virtual void postprocessFinishedToSendData() = 0;

    /**
     * When this operation is called, already all receives have passed through
     * completely, and also the release and deploy buffers are switched
     */
    virtual void postprocessStartToSendData() = 0;

    /**
     * There has been a receive task in the mpi queue. This one is received and
     * then passed to this operation. It is the responsibility of the
     * implementation ot trigger also receives for the corresponding data (if
     * there is any), and to insert the resulting data structures into the
     * queue _receiveTasks[Base::_currentReceiveBuffer].
     */
    virtual void handleAndQueueReceivedTask( const SendReceiveTaskType& receivedTask ) = 0;

    /**
     * No mpi operation done yet. Ensure that data is wrapped and sent out (if
     * necessary), but first insert sendTask into the send data structure if
     * the buffer shall later check for completition. Each call increases the
     * result of getNumberOfSentMessages().
     *
     * <h2> Implementation advices </h2>
     *
     * It is very important that all data that is sent out via triggerSend is
     * sent before we send out the meta data. As soon as the meta data is in
     * place, any receiveDanglingMessages will try to query the MPIReceive
     * objects. So if we do a triggerSend() prior to the sendData, it can
     * happen that a receiveDanglingMessages() squeezes in-between the meta
     * data send and the actual data send. The actual data send (see
     * SendReceiveTask::triggerSend()) does not trigger any receiveDanglingMessages().
     * We furthermore forbid background messages retrieval. Therefore, if we
     * get out the real data asap, then we should be fine overall.
     */
    virtual void handleAndQueueSendTask(
      const SendReceiveTaskType&  sendTask,
      const Data* const           data
    ) = 0;

    /**
     * Just identifies whether there are background receives possible. If not, the
     * exchanger does not use MPI_Test. This is important, as an MPI_Test on the
     * request handles otherwise yields a seg fault.
     */
    virtual bool dataExchangerCommunicatesInBackground() const = 0;

    BoundaryDataExchanger(const BoundaryDataExchanger&) = delete;

  public:
    /**
     * <h2> Background threads </h2>
     *
     * If I want to have a background threads constantly polling the MPI
     * queues, I may not spawn it here. The thread/job uses the boundary
     * data exchanger through a callback. Yet, the exchanger is not
     * completely constructed at this point. So we spawn it later.
     *
     * @see startToSendData()
     */
    BoundaryDataExchanger();

    /**
     *
     * <h2> The parameter _isCurrentlySending </h2>
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

    /**
     * This is called just before a new traversal, and it rolls over all
     * messages from the previous traversal. We communicate in a Jacobi-type
     * fashion, i.e. all data sent out in iteration n are actually used in
     * iteration n+1. Technically, we realise this as follows:
     *
     * - Send requests are enqueued into something we call send buffer and
     *   the boundary data exchanger issues MPI_ISends on these guys.
     * - When we call startToSendData(), i.e. before we start the subsequent
     *   traversal, this routine invokes releaseSentMessages() which
     *   effectively tests that MPI has delivered all data and then erases
     *   the send buffer.
     * - When we receive something (how this is done depends on the type of
     *   the boundary exchanger), we enqueue a task object into the receiver
     *   queue and trigger MPI_IRecv. This is done in iteration n or n+1.
     * - Once all send messages have left the system, startToSendData()
     *   invokes waitUntilNumberOfReceivedNeighbourMessagesEqualsNumberOfSentMessages().
     *   So we wait until at least all the messages from iteration n have
     *   arrived (releaseReceivedNeighbourMessagesRequests()). There might already be some messages dropping in from iteration
     *   n+1 if the neighbour rank is ahead, be we don't care here.
     * - Then we roll over the buffers, i.e. we make the old receive buffer a
     *   deploy buffer and continue.
     */
    void startToSendData(bool isTraversalInverted);
    void finishedToSendData(bool isTraversalInverted);

    /**
     *
     * <h2> Behaviour </h2>
     *
     * This operation consists of two parts. It allows the MPI engine to
     * progress and it looks out whether there are new messages arriving.
     *
     * <h3> Progress </h3>
     *
     * While MPI messages with the heap's tag are in the MPI queue (we check
     * through an iprobe on the meta data tag), the routine receives the meta
     * data. We work on the meta data as the heap can also be used to send out
     * zero data. In this case, no real data but only meta data might be
     * exchanged.
     *
     * It is important to receive the meta data with the sleep argument -1.
     * This way, Peano uses blocking MPI. Otherwise, the code would use a
     * non-blocking receive and invoke receiveDanglingMessages() until the
     * receive terminates. This would lead indirectly to recursive calls to
     * this operation and thus might mess up the receive order. Once the meta
     * data is in, we invoke handleAndQueueReceivedTask().
     *
     * The handleAndQueueReceivedTaks() routine has to enqueue the received
     * task immediately into _receiveTasks. Some additional steps then might
     * become necessary, before triggerReceive() on the task object is invoked.
     * No matter what exactly is done, you may not issue any routine that might
     * indirectly invoke receiveDanglingMessages() before the actual MPI
     * receive. Notably, you may not use Peano's/DaStGen's receive and send
     * operations which map logically blocking routines on their non-blocking
     * variants with busy polling.
     *
     * <h3> Progress </h3>
     *
     * Most MPI implementations cannot exchange data really  in the background.
     * They only do so if we call MPI_Test from time to time.
     *
     * <h3> Call points </h3>
     *
     * This operation is either called by any send or receive in Peano that is
     * logically blocking but does not return, or it is invoked through
     * waitUntilNumberOfReceivedNeighbourMessagesEqualsNumberOfSentMessages()
     * or releaseSentMessages() at the begin/end of a traversal.
     *
     * <h3> Background tasks </h3>
     *
     * While we do receive, we have to switch of the background tasks. Once we
     * are done, we should enable them if they had been enabled before. The
     * routine also is used in some release data phases where the background
     * tasks are suspended anyway. So we may not always switch background tasks
     * on again. We have to memorize the value and then restore this one. If we
     * are called by the background tasks, the whole thing is already protected
     * by a mutex (we ensure that noone changes the task's state while we get
     * in MPI data). So we may not switch ourself - this would otherwise
     * introduce a deadlock.
     *
     * @return Message did receive something.
     */
    bool receiveDanglingMessages();

    /**
     *
     *
     * <h2> MPI data exchange through background threads </h2>
     *
     * We have to switch off the background thread while we send out data. The
     * background threads run over the MPI requests and test them to allow MPI
     * to progress the data transfer. We however insert into exactly this data
     * structure. In return, we may not send out data while the background
     * thread is trying to finish MPI data.
     *
     * Background threads offer a switch statement. It uses an mutex to avoid
     * two concurrent switches. While the task calls receiveDanglingMessages()
     * the exactly same mutex is set to avoid that anybody changes the state.
     * So literally switching off the background threads before sending out
     * stuff is sufficient.
     */
    void sendData(
      const Data * const                                   data,
      int                                                  count,
      const tarch::la::Vector<DIMENSIONS, double>&         position,
      int                                                  level
    );

    /**
     * Receive data
     *
     * i.e. take data from the received buffer (it should be called deploy
     * buffer), copy it into the result data structure and send it back. The
     * copying might also induce some type conversion if only subsets of data
     * are actually exchanged via mpi.
     *
     * <h2> Validation </h2>
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
    VectorContainer receiveData(
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level
    );

    void plotStatistics() const;
    void clearStatistics();

    void shutdown();

    #ifdef Asserts
    /**
     * The standard boundary exchangers do augment their messages with
     * auxiliary data from which vertex this data stems. This allows us
     * to run assertions whether the right data has dropped in. While this is
     * the default, there are specialised boundary data exchangers that do not
     * store this auxiliary validation data and thus can't allow the superclass
     * to run its validation. By overloading this operation, they can
     * effectively switch those spatial assertions off.
     */
    virtual bool heapStoresVertexSpatialDataForValidation() const {
      return true;
    }
    #endif

    /**
     * There are boundary exchangers that do poll the MPI queues as they do
     * not know how many messages drop in a priori. Others know exactly which
     * and where to receive messages and thus switch of the polling which
     * means that the receiveDanglingMessages() operation only has to test
     * existing queries but doesn't have to poll for additional messages.
     *
     * The operation is const. Usually you either want the boundary exchanger
     * to do everything with polling or you insert your receive tasks manually.
     * In the latter case, you don't want the exchanger to interfere with your
     * operations.
     */
    virtual bool probeMPIQueues() const = 0;
};


//#ifdef Parallel
#include "peano/heap/BoundaryDataExchanger.cpph"
//#endif

#endif
