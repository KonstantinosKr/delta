#include "peano/parallel/loadbalancing/OracleForOnePhase.h"
#include "tarch/Assertions.h"
#include "tarch/parallel/Node.h"


#include <sstream>


std::string peano::parallel::loadbalancing::convertLoadBalancingFlagToString(LoadBalancingFlag flag) {
  assertion( static_cast<int>(flag)>=static_cast<int>(LoadBalancingFlag::ContinueButTryToJoinWorkers) );
  assertion( static_cast<int>(flag)<=static_cast<int>(LoadBalancingFlag::ForkAllChildrenAndBecomeAdministrativeRank) );

  std::ostringstream result;
  switch (flag) {
    case LoadBalancingFlag::ContinueButTryToJoinWorkers:
      result << "continue-but-try-to-join-workers";
      break;
    case LoadBalancingFlag::Continue:
      result << "continue";
      break;
    case LoadBalancingFlag::Join:
      result << "join";
      break;
    case LoadBalancingFlag::UndefinedLoadBalancingFlag:
      result << "undefined-lb-flag";
      break;
    case LoadBalancingFlag::ForkOnce:
      result << "Fork-once";
      break;
    case LoadBalancingFlag::ForkGreedy:
      result << "Fork-greedy";
      break;
    case LoadBalancingFlag::ForkAllChildrenAndBecomeAdministrativeRank:
      result << "fork-all-children/become-administrative-rank";
      break;
    default:
      result << "fork-" << static_cast<int>(flag) << "-times";
      break;
  }
  return result.str();
}
