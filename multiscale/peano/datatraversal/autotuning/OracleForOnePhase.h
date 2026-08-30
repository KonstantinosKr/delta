// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_FOR_ONE_PHASE_H_
#define _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_FOR_ONE_PHASE_H_


#include <map>
#include <string>


#include "peano/datatraversal/autotuning/GrainSize.h"


namespace peano {
  namespace datatraversal {
    namespace autotuning {
      class OracleForOnePhase;
    }
  }
}



/**
 * Abstract superclass of all oracles
 *
 * <h2> Copy constructor </h2>
 *
 * The oracle singleton never works with the original oracle. Instead, it
 * clones its oracle strategy for each new phase (see createNewOracle()).
 * There is one oracle per adapter.
 *
 * @author Tobias Weinzierl
 */
class peano::datatraversal::autotuning::OracleForOnePhase {
  public:
    virtual ~OracleForOnePhase() {}

    /**
     * This operation is not const, as the oracle might insert further computations.
     *
     * @see peano::datatraversal::autotuning::Oracle::parallelise()
     *
     * @return Tuple with grain size plus flag indicating weather you wanna be informed about runtime
     */
    virtual GrainSize parallelise(int problemSize, MethodTrace askingMethod) = 0;

    /*
     *
     * Informs oracle that the parallel code fraction has terminated. The last
     * fraction is the one, parallelise() has been called for before. There's
     * never more than one section running in parallel.
     */
    virtual void parallelSectionHasTerminated(int problemSize, int grainSize, MethodTrace askingMethod, double costPerProblemElement) = 0;

    /**
     * Plot statistics.
     *
     * Plot the statistics data into a file. You may also hand in std::cout.
     */
    virtual void plotStatistics(std::ostream& out, int oracleNumber) const = 0;

    /**
     * Load statistics from a file. Not every oracle has to support this
     * operation, i.e. the code might be empty.
     *
     * @param filename
     * @param oracleNumber
     */
    virtual void loadStatistics(const std::string& filename, int oracleNumber) = 0;

    /**
     * This operation is called by the oracle (management) on the active oracle
     * before it activates another one.
     */
    virtual void deactivateOracle() = 0;
    virtual void activateOracle() = 0;

    /**
     * Clone this oracle. This operation is used by the singleton whenever a
     * piece of code asks for parallelisation that never asked before.
     *
     * @param adapterNumber Number of your adapter. Have a closer look to your
     *        repository's state if you want to find out which adapters are
     *        mapped to which state. You can even use the toString() operation
     *        there to map this parameter to a string. Sometimes, I use the
     *        term phase as an alias. See NumberOfPredefinedAdapters.
     */
    virtual OracleForOnePhase* createNewOracle() const = 0;
};


#endif
