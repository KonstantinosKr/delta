// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_TASK_SET_H_
#define _PEANO_DATA_TRAVERSAL_TASK_SET_H_


#ifdef SharedTBB
#include <tbb/task.h>
#endif


namespace peano {
  namespace datatraversal {
    class TaskSet;
  }
}


/**
 * Task Set
 *
 * This is my very simple wrapper of task parallelism in Peano. The class
 * provides only one type of constructor. This constructor is passed a set of
 * instances representing one functor each. The constructor executes all of
 * them in parallel (if this is the user's wish) and returns. Consequently,
 * this wrapper is the right choice iff all the task are totally independent
 * of each other.
 *
 * The standard variants do not copy the task you hand in and I expect the
 * functors to be handed in as references. If the tasks are not independent
 * of each other, they thus might induce data races.
 *
 * Please see the documentation of the constructor with only one argument for
 * further details, as this one behaves differently.
 *
 * @author Tobias Weinzierl
 */
class peano::datatraversal::TaskSet {
  private:

    #ifdef SharedTBB
    template <class Functor>
    class GenericTask: public tbb::task {
      private:
        /**
         * See the outer class description for an explanation why this is an
         * attribute, i.e. why we copy the functor here always.
         */
        Functor&   _functor;
      public:
        GenericTask(Functor& functor);
        tbb::task* execute();
    };


    template <class Functor>
    class GenericTaskWithCopy: public tbb::task {
      private:
        /**
         * See the outer class description for an explanation why this is an
         * attribute, i.e. why we copy the functor here always.
         */
        Functor   _functor;
      public:
        GenericTaskWithCopy(const Functor& functor);
        tbb::task* execute();
    };
    #endif

  public:
    /**
     * Spawn One Asynchronous Task
     *
     * Different to other tasks, I have to copy the functor here. Otherwise,
     * this operation might return, the calling code might destroy the functor,
     * and the asynchronous task then tries to invoke it. This would result in
     * a seg fault.
     *
     * As a consequence, you have to very carefully when the destructor is not
     * empty. Task objects might be copied multiple times and you never know
     * which destructor is the one belonging to the task object that is really
     * executed.
     *
     * <h2> TBB </h2>
     *
     * I do not use the spawn command of TBB here but the enqueue. In
     * particular on Windows systems, I often encounter starvation processes if
     * a load vertices or store vertices task splits very often and spawns too
     * many task children due to this operation.
     *
     * <h2> Deadlocks </h2>
     *
     * Please note that your code might deadlock if you spawn a task without
     * multicore support and if you hope/rely on the fact that this task cannot
     * complete right at the moment but will later on be able to do so.
     */
    template <class Functor>
    inline TaskSet(
      Functor&  task
    );

    /**
     * Execute two tasks in parallel
     *
     * Takes the two tasks and executes them in parallel if parallelise equals
     * 1. parallelise is kind of a veto mechanism. It is defined as integer
     * although it has the semantics of a boolean. This decision is due to the
     * idea to have all the operations using the autotuning's oracle use the
     * same (integer) semantics.
     *
     * This operation does not work for recursive tasks.
     *
     * <h2> Copying policy </h2>
     *
     * The TBB implementation wraps the two functions with GenericTask, i.e.
     * with an additional type holding a reference to the original object.
     * As these wrappers do hold a reference, the functor is not copied. If
     * your functor requires copying, you have to do this manually.
     *
     * @param parallelise             Either 0 or 1.
     */
    template <class Functor0, class Functor1>
    inline TaskSet(
      Functor0&  task0,
      Functor1&  task1,
      int        parallelise
    );

    /**
     * Execute three tasks in parallel
     *
     * See other constructor for a detailed description. Is the same thing but
     * handles three parallel tasks.
     *
     * This operation does not work for recursive tasks.
     *
     * <h2> Copying policy </h2>
     *
     * See constructor with three arguments.
     *
     * @param parallelise             Either 0 or 1.
     */
    template <class Functor0, class Functor1, class Functor2>
    inline TaskSet(
      Functor0&  task0,
      Functor1&  task1,
      Functor2&  task2,
      int        parallelise
    );
};


#include "peano/datatraversal/TaskSet.cpph"

#endif

