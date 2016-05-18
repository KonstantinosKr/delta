// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_HEAP_H_
#define _PEANO_HEAP_HEAP_H_

#include <map>
#include <vector>
#include <list>

#include "peano/heap/AbstractHeap.h"
#include "peano/heap/SendReceiveTask.h"
#include "peano/heap/SynchronousDataExchanger.h"
#include "peano/heap/PlainBoundaryDataExchanger.h"
#include "peano/heap/RLEBoundaryDataExchanger.h"

#include "peano/heap/HeapAllocator.h"
#include "peano/heap/records/FloatHeapData.h"
#include "peano/heap/records/CharHeapData.h"
#include "peano/heap/records/DoubleHeapData.h"
#include "peano/heap/records/BooleanHeapData.h"
#include "peano/heap/records/IntegerHeapData.h"

#include "peano/utils/Globals.h"

#include "tarch/logging/Log.h"

#include "tarch/services/Service.h"


namespace peano {
  namespace heap {
    template<
      class Data,
      class MasterWorkerExchanger,
      class JoinForkExchanger,
      class NeighbourDataExchanger
    >
    class Heap;

    /**
     * Heap with standard configurations to enable users to work only with
     * one template arguments instead of four.
     */
    template<class Data>
    class PlainHeap: public Heap<
      Data,
      SynchronousDataExchanger< Data, true >,
      SynchronousDataExchanger< Data, true >,
      PlainBoundaryDataExchanger< Data, true >
    > {
      public:
        virtual ~PlainHeap() {}
    };

    template<class Data>
    class RLEHeap: public Heap<
      Data,
      SynchronousDataExchanger< Data, true >,
      SynchronousDataExchanger< Data, true >,
      RLEBoundaryDataExchanger< Data, true >
    > {
      public:
        virtual ~RLEHeap() {}
    };

    template<class Data>
    class PlainHeapWithoutDataCopyingForBoundarySends: public Heap<
      Data,
      SynchronousDataExchanger< Data, true >,
      SynchronousDataExchanger< Data, true >,
      PlainBoundaryDataExchanger< Data, false >
    > {
      public:
        virtual ~PlainHeapWithoutDataCopyingForBoundarySends() {}
    };

    template<class Data>
    class RLEHeapWithoutDataCopyingForBoundarySends: public Heap<
      Data,
      SynchronousDataExchanger< Data, true >,
      SynchronousDataExchanger< Data, true >,
      RLEBoundaryDataExchanger< Data, false >
    > {
      public:
        virtual ~RLEHeapWithoutDataCopyingForBoundarySends() {}
    };

    /**
     * Plain heap over integers.
     *
     * I recommend to use this for first prototyping or applications with a low
     * communication memory footprint. It is pretty fast. However, I also
     * observe that this heap implementation plains to deadlock quickly for big
     * data sets where lots of messages are exchanged. In that case, I
     * recommend to use the RLEHeap below.
     */
    typedef PlainHeap<peano::heap::records::IntegerHeapData>  PlainIntegerHeap;
    typedef RLEHeap<peano::heap::records::IntegerHeapData>    RLEIntegerHeap;


    /**
     * Plean heap over doubles.
     *
     * I recommend to use this for first prototyping or applications with a low
     * communication memory footprint. It is pretty fast. However, I also
     * observe that this heap implementation plains to deadlock quickly for big
     * data sets where lots of messages are exchanged. In that case, I
     * recommend to use the RLEHeap below.
     *
     * You might want to have a closer look into the DoubleHeap in DoubleHeap.h or
     * FixedSizeDoubleHeap which are specialised versions of the heap for doubles
     * and come along with a smaller overhead.
     */
    typedef PlainHeap<peano::heap::records::DoubleHeapData>   PlainDoubleHeapWithDaStGenRecords;
    typedef RLEHeap<peano::heap::records::DoubleHeapData>     RLEDoubleHeapWithDaStGenRecords;


    /**
     * Plean heap over floats.
     *
     * I recommend to use this for first prototyping or applications with a low
     * communication memory footprint. It is pretty fast. However, I also
     * observe that this heap implementation plains to deadlock quickly for big
     * data sets where lots of messages are exchanged. In that case, I
     * recommend to use the RLEHeap below.
     */
    typedef PlainHeap<peano::heap::records::FloatHeapData>    PlainFloatHeap;
    typedef RLEHeap<peano::heap::records::FloatHeapData>      RLEFloatHeap;

