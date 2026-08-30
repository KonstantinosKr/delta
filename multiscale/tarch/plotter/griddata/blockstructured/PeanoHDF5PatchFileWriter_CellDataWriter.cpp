#include "tarch/plotter/griddata/blockstructured/PeanoHDF5PatchFileWriter.h"



tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter::CellDataWriter::CellDataWriter(
  const std::string& identifier,
  int                numberOfUnknowns,
  const std::string& metaData,
  double*            mapping,
  tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter& writer
):
  _writer(writer),
  _identifier(identifier),
  _numberOfUnknowns(numberOfUnknowns) {
  #ifdef HDF5
  /**
   * Create scalar attribute.
   */
  hid_t numberOfUnknownsDataSpace = H5Screate(H5S_SCALAR);
  hid_t numberOfUnknownsAttribute = H5Acreate(
    _writer._file, (_writer.getNameOfCurrentDataset()+"/celldata/numberofunknowns/"+_identifier).c_str(), H5T_NATIVE_INT,
      numberOfUnknownsDataSpace, H5P_DEFAULT, H5P_DEFAULT
    );

  /**
   * Write scalar attribute.
   */
  H5Awrite(numberOfUnknownsAttribute, H5T_NATIVE_INT, &numberOfUnknowns);

  H5Aclose(numberOfUnknownsAttribute);
  H5Sclose(numberOfUnknownsDataSpace);
  #endif

  if (!metaData.empty()) {
    #ifdef HDF5
    hid_t metaDataAttribute = H5Screate(H5S_SCALAR);
    hid_t metaDataType      = H5Tcopy(H5T_C_S1);

    H5Tset_size(metaDataType, metaData.size());
    H5Tset_strpad(metaDataType,H5T_STR_NULLTERM);
    hid_t attribute = H5Acreate2(
      _writer._file,
      (_writer.getNameOfCurrentDataset()+"/celldata/metadata/"+_identifier).c_str(),
      metaDataType, metaDataAttribute, H5P_DEFAULT, H5P_DEFAULT);

    /*
     * Write string attribute.
     */
    H5Awrite(attribute, metaDataType, metaData.c_str());

    /*
     * Close attribute and file dataspaces, and datatype.
     */
    H5Aclose(attribute);
    H5Sclose(metaDataAttribute);
    #endif
  }

  if (mapping!=nullptr) {
    #ifdef HDF5
    //
    // Create the data space with unlimited dimensions.
    //
    hsize_t tableDimensions[] = {
      _writer._dimensions,
      std::pow(_writer._numberOfCellsPerAxis,_writer._dimensions)
    };

    //
    // Set up handles/tables
    //
    hid_t dataTable = H5Screate_simple(2, tableDimensions, NULL);
    hid_t dataset   = H5Dcreate(
      _writer._file,
      (_writer.getNameOfCurrentDataset()+"/celldata/mapping/"+_identifier).c_str(),
      H5T_NATIVE_DOUBLE,
      dataTable, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT
    );

    //
    // Write data
    //
    H5Dwrite(
      dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
      mapping
    );

    //
    // Close/release handles
    //
    H5Dclose(dataset);
    H5Sclose(dataTable);
    #endif
  }
}


tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter::CellDataWriter::~CellDataWriter() {
}


void tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter::CellDataWriter::plotCell( int index, double value ) {
  while (static_cast<int>(_data.size())<(index+1)*_numberOfUnknowns) {
    _data.resize( (index+1)*_numberOfUnknowns );
  }

  _data[index*_numberOfUnknowns] = value;

  for (int i=1; i<_numberOfUnknowns; i++) {
    _data[index*_numberOfUnknowns+i] = 0.0;
  }
}


void tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter::CellDataWriter::plotCell( int index, const tarch::la::Vector<2,double>& value ) {
  while (static_cast<int>(_data.size())<(index+1)*_numberOfUnknowns) {
    _data.resize( (index+1)*_numberOfUnknowns );
  }

  _data[index*_numberOfUnknowns+0] = value(0);
  _data[index*_numberOfUnknowns+1] = value(1);

  for (int i=2; i<_numberOfUnknowns; i++) {
    _data[index*_numberOfUnknowns+i] = 0.0;
  }
}


void tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter::CellDataWriter::plotCell( int index, const tarch::la::Vector<3,double>& value ) {
  while (static_cast<int>(_data.size())<(index+1)*_numberOfUnknowns) {
    _data.resize( (index+1)*_numberOfUnknowns );
  }

  _data[index*_numberOfUnknowns+0] = value(0);
  _data[index*_numberOfUnknowns+1] = value(1);
  _data[index*_numberOfUnknowns+2] = value(2);

  for (int i=3; i<_numberOfUnknowns; i++) {
    _data[index*_numberOfUnknowns+i] = 0.0;
  }
}


void tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter::CellDataWriter::plotCell( int index, double* values, int numberOfValues ) {
  while (static_cast<int>(_data.size())<(index+1)*_numberOfUnknowns) {
    _data.resize( (index+1)*_numberOfUnknowns );
  }

  for (int i=0; i<numberOfValues; i++) {
    _data[index*_numberOfUnknowns+i] = values[i];
  }

  for (int i=numberOfValues; i<_numberOfUnknowns; i++) {
    _data[index*_numberOfUnknowns+i] = 0.0;
  }
}


void tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter::CellDataWriter::close() {
  assignRemainingCellsDefaultValues();

  #ifdef HDF5
  const int lineLenght = std::pow(_writer._numberOfCellsPerAxis,_writer._dimensions);

  assertion6(
    _data.size()%lineLenght == 0,
    _data.size()%lineLenght, _identifier,
    _data.size(), lineLenght, _writer._numberOfCellsPerAxis, _writer._dimensions
  );

  assertion5( _data.size()%lineLenght == 0, _data.size(), lineLenght, _writer._numberOfCellsPerAxis, _writer._dimensions, _identifier);

  logDebug( "close()", "create data table of " << _identifier << " with " << _data.size() << " entries");

  //
  // Create the data space with unlimited dimensions.
  //
  hsize_t tableDimensions[] = {
    lineLenght,
    static_cast<int>(_data.size())/lineLenght
  };

  //
  // Set up handles/tables
  //
  hid_t dataTable = H5Screate_simple(2, tableDimensions, NULL);
  hid_t dataset   = H5Dcreate(
    _writer._file,
    (_writer.getNameOfCurrentDataset()+"/celldata/data/"+_identifier).c_str(),
    H5T_NATIVE_DOUBLE,
    dataTable, H5P_DEFAULT,
    _writer.createDataTableProperties(lineLenght,static_cast<int>(_data.size())/lineLenght), H5P_DEFAULT
  );

  logDebug( "close()", "dump data of " << _identifier );

  //
  // Write data
  //
  H5Dwrite(
    dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
    _data.data()
  );

  //
  // Close/release handles
  //
  H5Dclose(dataset);
  H5Sclose(dataTable);
  #endif
}




void tarch::plotter::griddata::blockstructured::PeanoHDF5PatchFileWriter::CellDataWriter::assignRemainingCellsDefaultValues() {
  while (static_cast<int>(_data.size())<_writer._cellCounter*_numberOfUnknowns) {
    _data.push_back(0.0);
  }
}

