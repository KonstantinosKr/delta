#include "peano/datatraversal/TaskSet.h"


#include <chrono>
#include <thread>


#include "tarch/multicore/Jobs.h"
#include "tarch/Assertions.h"

tarch::logging::Log  peano::datatraversal::TaskSet::_log( "peano::datatraversal::TaskSet" );



int  peano::datatraversal::TaskSet::translateIntoJobClass( TaskType type ) {
  const int Default = 7;
  switch ( type ) {
    case TaskType::IsTaskAndRunAsSoonAsPossible:
   	  return 0;
    case TaskType::IsTaskAndRunImmediately:
     	  return Default;
    case TaskType::LoadCells:
 	  return 1;
    case TaskType::LoadVertices:
 	  return 2;
    case TaskType::TriggerEvents:
 	  return 3;
    case TaskType::StoreCells:
 	  return 4;
    case TaskType::StoreVertices:
 	  return 5;
    case TaskType::Background:
   	  return Default;
    case TaskType::BackgroundMPIReceiveTask:
   	  return Default;
  }
  return Default;
}


tarch::multicore::jobs::JobType peano::datatraversal::TaskSet::translateIntoJobType( TaskType type ) {
  switch ( type ) {
    case TaskType::IsTaskAndRunAsSoonAsPossible:
      return tarch::multicore::jobs::JobType::RunTaskAsSoonAsPossible;
    case TaskType::IsTaskAndRunImmediately:
   	  return tarch::multicore::jobs::JobType::ProcessImmediately;
    case TaskType::LoadCells:
    case TaskType::LoadVertices:
    case TaskType::TriggerEvents:
    case TaskType::StoreCells:
    case TaskType::StoreVertices:
   	  return tarch::multicore::jobs::JobType::Job;
    case TaskType::Background:
   	  return tarch::multicore::jobs::JobType::Task;
    case TaskType::BackgroundMPIReceiveTask:
      return tarch::multicore::jobs::JobType::MPIReceiveTask;
  }
  return tarch::multicore::jobs::JobType::ProcessImmediately;
}


void peano::datatraversal::TaskSet::waitForLoadCellsTask() {
  logDebug( "waitForAllLoadCellsTasks()", "start to wait for load cell tasks" );
  tarch::multicore::jobs::processJobs(translateIntoJobClass(TaskType::LoadCells));
  logDebug( "waitForAllLoadCellsTasks()", "wait for load cell tasks finished" );
}


void peano::datatraversal::TaskSet::waitForLoadVerticesTask() {
  logDebug( "waitForAllLoadVerticesTasks()", "start to wait for load vertices tasks" );
  tarch::multicore::jobs::processJobs(translateIntoJobClass(TaskType::LoadVertices));
  logDebug( "waitForAllLoadVerticesTasks()", "wait for load vertices tasks finished" );
}


void peano::datatraversal::TaskSet::waitForEventTask() {
  logDebug( "waitForAllEventTasks()", "start to wait for event tasks" );
  tarch::multicore::jobs::processJobs(translateIntoJobClass(TaskType::TriggerEvents));
  logDebug( "waitForAllEventTasks()", "wait for event tasks finished" );
}


void peano::datatraversal::TaskSet::waitForStoreCellsTask() {
  logDebug( "waitForAllStoreCellsTasks()", "start to wait for store cell tasks" );
  tarch::multicore::jobs::processJobs(translateIntoJobClass(TaskType::StoreCells));
  logDebug( "waitForAllStoreCellsTasks()", "wait for store cell tasks finished" );
}


void peano::datatraversal::TaskSet::waitForStoreVerticesTask() {
  logDebug( "waitForAllStoreVerticesTasks()", "start to wait for store vertices tasks" );
  tarch::multicore::jobs::processJobs(translateIntoJobClass(TaskType::StoreVertices));
  logDebug( "waitForAllStoreVerticesTasks()", "wait for store vertices tasks finished" );
}


peano::datatraversal::TaskSet::TaskSet(
  std::function<bool ()>&& function1,
  std::function<bool ()>&& function2,
  TaskType                 type1,
  TaskType                 type2,
  bool                     parallelise
) {
  if (parallelise) {
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(2,2);

    tarch::multicore::jobs::spawnAndWait(
      function1,
	  function2,
	  translateIntoJobType(type1),
	  translateIntoJobType(type2),
	  translateIntoJobClass(type1),
	  translateIntoJobClass(type2)
    );

    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(-2,-2);
  }
  else {
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(0,2);
    function1();
    function2();
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(0,-2);
  }
}


