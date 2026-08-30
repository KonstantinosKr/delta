// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_DOUBLE_HEAP_H_
#define _PEANO_HEAP_DOUBLE_HEAP_H_


#if defined(SharedTBB)
#include <tbb/cache_aligned_allocator.h>
#include <tbb/concurrent_hash_map.h>
#else
#include <map>
#endif

#include <set>


#include "peano/heap/Heap.h"
#include "peano/heap/AlignedDoubleSendReceiveTask.h"


#include "tarch/multicore/BooleanSemaphore.h"


namespace peano {
  namespace heap {
    template<
      class MasterWorkerExchanger,
      class JoinForkExchanger,
      class NeighbourDataExchanger,
      class VectorContainer = std::vector<double>
    >
    class DoubleHeap;


    /**
     * TBB
     *
     * It makes sense to replace the std::vector with
     *
     * std::vector<double, tbb::cache_aligned_allocator<double> >
     *
     * to have a proper cache alignment if you don't manually align your stuff
     * anyway:
     *
    template<
      class MasterWorkerExchanger,
      class JoinForkExchanger,
      class NeighbourDataExchanger,
      class VectorContainer = std::vector<double, tbb::cache_aligned_allocator<double> >
    >
    class DoubleHeap;
     */
    typedef DoubleHeap<
      SynchronousDataExchanger< double, true, SendReceiveTask<double> >,
      SynchronousDataExchanger< double, true, SendReceiveTask<double> >,
      PlainBoundaryDataExchanger< double, true, SendReceiveTask<double> >
    >     PlainDoubleHeap;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<16>, std::vector< double, HeapAllocator<double, 16> > >,
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<16>, std::vector< double, HeapAllocator<double, 16> > >,
      PlainBoundaryDataExchanger< double, true, AlignedDoubleSendReceiveTask<16>, std::vector< double, HeapAllocator<double, 16> > >,
      std::vector< double, HeapAllocator<double, 16> >
    >     PlainDoubleHeapAlignment16;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<32>, std::vector< double, HeapAllocator<double, 32> > >,
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<32>, std::vector< double, HeapAllocator<double, 32> > >,
      PlainBoundaryDataExchanger< double, true, AlignedDoubleSendReceiveTask<32>, std::vector< double, HeapAllocator<double, 32> > >,
      std::vector< double, HeapAllocator<double, 32> >
    >     PlainDoubleHeapAlignment32;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<64>, std::vector< double, HeapAllocator<double, 64> > >,
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<64>, std::vector< double, HeapAllocator<double, 64> > >,
      PlainBoundaryDataExchanger< double, true, AlignedDoubleSendReceiveTask<64>, std::vector< double, HeapAllocator<double, 64> > >,
      std::vector< double, HeapAllocator<double, 64> >
    >     PlainDoubleHeapAlignment64;


    typedef DoubleHeap<
      SynchronousDataExchanger< double, true, SendReceiveTask<double> >,
      SynchronousDataExchanger< double, true, SendReceiveTask<double> >,
      RLEBoundaryDataExchanger< double, true, SendReceiveTask<double> >
    >     RLEDoubleHeap;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<16>, std::vector< double, HeapAllocator<double, 16> > >,
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<16>, std::vector< double, HeapAllocator<double, 16> > >,
      RLEBoundaryDataExchanger< double, true, AlignedDoubleSendReceiveTask<16>, std::vector< double, HeapAllocator<double, 16> > >,
      std::vector< double, HeapAllocator<double, 16> >
    >     RLEDoubleHeapAlignment16;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<32>, std::vector< double, HeapAllocator<double, 32> > >,
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<32>, std::vector< double, HeapAllocator<double, 32> > >,
      RLEBoundaryDataExchanger< double, true, AlignedDoubleSendReceiveTask<32>, std::vector< double, HeapAllocator<double, 32> > >,
      std::vector< double, HeapAllocator<double, 32> >
    >     RLEDoubleHeapAlignment32;

    typedef DoubleHeap<
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<64>, std::vector< double, HeapAllocator<double, 64> > >,
      SynchronousDataExchanger< double, true, AlignedDoubleSendReceiveTask<64>, std::vector< double, HeapAllocator<double, 64> > >,
      RLEBoundaryDataExchanger< double, true, AlignedDoubleSendReceiveTask<64>, std::vector< double, HeapAllocator<double, 64> > >,
      std::vector< double, HeapAllocator<double, 64> >
    >     RLEDoubleHeapAlignment64;
  }
}



