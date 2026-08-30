// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PEANO_TEXT_PATCH_FILE_WRITER_H_
#define _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PEANO_TEXT_PATCH_FILE_WRITER_H_


#include "tarch/plotter/griddata/Writer.h"
#include "tarch/logging/Log.h"
#include "PeanoPatchFileWriter.h"


#include <fstream>


namespace tarch {
  namespace plotter {
    namespace griddata {
      namespace blockstructured {
        class PeanoTextPatchFileWriter;
      }
    }
  }
}


class tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter: public tarch::plotter::griddata::blockstructured::PeanoPatchFileWriter {
  protected:
    static tarch::logging::Log _log;
    static const std::string HEADER;

    bool _writtenToFile;

    int  _dimensions;

    int  _numberOfCellsPerAxis;

    int _vertexCounter;
    int _cellCounter;

    std::ofstream     _metaFileOut;
    std::stringstream _snapshotFileOut;

    bool _haveWrittenAtLeastOnePatch;

    int getCellsPerPatch() const;
    int getVerticesPerPatch() const;

  public:
    class CellDataWriter: public tarch::plotter::griddata::Writer::CellDataWriter {
      protected:
        tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter& _writer;

        const std::string _identifier;
        const int         _numberOfUnknowns;
        int               _patchCounter;
        std::stringstream _out;

        void flushIfPatchIsComplete();
      public:
        CellDataWriter(
          const std::string& identifier,
          int                numberOfUnknowns,
          const std::string& metaData,
          double*            mapping,
          tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter& writer
        );
        virtual ~CellDataWriter();

         /**
          * Write data for one cell.
          *
          * @param index Index of the cell. This index has to equal the index
          *              used for the cell within the VTKWriter class
          *              interface.
          * @param value Value for the cell.
          */
         void plotCell( int index, double value ) override;
         void plotCell( int index, const tarch::la::Vector<2,double>& value ) override;
         void plotCell( int index, const tarch::la::Vector<3,double>& value ) override;
         void plotCell( int index, double* values, int numberOfValues ) override;

         void close() override;

         void assignRemainingCellsDefaultValues() override;
     };

     class VertexDataWriter: public tarch::plotter::griddata::Writer::VertexDataWriter {
       protected:
         tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter& _writer;

         const std::string _identifier;
         const int         _numberOfUnknowns;

         /**
          * Number of entries written within a patch.
          */
         int               _patchCounter;
         std::stringstream _out;

         void flushIfPatchIsComplete();
       public:
         VertexDataWriter(
           const std::string& identifier,
           int                numberOfUnknowns,
           const std::string& metaData,
           double*            mapping,
           tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter& writer
         );
         ~VertexDataWriter();

         /**
          * Write data for one cell.
          *
          * @param index Index of the vertex. This index has to equal the index
          *              used for the cell within the VTKWriter class
          *              interface.
          * @param value Value for the cell.
          */
         void plotVertex( int index, double value ) override;
         void plotVertex( int index, const tarch::la::Vector<2,double>& value ) override;
         void plotVertex( int index, const tarch::la::Vector<3,double>& value ) override;
         void plotVertex( int index, double* values, int numberOfValues ) override;

         void close() override;

         /**
          * @see close()
          */
         void assignRemainingVerticesDefaultValues() override;
     };

    /**
     * @param filename Is the filename of the meta/root file, i.e. the file
     *                 including the snapshots from the various MPI ranks.
     */
    PeanoTextPatchFileWriter(int dimension, int numberOfCellsPerAxis, const std::string& filename, bool append);
    virtual ~PeanoTextPatchFileWriter();

    /**
     * Caller has to destroy this instance manually.
     */
    CellDataWriter*    createCellDataWriter( const std::string& identifier, int recordsPerCell ) override;
    CellDataWriter*    createCellDataWriter( const std::string& identifier, int recordsPerCell, const std::string& metaData ) override;
    CellDataWriter*    createCellDataWriter( const std::string& identifier, int recordsPerCell, const std::string& metaData, double* mapping ) override;

    /**
     * Caller has to destroy this instance manually.
     */
    VertexDataWriter*  createVertexDataWriter( const std::string& identifier, int recordsPerVertex ) override;
    VertexDataWriter*  createVertexDataWriter( const std::string& identifier, int recordsPerVertex, const std::string& metaData  ) override;
    VertexDataWriter*  createVertexDataWriter( const std::string& identifier, int recordsPerVertex, const std::string& metaData, double* mapping ) override;

    std::pair<int,int> plotPatch(
      const tarch::la::Vector<2,double>& offset,
      const tarch::la::Vector<2,double>& size
    ) override;

    std::pair<int,int> plotPatch(
      const tarch::la::Vector<3,double>& offset,
      const tarch::la::Vector<3,double>& size
    ) override;

    /**
     * @param filename The filename is the filename. The code does append a
     *                 rank identifier itself, but if you have multiple
     *                 snapshots (time sequence, e.g.) then please ensure that
     *                 each writeToFile has its unique parameter per time step.
     *                 Notably, please ensure that the argument is not the same
     *                 as the one passed to the constructor.
     * @return Write has been successful
     */
    bool writeToFile( const std::string& filename ) override;

    /**
     * @return Whether writer is ready to accept data.
     */
    bool isOpen() override;

    /**
     * Clear the writer, i.e. erase all the data. However, as the writer does
     * not track how many vertex and cell writers you've created, it's up to
     * you to ensure that none of these instances is left.
     */
    void clear() override;

    void addMetaData(const std::string& metaData) override;
};


#endif
