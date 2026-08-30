// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_CHAR_HEAP_H_
#define _PEANO_HEAP_CHAR_HEAP_H_


#if defined(SharedTBB)
#include <tbb/cache_aligned_allocator.h>
#include <tbb/concurrent_hash_map.h>
#else
#include <map>
#endif

#include <set>


#include "peano/heap/Heap.h"
#include "peano/heap/AlignedCharSendReceiveTask.h"


#include "tarch/multicore/BooleanSemaphore.h"


namespace peano {
  namespace heap {
    template<
      class MasterWorkerExchanger,
      class JoinForkExchanger,
      class NeighbourDataExchanger,
      class VectorContainer = std::vector<char>
    >
    class CharHeap;


    /**
     * TBB
     *
     * It makes sense to replace the std::vector with
     *
     * std::vector<char, tbb::cache_aligned_allocator<char> >
     *
     * to have a proper cache alignment if you don't manually align your stuff
     * anyway:
     *
    template<
      class MasterWorkerExchanger,
      class JoinForkExchanger,
      class NeighbourDataExchanger,
      class VectorContainer = std::vector<char, tbb::cache_aligned_allocator<char> >
    >
    class CharHeap;
     */
    typedef CharHeap<
      SynchronousDataExchanger< char, true, SendReceiveTask<char> >,
      SynchronousDataExchanger< char, true, SendReceiveTask<char> >,
      PlainBoundaryDataExchanger< char, true, SendReceiveTask<char> >
    >     PlainCharHeap;

    typedef CharHeap<
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<16>, std::vector< char, HeapAllocator<char, 16> > >,
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<16>, std::vector< char, HeapAllocator<char, 16> > >,
      PlainBoundaryDataExchanger< char, true, AlignedCharSendReceiveTask<16>, std::vector< char, HeapAllocator<char, 16> > >,
      std::vector< char, HeapAllocator<char, 16> >
    >     PlainCharHeapAlignment16;

    typedef CharHeap<
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<32>, std::vector< char, HeapAllocator<char, 32> > >,
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<32>, std::vector< char, HeapAllocator<char, 32> > >,
      PlainBoundaryDataExchanger< char, true, AlignedCharSendReceiveTask<32>, std::vector< char, HeapAllocator<char, 32> > >,
      std::vector< char, HeapAllocator<char, 32> >
    >     PlainCharHeapAlignment32;

    typedef CharHeap<
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<64>, std::vector< char, HeapAllocator<char, 64> > >,
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<64>, std::vector< char, HeapAllocator<char, 64> > >,
      PlainBoundaryDataExchanger< char, true, AlignedCharSendReceiveTask<64>, std::vector< char, HeapAllocator<char, 64> > >,
      std::vector< char, HeapAllocator<char, 64> >
    >     PlainCharHeapAlignment64;


    typedef CharHeap<
      SynchronousDataExchanger< char, true, SendReceiveTask<char> >,
      SynchronousDataExchanger< char, true, SendReceiveTask<char> >,
      RLEBoundaryDataExchanger< char, true, SendReceiveTask<char> >
    >     RLECharHeap;

    typedef CharHeap<
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<16>, std::vector< char, HeapAllocator<char, 16> > >,
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<16>, std::vector< char, HeapAllocator<char, 16> > >,
      RLEBoundaryDataExchanger< char, true, AlignedCharSendReceiveTask<16>, std::vector< char, HeapAllocator<char, 16> > >,
      std::vector< char, HeapAllocator<char, 16> >
    >     RLECharHeapAlignment16;

    typedef CharHeap<
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<32>, std::vector< char, HeapAllocator<char, 32> > >,
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<32>, std::vector< char, HeapAllocator<char, 32> > >,
      RLEBoundaryDataExchanger< char, true, AlignedCharSendReceiveTask<32>, std::vector< char, HeapAllocator<char, 32> > >,
      std::vector< char, HeapAllocator<char, 32> >
    >     RLECharHeapAlignment32;

    typedef CharHeap<
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<64>, std::vector< char, HeapAllocator<char, 64> > >,
      SynchronousDataExchanger< char, true, AlignedCharSendReceiveTask<64>, std::vector< char, HeapAllocator<char, 64> > >,
      RLEBoundaryDataExchanger< char, true, AlignedCharSendReceiveTask<64>, std::vector< char, HeapAllocator<char, 64> > >,
      std::vector< char, HeapAllocator<char, 64> >
    >     RLECharHeapAlignment64;
  }
}



/**
 * <h1> CharHeap </h1>
 *
 * This is a specialised variant of the heap for chars. It works directly
 * with chars held in a std::vector. It does not rely on DaStGen for the
 * data at all and not wrap any data into DaStGen records. It thus should be
 * faster than the standard version.
 *
 * <h2> Working with plain char pointer </h2>
 *
 * With this class, you may use getData().data() yielding a plain char
 * pointer. It is probably aligned if you choose alignment.
 *
 *
 * <h2> Alignment </h2>
 *
 * A big difference to the standard heap class is that this class can work with
 * aligned data structures. This makes the class however incompatible with
 * other std::vector<char> instances where no alignment is used. Please consult
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
class peano::heap::CharHeap: public tarch::services::Service, peano::heap::AbstractHeap {
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

    CharHeap();

    ~CharHeap();

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

    static CharHeap& getInstance();

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

    void addData( int index, const char&        entry );

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
     * Send plain char array throughout heap
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
     * routine wraps the char pointer into a std::vector and hands it over
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
      const char*                                   data,
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
      char*                                         data,
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



#include "peano/heap/CharHeap.cpph"


#endif
