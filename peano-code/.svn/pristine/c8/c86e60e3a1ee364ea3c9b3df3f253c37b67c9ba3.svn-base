// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_DOUBLE_HEAP_H_
#define _PEANO_HEAP_DOUBLE_HEAP_H_


#include "peano/heap/Heap.h"


namespace peano {
  namespace heap {
    template<
      class MasterWorkerExchanger,
      class JoinForkExchanger,
      class NeighbourdoubleExchanger,
      // @tood Perhaps remove default and always align?
      class VectorContainer = std::vector<double>
    >
    class DoubleHeap;


    typedef DoubleHeap<
      SynchronousDataExchanger< double, true >,
      SynchronousDataExchanger< double, true >,
      PlainBoundaryDataExchanger< double, true >
    >     PlainDoubleHeap;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true >,
      SynchronousDataExchanger< double, true >,
      PlainBoundaryDataExchanger< double, true >,
      std::vector< double, HeapAllocator<double, 32 > >
    >     PlainDoubleHeapAlignment32;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true >,
      SynchronousDataExchanger< double, true >,
      PlainBoundaryDataExchanger< double, true >,
      std::vector< double, HeapAllocator<double, 64 > >
    >     PlainDoubleHeapAlignment64;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true >,
      SynchronousDataExchanger< double, true >,
      RLEBoundaryDataExchanger< double, true >
    >     RLEDoubleHeap;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true >,
      SynchronousDataExchanger< double, true >,
      RLEBoundaryDataExchanger< double, true >,
      std::vector< double, HeapAllocator<double, 32 > >
    >     RLEDoubleHeapAlignment32;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true >,
      SynchronousDataExchanger< double, true >,
      RLEBoundaryDataExchanger< double, true >,
      std::vector< double, HeapAllocator<double, 64 > >
    >     RLEDoubleHeapAlignment64;

  }
}



/**
 * DoubleHeap
 *
 * This is a specialised variant of the heap for doubles. It works directly
 * with doubles held in a std::vector. It does not rely on DaStGen for the
 * data at all and not wrap any data into DaStGen records. It thus should be
 * faster than the standard version.
 *
 * <h1> Working with plain double pointer </h1>
 *
 * With this class, you may use getData().data() yielding a plain double
 * pointer. It is probably aligned if you choose alignment.
 *
 *
 * <h1> Alignment </h1>
 *
 * A big difference to the standard heap class is that this class can work with
 * aligned data structuures. This makes the class however incompatible with
 * other std::vector<double> instances where no alignment is used. Please consult
 * the HeapAllocator for defails on the alignment.
 *
 * @author Tobias Weinzierl
 */
template <class MasterWorkerExchanger, class JoinForkExchanger, class NeighbourdoubleExchanger, class VectorContainer>
class peano::heap::DoubleHeap: public tarch::services::Service, peano::heap::AbstractHeap {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    typedef std::map<int, VectorContainer*>  HeapContainer;

    /**
     * Map that holds all data that is stored on the heap
     * via this class.
     */
    HeapContainer    _heapData;

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
    int                                    _neighbourdoubleExchangerMetadoubleTag;
    int                                    _neighbourdoubleExchangerdoubleTag;

    MasterWorkerExchanger                  _masterWorkerExchanger;
    JoinForkExchanger                      _joinForkExchanger;
    std::map<int, NeighbourdoubleExchanger>  _neighbourdoubleExchanger;
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
    DoubleHeap();

    /**
     * Private destructor to free the MPI datatypes.
     */
    ~DoubleHeap();

  public:
    typedef VectorContainer  HeapEntries;

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
     * Counterpart of startToSendSynchronousdouble().
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
     * Counterpart of startToSendSynchronousdouble(). Should be called around in each
     * traversal where you've also called startToSendSynchronousdouble(). When this
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
     * finishedToSenddouble() in endIteration(). Instead, you have to notify the
     * heap that you finished to send in prepareSendToMaster().
     *
     * This induces an error on the global master where prepareSendToMaster()
     * is never called if you have invoked startToSendSynchronousdouble() in
     * beginIteration(). In this case,
     *
     * - either call startToSendSynchronousdouble() when you receive data from
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
     * sure that you place the finishedToSendSynchronousdouble() after the receive
     * command, as finishedToSendSynchronousdouble() waits until data has been
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
     * SenddoubleAndStateAfterProcessingOfLocalSubtree or
     * MaskOutWorkerMasterdoubleAndStateExchange as these two guys call
     * endIteration() too early.
     */
    virtual void finishedToSendBoundaryData(bool isTraversalInverted);

    /**
     * The Heapdouble class is a singleton, thus one needs to
     * use the getInstance() method to retrieve the single
     * instance of this class.
     */
    static DoubleHeap& getInstance();

    /**
     * Retrieves the data that corresponds to the given index.
     */
    VectorContainer& getData(int index);

    /**
     * Retrieves the data that corresponds to the given index.
     */
    const VectorContainer& getData(int index) const;

    /**
     * Create new heap entry
     *
     * Creates new data on the heap and returns the corresponding index. The
     * counterpart of this operation is deletedouble().
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
     * heap index. They create one one index through createdouble() and then
     * invoke createdoubleForIndex() with this index on all other heaps.
     * Typically, such code sequences are found in creational operations.
     *
     * !!! Frequent bug
     *
     * Whenever you have a createdoubleForIndex for a heap, please check whether
     * there's a corresponding deletedouble() call. If you work with grids that
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
     * index already for the next createdouble() call.
     */
    void deleteData(int index);

    /**
     * Deletes all data that has been allocated by the application
     * by means of the createdouble() method.
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
    void addData( int index, const double&        entry );

    /**
     * This method discards all heap data and prepares the Heapdouble
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
      const VectorContainer&                    data,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    /**
     * Receive heap data associated to one index from one rank.
     *
     * Wrapper forwarding to the other receivedouble() operation with default
     * values. Operation should be used in release mode, as all additional
     * attributes of the overloaded receivedouble() operation are used for
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
     * @see receivedouble(int)
     */
    VectorContainer receiveData(
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    /**
     * Receive data and append it to local data.
     *
     * @see receivedouble()
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



#include "peano/heap/DoubleHeap.cpph"


#endif
