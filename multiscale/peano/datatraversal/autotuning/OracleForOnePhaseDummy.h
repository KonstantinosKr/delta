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
  public:
    enum class SplitVertexReadsOnRegularSubtree {
      DoNotSplit,
      Split,
      SplitButDoNotParalleliseEvents
    };

    static std::string toString(SplitVertexReadsOnRegularSubtree value);
  private:
    static tarch::logging::Log                 _log;

    const bool                                 _useMulticore;
    const int                                  _grainSizeOfUserDefinedRegions;
    const SplitVertexReadsOnRegularSubtree     _splitTheTree;
    const bool                                 _pipelineDescendProcessing;
    const bool                                 _pipelineAscendProcessing;

    const int                                  _smallestProblemSizeForAscendDescend;
    const int                                  _grainSizeForAscendDescend;

    const int                                  _smallestProblemSizeForEnterLeaveCell;
    const int                                  _grainSizeForEnterLeaveCell;

    const int                                  _smallestProblemSizeForTouchFirstLast;
    const int                                  _grainSizeForTouchFirstLast;

    const int                                  _smallestProblemSizeForSplitLoadStore;
    const int                                  _grainSizeForSplitLoadStore;
  public:

    /**
     * Dummy oracle
     *
     * @param useMultithreading               Allows users to switch multithreading completely on/off
     * @param grainSizeOfUserDefinedRegions   Value used as grain size in user-defined regions by default. 0 (switching off) works if there are no user-defined parallel regions
     * @param splitTheTree                    See enumeration SplitVertexReadsOnRegularSubtree
     * @param pipelineDescendProcessing       Enable pipelining, i.e. the overlapping of data loads and processing
     * @param pipelineDescendProcessing       Enable pipelining, i.e. the overlapping of data processing and stores
     * @param smallestProblemSizeForAscendDescend  If a problem size is small than this threshold, no parallelisation is used
     * @param grainSizeForAscendDescend            If a problem is bigger than the threshold, then this grain size is used
     */
    OracleForOnePhaseDummy(
      bool useMultithreading                  = true,
      int  grainSizeOfUserDefinedRegions      = 0,
      SplitVertexReadsOnRegularSubtree splitTheTree = SplitVertexReadsOnRegularSubtree::Split,
      bool pipelineDescendProcessing          = false,
      bool pipelineAscendProcessing           = false,
      int  smallestProblemSizeForAscendDescend  = tarch::la::aPowI(DIMENSIONS,3*3*3*3/2),
      int  grainSizeForAscendDescend          = 3,
      int  smallestProblemSizeForEnterLeaveCell = tarch::la::aPowI(DIMENSIONS,9/2),
      int  grainSizeForEnterLeaveCell         = 2,
      int  smallestProblemSizeForTouchFirstLast = tarch::la::aPowI(DIMENSIONS,3*3*3*3+1),
      int  grainSizeForTouchFirstLast         = 64,
      int  smallestProblemSizeForSplitLoadStore = tarch::la::aPowI(DIMENSIONS,3*3*3),
      int  grainSizeForSplitLoadStore         = 8
    );

    virtual ~OracleForOnePhaseDummy();

    GrainSize parallelise(int problemSize, MethodTrace askingMethod) override;
    void parallelSectionHasTerminated(int problemSize, int grainSize, MethodTrace askingMethod, double costPerProblemElement) override;

    /**
     * This routine dumps the used settings into the specified output file. We
     * actually do dump it twice, i.e. in two formats. There's a comment-like
     * line first that summarises all arguments of the constructor. Below that,
     * we create "fake" learning entries in a format that is compatible with
     * the shrinking oracle postprocessing scripts.
     */
    void plotStatistics(std::ostream& out, int oracleNumber) const override;

    /**
     * Not implemented
     */
    void loadStatistics(const std::string& filename, int oracleNumber) override;

    void deactivateOracle() override;
    void activateOracle() override;

    /**
     * For this oracle type, the adapter number is completely irrelevant.
     */
    OracleForOnePhase* createNewOracle() const override;

    std::string toString(int oracleNumber=-1) const;
};


#endif