    /**
     * Plean heap over chars/bytes.
     *
     * I recommend to use this for first prototyping or applications with a low
     * communication memory footprint. It is pretty fast. However, I also
     * observe that this heap implementation plains to deadlock quickly for big
     * data sets where lots of messages are exchanged. In that case, I
     * recommend to use the RLEHeap below.
     */
    typedef PlainHeap<peano::heap::records::CharHeapData>    PlainCharHeap;
    typedef RLEHeap<peano::heap::records::CharHeapData>      RLECharHeap;


    /**
     * Plean heap over booleans.
     *
     * I recommend to use this for first prototyping or applications with a low
     * communication memory footprint. It is pretty fast. However, I also
     * observe that this heap implementation plains to deadlock quickly for big
     * data sets where lots of messages are exchanged. In that case, I
     * recommend to use the RLEHeap below.
     *
     * Please note that this heap works with the real bool data type of C++. In
     * many cases, it might be better to work with integers instead and use the
     * integers at bitfield. It saves memory.
     */
    typedef PlainHeap<peano::heap::records::BooleanHeapData>    PlainBooleanHeap;
    typedef RLEHeap<peano::heap::records::BooleanHeapData>      RLEBooleanHeap;

    /**
     * Flags to specify which kind of message is sent or
     * received.
     */
    enum MessageType {
      NeighbourCommunication,
      ForkOrJoinCommunication,
      MasterWorkerCommunication
    };
  }
}




/**
 * Heap Singleton - Single Point of Contact for Heap Management
 *
 * The data held on the heap has to be generated
 * by DaStGen to provide MPI methods for sending and receiving. The idea of
 * heaps in Peano is that you model your vertex to have one integer index
 * per associated heap object with arbitrary cardinality. Whenever you
 * create a vertex, you ask the heap for a new index and store this index
 * within the vertex. Afterwards, you befill the heap with data.
 *
 * A similar reasoning holds for cells as well.
 *
 * Since a heap is kind of a global thing, this class is a singleton. Please
 * consult init() when you use the heap.
 *
 * <h1>MPI Handling</h1>
 *
 * The elements stored on the heap have to be modeled due to DaStGen. As a
 * result, we can send individual elements as well as sequences of classes
 * away as MPI data types. This is done whenever the user calls send on a
 * specific heap element, i.e. the user calls send with a heap element index
 * and the heap then sends away all elements associated to this heap. This way,
 * the user has full control which heap elements to distribute to which nodes.
 * However, no heap data is exchanged automatically - you have to do this within
 * the mapping's prepare send and merge operations.
 *
 * If an adapter sends or received data, it has first to inform the heap that
 * is will communicate due to MPI. For this, you have to call
 * startToSend... - typically in beginIteration(). At the end
 * of the iteration, you have to call the corresponing finishedToSend...().
 * Of both operations there are two variants: One for the exchange of boundary
 * data and one for the exchange between masters and workers. Internally, these
 * two communication modes differ significantly and thus have to be handled
 * separately. From the user point of view, it is important to note that
 * boundary data is not available at the neighbour node before the next
 * iteration - it might indeed make sense to wait several iterations before one
 * works with exchanged data again. Master-worker data is availble in exactly
 * the same traversal. For more information see the general heap information
 * in heap.doxys.
 *
 * A send operation triggered due to sendData() for a given heap element is
 * a process with three steps. First, the data is copied into a temporary array
 * and meta information is assembled. Then, the meta information is sent away
 * due to a blocking send. Meta data is modeled by
 * \code
\include dastgen/MetaInformation.def
\endcode (see struct MetaInformation). Then, a non-blocking send for the actual
 * data is launched and the send object is pushed on a stack _sendTasks. When
 * one grid traversal terminates, all send processes are finalised. After an
 * iteration, no send operations are dangling anymore, and all data has left
 * the current MPI node.
 *
 * The receive operation is more complex: Peano runs through the spacetree grid
 * forth and back, i.e. the traversal direction of the automaton is inverted
 * after each iteration. As a consequence, we may not simply trigger receive
 * calls when we need data from the heap, as receives in a subsequent traversal
 * have a mirrored order. Instead, the heap has to receive data from a given
 * node in the background until all data has been received. The received data
 * has to be stored in a temporary buffer that we call receive buffer. When all
 * data is available, the read order is the inverse of the receive order, i.e.
 * data from the receive buffer is deployed to the receiving tree automaton in
 * reserve order.
 *
 * To be able to identify whether all data is received, we assume that the
 * number of outgoing messages equals the number of incoming messages. That
 * does however not mean that the cardinalities of the individual messages
 * are the same. We also do not enforce that the data sent in one step is
 * received in the next step. Several additional adapter runs might be
 * triggered before data actually is used due to receiveData() calls. However,
 * we do not support that multiple send traversals may follow each other
 * without receive traversals in-between. The earliest time when you may send
 * heap data again after you've sent out heap data before is the grid traversal
 * where you also receive data.
 *
 * <h1> Memory footprint and alignment </h1>
 *
 * To reduce the memory footprint of the heaps, you may want to apply two
 * different optimisations:
 *
 * - The define PackRecordsInHeaps (by default, it is defined) allows you to
 *   remove padding on the heap. This is different to packing in the context
 *   of DaStGen:
 * - Use the packed variants of the heap records to save lots of data. In this
 *   case however, you have to ensure that you convert all data types properly
 *   in your code.
 *
 * In Peano, we typically use the heaps to work with aligned data.
 * Unfortunately, it is very hard to make the C++ std::vector use aligned data.
 * I therefore decided not to use the standard C++ class, but to define my own
 * Vector.h. If you use the flag
 *
 * -DAlignDataOnHeaps=16
 *
 * to a value greater than 0, you thus automatically should have proper
 * alignment.
 *
 *
 * <h1> Efficiency notes </h1>
 *
 * If a lot of heap data is exchanged, the asynchronous exchange of information
 * can block all MPI buffers. In this case, it is useful to call
 * receiveDanglingMessages() from time to time manually within your mappings.
 *
 * <h1> Heap data exchange throughout forks, joins, and master-worker exchange </h1>
 *
 * The arguing above (with the inversed read order in most cases) does not hold
 * for the forks and joins, and it also does not hold if a master sends heap
 * data to a worker or the other way round. Here, all data is sent-received
 * in-order and synchronous. For these special cases, all communication is
 * deployed to a separate tag to avoid confusion. You switch to this mode due
 * to the synchronous flag of the send and receive operation.
 *
 * <h1> Multithreading </h1>
 *
 * The heap class is not thread-safe and does not provide any threading
 * facilities. If you use it within a multithreaded application, you have to
 * ensure all the data consistency - either via semaphores or an a priori
 * exclusion of races.
 *
 * <h1> Troubleshooting </h1>
 *
 * - Is your receiving mapping also calling startToSendOrReceiveHeapData()?
 * - Does each node send exactly the same number of vertices to the other nodes
 *   that it wants to receive? The size of the heap records doesn't have to be
 *   the same but each vertex that wants to receive data also has to send a
 *   (probably empty) record the the corresponding neighbour.
 *
 *
 * @author Kristof Unterweger, Tobias Weinzierl
 */
