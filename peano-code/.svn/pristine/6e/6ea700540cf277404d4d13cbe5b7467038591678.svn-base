// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_FOR_ONE_PHASE_H_
#define _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_FOR_ONE_PHASE_H_


#include <map>
#include <string>


namespace peano {
  namespace datatraversal {
    namespace autotuning {
      class OracleForOnePhase;

      enum MethodTrace {
        LoadVertices                                     = 0,
        LoadVerticesOnRegularStationaryGrid              = 1, // not unrolled
        LoadVerticesOnIrregularStationaryGrid            = 2,
        StoreVertices                                    = 3,
        StoreVerticesOnRegularStationaryGrid             = 4, // not unrolled
        StoreVerticesOnIrregularStationaryGrid           = 5,
        CallEnterCellAndLoadSubCellsWithinTraverse       = 6,
        CallLeaveCellAndStoreSubCellsWithinTraverse      = 7,
        CallEnterCellAndInitialiseEnumeratorsOnRegularStationaryGrid = 8,
        CallTouchFirstTimeOnRegularStationaryGrid        = 9,
        CallTouchLastTimeOnRegularStationaryGrid         = 10,
        CallEnterCellOnRegularStationaryGrid             = 11,
        CallLeaveCellOnRegularStationaryGrid             = 12,
        PipelineAscendTask                               = 13,
        PipelineDescendTask                              = 14,
        SplitLoadVerticesTaskOnRegularStationaryGrid     = 15,
        SplitStoreVerticesTaskOnRegularStationaryGrid    = 16,
        AscendOnRegularStationaryGrid                    = 17,
        DescendOnRegularStationaryGrid                   = 18,

        UserDefined0  = 19,
        UserDefined1  = 20,
        UserDefined2  = 21,
        UserDefined3  = 22,
        UserDefined4  = 23,
        UserDefined5  = 24,
        UserDefined6  = 25,
        UserDefined7  = 26,
        UserDefined8  = 27,
        UserDefined9  = 28,
        UserDefined10 = 29,
        UserDefined11 = 30,
        UserDefined12 = 31,

        NumberOfDifferentMethodsCalling                  = 32
      };

      /**
       * The repository management needs a couple of adapters itself. So, if you
       * speak about adapter no 5 given you by the repository statistics, it is
       * actually the 8th adapter.
       */
      const int NumberOfPredefinedAdapters = 4;

      std::string toString( const MethodTrace& methodTrace );
      MethodTrace toMethodTrace(int value);
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
 *
 * @author Svetlana Nogina, Tobias Weinzierl
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
    virtual std::pair<int,bool> parallelise(int problemSize) = 0;

    /**
     * Informs oracle that the parallel code fraction has terminated. The last
     * fraction is the one, parallelise() has been called for before. There's
     * never more than one section running in parallel.
     */
    virtual void parallelSectionHasTerminated(double elapsedCalendarTime) = 0;

    /**
     * Plot statistics.
     *
     * Plot the statistics data into a file. If the filename is empty, all
     * statistics are written onto the info log level. Several oracles also are
     * able to use statistics to configure themselves and, thus, are able to
     * read statistics as well.
     *
     * The statistics usually are
     */
    virtual void plotStatistics(std::ostream& out) const = 0;

    /**
     * Load statistics from a file. Not every oracle has to support this
     * operation, i.e. the code might be empty.
     */
    virtual void loadStatistics(const std::string& filename) = 0;

    /**
     * This operation is called by the oracle (management) on all oracles. Can
     * be used to adopt oracle behaviour to global runtime.
     */
    virtual void informAboutElapsedTimeOfLastTraversal(double elapsedTime) = 0;

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
    virtual OracleForOnePhase* createNewOracle(int adapterNumber, const MethodTrace& methodTrace) const = 0;
};


#endif
