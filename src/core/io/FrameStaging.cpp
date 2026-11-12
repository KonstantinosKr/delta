/*
 * FrameStaging.cpp
 *
 * The one .vtu / .pvd writer. Every model - 3D particles, the AMReX space
 * decomposition, 2D dem2d - fills a FrameStaging and calls writeVTU, so the
 * file format lives in exactly one place.
 */

#include "FrameStaging.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>

#include <glob.h>

#ifdef DELTA_HAVE_VTK
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>
#endif

namespace {

#ifdef DELTA_HAVE_VTK
vtkSmartPointer<vtkDataArray> makeArray(
    const delta::core::io::FrameStaging::Field& field)
{
  if (field.integer) {
    auto array = vtkSmartPointer<vtkIntArray>::New();
    array->SetNumberOfComponents(field.components);
    array->SetName(field.name.c_str());
    for (std::size_t i = 0; i < field.data.size(); ++i)
      array->InsertNextTuple1(static_cast<int>(field.data[i]));
    return array;
  }
  auto array = vtkSmartPointer<vtkDoubleArray>::New();
  array->SetNumberOfComponents(field.components);
  array->SetName(field.name.c_str());
  for (std::size_t i = 0; i < field.data.size(); ++i)
    array->InsertNextTuple1(field.data[i]);
  return array;
}
#endif

}  // namespace

void delta::core::io::FrameStaging::validate() const
{
  for (std::size_t c = 0; c < cells.size(); ++c) {
    const std::vector<long long>& ids = cells[c].second;
    if (ids.size() < 2)
      throw std::logic_error("FrameStaging: cell " + std::to_string(c) +
                             " has fewer than two points");
    for (std::size_t k = 0; k < ids.size(); ++k)
      if (ids[k] < 0 || static_cast<std::size_t>(ids[k]) >= points.size())
        throw std::logic_error("FrameStaging: cell " + std::to_string(c) +
                               " references point " + std::to_string(ids[k]) +
                               " outside [0," + std::to_string(points.size()) + ")");
  }
  for (std::size_t f = 0; f < pointFields.size(); ++f) {
    const Field& field = pointFields[f];
    const std::size_t expected = points.size() * std::size_t(field.components);
    if (field.data.size() != expected)
      throw std::logic_error("FrameStaging: point field '" + field.name + "' has " +
                             std::to_string(field.data.size()) + " values, expected " +
                             std::to_string(expected));
  }
  for (std::size_t f = 0; f < cellFields.size(); ++f) {
    const Field& field = cellFields[f];
    const std::size_t expected = cells.size() * std::size_t(field.components);
    if (field.data.size() != expected)
      throw std::logic_error("FrameStaging: cell field '" + field.name + "' has " +
                             std::to_string(field.data.size()) + " values, expected " +
                             std::to_string(expected));
  }
}

bool delta::core::io::hasVtkWriter()
{
#ifdef DELTA_HAVE_VTK
  return true;
#else
  return false;
#endif
}

#ifdef DELTA_HAVE_VTK
void delta::core::io::writeVTU(const std::string& path, const std::string& name,
                               int step, const FrameStaging& buffer)
{
  buffer.validate();

  auto points = vtkSmartPointer<vtkPoints>::New();
  points->SetDataTypeToDouble();
  for (std::size_t i = 0; i < buffer.points.size(); ++i)
    points->InsertNextPoint(buffer.points[i][0], buffer.points[i][1],
                            buffer.points[i][2]);

  auto grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
  grid->SetPoints(points);
  for (std::size_t c = 0; c < buffer.cells.size(); ++c) {
    const std::vector<long long>& ids = buffer.cells[c].second;
    std::vector<vtkIdType> vtkIds(ids.begin(), ids.end());
    grid->InsertNextCell(buffer.cells[c].first,
                         static_cast<vtkIdType>(vtkIds.size()), vtkIds.data());
  }

  for (std::size_t f = 0; f < buffer.pointFields.size(); ++f)
    grid->GetPointData()->AddArray(makeArray(buffer.pointFields[f]));
  for (std::size_t f = 0; f < buffer.cellFields.size(); ++f)
    grid->GetCellData()->AddArray(makeArray(buffer.cellFields[f]));
  if (!buffer.activeScalars.empty())
    grid->GetPointData()->SetActiveScalars(buffer.activeScalars.c_str());
  if (!buffer.activeVectors.empty())
    grid->GetPointData()->SetActiveVectors(buffer.activeVectors.c_str());

  const std::string filename = path + name + "_" + std::to_string(step) + ".vtu";
  auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
  writer->SetFileName(filename.c_str());
  writer->SetInputData(grid);
  writer->Write();

  writePVD(path, name);
}
#else
void delta::core::io::writeVTU(const std::string& path, const std::string& name,
                               int step, const FrameStaging& buffer)
{
  /* Without VTK the only emitter is the ascii one. Fall back rather than
   * producing no file, so a caller that just wants output still gets it. */
  writeVTUAscii(path, name, step, buffer);
}
#endif

/* ---------------------------------------------------------------------------
 * writeVTUAscii - the same file format, emitted by hand.
 *
 * Layout follows the .vtu XML schema for an unstructured grid in ascii mode:
 * Points, then Cells as the three parallel arrays connectivity / offsets /
 * types, then the fields. Offsets are prefix sums of the per-cell point counts;
 * types are the VTK cell-type integers, which FrameStaging already stores.
 *
 * Numeric output is %.17g so a double survives the round trip exactly; that is
 * what lets the test read the file back and compare against the staging.
 * ------------------------------------------------------------------------- */
