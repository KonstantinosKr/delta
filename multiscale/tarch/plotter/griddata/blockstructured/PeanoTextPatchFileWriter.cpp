#include <fstream>

#include "PeanoTextPatchFileWriter.h"
#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"

tarch::logging::Log tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::_log( "tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter" );


const std::string tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::HEADER =
"# \n" \
"# Peano patch file \n" \
"# Version 0.1 \n" \
"# \n";



tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::PeanoTextPatchFileWriter(
  int                 dimension,
  int                 numberOfCellsPerAxis,
  const std::string&  filename,
  bool                append
):
  _writtenToFile(false),
  _dimensions(dimension),
  _numberOfCellsPerAxis(numberOfCellsPerAxis) {
  assertion( dimension>=2 );
  assertion( dimension<=3 );
  assertion( numberOfCellsPerAxis>1 );

  clear();

  if (filename.rfind(".peano-patch-file")!=std::string::npos) {
    logWarning( "PeanoTextPatchFileWriter()", "filename should not end with .peano-patch-file as routine adds extension automatically. Chosen filename=" << filename );
  }

  if (append) {
    _metaFileOut.open( (filename+".peano-patch-file").c_str(), std::ios::app );
  }
  else {
    _metaFileOut.open( (filename+".peano-patch-file").c_str(), std::ios::out );

    if ( (!_metaFileOut.fail()) && _metaFileOut.is_open() ) {
      _metaFileOut << HEADER
                   << "format ASCII" << std::endl;
    }
  }

  if ( !_metaFileOut.is_open() ) {
    logError("PeanoTextPatchFileWriter()", "have not been able to open file " << filename << ".peano-patch-file");
  }
}


tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::~PeanoTextPatchFileWriter() {
  _metaFileOut.close();
}


tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::CellDataWriter*
tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::createCellDataWriter( const std::string& identifier, int recordsPerCell ) {
  return new CellDataWriter(identifier, recordsPerCell, "", nullptr, *this);
}


tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::CellDataWriter*
tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::createCellDataWriter( const std::string& identifier, int recordsPerCell, const std::string& metaData ) {
  return new CellDataWriter(identifier, recordsPerCell, metaData, nullptr, *this);
}


tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::CellDataWriter*
tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::createCellDataWriter( const std::string& identifier, int recordsPerCell, const std::string& metaData, double* mapping ) {
  return new CellDataWriter(identifier, recordsPerCell, metaData, mapping, *this);
}


tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter*
tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::createVertexDataWriter( const std::string& identifier, int recordsPerVertex ) {
  return new VertexDataWriter(identifier, recordsPerVertex , "", nullptr, *this);
}


tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter*
tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::createVertexDataWriter( const std::string& identifier, int recordsPerVertex, const std::string& metaData ) {
  return new VertexDataWriter(identifier, recordsPerVertex , metaData, nullptr, *this);
}


tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter*
tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::createVertexDataWriter( const std::string& identifier, int recordsPerVertex, const std::string& metaData, double* mapping ) {
  return new VertexDataWriter(identifier, recordsPerVertex , metaData, mapping, *this);
}


std::pair<int,int> tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::plotPatch(
  const tarch::la::Vector<2,double>& offset,
  const tarch::la::Vector<2,double>& size
) {
  if (_haveWrittenAtLeastOnePatch) {
    _snapshotFileOut << "end patch" << std::endl << std::endl;
  }

  _snapshotFileOut << "begin patch" << std::endl
       << "  offset";

  for (int d=0; d<2; d++) {
    _snapshotFileOut << " " << offset(d);
  }
  if (_dimensions==3) {
    _snapshotFileOut << " 0";
  }
  _snapshotFileOut << std::endl;

  _snapshotFileOut << "  size";

  for (int d=0; d<2; d++) {
    _snapshotFileOut << " " << size(d);
  }
  if (_dimensions==3) {
    _snapshotFileOut << " 0";
  }
  _snapshotFileOut << std::endl;

  _haveWrittenAtLeastOnePatch = true;

  std::pair<int,int> result(_vertexCounter,_cellCounter);
  _vertexCounter += std::pow(_numberOfCellsPerAxis+1,_dimensions);
  _cellCounter   += std::pow(_numberOfCellsPerAxis,_dimensions);
  return result;
}


