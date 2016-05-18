// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PERFORMANCE_ANALYIS_ANALYSIS_H_
#define _PEANO_PERFORMANCE_ANALYIS_ANALYSIS_H_


#include "peano/performanceanalysis/Analyser.h"


namespace peano {
  namespace performanceanalysis {
    class Analysis;
  }
}



#ifdef PerformanceAnalysis

/**
 * Analyser Singleton
 *
 * This type/component results from the Peano project however is of more
 * general nature. It yields performance analysis and load balancing data with
 * respect to mpi. By default, the tarch forwards such information to the
 * logging facilities. However, you can also redirect them to your own types
 * to connect to postprocessing tools, e.g.
 *
 * Analysis is a plain singleton that holds an instance of Analyser. While
 * there is a default Analyser forwarding data to the info log level, you can
 * switch the device yourself.
 *
 * @author Roland Wittmann, Tobias Weinzierl
 */
class peano::performanceanalysis::Analysis: public peano::performanceanalysis::Analyser {
  private:
    Analysis();

    Analyser*  _device;

    void deleteDevice();
  public:
    ~Analysis();

    static Analysis& getInstance();

    /**
     * Set new device. The Analysis object becomes responsible for the pointer,
     * i.e. will delete it automatically at shutdown (or if you set another
     * device again).
     */
    void setDevice( Analyser* device );


    /**
     * =========================================================================
     * Everything below is inherited and forwards the call to the current device.
     * =========================================================================
     */


    /**
     * @see Analyser
     */
    virtual void beginIteration();

    /**
     * @see beginIteration()
     */
    virtual void endIteration(double numberOfInnerLeafCells, double numberOfOuterLeafCells, double numberOfInnerCells, double numberOfOuterCells, double numberOfLocalCells, double numberOfLocalVertices);

    virtual void enterCentralElementOfEnclosingSpacetree();
    virtual void leaveCentralElementOfEnclosingSpacetree();

    /**
     * @see Analyser
     */
    virtual void addWorker(
      int                                 workerRank,
      int                                 level
    );

    /**
     * @see Analyser
     */
    virtual void removeWorker(
      int                                 workerRank,
      int                                 level
    );

    /**
     * @see Analyser
     */
    virtual void dataWasNotReceivedInBackground( int fromRank, int tag, int cardinality, int pageSize );

    /**
     * @see Analyser
     */
    virtual void beginToReceiveDataFromWorker();
    virtual void endToReceiveDataFromWorker( int fromRank );

    virtual void beginToReleaseSynchronousHeapData();
    virtual void endToReleaseSynchronousHeapData();
    virtual void beginToPrepareAsynchronousHeapDataExchange();
    virtual void endToPrepareAsynchronousHeapDataExchange();
    virtual void endReleaseOfJoinData();
    virtual void endReleaseOfBoundaryData();

    virtual void changeConcurrencyLevel(int actualChange, int maxPossibleChange);
    virtual void fireAndForgetBackgroundTask(int taskCount);
};


#else

class peano::performanceanalysis::Analysis {
  public:
    ~Analysis() {}

    static Analysis& getInstance() {
      static Analysis singleton;
      return singleton;
    };

    /**
     * Set new device. The Analysis object becomes responsible for the pointer,
     * i.e. will delete it automatically at shutdown (or if you set another
     * device again).
     */
    void setDevice( Analyser* device ) {}


    /**
     * =========================================================================
     * Everything below is inherited and forwards the call to the current device.
     * =========================================================================
     */


    /**
     * @see Analyser
     */
    void beginIteration() {}

    /**
     * @see beginIteration()
     */
    void endIteration(double numberOfInnerLeafCells, double numberOfOuterLeafCells, double numberOfInnerCells, double numberOfOuterCells, double numberOfLocalCells, double numberOfLocalVertices) {}

    void enterCentralElementOfEnclosingSpacetree() {}
    void leaveCentralElementOfEnclosingSpacetree() {}

    /**
     * @see Analyser
     */
    void addWorker(
      int                                 workerRank,
      int                                 level
    ) {}

    /**
     * @see Analyser
     */
    void removeWorker(
      int                                 workerRank,
      int                                 level
    ) {}

    /**
     * @see Analyser
     */
    void dataWasNotReceivedInBackground( int fromRank, int tag, int cardinality, int pageSize ) {}

    /**
     * @see Analyser
     */
    void beginToReceiveDataFromWorker() {}
    void endToReceiveDataFromWorker( int fromRank ) {}

    void beginToReleaseSynchronousHeapData() {}
    void endToReleaseSynchronousHeapData() {}
    void beginToPrepareAsynchronousHeapDataExchange() {}
    void endToPrepareAsynchronousHeapDataExchange() {}
    void endReleaseOfJoinData() {}
    void endReleaseOfBoundaryData()  {}

    virtual void changeConcurrencyLevel(int actualChange, int maxPossibleChange) {}
    virtual void fireAndForgetBackgroundTask(int taskCount) {}
};

#endif


#endif