template <class Data, class MasterWorkerExchanger, class JoinForkExchanger, class NeighbourDataExchanger>
class peano::heap::Heap: public tarch::services::Service, peano::heap::AbstractHeap {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    typedef std::map<int, std::vector<Data>*>  HeapContainer;

    /**
     * Map that holds all data that is stored on the heap
     * via this class.
     */
    HeapContainer _heapData;

    std::list<int>   _freedHeapIndices;

    /**
     * Stores the next available index. By now the indices
     * are generated in a linear order, so no attention is
     * payed by now to fragmentation of the index space by
     * deleting data.
     */
    int _nextIndex;

    #ifdef Parallel
    /**
     * Class has to remember the neighbour tags, as we need them whenever we
     * create a new exchanger. For the synchronous data exchangers, we may
     * create tags on-the-fly.
     */
    int                                    _neighbourDataExchangerMetaDataTag;
    int                                    _neighbourDataExchangerDataTag;

    MasterWorkerExchanger                  _masterWorkerExchanger;
    JoinForkExchanger                      _joinForkExchanger;
    std::map<int, NeighbourDataExchanger>  _neighbourDataExchanger;
    #endif

    /**
     * Stores the maximum number of heap objects that was stored
     * in this object at any time.
     */
    int _maximumNumberOfHeapEntries;

    /**
     * Stores the number of heap objects that have been allocated within
     * this object during the program's runtime.
     */
    int _numberOfHeapAllocations;

    int _numberOfHeapFrees;

    /**
     * Name for this heap object. Used for plotting statistics.
     */
    std::string _name;