/**
 * <h1> DoubleHeap </h1>
 *
 * This is a specialised variant of the heap for doubles. It works directly
 * with doubles held in a std::vector. It does not rely on DaStGen for the
 * data at all and not wrap any data into DaStGen records. It thus should be
 * faster than the standard version.
 *
 * <h2> Working with plain double pointer </h2>
 *
 * With this class, you may use getData().data() yielding a plain double
 * pointer. It is probably aligned if you choose alignment.
 *
 *
 * <h2> Alignment </h2>
 *
 * A big difference to the standard heap class is that this class can work with
 * aligned data structures. This makes the class however incompatible with
 * other std::vector<double> instances where no alignment is used. Please consult
 * the HeapAllocator for details on the alignment.
 *
 * <h2> Method documentation </h2>
 *
 * This is a specialisation of the general-purpose heap. As such, the
 * documentation for the routines here is empty. Please study peano::heap::Heap
 * to find out about the intended semantics.
 *
 *
 * @author Tobias Weinzierl
 */
template <class MasterWorkerExchanger, class JoinForkExchanger, class NeighbourDataExchanger, class VectorContainer>
class peano::heap::DoubleHeap: public tarch::services::Service, peano::heap::AbstractHeap {
  private:
    static tarch::logging::Log _log;

    static tarch::multicore::BooleanSemaphore _recycleAndDeleteSemaphore;

    #if defined(SharedTBB)
    typedef tbb::concurrent_hash_map<int, VectorContainer*>  HeapContainer;
    #else
    typedef std::map<int, VectorContainer*>  HeapContainer;
    #endif

    typedef std::set<int>                    RecycledAndDeletedEntriesContainer;

    HeapContainer    _heapData;

    RecycledAndDeletedEntriesContainer   _deletedHeapIndices;

    RecycledAndDeletedEntriesContainer   _recycledHeapIndices;

    int _nextIndex;

    #ifdef Parallel
    int                                     _neighbourDataExchangerMetaDataTag;
    int                                     _neighbourDataExchangerDataTag;

    MasterWorkerExchanger                   _masterWorkerExchanger;
    JoinForkExchanger                       _joinForkExchanger;
    std::map<int, NeighbourDataExchanger*>  _neighbourDataExchanger;

    tarch::multicore::BooleanSemaphore      _neighbourDataMapSemaphore;
    #endif

    int _maximumNumberOfHeapEntries;

    int _numberOfHeapAllocations;

    int _numberOfHeapFrees;

    std::string _name;

    DoubleHeap();

    ~DoubleHeap();

  public:
    enum class Allocation {
      DoNotUseAnyRecycledEntry,
      UseOnlyRecycledEntries,
      UseRecycledEntriesIfPossibleCreateNewEntriesIfRequired
    };

    typedef VectorContainer  HeapEntries;

    virtual void startToSendSynchronousData();

    virtual void startToSendBoundaryData(bool isTraversalInverted);

    virtual void finishedToSendSynchronousData();

    virtual void finishedToSendBoundaryData(bool isTraversalInverted);

    static DoubleHeap& getInstance();

    HeapEntries& getData(int index);

    const HeapEntries& getData(int index) const;

    int createData(int numberOfEntries=0, int initialCapacity=0, Allocation allocation = Allocation::UseRecycledEntriesIfPossibleCreateNewEntriesIfRequired);

    void createDataForIndex(int wantedIndex, int numberOfEntries=0, int initialCapacity=0);

    void reserveHeapEntriesForRecycling(int numberOfEntries);

    bool areRecycleEntriesAvailable() const;

    bool isValidIndex(int index) const;

    void deleteData(int index, bool recycle = false);

    void deleteAllData();

    int getNumberOfAllocatedEntries() const;

    void moveData( int toIndex, int fromIndex );

    void addData( int index, const HeapEntries& entries );

    void addData( int index, const double&        entry );

    void restart();

    void shutdown();

    void setName(std::string name);

    void createBuffersManually( int communicationRank );

    void sendData(
      int                                           index,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    void sendData(
      const HeapEntries&                            data,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    /**
     * Send plain double array throughout heap
     *
     * Please ensure that you do not erase data if you use a heap without data
     * copies until the next grid sweep has successfully started. The heap
     * gives MPI time to do all data transers until the next
     * startToSendBoundaryData() is invoked. So until then, the pointer has to
     * remain valid.
     *
     * <h2> Realisation </h2>
     *
     * If we work with data copies, the handling is straightforward. The
     * routine wraps the double pointer into a std::vector and hands it over
     * to the other routines.
     *
     * If we work without data copies, we may not
     * follow this approach as the std::vector copies all data and then would
     * be released too early.
     *
     *
     *
     */
    void sendData(
      const double*                                 data,
      int                                           size,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    HeapEntries receiveData(
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    void receiveData(
      double*                                       data,
      int                                           size,
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    int receiveData(
      int                                           index,
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS, double>&  position,
      int                                           level,
      MessageType                                   messageType
    );

    virtual void receiveDanglingMessages();

    std::string toString() const;

    void plotStatistics() const;

    void clearStatistics();

    void logContentToWarningDevice();

    bool validateThatIncomingJoinBuffersAreEmpty();
};



#include "peano/heap/DoubleHeap.cpph"


#endif
