#include "peano/heap/Heap.h"


std::string peano::heap::toString(Allocation type) {
  switch (type) {
    case Allocation::DoNotUseAnyRecycledEntry:
      return "do-not-use-any-recycled-entry";
    case Allocation::UseOnlyRecycledEntries:
      return "use-only-recycled-entries";
    case Allocation::UseRecycledEntriesIfPossibleCreateNewEntriesIfRequired:
      return "use-recycled-entries-if-possible-create-new-entries-if-required";
  }

  assertionMsg( false, "should not be entered ever" );
  return "<undef>";
}


std::string peano::heap::toString(MessageType type) {
  switch (type) {
    case MessageType::NeighbourCommunication:
      return "neighbour";
    case MessageType::ForkOrJoinCommunication:
      return "fork-or-join";
    case MessageType::MasterWorkerCommunication:
      return "master-worker";
  }

  assertionMsg( false, "should not be entered ever" );
  return "<undef>";
}
