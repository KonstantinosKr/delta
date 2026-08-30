#include "../Jobs.h"
#include "tarch/multicore/Core.h"

#if defined(SharedTBB)

#include "tarch/logging/Log.h"
#include "tarch/Assertions.h"
#include "tarch/multicore/tbb/Jobs.h"


#include <vector>
#include <limits>





tarch::logging::Log tarch::multicore::jobs::internal::_log( "tarch::multicore::jobs::internal" );


tbb::atomic<int>                              tarch::multicore::jobs::internal::_numberOfRunningBackgroundJobConsumerTasks(0);
tarch::multicore::jobs::internal::JobQueue    tarch::multicore::jobs::internal::_pendingJobs[NumberOfJobQueues];

//
// This is a bug in Intel's TBB as documented on
//
// https://software.intel.com/en-us/forums/intel-threading-building-blocks/topic/700057
//
// These task groups have to be static inside a cpp file.
//
static tbb::task_group_context  backgroundTaskContext(tbb::task_group_context::isolated);
static tbb::task_group_context  importantTaskContext(tbb::task_group_context::isolated);


tarch::multicore::jobs::internal::BackgroundJobConsumerTask::BackgroundJobConsumerTask(int maxJobs):
  _maxJobs(maxJobs) {
}


tarch::multicore::jobs::internal::BackgroundJobConsumerTask::BackgroundJobConsumerTask(const BackgroundJobConsumerTask& copy):
  _maxJobs(copy._maxJobs) {
}


void tarch::multicore::jobs::internal::BackgroundJobConsumerTask::enqueue() {
  _numberOfRunningBackgroundJobConsumerTasks.fetch_and_add(1);
  const int jobsPerThread = static_cast<int>(internal::getJobQueue( internal::BackgroundJobsJobClassNumber ).unsafe_size())/tarch::multicore::Core::getInstance().getNumberOfThreads();
  BackgroundJobConsumerTask* tbbTask = new (tbb::task::allocate_root(::backgroundTaskContext)) BackgroundJobConsumerTask( std::max(
	TBBMinimalNumberOfJobsPerBackgroundConsumerRun,
	std::max( jobsPerThread, TBBMaximalNumberOfJobsPerBackgroundConsumerRun )
  ));
  tbb::task::enqueue(*tbbTask);
  ::backgroundTaskContext.set_priority(tbb::priority_low);
  logDebug( "enqueue()", "spawned new background consumer task" );
}


tbb::task* tarch::multicore::jobs::internal::BackgroundJobConsumerTask::execute() {
  const int oldNumberOfBackgroundJobs = internal::getJobQueue( internal::BackgroundJobsJobClassNumber ).unsafe_size();

  processJobs(internal::BackgroundJobsJobClassNumber,_maxJobs);

  const int newNumberOfBackgroundJobs = internal::getJobQueue( internal::BackgroundJobsJobClassNumber ).unsafe_size();

  _numberOfRunningBackgroundJobConsumerTasks.fetch_and_add(-1);

  if (
	(newNumberOfBackgroundJobs!=oldNumberOfBackgroundJobs and newNumberOfBackgroundJobs>0)
	or
	(_numberOfRunningBackgroundJobConsumerTasks.load()==0)
  ) {
    enqueue();
  }
  return nullptr;
}


tarch::multicore::jobs::internal::JobQueue& tarch::multicore::jobs::internal::getJobQueue( int jobClass ) {
  assertion( jobClass>=0 );

  return _pendingJobs[jobClass % NumberOfJobQueues];
}


void tarch::multicore::jobs::internal::spawnBlockingJob(
  std::function<bool()>&  job,
  JobType                 jobType,
  int                     jobClass,
  tbb::atomic<int>&       semaphore
) {
  if ( jobType!=JobType::Job ) {
    job();
    semaphore.fetch_and_add(-1);
  }
  else {
    internal::getJobQueue(jobClass).push(
      new JobWithoutCopyOfFunctorAndSemaphore(job, jobType, jobClass, semaphore )
    );

    logDebug( "spawnBlockingJob(...)", "enqueued job. tasks in this queue of class " << jobClass << "=" << internal::getJobQueue(jobClass).jobs.unsafe_size() );
  }
}



