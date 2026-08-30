#include "tarch/multicore/Jobs.h"
#include "tarch/Assertions.h"


#include "tarch/multicore/MulticoreDefinitions.h"

#include <thread>
#include <queue>


int tarch::multicore::jobs::Job::_maxNumberOfRunningBackgroundThreads( std::thread::hardware_concurrency() );


void tarch::multicore::jobs::Job::setMaxNumberOfRunningBackgroundThreads(int maxNumberOfRunningBackgroundThreads) {
  assertion(maxNumberOfRunningBackgroundThreads==DontUseAnyBackgroundJobs || maxNumberOfRunningBackgroundThreads>=1);
  _maxNumberOfRunningBackgroundThreads = maxNumberOfRunningBackgroundThreads;
}


tarch::multicore::jobs::JobType tarch::multicore::jobs::Job::getJobType() const {
  return _jobType;
}


tarch::multicore::jobs::Job::Job( JobType jobType, int jobClass ):
  _jobType(jobType),
  _jobClass(jobClass) {
}


tarch::multicore::jobs::Job::~Job() {
}


bool tarch::multicore::jobs::Job::isTask() const {
  return _jobType!=JobType::Job;
}


int tarch::multicore::jobs::Job::getClass() const {
  return _jobClass;
}



tarch::multicore::jobs::GenericJobWithCopyOfFunctor::GenericJobWithCopyOfFunctor( const std::function<bool()>& functor, JobType jobType, int jobClass ):
  Job(jobType,jobClass),
  _functor(functor)  {
}


bool tarch::multicore::jobs::GenericJobWithCopyOfFunctor::run() {
  return _functor();
}


tarch::multicore::jobs::GenericJobWithCopyOfFunctor::~GenericJobWithCopyOfFunctor() {
}


tarch::multicore::jobs::GenericJobWithoutCopyOfFunctor::GenericJobWithoutCopyOfFunctor(std::function<bool()>& functor, JobType jobType, int jobClass ):
  Job(jobType,jobClass),
  _functor(functor)  {
}


bool tarch::multicore::jobs::GenericJobWithoutCopyOfFunctor::run() {
  return _functor();
}


tarch::multicore::jobs::GenericJobWithoutCopyOfFunctor::~GenericJobWithoutCopyOfFunctor() {
}


#ifndef SharedMemoryParallelisation

namespace {
  std::queue<tarch::multicore::jobs::Job* > backgroundJobs;
}

void tarch::multicore::jobs::spawnBackgroundJob(Job* task) {
  backgroundJobs.push( task );
}


void tarch::multicore::jobs::startToProcessBackgroundJobs() {
}


bool tarch::multicore::jobs::finishToProcessBackgroundJobs() {
  // Note: Only invoked if no shared memory parallelisation activated. If
  // TBB/C++/OpenMP are enabled, the routine of the respective subfolder is
  // invoked
  if (backgroundJobs.empty()) {
    return false;
  }
  else {
    while ( !backgroundJobs.empty() ) {
      Job* p = backgroundJobs.front();
      backgroundJobs.pop();
      while (p->run()) {};
      delete p;
    }
    return true;
  }
}


int tarch::multicore::jobs::getNumberOfWaitingBackgroundJobs() {
  return 0;
}


void tarch::multicore::jobs::spawn(Job*  job) {
  while( job->run() ) {};
  delete job;
}


void tarch::multicore::jobs::spawn(std::function<bool()>& job, JobType jobType, int jobClass) {
  job();
}


int tarch::multicore::jobs::getNumberOfPendingJobs() {
  return 0;
}



bool tarch::multicore::jobs::processJobs(int jobClass, int maxNumberOfJobs) {
  return false;
}


void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>&  job0,
  std::function<bool()>&  job1,
  JobType                 jobType0,
  JobType                 jobType1,
  int                     jobClass0,
  int                     jobClass1
) {
  while (job0()) {};
  while (job1()) {};
}


void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>& job0,
  std::function<bool()>& job1,
  std::function<bool()>& job2,
  JobType                    jobType0,
  JobType                    jobType1,
  JobType                    jobType2,
	 int                     jobClass0,
	 int                     jobClass1,
	 int                     jobClass2
) {
  while (job0()) {};
  while (job1()) {};
  while (job2()) {};
}


void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>& job0,
  std::function<bool()>& job1,
  std::function<bool()>& job2,
  std::function<bool()>& job3,
  JobType                    jobType0,
  JobType                    jobType1,
  JobType                    jobType2,
  JobType                    jobType3,
	 int                     jobClass0,
	 int                     jobClass1,
	 int                     jobClass2,
	 int                     jobClass3
) {
  while (job0()) {};
  while (job1()) {};
  while (job2()) {};
  while (job3()) {};
}


void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>& job0,
  std::function<bool()>& job1,
  std::function<bool()>& job2,
  std::function<bool()>& job3,
  std::function<bool()>& job4,
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
  while (job0()) {};
  while (job1()) {};
  while (job2()) {};
  while (job3()) {};
  while (job4()) {};
}


void tarch::multicore::jobs::spawnAndWait(
  std::function<bool()>& job0,
  std::function<bool()>& job1,
  std::function<bool()>& job2,
  std::function<bool()>& job3,
  std::function<bool()>& job4,
  std::function<bool()>& job5,
  JobType                    jobType0,
  JobType                    jobType1,
  JobType                    jobType2,
  JobType                    jobType3,
  JobType                    jobType4,
  JobType                    jobType5,
	 int                     jobClass0,
	 int                     jobClass1,
	 int                     jobClass2,
	 int                     jobClass3,
	 int                     jobClass4,
	 int                     jobClass5
) {
  while (job0()) {};
  while (job1()) {};
  while (job2()) {};
  while (job3()) {};
  while (job4()) {};
  while (job5()) {};
}


#endif

