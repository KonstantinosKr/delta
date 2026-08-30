#include "tarch/plotter/griddata/VTUTimeSeriesWriter.h"

#include "tarch/parallel/NodePool.h"

#include <fstream>

tarch::logging::Log tarch::plotter::griddata::VTUTimeSeriesWriter::_log( "tarch::plotter::griddata::VTUTimeSeriesWriter" );



tarch::plotter::griddata::VTUTimeSeriesWriter::VTUTimeSeriesWriter() {
  _out << "<?xml version=\"1.0\"?>" << std::endl
       << "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\">" << std::endl
       << "<Collection>" << std::endl;

}


void tarch::plotter::griddata::VTUTimeSeriesWriter::addSnapshot(const std::string& snapshotFileName, double time) {
  _out << "<DataSet timestep=\"" << time << "\" group=\"\" part=\"0\" file=\"";

  if (snapshotFileName.find("/")!=std::string::npos) {
    _out << snapshotFileName.substr( snapshotFileName.rfind("/")+1 );
  }
  else {
    _out << snapshotFileName;
  }

  _out
       #ifdef Parallel
       << ".pvtu"
       #else
       << ".vtu"
       #endif
       << "\"/>" << std::endl;
}


bool tarch::plotter::griddata::VTUTimeSeriesWriter::writeFile(const std::string& filenamePrefix) {
  if (!tarch::parallel::Node::getInstance().isGlobalMaster()) return true;

  if (filenamePrefix.rfind(".pvd")!=std::string::npos) {
    logWarning( "writeToFile()", "filename should not end with .pvd as routine adds extension automatically. Chosen filename prefix=" << filenamePrefix );
  }
  std::ostringstream filenameStream;
  filenameStream << filenamePrefix
                 << ".pvd";
  const std::string filename = filenameStream.str();

  std::ofstream out;
  out.open( filename.c_str(), std::ios::binary );
  if ( (!out.fail()) && out.is_open() ) {
    _log.debug( "close()", "opened data file " + filename );

    out << _out.str();

    out << "</Collection>" << std::endl
	<< "</VTKFile>" << std::endl;

    _log.debug( "close()", "data written to " + filename );

    return true;
  }
  else {
    _log.error( "close()", "unable to write output file " + filename );
    return false;
  }
}