void tarch::multicore::jobs::terminateAllPendingBackgroundConsumerJobs() {
  backgroundTaskContext.cancel_group_execution();
}


void tarch::multicore::jobs::spawnBackgroundJob(Job* job) {
  switch (job->getJobType()) {
    case JobType::ProcessImmediately:
      while (job->run()) {};
      delete job;
      break;
    case JobType::RunTaskAsSoonAsPossible:
      {
        internal::TBBJobWrapper* tbbTask = new(tbb::task::allocate_root(importantTaskContext)) internal::TBBJobWrapper(job);
        // we may not use spawn as spawn relies on the current context and thus kills us if this context is already freed
        //        tbb::task::spawn(*tbbTask);
        tbb::task::enqueue(*tbbTask);
        importantTaskContext.set_priority(tbb::priority_high);
      }
      break;
    case JobType::MPIReceiveTask:
    case JobType::Task:
    case JobType::Job:
      {
        internal::getJobQueue( internal::BackgroundJobsJobClassNumber ).push(job);
        
        const int currentlyRunningBackgroundThreads = internal::_numberOfRunningBackgroundJobConsumerTasks.load();
        if (
          currentlyRunningBackgroundThreads<Job::_maxNumberOfRunningBackgroundThreads
        ) {
          internal::BackgroundJobConsumerTask::enqueue();
        }
      }
      break;
  }
}



int tarch::multicore::jobs::getNumberOfWaitingBackgroundJobs() {
  return internal::getJobQueue( internal::BackgroundJobsJobClassNumber ).unsafe_size() + internal::_numberOfRunningBackgroundJobConsumerTasks.load();
}


/**
 * Spawn means a thread fires a new job and wants to continue itself.
 *
 * <h2> The spawned job is a task </h2>
 *
 * That means that the new job has no dependencies on any other job. It is
 * thus convenient to launch a real TBB task for it.
 *
 * <h2> The spawned job is not a task </h2>
 *
 * We enqueue it. We may not immediately spawn a job consumer task, as this
 * might mean that TBB might immediately start to consume the job and halt the
 * current thread. This is not what we want: We want to continue with the
 * calling thread immediately.
 */
void tarch::multicore::jobs::spawn(Job*  job) {
  if ( job->isTask() ) {
    internal::TBBJobWrapper* tbbTask = new(tbb::task::allocate_root(importantTaskContext)) internal::TBBJobWrapper(job);
    tbb::task::enqueue(*tbbTask);
  }
  else {
    internal::getJobQueue(job->getClass()).push(job);

    logDebug( "spawn(Job*)", "enqueued job of class " << job->getClass() );
  }
}


void tarch::multicore::jobs::spawn(std::function<bool()>& job, JobType jobType, int jobClass) {
  spawn( new tarch::multicore::jobs::GenericJobWithCopyOfFunctor(job,jobType,jobClass) );
}


bool tarch::multicore::jobs::processJobs(int jobClass, int maxNumberOfJobs) {
  logDebug( "processJobs()", "search for jobs of class " << jobClass );

  // Ensure that we do not redo all re-enqueued jobs within the same while loop
  maxNumberOfJobs = std::min(
    maxNumberOfJobs,
	static_cast<int>( internal::getJobQueue(jobClass).unsafe_size() )
  );

  Job* myTask   = nullptr;
  bool gotOne   = internal::getJobQueue(jobClass).try_pop(myTask);
  bool result   = false;
  while (gotOne) {
    logDebug( "processJob(int)", "start to process job of class " << jobClass );
    bool reschedule = myTask->run();
    if (reschedule) {
      internal::getJobQueue(jobClass).push(myTask);
      maxNumberOfJobs--;
    }
    else {
      delete myTask;
      maxNumberOfJobs--;
    }
    result   = true;
    logDebug(
      "processJob(int)", "job of class " << jobClass << " complete, there are still " <<
	  internal::getJobQueue(jobClass).jobs.unsafe_size() <<
	  " jobs of this class pending"
	);
    if ( maxNumberOfJobs>0 ) {
      gotOne = internal::getJobQueue(jobClass).try_pop(myTask);
    }
    else {
      gotOne = false;
    }
  }

  return result;
}