peano::datatraversal::TaskSet::TaskSet(
  std::function<bool ()>&& function1,
  std::function<bool ()>&& function2,
  std::function<bool ()>&& function3,
  TaskType                 type1,
  TaskType                 type2,
  TaskType                 type3,
  bool                     parallelise
) {
  if (parallelise) {
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(3,3);

    tarch::multicore::jobs::spawnAndWait(
      function1,
	  function2,
	  function3,
	  translateIntoJobType(type1),
	  translateIntoJobType(type2),
	  translateIntoJobType(type3),
	  translateIntoJobClass(type1),
	  translateIntoJobClass(type2),
	  translateIntoJobClass(type3)
    );

    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(-3,-3);
  }
  else {
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(0,3);
    function1();
    function2();
    function3();
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(0,-3);
  }
}


peano::datatraversal::TaskSet::TaskSet(
  std::function<bool ()>&& function1,
  std::function<bool ()>&& function2,
  std::function<bool ()>&& function3,
  std::function<bool ()>&& function4,
  TaskType                 type1,
  TaskType                 type2,
  TaskType                 type3,
  TaskType                 type4,
  bool                     parallelise
) {
  if (parallelise) {
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(4,4);

    tarch::multicore::jobs::spawnAndWait(
      function1,
	  function2,
	  function3,
	  function4,
	  translateIntoJobType(type1),
	  translateIntoJobType(type2),
	  translateIntoJobType(type3),
	  translateIntoJobType(type4),
	  translateIntoJobClass(type1),
	  translateIntoJobClass(type2),
	  translateIntoJobClass(type3),
	  translateIntoJobClass(type4)
    );

    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(-4,-4);
  }
  else {
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(0,4);
    function1();
    function2();
    function3();
    function4();
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(0,-4);
  }
}


peano::datatraversal::TaskSet::TaskSet(
  std::function<bool ()>&& function1,
  std::function<bool ()>&& function2,
  std::function<bool ()>&& function3,
  std::function<bool ()>&& function4,
  std::function<bool ()>&& function5,
  TaskType                 type1,
  TaskType                 type2,
  TaskType                 type3,
  TaskType                 type4,
  TaskType                 type5,
  bool                     parallelise
) {
  if (parallelise) {
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(4,4);

    tarch::multicore::jobs::spawnAndWait(
      function1,
	  function2,
	  function3,
	  function4,
	  function5,
	  translateIntoJobType(type1),
	  translateIntoJobType(type2),
	  translateIntoJobType(type3),
	  translateIntoJobType(type4),
	  translateIntoJobType(type5),
	  translateIntoJobClass(type1),
	  translateIntoJobClass(type2),
	  translateIntoJobClass(type3),
	  translateIntoJobClass(type4),
	  translateIntoJobClass(type5)
    );

    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(-4,-4);
  }
  else {
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(0,4);
    function1();
    function2();
    function3();
    function4();
    function5();
    peano::performanceanalysis::Analysis::getInstance().changeConcurrencyLevel(0,-4);
  }
}


peano::datatraversal::TaskSet::TaskSet(
  std::function<bool ()>&&  myTask,
  TaskType                  taskType
) {
  typedef tarch::multicore::jobs::GenericJobWithCopyOfFunctor           Job;
  switch (taskType) {
    case TaskType::IsTaskAndRunImmediately:
      myTask();
      break;
    case TaskType::IsTaskAndRunAsSoonAsPossible:
    case TaskType::LoadCells:
    case TaskType::LoadVertices:
    case TaskType::TriggerEvents:
    case TaskType::StoreCells:
    case TaskType::StoreVertices:
      tarch::multicore::jobs::spawn( new Job(myTask,translateIntoJobType(taskType),translateIntoJobClass(taskType) ) );
      break;
    case TaskType::Background:
    case TaskType::BackgroundMPIReceiveTask:
   	  peano::performanceanalysis::Analysis::getInstance().minuteNumberOfBackgroundTasks(
   	    tarch::multicore::jobs::getNumberOfWaitingBackgroundJobs()
   	  );
      tarch::multicore::jobs::spawnBackgroundJob( new Job(myTask,translateIntoJobType(taskType),translateIntoJobClass(taskType) ) );
      break;
  }
}


void peano::datatraversal::TaskSet::startToProcessBackgroundJobs() {
  tarch::multicore::jobs::startToProcessBackgroundJobs();
}


bool peano::datatraversal::TaskSet::finishToProcessBackgroundJobs() {
  bool result = tarch::multicore::jobs::finishToProcessBackgroundJobs();
  peano::performanceanalysis::Analysis::getInstance().minuteNumberOfBackgroundTasks(
    tarch::multicore::jobs::getNumberOfWaitingBackgroundJobs()
  );
  return result;
}
