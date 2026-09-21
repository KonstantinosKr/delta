/*
 * MeshBuffer.cpp
 *
 * The one .vtu / .pvd writer. Every model - 3D particles, the AMReX
 * decomposition grid, 2D dem2d - fills a MeshBuffer and calls writeVTU, so the
 * file format lives in exactly one place.
 */

#include "MeshBuffer.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>

#include <glob.h>

#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>

namespace {

vtkSmartPointer<vtkDataArray> makeArray(
    const delta::core::io::MeshBuffer::Field& field)
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

}  // namespace

void delta::core::io::MeshBuffer::validate() const
{
  for (std::size_t c = 0; c < cells.size(); ++c) {
    const std::vector<long long>& ids = cells[c].second;
    if (ids.size() < 2)
      throw std::logic_error("MeshBuffer: cell " + std::to_string(c) +
                             " has fewer than two points");
    for (std::size_t k = 0; k < ids.size(); ++k)
      if (ids[k] < 0 || static_cast<std::size_t>(ids[k]) >= points.size())
        throw std::logic_error("MeshBuffer: cell " + std::to_string(c) +
                               " references point " + std::to_string(ids[k]) +
                               " outside [0," + std::to_string(points.size()) + ")");
  }
  for (std::size_t f = 0; f < pointFields.size(); ++f) {
    const Field& field = pointFields[f];
    const std::size_t expected = points.size() * std::size_t(field.components);
    if (field.data.size() != expected)
      throw std::logic_error("MeshBuffer: point field '" + field.name + "' has " +
                             std::to_string(field.data.size()) + " values, expected " +
                             std::to_string(expected));
  }
  for (std::size_t f = 0; f < cellFields.size(); ++f) {
    const Field& field = cellFields[f];
    const std::size_t expected = cells.size() * std::size_t(field.components);
    if (field.data.size() != expected)
      throw std::logic_error("MeshBuffer: cell field '" + field.name + "' has " +
                             std::to_string(field.data.size()) + " values, expected " +
                             std::to_string(expected));
  }
}

void delta::core::io::writeVTU(const std::string& path, const std::string& name,
                               int step, const MeshBuffer& buffer)
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
