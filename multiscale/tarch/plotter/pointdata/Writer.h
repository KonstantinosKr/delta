// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_POINTDATA_WRITER_H_
#define _TARCH_PLOTTER_POINTDATA_WRITER_H_


#include "tarch/la/Vector.h"


namespace tarch {
  namespace plotter {
    namespace pointdata {
  	  class Writer;
    }
  }
}


/**
 * Interface for all Plotters.
 *
 * @author Tobias Weinzierl
 */
class tarch::plotter::pointdata::Writer {
  private:
    /**
     * Copy constructor
     *
     * A writer never should be copied. However, we have to make this operation
     * protected to allow implementations to hide their copy constructor as
     * well.
     */
    Writer(const Writer& writer){}

    /**
     * Assignment operator.
     *
     * A writer never should be copied. However, we have to make this operation
     * protected to allow implementations to hide their copy constructor as
     * well.
     */
    Writer& operator=(const Writer& writer) {return *this;}

	public:
    Writer() {}

    virtual ~Writer() {}

    virtual bool writeToFile( const std::string& filename ) = 0;

    /**
     * @return Whether writer is ready to accept data.
     */
    virtual bool isOpen() = 0;

    /**
     * Clear the writer, i.e. erase all the data. However, as the writer does
     * not track how many vertex and cell writers you've created, it's up to
     * you to ensure that none of these instances is left.
     */
    virtual void clear() = 0;

    virtual int plotPoint(const tarch::la::Vector<2,double>& position) = 0;
    virtual int plotPoint(const tarch::la::Vector<3,double>& position) = 0;

    virtual void close() = 0;

    /**
     * A writer to assign points a value.
     */
    class PointDataWriter {
   	  public:
        virtual ~PointDataWriter() {};

        /**
         * Write data for one point.
         *
         * @param index Index of the point. This index has to equal the index
         *              used for the point.
         * @param value Value for the point.
         */
        virtual void plot( int index, double value ) = 0;
        virtual void plot( int index, const tarch::la::Vector<2,double>& value ) = 0;
        virtual void plot( int index, const tarch::la::Vector<3,double>& value ) = 0;

        /**
         * If you close your writer, each point has to be assigned a
         * value, i.e. you may not add less data than you have points.
         */
        virtual void close() = 0;

        /**
         * @see close()
         */
        virtual void assignRemainingPointsDefaultValues() = 0;
    };

    /**
     * Caller has to destroy this instance manually.
     */
    virtual PointDataWriter*    createPointDataWriter( const std::string& identifier, int recordsPerPoint )   = 0;
};

#endif
