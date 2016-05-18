// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_CELL_H_
#define _PEANO_GRID_CELL_H_


#include "peano/utils/PeanoOptimisations.h"
#include "peano/grid/State.h"


namespace peano {
    namespace grid {
      namespace aspects {
        class CellRefinement;
        class CellPeanoCurve;
        class CellLocalPeanoCurve;
        class ParallelMerge;
      }

      namespace tests {
        class RefinementTest;
        class CellLocalPeanoCurveTest;
      }

      template <class CellData>
      class Cell;
    }
}



/**
 * Super class for all cells.
 *
 * @author Tobias Weinzierl
 */
template <class CellData>
class peano::grid::Cell {
  public:
    /**
     * This class exists to allow Peano to overload the standard constructor.
     */
    class DoNotCallStandardConstructor {};

    #if defined(StoreOnlyPersistentAttributes) && defined(PackedRecords)
    typedef typename CellData::Packed::PersistentRecords  PersistentCell;
    #endif

    #if !defined(StoreOnlyPersistentAttributes) && defined(PackedRecords)
    typedef typename CellData::Packed                     PersistentCell;
    #endif

    #if defined(StoreOnlyPersistentAttributes) && !defined(PackedRecords)
    typedef typename CellData::PersistentRecords          PersistentCell;
    #endif

    #if !defined(StoreOnlyPersistentAttributes) && !defined(PackedRecords)
    typedef CellData                                      PersistentCell;
    #endif

    typedef CellData                                      Records;

    #ifdef Parallel
    /**
     * This operation is robust, i.e. you can call it several times.
     */
    static void initDatatype();
    static void shutdownDatatype();
    #endif

    #if defined(ParallelExchangePackedRecordsBetweenMasterAndWorker)
    typedef typename CellData::Packed       MPIDatatypeContainer;
    #else
    typedef CellData                        MPIDatatypeContainer;
    #endif
  private:
    /**
     * Logging device
     */
    static tarch::logging::Log _log;

  protected:
    /**
     * Aspect
     */
    friend class peano::grid::aspects::CellRefinement;
    friend class peano::grid::aspects::CellPeanoCurve;
    friend class peano::grid::aspects::CellLocalPeanoCurve;
    friend class peano::grid::aspects::ParallelMerge;

    friend class peano::grid::tests::RefinementTest;
    friend class peano::grid::tests::CellLocalPeanoCurveTest;


    CellData _cellData;

    Cell();
    Cell(const PersistentCell& argument);
    Cell(const DoNotCallStandardConstructor&);

  public:
    ~Cell();

    bool isOutside() const;
    bool isInside() const;

    /**
     * Switch cell to inside
     *
     * @pre not isInside()
     */
    void switchToInside();

    /**
     * Switch cell to outside
     *
     * @pre isInside()
     */
    void switchToOutside();

    /**
     * Only allowed for a leaf.
     *
     * @param rootLevel Level of root element, i.e. 0 for a serial run.
     */
    void switchToRoot(int rootLevel);

    /**
     * Only allowed for refined events. There is no counterpart of this
     * operation, i.e. an operation making a leaf a refined event, as this
     * switching is done by the refinement aspect.
     */
    void switchToLeaf();

    bool isLeaf() const;
    bool isRefined() const;
    bool isRoot() const;

    /**
     * @return The persistent subattributes of the cell.
     */
    PersistentCell getRecords() const;

    /**
     * Proxy.
     */
    std::string toString() const;

    /**
     * Proxy.
     */
    void toString(std::ostream& out) const;

    /**
     * Is cell remote?
     *
     * We need the state as a cell might be assigned to a different rank,
     * however this fork is only triggered. In this case, we nevertheless
     * have to handle it locally, even if it will be streamed to another
     * process next.
     */
    template <class State>
    bool isRemote(
      const State&  state,
      bool          considerForkingNodesToBeRemote,
      bool          considerJoiningNodesToBeRemote
    ) const;


    /**
     * !!! Warning
     *
     * This operation is used by the send and receive buffers, e.g.
     * It violates the ideas of OO and thus should be used very carefully.
     */
    void setCellData(const CellData& cellData);

    /**
     * !!! Warning
     *
     * This operation is used by the send and receive buffers, e.g.
     * It violates the ideas of OO and thus should be used very carefully.
     */
    CellData getCellData() const;

    /*
     * !!! Warning
     *
     * This operation is used by the send and receive buffers, e.g.
     * It violates the ideas of OO and thus should be used very carefully.
     */
    CellData& getCellData();

    #ifdef Parallel
    bool isCellAForkCandidate() const;

    /**
     * By default, each cell is a fork candidate and can be forked by Peano to
     * a different rank. With this operation, you can explicitly filter out
     * cells you don't want to be forked.
     */
    void setCellIsAForkCandidate(bool value);

