// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_TASK_SET_H_
#define _PEANO_DATA_TRAVERSAL_TASK_SET_H_

#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/logging/Log.h"
#include "tarch/multicore/Jobs.h"


#include "peano/performanceanalysis/Analysis.h"


#include <functional>


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
 * <h2> Issuing background tasks </h2>
 *
 * If you spawn a task, I cannot know when it is actually executed. As a
 * result, I never can rely where data is at the very moment the task then is
 * actually triggered. Notably if you directly try to access vertices/cells,
 * they might already have been moved to a different memory location. So what
 * you have to do is that you store the data you wanna manipulate on heaps as
 * data on heaps doesn't move. And then you equip your tasks with pointers
 * (indices) to the heap. These pointers/indices are copied but the heap index
 * itself remains valid, so it doesn't really matter to you if the vertex
 * holding pointers too moves around.
 *
 * If you want to get data back from a background tasks, you have to take into
 * account that the task you hand over to the TaskSet constructor is not directly executed
 *   if you declare it as background task.
 *   Instead, the runtime system makes a copy of your functor/task, stores it
 *   safely away and executes this one. That means once you hand over a
 *   functor, you can safely destroy this object. The tasking system has
 *   created a copy of its own.
 *
 * Please note that you notably should have a boolean flag somewhere that
 * indicates whether your task has terminated. I often store entries with
 * an enum on a heap. The enum encodes the states BackGroundTaskSpawned and
 * BackGroundTaskCompleted. I do protect the access to these enums with one
 * global semaphore to ensure that no two threads read/write it
 * simultaneously. If a task is spawned, the corresponding value on the heap is
 * set to BackGroundTaskSpawned. The operator() of the task sets the value to
 * BackGroundTaskCompleted as very last action. In a mapping where I need the
 * task result, I add a while loop that looks whether the task has already
 * terminated:
 *
 * <pre>
bool taskHasTerminated = false;
while (!taskHasTerminated) {
  tarch::multicore::Lock myLock(_globalBackgroundTaskStatusSemphore);
  taskHasTerminated = MyStatusHeap.getIndex(...).getStatus()==BackGroundTaskCompleted;
  myLock.free();
}
</pre>
 *
 * Alternatively, you can ask the tarch::multicore::jobs component whether all
 * background tasks have terminated.
 *
 * <h2> Number of background tasks </h2>
 *
 * I did occasionally run into situations where too many background tasks made the
 * overall system starve. So you can restrict the number of background tasks now
 * manually. The system queues all tasks and then checks whether a background task
 * is active already. If not, it launches a consumer task. See tarch::multicore::jobs.
 *
 * <h2> Difference of tasks and jobs</h2>
 *
 * In Peano, I prefer to speak of jobs rather than tasks. Tasks are atomic, i.e.
 * once they kick off, there are no further dependencies that they require to
 * terminate. Jobs in turn might internally wait for further input data. As a
 * consequence, jobs easily deadlock. Tasks are a special case of jobs.
 *
 * The name TaskSet consequently is not perfect. It should be JobSet. Anyway, you
 * can always tell Peano whether a job you pass is actually a task. If it is a
 * task, Peano can optimise the execution pattern.
 *
 * @author Tobias Weinzierl
 */
