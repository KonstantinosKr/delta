// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PATCH_WRITER_H_
#define _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PATCH_WRITER_H_

#include "tarch/plotter/griddata/Writer.h"

#include "tarch/multicore/BooleanSemaphore.h"


namespace tarch {
  namespace plotter {
    namespace griddata {
      namespace blockstructured {
        class PatchWriter;
      }
    }
  }
}


/**
 * Abstract base class for a patch plotter
 *
 * Patch plotters are either wrappers around existing plotters or real
 * plotters. This is their abstract interface. They are different to
 * the
 *
 * @author Kristof Unterweger, Tobias Weinzierl
 */
class tarch::plotter::griddata::blockstructured::PatchWriter:
  public tarch::plotter::griddata::Writer {
  public:
    /**
     * Writes patches
     *
     * Typically, you use this writer within enterCell as follows:
     * \code
    int unknownIndex = _patchWriter->plotPatch(
      fineGridVerticesEnumerator.getVertexPosition(),
      fineGridVerticesEnumerator.getCellSize(),
      Experiment::getInstance().getNumberOfInnerCellsPerSpacetreeLeaf()
    ).first;

    const tarch::la::Vector<DIMENSIONS,int> offsetOfInnerDataWithinPatch = tarch::la::Vector<DIMENSIONS,int>(
      Experiment::getInstance().getNumberOfGhostCellsPerSpacetreeLeaf()
    );

    dfor(i,Experiment::getInstance().getNumberOfInnerCellsPerSpacetreeLeaf()+1) {
      const tarch::la::Vector<DIMENSIONS,int> currentVertex = offsetOfInnerDataWithinPatch + i;
      const int linearisedCurrentVertex =
        peano::utils::dLinearisedWithoutLookup(currentVertex,Experiment::getInstance().getNumberOfCellsPerSpacetreeLeaf()+1);

      assertion( static_cast<int>( DataHeap::getInstance().getData(patchDescription.getU()).size() )>linearisedCurrentVertex);

      _solutionWriter->plotVertex(
        unknownIndex,
        DataHeap::getInstance().getData(patchDescription.getU())[linearisedCurrentVertex]._persistentRecords._u
      );
      unknownIndex++;
    }
     \endcode
     */
    class SinglePatchWriter {
      public:
        virtual ~SinglePatchWriter() {}

        /**
         * Create grid structure for one patch
         *
         * @param offset
         * @param size
         * @param cells
         *
         * @return Pair of indices. The first index is the index of very first
         *         vertex in the patch, the second index is the index of the
         *         very first cell.
         */
        virtual std::pair<int,int> plotPatch(
          const tarch::la::Vector<2,double>& offset,
          const tarch::la::Vector<2,double>& size,
          const tarch::la::Vector<2,int>&    cells
        ) = 0;
        virtual std::pair<int,int> plotPatch(
          const tarch::la::Vector<3,double>& offset,
          const tarch::la::Vector<3,double>& size,
          const tarch::la::Vector<3,int>&    cells
        ) = 0;
/*
        virtual std::pair<int,int> plotPatch(
          const tarch::la::Vector<2,double>& offset,
          const tarch::la::Vector<2,double>& size,
          int                                cellsPerDimension
        ) = 0;
        virtual std::pair<int,int> plotPatch(
          const tarch::la::Vector<3,double>& offset,
          const tarch::la::Vector<3,double>& size,
          int                                cellsPerDimension
        ) = 0;
*/

        /**
         * Pleaes close the patch writer before you close any data writer.
         */
        virtual void close() = 0;
    };

    /**
     * Caller has to destroy this instance manually. Do not create more than
     * one vertex writer.
     */
    virtual SinglePatchWriter*   createSinglePatchWriter() = 0;

    virtual ~PatchWriter();
};

#endif