    /**
     * Private constructor to hide the possibility
     * to instantiate an object of this class.
     */
    Heap();

    /**
     * Private destructor to free the MPI datatypes.
     */
    ~Heap();

  public:
    typedef std::vector<Data>  HeapEntries;

    /**
     * Start to send data
     *
     * This operation is typically called in beginIteration(). However, please
     * be aware that the data from the master is received and merged before.
     *
     * This operation is to be re-called in each traversal and should be
     * followed by a finish call in the each traversal as well.
     *
     * You are not allowed to send heap data from the master to the worker or
     * back before this operation has been called. This holds also for joins
     * and merges - if they exchange heap data, this operation has to be called
     * before. Synchronous means that you receive all sent data in the very
     * same iteration.
     */
    virtual void startToSendSynchronousData();

    /**
     * Counterpart of startToSendSynchronousData().
     *
     * If you exchange data along the boundaries, you have to call this variant
     * of start to send together with its finish operation later on. It opens
     * a kind of communication channel for boundary data exchange. Boundary
     * data is not received on the addressee side before the next iteration.
     * You even might decide to wait a couple of iterations before you receive
     * the boundary records.
     *
     * Please hand in the state's traversal bool that informs the heap about
     * the direction of the Peano space-filling curve.
     */
    virtual void startToSendBoundaryData(bool isTraversalInverted);

    /**
     * Stop to send data
     *
     * Counterpart of startToSendSynchronousData(). Should be called around in each
     * traversal where you've also called startToSendSynchronousData(). When this
     * operation is called, you are not allowed to send heap data anymore.
     *
     * This operation runs through all sent master-worker and join-fork
     * messages and waits for each sent
     * message until the corresponding non-blocking MPI request is freed, i.e.
     * until the message has left the system. As the underlying MPI_Test
     * modifies the MPI handles, the operation is not const. The method also
     * has some deadlock detection.
     *
     * !!! Frequently Done Bug
     *
     * Please note that the event prepareSendToMaster() is invoked by Peano
     * after endIteration() is called. Many heap users send data to the master
     * in prepareSendToMaster(). For such codes, you may not call
     * finishedToSendData() in endIteration(). Instead, you have to notify the
     * heap that you finished to send in prepareSendToMaster().
     *
     * This induces an error on the global master where prepareSendToMaster()
     * is never called if you have invoked startToSendSynchronousData() in
     * beginIteration(). In this case,
     *
     * - either call startToSendSynchronousData() when you receive data from
     *   the master or just when you start to send data back, i.e.
     *   prepareSendToMaster(),
     * - or call finish in endIteration() as well - but if and only if you are
     *   on the global master.
     *
     * Please note however that all of these operations
     *
     * !!! Congestion
     *
     * Heaps in Peano often are used to manage large pieces of data. As such,
     * we observe that congestion happens quite frequently - two pieces of code
     * send out large data and block each other. The heap is particularly
     * dangerous here. If you send out big data and call receive afterwards, be
     * sure that you place the finishedToSendSynchronousData() after the receive
     * command, as finishedToSendSynchronousData() waits until data has been
     * delivered. Now, this can't happen for big messages if the corresponding
     * receive hasn't been triggered.
     */
    virtual void finishedToSendSynchronousData();

    /**
     * Finish boundary data exchange
     *
     * Counterpart of start operation. Should be called in endIteration().
     *
     * !!! Parallelisation with MPI
     *
     * If you parallelise with MPI, this operation has to be called by
     * endIteration() at the end of the traversal of the whole grid and not
     * only the local data structure. As a consequence, you may not combine
     * this operation with the communication specification
     * SendDataAndStateAfterProcessingOfLocalSubtree or
     * MaskOutWorkerMasterDataAndStateExchange as these two guys call
     * endIteration() too early.
     */
    virtual void finishedToSendBoundaryData(bool isTraversalInverted);

    /**
     * The HeapData class is a singleton, thus one needs to
     * use the getInstance() method to retrieve the single
     * instance of this class.
     */
    static Heap& getInstance();

    /**
     * Retrieves the data that corresponds to the given index.
     */
    std::vector<Data>& getData(int index);

    /**
     * Retrieves the data that corresponds to the given index.
     */
    const std::vector<Data>& getData(int index) const;

