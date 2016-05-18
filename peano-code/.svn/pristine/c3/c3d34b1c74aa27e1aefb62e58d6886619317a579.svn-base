// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_REGULAR_GRID_CONTAINER_H_
#define _PEANO_GRID_REGULAR_GRID_CONTAINER_H_


#include "tarch/logging/Log.h"

#include "tarch/multicore/BooleanSemaphore.h"

#include "peano/utils/Globals.h"

#include "peano/grid/SingleLevelEnumerator.h"
#include "peano/grid/UnrolledLevelEnumerator.h"
#include "peano/utils/PeanoOptimisations.h"

#include <vector>

#ifdef SharedTBB
#include <tbb/cache_aligned_allocator.h>
#endif




/**
 * Consistency checks
 */
#if ( \
  !defined(RegularGridContainerUsesPlainArrays) && \
  !defined(RegularGridContainerUsesRawArrays) && \
  !defined(RegularGridContainerUsesSTDArrays) \
  )
  #error No regular grid realisation chosen
#endif

#if ( \
  defined(RegularGridContainerUsesPlainArrays) && \
  (defined(RegularGridContainerUsesRawArrays)  || defined(RegularGridContainerUsesSTDArrays)) \
  )
  #error Multiple regular grid realisations chosen
#endif

#if ( \
  defined(RegularGridContainerUsesRawArrays) && \
  (defined(RegularGridContainerUsesPlainArrays)  || defined(RegularGridContainerUsesSTDArrays)) \
  )
  #error Multiple regular grid realisations chosen
#endif

#if ( \
  defined(RegularGridContainerUsesSTDArrays) && \
  (defined(RegularGridContainerUsesRawArrays)  || defined(RegularGridContainerUsesPlainArrays)) \
  )
  #error Multiple regular grid realisations chosen
#endif


namespace peano {
  namespace grid {
    template <
      class Vertex,
      class Cell
    > class RegularGridContainer;

    /**
     * Are We Allowed to Fork Throughout a Vertex Load or Store Process on a Regular Subtree?
     *
     * We are allowed to fork if the following three properties hold.
     *
     * - The current level is smaller or equal to the maximum level that may be
     *   forked.
     * - The task currently already has processed at least one level. This is
     *   not the case if the current level equals the coarsest level+1 - if
     *   the latter holds, we first have to load at least one level before
     *   forking further.
     * - The @f$ 3^d @f$ cells to be handled next are not on the whole patch's
     *   boundary.
     *
     * Please note that these conditions comprise that current level always is
     * the level that is to be loaded next. Consequently,
     * cellsPerAxisOnCurrentLevel also refers to currentLevel and not to the
     * current coarse grid cell.
     *
     * This operation is used by StoreVerticesOnRegularRefinedPatch and
     * LoadVerticesOnRegularRefinedPatch. They also fix the maxLevelToFork for
     * a whole subtree. It results from the variable SplitStoreVerticesTask
     * there. This variable in turn is the result of peano::grid::nodes::transformOracleResult()
     * which can be found in RegularRefined.
     *
     * @param currentLevel                 Current level of the root of a subtree that might be deployed to a thread of its own. This is always
     *                                     relative to the root of the whole regular subtree that might be split up among different tasks.
     * @param isParentCellAtPatchBoundary  If the parent cell is adjacent to the patch boundary, we may not deploy a subtree to a thread of its own
     * @param maxLevelToFork               It does not pay off to fork too small subtrees. This flag indicates how big a subtree has to be at least.
     *                                     The parameter also allows us to switch off all parallelisation completely.
     * @param coarsestLevelOfThisTask      If coarsestLevelofThisTask<currentLevel then this operation is not called for the root of a subtree.
     * @param expectedNumberOfLoadsOrStores If there are no loads or stored coming from a particular task, it does not make sense to deploy it onto
     *                                     a thread of its own. We want split tasks to read whole blocks from the input stack en block.
     */
    bool mayForkLoadOrStoreVertexTaskOnRegularSubtree(
      const int   currentLevel,
      const bool  isParentCellAtPatchBoundary,
      const int   maxLevelToFork,
      const int   coarsestLevelOfThisTask,
      const int   expectedNumberOfLoadsOrStores
    );
  }
}


template < class Vertex, class Cell >
class peano::grid::RegularGridContainer {
  private:
    static tarch::logging::Log _log;

    static tarch::multicore::BooleanSemaphore _semaphore;

