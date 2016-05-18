// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_FOR_ONE_PHASE_DUMMY_H_
#define _PEANO_DATA_TRAVERSAL_AUTOTUNING_ORACLE_FOR_ONE_PHASE_DUMMY_H_


#include "tarch/logging/Log.h"
#include "peano/datatraversal/autotuning/OracleForOnePhase.h"
#include "tarch/timing/Measurement.h"
#include "tarch/la/ScalarOperations.h"
#include "peano/utils/Globals.h"


#include <map>


namespace peano {
  namespace datatraversal {
    namespace autotuning {
      class OracleForOnePhaseDummy;
    }
  }
}


/**
 * Default oracle (dummy)
 *
 * This oracle allows you to configure some fixed default values for all grid
 * run throughs. By default, it enables multithreading and also splits up the
 * tree, while is does not measure any runtimes.
 *
 * If you run this oracle on a decently fine grid with sufficient regularity,
 * you will obtain forked tasks in the background. To avoid those, you have to
 * set the third parameter in the constructor (splitTheTree) to 0.
 *
 *
 * @author Tobias Weinzierl
 */
class peano::datatraversal::autotuning::OracleForOnePhaseDummy: public peano::datatraversal::autotuning::OracleForOnePhase {
  private:
    static tarch::logging::Log                 _log;

    const bool                                 _useMulticore;
    const bool                                 _measureRuntimes;
    const int                                  _grainSizeOfUserDefinedRegions;
    std::map<int, tarch::timing::Measurement>  _executionTime;
    const int                                  _adapterNumber;
    const MethodTrace                          _methodTrace;
    const int                                  _splitTheTree;
    const bool                                 _pipelineDescendProcessing;
    const bool                                 _pipelineAscendProcessing;

    int                                        _grainSize;
    int                                        _smallestProblemSize;

    int                                        _lastProblemSize;

    const int                                  _smallestGrainSizeForAscendDescend;
    const int                                  _grainSizeForAscendDescend;

    const int                                  _smallestGrainSizeForEnterLeaveCell;
    const int                                  _grainSizeForEnterLeaveCell;

    const int                                  _smallestGrainSizeForTouchFirstLast;
    const int                                  _grainSizeForTouchFirstLast;

    const int                                  _smallestGrainSizeForSplitLoadStore;
    const int                                  _grainSizeForSplitLoadStore;
  public:
    /**
     * Dummy oracle
     *
     * @param splitTheTree (0=no, 1=yes and parallelise, 2=yes, but do not parallelise any events on the regular subgrid
     */
    OracleForOnePhaseDummy(
      bool useMultithreading                  = true,
      bool measureRuntimes                    = false,
      int  grainSizeOfUserDefinedRegions      = 0,
      int  splitTheTree                       = 1,
      bool pipelineDescendProcessing          = false,
      bool pipelineAscendProcessing           = false,
      int  smallestGrainSizeForAscendDescend  = tarch::la::aPowI(DIMENSIONS,3*3*3*3/2),
      int  grainSizeForAscendDescend          = 3,
      int  smallestGrainSizeForEnterLeaveCell = tarch::la::aPowI(DIMENSIONS,9/2),
      int  grainSizeForEnterLeaveCell         = 2,
      int  smallestGrainSizeForTouchFirstLast = tarch::la::aPowI(DIMENSIONS,3*3*3*3+1),
      int  grainSizeForTouchFirstLast         = 64,
      int  smallestGrainSizeForSplitLoadStore = tarch::la::aPowI(DIMENSIONS,3*3*3),
      int  grainSizeForSplitLoadStore         = 8,
      int  adapterNumber                      = -1,
      const MethodTrace& methodTrace          = NumberOfDifferentMethodsCalling
    );

    virtual ~OracleForOnePhaseDummy();

    virtual std::pair<int,bool> parallelise(int problemSize);
    virtual void parallelSectionHasTerminated(double elapsedCalendarTime);
    virtual void plotStatistics(std::ostream& out) const;

    /**
     * Not implemented
     */
    virtual void loadStatistics(const std::string& filename);

    virtual void informAboutElapsedTimeOfLastTraversal(double elapsedTime);

    /**
     * For this oracle type, the adapter number is completely irrelevant.
     */
    virtual OracleForOnePhase* createNewOracle(int adapterNumber, const MethodTrace& methodTrace) const;

    std::string toString() const;
};


#endif
