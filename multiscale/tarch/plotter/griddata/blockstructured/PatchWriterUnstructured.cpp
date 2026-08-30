#include "tarch/plotter/griddata/blockstructured/PatchWriterUnstructured.h"


tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::PatchWriterUnstructured(tarch::plotter::griddata::unstructured::UnstructuredGridWriter*  writer):
  _writer(writer),
  _vertexWriter(0),
  _cellWriter(0) {
  _vertexWriter = _writer->createVertexWriter();
  _cellWriter   = _writer->createCellWriter();
}


tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::~PatchWriterUnstructured() {
  if (_vertexWriter!=0) {
    _vertexWriter->close();
    delete _vertexWriter;
    _vertexWriter = 0;
  }

  if (_cellWriter!=0) {
    _cellWriter->close();
    delete _cellWriter;
    _cellWriter   = 0;
  }

  delete _writer;
}


bool tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::writeToFile( const std::string& filename ) {
  assertionMsg( _vertexWriter==nullptr, "call close() on patch writer before" );
  assertionMsg( _cellWriter==nullptr,   "call close() on patch writer before" );

  return _writer->writeToFile( filename );
}


bool tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::isOpen() {
  return _writer->isOpen();
}


void tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::clear() {
  _writer->clear();
}


tarch::plotter::griddata::blockstructured::PatchWriter::SinglePatchWriter*   tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::createSinglePatchWriter() {
  return new tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::SinglePatchWriter(*this);
}


tarch::plotter::griddata::Writer::CellDataWriter*    tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::createCellDataWriter( const std::string& identifier, int recordsPerCell ) {
  return _writer->createCellDataWriter(identifier,recordsPerCell);
}


tarch::plotter::griddata::Writer::VertexDataWriter*  tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::createVertexDataWriter( const std::string& identifier, int recordsPerVertex ) {
  return _writer->createVertexDataWriter(identifier,recordsPerVertex);
}


tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::SinglePatchWriter::SinglePatchWriter(
    tarch::plotter::griddata::blockstructured::PatchWriterUnstructured& base
):
  _base(base) {
}


tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::SinglePatchWriter::~SinglePatchWriter() {
}


std::pair<int,int> tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::SinglePatchWriter::plotPatch(
  const tarch::la::Vector<2,double>& offset,
  const tarch::la::Vector<2,double>& size,
  const tarch::la::Vector<2,int>&    cells
) {
  int firstVertex = -1;

  for (int y=0; y<cells(1)+1; y++)
  for (int x=0; x<cells(0)+1; x++) {
    tarch::la::Vector<2, double> p;

    p(0) = offset(0) + x * size(0) / cells(0);
    p(1) = offset(1) + y * size(1) / cells(1);

    const int newVertexNumber = _base._vertexWriter->plotVertex(p);
    firstVertex = firstVertex==-1 ? newVertexNumber : firstVertex;
  }

  int firstCell = -1;

  for (int y=0; y<cells(1); y++)
  for (int x=0; x<cells(0); x++) {
    int cellsVertexIndices[4];
    cellsVertexIndices[0] = firstVertex + (x+0) + (y+0) * (cells(0)+1);
    cellsVertexIndices[1] = firstVertex + (x+1) + (y+0) * (cells(0)+1);
    cellsVertexIndices[2] = firstVertex + (x+0) + (y+1) * (cells(0)+1);
    cellsVertexIndices[3] = firstVertex + (x+1) + (y+1) * (cells(0)+1);
    const int newCellNumber = _base._cellWriter->plotQuadrangle(cellsVertexIndices);
    firstCell = firstCell==-1 ? newCellNumber : firstCell;
  }

  return std::pair<int,int>(firstVertex,firstCell);
}


std::pair<int,int> tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::SinglePatchWriter::plotPatch(
  const tarch::la::Vector<3,double>& offset,
  const tarch::la::Vector<3,double>& size,
  const tarch::la::Vector<3,int>&    cells
) {
  int firstVertex = -1;

  assertion3( size(0)>0.0, offset, size, cells );
  assertion3( size(1)>0.0, offset, size, cells );
  assertion3( size(2)>0.0, offset, size, cells );

  assertion3( cells(0)>0.0, offset, size, cells );
  assertion3( cells(1)>0.0, offset, size, cells );
  assertion3( cells(2)>0.0, offset, size, cells );

  for (int z=0; z<cells(2)+1; z++)
  for (int y=0; y<cells(1)+1; y++)
  for (int x=0; x<cells(0)+1; x++) {
    tarch::la::Vector<3, double> p;

    p(0) = offset(0) + x * size(0) / cells(0);
    p(1) = offset(1) + y * size(1) / cells(1);
    p(2) = offset(2) + z * size(2) / cells(2);

    const int newVertexNumber = _base._vertexWriter->plotVertex(p);
    firstVertex = firstVertex==-1 ? newVertexNumber : firstVertex;
  }

  int firstCell = -1;

  for (int z=0; z<cells(2); z++)
  for (int y=0; y<cells(1); y++)
  for (int x=0; x<cells(0); x++) {
    int cellsVertexIndices[8];
    cellsVertexIndices[0] = firstVertex + (x+0) + (y+0) * (cells(0)+1) + (z+0) * (cells(0)+1) * (cells(1)+1);
    cellsVertexIndices[1] = firstVertex + (x+1) + (y+0) * (cells(0)+1) + (z+0) * (cells(0)+1) * (cells(1)+1);
    cellsVertexIndices[2] = firstVertex + (x+0) + (y+1) * (cells(0)+1) + (z+0) * (cells(0)+1) * (cells(1)+1);
    cellsVertexIndices[3] = firstVertex + (x+1) + (y+1) * (cells(0)+1) + (z+0) * (cells(0)+1) * (cells(1)+1);
    cellsVertexIndices[4] = firstVertex + (x+0) + (y+0) * (cells(0)+1) + (z+1) * (cells(0)+1) * (cells(1)+1);
    cellsVertexIndices[5] = firstVertex + (x+1) + (y+0) * (cells(0)+1) + (z+1) * (cells(0)+1) * (cells(1)+1);
    cellsVertexIndices[6] = firstVertex + (x+0) + (y+1) * (cells(0)+1) + (z+1) * (cells(0)+1) * (cells(1)+1);
    cellsVertexIndices[7] = firstVertex + (x+1) + (y+1) * (cells(0)+1) + (z+1) * (cells(0)+1) * (cells(1)+1);
    const int newCellNumber = _base._cellWriter->plotHexahedron(cellsVertexIndices);
    firstCell = firstCell==-1 ? newCellNumber : firstCell;
  }

  return std::pair<int,int>(firstVertex,firstCell);
}


void tarch::plotter::griddata::blockstructured::PatchWriterUnstructured::SinglePatchWriter::close() {
  assertion( _base._vertexWriter!=nullptr );
  assertion( _base._cellWriter!=nullptr   );

  _base._vertexWriter->close();
  _base._cellWriter->close();

  delete _base._vertexWriter;
  delete _base._cellWriter;

  _base._vertexWriter = nullptr;
  _base._cellWriter   = nullptr;
}
