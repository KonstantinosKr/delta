#include "tarch/plotter/pointdata/vtk/VTKTextFileWriter.h"



tarch::plotter::pointdata::vtk::VTKTextFileWriter::VTKTextFileWriter():
  _vtkWriter(),
  _vertexWriter(_vtkWriter.createVertexWriter()),
  _cellWriter( _vtkWriter.createCellWriter()) {
}



tarch::plotter::pointdata::vtk::VTKTextFileWriter::~VTKTextFileWriter() {
  assertionMsg( _vertexWriter==0, "no close() called" );
  assertionMsg( _cellWriter==0, "no close() called" );
}


tarch::plotter::pointdata::Writer::PointDataWriter*    tarch::plotter::pointdata::vtk::VTKTextFileWriter::createPointDataWriter( const std::string& identifier, int recordsPerPoint ) {
  return new PointDataWriter(*this,identifier,recordsPerPoint);
}


bool tarch::plotter::pointdata::vtk::VTKTextFileWriter::writeToFile( const std::string& filename ) {
  return _vtkWriter.writeToFile(filename);
}


bool tarch::plotter::pointdata::vtk::VTKTextFileWriter::isOpen() {
  return _vtkWriter.isOpen();
}


void tarch::plotter::pointdata::vtk::VTKTextFileWriter::clear() {
  _vtkWriter.clear();
}


void tarch::plotter::pointdata::vtk::VTKTextFileWriter::close() {
  _vertexWriter->close();
  _cellWriter->close();

  delete _vertexWriter;
  delete _cellWriter;

  _vertexWriter            = 0;
  _cellWriter              = 0;
}


int tarch::plotter::pointdata::vtk::VTKTextFileWriter::plotPoint(const tarch::la::Vector<2,double>& position) {
  const int particleNumber = _vertexWriter->plotVertex(position);
  _cellWriter->plotPoint(particleNumber);
  return particleNumber;
}


int tarch::plotter::pointdata::vtk::VTKTextFileWriter::plotPoint(const tarch::la::Vector<3,double>& position) {
  const int particleNumber = _vertexWriter->plotVertex(position);
  _cellWriter->plotPoint(particleNumber);
  return particleNumber;
}


tarch::plotter::pointdata::vtk::VTKTextFileWriter::PointDataWriter::PointDataWriter(VTKTextFileWriter& myWriter, const std::string& identifier, int recordsPerPoint):
  _myWriter(myWriter),
  _particleDataWriter(_myWriter._vtkWriter.createVertexDataWriter(identifier,recordsPerPoint)) {
}


tarch::plotter::pointdata::vtk::VTKTextFileWriter::PointDataWriter::~PointDataWriter() {
  delete _particleDataWriter;
}


void tarch::plotter::pointdata::vtk::VTKTextFileWriter::PointDataWriter::plot( int index, double value ) {
  _particleDataWriter->plotVertex(index,value);
}


void tarch::plotter::pointdata::vtk::VTKTextFileWriter::PointDataWriter::plot( int index, const tarch::la::Vector<2,double>& value ) {
  _particleDataWriter->plotVertex(index,value);
}


void tarch::plotter::pointdata::vtk::VTKTextFileWriter::PointDataWriter::plot( int index, const tarch::la::Vector<3,double>& value ) {
  _particleDataWriter->plotVertex(index,value);
}


void tarch::plotter::pointdata::vtk::VTKTextFileWriter::PointDataWriter::close() {
  _particleDataWriter->close();
}


void tarch::plotter::pointdata::vtk::VTKTextFileWriter::PointDataWriter::assignRemainingPointsDefaultValues() {
  _particleDataWriter->assignRemainingVerticesDefaultValues();
}
