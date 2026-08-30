#if !defined(_TARCH_MULTICORE_PINNING_OBSERVER_) && defined(SharedTBB)
#define _TARCH_MULTICORE_PINNING_OBSERVER_


//#include <sched.h>
#include <stdlib.h>
#include <unistd.h>


#include <tbb/task_arena.h>
#include <tbb/task_scheduler_observer.h>
#include <tbb/atomic.h>

#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include <tbb/tick_count.h>

#include "tarch/logging/Log.h"

namespace tarch {
  namespace multicore {
    class PinningObserver;
  }
}


/**
 * This implementations follows very closely
 *
 * https://software.intel.com/en-us/blogs/2013/10/31/applying-intel-threading-building-blocks-observers-for-thread-affinity-on-intel
 *
 * @author Leonhard Rannabauer
 * @author Tobias Weinzierl
 */
class tarch::multicore::PinningObserver: public tbb::task_scheduler_observer {
  private:
    static tarch::logging::Log  _log;

    const int        _pinningStep;

    /**
     * Masking being available to process. This is basically a bitfield which
     * holds an entry for each core (hardware thread) the present application
     * is allowed to run on. If you run multiple MPI ranks for example, this
     * is a subset of the actual cores available on a node. The fild is
     * initialised in the constructor.
     */
    //cpu_set_t*    _mask;
    //AffinityMask*    _mask;

    int              _ncpus;

    tbb::atomic<int> _threadIndex;

    /**
     * How many threads have been registered through callback
     */
    tbb::atomic<int> _numThreads;

    /**
     * If the observer is switched on, it automatically pins all threads.
     */
    void pinCurrentThread();

  public:
    /**
     * @todo ncpus should be replaced by the concurrency operation from C++14
     *
     * @see _mask
     */
    PinningObserver( int pinningStep=1 );
    virtual ~PinningObserver();


    void on_scheduler_entry( bool ) override;
    void on_scheduler_exit( bool ) override;

    int getNumberOfRegisteredThreads();
};

#endif