namespace {

void writeDataArray(std::ostream& os, const char* type, const char* name,
                    int components, const std::vector<double>& data, bool integer)
{
  os << "        <DataArray type=\"" << type << "\"";
  if (name && name[0]) os << " Name=\"" << name << "\"";
  if (components != 1) os << " NumberOfComponents=\"" << components << "\"";
  os << " format=\"ascii\">\n          ";
  char buf[64];
  for (std::size_t i = 0; i < data.size(); ++i) {
    if (integer)
      std::snprintf(buf, sizeof(buf), "%d", static_cast<int>(data[i]));
    else
      std::snprintf(buf, sizeof(buf), "%.17g", data[i]);
    os << buf;
    os << (((i + 1) % components == 0) ? '\n' : ' ');
    if ((i + 1) % components == 0 && i + 1 < data.size()) os << "          ";
  }
  os << "        </DataArray>\n";
}

}  // namespace

void delta::core::io::writeVTUAscii(const std::string& path, const std::string& name,
                                    int step, const FrameStaging& buffer)
{
  buffer.validate();

  const std::string filename = path + name + "_" + std::to_string(step) + ".vtu";
  std::ofstream os(filename.c_str());
  if (!os) return;

  os << "<?xml version=\"1.0\"?>\n"
        "<VTKFile type=\"UnstructuredGrid\" version=\"1.0\" byte_order=\"LittleEndian\">\n"
        "  <UnstructuredGrid>\n"
        "    <Piece NumberOfPoints=\"" << buffer.points.size()
     << "\" NumberOfCells=\"" << buffer.cells.size() << "\">\n";

  /* --- points --------------------------------------------------------- */
  os << "      <Points>\n";
  {
    std::vector<double> flat;
    flat.reserve(buffer.points.size() * 3);
    for (std::size_t i = 0; i < buffer.points.size(); ++i)
      for (int d = 0; d < 3; ++d) flat.push_back(buffer.points[i][d]);
    writeDataArray(os, "Float64", "", 3, flat, false);
  }
  os << "      </Points>\n";

  /* --- cells ----------------------------------------------------------- */
  os << "      <Cells>\n";
  {
    std::vector<double> connectivity, offsets, types;
    long long running = 0;
    for (std::size_t c = 0; c < buffer.cells.size(); ++c) {
      const std::vector<long long>& ids = buffer.cells[c].second;
      for (std::size_t k = 0; k < ids.size(); ++k) connectivity.push_back(double(ids[k]));
      running += (long long)ids.size();
      offsets.push_back(double(running));
      types.push_back(double(buffer.cells[c].first));
    }
    writeDataArray(os, "Int64", "connectivity", 1, connectivity, true);
    writeDataArray(os, "Int64", "offsets",      1, offsets,      true);
    writeDataArray(os, "UInt8", "types",        1, types,        true);
  }
  os << "      </Cells>\n";

  /* --- fields ---------------------------------------------------------- */
  if (!buffer.pointFields.empty()) {
    os << "      <PointData";
    if (!buffer.activeScalars.empty()) os << " Scalars=\"" << buffer.activeScalars << "\"";
    if (!buffer.activeVectors.empty()) os << " Vectors=\"" << buffer.activeVectors << "\"";
    os << ">\n";
    for (std::size_t f = 0; f < buffer.pointFields.size(); ++f) {
      const FrameStaging::Field& field = buffer.pointFields[f];
      writeDataArray(os, field.integer ? "Int32" : "Float64", field.name.c_str(),
                     field.components, field.data, field.integer);
    }
    os << "      </PointData>\n";
  }
  if (!buffer.cellFields.empty()) {
    os << "      <CellData>\n";
    for (std::size_t f = 0; f < buffer.cellFields.size(); ++f) {
      const FrameStaging::Field& field = buffer.cellFields[f];
      writeDataArray(os, field.integer ? "Int32" : "Float64", field.name.c_str(),
                     field.components, field.data, field.integer);
    }
    os << "      </CellData>\n";
  }

  os << "    </Piece>\n"
        "  </UnstructuredGrid>\n"
        "</VTKFile>\n";
  os.close();

  writePVD(path, name);
}

void delta::core::io::writePVD(const std::string& path, const std::string& name)
{
  const std::string prefix = name + "_";
  const std::string suffix = ".vtu";

  std::vector<std::pair<int, std::string> > frames;

  glob_t matches;
  if (glob((path + prefix + "*" + suffix).c_str(), 0, nullptr, &matches) == 0) {
    for (size_t i = 0; i < matches.gl_pathc; ++i) {
      std::string full(matches.gl_pathv[i]);
      if (full.size() < path.size() + prefix.size() + suffix.size()) continue;
      std::string base   = full.substr(path.size());
      std::string digits = base.substr(prefix.size(),
                                       base.size() - prefix.size() - suffix.size());
      frames.push_back(std::make_pair(std::atoi(digits.c_str()), base));
    }
  }
  globfree(&matches);

  std::sort(frames.begin(), frames.end());

  std::ofstream pvd(path + name + ".pvd");
  if (!pvd) return;
  pvd << "<?xml version=\"1.0\"?>\n"
         "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
         "  <Collection>\n";
  char timestep[64];
  for (size_t i = 0; i < frames.size(); ++i) {
    std::snprintf(timestep, sizeof(timestep), "%.6g", double(frames[i].first));
    pvd << "    <DataSet timestep=\"" << timestep
        << "\" group=\"\" part=\"0\" file=\"" << frames[i].second << "\"/>\n";
  }
  pvd << "  </Collection>\n"
         "</VTKFile>\n";
}