    bool isAssignedToRemoteRank() const;

    /**
     * Blocking send. initDatatype() has to be called before.
     */
    void send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallel, bool exchangeDataBlocking );

    /**
     * Blocking receive. initDatatype() has to be called before.
     */
    void receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallel, bool exchangeDataBlocking );

    void assignToRemoteNode( int rank );

    int getRankOfRemoteNode() const;

    std::bitset<DIMENSIONS> getEvenFlags() const;

    void assignToLocalNode();

    /**
     * This operation is exclusively used by the Root node if it is running on
     * a parallel worker.
     */
    void replaceAccessNumbersAndEvenFlags( const Cell& otherCell );

    /**
     * Get Workload
     *
     * This operation returns the node's serial workload plus the workload of
     * all children (recursively). For this workload analysis, it does not take
     * any remote node into account, i.e. you get the workload on the local
     * rank.
     *
     * Please note that this workload is updated on regular subgrids if and
     * only if you specify your mapping to call ascend on the whole tree. On
     * changing/instationary subgrids, I update it in each traversal.
     *
     * @todo I think we should remove this workload model from the code
     */
    double getLocalWorkload() const;

    /**
     * Get Serial Workload
     *
     * This operation returns the node's serial workload plus the workload of
     * all children (recursively). For this workload analysis, it does not
     * distinguish between remote and local cells.
     *
     * Please note that this workload is updated on regular subgrids if and
     * only if you specify your mapping to call ascend on the whole tree. On
     * changing/instationary subgrids, I update it in each traversal.
     */
    double getTotalWorkload() const;

    double getMinWorkload() const;
    double getMaxWorkload() const;
    /**
     * Set Node Workload
     *
     * Each node has an attribute denoting its workload. Use this operation to
     * calibrate the workload. By default, it is set to 0 for outer cells and 1
     * for inner cells. Note that you can also modify out cell's workload within
     * the destroyCell() event.
     */
    void setNodeWorkload( double value );

    void clearWorkloadAndSubtreeFlags();
    void incWorkloadAndReduceSubtreeFlags( const Cell& childCell );

    /**
     * Copy mechanism. Works for both worker and master.
     */
    void setRemoteCellWorkloadAndSubtreeFlags( const Cell& workerCell );

    bool thisSubtreeHoldsWorker() const;
    #else
    bool isAssignedToRemoteRank() const {
      return false;
    }
    #endif

    #ifdef Debug
    int getLevel() const;
    void incLevel();
    #endif


    #ifdef SharedMemoryParallelisation
    /**
     * Clear statistics about unrolled tree
     *
     * This operation clears internal statistics, i.e. how many vertices have
     * been loaded in which subtree without temporary stack access. It is
     * invoked by the default grid traversal routines.
     */
    void clearInputOutputStackAccessStatistics();

    void setInputOutputStackAccessStatistics(int input, int output);

    void incrementLoadFromInputStackCounter();
    void incrementStoreToOutputStackCounter();

    /**
     * The only operation invoking this operation is the task StoreSubCells.
     * As this one is not parallelised itself, there is no need to make the
     * operation thread-safe.
     */
    void restrictInputOutputStackAccessStatistics( const Cell<CellData>& subcell );

    /**
     * Get Number of Loads From Input Stream Done by This Cell And Its Successors
     *
     * This method does not return the field input data but the number of
     * stores to the output stream. The reason is that we keep track of
     * these numbers in each iteration. Whenever an iteration is done and
     * we run again through the grid, the grid traversal is inverted, and
     * the numbers of loads and stores are to be interchanged.
     *
     * If the result equals 0, the result is invalid even for regular subtrees.
     * The reason is hidden in the realisation: Throughout the top-down, we
     * clear the statistics and re-analyse them every time for standard
     * spacetree nodes. Only if we encounter a regular subtree, we do not
     * erase the information as it hasn't changed since the last iteration.
     * For regular subtrees, the root node's information hence is lost already
     * when you enter LoadVerticesOnRegularRefinedPatch.
     */
    int getNumberOfLoadsFromInputStack() const;

    /**
     * @see getNumberOfLoadsFromInputStack()
     */
    int getNumberOfStoresToOutputStack() const;

    /**
     * Usually, we do analyse the number of loads and stores to input and
     * output stacks, respectively, all the time. This information is not
     * analysed, if we encounter a regular subtree. As I do not reanalyse
     * it, I have to switch store and load statistics after each iteration.
     */
    void storedCellOfRegularRefinedGrid();
    #endif

};


template <class CellData>
std::ostream& operator<<(std::ostream& out, const peano::grid::Cell<CellData>& properties);


#include "peano/grid/Cell.cpph"

#endif
