// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_VTU_TIME_SERIES_WRITER_H_
#define _TARCH_PLOTTER_VTU_TIME_SERIES_WRITER_H_


#include <sstream>
#include "tarch/logging/Log.h"


namespace tarch {
  namespace plotter {
    namespace griddata {
      class VTUTimeSeriesWriter;
    }
  }
}


/**
 * Writes time series in vtk/vtu format. According to the VTK spec, time series
 * are solely written serially, i.e. you may use this class on any rank, but it
 * only writes out data on Peano's global master.
 */
class tarch::plotter::griddata::VTUTimeSeriesWriter {
  private:
    static tarch::logging::Log _log;

    std::stringstream _out;
  public:
    VTUTimeSeriesWriter();

    /**
     * Add a new snapshot. Please pass full name incl. extension.
     */
    void addSnapshot(const std::string& snapshotFileName, double time);

    /**
     * This is the name of the VTU file. It has nothing to do with the actual
     * name of the data files.
     *
     * @return Has been successful
     */
    bool writeFile(const std::string& outputFileName);
};


#endif

