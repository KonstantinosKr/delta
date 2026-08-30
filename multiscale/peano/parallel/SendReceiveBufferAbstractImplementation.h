// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_SEND_RECEIVE_BUFFER_IMPLEMENTATION_H_
#define _PEANO_PARALLEL_SEND_RECEIVE_BUFFER_IMPLEMENTATION_H_


#include "peano/parallel/SendReceiveBuffer.h"
#include "peano/utils/PeanoOptimisations.h"

#include <vector>

#include "tarch/parallel/MPIConstants.h"

namespace peano {
  namespace parallel {
    template <class Vertex>
    class SendReceiveBufferAbstractImplementation;
  }
}


#ifdef ParallelExchangePackedRecordsAtBoundary
   #pragma pack (push, 1)
#endif



/**
 * Representates one send and one receive buffer for the vertices of one communication partner.
 *
 * The send buffer semantics is a buffered send: So all the messages are
 * accumulated within a buffer - a so called page. Everytime the buffer size
 * exceeds a sive given by _bufferPageSize or the user calls releaseMessages(),
 * the buffer is sent non-blocking to the receiver.
 *
 * For the receive buffer one has to state several issues:
 * - The send buffer is read as a stack. Therefore the complete data received
 *   has to be cached (the last message is read first).
 * - The number of messages to be received per iteration is not fixed a priori.
 * - The receive buffer has to be copied before it is used again (cache
 *   property). This has to be done when releaseMessages() is called.
 * - The number of sent messages equals the number of receive messages when the
 *   current iteration terminates.
 * - The received data is bundled into chunks of messages of size
 *   _bufferPageSize. These chunks are organized as pages. The last message
 *   chunk might be smaller than _bufferPageSize.
 * - The receiving can be done assynchronous compared to the send process.
 *
 * So here's the receive algorithm:
 * - Everytime one sends a message block, a corresponding receive block is
 *   created (the class provides array-pooling so actually there won't be a
 *   new alloc).
 * - Everytime one sends a message block, it is checked whether there's a
 *   message block ready to receive. If this is the case, the receive is called
 *   immediatly (non-blocking communication).
 * - On a releaseMessage() call, the number of received messages is compared to
 *   the number of sent messages. As long as the number is smaller, blocking
 *   receive commands are called. Furthermore, all messages stored in the the
 *   send buffer are send. Tnis is the only case a message smaller than
 *   _bufferPageSize is sent.
 * - The receive buffer is a swap buffer: The data is not copied on a
 *   releaseMessage() call but the swap buffers are interchanged.
 * - The deploy buffer (the receive buffer right now the program is reading from
 *   but not receiving into) is a stack, i.e. the buffer is read from right to
 *   left.
 *
 * Please note that the vertex exchange is realised in an assynchronous way
 * whereas the iteration management (master-slave communication) is done in a
 * synchronous manner. See e.g. ForkMessage or MPIVertexWrapper. As a
 * result, different tags for the SendReceiveBuffer and the synchronous messages
 * have to be used. Use the property CommunicationTag of the concerned classes.
 *
 * The send/receive buffers use the communication tag 1.
 *
 * @image html peano/parallel/parallel_SendReceiveBuffer.gif
 *
 * @image html peano/parallel/parallel_SendReceiveBuffer_Functionality.png
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.39 $
 */
template <class Vertex>
class peano::parallel::SendReceiveBufferAbstractImplementation: public peano::parallel::SendReceiveBuffer {
  protected:
    /**
     * Log device.
     */
    static tarch::logging::Log _log;

    /**
     * Maximal size of the send and the receive buffer.
     */
    int _bufferPageSize;

    /**
     * Number of elements sent already.
     */
    int _numberOfElementsSent;


    /**
     * Handle for the non-blocking receive calls. According to
     * http://www.netlib.org/utk/papers/mpi-book/node50.html#SECTION00383000000000000000
     * one has to reallocate the request buffer whenever it is used.
     */
    MPI_Request* _sendBufferRequestHandle;

    /**
     * Handle for receive calls
     *
     * Handle for the non-blocking receive calls. According to
     * http://www.netlib.org/utk/papers/mpi-book/node50.html#SECTION00383000000000000000
     * one has to reallocate the request buffer whenever it is used. Whenever
     * receiveDanglingMessages() finds new messages in the mpi queue, it
     * triggers the receive of those in the background. Afterward, no further
     * messages are received until this data exchange has terminated, i.e. until
     * the handle is released.
     */
    MPI_Request* _receiveBufferRequestHandle;