/**
 * This routine is typically invoked by user codes to ensure that all
 * background jobs have finished before the user code continues. It is not
 * modelling what happens throughout the computation.
 *
 * As some background jobs might reschedule themselves
 * again - see their operator() - we quickly check how many jobs are
 * still pending. The code is given an upper bound how many consumer tasks to
 * use, to we issue the difference. But obviously if and only if there are still
 * jobs pending.
 *
 * In a next step, we check whether there's still a number of background tasks
 * pending, and we handle those. Yet, we leave a few for the still pending
 * background tasks.
 */
void tarch::multicore::jobs::startToProcessBackgroundJobs() {
  const int maxBusyConsumerTasks = 
    std::max( 
      0, 
	  static_cast<int>(internal::getJobQueue( internal::BackgroundJobsJobClassNumber ).unsafe_size())/TBBMinimalNumberOfJobsPerBackgroundConsumerRun 
	);

  const int maxAdditionalBackgroundThreads =
    std::max(
      tarch::multicore::Core::getInstance().getNumberOfThreads()-1 - internal::_numberOfRunningBackgroundJobConsumerTasks.load(),
      0
    );
  
  const int additionalBackgroundThreads = std::min( maxBusyConsumerTasks, maxAdditionalBackgroundThreads );

  #ifdef Asserts
  if (additionalBackgroundThreads>0) {
    static tarch::logging::Log _log( "tarch::multicore::jobs" );
    logInfo(
      "startToProcessBackgroundJobs()",
      "spawn another " << additionalBackgroundThreads << " background job consumer tasks ("
        << internal::_numberOfRunningBackgroundJobConsumerTasks.load() << " task(s) already running)"
    );
  }
  #endif

  for (int i=0; i<additionalBackgroundThreads; i++) {
    internal::BackgroundJobConsumerTask::enqueue();
  }
}


bool tarch::multicore::jobs::finishToProcessBackgroundJobs() {
  processJobs( internal::BackgroundJobsJobClassNumber, TBBMinimalNumberOfJobsPerBackgroundConsumerRun );

  return !internal::getJobQueue( internal::BackgroundJobsJobClassNumber ).empty();
}


/**
 * @see spawnBlockingJob
 */
void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>&  job0,
  std::function<bool()>&  job1,
  JobType                 jobType0,
  JobType                 jobType1,
  int                     jobClass0,
  int                     jobClass1
) {

/*
  This version is serial and works
  --------------------------------

  job0();
  job1();
*/


/*
   This version is parallel and makes TBB crash
  ---------------------------------------------

   tbb::atomic<int>  semaphore(2);
  
  tbb::parallel_invoke(
    [&] () -> void {
	  internal::spawnBlockingJob( job0, semaphore, jobType0, jobClass0 );
    },
    [&] () -> void {
    	internal::spawnBlockingJob( job1, semaphore, jobType1, jobClass1 );
    }
  );
  while (semaphore>0) {
    tbb::parallel_invoke(
      [&] () -> void {
        processJobs(jobClass0);
      },
      [&] () -> void {
        processJobs(jobClass1);
      }
    );
  }*/

  tbb::atomic<int>  semaphore(2);
  tbb::task_group g;

  g.run( [&]() { internal::spawnBlockingJob( job0, jobType0, jobClass0, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job1, jobType1, jobClass1, semaphore ); });
  //g.wait();

  g.run( [&]() { processJobs(jobClass0); });
  g.run( [&]() { processJobs(jobClass1); });
  g.wait();

  #ifdef Asserts
  int deadlockCounter = 0;
  #endif
  while (semaphore.load()>0) {
    processJobs(jobClass0);
    processJobs(jobClass1); 

    // There are several gaming companies that report that busy loops/spinning
    // over atomics did deadlock in their case (the atomic never was updated)
    // if they didn't use a yield. In our case, some applications seem to
    // exhibit exactly the same behaviour, so I decided to yield here.
    tbb::this_tbb_thread::yield();

    #ifdef Asserts
    deadlockCounter++;
    if (deadlockCounter>65536) {
      static tarch::logging::Log _log( "tarch::multicore::jobs" );
      logInfo( "spawnAndWait(3xJob,...)", internal::report() );
      logInfo( "spawnAndWait(...)", "job-classes: " << jobClass0 << ", " << jobClass1 );
      deadlockCounter = 0;
    }
    #endif
  }
}


