// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_UTILS_PEANO_OPTIMISATIONS
#define _PEANO_UTILS_PEANO_OPTIMISATIONS


/**
 * This header provides multiple ways to tailor Peano algorithmically. However,
 * it does not provide ifdefs to tailor Peano to the chosen compiler. If you
 * wanna adopt your settings to your compiler, MPI installation, whatever,
 * please consult the compiler-specific settings.
 */
#include "tarch/compiler/CompilerSpecificSettings.h"


/**
 * Uses lookup tables within the dfor statements. To enable this features dfors
 * have to be used for 2,3,4 by 2,3,4 arrays only. Otherwise, the lookup table
 * mechanism will fail. By standard, lookup tables are only switched on for the
 * xPowI functions, where x is 2,3 or 4.
 */
#ifndef noDloopOptimiseAggressive
#define DloopOptimiseAggressive
#endif


/**
 * Store only the part of the objects that is marked as persistent. Reduces
 * size (memory consumption) of the grid data types.
 */
#ifndef noStoreOnlyPersistentAttributes
#define StoreOnlyPersistentAttributes
#endif


/**
 * Peano data packing enabled: less data per node/cell
 * Only in very rare cases, this flag might have negative influence on runtime.
 * You may even use it in Debug mode, as it is no compiler optimisation flag.
 */
#ifndef noPackedRecords
#define PackedRecords
#endif


/**
 * Send and receive data in packed version
 *
 * In the parallel code, the user either exchanges the raw vertices and cells or
 * the packed variants of either. Depending on the architecture (especially
 * bandwidth and latency), one option might result in a significant better
 * performance. The Altix, e.g., is faster if this flag is not set, but the
 * bandwidth requirements rise then.
 */
#if defined(PackedRecords)
  #ifndef noParallelExchangePackedRecordsAtBoundary
    #define ParallelExchangePackedRecordsAtBoundary
  #endif
  #ifndef noParallelExchangePackedRecordsBetweenMasterAndWorker
    #define ParallelExchangePackedRecordsBetweenMasterAndWorker
  #endif
  #ifndef noPackRecordsInHeaps
    #define PackRecordsInHeaps
  #endif
  #ifndef noParallelExchangePackedRecordsThroughoutJoinsAndForks
    #define ParallelExchangePackedRecordsThroughoutJoinsAndForks
  #endif
#endif


/**
 * Switch recursion unrolling on or off
 *
 * This flag might speed up your code but introduces a slightly bigger memory
 * footprint. With a shared memory parallelisation, it furthermore might be
 * that the runtime savings vanish, as the underlying computations that are
 * saved in a lookup table can be computed in parallel anyway.
 */
#ifndef noUseRecursionUnrollingOnRegularPatches
#define UseRecursionUnrollingOnRegularPatches
#endif


#ifndef noDistinguishStationaryAndInstationarySubtrees
#define DistinguishStationaryAndInstationarySubtrees
#endif


/**
 * Usually, I use lookup tables for all the sfc indices, vertex access codes
 * (which stack to use), and so forth. I call them action sets. For big spatial
 * dimensions, these lookup tables may become huge and it thus might make sense
 * not to precompute them but to compute all table entries on-the-fly whenever
 * entries are needed.
 *
 * Further, we sometimes run into cases where we have index overflows (they do
 * not fit into integers anymore) for big d. In this case, it also makes sense
 * to switch off the action set caching.
 *
 * @see ActionSetTraversal and CellLocalPeanoCurve
 */
#ifndef noCacheActionSets
  #define CacheActionSets
#endif


/**
 * Switch on manual alignment of vectors
 */
// @todo raus -> Wenn dann in Compiler Stuff
#define noUseManualAlignment

#ifdef noUseManualAlignment
  #if defined(VectorisationAlignment)
    #warning Specified VectorisationAlignment though manual alignment was switched off due to -DnoUseManualAlignment
  #endif
#elif !defined(VectorisationAlignment)
  #warning No alignment specified by compiler though UseManualAlignment is switched on
#endif


/**
 * The regular grid container has three different realisations to handle
 * regular grids. You can pick one of them. In my experiments, the plain
 * arrays outperformed all other realisations despite the fact that the
 * std variant uses tbb's memory allocators. Might nevertheless be worth
 * a try to study different variants as well.
 *
 * Please select only one out of these three variants.
 */
