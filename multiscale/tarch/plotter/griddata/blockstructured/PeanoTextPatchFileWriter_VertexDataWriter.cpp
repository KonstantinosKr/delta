#include "PeanoTextPatchFileWriter.h"



tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::VertexDataWriter(
  const std::string& identifier,
  int                numberOfUnknowns,
  const std::string& metaData,
  double*            mapping,
  tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter& writer
):
  _writer(writer),
  _identifier(identifier),
  _numberOfUnknowns(numberOfUnknowns),
  _patchCounter(0) {
  _writer._snapshotFileOut << "begin vertex-values \"" << identifier << "\"" << std::endl
                           << "  number-of-unknowns " << _numberOfUnknowns << std::endl;

  if (!metaData.empty()) {
    _writer._snapshotFileOut << "  begin meta-data" << std::endl
                             << "    " << metaData << std::endl
                             << "  end meta-data" << std::endl;
  }

  if (mapping!=nullptr) {
    _writer._snapshotFileOut << "  begin mapping" << std::endl;
    for (int i=0; i<_writer.getVerticesPerPatch() * _writer._dimensions; i++) {
      _writer._snapshotFileOut << " " << mapping[i];
    }
    _writer._snapshotFileOut << std::endl << "  end mapping" << std::endl;
  }

  _writer._snapshotFileOut << "end vertex-values" << std::endl << std::endl;
}


tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::~VertexDataWriter() {
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::plotVertex( int index, double value ) {
  assertion( !std::isnan(value) );
  _out << " " << value;
  for (int i=1; i<_numberOfUnknowns; i++) {
    _out << " 0";
  }
  _patchCounter++;
  flushIfPatchIsComplete();
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::plotVertex( int index, const tarch::la::Vector<2,double>& value ) {
  assertion( !std::isnan(value(0)) );
  assertion( !std::isnan(value(1)) );
  _out << " " << value(0) << " " << value(1);
  for (int i=2; i<_numberOfUnknowns; i++) {
    _out << " 0";
  }
  _patchCounter++;
  flushIfPatchIsComplete();
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::plotVertex( int index, const tarch::la::Vector<3,double>& value ) {
  assertion( !std::isnan(value(0)) );
  assertion( !std::isnan(value(1)) );
  assertion( !std::isnan(value(2)) );
  _out << " " << value(0) << " " << value(1) << " " << value(2);
  for (int i=3; i<_numberOfUnknowns; i++) {
    _out << " 0";
  }
  _patchCounter++;
  flushIfPatchIsComplete();
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::plotVertex( int index, double* values, int numberOfValues ) {
  for (int i=0; i<numberOfValues; i++) {
    _out << " " << values[i];
    assertion( !std::isnan(values[i]) );
  }
  for (int i=numberOfValues; i<_numberOfUnknowns; i++) {
    _out << " 0";
  }
  _patchCounter++;
  flushIfPatchIsComplete();
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::close() {
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::assignRemainingVerticesDefaultValues() {
  if (_patchCounter>=_writer.getVerticesPerPatch()) {
    _out << std::flush;
    _writer._snapshotFileOut << "  begin vertex-values \"" << _identifier << "\"" << std::endl
                 << "    " << _out.rdbuf() << std::endl
                 << "  end vertex-values" << std::endl;
    _out.clear();
    _patchCounter = 0;
  }
}


void tarch::plotter::griddata::blockstructured::PeanoTextPatchFileWriter::VertexDataWriter::flushIfPatchIsComplete() {
  if (_patchCounter>=_writer.getVerticesPerPatch()) {
    _out << std::flush;
    _writer._snapshotFileOut << "  begin vertex-values \"" << _identifier << "\"" << std::endl
                 << "    " << _out.rdbuf() << std::endl
                 << "  end vertex-values" << std::endl;
    _out.clear();
    _patchCounter = 0;
  }
}
