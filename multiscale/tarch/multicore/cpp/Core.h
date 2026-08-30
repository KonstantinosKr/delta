// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#if !defined( _TARCH_MULTICORE_CPP_CORE_H_ ) && defined(SharedCPP)
#define _TARCH_MULTICORE_CPP_CORE_H_


#include <list>


#include "tarch/logging/Log.h"



namespace tarch {
  namespace multicore {
    class Core;

    namespace internal {
      /**
       * Forward declaration
       */
      class JobConsumerController;
    }
  }
}


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
	static tarch::logging::Log _log;

	int    _numberOfThreads;
	bool   _pin;


    /**
     * Masking being available to process. This is basically a bitfield which
     * holds an entry for each core (hardware thread) the present application
     * is allowed to run on. If you run multiple MPI ranks for example, this
     * is a subset of the actual cores available on a node. The fild is
     * initialised in the constructor.
     */
    cpu_set_t*    _mask;


    Core();

    /**
     * We use these structs for the Core to communicate with the job consumers.
     * The list never shrinks.
     */
    std::list<internal::JobConsumerController*>  _jobConsumerControllers;

    void createOneJobConsumerPerThread();
    void shutdownRunningJobConsumers();
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

    void pinThreads(bool value);
};


#endif
