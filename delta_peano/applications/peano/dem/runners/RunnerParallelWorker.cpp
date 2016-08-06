#include "dem/runners/Runner.h"


#ifdef Parallel
#include "peano/utils/Globals.h"
#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/parallel/NodePool.h"
#include "peano/parallel/messages/ForkMessage.h"
#include "dem/repositories/Repository.h"


int dem::runners::Runner::runAsWorker(dem::repositories::Repository& repository) {
  int newMasterNode = tarch::parallel::NodePool::getInstance().waitForJob(); 
  while ( newMasterNode != tarch::parallel::NodePool::JobRequestMessageAnswerValues::Terminate ) {
    if ( newMasterNode >= tarch::parallel::NodePool::JobRequestMessageAnswerValues::NewMaster ) {
      peano::parallel::messages::ForkMessage forkMessage;
      forkMessage.receive(tarch::parallel::NodePool::getInstance().getMasterRank(),tarch::parallel::NodePool::getInstance().getTagForForkMessages(), true, ReceiveIterationControlMessagesBlocking);

      repository.restart(
        forkMessage.getH(),
        forkMessage.getDomainOffset(),
        forkMessage.getLevel(),
        forkMessage.getPositionOfFineGridCellRelativeToCoarseGridCell()
      );
  
      bool continueToIterate = true;
      while (continueToIterate) {
        switch (repository.continueToIterate()) {
          case dem::repositories::Repository::Continue:
            repository.iterate();  
            break;
          case dem::repositories::Repository::Terminate:
            continueToIterate = false;  
            break;
          case dem::repositories::Repository::RunGlobalStep:
            runGlobalStep();  
            break;
        }
      }
    
      // insert your postprocessing here
      // -------------------------------  

      // -------------------------------

      repository.terminate();
    }
    else if ( newMasterNode == tarch::parallel::NodePool::JobRequestMessageAnswerValues::RunAllNodes ) {
      runGlobalStep();  
    }
    newMasterNode = tarch::parallel::NodePool::getInstance().waitForJob(); 
  }
  return 0;
}


void dem::runners::Runner::runGlobalStep() {
  // You might want to remove this assertion, but please consult the 
  // documentation before.
  assertion( !peano::parallel::loadbalancing::Oracle::getInstance().isLoadBalancingActivated() );
  
  // insert yourcode here
  // -------------------------------

  // -------------------------------
}
#endif
