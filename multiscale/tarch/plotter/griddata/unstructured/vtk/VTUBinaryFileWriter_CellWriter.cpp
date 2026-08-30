#include "tarch/plotter/griddata/unstructured/vtk/VTUBinaryFileWriter.h"
#include "tarch/plotter/ByteSwap.h"


tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellWriter::CellWriter(VTUBinaryFileWriter& writer, std::string dataType):
  _dataType(dataType),
  _currentCellNumber(0),
  _currentCellOffset(0),
  _myWriter(writer),
  _connectivityOut(),
  _offsetsOut(),
  _typesOut() {
  assertion( _myWriter._numberOfCells==0 );
}


tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellWriter::~CellWriter() {
  if (_currentCellNumber>=0) {
    close();
  }
}


int tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellWriter::plotPoint(int vertexIndex) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 1;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut  << vertexIndex << " ";
  _typesOut         << "1 ";

  assertionMsg(false,"not implemented yet");

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellWriter::plotHexahedron(int vertexIndex[8]) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 8;


  int tmp;
  tmp = 8;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _offsetsOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));

  for (int i=0; i<8; i++) {
    for (int j=i+1; j<8; j++) assertion(vertexIndex[i] != vertexIndex[j]);
    tmp = vertexIndex[i];
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _connectivityOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  }

  tmp = 11;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _typesOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellWriter::plotQuadrangle(int vertexIndex[4]) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 5;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut
  << vertexIndex[0] << " "
  << vertexIndex[1] << " "
  << vertexIndex[2] << " "
  << vertexIndex[3] << " "
    << std::endl;
  _typesOut         << "8 ";

  assertionMsg(false,"not implemented yet");

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellWriter::plotLine(int vertexIndex[2]) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 3;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut
  << vertexIndex[0] << " "
  << vertexIndex[1] << " "
    << std::endl;
  _typesOut         << "3 ";

  assertionMsg(false,"not implemented yet");

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellWriter::plotTriangle(int vertexIndex[3]) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 4;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut
  << vertexIndex[0] << " "
  << vertexIndex[1] << " "
  << vertexIndex[2] << " "
    << std::endl;
  _typesOut         << "5 ";

  assertionMsg(false,"not implemented yet");

  return _currentCellNumber-1;
}


void tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellWriter::close() {
  assertion( _myWriter._numberOfCells==0 );
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );
  assertionMsg( _myWriter.isOpen(), "Maybe you forgot to call close() on a data writer before you destroy your writer?" );

  _myWriter._numberOfCells       = _currentCellNumber;

  _myWriter._cellDescription
      << "<Cells><DataArray type=\"Int32\" Name=\"connectivity\" format=\"binary\">"
      << _connectivityOut.str()
      << "</DataArray>"
      << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"binary\">"
      << _offsetsOut.str()
      << "</DataArray>"
      << "<DataArray type=\"UInt8\" Name=\"types\" format=\"binary\">"
      << _typesOut.str()
      << "</DataArray></Cells>";

  _currentCellNumber = -1;
  _currentCellOffset = -1;
}