#define RegularGridContainerUsesPlainArrays
//#define RegularGridContainerUsesRawArrays
//#define RegularGridContainerUsesSTDArrays


#ifndef noPersistentRegularSubtrees
  #ifndef UseRecursionUnrollingOnRegularPatches
    #error PersistentRegularSubtrees is enabled though UseRecursionUnrollingOnRegularPatches is disabled
  #endif
  #define PersistentRegularSubtrees
#endif

/**
 * Some MPI implementations cannot do data transfers in the background of
 * computations but need the code to call MPI_Test over and over again to
 * realise the data transfer.
 *
 * Flag only plays a role if you are heavily relying on heaps. For the standard
 * boundary data exchange, the testing is always implicitly enabled.
 *
 * It seems however that for most applications switching on this feature is not
 * a good idea.
 */
#if !defined(MPIProgressionReliesOnMPITest) &&  !defined(noMPIProgressionReliesOnMPITest)
#define noMPIProgressionReliesOnMPITest
#endif

/**
 * We usually do all the heap data exchange via non-blocking calls, i.e. all
 * meta data (how many records are exchanged) is communicated immediately while
 * the actual data transfer might linger in the background. For debugging and
 * optimisation, i.e. to free MPI from too many dangling requests, you can
 * alter this behaviour and make each heap send/receive a blocking
 * send/receive.
 *
 * Heap data is usually sent out in pairs: There's a meta data message
 * containing (among other data) the information how many records are
 * exchanged. And then next we transfer the actual data. So whenever we
 * receive a meta data message, we know that there will be real data, too.
 * With this flag, one can select whether this very data is to be received
 * blocking or non-blocking. By default, I try to do this real data exchange
 * in the background as I assume that this data is massive.
 */
#if !defined(noNonblockingHeapDataReceives) and !defined(NonblockingHeapDataReceives)
  #define NonblockingHeapDataReceives
#endif

/**
 * The boundary data exchange (grid data) relies on probing the MPI queues: We
 * check from time to time whether there are messages pending. If there's one,
 * we issue a receive. This receive either can be blocking or non-blocking. The
 * latter is the default as it implies that computation and communication can
 * overlap. There might however be cases where the non-blocking management
 * overhead renders a blocking data exchange advantageous.
 *
 * For most of the existing Peano applications, this flag did not make any
 * difference.
 */
#if !defined(noNonblockingBoundaryDataReceives) and !defined(NonblockingBoundaryDataReceives)
  #define NonblockingBoundaryDataReceives
#endif

#if defined(MultipleThreadsMayTriggerMPICalls) && !defined(noMPIHeapUsesItsOwnThread) && !defined(MPIHeapUsesItsOwnThread)
#define noMPIHeapUsesItsOwnThread
#endif

/**
 * We found that this feature is extremely sensitive to IprobeEveryKIterations.
 * See remarks and documentation in SendReceiveBufferPool::BackgroundThread::operator().
 */
#if defined(MultipleThreadsMayTriggerMPICalls) && !defined(noMPIUsesItsOwnThread) && !defined(MPIUsesItsOwnThread)
#define noMPIUsesItsOwnThread
#endif

#if !defined(UseTBBsParallelForAndReduce) &&  !defined(noUseTBBsParallelForAndReduce)
#define UseTBBsParallelForAndReduce
#endif

#if !defined(BooleanSemaphoreUsesASpinLock) && !defined(noBooleanSemaphoreUsesASpinLock)
#define BooleanSemaphoreUsesASpinLock
#endif

#if !defined(JobQueueUsesSpinLockInsteadOfMutex) && !defined(noJobQueueUsesSpinLockInsteadOfMutex)
#define JobQueueUsesSpinLockInsteadOfMutex
#endif

#if !defined(JobQueueUsesStackOfBefilledQueues) && !defined(noJobQueueUsesStackOfBefilledQueues)
#define JobQueueUsesStackOfBefilledQueues
#endif


#if !defined(ReceiveDanglingMessagesReceivesAtMostOneMessageAtATime) && !defined(noReceiveDanglingMessagesReceivesAtMostOneMessageAtATime)
#define noReceiveDanglingMessagesReceivesAtMostOneMessageAtATime
#endif


#endif
