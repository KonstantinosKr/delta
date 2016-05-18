// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_H_
#define _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_H_


#include <map>


#include "tarch/logging/Log.h"
#include "tarch/timing/Watch.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "peano/datatraversal/autotuning/OracleForOnePhase.h"


namespace peano {
    namespace datatraversal {
      namespace autotuning {
        class Oracle;
      }
  }
}


/**
 * Oracle for the Autotuning
 *
 * The oracle holds a set of OraceForOnePhase instances. If the respositories
 * switch to another adapter, they notify the oracle that they wanna switched
 * (switchToOracle()).
 *
 * <h2> Jobs </h2>
 *
 * @image html peano/datatraversal/autotuning/Oracle-class-diagram.png
 *
 * - Administer oracles.
 * - Keep track of the current phase, i.e. of the adapter used at the moment.
 * - Measure times needed for the parallelisation and give the concrete oracles
 *   feedback about the time needed.
 *
 * @author Svetlana Nogina, Tobias Weinzierl
 */
class peano::datatraversal::autotuning::Oracle {
  private:
    static const int AdapterStatesReservedForRepositorySteering = 4;

    static tarch::logging::Log  _log;

    static tarch::multicore::BooleanSemaphore  _semaphore;

    Oracle();

    struct ValuesPerOracleKey {
      /**
       * The oracle mechanism does not support recursive usage (anymore). In
       * the asserts mode, we check that no recursive calls do pop up.
       */
      int                    _recursiveCallsForThisOracle;
      int                    _lastGrainSizeReturned;
      bool                   _measureTime;
      tarch::timing::Watch*  _watch;
      OracleForOnePhase*     _oracle;
    };

    ValuesPerOracleKey*                        _oracles;

    /**
     * Timer for whole iterations
     *
     * This timer keeps track how long one iteration did last. It is restarted
     * every time one calls switchToOracle(), i.e. at the beginning of each
     * Peano grid sweep.
     */
    tarch::timing::Watch                       _watchSinceLastSwitchCall;

    /**
     * Oracle, method and problem size must not change between the
     * parallelise() and loopHasTerminated() calls.
     */
    int                                        _currentPhase;

    OracleForOnePhase*                         _oraclePrototype;

    int                                        _numberOfOracles;

    void createOracles();
    void deleteOracles();

    int getTotalNumberOfOracles() const;
    int getKey(const MethodTrace& askingMethod ) const;
  public:
    ~Oracle();

    static Oracle& getInstance();

    /**
     * Plot some statistics on the parallel usage. You have to call this
     * operation explicitly, if you are interested in the statistics. The data
     * goes into a file if a filename is specified. If no filename is specified
     * (empty string), we plot into the logInfo device.
     *
     * @see OracleForOnePhase
     */
    void plotStatistics(const std::string& filename);

    /**
     *
     * @see OracleForOnePhase
     *
     */
    void loadStatistics(const std::string& filename);

    /**
     * Tell the oracle how many different adapters you'll gonna use.
     */
    void setNumberOfOracles(int value);

    /**
     * Tell the oracle which adapter is used right now.
     *
     * You have to call setOracle() before. Otherwise, the operation fails with
     * an assertion A typical .call is
     *
     * @pre setNumberOfOracles() has to be called before, and id has to
     *      be smaller then setNumberOfOracles' value. setNumberOfOracles()
     *      typically is invoked by the repositories, i.e. you don't have to
     *      care about this as the repositories are generated automatically.
     *
     * @param id This is the adapter number. See your repository's state if
     *           you wanna know which adapter is mapped to which adapter
     *           number.
     */
    void switchToOracle(int id);

    /**
     * Set the active oracle. Do not delete this argument - the oracle will
     * do it.
     */
    void setOracle( OracleForOnePhase* oraclePrototype );

    /**
     * Ask oracle whether to run a piece of code in parallel and what data
     * chunk to use. Besides the analysis, the operation also starts an
     * internal timer, i.e. you have to tell the oracle afterwards that your
     * loop has terminated. To do so, use loopHasTerminated().
     *
     * <h2> Implementation </h2>
     *
     * - The operation ain't const as it has to start the timer.
     * - If one operation asked twice for parallelisation, the oracle returns
     *   a no, i.e. the level of folded parallelism is restricted. If the same
     *   operation asked twice whether it should run in parallel, the first
     *   one gets the real oracle's answer, the second one always a no.
     *
     * @return grain size describing minimum size how to split up problem or
     *         zero this code piece shall not run in parallel
     */
    int parallelise( int problemSize, MethodTrace askingMethod );

    /**
     * The running time for the tested grain size is measured after loop terminating, to estimate
     * best grain size.
     *
     * The grain size for the next iteration is set to the middle
     * between the former best grain size and the recently tested grain size.
     * If the difference between both is <= 1, test is finished for this oracle-method-size triple.
     */
    void parallelSectionHasTerminated( MethodTrace askingMethod );
};

#endif