class peano::datatraversal::TaskSet {
  public:
    enum class TaskType {
      /**
       * Job does not depend on any other job or input or IO at all. Peano
       * runs it as soon as possible. If you use this arguments for TaskSet
       * with a single task, it does not really make sense: The whole thing
       * become a direct function call. Otherwise, eapch call is deployed to
       * its own task.
       */
      IsTaskAndRunImmediately,
	  /**
	   * Not different to IsTaskAndRunImmediately if used in a construct with 
       * multiple functors. If you use only the single functor, it usually 
       * behaves the same as IsTaskAndRunImmediately, but technically it 
       * launches a new task, i.e. something else might squeeze in-between.
	   */
	  IsTaskAndRunAsSoonAsPossible,
      /**
       * A classic background task that is processed any time Peano thinks it
       * to be appropriate. You can set an upper bound on the number of 
       * background tasks that run concurrently. See tarch::multicore::jobs.
       *
       * Please note that job passed has to be a task. See class documentation
       * on the difference between jobs and tasks.
       */
  	  Background,
  	  BackgroundMPIReceiveTask,
      /**
       * Used by Peano's grid management.
       */
	  LoadCells,
      /**
       * Used by Peano's grid management.
       */
	  LoadVertices,
      /**
       * Used by Peano's grid management.
       */
	  TriggerEvents,
      /**
       * Used by Peano's grid management.
       */
	  StoreCells,
      /**
       * Used by Peano's grid management.
       */
	  StoreVertices
    };
  private:
    static tarch::logging::Log  _log;

    static int                               translateIntoJobClass( TaskType type );
    static tarch::multicore::jobs::JobType   translateIntoJobType( TaskType type );

  public:
    /**
     * Spawn One Task
     *
     * If you spawn one task, you have to clarify which type of task you spawn.
     * This is controlled through TaskType. See the documentation there.
     * Actually, it rarely makes sense to pass a job through this constructor
     * that is not some type of a background task, i.e. can run at any time
     * later without any further dependencies.
     *
     * <h2> Spawn task through a dedicated task class</h2>
     *
     * If you go down this route, then you write your own little class that
     * represents your job. The class has the following properties:
     *
     * - It has attributes that basically store all the data you require for
     *   this job to complete. If you job needs some parameters, the class
     *   needs local attributes. If your job has to write some global output
     *   data, your class needs a reference or pointer to these data.
     * - The class needs a constructor that initialises all class attributes.
     *   Please note that background tasks could be run at any time throughout
     *   the application execution. Hence, a job class may not hold references
     *   or pointers to local variables of the calling function.
     * - The class needs a functor that does the actual work.
     *
     * Here's an example of a  job class from the ExaHyPE project:
     * <pre>

  class PredictionTask {
  private:
    ADERDGSolver&    _solver;
    CellDescription& _cellDescription;
  public:
    PredictionTask(
        ADERDGSolver&     solver,
        CellDescription&  cellDescription);

    void operator()();
  };

       </pre>
     *
     * This task then is used as follows:
     * <pre>

      PredictionTask predictionTask( myPointer, cellDescription );
      peano::datatraversal::TaskSet spawnedSet( predictionTask, peano::datatraversal::TaskSet::TaskType::Background  );

       </pre>
     *
     *
     *
     * <h2> Lambda calculus </h2>
     *
     * In principle, you can simply pass in a lambda expression instead of a
     * defined functor:
     *
     * <pre>
        peano::datatraversal::TaskSet backgroundTask(
         [=] () {
          // do something
         },
         peano::datatraversal::TaskSet::TaskType::Background
       );
       </pre>
     *
     * It is important that myFunctor catches everything via copy. As a
     * consequence do something only includes static and const accessors.
     *
     * Note: This interface is only used for tasks that do not have to run
     * persistently (in the background) for a very long time. It is only
     * to be used for tasks that are reasonably short. So you can use it if
     * your routine checks whether the tasks are all complete before it
     * terminates.
     */
    TaskSet(
      std::function<bool()>&&  task,
      TaskType                 taskType
    );

