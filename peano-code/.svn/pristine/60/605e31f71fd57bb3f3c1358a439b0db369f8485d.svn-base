// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_JOIN_DATA_BUFFER_H_
#define _PEANO_PARALLEL_JOIN_DATA_BUFFER_H_


namespace peano {
  namespace parallel {
    class JoinDataBuffer;
  }
}



/**
 * Interface for join buffer
 *
 * A join data buffer is a distributed FIFO out, i.e. it is either a receive or
 * a send buffer locally. We can also consider the join data buffer to
 * implement a message queue where this data is either the well or the source.
 *
 * @see     JoinDataBufferImplementation
 *
 * @author  Tobias Weinzierl
 */
class peano::parallel::JoinDataBuffer {
  public:
    virtual ~JoinDataBuffer() {};

    /**
     * Is this a receive or a send buffer?
     */
    virtual bool isReceiveBuffer() const = 0;

    /**
     * Is the buffer empty? This operation works no matter whether it is a
     * receive or a send buffer.
     */
    virtual bool isEmpty() const = 0;

    /**
     * May not be called if the buffer is a send buffer. The operation probes
     * the MPI queue and appends the messages stored there (if there are
     * messages) to the local buffer.
     */
    virtual void receivePageIfAvailable() = 0;

    /**
     * Is undefined if the buffer is a receive buffer. Otherwise, it sends away
     * the remaining messages not sent yet.
     */
    virtual void releaseMessages() = 0;

    /**
     * This operation is only defined for the receive buffer variant.
     */
    virtual void removeTopElementFromStream() = 0;

    virtual std::string toString() const = 0;
};


#endif