std::pair<int,int> tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::plotPatch(
  const tarch::la::Vector<3,double>& offset,
  const tarch::la::Vector<3,double>& size
) {
  assertion( _dimensions==3 );

  if (_haveWrittenAtLeastOnePatch) {
    _snapshotFileOut << "end patch" << std::endl << std::endl;
  }

  _snapshotFileOut << "begin patch" << std::endl
       << "  offset";

  for (int d=0; d<3; d++) {
    _snapshotFileOut << " " << offset(d);
  }
  _snapshotFileOut << std::endl;

  _snapshotFileOut << "  size";

  for (int d=0; d<3; d++) {
    _snapshotFileOut << " " << size(d);
  }
  _snapshotFileOut << std::endl;

  _haveWrittenAtLeastOnePatch = true;

  std::pair<int,int> result(_vertexCounter,_cellCounter);
  _vertexCounter += std::pow(_numberOfCellsPerAxis+1,_dimensions);
  _cellCounter   += std::pow(_numberOfCellsPerAxis,_dimensions);
  return result;
}


bool tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::writeToFile( const std::string& filenamePrefix ) {
  assertion( !_writtenToFile );

  if (tarch::parallel::Node::getInstance().isGlobalMaster()) {
    _metaFileOut << std::endl << "begin dataset" << std::endl;

    for (int i=0; i<tarch::parallel::Node::getInstance().getNumberOfNodes(); i++) {
      if ( i==0 || !tarch::parallel::NodePool::getInstance().isIdleNode(i) ) {
        std::ostringstream referencedFilename;
        if (filenamePrefix.find("/")!=std::string::npos) {
          referencedFilename << filenamePrefix.substr( filenamePrefix.rfind("/")+1 );
        }
        else {
          referencedFilename << filenamePrefix;
        }
        referencedFilename << "-rank-" << i
                           << ".peano-patch-file";
        _metaFileOut << "  include \"" << referencedFilename.str() << "\"" << std::endl;
      }
    }

    _metaFileOut << "end dataset" << std::endl;
  }

  if (filenamePrefix.rfind(".peano-patch-file")!=std::string::npos) {
    logWarning( "writeToFile()", "filename should not end with .peano-patch-file as routine adds extension automatically. Chosen filename prefix=" << filenamePrefix );
  }
  std::ostringstream filenameStream;
  filenameStream << filenamePrefix
    #ifdef Parallel
                 << "-rank-" << tarch::parallel::Node::getInstance().getRank()
    #endif
                 << ".peano-patch-file";
  const std::string filename = filenameStream.str();

  std::ofstream out;
  out.open( filename.c_str(), std::ios::binary );
  if ( (!out.fail()) && out.is_open() ) {
    _log.debug( "writeToFile()", "opened data file " + filename );

    if (_haveWrittenAtLeastOnePatch) {
      _snapshotFileOut << "end patch" << std::endl << std::endl;
    }

    out << _snapshotFileOut.rdbuf();

    _writtenToFile = true;
    return true;
  }
  else {
    _log.error( "close()", "unable to write output file " + filename );
    return false;
  }
}


bool tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::isOpen() {
  return !_writtenToFile;
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::clear() {
  _writtenToFile       = false;

  _snapshotFileOut.clear();

  _vertexCounter = 0;
  _cellCounter   = 0;
  _haveWrittenAtLeastOnePatch = false;


  _snapshotFileOut << HEADER
      << "format ASCII" << std::endl
      << "dimensions " << _dimensions  << std::endl
      << "patch-size" ;
  for (int d=0; d<_dimensions; d++) {
    _snapshotFileOut << " " << _numberOfCellsPerAxis;
  }
  _snapshotFileOut << std::endl << std::endl;
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::addMetaData(const std::string& metaData) {
  _snapshotFileOut << "begin meta-data" << std::endl
       << metaData << std::endl
       << "end meta-data" << std::endl << std::endl;
}


int tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::getCellsPerPatch() const {
  return std::pow( _numberOfCellsPerAxis, _dimensions );
}


int tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::getVerticesPerPatch() const {
  return std::pow( _numberOfCellsPerAxis+1, _dimensions );
}


