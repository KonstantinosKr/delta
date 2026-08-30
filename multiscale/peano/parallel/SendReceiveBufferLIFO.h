// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_SEND_RECEIVE_BUFFER_LIFO_H_
#define _PEANO_PARALLEL_SEND_RECEIVE_BUFFER_LIFO_H_

#include "peano/parallel/SendReceiveBufferAbstractImplementation.h"
#include <vector>


namespace peano {
  namespace parallel {
    template <class Vertex>
    class SendReceiveBufferLIFO;
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
class peano::parallel::SendReceiveBufferLIFO: public peano::parallel::SendReceiveBufferAbstractImplementation<Vertex> {
  private:
    /**
     * Log device.
     */
    static tarch::logging::Log _log;

    typedef peano::parallel::SendReceiveBufferAbstractImplementation<Vertex> Base;

  protected:
    virtual void updateDeployCounterDueToSwitchReceiveAndDeployBuffer();
    virtual void moveDeployBufferPointerDueToGetVertex();

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
    SendReceiveBufferLIFO( int toRank, int bufferSize );

    virtual ~SendReceiveBufferLIFO();

    /**
     * Return Number of Deployed Messages
     *
     * Returns the number of deployed messages, that is how many messages are
     * deployed since the last releaseMessage() call. This operation is used
     * exclusively by the test cases.
     */
    int getNumberOfDeployedMessages() const;
};

#ifdef ParallelExchangePackedRecordsAtBoundary
#pragma pack (pop)
#endif

#include "peano/parallel/SendReceiveBufferLIFO.cpph"


#endif
