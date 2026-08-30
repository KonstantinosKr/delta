// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_MULTICORE_CORE_H_
#define _TARCH_MULTICORE_CORE_H_


#include <thread>


namespace tarch {
  namespace multicore {
    class Core;

    /**
     * Gives you the CPU number on which your current execution is running.
     * This is a wrapper around a UNIX operation and thus might not be
     * available on all machines - notably not on MAC.
     */
    int getCPUNumber();
    std::thread::id getThreadNumber();
  }
}


#if defined(SharedTBB)
#include "tarch/multicore/tbb/Core.h"
#elif SharedOMP
#include "tarch/multicore/omp/Core.h"
#elif SharedCPP
#include "tarch/multicore/cpp/Core.h"
#else


/**
 * Core
 *
 * Any shared memory implementation has to provide a singleton Core. Its full
 * qualified name is tarch::multicore::Core. If no shared memory variant is
 * switched on, this is the default Core implementation that does nothing.
 *
 * @author Tobias Weinzierl
 */
class tarch::multicore::Core {
  private:
    Core();
  public:
    static constexpr int UseDefaultNumberOfThreads = 0;

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
     *
     * @param numberOfThreads Numer of threads that shall be used. This
     *        parameter either is greater than zero (which defines the number
     *        of threads) or it equals DefaultNumberOfThreads which means that the code should
     *        use the default number of threads.
     */
    void configure( int numberOfThreads );

    /**
     * Shutdown parallel environment.
     */
    void shutDown();

    /**
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
};


#endif
#endif