    /**
     * Send buffer.
     */
    #if defined(ParallelExchangePackedRecordsAtBoundary)
    typedef typename Vertex::Records::Packed       MPIDatatypeContainer;
    #else
    typedef typename Vertex::Records               MPIDatatypeContainer;
    #endif

    MPIDatatypeContainer* _sendBuffer[2];

    /**
     * Determines how many elements within the current send buffer page are
     * written. If this counter equals _bufferPageSize, the send buffer is sent.
     */
    int _sendBufferCurrentPageElement;

    /**
     * Current send buffer (0 or 1)
     *
     * The send buffer is actually two send buffers. Whenever one of them is
     * full, Peano triggers the send and continues to work with the other
     * buffer instead. This way, the send in the background can last until the
     * buffers are to be swapped again.
     */
    int _currentSendBuffer;

    /**
     * There are two receive buffers: One the application reads from and one
     * that is used for the assynchronous receive operations. The size of both
     * buffers is exactly the same.
     *
     * Each buffer is a sequence of arrays. Each array has the size
     * _bufferPageSize.
     */
    std::vector<MPIDatatypeContainer*> _receiveBuffer[2];

    /**
     * Number of the node this send / receive buffer is communicating with.
     */
    int _destinationNodeNumber;

    /**
     * Equals either 0 or 1. It determines the receive buffer to use, to
     * 1-_currentReceiveBuffer identifies the current deploy buffer.
     */
    int _currentReceiveBuffer;

    /**
     * Holds the total number of messages within the deploy buffer. That number
     * has to be smaller equal the number of messages space would be available.
     * Nevertheless, this is a message counter, not a space counter.
     */
    int _sizeOfDeployBuffer;

    /**
     * Holds the total number of messages within the receive buffer. That number
     * has to be smaller equal the number of messages space would be available.
     * Nevertheless, this is a message counter, not a space counter.
     */
    int _sizeOfReceiveBuffer;

    /**
     * Current receive page. Note that the receive buffer is filled from left
     * to right, i.e. this counter is an increasing one. Since the messages are
     * send en block (see _bufferPageSize) there's no need for an attribute
     * _currrentReceiveBufferElement. The counter points to the first free page
     * available, i.e. if it points to 0 no pages have been received.
     */
    int _currentReceiveBufferPage;

    /**
     * Whenever the application wants to send a vertex but the buffer isn't
     * freed by MPI yet, it has to wait. This overhead is tracked in this
     * variable.
     */
    clock_t _sendDataWaitTime;

    /**
     * Current active page within the deploy buffer. Note the deploy buffer is
     * read from right to left.
     */
    int _currentDeployBufferPage;

    /**
     * Current active element within the deploy buffer. Note the deploy buffer
     * is read from right to left. If the counter equals -1, there is no active
     * element within the current page, if the counter equals 0 the first
     * element of current page is to be delivered on the next method call.
     */
    int _currentDeployBufferElement;

    /**
     * Sends all messages contained within the send buffer (see
     * _sendBufferCurrentPageElement ) to the destination node.
     */
    void sendBuffer();

    /**
     * Wait until all send tasks have finished.
     *
     * Actually, there's at most one send task active at a time, and it is
     * always the one that's not corresponding to the active send buffer, as
     * the active send buffer is flushed with messages.
     */
    void finishOngoingSendTask();

