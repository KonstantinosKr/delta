// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PEANO_PATCH_FILE_WRITER_H_
#define _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PEANO_PATCH_FILE_WRITER_H_


#include "tarch/plotter/griddata/Writer.h"
#include "tarch/logging/Log.h"


namespace tarch {
  namespace plotter {
    namespace griddata {
      namespace blockstructured {
        class PeanoPatchFileWriter;
      }
    }
  }
}


class tarch::plotter::griddata::blockstructured::PeanoPatchFileWriter: public tarch::plotter::griddata::Writer {
  public:
    /**
     * Add a patch to the output format with a given spec.
     *
     * @return A pair of indices. The first one is the first vertex to be
     *         written and the second one is the index of the first cell
     *         to be written.
     */
    virtual std::pair<int,int> plotPatch(
      const tarch::la::Vector<2,double>& offset,
      const tarch::la::Vector<2,double>& size
    ) = 0;

    virtual std::pair<int,int> plotPatch(
      const tarch::la::Vector<3,double>& offset,
      const tarch::la::Vector<3,double>& size
    ) = 0;

    virtual void addMetaData(const std::string& metaData) = 0;

    virtual CellDataWriter*  createCellDataWriter( const std::string& identifier, int recordsPerCell ) = 0;
    virtual CellDataWriter*  createCellDataWriter( const std::string& identifier, int recordsPerCell, const std::string& metaData ) = 0;
    /**
     * The mapping is an additional field that has d * (n+1)^d doubles that
     * describe how the vertices within a unit cube are distributed. d is the
     * dimension of the plotter, n is the number of cells per axis.
     *
     * <h2> Mapping </h2>
     *
     * A Peano patch is a topologically regular grid. By default, we assume
     * that it is equidistantly split. However, the mapping allows users to
     * specify some mesh 'distortion'. ExaHyPE for example uses Legendre nodes
     * and no Cartesian polynomials per patch. We assume that the mapping
     * specifies the spacing in-between the logical axes of the patch relative
     * to the unit square/cube.
     *
     * A mapping runs over all vertices and gives per vertex the two or three,
     * respective, coordinates of the vertex relative to the unit cube.
     */
    virtual CellDataWriter*  createCellDataWriter( const std::string& identifier, int recordsPerCell, const std::string& metaData, double* mapping ) = 0;
    virtual VertexDataWriter*  createVertexDataWriter( const std::string& identifier, int recordsPerVertex ) = 0;
    virtual VertexDataWriter*  createVertexDataWriter( const std::string& identifier, int recordsPerVertex, const std::string& metaData  ) = 0;

    /**
     * @see createCellDataWriter() for details on the mapping.
     */
    virtual VertexDataWriter*  createVertexDataWriter( const std::string& identifier, int recordsPerVertex, const std::string& metaData, double* mapping ) = 0;
};


#endif
