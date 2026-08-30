// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_SEND_RECEIVE_BUFFER_POOL_H_
#define _PEANO_PARALLEL_SEND_RECEIVE_BUFFER_POOL_H_


#include "tarch/logging/Log.h"

#include "tarch/services/Service.h"

#include "tarch/compiler/CompilerSpecificSettings.h"

#include "peano/parallel/SendReceiveBuffer.h"
#include "peano/utils/PeanoOptimisations.h"

#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/multicore/BooleanSemaphore.h"




#include <map>

namespace peano {
  namespace parallel {
    class SendReceiveBufferPool;
  }
}


/**
 * Send/Receive Buffer Pool
 *
 * This class is the single point of contact to exchange PDE-specific data.
 * It stores all the SendReceiveBuffer instances belonging to one node.
 *
 * Furthermore, the class is responsible for distributing all the vertices
 * among the different receive buffers, i.e. it analyses the vertex's
 * information which subdomains are adjacent. Consequently, you may not pass it
 * vertices that are not part of the parallel boundary.
 *
 * The buffer management is a lazy management, i.e. buffers required are
 * created on demand.
 *
 * @author Tobias Weinzierl
 */
class peano::parallel::SendReceiveBufferPool: public tarch::services::Service {
  public:
    enum BufferAccessType {
      LIFO,FIFO
    };
  private:
    enum SendReceiveMode {
      SendAndDeploy,
      DeployButDoNotSend,
      SendButDoNotDeploy,
      NeitherDeployNorSend
    };


    struct BackgroundThread {
      public:
        enum class State {
          Running,
	      Terminate
        };

        static std::string toString(State state);
      private:
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
        BackgroundThread();
        virtual ~BackgroundThread();

        /**
         * The operator returns true as long as the state equals Running. For
         * TBB (for example) this means that the job is processed and afterward
         * directly again enqueued into the job list.
         *
         * <h2> Optimisation </h2>
         *
         * I tried an additional guard around this operator which probes the
         * respective MPI tags. It turned out that this is a poor idea. Also,
         * invoking yield() in a TBB context did not pay off. What however
         * turned out to be absolutely crucial is the value of IprobeEveryKIterations.
         * If it is too big, the code tends to run into timeouts. If it is too
         * small, the MPI data exchange seems to suffer from the MPI call
         * overheads and it starts to deadlock, too. So we kind of try to adopt
         * it manually throughout the runs.
         *
         * The second insight stems from Philipp Samfass. The whole idea to get
         * in data in the background is fine, as long as
         */
        bool operator()();
        std::string toString() const;

        /**
         * @see _semaphore
         */
        void terminate();
    };

    #if defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
    /**
     * We hold a pointer to the background thread object, but we do not delete
     * it as it is passed into Peano's job interface. This job interface has
     * the responsibility to delete all job objects.
     */
    BackgroundThread* _backgroundThread;
    tarch::multicore::BooleanSemaphore   _backgroundThreadSemaphore;
    #endif

    void terminateBackgroundThread();
    void disconnectBackgroundThread();

    static tarch::logging::Log _log;

    /**
     * Set by the constructor and then never changed again.
     */
    int  _iterationManagementTag;

    /**
     * Set by the constructor and then never changed again.
     */
    int  _iterationDataTag;

    /**
     * Maps ranks to buffers.
     */
    std::map<int,SendReceiveBuffer*> _map;

    /**
     * By default 1.
     */
    int _bufferSize;

    SendReceiveMode _mode;

    SendReceiveBufferPool();

    /**
     * Nobody is allowed to copy the pool.
     */
    SendReceiveBufferPool(const SendReceiveBufferPool& copy) {}

    static std::string toString( SendReceiveMode  mode);

    bool receiveDanglingMessagesIfAvailable();
  public:
    /**
     * Delete all the buffers not yet deleted.
     *
     * For the warning, I use std::cerr, as the logging might already be shut
     * down when the destructor of the pool is invoked.
     */
    ~SendReceiveBufferPool();

    static SendReceiveBufferPool& getInstance();

    /**
     * Means that all buffers are cleared and freed (deleted). As the pool
     * realises a lazy creation pattern, it might make sense to call this
     * operation whenever the rank has joined into its father and accepts a
     * new job.
     */
    void terminate();

    /**
     * Create buffer manually
     *
     * If we have a spacetree node with a new master, the following situation
     * might occur (and does occur):
     *
     * - The master books the new worker and finishes this iteration
     * - The master starts the new iteration and writes several messages (both
     *   boundary and fork) to the new worker
     * - The master sends the startup message
     * - The master continues to send boundary and fork/join data to its worker
     *
     * If the worker now cannot handle that many messages as it runs out of
     * buffers, this startup messages is somehow hidden in all the fork and
     * boundary data. At the same time, the worker has not ever received any
     * data from the master, i.e. it hasn't even created the corresponding
     * buffers. We'll thus run into a deadlock.
     *
     * Usually, this operation is used with the argument
     * peano::parallel::SendReceiveBufferPool::LIFO by users. Please do not
     * forget also to create heap buffers manually in case you use heaps.
     *
     *
     * @param toRank Usually always the master process if you call it
     *               externally. However, I also use this method inside the
     *               class and there it may have a different value.
     */
    template <class Vertex>
    void createBufferManually(int toRank, const BufferAccessType& bufferAccessType );
    
    /**
     * Restart the Node Pool.
     *
     * As the buffer implements a lazy behaviour, this operation does not create
     * new buffers.
     *
     * For each restart(), the user has to call terminate() before. Consequently,
     * the maps with the send and receive buffers has to be empty when restart()
     * is invoked. For the Peano repositories this implies the following
     * constraint: the initialisation of the (regular grid) data containers
     * sends away vertices. Hence, the initialisation implies the creation of
     * send buffers. Hence, the reset of the pool has to be done #before# the
     * data container is initialised.
     */
    void restart();

