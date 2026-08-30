// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PERFORMANCE_ANALYIS_DEFAULT_ANALYSER_H_
#define _PEANO_PERFORMANCE_ANALYIS_DEFAULT_ANALYSER_H_


#include "peano/performanceanalysis/Analyser.h"

#include "tarch/logging/Log.h"
#include "tarch/timing/Watch.h"

#include "tarch/multicore/BooleanSemaphore.h"


namespace peano {
  namespace performanceanalysis {
    class DefaultAnalyser;
  }
}



/**
 * Default Analyser
 *
 * This analyser pipes all data to the info output. It is usually used in
 * combination with the Python script, i.e. you run the code with this
 * analyser and afterwards postprocess the output with the script.
 *
 * @author Roland Wittmann, Tobias Weinzierl
 */
class peano::performanceanalysis::DefaultAnalyser: public peano::performanceanalysis::Analyser {
  private:
    static tarch::logging::Log     _log;

    bool _isSwitchedOn;

    /**
     * Total watch runs from one endIteration() to the next one.
     */
    tarch::timing::Watch           _totalWatch;

    /**
     * This one runs between beginIteration() and endIteration(). Cmp to _totalWatch.
     */
    tarch::timing::Watch           _traversalWatch;
    tarch::timing::Watch           _actualDomainTraversalWatch;
    tarch::timing::Watch           _waitForWorkerDataWatch;
    tarch::timing::Watch           _waitForMasterDataWatch;
    tarch::timing::Watch           _synchronousHeapWatch;
    tarch::timing::Watch           _asynchronousHeapWatch;
    tarch::timing::Watch           _releaseJoinDataWatch;
    tarch::timing::Watch           _releaseBoundaryDataWatch;

    tarch::timing::Watch                _concurrencyReportWatch;
    tarch::multicore::BooleanSemaphore  _concurrencyReportSemaphore;

    int                                 _currentConcurrencyLevel;
    int                                 _currentPotentialConcurrencyLevel;

    double                              _maxConcurrencyLevel;
    double                              _maxPotentialConcurrencyLevel;

    double                              _timeAveragedConcurrencyLevel;
    double                              _timeAveragedPotentialConcurrencyLevel;

    int                                 _numberOfSpawnedBackgroundTask;

    /**
     * Is never piped out. It is used to find out whether to stream a snapshot
     * or not, i.e. to control the data dumps.
     */
    double                              _lastConcurrencyDataUpdateRealTimeStamp;
    double                              _lastConcurrencyDataWriteRealTimeStamp;
    double                              _lastConcurrencyDataWriteCPUTimeStamp;

    /**
     * Given in seconds.
     */
    static double MinTimeInBetweenTwoConcurrencyLogs;

    /**
     * Given in seconds.
     */
    static double TimeInBetweenTwoConcurrencyDataDumps;


  public:
    DefaultAnalyser();
    virtual ~DefaultAnalyser();

    virtual void beginIteration();

    virtual void endIteration(double numberOfInnerLeafCells, double numberOfOuterLeafCells, double numberOfInnerCells, double numberOfOuterCells, double numberOfLocalCells, double numberOfLocalVertices);

    virtual void enterCentralElementOfEnclosingSpacetree();
    virtual void leaveCentralElementOfEnclosingSpacetree();

    virtual void addWorker(
      int                                 workerRank,
      int                                 level
    );

    virtual void removeWorker(
      int                                 workerRank,
      int                                 level
    );

    /**
     * The worker receives are slightly different than all the other receives.
     * As a master may have multiple workers, this routine resets the timer no
     * matter whether it has been switched on/off before. In return, the
     * corresponding end does not switch off any timer. It just grabs the time
     * stamp and then immediately restarts the clock.
     */
    virtual void beginToReceiveDataFromWorker();
    virtual void endToReceiveDataFromWorker( int fromRank );
    virtual void beginToReceiveDataFromMaster();
    virtual void endToReceiveDataFromMaster();

    virtual void dataWasNotReceivedInBackground( int fromRank, int tag, int cardinality, int pageSize );

    /**
     * @see endToReleaseSynchronousHeapData().
     */
    virtual void beginToReleaseSynchronousHeapData();

    /**
     * Writes how long it took to release synchronous heap data. The
     * counterpart beginToReleaseSynchronousHeapData() again does not
     * write any information but starts a watch.
     */
    virtual void endToReleaseSynchronousHeapData();

    /**
     * Does not write anything but start an internal watch so we can keep track
     * how much asynchronous data is outstanding.
     */
    virtual void beginToPrepareAsynchronousHeapDataExchange();

    /**
     * Now we write that we've basically done all the asynchronous exchange and we
     * also give timings.
     */
    virtual void endToPrepareAsynchronousHeapDataExchange();

    virtual void beginReleaseOfJoinData();
    virtual void endReleaseOfJoinData();

    virtual void beginReleaseOfBoundaryData();
    virtual void endReleaseOfBoundaryData();

    virtual void changeConcurrencyLevel(int actualChange, int maxPossibleChange);
    virtual void minuteNumberOfBackgroundTasks(int taskCount);

    virtual void enable(bool value);
};


#endif
