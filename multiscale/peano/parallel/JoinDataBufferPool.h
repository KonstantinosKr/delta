// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_JOIN_DATA_BUFFER_POOL_H_
#define _PEANO_PARALLEL_JOIN_DATA_BUFFER_POOL_H_

#include "tarch/logging/Log.h"
#include "tarch/services/Service.h"
#include "tarch/compiler/CompilerSpecificSettings.h"
#include "peano/utils/PeanoOptimisations.h"

#include "peano/utils/Globals.h"
#include "peano/parallel/JoinDataBuffer.h"

#include <map>
#include <vector>
#include <bitset>


namespace peano {
  namespace parallel {
    class JoinDataBufferPool;
  }
}



/**
 * Join Data Buffer
 *
 * !!! Buffer lifecycle
 *
 * You can create buffers manually. They are destroyed upon a releaseMessages()
 * call if they are empty, i.e. the destruction happens automatically in the
 * background.
 *
 * The manually creation is required if and only if you wanna rely on MPI
 * message polling. Otherwise, the buffers are created on-the-fly as soon as
 * someone asks for vertices/cells/flags or wants to send some.
 *
 * @image html peano/parallel/JoinDataBuffer.png
 *
 * @author Tobias Weinzierl
 */
class peano::parallel::JoinDataBufferPool: public tarch::services::Service {
  private:
    static tarch::logging::Log _log;

    #if defined(Debug) && (defined(Dim2) || defined(Dim3) || defined(Dim4))
    static const int OffsetForAdditionalCellLevelEncoding;
    #endif

    struct BufferSet {
      JoinDataBuffer*  _vertexBuffer;
      JoinDataBuffer*  _cellBuffer;
      JoinDataBuffer*  _cellMarkerBuffer;

      BufferSet();
    };

    /**
     * Maps ranks to buffers.
     */
    std::map<int,BufferSet>   _map;

    /**
     * By default 1.
     */
    int _bufferSize;

    int _vertexTag;
    int _cellTag;
    int _cellMarkerTag;

    /**
     * It is yet another singleton.
     */
    JoinDataBufferPool();

    int    getRawCellMarkerFromStream(int fromRank);

    /**
     * Invoked by getRawCellMarkerFromStream()
     */
    void createCellMarkerBufferManually(bool isReceiveBuffer, int fromOrToRank);
  public:
    static JoinDataBufferPool& getInstance();

    virtual void receiveDanglingMessages();

    /**
     * Release the messages and remove the buffers
     *
     * We can first send away all messages that were not sent before. Then, we
     * can erase the message buffers, as they will not be needed anymore.
     */
    void releaseMessages();

    /**
     * See SendReceiveBufferPool
     */
    template <class Vertex>
    void createVertexBufferManually(bool isReceiveBuffer, int toRank);

    template <class Cell>
    void createCellBufferManually(bool isReceiveBuffer, int toRank);


    template <class Vertex>
    void   sendVertex( const Vertex& vertex, int toRank );

    /**
     * Send a cell
     *
     * Each cell should be sent together with a marker telling the receiver
     * which vertices are adjacent to the cell that in turn again are adjacent
     * to the master's partition. This way, the receiver first
     * can receive the cell together with its marker. Due to the marker, it
     * then knows how many vertices are yet to come.
     *
     * @param cellMarker Integer that tells you which adjacent vertices are
     *                   in turn adjacent to the master.
     */
    template <class Cell>
    void   sendCell( const Cell&  cell, const std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT>&  cellMarker, int toRank );

    /**
     * Return next vertex in a FIFO manner. This operation does not remove the
     * vertex from the stream, i.e. two subsequent calls to the function return
     * the same value.
     */
    template <class Vertex>
    Vertex getVertexFromStream(int fromRank);

    /**
     * Return next vertex in a FIFO manner. This operation does not remove the
     * vertex from the stream, i.e. two subsequent calls to the function return
     * the same value.
     */
    template <class Cell>
    Cell   getCellFromStream(int fromRank);

    /**
     * Return next vertex in a FIFO manner. This operation does not remove the
     * vertex from the stream, i.e. two subsequent calls to the function return
     * the same value.
     */
    std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT>  getCellMarkerFromStream(int fromRank);

    #if defined(Debug)
    int    getCellLevelFromStream(int fromRank);
    #endif

    void removeVertexFromStream(int rank);
    void removeCellFromStream(int rank);
    void removeCellMarkerFromStream(int rank, bool calledOnMasterJoiningWithItsWorker);
    void removeUnusedAdjacentFlags(int rank);

    /**
     * Set a new buffer size.
     */
    void setBufferSize( int bufferSize );
};


#include "peano/parallel/JoinDataBufferPool.cpph"

#endif
