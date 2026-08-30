// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PATCH_WRITER_UNSTRUCTURED_H_
#define _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PATCH_WRITER_UNSTRUCTURED_H_


#include "tarch/multicore/BooleanSemaphore.h"

#include "tarch/plotter/griddata/blockstructured/PatchWriter.h"
#include "tarch/plotter/griddata/unstructured/UnstructuredGridWriter.h"


namespace tarch {
  namespace plotter {
    namespace griddata {
      namespace blockstructured {
        class PatchWriterUnstructured;
      }
    }
  }
}



/**
 * This is not really a patch writer, but rather a wrapper around an unstructured grid writer.
 *
 * @author Kristof Unterweger, Tobias Weinzierl
 */
class tarch::plotter::griddata::blockstructured::PatchWriterUnstructured:
  public tarch::plotter::griddata::blockstructured::PatchWriter {
  public:
    class SinglePatchWriter: public tarch::plotter::griddata::blockstructured::PatchWriter::SinglePatchWriter {
      private:
        friend class tarch::plotter::griddata::blockstructured::PatchWriterUnstructured;

        tarch::plotter::griddata::blockstructured::PatchWriterUnstructured&   _base;

        SinglePatchWriter(
          tarch::plotter::griddata::blockstructured::PatchWriterUnstructured& base
        );
      public:
        virtual ~SinglePatchWriter();

        /**
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
        );

        virtual std::pair<int,int> plotPatch(
          const tarch::la::Vector<3,double>& offset,
          const tarch::la::Vector<3,double>& size,
          const tarch::la::Vector<3,int>&    cells
        );

        virtual void close();
    };

   private:
    friend class SinglePatchWriter;

    /**
     * General vtk writer
     */
    tarch::plotter::griddata::unstructured::UnstructuredGridWriter*                               _writer;

    /**
     * Plotter for vertices
     */
    tarch::plotter::griddata::unstructured::UnstructuredGridWriter::VertexWriter*                 _vertexWriter;

    /**
     * Plotter for cells
     */
    tarch::plotter::griddata::unstructured::UnstructuredGridWriter::CellWriter*                   _cellWriter;
  public:
    /**
     * Pass the patch writer an instance of an unstructured writer. The patch
     * writer afterwards is responsible for that instance, i.e. will delete it
     * himself.
     */
    PatchWriterUnstructured( tarch::plotter::griddata::unstructured::UnstructuredGridWriter* writer );

    virtual ~PatchWriterUnstructured();

    /**
     * Closes internal vertex and cell writer and delegates. Please call close()
     * on your SinglePatchWriter before you stream data to a file. If you do not
     * write any data to your patches, actually it doesn't matter whether you
     * have called close before. If you write data, it is important first to
     * close the SinglePatchWriter, then the data writers and finally send
     * the data to a file.
     */
    virtual bool writeToFile( const std::string& filename );

    /**
     * Delegate
     */
    virtual bool isOpen();

    /**
     * Delegate
     */
    virtual void clear();

    virtual tarch::plotter::griddata::blockstructured::PatchWriter::SinglePatchWriter*   createSinglePatchWriter();
    virtual tarch::plotter::griddata::Writer::CellDataWriter*                            createCellDataWriter( const std::string& identifier, int recordsPerCell );
    virtual tarch::plotter::griddata::Writer::VertexDataWriter*                          createVertexDataWriter( const std::string& identifier, int recordsPerVertex );
};

#endif
