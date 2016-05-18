#include "tarch/plotter/pointdata/vtk/VTKBinaryFileWriter.h"

#include <limits>
#include <iomanip>

tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::PointDataWriter(
  VTKBinaryFileWriter& myWriter, const std::string& identifier, int recordsPerPoint
):
  _myWriter(myWriter),
  _recordsPerPoint(recordsPerPoint),
  _lastWriteCommandVertexNumber(-1),
  _minValue(std::numeric_limits<double>::max()),
  _maxValue(std::numeric_limits<double>::min()) {
  assertion(_recordsPerPoint>0);

  if (_recordsPerPoint!=3) {
    _out << "SCALARS " << identifier << " " << _myWriter._doubleOrFloat << " " << _recordsPerPoint << std::endl
         << "LOOKUP_TABLE default" << std::endl;
  }
  else {
    _out << "VECTORS " << identifier << " " << _myWriter._doubleOrFloat << " " << std::endl;
  }
}


tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::~PointDataWriter() {
  if (_lastWriteCommandVertexNumber>=-1) {
    close();
  }
}


void tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::assignRemainingPointsDefaultValues() {
  assertion3(
    _lastWriteCommandVertexNumber <= _myWriter._numberOfPoints-1,
    _lastWriteCommandVertexNumber, _myWriter._numberOfPoints,
    "please call close on the vertex writer before"
  );

  while (_lastWriteCommandVertexNumber<_myWriter._currentPointNumber-1) {
    plot(_lastWriteCommandVertexNumber+1,0.0);
  }
}


void tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::close() {
  assertion1(
    _lastWriteCommandVertexNumber>-2,
    "closed twice"
  );
  assertionEquals1(
    _lastWriteCommandVertexNumber, _myWriter._currentPointNumber-1,
    "one record has to be written per vertex"
  );
  assertionMsg( _myWriter.isOpen(), "Maybe you forgot to call close() or assignRemainingVerticesDefaultValues() on a data writer before you destroy your writer"  );

  _myWriter._streamData << _out.str();

  _lastWriteCommandVertexNumber = -2;
}


void tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::plot( int index, double value ) {
  assertion(_lastWriteCommandVertexNumber>=-1);
  assertion(1<=_recordsPerPoint);

  assertion2( value != std::numeric_limits<double>::infinity(), index, value );
  assertion2( value == value, index, value );  // test for not a number

  while (_lastWriteCommandVertexNumber<index-1) {
    plot(_lastWriteCommandVertexNumber+1,0.0);
  }

  _lastWriteCommandVertexNumber = index;

  if (value<_minValue) _minValue = value;
  if (value>_maxValue) _maxValue = value;

  if (_myWriter._precision < 7){
    float tmp = value;
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    for (int i=1; i<_recordsPerPoint; i++) {
      tmp = byteSwapForParaviewBinaryFiles(tmp);
      _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    }
  } else {
    double tmp = value;
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    for (int i=1; i<_recordsPerPoint; i++) {
      tmp = byteSwapForParaviewBinaryFiles(tmp);
      _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    }
  }
}


void tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::plot( int index, const tarch::la::Vector<2,double>& value ) {
  assertion(_lastWriteCommandVertexNumber>=-1);
  assertion(2<=_recordsPerPoint);

  assertion1( value(0) != std::numeric_limits<double>::infinity(), value(0) );
  assertion1( value(0) == value(0), value(0) );  // test for not a number

  assertion1( value(1) != std::numeric_limits<double>::infinity(), value(1) );
  assertion1( value(1) == value(1), value(1) );  // test for not a number

  while (_lastWriteCommandVertexNumber<index-1) {
    plot(_lastWriteCommandVertexNumber+1,0.0);
  }

  _lastWriteCommandVertexNumber = index;

  if (_myWriter._precision < 7){
    float tmp;
    tmp = value(0);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = value(1);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = 0.0;
    for (int i=2; i<_recordsPerPoint; i++) {
      tmp = byteSwapForParaviewBinaryFiles(tmp);
      _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    }
  } else {
    double tmp;
    tmp = value(0);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = value(1);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = 0.0;
    for (int i=2; i<_recordsPerPoint; i++) {
      tmp = byteSwapForParaviewBinaryFiles(tmp);
      _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    }
  }

  if (value(0)<_minValue) _minValue = value(0);
  if (value(0)>_maxValue) _maxValue = value(0);
  if (value(1)<_minValue) _minValue = value(1);
  if (value(1)>_maxValue) _maxValue = value(1);
}


void tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::plot( int index, const tarch::la::Vector<3,double>& value ) {
  assertion(_lastWriteCommandVertexNumber>=-1);
  assertion(3<=_recordsPerPoint);

  assertion1( value(0) != std::numeric_limits<double>::infinity(), value(0) );
  assertion1( value(0) == value(0), value(0) );  // test for not a number

  assertion1( value(1) != std::numeric_limits<double>::infinity(), value(1) );
  assertion1( value(1) == value(1), value(1) );  // test for not a number

  assertion1( value(2) != std::numeric_limits<double>::infinity(), value(2) );
  assertion1( value(2) == value(2), value(2) );  // test for not a number

  while (_lastWriteCommandVertexNumber<index-1) {
    plot(_lastWriteCommandVertexNumber+1,0.0);
  }

  _lastWriteCommandVertexNumber = index;

  if (_myWriter._precision < 7){
    float tmp;
    tmp = value(0);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = value(1);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = value(2);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  } else {
    double tmp;
    tmp = value(0);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = value(1);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = value(2);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  }

  if (value(0)<_minValue) _minValue = value(0);
  if (value(0)>_maxValue) _maxValue = value(0);
  if (value(1)<_minValue) _minValue = value(1);
  if (value(1)>_maxValue) _maxValue = value(1);
  if (value(2)<_minValue) _minValue = value(2);
  if (value(2)>_maxValue) _maxValue = value(2);
}


double tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::getMinValue() const {
  return _minValue;
}


double tarch::plotter::pointdata::vtk::VTKBinaryFileWriter::PointDataWriter::getMaxValue() const {
  return _maxValue;
}