    /**
     * Alternative to other TaskSet constructor. Ownership goes to TaskSet
     * class, i.e. you don't have to delete it.
     */
    template <typename T>
    TaskSet(
      T*                      myTask,
      TaskType                taskType
    ) {
      switch (taskType) {
        case peano::datatraversal::TaskSet::TaskType::IsTaskAndRunImmediately:
        case peano::datatraversal::TaskSet::TaskType::IsTaskAndRunAsSoonAsPossible:
        case peano::datatraversal::TaskSet::TaskType::LoadCells:
        case peano::datatraversal::TaskSet::TaskType::LoadVertices:
        case peano::datatraversal::TaskSet::TaskType::TriggerEvents:
        case peano::datatraversal::TaskSet::TaskType::StoreCells:
        case peano::datatraversal::TaskSet::TaskType::StoreVertices:
          tarch::multicore::jobs::spawn( new tarch::multicore::jobs::GenericJobWithPointer<T>(myTask,translateIntoJobType(taskType),translateIntoJobClass(taskType) ) );
          break;
        case peano::datatraversal::TaskSet::TaskType::BackgroundMPIReceiveTask:
        case peano::datatraversal::TaskSet::TaskType::Background:
       	  peano::performanceanalysis::Analysis::getInstance().minuteNumberOfBackgroundTasks(
       	    tarch::multicore::jobs::getNumberOfWaitingBackgroundJobs()
       	  );
          tarch::multicore::jobs::spawnBackgroundJob( new tarch::multicore::jobs::GenericJobWithPointer<T>(myTask,translateIntoJobType(taskType),translateIntoJobClass(taskType)) );
          break;
      }
    }


    /**
     * Invoke operations in parallel. Works fine with lambda
     * expressions:
     *
  peano::datatraversal::TaskSet runParallelTasks(
    [&]() -> void {

    ...
    },
    [&]() -> void {

    ...
    },
    true
  );
     *
     * Please do not invoke any background threads through this operation.
     */
    TaskSet(
      std::function<bool ()>&&  function1,
      std::function<bool ()>&&  function2,
	  TaskType                  taskType1,
	  TaskType                  taskType2,
      bool                      parallelise
    );

    TaskSet(
      std::function<bool ()>&& function1,
      std::function<bool ()>&& function2,
      std::function<bool ()>&& function3,
	  TaskType                 taskType1,
	  TaskType                 taskType2,
	  TaskType                 taskType3,
      bool                     parallelise
    );

    TaskSet(
      std::function<bool ()>&& function1,
      std::function<bool ()>&& function2,
      std::function<bool ()>&& function3,
      std::function<bool ()>&& function4,
	  TaskType                 taskType1,
	  TaskType                 taskType2,
	  TaskType                 taskType3,
	  TaskType                 taskType4,
      bool                     parallelise
    );

    TaskSet(
      std::function<bool ()>&& function1,
      std::function<bool ()>&& function2,
      std::function<bool ()>&& function3,
      std::function<bool ()>&& function4,
      std::function<bool ()>&& function5,
	  TaskType                 taskType1,
	  TaskType                 taskType2,
	  TaskType                 taskType3,
	  TaskType                 taskType4,
	  TaskType                 taskType5,
      bool                     parallelise
    );

    /**
     * <h2> Use waitFor within busy waiting loop </h2>
     *
     * waitForXXX is often used within while loops actually implementing busy
     * waiting. In principle, that's what the waits have been designed for.
     * However, there's one pitfall: If a job from class K splits up into many
     * other jobs of class K, none of these jobs may wait for the other class
     * K jobs to complete. In general, no job shall ever wait for jobs of the
     * same class.
     *
     * The reason is that we allow Peano's tasking systems to grab multiple
     * jobs of one class in one rush and process them en block. If jobs wait
     * for their own class, it might happen that one hardware thread graps
     * 10 jobs of class K, removes them from the job queue, starts to run the
     * first job, and then deadlocks as the first one is waiting for the
     * other nine guys.
     */
    static void waitForLoadCellsTask();
	static void waitForLoadVerticesTask();
	static void waitForEventTask();
	static void waitForStoreCellsTask();
	static void waitForStoreVerticesTask();

	/**
	 * There is such a routine in Peano's tarch as well. This essentially is a
	 * wrapper which however also keeps track of the statistics.
	 */
	static void startToProcessBackgroundJobs();
	static bool finishToProcessBackgroundJobs();
};




#endif

