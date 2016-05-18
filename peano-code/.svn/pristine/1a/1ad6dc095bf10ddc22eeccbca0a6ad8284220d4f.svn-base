// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_LOADBALANCING_ORACLE_FOR_ONE_PHASE_H_
#define _PEANO_PARALLEL_LOADBALANCING_ORACLE_FOR_ONE_PHASE_H_


#include <map>
#include <string>


#include "peano/utils/Globals.h"
#include "tarch/la/Vector.h"


namespace peano {
  namespace parallel {
    namespace loadbalancing {
      class OracleForOnePhase;

      /**
       * Represents action to be done.
       */
      enum LoadBalancingFlag {
        ContinueButTryToJoinWorkers=-3,
        Continue=-2,
        Join=-1,
        UndefinedLoadBalancingFlag=0,
        ForkOnce=1,
        ForkGreedy=THREE_POWER_D_MINUS_ONE,
        ForkAllChildrenAndBecomeAdministrativeRank=THREE_POWER_D
      };

      std::string convertLoadBalancingFlagToString(int flag);
    }
  }
}


/**
 * Abstract superclass of all oracles
 *
 * This is the principal plugin point for any load balancing strategy. This
 * class decides for existing workers whether these workers shall continue
 * their work, shall merge their domains with their master (where this
 * oracle is running on), or whether they should try to fork further. The
 * important operations of this class are OracleForOnePhase::receivedStartCommand(),
 * OracleForOnePhase::getCommandForWorker(), and OracleForOnePhase::receivedTerminateCommand().
 *
 * @image html peano/parallel/loadbalancing/OracleForOnePhase0.png
 *
 * @image html peano/parallel/loadbalancing/OracleForOnePhase1.png
 *
 * !!! Copy constructor
 *
 * The oracle singleton never works with the original oracle. Instead, it
 * clones its oracle strategy for each new phase (see createNewOracle()).
 *
 * @author Tobias Weinzierl
 */
class peano::parallel::loadbalancing::OracleForOnePhase {
  public:
    virtual ~OracleForOnePhase() {}

    /**
     * Receive start command from master
     *
     * This operation is invoked per grid traversal at the very beginning. The
     * master tells its worker what to do. Hence, also the worker's oracle is
     * informed about the master's strategy. For example, if the master tells
     * this node to fork further, i.e. to reduce its local workload, this might
     * also be relevant to the oracle telling workers of the worker to fork, too.
     *
     * If you wanna keep track of the runtime per worker, the simplest way to
     * realise this is to write a time stamp within this method, as this method
     * is invoked exactly once per traversal.
     *
     * @see Oracle
     *
     * @param commandFromMaster Usually is a value from the enum LoadBalancingFlag
     */
    virtual void receivedStartCommand( int commandFromMaster ) = 0;

    /**
     * Get command for one single worker
     *
     * This way, Peano asks the oracle how to proceed with the worker
     * workerRank. Shall the parallel travesal simply continue, shall the
     * worker in turn try to fork its partition, or shall the worker merge
     * with the master. The latter is not allowed all the time, i.e. if
     * joinIsAllowed is not set, please do not return Join as result.
     *
     * The operation provides a way to implement dynamic load balancing. You
     * always are notices by receivedTerminateCommand() how long a worker has
     * needed to complete its tasks. This is the right plugin point for an
     * oracle for one phase to keep track of the runtimes. If a worker is too
     * slow, you should pass im a fork command the next time it is asking for
     * getCommandForWorker().
     *
     * There is one oracle per phase, i.e. per adapter. Usually, one or two
     * different adapter coin the runtime, while others have completely different
     * runtime characteristics. An example: For a CFD code for incompressible
     * fluids with an explicit time stepping, the update of time steps and the
     * computation of the right-hand side are rather cheap and/or invoked not that
     * often. Most of the time is spent to solve the pressure poisson equation. As
     * a consequence, the load balancing should only be realised in the oracle for
     * this pressure poisson equation.
     *
     * When you design your load balancing, please keep in mind that the whole
     * Peano code defines a logical tree topology on all the ranks, and that the
     * oracles are not working globally but do exist on each MPI rank. This
     * directly yields the design of getCommandForWorker() and
     * receivedTerminateCommand(): The global automaton runs through the spacetree
     * (or regular grid) top-down. Throughout the top-down steps or traversal,
     * respectively, it invokes getCommandForWorker() for its workers. They then
     * start to traverse their own stuff (and to invoke their own workers in
     * turn). When the local tree/grid is traversed, the node waits for its
     * workers and, as soon as they have finished their stuff, calls
     * receivedTerminateCommand().
     *
     * @see Oracle
     * @see receivedTerminateCommand()
     */
    virtual int getCommandForWorker( int workerRank, bool forkIsAllowed, bool joinIsAllowed ) = 0;