    /**
     * Switch Receive And Deploy Buffer
     *
     * Switches the receive and the deploy buffer. If the number of received
     * messages and sent messages is equal, it is sufficient to interchange
     * receive and deploy buffer. Yet, iterations might overlap and neighbour
     * messages might (in the same iteration) send messages before the current
     * process starts to send anything. The receiving node is not able to
     * differ between the two cases, so both cases are allowed (see
     * receivePageIfAvailable()).
     *
     * The number of received messages hence might outnumber the sent messages.
     * In this case, only getNumberOfSentMessages() are to be transferred into
     * the deploy buffer. The remaining messages are moved to the beginning of
     * the receive buffer.
     *
     * As the operation tries to avoid copying, it interchanges deploy and
     * receive buffers. If the number of received messages exceeds the number of
     * sent messages, the additional messages thus have to be copied from the
     * receive buffer into the deploy buffer before the two buffers are
     * interchanged.
     *
     * @image html peano/parallel/parallel_SendReceiveBuffer_receivePageIfAvailable.png
     *
     * !!! Algorithm
     *
     * - If neither vertices are sent nor vertices received, return.
     * - Determine number of pages to transfer: It is the number of sent
     *   messages divided by buffer size. As it is integer arithmetics, use the
     *   upper Gaussian bracket.
     * - Determine deploy buffer properties.
     * - Copy additional blocks from receive buffer to deploy buffer. Start at
     *   index 0 at deploy buffer.
     * - _currentReceiveBufferPage is set to number of blocks copied to deploy
     *   buffer.
     * - Decrement _sizeOfReceiveBuffer by the number of sent messages.
     * - _numberOfElementsSent becomes zero.
     * - Interchange deploy and receive buffer.
     *
     */
    virtual void switchReceiveAndDeployBuffer();

    /**
     * Add another pair of send and receive buffers.
     */
    void addAdditionalReceiveDeployBuffer();

  protected:

    virtual void updateDeployCounterDueToSwitchReceiveAndDeployBuffer() = 0;
    virtual void moveDeployBufferPointerDueToGetVertex() = 0;

  public:
    /**
     * Allocates the send and receive array and sets the initial values for
     * the attributes.
     *
     * The bufferSize could be taken from the Node singleton instance, too, but
     * to be able to code simple test cases, it has to be passed explicitely to
     * the object.
     *
     * @param destinationNodeNumber number of node this buffer is
     *                              communicating with.
     * @param bufferSize            maximal size of buffer to use.
     */
    SendReceiveBufferAbstractImplementation( int toRank, int bufferSize );

    /**
     * Destructur
     *
     * Tehe destructor deletes all the buffer memory on the heap. Before, it
     * polls for outstanding messages in the MPI queues. I'm not sure whether
     * there'll ever be outstanding messages, as the terminate() command
     * should be called before the buffers are destroyed. Before terminate()
     * in turn, we usually do a releaseMesssages(). And releaseMessages()
     * ensures that there are not MPI messages hanging in the queues which
     * holds if no other ranks already started to iterate once more (which
     * does not apply, if the simulation shuts down).
     */
    virtual ~SendReceiveBufferAbstractImplementation();

    /**
     * Sends a message to the destination node. The vertex might be buffered, so
     * no send is triggered immediately.
     */
    void sendVertex(const Vertex& vertex);

    /**
     * Get Next Vertex
     *
     * Returns the next element from the receive buffer. Internally there are
     * two receive buffers to make the program able to do asynchronous
     * receives. So the receive buffers are called receive and deploy buffer and
     * this operation returns the elements of the deploy buffer. The deploy
     * buffer is either read in a stack manner, i.e. from right to left, or as
     * FIFO buffer from left to right. This depends on the original
     * configuration of the buffer.
     *
     * !!! Assertions
     * One assertion tests that there are really only one receive buffer and
     * one deploy buffer. The second assertion ensures that there is a deploy
     * buffer page availabe: the buffers are organised into pages each holding
     * a fixed number of messages, i.e. if there are no pages there are no
     * messages left. The third assertion finally ensures that on the current
     * page there are messages left. Otherwise, this page had to be thrown
     * away before.
     */
    Vertex getVertex();

    /**
     * This is just a debug counter to check how many sendVertex() operations
     * have been called.
     */
    int getNumberOfSentMessages() const override;

    /**
     * Return Size of Receive Buffer
     *
     * Returns the size of the receive buffer. This number is greater equal the
     * operation getNumberOfReceivedMessages().
     */
    int getSizeOfReceiveBuffer() const;

    bool receivePageIfAvailable() override;
    int getNumberOfReceivedMessages() const override;
    void releaseSentMessages() override;
    void releaseReceivedMessages(bool) override;
};

#ifdef ParallelExchangePackedRecordsAtBoundary
#pragma pack (pop)
#endif

#include "peano/parallel/SendReceiveBufferAbstractImplementation.cpph"

#endif

