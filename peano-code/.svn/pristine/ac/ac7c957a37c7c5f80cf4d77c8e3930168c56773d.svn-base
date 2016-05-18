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
#if defined(PackedRecords) && !defined(CompilerCLX)
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

#endif
