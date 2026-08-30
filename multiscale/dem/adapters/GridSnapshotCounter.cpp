#include "dem/adapters/GridSnapshotCounter.h"

namespace dem {
  namespace adapters {

    namespace {
      int _gridSnapshotCounter = 0;
    }

    int getGridSnapshotCounter() {
      return _gridSnapshotCounter;
    }

    int getAndIncrementGridSnapshotCounter() {
      return _gridSnapshotCounter++;
    }

  }
}
