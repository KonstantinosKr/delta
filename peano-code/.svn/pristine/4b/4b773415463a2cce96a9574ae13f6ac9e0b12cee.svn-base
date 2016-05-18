// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#if !defined( _TARCH_MULTICORE_OMP_CORE_H_) && defined(SharedOMP)
#define _TARCH_MULTICORE_OMP_CORE_H_



namespace tarch {
  namespace multicore {
    class Core;
  }
}


/**
 * OpenMP Core
 *
 * Abstraction of the OpenMP system. This class is a singleton.
 *
 * @author Kristof Unterweger
 */
class tarch::multicore::Core {
  private:
    Core();

    int _numberOfThreads;
  public:
    /**
     * Destructor
     */
    ~Core();

    /**
     * @return Singleton instance
     */
    static Core& getInstance();

    /**
     * Configures the whole thing. If numberOfThreads equals 0, the core is
     * using the number of standard threads.
     *
     * @param numberOfThreads Numer of threads that shall be used. This
     *        parameter either is greater than zero (which defines the number
     *        of threads) or it equals zero which means that the code should
     *        use the default number of threads. The latter value fits to the
     *        semantics of
     *        tarch::multicore::configurations::CoreConfiguration::getNumberOfThreads()
     */
    void configure( int numberOfThreads );

    /**
     * Shutdown parallel environment.
     */
    void shutDown();

    /**
     * Returns the number of threads that is used by OpenMP. This routine usually
     * is not of interest at all as OpenMP should do all the thread management
     * automatically. However, the user interface plots some information on the
     * number of threads used, and sometimes I found it useful.
     *
     * @return Number of threads available.
     */
    int getNumberOfThreads() const;

    /**
     * @return Shared memory environment is up and runnning.
     */
    bool isInitialised() const;
};


#endif
