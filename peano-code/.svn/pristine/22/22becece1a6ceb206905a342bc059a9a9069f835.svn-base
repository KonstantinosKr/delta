#include "tarch/plotter/griddata/unstructured/vtk/tests/VTKBinaryFileTestCase.h"


#include "tarch/plotter/griddata/unstructured/vtk/VTKBinaryFileWriter.h"


#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/tests/TestCaseFactory.h"
registerIntegrationTest(tarch::plotter::griddata::unstructured::vtk::tests::VTKBinaryFileTestCase)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif

 
tarch::plotter::griddata::unstructured::vtk::tests::VTKBinaryFileTestCase::VTKBinaryFileTestCase():
  tarch::tests::TestCase( "tarch::plotter::griddata::unstructured::vtk::tests::VTKBinaryFileTestCase" ) {
}


tarch::plotter::griddata::unstructured::vtk::tests::VTKBinaryFileTestCase::~VTKBinaryFileTestCase() {
}


void tarch::plotter::griddata::unstructured::vtk::tests::VTKBinaryFileTestCase::run() {
  testMethod( testHexahedron );
}


void tarch::plotter::griddata::unstructured::vtk::tests::VTKBinaryFileTestCase::testHexahedron() {

  VTKBinaryFileWriter writer;

  tarch::la::Vector<3,double> p;
  tarch::plotter::griddata::unstructured::UnstructuredGridWriter::VertexWriter* vertexWriter = writer.createVertexWriter();
  p = 0.0, 0.0, 0.0;
  vertexWriter->plotVertex(p);
  p = 1.0, 0.0, 0.0;
  vertexWriter->plotVertex(p);
  p = 0.0, 1.0, 0.0;
  vertexWriter->plotVertex(p);
  p = 1.0, 1.0, 0.0;
  vertexWriter->plotVertex(p);
  p = 0.0, 0.0, 1.0;
  vertexWriter->plotVertex(p);
  p = 1.0, 0.0, 1.0;
  vertexWriter->plotVertex(p);
  p = 0.0, 1.0, 1.0;
  vertexWriter->plotVertex(p);
  p = 1.0, 1.0, 1.0;
  vertexWriter->plotVertex(p);
  vertexWriter->close();
  delete vertexWriter;

  int v[8];
  tarch::plotter::griddata::unstructured::UnstructuredGridWriter::CellWriter* cellWriter = writer.createCellWriter();
  v[0] = 0;
  v[1] = 1;
  v[2] = 2;
  v[3] = 3;
  v[4] = 4;
  v[5] = 5;
  v[6] = 6;
  v[7] = 7;
  cellWriter->plotHexahedron(v);
  cellWriter->close();
  delete cellWriter;

  tarch::plotter::griddata::Writer::VertexDataWriter* vertexDataWriter = writer.createVertexDataWriter("one", 1);
  vertexDataWriter->plotVertex(0,0.0);
  vertexDataWriter->plotVertex(1,1.0);
  vertexDataWriter->plotVertex(2,2.0);
  vertexDataWriter->plotVertex(3,3.0);
  vertexDataWriter->plotVertex(4,4.0);
  vertexDataWriter->plotVertex(5,5.0);
  vertexDataWriter->plotVertex(6,6.0);
  vertexDataWriter->plotVertex(7,7.0);
  vertexDataWriter->close();
  delete vertexDataWriter;

  vertexDataWriter = writer.createVertexDataWriter("two", 2);
  tarch::la::Vector<2,double> d;
  d = -10.0, 99.0;
  vertexDataWriter->plotVertex(0,d);
  d = -11.0, 98.0;
  vertexDataWriter->plotVertex(1,d);
  d = -12.0, 97.0;
  vertexDataWriter->plotVertex(2,d);
  d = -13.0, 96.0;
  vertexDataWriter->plotVertex(3,d);
  d = -14.0, 95.0;
  vertexDataWriter->plotVertex(4,d);
  d = -15.0, 94.0;
  vertexDataWriter->plotVertex(5,d);
  d = -16.0, 93.0;
  vertexDataWriter->plotVertex(6,d);
  d = -17.0, 92.0;
  vertexDataWriter->plotVertex(7,d);
  vertexDataWriter->close();
  delete vertexDataWriter;

  tarch::plotter::griddata::Writer::CellDataWriter* cellDataWriter = writer.createCellDataWriter("cell", 1);
  cellDataWriter->plotCell(0,20.0);
  cellDataWriter->close();
  delete cellDataWriter;

  writer.writeToFile( _outputDirectory + "/VTKBinaryFileTestCase_testHexahedron.vtk" );
}


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
