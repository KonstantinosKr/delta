// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#if !defined(_TARCH_MULTICORE_TBB_CORE_H_) && defined(SharedTBB)
#define _TARCH_MULTICORE_TBB_CORE_H_


// This seems to be an Intel requirement as this feature isnt' released yet officially.
#define TBB_PREVIEW_GLOBAL_CONTROL 1
#include <tbb/global_control.h>

#include "tarch/logging/Log.h"
#include "tarch/multicore/MulticoreDefinitions.h"


#include "tarch/multicore/tbb/PinningObserver.h"



namespace tarch {
  namespace multicore {
    class Core;
    class BackgroundTask;
    class BooleanSemaphore;
  }
}


/**
 * TBB Core
 *
 * Abstraction of the TBB routines. This class is a singleton.
 *
 * @author Tobias Weinzierl
 */
class tarch::multicore::Core {
  private:
    Core();

    static tarch::logging::Log  _log;

    int                         _numberOfThreads;

    ::tbb::global_control*      _globalThreadCountControl;
    ::tbb::global_control*      _globalStackSizeControl;

    PinningObserver             _pinningObserver;
  public:
    /**
     * Tells the constructor to use the default. Does not mean that 0 is used
     * as core count, as that would not make any sense. Usually implies that
     * all cores plus hyperthreading are used.
     */
    static constexpr int UseDefaultNumberOfThreads = 0;
    /**
     * Tells the constructor to use the default. Does not mean that 0 is used
     * as stack size, as that would not make any sense.
     */
    static constexpr int UseDefaultStackSize       = 0;

    /**
     * Destructor
     */
    ~Core();

    /**
     * @return Singleton instance
     */
    static Core& getInstance();

    /**
     * Configure the whole thing. If numberOfThreads equals 0, the core is
     * using the number of standard threads.
     *
     * @param numberOfThreads Number of threads that shall be used. This
     *        parameter either is greater than zero (which defines the number
     *        of threads) or it equals DefaultNumberOfThreads which means that the code should
     *        use the default number of threads.
     * @param stackSize Use UseDefaultStackSize if you are fine with TBB's
     *        default settings (which should work out in most of the cases).
     *        Otherwise, consult https://software.intel.com/en-us/node/589744
     *        for meaningful values.
     *
     * @see UseDefaultStackSize
     * @see UseDefaultNumberOfThreads
     */
    void configure( int numberOfThreads, int stackSize );

    /**
     * Shutdown parallel environment.
     */
    void shutDown();

    /**
     * This routine basically always return true, as the shared memory stuff
     * always works properly even without a particular configuration.
     *
     * @return Shared memory environment is up and runnning.
     */
    bool isInitialised() const;

    /**
     * Returns the number of threads that is used by TBB. This routine usually
     * is not of interest at all as TBB should do all the thread management
     * automatically. However, the user interface plots some information on the
     * number of threads used, and sometimes I found it useful.
     *
     * @return Number of threads available.
     */
    int getNumberOfThreads() const;

    void pinThreads(bool value);
};


#endif