/**
 * @see spawnBlockingJob
 */
void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>&  job0,
  std::function<bool()>&  job1,
  std::function<bool()>&  job2,
  JobType                 jobType0,
  JobType                 jobType1,
  JobType                 jobType2,
  int                     jobClass0,
  int                     jobClass1,
  int                     jobClass2
) {
  tbb::atomic<int>  semaphore(3);
  tbb::task_group g;

  g.run( [&]() { internal::spawnBlockingJob( job0, jobType0, jobClass0, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job1, jobType1, jobClass1, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job2, jobType2, jobClass2, semaphore ); });
  //g.wait();

  g.run( [&]() { processJobs(jobClass0); });
  g.run( [&]() { processJobs(jobClass1); });
  g.run( [&]() { processJobs(jobClass2); });
  g.wait();

  #ifdef Asserts
  int deadlockCounter = 0;
  #endif
  while (semaphore.load()>0) {
    processJobs(jobClass0);
    processJobs(jobClass1);
    processJobs(jobClass2);

    tbb::this_tbb_thread::yield();

    #ifdef Asserts
    deadlockCounter++;
    if (deadlockCounter>65536) {
      static tarch::logging::Log _log( "tarch::multicore::jobs" );
      logInfo( "spawnAndWait(3xJob,...)", internal::report() );
      logInfo( "spawnAndWait(...)", "job-classes: " << jobClass0 << ", " << jobClass1 << ", " << jobClass2 );
      deadlockCounter = 0;
    }
    #endif
  }
}


/**
 * @see spawnBlockingJob
 */
void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>&  job0,
  std::function<bool()>&  job1,
  std::function<bool()>&  job2,
  std::function<bool()>&  job3,
  JobType                 jobType0,
  JobType                 jobType1,
  JobType                 jobType2,
  JobType                 jobType3,
  int                     jobClass0,
  int                     jobClass1,
  int                     jobClass2,
  int                     jobClass3
) {
  tbb::atomic<int>  semaphore(4);
  tbb::task_group g;

  g.run( [&]() { internal::spawnBlockingJob( job0, jobType0, jobClass0, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job1, jobType1, jobClass1, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job2, jobType2, jobClass2, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job3, jobType3, jobClass3, semaphore ); });
  //g.wait();

  g.run( [&]() { processJobs(jobClass0); });
  g.run( [&]() { processJobs(jobClass1); });
  g.run( [&]() { processJobs(jobClass2); });
  g.run( [&]() { processJobs(jobClass3); });
  g.wait();

  #ifdef Asserts
  int deadlockCounter = 0;
  #endif
  while (semaphore.load()>0) {
    processJobs(jobClass0);
    processJobs(jobClass1);
    processJobs(jobClass2);
    processJobs(jobClass3);

    tbb::this_tbb_thread::yield();

    #ifdef Asserts
    deadlockCounter++;
    if (deadlockCounter>65536) {
      static tarch::logging::Log _log( "tarch::multicore::jobs" );
      logInfo( "spawnAndWait(4xJob,...)", internal::report() );
      logInfo( "spawnAndWait(...)", "job-classes: " << jobClass0 << ", " << jobClass1 << ", " << jobClass2 << ", " << jobClass3 );
      deadlockCounter = 0;
    }
    #endif
  }
}


/**
 * @see spawnBlockingJob
 */
