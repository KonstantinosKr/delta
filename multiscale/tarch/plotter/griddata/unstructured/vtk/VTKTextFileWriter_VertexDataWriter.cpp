#include "tarch/plotter/griddata/unstructured/vtk/VTKTextFileWriter.h"

#include <limits>
#include <iomanip>

tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::VertexDataWriter(
  const std::string& identifier, VTKTextFileWriter& writer, int recordsPerVertex
):
  _lastWriteCommandVertexNumber(-1),
  _myWriter(writer),
  _out(),
  _recordsPerVertex(recordsPerVertex),
  _minValue(std::numeric_limits<double>::max()),
  _maxValue(std::numeric_limits<double>::min()),
  _identifier(identifier) {
  assertion(_recordsPerVertex>0);

  _out << std::setprecision(_myWriter._precision);
  if (_recordsPerVertex!=3) {
    _out << "SCALARS " << _identifier << " " << _myWriter._doubleOrFloat << " " << _recordsPerVertex << std::endl
         << "LOOKUP_TABLE default" << std::endl;
  }
  else {
    _out << "VECTORS " << _identifier << " " << _myWriter._doubleOrFloat << " " << std::endl;
  }
}


tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::~VertexDataWriter() {
  if (_lastWriteCommandVertexNumber>=-1) {
    close();
  }
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::assignRemainingVerticesDefaultValues() {
  assertion4(
    _lastWriteCommandVertexNumber <= _myWriter._numberOfCells-1,
    _lastWriteCommandVertexNumber, _myWriter._numberOfCells,
    _identifier,
    "please call close on the vertex writer before"
  );

  while (_lastWriteCommandVertexNumber<_myWriter._numberOfCells-1) {
    plotVertex(_lastWriteCommandVertexNumber+1,0.0);
  }
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::close() {
  assertion2(
    _lastWriteCommandVertexNumber>-2,
    _identifier,
    "closed twice"
  );
  assertionMsg(
    _lastWriteCommandVertexNumber==_myWriter._numberOfVertices-1,
    "one record has to be written per vertex on writer " << _identifier <<
    " (vertex entries written=" <<  _lastWriteCommandVertexNumber <<
    ", vertex entries expected=" << _myWriter._numberOfVertices << ")"
  );
  assertionMsg( _myWriter.isOpen(), "Maybe you forgot to call close() or assignRemainingVerticesDefaultValues() on a data writer before you destroy your writer for value " << _identifier );

  if (_lastWriteCommandVertexNumber>=-1) {
    _out << std::endl;
    _myWriter._vertexDataDescription += _out.str();
  }
  _lastWriteCommandVertexNumber = -2;
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::plotVertex( int index, double value ) {
  assertion(_lastWriteCommandVertexNumber>=-1);
  assertion(1<=_recordsPerVertex);

  assertion3( value != std::numeric_limits<double>::infinity(), index, value, _identifier);
  assertion3( value == value, index, value, _identifier);  // test for not a number

  while (_lastWriteCommandVertexNumber<index-1) {
    plotVertex(_lastWriteCommandVertexNumber+1,0.0);
  }

  _lastWriteCommandVertexNumber = index;
  _out << value << " ";
  for (int i=1; i<_recordsPerVertex; i++) {
    _out << 0.0 << " ";
  }

  _out << std::endl;

  if (value<_minValue) _minValue = value;
  if (value>_maxValue) _maxValue = value;
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::plotVertex( int index, const tarch::la::Vector<2,double>& value ) {
  assertion(_lastWriteCommandVertexNumber>=-1);
  assertion(2<=_recordsPerVertex);

  assertion1( value(0) != std::numeric_limits<double>::infinity(), value(0) );
  assertion1( value(0) == value(0), value(0) );  // test for not a number

  assertion1( value(1) != std::numeric_limits<double>::infinity(), value(1) );
  assertion1( value(1) == value(1), value(1) );  // test for not a number

  while (_lastWriteCommandVertexNumber<index-1) {
    plotVertex(_lastWriteCommandVertexNumber+1,0.0);
  }

  _lastWriteCommandVertexNumber = index;
  _out << value(0) << " ";
  _out << value(1) << " ";
  for (int i=2; i<_recordsPerVertex; i++) {
    _out << 0.0 << " ";
  }
  _out << std::endl;

  if (value(0)<_minValue) _minValue = value(0);
  if (value(0)>_maxValue) _maxValue = value(0);
  if (value(1)<_minValue) _minValue = value(1);
  if (value(1)>_maxValue) _maxValue = value(1);
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::plotVertex( int index, const tarch::la::Vector<3,double>& value ) {
  assertion(_lastWriteCommandVertexNumber>=-1);
  assertion(3<=_recordsPerVertex);

  assertion1( value(0) != std::numeric_limits<double>::infinity(), value(0) );
  assertion1( value(0) == value(0), value(0) );  // test for not a number

  assertion1( value(1) != std::numeric_limits<double>::infinity(), value(1) );
  assertion1( value(1) == value(1), value(1) );  // test for not a number

  assertion1( value(2) != std::numeric_limits<double>::infinity(), value(2) );
  assertion1( value(2) == value(2), value(2) );  // test for not a number

  while (_lastWriteCommandVertexNumber<index-1) {
    plotVertex(_lastWriteCommandVertexNumber+1,0.0);
  }

  _lastWriteCommandVertexNumber = index;
  _out << value(0) << " ";
  _out << value(1) << " ";
  _out << value(2) << " ";
  for (int i=3; i<_recordsPerVertex; i++) {
    _out << 0.0 << " ";
  }
  _out << std::endl;

  if (value(0)<_minValue) _minValue = value(0);
  if (value(0)>_maxValue) _maxValue = value(0);
  if (value(1)<_minValue) _minValue = value(1);
  if (value(1)>_maxValue) _maxValue = value(1);
  if (value(2)<_minValue) _minValue = value(2);
  if (value(2)>_maxValue) _maxValue = value(2);
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::plotVertex( int index, double* values, int numberOfValues ) {
  assertion(_lastWriteCommandVertexNumber>=-1);
  assertion(numberOfValues<=_recordsPerVertex);

  for( int i=0; i<numberOfValues; i++) {
    assertion1( values[i] != std::numeric_limits<double>::infinity(), values[i] );
    assertion1( values[i] == values[i], values[i] );  // test for not a number
  }

  while (_lastWriteCommandVertexNumber<index-1) {
    plotVertex(_lastWriteCommandVertexNumber+1,0.0);
  }

  _lastWriteCommandVertexNumber = index;

  for( int i=0; i<numberOfValues; i++) {
    _out << values[i] << " ";
  }
  for (int i=numberOfValues; i<_recordsPerVertex; i++) {
    _out << 0.0 << " ";
  }

  for( int i=0; i<numberOfValues; i++) {
    if (values[i]<_minValue) _minValue = values[i];
    if (values[i]>_maxValue) _maxValue = values[i];
  }
}


double tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::getMinValue() const {
  return _minValue;
}


double tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter::getMaxValue() const {
  return _maxValue;
}