    /**
     * This tag is used to send and receive the states and cells throughout
     * normal iterations. The repository states, i.e. which adapter to use,
     * are also sent due to this tag. Cells are not exchanged by the regular
     * grid.
     */
    int getIterationManagementTag() const;

    /**
     * Exchange data of the non-overlapping boundary.
     */
    int getIterationDataTag() const;

    /**
     * Poll the MPI queues whether there are messages pending. If so, take them
     * and insert them into the local queue.
     *
     * Right now, the operation only searches for messages from buffers where
     * it knows, that the local node exchanges messages with this rank. However,
     * it might be that there are already messages from other nodes (due to an
     * additional fork or join) that are not yet known to be communication
     * partners. The original code of Peano 1 took care of this behaviour and
     * inserted something like:
     * \code

  if (!receivedPage) {
    MPI_Status status;
    int        flag;
    int probeResult = MPI_Iprobe(
      MPI_ANY_SOURCE,
      Vertex::DataExchangeTag,
      Node::getInstance().getCommunicator(),
      &flag, &status
    );
    if (probeResult!=MPI_SUCCESS) {
      std::ostringstream msg;
      msg << "probing for dangling messages failed: "
          << MPIReturnValueToString(probeResult);
      _log.error("receiveDanglingMessages()", msg.str() );
    }
    if (flag && _map.count(status.MPI_SOURCE)==0 ) {
      #ifdef Debug
      std::ostringstream msg;
      msg << "there's a message from node " << status.MPI_SOURCE
          << ", but there's no buffer. Create buffer";
      _log.debug("receiveDanglingMessages()", msg.str() );
      #endif
      createBuffer( status.MPI_SOURCE );
      _map[ status.MPI_SOURCE ].receivePageIfAvailable();
    }
  }

  \endcode
     *
     * It did not transfer this piece of code into Peano's second release, as I
     * have to know the vertex type to create a new buffer. The vertex type
     * however ain't known in receiveDanglingMessages(). So I just removed this
     * code fragment and hope that the code does not run into a deadlock.
     */
    virtual void receiveDanglingMessages();

    /**
     * Releases all the messages. Should be called after every iteration. The
     * operation runs through all the buffers and calls release for each of
     * them. First, the sent messages are released, then, the node has to wait
     * until all the required messages for the next iteration are received.
     * Both steps might in turn invoke indirectly receiveDanglingMessages() due
     * to the Node services. Consequently, the implementation can reduce to a
     * simple wait for enough messages - it does not have to actively receive
     * messages (this is done due to receiveDanglingMessages() anyway).
     *
     * It basically does not matter in which order we run through the sent
     * messages, as all of them have to be released prior to the receive
     * process. For the receive order I wanted to use a reverse operator.  My
     * assumption has been that the smaller the rank the more messages we receive
     * from it. If this holds and if we would receive smaller ranks first, all
     * the data from bigger ranks would be on place already when we try this one,
     * and all statements on `how long do I have to wait` are irrelevant. However,
     * I want to remove buffers if they are used anymore. And C++'s erase is
     * only defined on (forward) iterators. So this means I use this one.
     *
     * <h2> Multithreading </h2>
     *
     * The releaseMessages() call terminates the background task. We may assume
     * that no further messages drop in after we have released all messages.
     * The subsequent sends then however will issue a new thread.
     */
    void releaseMessages();

    /**
     * Sends a message to the destination node. The vertex might be buffered, so
     * no send is triggered immediately. This operation also implements the lazy
     * buffer creation, i.e. if a vertex is sent to a rank for which no buffer
     * exists yet, it creates this buffer. This is a fundamental difference to
     * the corresponding receive operation.
     *
     * As the operation also creates the buffers, it has to know how one will
     * access the buffer, i.e. in a LIFO or FIFO order.
     */
    template <class Vertex>
    void sendVertex( const Vertex& vertex, int toRank, const BufferAccessType& bufferAccessType );

    /**
     * Returns the next element from the receive buffer. Internally there are
     * two receive buffers to make the program able to do asynchronous
     * receives. So the receive buffers are called receive and deploy buffer and
     * this operation returns the elements of the deploy buffer. The deploy
     * buffer is read in a stack manner, i.e. from right to left.
     *
     * You are never allowed to receive a vertex from a rank to which you
     * haven't sent a vertex before.
     *
     * The operation ain't a const operation at the getter might and will
     * trigger reorganisations and state changes of the underlying buffers.
     */
    template <class Vertex>
    Vertex getVertex(int fromRank);

    /**
     * Set a new buffer size.
     */
    void setBufferSize( int bufferSize );

    /**
     * Switch data send and received on/off.
     *
     * The operation affects both the pool's behaviour and the result of
     * deploysValidData(). The result of the latter operation however changes
     * with one traversal delay - if you switch off the sends, the pool
     * continues to deliver valid data (from the previous iteration), but then
     * stops to hold data in the subsequent iteration.
     *
     * @see Autogenerated documentation of any Repository interface.
     *
     * @image html peano/parallel/SendReceiveBufferPoolModes.png
     */
    void exchangeBoundaryVertices(bool value);

    /**
     * If the send receive buffer pool is in a mode where it actually does not
     * deploy, this operation returns false. In this case, do not try to take
     * data from the pool and to merge it into your local data structures.
     *
     * @see exchangeBoundaryVertices(bool)
     */
    bool deploysValidData() const;
};

#include "peano/parallel/SendReceiveBufferPool.cpph"


#endif
