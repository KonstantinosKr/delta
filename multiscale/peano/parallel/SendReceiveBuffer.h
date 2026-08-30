// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_SEND_RECEIVE_BUFFER_H_
#define _PEANO_PARALLEL_SEND_RECEIVE_BUFFER_H_


namespace peano {
  namespace parallel {
    class SendReceiveBuffer;
  }
}


/**
 * Abstract base class of all send receive buffers.
 *
 * This is an abstract base class for all send receive buffers that was
 * introduced for technical reasons. A buffer is a template, i.e. different
 * buffers belonging to different data types are completely unrelated types.
 * The pool has a map holding all these send receive buffers. Consequently, all
 * these template classes need a common superclass. Well, here it is.
 *
 * The perhaps most interesting fact about these buffers is their swap
 * mechanism. It is invoked by the pool's releaseMessages() operation. This one
 * calls releaseSentMessages() and releaseReceivedMessages(), i.e. the swap is
 * split up into two methods. For a more detailed description of these two
 * operations, switch to their description.
 */
class peano::parallel::SendReceiveBuffer {
  public:
    virtual ~SendReceiveBuffer() {}

    /**
     * Receive a Block of Messages if One is Available
     *
     * Checks in a non-blocking way wheather there's a new page of messages
     * available. If this is the case, the current receive page is filled with
     * this messages (assynchronous) and the current receive page counter
     * _currentReceiveBufferPage is increased.
     *
     * @image html peano/parallel/parallel_SendReceiveBuffer_receivePageIfAvailable.png
     *
     * Each new message block is stored in a new buffer page. If two iterations
     * overlap, one line in the receive buffer thus is not filled completely.
     *
     * @return A message has been received
     */
    virtual bool receivePageIfAvailable() = 0;

    /**
     * Return Number of Received Messages
     *
     * Returns the number of received messages, that is how many messages are
     * received since the last releaseMessage() call in an asynchronous way.
     * Added to this number are the received messages not copied from the
     * receive buffer to the deploy buffer last time.
     */
    virtual int getNumberOfReceivedMessages() const = 0;

    /**
     * Release Sent Messages
     *
     * The release message process is split up into two operations:
     * releaseSentMessages() and releaseReceivedMessages(). Both have to be
     * called by a release process with releaseSentMessages() being the first
     * operation.
     *
     * !!! Release Process
     *
     * Releases the messages, i.e. sends away all messages still stored in the
     * outgoing buffers. Afterwards, the operation waits until at least the
     * same number of messages has been received. Finally, the deploy and the
     * receive buffer are interchanged via switchReceiveAndDeployBuffer().
     *
     * !!! Release Sent Messages
     *
     * Wait until all the send buffers are emptied. Meanwhile, the operation has
     * to receive dangling messages from the receive queue. Otherwise, deadlocks
     * might occur. Since the operation is build atop of
     * finishOngoingSendTask(), finishOngoingSendTask() takes care of the
     * deadlock cchallenge.
     *
     * @see releaseReceivedMessages()
     */
    virtual void releaseSentMessages() = 0;

    /**
     * Release Sent Messages
     *
     * The release message process is split up into two operations:
     * releaseSentMessages() and releaseReceivedMessages(). Both have to be
     * called by a release process with releaseSentMessages() being the first
     * operation.
     *
     * !!! Release Process
     *
     * Releases the messages, i.e. sends away all messages still stored in the
     * outgoing buffers. Afterwards, the operation waits until at least the
     * same number of messages has been received. Finally, the deploy and the
     * receive buffer are interchanged via switchReceiveAndDeployBuffer().
     *
     * !!! Release Received Messages
     *
     * The release process terminates, i.e. it returns true, if the number of
     * received messages is greater or equal the number of sent messages.
     * Afterwards, it switches receive and deploy buffers via
     * switchReceiveAndDeployBuffer(). As long as one send-receive buffer
     * returns false, the superviser of the release process has to continue to
     * invoke release messages.
     *
     * Within the wait process, the send receive buffer pool is invoked to avoid
     * deadlocks: On some systems (Infiniband, e.g.) big messages with another
     * tag or from another node might block the incoming messages
     * releaseMessages() is waiting for. Node's receiveDanglingMessages() would
     * buffer such blocking messages. To be able to do this, Node polls all the
     * incoming messages and creates incoming buffers for messages if the
     * buffers do not exist via the buffer pool - it creates receive buffers
     * on-the-fly.
     *
     * @image html peano/parallel/BufferSwapProcess.png
     *
     * The buffer pool is a singleton. Thus, the buffers are singletons, too.
     * For some test cases I create buffers all alone, i.e. these buffers are
     * not managed by the pool. The deadlock aspect now might create an
     * additional buffer throughout the MPI polling. For testing, I thus
     * introduced this bool that switches off the deadlock aspect. Outside of
     * the test cases, the parameter holds.
     *
     * @see releaseSentMessages()
     * @return Release has finished.
     */
    virtual void releaseReceivedMessages(bool) = 0;

    virtual int getNumberOfSentMessages() const = 0;
};


#endif
