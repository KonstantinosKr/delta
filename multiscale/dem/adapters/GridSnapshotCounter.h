// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef DEM_ADAPTERS_GridSnapshotCounter_H_
#define DEM_ADAPTERS_GridSnapshotCounter_H_

namespace dem {
  namespace adapters {

    // Shared across all grid visualiser adapters so that grid-N.vtk file
    // numbering is unique within one simulation run (PlotData, CreateGridAndPlot
    // and the TimeStepAndPlot variants all write into the same output directory).
    int getGridSnapshotCounter();
    int getAndIncrementGridSnapshotCounter();

  }
}

#endif
