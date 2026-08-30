// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_POINTDATA_VTK_TEXT_FILE_WRITER_H_
#define _TARCH_PLOTTER_POINTDATA_VTK_TEXT_FILE_WRITER_H_


#include "tarch/la/Vector.h"
#include "tarch/plotter/pointdata/Writer.h"
#include "tarch/plotter/griddata/unstructured/vtk/VTKTextFileWriter.h"


namespace tarch {
  namespace plotter {
    namespace pointdata {
      namespace vtk {
        class VTKTextFileWriter;
      }
    }
  }
}


/**
 * Implementation of the point data writer that basically maps the points onto
 * a plain vtk text file, i.e. it uses the unstructured grid writer. First
 * prototype of the writer. Isn't fast and should only be used to debug the
 * plot output.
 *
 * @author Tobias Weinzierl
 */
class tarch::plotter::pointdata::vtk::VTKTextFileWriter: public tarch::plotter::pointdata::Writer {
  private:
    tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter                  _vtkWriter;
    tarch::plotter::griddata::unstructured::UnstructuredGridWriter::VertexWriter*   _vertexWriter;
    tarch::plotter::griddata::unstructured::UnstructuredGridWriter::CellWriter*     _cellWriter;

    /**
     * Copy constructor
     *
     * A writer never should be copied. However, we have to make this operation
     * protected to allow implementations to hide their copy constructor as
     * well.
     */
    VTKTextFileWriter(const Writer& writer){}

    /**
     * Assignment operator.
     *
     * A writer never should be copied. However, we have to make this operation
     * protected to allow implementations to hide their copy constructor as
     * well.
     */
    VTKTextFileWriter& operator=(const Writer& writer) {return *this;}

  public:
    VTKTextFileWriter();

    virtual ~VTKTextFileWriter();

    virtual bool writeToFile( const std::string& filename );

    /**
     * @return Whether writer is ready to accept data.
     */
    virtual bool isOpen();

    /**
     * Clear the writer, i.e. erase all the data. However, as the writer does
     * not track how many vertex and cell writers you've created, it's up to
     * you to ensure that none of these instances is left.
     */
    virtual void clear();

    virtual int plotPoint(const tarch::la::Vector<2,double>& position);
    virtual int plotPoint(const tarch::la::Vector<3,double>& position);

    virtual void close();

    /**
     * A writer to assign points a value.
     */
    class PointDataWriter: public Writer::PointDataWriter {
      private:

        /**
         * Underlying VTK writer.
         */
        VTKTextFileWriter& _myWriter;

        tarch::plotter::griddata::Writer::VertexDataWriter*  _particleDataWriter;
      public:
        PointDataWriter(VTKTextFileWriter& myWriter, const std::string& identifier, int recordsPerPoint);
        virtual ~PointDataWriter();

        /**
         * Write data for one point.
         *
         * @param index Index of the point. This index has to equal the index
         *              used for the point.
         * @param value Value for the point.
         */
        virtual void plot( int index, double value );
        virtual void plot( int index, const tarch::la::Vector<2,double>& value );
        virtual void plot( int index, const tarch::la::Vector<3,double>& value );

        /**
         * If you close your writer, each point has to be assigned a
         * value, i.e. you may not add less data than you have points.
         */
        virtual void close();

        /**
         * @see close()
         */
        virtual void assignRemainingPointsDefaultValues();
    };

    /**
     * Caller has to destroy this instance manually.
     */
    virtual Writer::PointDataWriter*    createPointDataWriter( const std::string& identifier, int recordsPerPoint );
};

#endif