    /**
     * Create new heap entry
     *
     * Creates new data on the heap and returns the corresponding index. The
     * counterpart of this operation is deleteData().
     *
     * @param numberOfEntries If you know a priori how many elements you'll
     *        gonna store for this heap entry, use this constructor. You can
     *        always add more elements later, but using this parameter should
     *        be faster and leads to a lower memory fragmentation
     * @param initialCapacity Has to be bigger/equal than numberOfEntries or
     *        zero. It tells the heap how much entries you expect to be stored
     *        for this index. You can always use more elements later on, so
     *        this is just a tuning parameter to avoid frequent reallocation.
     *
     * @return The index return is always a non-negativ number.
     */
    int createData(int numberOfEntries=0, int initialCapacity=0);

    /**
     * Creates a heap entry for the index wantedIndex. This is operation
     * typically is used by codes that manage multiple heaps with one
     * heap index. They create one one index through createData() and then
     * invoke createDataForIndex() with this index on all other heaps.
     * Typically, such code sequences are found in creational operations.
     *
     * !!! Frequent bug
     *
     * Whenever you have a createDataForIndex for a heap, please check whether
     * there's a corresponding deleteData() call. If you work with grids that
     * refine only (and/or are static) you can omit the delete as long as you
     * work without MPI. If you use MPI, you always have to implement the
     * deletion as any partitioning involves a grid destruction on the node
     * that deploys parts of its grid to another rank.
     */
    void createDataForIndex(int wantedIndex, int numberOfEntries=0, int initialCapacity=0);

    /**
     * Returns, if the given index is a known index and, thus,
     * refers to a valid heap data object.
     */
    bool isValidIndex(int index) const;

    /**
     * Deletes the data with the given index and remove index from heap.
     * First, all data associated to this index is cleared. Afterwards, we
     * clear the heap entry. The user however is responsible not to use
     * index anymore. This is important, as the heap might decide to reuse
     * index already for the next createData() call.
     */
    void deleteData(int index);

    /**
     * Deletes all data that has been allocated by the application
     * by means of the createData() method.
     */
    void deleteAllData();

    /**
     * Returns the number of entries being held by this object.
     */
    int getNumberOfAllocatedEntries() const;

    /**
     * Takes data from fromIndex and reassings it to toIndex.
     *
     * If toIndex already points to data, the data from fromIndex is appended
     * to the original data, i.e. no data is erased at toIndex. The operation
     * does not remove fromIndex though fromIndex will afterwards identify an
     * empty data container.
     */
    void moveData( int toIndex, int fromIndex );

    void addData( int index, const HeapEntries& entries );
    void addData( int index, const Data&        entry );

    /**
     * This method discards all heap data and prepares the HeapData
     * management to handle new data requests. After calling this
     * method all indices retrieved earlier are invalid.
     */
    void restart();

    /**
     * Shuts down the memory management for heap data and frees all allocated
     * memory.
     */
    void shutdown();

    /**
     * Assign the heap a name (identifier) such that messages from the heap can
     * be assigned to the heap type. In particular important if you handle
     * multiple heaps, so you can distinguish error messages, e.g.
     */
    void setName(std::string name);

    /**
     * Sends heap data associated to one index to one rank.
     *
     * Please note that these sends are assynchronous, i.e. if you change the
     * vertices afterwards, you might run into problems
     */
    void sendData(
      int                                           index,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    void sendData(
      const std::vector<Data>&                      data,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    /**
     * Receive heap data associated to one index from one rank.
     *
     * Wrapper forwarding to the other receiveData() operation with default
     * values. Operation should be used in release mode, as all additional
     * attributes of the overloaded receiveData() operation are used for
     * validation purposes.
     *
     * !!! Rationale
     *
     * Though the operation only deploys data that has been received before, it
     * is not const as it frees data of the local buffers.
     *
     * This operation is not const, as local handles might be deleted
     * afterwards.
     *
     * @see Heap
     * @see receiveData(int)
     */
    std::vector< Data > receiveData(
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    /**
     * Receive data and append it to local data.
     *
     * @see receiveData()
     * @return Number of appended entries
     */
    int receiveData(
      int                                           index,
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    /**
     * @see Heap
     */
    virtual void receiveDanglingMessages();

    /**
     * @return Brief description of heap incl its identifier (if set)
     */
    std::string toString() const;

    /**
     * Plots statistics for this heap data.
     */
    void plotStatistics() const;

    void clearStatistics();

    void logContentToWarningDevice();
};


#include "peano/heap/Heap.cpph"

#endif
