#include "tarch/plotter/griddata/unstructured/vtk/VTUTextFileWriter.h"


tarch::plotter::griddata::unstructured::vtk::VTUTextFileWriter::CellWriter::CellWriter(VTUTextFileWriter& writer, std::string dataType):
  _dataType(dataType),
  _currentCellNumber(0),
  _currentCellOffset(0),
  _myWriter(writer),
  _connectivityOut(),
  _offsetsOut(),
  _typesOut() {
  assertion( _myWriter._numberOfCells==0 );
}


tarch::plotter::griddata::unstructured::vtk::VTUTextFileWriter::CellWriter::~CellWriter() {
  if (_currentCellNumber>=0) {
    close();
  }
}


int tarch::plotter::griddata::unstructured::vtk::VTUTextFileWriter::CellWriter::plotPoint(int vertexIndex) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 1;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut  << vertexIndex << " ";
  _typesOut         << "1 ";

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTUTextFileWriter::CellWriter::plotHexahedron(int vertexIndex[8]) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 8;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut
    << vertexIndex[0] << " "
    << vertexIndex[1] << " "
    << vertexIndex[2] << " "
    << vertexIndex[3] << " "
    << vertexIndex[4] << " "
    << vertexIndex[5] << " "
    << vertexIndex[6] << " "
    << vertexIndex[7] << " "
    << std::endl;
  _typesOut         << "11 ";

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTUTextFileWriter::CellWriter::plotQuadrangle(int vertexIndex[4]) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 4;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut
  << vertexIndex[0] << " "
  << vertexIndex[1] << " "
  << vertexIndex[2] << " "
  << vertexIndex[3] << " "
    << std::endl;
  _typesOut         << "8 ";

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTUTextFileWriter::CellWriter::plotLine(int vertexIndex[2]) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 2;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut
  << vertexIndex[0] << " "
  << vertexIndex[1] << " "
    << std::endl;
  _typesOut         << "3 ";

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTUTextFileWriter::CellWriter::plotTriangle(int vertexIndex[3]) {
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );

  _currentCellNumber ++;
  _currentCellOffset += 3;

  _offsetsOut       << _currentCellOffset << " ";
  _connectivityOut
  << vertexIndex[0] << " "
  << vertexIndex[1] << " "
  << vertexIndex[2] << " "
    << std::endl;
  _typesOut         << "5 ";

  return _currentCellNumber-1;
}


void tarch::plotter::griddata::unstructured::vtk::VTUTextFileWriter::CellWriter::close() {
  assertion( _myWriter._numberOfCells==0 );
  assertion( _currentCellOffset>=0 );
  assertion( _currentCellNumber>=0 );
  assertionMsg( _myWriter.isOpen(), "Maybe you forgot to call close() on a data writer before you destroy your writer?" );

  _myWriter._numberOfCells       = _currentCellNumber;

  _myWriter._cellDescription =
      "<Cells><DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">" +
      _connectivityOut.str() +
      "</DataArray>" +
      "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">" +
      _offsetsOut.str() +
      "</DataArray>" +
      "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">" +
      _typesOut.str() +
      "</DataArray></Cells>";

  _currentCellNumber = -1;
  _currentCellOffset = -1;
}
