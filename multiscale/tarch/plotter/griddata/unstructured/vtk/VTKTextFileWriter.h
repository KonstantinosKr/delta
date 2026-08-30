// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_GRIDDATA_UNSTRUCTURED_VTK_VTKTEXTFILEWRITER_H_
#define _TARCH_PLOTTER_GRIDDATA_UNSTRUCTURED_VTK_VTKTEXTFILEWRITER_H_


#include "tarch/logging/Log.h"
#include "tarch/plotter/griddata/unstructured/UnstructuredGridWriter.h"


namespace tarch {
  namespace plotter {
    namespace griddata {
      namespace unstructured {
        namespace vtk {
          class VTKTextFileWriter;
        }
      }
    }
  }
}


/**
 * VTK Writer
 *
 * Output for vtk files (paraview) as text files.
 *
 * !! Usage
 *
 * - Create an instance of the VTKTextFileWriter.
 * - For the vertices you want to write, you have to create your own
 *   VertexWriter.
 * - Pass all the vertices to this writer (both hanging and persistent nodes).
 *   For each vertex you receive an unique number. Remember this number.
 * - For the elements you want to write, you have to create your own
 *   ElementWriter.
 * - For each record create a data writer. There's two writers: One for the
 *   vertices and one for the cells.
 *
 * !!! Thread-safety
 *
 * The plotter is not thread-safe and shall never be thread-safe: It is the
 * responsibility of the using system to implement thread-safe calls. For
 * Peano, this is the mappings where some events could occur simultaneously.
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.1 $
 */
class tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter:
  public tarch::plotter::griddata::unstructured::UnstructuredGridWriter {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    static const std::string HEADER;

    bool _writtenToFile;

    /** precision */
    const int _precision;

    /** either "float" or "double" depending on _precision */
    const std::string _doubleOrFloat;

    /**
     * Total number of vertices
     */
    int _numberOfVertices;

    /**
     * Total number of cells
     */
    int _numberOfCells;

    /**
     * Total number of cell entries. See _cellListEntries.
     */
    int _numberOfCellEntries;

    std::string _vertexDescription;
    std::string _cellDescription;
    std::string _cellTypeDescription;
    std::string _vertexDataDescription;
    std::string _cellDataDescription;

    void validateDataWriterIdentifier( const std::string& identifier ) const;


    std::string setDoubleOrFloatString(const int precision){
      if (precision < 7){
        return "float";
      } else {
        return "double";
      }
    }

  public:
    VTKTextFileWriter(const int precision=6);
    virtual ~VTKTextFileWriter();

    virtual bool isOpen();

    virtual bool writeToFile( const std::string& filename );

    virtual void clear();

    virtual VertexWriter*      createVertexWriter();
    virtual CellWriter*        createCellWriter();
    virtual CellDataWriter*    createCellDataWriter( const std::string& identifier, int recordsPerCell );
    virtual VertexDataWriter*  createVertexDataWriter( const std::string& identifier, int recordsPerVertex );

    /**
     * This is the vertex writer you have to create to plot the vertices.
     * Besides the pure syntax management, the writer also provides a number
     * generator which provides you with a unique id for each vertex.
     *
     * Please ensure that you call close() on the vertex writer before you
     * close the underlying VTKTextFileWriter.
     */
    class VertexWriter:
      public tarch::plotter::griddata::unstructured::UnstructuredGridWriter::VertexWriter {
      private:
        /**
         * The father class is a friend. There are no other friends.
         */
        friend class VTKTextFileWriter;

        /**
         * Counter for the vertices written. Holds the maximum index.
         */
        int _currentVertexNumber;

        /**
         * Underlying VTK writer.
         */
        VTKTextFileWriter& _myWriter;

        /**
         * Output stream
         */
        std::ostringstream _out;

        VertexWriter(VTKTextFileWriter& writer);

        /**
         * Do not copy a vertex writer.
         */
        VertexWriter(const VertexWriter& writer):
          _currentVertexNumber(writer._currentVertexNumber),
          _myWriter(writer._myWriter),
          _out() {
          assertion(false);
        }
      public:
        virtual ~VertexWriter();

        virtual int plotVertex(const tarch::la::Vector<2,double>& position);
        virtual int plotVertex(const tarch::la::Vector<3,double>& position);

        virtual void close();
    };

    /**
     * Writes the element data.
     */
    class CellWriter:
      public tarch::plotter::griddata::unstructured::UnstructuredGridWriter::CellWriter {
      private:
        /**
         * The father class is a friend. There are no other friends.
         */
        friend class VTKTextFileWriter;

        /**
         * Counter for the elements written. Holds the maximum index.
         */
        int _currentCellNumber;

        /**
         * Underlying VTK writer.
         */
        VTKTextFileWriter& _myWriter;

        /**
         * The tag CELLS in a vtk file requires the number of total entries in the
         * following list of cell-interconnection (for a triangle, an entry could
         * look like this: "3 1 2 4", which states that the triangle has 3 links to
         * vetrices with indices 1, 2 and 4. this makes up four entries). For an
         * unstructured mesh, the element type is not fixed and, hence, the total
         * amount of list entries must be counted by summing up the contributions
         * of each element, when adding the element.
         */
        int _cellListEntries;

        /**
         * Output stream
         */
        std::ostringstream _cellOut;

        /**
         * Output stream
         */
        std::ostringstream _cellTypeOut;

        CellWriter(VTKTextFileWriter& writer);

        CellWriter(const CellWriter& writer):
          _currentCellNumber(writer._currentCellNumber),
          _myWriter(writer._myWriter),
          _cellListEntries(writer._cellListEntries),
          _cellOut(),
          _cellTypeOut() {
          assertion(false);
        }
      public:
        virtual ~CellWriter();

        virtual int plotHexahedron(int vertexIndex[8]);

        virtual int plotQuadrangle(int vertexIndex[4]);

        virtual int plotLine(int vertexIndex[2]);

        virtual int plotTriangle(int vertexIndex[3]);

        virtual int plotPoint(int vertexIndex);

        virtual void close();
    };

    class CellDataWriter:
      public tarch::plotter::griddata::Writer::CellDataWriter {
        private:
          /**
           * The father class is a friend. There are no other friends.
           */
          friend class VTKTextFileWriter;

          /**
           * Only required for assertions
           */
          const std::string _identifier;

          /**
           * Is set to -1 in the beginning and then incremented by each data
           * plot call. If we close the writer, the counter is reset to -2.
           */
          int _lastWriteCommandCellNumber;

          /**
           * Underlying VTK writer.
           */
          VTKTextFileWriter& _myWriter;

          /**
           * Output stream
           */
          std::ostringstream _out;

          int _recordsPerCell;

          double _minValue;
          double _maxValue;
          CellDataWriter(const std::string& dataIdentifier, VTKTextFileWriter& writer, int recordsPerCell);

          CellDataWriter(const CellDataWriter& copy):
            _myWriter(copy._myWriter) {
            assertion(false);
          }
      public:
        virtual ~CellDataWriter();

        virtual void close();

        virtual void plotCell( int index, double value );
        virtual void plotCell( int index, const tarch::la::Vector<2,double>& value );
        virtual void plotCell( int index, const tarch::la::Vector<3,double>& value );
        virtual void plotCell( int index, double* values, int numberOfValues );

        virtual double getMinValue() const;
        virtual double getMaxValue() const;

        virtual void assignRemainingCellsDefaultValues();
    };

    class VertexDataWriter:
      public tarch::plotter::griddata::Writer::VertexDataWriter {
        private:
        /**
         * The father class is a friend. There are no other friends.
         */
        friend class VTKTextFileWriter;

        /**
         *
         */
        int _lastWriteCommandVertexNumber;

        /**
         * Underlying VTK writer.
         */
        VTKTextFileWriter& _myWriter;

        /**
         * Output stream
         */
        std::ostringstream _out;

        int _recordsPerVertex;

        double _minValue;
        double _maxValue;

        std::string  _identifier;

        VertexDataWriter(const std::string& dataIdentifier, VTKTextFileWriter& writer, int recordsPerVertex);

        VertexDataWriter(const VertexDataWriter& copy):
          _myWriter(copy._myWriter) {
          assertion(false);
        }
      public:
        virtual ~VertexDataWriter();

        virtual void close();

        virtual void plotVertex( int index, double value );
        virtual void plotVertex( int index, const tarch::la::Vector<2,double>& value );
        virtual void plotVertex( int index, const tarch::la::Vector<3,double>& value );
        virtual void plotVertex( int index, double* values, int numberOfValues );

        virtual double getMinValue() const;
        virtual double getMaxValue() const;

        virtual void assignRemainingVerticesDefaultValues();
    };
};

#endif
