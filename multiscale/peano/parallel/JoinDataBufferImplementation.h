// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_JOIN_DATA_BUFFER_IMPLEMENTATION_H_
#define _PEANO_PARALLEL_JOIN_DATA_BUFFER_IMPLEMENTATION_H_


#include "peano/parallel/JoinDataBuffer.h"

#include <vector>


namespace peano {
  namespace parallel {
    template <class DataType>
    class JoinDataBufferImplementation;
  }
}


/**
 * Join data buffer implementation
 *
 * @see JoinDataBuffer
 *
 * @author Tobias Weinzierl
 */
template <class DataType>
class peano::parallel::JoinDataBufferImplementation: public peano::parallel::JoinDataBuffer {
  private:
    static tarch::logging::Log  _log;

    const bool          _isReceiveBuffer;
    const int           _bufferSize;
    const MPI_Datatype  _mpiDatatype;

    std::vector<DataType>  _receiveBuffer;
    DataType*              _sendBuffer;

    int                 _currentElement;

    int                 _rank;

    int                 _tag;

    #ifdef Debug
    /**
     * Is just for debugging
     */
    int                 _totalNumberOfElements;
    #endif

  public:
    JoinDataBufferImplementation(bool isReceiveBuffer, int bufferSize, const MPI_Datatype& mpiDatatype, int rank, int tag );

    virtual ~JoinDataBufferImplementation();

    virtual bool isReceiveBuffer() const;

    virtual bool isEmpty() const;

    virtual void receivePageIfAvailable();

    virtual void releaseMessages();

    virtual void removeTopElementFromStream();

    void send( const DataType& value );

    virtual std::string toString() const;

    DataType getTopElement();
};


#include "peano/parallel/JoinDataBufferImplementation.cpph"

#endif
