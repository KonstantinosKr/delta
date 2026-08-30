// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_H_
#define _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_H_


#include <vector>


#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "peano/datatraversal/autotuning/GrainSize.h"
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
 * @author Tobias Weinzierl
 */
class peano::datatraversal::autotuning::Oracle {
  private:
    //    static const int AdapterStatesReservedForRepositorySteering = 4;

    static tarch::logging::Log  _log;

    Oracle();

    /**
     * Set of oracles.
     */
    typedef std::vector<OracleForOnePhase*>  OracleDatabase;

    /**
     * Maps ranks to oracles
     */
    OracleDatabase                           _oracles;

    #if defined(SharedMemoryParallelisation)
    /**
     * Oracle, method and problem size must not change between the
     * parallelise() and loopHasTerminated() calls.
     *
     * On MacOS X/LLVM, the compiler tends to complain if we omit the
     * ifdef as the variable then is set but never used.
     */
    int                                        _currentOracle;
    #endif

    OracleForOnePhase*                         _oraclePrototype;

    int                                        _numberOfOracles;

    void createOracles();
    void deleteOracles();

    ~Oracle();
  public:
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
     * It is important that statistics files are loaded after we have created
     * all oracles through setNumberOfOracles(). The routine runs through all
     * existing oracles and tells each one to load the statistics file. It is
     * consequently important that these do exist before. setNumberOfOracles()
     * is invoked by the repository constructor, i.e. if you have created your
     * repository before you load a statistics file, you are fine.
     */
    void loadStatistics(const std::string& filename);

    /**
     * Tell the oracle how many different adapters you'll gonna use.
     */
    void setNumberOfOracles(int numberOfAdapters);

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
    GrainSize  parallelise( int problemSize, MethodTrace askingMethod );
};

#endif
