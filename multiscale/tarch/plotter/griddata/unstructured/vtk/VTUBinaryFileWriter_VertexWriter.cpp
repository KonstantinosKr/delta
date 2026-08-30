#include "tarch/plotter/griddata/unstructured/vtk/VTUBinaryFileWriter.h"
#include "tarch/plotter/ByteSwap.h"

#include <iomanip>

tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::VertexWriter::VertexWriter(VTUBinaryFileWriter& writer, std::string dataType):
  _dataType(dataType),
  _currentVertexNumber(0),
  _myWriter(writer),
  _out() {
  assertion( _myWriter._numberOfVertices==0 );
}


tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::VertexWriter::~VertexWriter() {
  if (_currentVertexNumber>=0) {
    close();
  }
}



int tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::VertexWriter::plotVertex(const tarch::la::Vector<2,double>& position) {
  assertion1( _currentVertexNumber>=0, _currentVertexNumber );

  tarch::la::Vector<3,double> p;
  p(0) = position(0);
  p(1) = position(1);
  p(2) = 0.0;

  return plotVertex(p);
}


int tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::VertexWriter::plotVertex(const tarch::la::Vector<3,double>& position) {
  assertion( _currentVertexNumber>=0 );

  assertion1( position(0)==position(0), position );
  assertion1( position(1)==position(1), position );
  assertion1( position(2)==position(2), position );

  _currentVertexNumber++;

  if (_myWriter._dataType.compare( "Float32" ) ==0 ){
    float tmp;
    tmp = position(0);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = position(1);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = position(2);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  } else {
    double tmp;
    tmp = position(0);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = position(1);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
    tmp = position(2);
    tmp = byteSwapForParaviewBinaryFiles(tmp);
    _out.write( reinterpret_cast<char*>(&tmp) , sizeof(tmp));
  }

  return _currentVertexNumber-1;
}


void tarch::plotter::griddata::unstructured::vtk::VTUBinaryFileWriter::VertexWriter::close() {
  assertion( _myWriter._numberOfVertices==0 );
  assertionMsg( _myWriter.isOpen(), "Maybe you forgot to call close() on a data writer before you destroy your writer?" );

  _myWriter._numberOfVertices  = _currentVertexNumber;
  _myWriter._vertexDescription << "<Points><DataArray type=\"" + _dataType + "\" NumberOfComponents=\"3\" format=\"binary\">"
                               << _out.str()
                               << "</DataArray></Points>";
  _currentVertexNumber         = -1;
}
