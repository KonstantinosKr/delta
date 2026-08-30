// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_POINTDATA_VTK_BINARY_FILE_WRITER_H_
#define _TARCH_PLOTTER_POINTDATA_VTK_BINARY_FILE_WRITER_H_


#include <stdio.h>
#include <fstream>
#include <iomanip>
#include "tarch/plotter/ByteSwap.h"
#include "tarch/la/Vector.h"
#include "tarch/logging/Log.h"
#include "tarch/plotter/pointdata/Writer.h"


namespace tarch {
  namespace plotter {
    namespace pointdata {
      namespace vtk {
        class VTKBinaryFileWriter;
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
class tarch::plotter::pointdata::vtk::VTKBinaryFileWriter: public tarch::plotter::pointdata::Writer {
  private:

    
    static tarch::logging::Log _log;

    static const std::string HEADER;
    
    bool              _writtenToFile;

    /** precision */
    const int         _precision;

    /** either "float" or "double" depending on _precision */
    const std::string   _doubleOrFloat;

    int                 _numberOfPoints;

    std::stringstream   _streamPositions;
    std::stringstream   _streamData;

    /**
     * Counter for the vertices written. Holds the maximum index.
     */
    int _currentPointNumber;
    
    std::string setDoubleOrFloatString(const int precision){
      if (precision < 7){
        return "float";
      } else {
        return "double";
      }
    }

    void validateDataWriterIdentifier( const std::string& identifier ) const;
    /**
     * Copy constructor
     *
     * A writer never should be copied. However, we have to make this operation
     * protected to allow implementations to hide their copy constructor as
     * well.
     */
    VTKBinaryFileWriter(const Writer& writer):_precision(0){}

    /**
     * Assignment operator.
     *
     * A writer never should be copied. However, we have to make this operation
     * protected to allow implementations to hide their copy constructor as
     * well.
     */
    VTKBinaryFileWriter& operator=(const Writer& writer) {return *this;}

  public:
    VTKBinaryFileWriter(const int precision=6);

    virtual ~VTKBinaryFileWriter();

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
    class PointDataWriter: public tarch::plotter::pointdata::Writer::PointDataWriter {
      private:
        friend class VTKBinaryFileWriter;

        /**
         * Underlying VTK writer.
         */
        VTKBinaryFileWriter& _myWriter;

        int _recordsPerPoint;

        /**
         *
         */
        int _lastWriteCommandVertexNumber;

        double _minValue;
        double _maxValue;

        std::stringstream   _out;
      public:
        PointDataWriter(
          VTKBinaryFileWriter& myWriter,
          const std::string& identifier,
          int recordsPerPoint
        );
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

        virtual double getMinValue() const;
        virtual double getMaxValue() const;

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