    /**
     * Information about termination call
     *
     * Is called whenever the master receives the acknowledgement from the
     * worker that the latter has finished its local traversal. Provides
     * statistics that you might want to bookkeep for load balancing. All t
     * the workload doubles stem from the cells' workload. See
     * Cell::setNodeWorkload() to adopt these figures to your own workload
     * model.
     *
     * @see receivedStartCommand()
     * @see getCommandForWorker()
     * @see Oracle
     *
     * @param workerRank   Rank of the worker that has just reported that it
     *                     finished its traversal.
     * @param workerNumberOfInnerVertices  Number of inner vertices handled by
     *                     this worker. If you require the total number, you
     *                     have to feed your oracle manually within
     *                     endIteration(). Here, you have access to the state
     *                     object holding the total numbers.
     * @param workerNumberOfBoundaryVertices Number of boundary vertices
     *                     handled by this worker.
     * @param workerNumberOfOuterVertices Number of outer vertices handled by
     *                     this worker.
     * @param workerNumberOfInnerCells Number of inner cells handled by
     *                     this worker.
     * @param workerNumberOfOuterCells Number of outer cells handled by this
     *                     worker.
     * @param workerMaxLevel Maximum level handled by the worker. If you
     *                     compare it to current level, you have information
     *                     about the height of the worker tree.
     * @param workerLocalWorkload The workload handled by the worker. This is
     *                     the worker's local workload, i.e. if the worker has
     *                     forked itself again, workload of these children is
     *                     not contained within this figure.
     * @param workerTotalWorkload The workload represented by the worker. This
     *                     number is equal to workerLocalWorkload if the worker
     *                     has not forked again, i.e. if it does not have any
     *                     children. Otherwise, total workload comprises both
     *                     the worker's workload and those of its children.
     * @param currentLevel Current level, i.e. level of the root cell of the
     *                     worker partition.
     * @param parentCellLocalWorkload Local workload of the next coarser cell
     *                     on this rank. This number does not comprise workload
     *                     of any worker, i.e. it does in particular neither
     *                     comprise workerLocalWorkload nor
     *                     workerTotalWorkload.
     * @param parentCellTotalWorkload Total workload of the next coarser cell
     *                     comprising the ranks workload of the whole tree
     *                     induced by this cell plus the workloads of all
     *                     forked subtrees.
     * @param boundingBoxOffset Bounding box of this worker subtree.
     * @param boundingBoxSize   Bounding box of this worker subtree.
     */
    virtual void receivedTerminateCommand(
      int     workerRank,
      double  workerNumberOfInnerVertices,
      double  workerNumberOfBoundaryVertices,
      double  workerNumberOfOuterVertices,
      double  workerNumberOfInnerCells,
      double  workerNumberOfOuterCells,
      int     workerMaxLevel,
      double  workerLocalWorkload,
      double  workerTotalWorkload,
      double  workerMaxWorkload,
      double  workerMinWorkload,
      int     currentLevel,
      double  parentCellLocalWorkload,
      const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset,
      const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize,
      bool    workerCouldNotEraseDueToDecomposition
    ) = 0;

    /**
     * Plot something to info log device.
     */
    virtual void plotStatistics() = 0;

    /**
     * Clone this oracle. This operation is used by the singleton whenever a
     * piece of code asks for parallelisation that never asked before.
     *
     * @param adapterNumber Number of your adapter. Have a closer look to your
     *        repository's state if you want to find out which adapters are
     *        mapped to which state. You can even use the toString() operation
     *        there to map this parameter to a string.
     */
    virtual OracleForOnePhase* createNewOracle(int adapterNumber) const = 0;

    /**
     * @see Oracle
     */
    virtual void forkFailed() = 0;

    /**
     * @see Oracle
     */
    virtual int getCoarsestRegularInnerAndOuterGridLevel() const = 0;
};


#endif
