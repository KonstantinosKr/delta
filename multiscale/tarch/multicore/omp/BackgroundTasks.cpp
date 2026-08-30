#include "../Jobs.h"
#include "tarch/Assertions.h"


#if defined(SharedOMP) && defined(SharedTBB)

#include <vector>

namespace {
  std::vector<tarch::multicore::BackgroundTask*>  _backgroundTasks;
  int                                             _maxNumberOfBackgroundTasks(1);
}


void tarch::multicore::spawnBackgroundTask(BackgroundTask* task) {
  if (
    _maxNumberOfRunningBackgroundThreads==static_cast<int>(MaxNumberOfRunningBackgroundThreads::ProcessBackgroundTasksImmediately)
  ) {
    task->run();
    delete task;
  }
  else {
    #error This might not work at the moment
    #pragma omp critical(BackgroundCriticalSection)
    _backgroundTasks.push_back(task);
  }
}


/**
 * There are two different implementations: If there is only one background
 * task or we are not allowed to use more than one thread to process background
 * tasks anyway, then we simply run over _backgroundTasks and do all the jobs
 * therein. Otherwise, we extract as many tasks as we are allowed to use
 * threads and process those guys concurrently.
 */
bool tarch::multicore::processBackgroundTasks() {
  bool moreThanOneTask;
  #pragma omp critical(BackgroundCriticalSection)
  {
    moreThanOneTask = _maxNumberOfBackgroundTasks>1 && static_cast<int>(_backgroundTasks.size())>1;
  }

  if (moreThanOneTask) {
    std::vector<tarch::multicore::BackgroundTask*>  triggeredTasks;
    int tasksToTrigger;
    #pragma omp critical(BackgroundCriticalSection)
    {
      tasksToTrigger = std::min( _maxNumberOfBackgroundTasks, static_cast<int>(_backgroundTasks.size()) );
      triggeredTasks = std::vector<tarch::multicore::BackgroundTask*>(_backgroundTasks.begin(),_backgroundTasks.begin()+tasksToTrigger);
      _backgroundTasks.erase(_backgroundTasks.begin(),_backgroundTasks.begin()+tasksToTrigger);
    }

    #pragma omp parallel for
    for (int i=0; i<tasksToTrigger; i++) {
      triggeredTasks[i]->run();
      delete triggeredTasks[i];
    }
  }
  else {
    #pragma omp critical(BackgroundCriticalSection)
    {
      for (int i=0; i<static_cast<int>(_backgroundTasks.size()); i++) {
        triggeredTasks[i]->run();
        delete triggeredTasks[i];
      }
      _backgroundTasks.clear();
    }
  }


  return tasksToTrigger;
}



int tarch::multicore::getNumberOfWaitingBackgroundTasks() {
  int result = 0;

  #pragma omp critical(BackgroundCriticalSection)
  {
    result = _backgroundTasks.size();
  }

  return result;
}


#endif