void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>&  job0,
  std::function<bool()>&  job1,
  std::function<bool()>&  job2,
  std::function<bool()>&  job3,
  std::function<bool()>&  job4,
  JobType                    jobType0,
  JobType                    jobType1,
  JobType                    jobType2,
  JobType                    jobType3,
  JobType                    jobType4,
  int                     jobClass0,
  int                     jobClass1,
  int                     jobClass2,
  int                     jobClass3,
  int                     jobClass4
) {
  tbb::atomic<int>  semaphore(5);
  tbb::task_group g;

  g.run( [&]() { internal::spawnBlockingJob( job0, jobType0, jobClass0, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job1, jobType1, jobClass1, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job2, jobType2, jobClass2, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job3, jobType3, jobClass3, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job4, jobType4, jobClass4, semaphore ); });
  //g.wait();

  g.run( [&]() { processJobs(jobClass0); });
  g.run( [&]() { processJobs(jobClass1); });
  g.run( [&]() { processJobs(jobClass2); });
  g.run( [&]() { processJobs(jobClass3); });
  g.run( [&]() { processJobs(jobClass4); });
  g.wait();

  #ifdef Asserts
  int deadlockCounter = 0;
  #endif
  while (semaphore.load()>0) {
    processJobs(jobClass0); 
    processJobs(jobClass1); 
    processJobs(jobClass2); 
    processJobs(jobClass3); 
    processJobs(jobClass4); 

    tbb::this_tbb_thread::yield();

    #ifdef Asserts
    deadlockCounter++;
    if (deadlockCounter>65536) {
      static tarch::logging::Log _log( "tarch::multicore::jobs" );
      logInfo( "spawnAndWait(5xJob,...)", internal::report() );
      logInfo( "spawnAndWait(...)", "job-classes: " << jobClass0 << ", " << jobClass1 << ", " << jobClass2 << ", " << jobClass3 << ", " << jobClass4 );
      deadlockCounter = 0;
    }
    #endif
  }
}


/**
 * @see spawnBlockingJob
 */
void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>&  job0,
  std::function<bool()>&  job1,
  std::function<bool()>&  job2,
  std::function<bool()>&  job3,
  std::function<bool()>&  job4,
  std::function<bool()>&  job5,
  JobType                 jobType0,
  JobType                 jobType1,
  JobType                 jobType2,
  JobType                 jobType3,
  JobType                 jobType4,
  JobType                 jobType5,
  int                     jobClass0,
  int                     jobClass1,
  int                     jobClass2,
  int                     jobClass3,
  int                     jobClass4,
  int                     jobClass5
) {
  tbb::atomic<int>  semaphore(6);
  tbb::task_group g;

  g.run( [&]() { internal::spawnBlockingJob( job0, jobType0, jobClass0, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job1, jobType1, jobClass1, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job2, jobType2, jobClass2, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job3, jobType3, jobClass3, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job4, jobType4, jobClass4, semaphore ); });
  g.run( [&]() { internal::spawnBlockingJob( job5, jobType5, jobClass5, semaphore ); });
  //g.wait();

  g.run( [&]() { processJobs(jobClass0); });
  g.run( [&]() { processJobs(jobClass1); });
  g.run( [&]() { processJobs(jobClass2); });
  g.run( [&]() { processJobs(jobClass3); });
  g.run( [&]() { processJobs(jobClass4); });
  g.run( [&]() { processJobs(jobClass5); });
  g.wait();

  #ifdef Asserts
  int deadlockCounter = 0;
  #endif
  while (semaphore.load()>0) {
    processJobs(jobClass0); 
    processJobs(jobClass1); 
    processJobs(jobClass2); 
    processJobs(jobClass3); 
    processJobs(jobClass4);
    processJobs(jobClass5);

    tbb::this_tbb_thread::yield();

    #ifdef Asserts
    deadlockCounter++;
    if (deadlockCounter>65536) {
      static tarch::logging::Log _log( "tarch::multicore::jobs" );
      logInfo( "spawnAndWait(6xJob,...)", internal::report() );
      logInfo( "spawnAndWait(...)", "job-classes: " << jobClass0 << ", " << jobClass1 << ", " << jobClass2 << ", " << jobClass3 << ", " << jobClass4 << ", " << jobClass5 );
      deadlockCounter = 0;
    }
    #endif
  }
}


std::string tarch::multicore::jobs::internal::report() {
  std::ostringstream msg;
  msg << "job-system-status: background-queue-no=" << internal::BackgroundJobsJobClassNumber
	  << ", no-of-running-bg-consumer-tasks=" << _numberOfRunningBackgroundJobConsumerTasks.load();
  for (int i=0; i<NumberOfJobQueues; i++) {
	msg << ",queue[" << i
        << "]=" << _pendingJobs[i].unsafe_size() << " job(s)";
  }
  return msg.str();
}

#endif
