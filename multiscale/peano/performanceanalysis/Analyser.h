// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PERFORMANCE_ANALYIS_ANALYSER_H_
#define _PEANO_PERFORMANCE_ANALYIS_ANALYSER_H_


#include "tarch/la/Vector.h"


namespace peano {
  namespace performanceanalysis {
    class Analyser;
  }
}



/**
 * Analyser Interface
 *
 * @author Roland Wittmann, Tobias Weinzierl
 */
class peano::performanceanalysis::Analyser {
  public:
    virtual ~Analyser() {};

    /**
     * Indicates start of a new global iteration run
     *
     *
     * The grid informs the analyser when it start to traverse. At the end of
     * the traversal, it invokes endIteration(). The time in-between
     * endIteration() and beginIteration() consequently is idle time.
     */
    virtual void beginIteration() = 0;

    /**
     * Counterpart of beginIteration()
     */
    virtual void endIteration(double numberOfInnerLeafCells, double numberOfOuterLeafCells, double numberOfInnerCells, double numberOfOuterCells, double numberOfLocalCells, double numberOfLocalVertices) = 0;

    /**
     * Only a subpart of the total traversal time is spent in the central
     * element. This is tracked via this operation.
     */
    virtual void enterCentralElementOfEnclosingSpacetree() = 0;

    /**
     * Counterpart of enterCentralElementOfEnclosingSpacetree()
     */
    virtual void leaveCentralElementOfEnclosingSpacetree() = 0;

    /**
     * Inform analysis device about a worker and its associated domain
     * information. This information is 3d though it might be a 2d
     * simulation only. In this case, the third entry should equals 0
     * due to the overloaded function below.
     */
    virtual void addWorker(
      int                                 workerRank,
      int                                 level
    ) = 0;

    virtual void removeWorker(
      int                                 workerRank,
      int                                 level
    ) = 0;

    /**
     * Inform analyser that there was data that should have been received in
     * background but that wasn't there, when we needed it.
     *
     * Peano tries to receive all data from neighbours, i.e. boundary data, in
     * the background. One idea is that once the traversal is done, already
     * all/most of the records for the subsequent traversal are already
     * available. If this is not the case, the buffers at least send a
     * notification to the analysis.
     *
     * @param fromRank     From which rank was data expected.
     * @param tag          On which tag was the data expected.
     * @param cardinality  How much data (i.e. how many integeres, e.g.) were expected.
     * @param pageSize     In which batches should this data arrive.
     */
    virtual void dataWasNotReceivedInBackground( int fromRank, int tag, int cardinality, int pageSize ) = 0;


    virtual void beginToReceiveDataFromWorker() = 0;
    virtual void endToReceiveDataFromWorker( int fromRank ) = 0;
    virtual void beginToReceiveDataFromMaster() = 0;
    virtual void endToReceiveDataFromMaster() = 0;
    virtual void beginToReleaseSynchronousHeapData() = 0;
    virtual void endToReleaseSynchronousHeapData() = 0;
    virtual void beginToPrepareAsynchronousHeapDataExchange() = 0;
    virtual void endToPrepareAsynchronousHeapDataExchange() = 0;
    virtual void beginReleaseOfJoinData() = 0;
    virtual void endReleaseOfJoinData() = 0;
    virtual void beginReleaseOfBoundaryData() = 0;
    virtual void endReleaseOfBoundaryData() = 0;

    /**
     * Inform the analysis that code has entered or left a parallel section.
     *
     * We have to reduce the parameters by one before we add them to the
     * internal values as we assume that a parallel region with actualChange
     * parallel threads is entered by a single thread, i.e. one thread of the
     * newly entered parallel section has already been there.
     *
     * @param actualChange Concurrency level of the just entered parallel
     *          section or, alternatively, a negative value if it has left a
     *          parallel region.
     */
    virtual void changeConcurrencyLevel(int actualChange, int maxPossibleChange) = 0;

    virtual void minuteNumberOfBackgroundTasks(int taskCount) = 0;

    /**
     * Switch on/off.
     */
    virtual void enable(bool value = true) = 0;
};


#endif
