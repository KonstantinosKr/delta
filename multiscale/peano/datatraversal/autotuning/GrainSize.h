// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_AUTOTUNING_GRAIN_SIZE_H_
#define _PEANO_DATA_TRAVERSAL_AUTOTUNING_GRAIN_SIZE_H_


#include "tarch/timing/Watch.h"
#include "peano/datatraversal/autotuning/MethodTrace.h"


namespace peano {
  namespace datatraversal {
    namespace autotuning {
      class GrainSize;
      class OracleForOnePhase;
    }
  }
}


/**
 * The grain size object is the object a shared memory oracle returns for a
 * given section that could be parallelised. It encapsulates at first glance
 * only one integer, i.e. the grain size. However, it also ensures that, if the
 * oracle would like to get some feedback on the actual timing, such feedback
 * is provided once the object is destroyed (or explicitly closed). The class
 * design thus is very similar to TBB's lock.
 *
 * @author Tobias Weinzierl
 */
class peano::datatraversal::autotuning::GrainSize {
  private:
    const int              _grainSize;
    const bool             _useTimer;
    const int              _problemSize;
    const MethodTrace      _askingMethod;
    OracleForOnePhase*     _hostOracle;
    tarch::timing::Watch*  _watch;
  public:
    /**
     * Construct the answer object
     *
     * @param grainSize Grain size to be taken for a particular parallel
     *                  region. Can be 0 if no parallel features shall be used.
     * @param useTimer  If you set this flag, the GrainSize object
     *                  automatically will keep track how long a parallel
     *                  section ran. It will then feed this data back into the
     *                  oracle.
     * @param askingMethod To enable the answer object to feed back runtime data,
     *                  we have to know by which method the parallelisation
     *                  request has been made.
     * @param hostOracle This pointer is required to enable the object to actually
     *                  know who's to be informed about elapsed time. If you set
     *                  useTimer to false, this may be a nullptr.
     */
    GrainSize( int grainSize, bool useTimer, int problemSize, MethodTrace askingMethod, OracleForOnePhase* hostOracle );
    GrainSize( const GrainSize& rhs ) = delete;

    /**
     * Whenever we move stuff around, we have to ensure that only the target
     * instance remains responsible for the watch and informs the oracle. We
     * do this by copying the watch pointer and removing the link from the old
     * (source) object to this watch afterwards.
     */
    GrainSize( GrainSize&& rhs );

    /**
     * If parallelSectionHasTerminated() hasn't been called manually, we do so
     * here.
     */
    ~GrainSize();
    int getGrainSize() const;
    bool runsParallel() const;
    void parallelSectionHasTerminated();
};


#endif
