#include "tarch/plotter/griddata/unstructured/vtk/VTKBinaryFileWriter.h"
#include "tarch/plotter/ByteSwap.h"


tarch::plotter::griddata::unstructured::vtk::VTKBinaryFileWriter::CellWriter::CellWriter(VTKBinaryFileWriter& writer):
  _currentCellNumber(0),
  _myWriter(writer),
  _cellListEntries(0),
  _cellOut(),
  _cellTypeOut() {
  assertion( _myWriter._numberOfCells==0 );
  assertion( _myWriter._numberOfCellEntries==0 );
}


tarch::plotter::griddata::unstructured::vtk::VTKBinaryFileWriter::CellWriter::~CellWriter() {
  if (_currentCellNumber>=0) {
    close();
  }
}


int tarch::plotter::griddata::unstructured::vtk::VTKBinaryFileWriter::CellWriter::plotPoint(int vertexIndex) {
  assertion( _currentCellNumber>=0 );
  assertion( _cellListEntries>=0 );

  _currentCellNumber++;
  _cellListEntries += 2;

  int tmp;
  tmp = 1;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  tmp = vertexIndex;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));

  tmp = 1;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellTypeOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTKBinaryFileWriter::CellWriter::plotHexahedron(int vertexIndex[8]) {
  assertion( _currentCellNumber>=0 );
  assertion( _cellListEntries>=0 );

  _currentCellNumber++;
  _cellListEntries += 9;

  int tmp;
  tmp = 8;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  for (int i=0; i<8; i++) {
    for (int j=i+1; j<8; j++) assertion(vertexIndex[i] != vertexIndex[j]);
    tmp = vertexIndex[i];
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  }

  tmp = 11;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellTypeOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTKBinaryFileWriter::CellWriter::plotQuadrangle(int vertexIndex[4]) {
  assertion( _currentCellNumber>=0 );
  assertion( _cellListEntries>=0 );

  _currentCellNumber++;
  _cellListEntries += 5;

  int tmp;
  tmp = 4;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  for (int i=0; i<4; i++) {
    for (int j=i+1; j<4; j++) assertion(vertexIndex[i] != vertexIndex[j]);
    tmp = vertexIndex[i];
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  }

  tmp = 8;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellTypeOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTKBinaryFileWriter::CellWriter::plotLine(int vertexIndex[2]) {
  assertion( _currentCellNumber>=0 );
  assertion( _cellListEntries>=0 );

  _currentCellNumber++;
  _cellListEntries += 3;

  int tmp;
  tmp = 2;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  for (int i=0; i<2; i++) {
    tmp = vertexIndex[i];
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  }

  tmp = 3;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellTypeOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));

  return _currentCellNumber-1;
}


int tarch::plotter::griddata::unstructured::vtk::VTKBinaryFileWriter::CellWriter::plotTriangle(int vertexIndex[3]) {
  assertion( _currentCellNumber>=0 );
  assertion( _cellListEntries>=0 );

  _currentCellNumber++;
  _cellListEntries += 4;

  int tmp;
  tmp = 3;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  for (int i=0; i<3; i++) {
    tmp = vertexIndex[i];
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _cellOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  }

  tmp = 5;
  tmp = byteSwapForParaviewBinaryFiles(tmp);
  _cellTypeOut.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));

  return _currentCellNumber-1;
}


void tarch::plotter::griddata::unstructured::vtk::VTKBinaryFileWriter::CellWriter::close() {
  assertion( _myWriter._numberOfCells==0 );
  assertion( _myWriter._numberOfCellEntries==0 );
  assertionMsg( _myWriter.isOpen(), "Maybe you forgot to call close() on a data writer before you destroy your writer?" );

  _myWriter._numberOfCells       = _currentCellNumber;
  _myWriter._numberOfCellEntries = _cellListEntries;

  _myWriter._cellDescription      << _cellOut.str();
  _myWriter._cellTypeDescription  << _cellTypeOut.str();

  _currentCellNumber = -1;
  _cellListEntries   = -1;
}