    /**
     * Holds the data of one level of the regularly refined subtree
     *
     * !!! Memory management
     *
     * The level data object is a manager for five arrays representing regular
     * Cartesian meshes. They will after allocation be filled with data from
     * the stacks. Either by one thread or multiple threads (split tree
     * strategy).
     *
     * In the construction phase (cmp. operation init()), we always try to
     * allocate the required memory. Try means that we use new with
     * std::nothrow. If the allocation fails, init() returns false and the
     * calling algorithmic phase then knows that the grid container cannot be
     * used for that big regular subgrids, i.e. it cannot use recursion
     * unrolling here. In that sense, the container is memory safe.
     *
     * As all the elements of the Cartesian regular mesh are befilled with
     * data from the input stream, it does not make any sense to call the
     * default constructor for the entries. Using the default constructor not
     * only requires time, it also induces problems in a shared memory
     * environment if the system relies on a touch-first placement strategy.
     * In this case, the master thread initialises the Cartesian mesh, i.e. it
     * calls the default constructor, i.e. all memory is paged to thread 0.
     *
     * For these reasons, we use non-initialised memory and the placement-new
     * strategy, and hence need two pointers per administered array. Please
     * consult Scott Meyers' More Effective C++ Chapter 1.4 for more details
     * on this technique.
     */
    struct LevelData {
      public:
        #if defined(RegularGridContainerUsesSTDArrays) && !defined(SharedTBB)
          std::vector<Vertex>  vertex;
          std::vector<Cell>    cell;
          std::vector<int>     counter;
          std::vector<bool>    isReadFromTemp;
          std::vector<bool>    isToBeWrittenToTemp;
        #elif defined(RegularGridContainerUsesSTDArrays) && defined(SharedTBB)
          std::vector<Vertex,tbb::cache_aligned_allocator<Vertex> >  vertex;
          std::vector<Cell,tbb::cache_aligned_allocator<Cell> >      cell;
          std::vector<int,tbb::cache_aligned_allocator<int> >        counter;
          std::vector<bool,tbb::cache_aligned_allocator<bool> >      isReadFromTemp;
          std::vector<bool,tbb::cache_aligned_allocator<bool> >      isToBeWrittenToTemp;
        #else
        /**
         * Vertices
         */
        Vertex*  vertex;

        /**
         * Cells
         */
        Cell*    cell;

        /**
         * Counter
         *
         * The counters here always start to count from persistent vertex
         * upwards. However, they are not always increased/decreased
         * @f$ 2^d @f$ times - if inner subtrees are deployed to separate
         * threads, their increases/decreases are not conducted.
         */
        int*     counter;

        /**
         * I need this flag to determine later whether I have to call
         * touchVertexFirstTime() or not.
         */
        bool*    isReadFromTemp;

        /**
         * I need this flag to determine later whether I have to call
         * touchVertexLastTime() or not.
         */
        bool*    isToBeWrittenToTemp;
        #endif

        /**
         * Holds some statistics how often a level is needed. Besides the
         * statistics, the container also deletes the finest level data if it
         * has not been used in the iteration before. This way, I try to reduce
         * the total memory footprint if possible.
         */
        int      usedPerTraversal;

        /**
         * Each level has @f$ 3^d @f$ patches inside that are befilled en block
         * by Peano. This counter holds how many of them are not set yet.
         */
        int      uninitalisedThreeTimeThreePatchesVertices;
        int      uninitalisedThreeTimeThreePatchesCells;

        /**
         * Is protected by the vertex semaphore
         */
        bool     haveCalledAllEventsOnThisLevel;

        UnrolledLevelEnumerator  enumerator;

        /**
         * NOP
         *
         * Use init() instead.
         */
        LevelData();

        /**
         * Counterpart of init().
         */
        ~LevelData();

        /**
         * Init
         *
         * Allocates and initialises the data structures but does not set the
         * enumerator as the enumerator's state depends on the actual spatial
         * position of the patch, i.e. might change several times throughout each
         * traversal.
         */
        bool init( int level );

        static double getApproximateMemoryFootprint(int level);
      private:
        #if defined(RegularGridContainerUsesRawArrays)
          void*    rawVertex;
          void*    rawCell;
          void*    rawCounter;
          void*    rawIsReadFromTemp;
          void*    rawIsToBeWrittenToTemp;
        #endif


        void freeHeap();
    };

    /**
     * Holds the data of the individual levels. Entry 0, i.e. the first entry,
     * is level 0, i.e. the one-cell-level.
     */
    std::vector<LevelData*>  _data;

    double                   _maximumMemoryFootprintForTemporaryRegularGrid;

    /**
     * Only for assert mode
     */
    void validateThatRegularSubtreeIsAvailable( int level ) const;

    void validateInitialValuesUpToLevel( int level ) const;

    /**
     * Noone shall ever copy a grid container.
     */
    RegularGridContainer(const RegularGridContainer& copy):
      _data(),
      _maximumMemoryFootprintForTemporaryRegularGrid(1.0) {
    }
  public:
    RegularGridContainer();
    ~RegularGridContainer();


