#include "tarch/plotter/ByteSwap.h"
#include "tarch/plotter/griddata/unstructured/vtk/VTUBinaryFileWriter.h"

#include <limits>
#include <iomanip>

tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::CellDataWriter(
  const std::string& dataIdentifier, VTUBinaryFileWriter& writer, int recordsPerCell, std::string dataType
):
  _dataType(dataType),
  _identifier(dataIdentifier),
  _lastWriteCommandCellNumber(-1),
  _myWriter(writer),
  _out(),
  _recordsPerCell(recordsPerCell),
  _minValue(std::numeric_limits<double>::max()),
  _maxValue(std::numeric_limits<double>::min()) {
  assertion(_recordsPerCell>0);

  _out << "  <DataArray type=\""
       << _dataType << "\" Name=\"" << _identifier
       << "\" format=\"ascii\""
       << " NumberOfComponents=\"" << _recordsPerCell << "\" >"
       << std::endl;
}


tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::~CellDataWriter() {
  if (_lastWriteCommandCellNumber>=-1) {
    close();
  }
}


void tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::assignRemainingCellsDefaultValues() {
  assertion4(
    _lastWriteCommandCellNumber <= _myWriter._numberOfCells-1,
    _lastWriteCommandCellNumber, _myWriter._numberOfCells,
    _identifier,
    "please call close on the cell/vertex writer before"
  );

  while (_lastWriteCommandCellNumber<_myWriter._numberOfCells-1) {
    plotCell(_lastWriteCommandCellNumber+1,0.0);
  }
}


void tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::close() {
  if (_myWriter._numberOfCells==0 && _lastWriteCommandCellNumber>0 ) {
    logWarning( "close()", "writer " << _identifier << " is closed though underlying grid writer does not hold any vertices/cells. Ensure you call close() on the vertex/cell writer before" );
  }
  assertion2(
    _lastWriteCommandCellNumber>-2,
    _identifier,
    "closed twice"
  );
  assertionMsg(
    _lastWriteCommandCellNumber==_myWriter._numberOfCells-1,
    "one record has to be written per cell on writer " << _identifier <<
    " (cell entries written=" <<  _lastWriteCommandCellNumber <<
    ", cell entries expected=" << _myWriter._numberOfCells << ")"
  );
  assertionMsg( _myWriter.isOpen(), "Maybe you forgot to call close() or assignRemainingCellsDefaultValues() on a data writer before you destroy your writer?" );

  if (_lastWriteCommandCellNumber>=-1) {
    _out << "</DataArray>" << std::endl;
    _myWriter._cellDataDescription << _out.str();
  }
  _lastWriteCommandCellNumber = -2;
}


void tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::plotCell( int index, double value ) {
  assertion(_lastWriteCommandCellNumber>=-1);
  assertion(1<=_recordsPerCell);

  assertion6(
    value != std::numeric_limits<double>::infinity(),
    value,
    _lastWriteCommandCellNumber,
    _recordsPerCell,
    _minValue,
    _maxValue,
    _identifier
  );


  // test for not a number
  assertion6(
    value == value,
    value,
    _lastWriteCommandCellNumber,
    _recordsPerCell,
    _minValue,
    _maxValue,
    _identifier
  );

  while (_lastWriteCommandCellNumber<index-1) {
    plotCell(_lastWriteCommandCellNumber+1,0.0);
  }

  _lastWriteCommandCellNumber = index;
  _out << value << " ";
  for (int i=1; i<_recordsPerCell; i++) {
    _out << 0.0 << " ";
  }

  _out << std::endl;

  if (value<_minValue) _minValue = value;
  if (value>_maxValue) _maxValue = value;
}


void tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::plotCell( int index, const tarch::la::Vector<2,double>& value ) {
  assertion(_lastWriteCommandCellNumber>=-1);
  assertion(2<=_recordsPerCell);

  assertion1( value(0) != std::numeric_limits<double>::infinity(), value(0) );
  assertion1( value(0) == value(0), value(0) );  // test for not a number

  assertion1( value(1) != std::numeric_limits<double>::infinity(), value(1) );
  assertion1( value(1) == value(1), value(1) );  // test for not a number

  while (_lastWriteCommandCellNumber<index-1) {
    plotCell(_lastWriteCommandCellNumber+1,0.0);
  }

  _lastWriteCommandCellNumber = index;
  _out << value(0) << " ";
  _out << value(1) << " ";
  for (int i=2; i<_recordsPerCell; i++) {
    _out << 0.0 << " ";
  }
  _out << std::endl;

  if (value(0)<_minValue) _minValue = value(0);
  if (value(0)>_maxValue) _maxValue = value(0);
  if (value(1)<_minValue) _minValue = value(1);
  if (value(1)>_maxValue) _maxValue = value(1);
}


void tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::plotCell( int index, const tarch::la::Vector<3,double>& value ) {
  assertion(_lastWriteCommandCellNumber>=-1);
  assertion(3<=_recordsPerCell);

  assertion1( value(0) != std::numeric_limits<double>::infinity(), value(0) );
  assertion1( value(0) == value(0), value(0) );  // test for not a number

  assertion1( value(1) != std::numeric_limits<double>::infinity(), value(1) );
  assertion1( value(1) == value(1), value(1) );  // test for not a number

  assertion1( value(2) != std::numeric_limits<double>::infinity(), value(2) );
  assertion1( value(2) == value(2), value(2) );  // test for not a number

  while (_lastWriteCommandCellNumber<index-1) {
    plotCell(_lastWriteCommandCellNumber+1,0.0);
  }

  _lastWriteCommandCellNumber = index;
  _out << value(0) << " ";
  _out << value(1) << " ";
  _out << value(2) << " ";
  for (int i=3; i<_recordsPerCell; i++) {
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


void tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::plotCell( int index, double* values, int numberOfValues ) {
  assertion(_lastWriteCommandCellNumber>=-1);
  assertion(numberOfValues<=_recordsPerCell);

  for( int i=0; i<numberOfValues; i++) {
    assertion1( values[i] != std::numeric_limits<double>::infinity(), values[i] );
    assertion1( values[i] == values[i], values[i] );  // test for not a number
  }

  while (_lastWriteCommandCellNumber<index-1) {
    plotCell(_lastWriteCommandCellNumber+1,0.0);
  }

  _lastWriteCommandCellNumber = index;
  for( int i=0; i<numberOfValues; i++) {
    _out << values[i] << " ";
  }
  for (int i=numberOfValues; i<_recordsPerCell; i++) {
    _out << 0.0 << " ";
  }
  _out << std::endl;

  for( int i=0; i<numberOfValues; i++) {
    if (values[i]<_minValue) _minValue = values[i];
    if (values[i]>_maxValue) _maxValue = values[i];
  }
}


double tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::getMinValue() const {
  return _minValue;
}


double tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::CellDataWriter::getMaxValue() const {
  return _maxValue;
}
