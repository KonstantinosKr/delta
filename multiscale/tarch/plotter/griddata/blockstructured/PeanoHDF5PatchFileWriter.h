// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PEANO_HDF5_PATCH_FILE_WRITER_H_
#define _TARCH_PLOTTER_GRID_DATA_BLOCK_STRUCTURED_PEANO_HDF5_PATCH_FILE_WRITER_H_


#include "PeanoPatchFileWriter.h"


#include <vector>


#ifdef HDF5
#include "hdf5.h"
#endif


namespace tarch {
  namespace plotter {
    namespace griddata {
      namespace blockstructured {
        class PeanoHDF5PatchFileWriter;
      }
    }
  }
}


/**
 * HDF 5 writer
 *
 * This class works if and only if you have compiled Peano with -DHDF5.
 */
class tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter: public tarch::plotter::griddata::blockstructured::PeanoPatchFileWriter {
  protected:
    static tarch::logging::Log _log;
    static const std::string HEADER;

    const int  _dimensions;
    const int  _numberOfCellsPerAxis;
    const bool _compress;

    int _vertexCounter;
    int _cellCounter;

    #ifdef HDF5
    hid_t       _file;

    hid_t  createDataTableProperties(int lineWidth, int rowCount) const;
    #endif

    bool        _isOpen;

    std::vector<double>  _geometryData;

    int getCellsPerPatch() const;
    int getVerticesPerPatch() const;

    /**
     * See the cookbook. At any time, the writer pipes data only into one
     * dataset (subdirectory) which is identified through a unique number.
     * Yet, HDF5 works with identifiers (string) instead of numbers, so you
     * have to convert it through getNameOfCurrentDataset().
     */
    int         _numberOfActiveDataset;

    /**
     * @see _numberOfActiveDataset
     */
    std::string  getNameOfCurrentDataset() const;

  public:
    class CellDataWriter: public tarch::plotter::griddata::Writer::CellDataWriter {
      protected:
        tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter& _writer;

        const std::string    _identifier;
        const int            _numberOfUnknowns;
        std::vector<double>  _data;
      public:
        CellDataWriter(
          const std::string& identifier,
          int                numberOfUnknowns,
          const std::string& metaData,
          double*            mapping,
          tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter& writer
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
         tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter& _writer;

         const std::string    _identifier;
         const int            _numberOfUnknowns;
         std::vector<double>  _data;
       public:
         VertexDataWriter(
           const std::string& identifier,
           int                numberOfUnknowns,
           const std::string& metaData,
           double*            mapping,
           tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter& writer
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

    PeanoHDF5PatchFileWriter(
      int                  dimension,
      int                  numberOfCellsPerAxis,
      const std::string&   filename,
      bool                 append,
      bool                 compress
    );
    virtual ~PeanoHDF5PatchFileWriter();


    /**
     * Caller has to destroy this instance manually.
     */
    CellDataWriter*    createCellDataWriter( const std::string& identifier, int recordsPerCell ) override;
    CellDataWriter*    createCellDataWriter( const std::string& identifier, int recordsPerCell, const std::string& metaData ) override;

    /**
     * The mapping is an additional field that has d * (n+1)^d doubles that
     * describe how the vertices within a unit cube are distributed. d is the
     * dimension of the plotter, n is the number of cells per axis.
     */
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

    void addMetaData(const std::string& metaData);
};


#endif