    /**
     * Does some logging statements, resets the usage counters per grid level,
     * and frees the biggest level if it hasn't been used in this iteration.
     * This way, I try permanently to decrease the memory footprint of the
     * application.
     */
    void endOfIteration();

    /**
     * Ask container whether regular grid is supported
     *
     * This is not a const operation though it sounds like that. What the
     * operation does is a sequence of steps:
     *
     * # Check whether such an array can be loaded with the memory already
     *   allocated before. If yes, return immediately.
     * # Check whether the container is allowed to allocate additional
     *   levels.
     * # Then, the operation tries to allocate memory for up to the given
     *   level if not available already. If an out-of-memory exception pops up,
     *   we stop trying.
     *
     * If the operation returns false, the grid management is not allowed to do the
     * recursion unrolling on the tree and falls back to the standard run. As
     * a consequence, it will automatically try on the next finer level to
     * use the unrolling there.
     *
     * @param requestedHeight>0
     */
    bool isRegularSubtreeAvailable( int requestedHeight );
    
    void haveReadPatchsVertices( int level );

    void haveReadPatchsVertices( const std::vector<int>& readsOfPatchesPerLevel );

    /**
     * Is used by the load cells task to inform the container level by level
     * whenever cells have been read. This operation is used only by the task,
     * if it runs in parallel to other descend/load tasks. Otherwise, it skips
     * the level-by-level setter and sets all the level at once in the very
     * end (see haveReadAllPatchsCells()).
     */
    void haveReadPatchsCells( int level );

    void haveReadAllPatchsCells( int maxLevel );

    void haveStoredAllPatchsVertices( int maxLevel );
    void haveStoredAllPatchsCells( int maxLevel );

    /**
     * All events called on this level
     *
     * This operation is invoked by the ascend task, i.e. when the task runs
     * bottom-up through the tree and invoked touchVertexLastTime() on each
     * vertex. It is called only once per level.
     */
    void haveCalledAllEventsOnThisLevel( int level );
    
    /**
     * Is a level completely loaded, i.e. is all data read
     *
     * Is thread-safe. I have to use two semaphores. As the cell load process   
     * uses to be signficiantly faster than the vertex load process, the
     * operation first blocks the cell semaphore (doesn't matter if the cell 
     * load process is delayed) and then the vertex semaphore.
     *
     * @see haveReadPatch()
     * @see haveStoredPatch()
     */
    bool isLevelInitialised( int level ) const;

    /**
     * Subpart of isLevelIinitialised();
     */
    bool areCellsOfLevelLoaded( int level ) const;
    bool areAllEventsOnThisLevelCalled( int level ) const;

    /**
     * Attention: Level starts to count with 1.
     */
    void setVertexEnumerator(
      int                             level,
      const UnrolledLevelEnumerator&  enumerator
    );

    UnrolledLevelEnumerator& getVertexEnumerator( int level );
    const UnrolledLevelEnumerator& getVertexEnumerator( int level ) const;

    Cell& getCell( int level, int cellIndex );
    const Cell& getCell( int level, int cellIndex ) const;
    void setCell( int level, int cellIndex, const Cell& cell );
    Cell* getCell( int level );

    int& getCounter( int level, int vertexIndex );

    Vertex& getVertex( int level, int vertexIndex );
    const Vertex& getVertex( int level, int vertexIndex ) const;
    void setVertex( int level, int vertexIndex, const Vertex&  vertex );
    Vertex* getVertex( int level );

    void setIsReadFromTemporaryStack( int level, int vertexIndex, bool value );
    void setIsToBeWrittenToTemporaryStack( int level, int vertexIndex, bool value );

    bool isReadFromTemporaryStack( int level, int vertexIndex) const;
    bool isToBeWrittenToTemporaryStack( int level, int vertexIndex) const;

    /**
     * Copy cell and vertices into data structure of regular grid. The vertex
     * enumerator is not copied.
     */
    void copyRootNodeDataIntoRegularPatch(
      const Cell&                               fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator
    );

    /**
     * Counterpart of copyRootNodeDataIntoRegularPatch().
     */
    void copyRootNodeDataFromRegularPatch(
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator
    ) const;

    static tarch::la::Vector<DIMENSIONS,int> getNumberOfCells( int level );
    static tarch::la::Vector<DIMENSIONS,int> getNumberOfVertices( int level );

    std::string toString() const;

    bool isCellAtPatchBoundaryWithinRegularSubtree(
      const tarch::la::Vector<DIMENSIONS,int>&  offsetWithinPatch,
      const int                                 level
    ) const;

    void setMaximumMemoryFootprintForTemporaryRegularGrids(double value);
};


#include "peano/grid/RegularGridContainer.cpph"

#endif
