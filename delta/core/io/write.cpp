/*
 * write.cpp
 *
 *  Created on: 20 Oct 2022
 *      Author: konstantinos
 */

#include "write.h"

#include <glob.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace {

/*
 * Rewrite the ParaView collection file so all geometry_<step>.vtu files
 * currently on disk load as one animated time series instead of a single
 * frame. Stateless: it rescans the directory on every call.
 */
void writePVD(const std::string& path, const std::string& name) {
  const std::string prefix = name + "_";
  const std::string suffix = ".vtu";

  std::vector<std::pair<int, std::string>> frames;

  glob_t matches;
  if (glob((path + prefix + "*" + suffix).c_str(), 0, nullptr, &matches) == 0) {
    for (size_t i = 0; i < matches.gl_pathc; ++i) {
      std::string full(matches.gl_pathv[i]);
      if (full.size() < path.size() + prefix.size() + suffix.size()) continue;
      std::string base  = full.substr(path.size());
      std::string digits = base.substr(prefix.size(), base.size() - prefix.size() - suffix.size());
      frames.emplace_back(std::atoi(digits.c_str()), base);
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
  for (const auto& frame : frames) {
    std::snprintf(timestep, sizeof(timestep), "%.6g", double(frame.first));
    pvd << "    <DataSet timestep=\"" << timestep
        << "\" group=\"\" part=\"0\" file=\"" << frame.second << "\"/>\n";
  }
  pvd << "  </Collection>\n"
         "</VTKFile>\n";
}

}  // namespace

void delta::core::io::writeGeometryToVTKVTK(
    std::string                                     path,
    int                                             step,
    std::vector<delta::core::data::ParticleRecord>& geometries) {

  std::string filename = path + "geometry_" + std::to_string(step) + ".vtu";

  auto unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

  //Per-vertex fields. Kept on points so ParaView glyphs keep working.
  auto direction = vtkSmartPointer<vtkDoubleArray>::New();
  direction->SetNumberOfComponents(3);
  direction->SetName("velocity");

  auto radius = vtkSmartPointer<vtkDoubleArray>::New();
  radius->SetNumberOfComponents(1);
  radius->SetName("radius");

  auto epsilon = vtkSmartPointer<vtkDoubleArray>::New();
  epsilon->SetNumberOfComponents(1);
  epsilon->SetName("epsilon");

  //Per-triangle fields.
  auto globalId = vtkSmartPointer<vtkIntArray>::New();
  globalId->SetNumberOfComponents(1);
  globalId->SetName("global_id");

  auto material = vtkSmartPointer<vtkIntArray>::New();
  material->SetNumberOfComponents(1);
  material->SetName("material");

  auto points = vtkSmartPointer<vtkPoints>::New();

  vtkIdType offset = 0;

  for (auto& geometry : geometries) {
    const int   numberOfTriangles = geometry.getNumberOfTriangles();
    const iREAL rad               = geometry.getRad();
    const iREAL eps               = geometry.getEpsilon();

    auto addVertex = [&](iREAL x, iREAL y, iREAL z) {
      points->InsertNextPoint(x, y, z);
      direction->InsertNextTuple3(geometry._linearVelocity[0],
                                  geometry._linearVelocity[1],
                                  geometry._linearVelocity[2]);
      radius->InsertNextTuple1(rad);
      epsilon->InsertNextTuple1(eps);
    };

    if (numberOfTriangles == 0) {
      //Non-mesh particle (sphere): keep a point so glyph rendering still works.
      addVertex(geometry._centre[0], geometry._centre[1], geometry._centre[2]);
      ++offset;
      continue;
    }

    const int numberOfVertices = numberOfTriangles * 3;
    for (int v = 0; v < numberOfVertices; ++v) {
      addVertex(geometry._xCoordinates[v],
                geometry._yCoordinates[v],
                geometry._zCoordinates[v]);
    }

    vtkIdType triangle[3];
    for (int t = 0; t < numberOfTriangles; ++t) {
      triangle[0] = offset + t * 3 + 0;
      triangle[1] = offset + t * 3 + 1;
      triangle[2] = offset + t * 3 + 2;
      unstructuredGrid->InsertNextCell(VTK_TRIANGLE, 3, triangle);
      globalId->InsertNextTuple1(geometry.getGlobalParticleID());
      material->InsertNextTuple1(int(geometry.getMaterial()));
    }

    offset += numberOfVertices;
  }

  unstructuredGrid->SetPoints(points);
  unstructuredGrid->GetPointData()->AddArray(direction);
  unstructuredGrid->GetPointData()->AddArray(radius);
  unstructuredGrid->GetPointData()->AddArray(epsilon);
  unstructuredGrid->GetPointData()->SetActiveScalars("radius");
  unstructuredGrid->GetCellData()->AddArray(globalId);
  unstructuredGrid->GetCellData()->AddArray(material);

  auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
  writer->SetFileName(filename.c_str());
  writer->SetInputData(unstructuredGrid);
  writer->Write();

  writePVD(path, "geometry");
}

void delta::core::io::writeGridGeometryToVTKVTK(
    std::string 					path,
    int 							step,
    std::vector<std::array<iREAL, 6>> boundary)
{
    std::string filename = path + "grid_" + std::to_string(step) + ".vtu";

    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

    auto normals = vtkSmartPointer<vtkDoubleArray>::New();
    normals->SetNumberOfComponents(3);
    normals->SetName("vector123");
    double xnorm[3] = {-1., 0., 0.};

    // Points accumulate across boxes and each cell indexes its own box's eight
    // points; recreating vtkPoints per box would collapse every box onto the last.
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkIdType offset = 0;

    for(const auto& value: boundary) {
        iREAL lo[3], hi[3];

        lo[0] = value[0]; // lower corner
        lo[1] = value[1]; // lower corner
        lo[2] = value[2]; // lower corner

        hi[0] = value[3]; // upper corner
        hi[1] = value[4]; // upper corner
        hi[2] = value[5]; // upper corner

        points->InsertNextPoint(lo[0], lo[1], lo[2]); //0: A
        points->InsertNextPoint(lo[0], hi[1], lo[2]); //1: B
        points->InsertNextPoint(lo[0], hi[1], hi[2]); //2: E
        points->InsertNextPoint(lo[0], lo[1], hi[2]); //3: F

        points->InsertNextPoint(hi[0], hi[1], hi[2]); //4: H
        points->InsertNextPoint(hi[0], lo[1], hi[2]); //5: G
        points->InsertNextPoint(hi[0], lo[1], lo[2]); //6: D
        points->InsertNextPoint(hi[0], hi[1], lo[2]); //7: C

        normals->InsertNextTuple(xnorm);
        normals->InsertNextTuple(xnorm);
        normals->InsertNextTuple(xnorm);
        normals->InsertNextTuple(xnorm);

        normals->InsertNextTuple(xnorm);
        normals->InsertNextTuple(xnorm);
        normals->InsertNextTuple(xnorm);
        normals->InsertNextTuple(xnorm);


        //AB | 0->1
        //AD | 0->3
        //AG | 0->6

        //EC | 4->2
        //EH | 4->7
        //EF | 4->5

        //BC | 1->C
        //BH | 1->H
        //CD | 2->3
        //DF | 3->5
        //GH | 6->7
        //GF | 6->5

        const vtkIdType lA = offset + 0;
        const vtkIdType lB = offset + 1;
        const vtkIdType lE = offset + 2;
        const vtkIdType lF = offset + 3;
        const vtkIdType lH = offset + 4;
        const vtkIdType lG = offset + 5;
        const vtkIdType lD = offset + 6;
        const vtkIdType lC = offset + 7;

        vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lA);
        line->GetPointIds()->SetId(1, lB);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lA);
        line->GetPointIds()->SetId(1, lF);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lA);
        line->GetPointIds()->SetId(1, lD);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lB);
        line->GetPointIds()->SetId(1, lE);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lB);
        line->GetPointIds()->SetId(1, lC);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lE);
        line->GetPointIds()->SetId(1, lF);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lF);
        line->GetPointIds()->SetId(1, lG);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lD);
        line->GetPointIds()->SetId(1, lC);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lD);
        line->GetPointIds()->SetId(1, lG);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lH);
        line->GetPointIds()->SetId(1, lE);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lH);
        line->GetPointIds()->SetId(1, lC);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, lH);
        line->GetPointIds()->SetId(1, lG);
        unstructuredGrid->SetPoints(points);
        unstructuredGrid->InsertNextCell(VTK_LINE, line->GetPointIds());

        offset += 8;
    }

    unstructuredGrid->GetPointData()->SetVectors(normals);


    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(filename.c_str());
    writer->SetInputData(unstructuredGrid);
    writer->Write();

    writePVD(path, "grid");
}

void delta::core::io::writeGeometryToVTK(
    std::string 									path,
    int 											step,
    std::vector<delta::core::data::ParticleRecord>& geometries)
{
  char iter[100];
  sprintf(iter, "%u.vtk", step);
  char base[100] = "geometry_"; //care or buffer overflow
  strcat(base, iter);

  path += base;
  const char *filename = path.c_str();

  FILE *fp = fopen(filename, "w+");
  if( fp == NULL )
  {
    perror("Error while opening the file.\n");
    return;
  }

  int numVertices = 0;
  int numberOfFaces = 0;
  for(int i=0; i<geometries.size(); i++)
  {
    numVertices += geometries[i].getNumberOfTriangles()*3.0;
    numberOfFaces += geometries[i].getNumberOfTriangles();
  }

  fprintf(fp,"# vtk DataFile Version 2.0\n"
             "Output vtk file\n"
             "ASCII\n\n"
             "DATASET UNSTRUCTURED_GRID\n"
             "POINTS %i double\n", numVertices);

  std::vector<int> meshStartPivotPoint;
  for(int i=0; i<geometries.size(); i++)
  {
    for(int j = 0; j < geometries[i].getNumberOfTriangles()*3; j++)
    {
      fprintf(fp,"%.5f %.5f %.5f\n",
      geometries[i]._xCoordinates[j],
      geometries[i]._yCoordinates[j],
      geometries[i]._zCoordinates[j]);
    }
    if(i == 0)
    {
      meshStartPivotPoint.push_back(0);
    }
    else{
      meshStartPivotPoint.push_back(meshStartPivotPoint[i-1] + geometries[i].getNumberOfTriangles()*3.0);
    }
  }

  int cellPointers 	= numberOfFaces*4;
  int cellNumber 	= numberOfFaces;

  fprintf(fp,"\nCELLS %i %i\n", cellNumber, cellPointers);

  for(int i=0; i<geometries.size(); i++)
  {
    for(int j = 0; j < geometries[i].getNumberOfTriangles()*3.0; j=j+3)
    {
      unsigned int A = meshStartPivotPoint[i] + j;
      unsigned int B = meshStartPivotPoint[i] + j+1;
      unsigned int C = meshStartPivotPoint[i] + j+2;

      fprintf(fp,"3 %i %i %i\n", A, B, C);
    }
  }

  fprintf(fp,"\nCELL_TYPES %i\n", cellNumber);
  for(int i=0; i<geometries.size(); i++)
  {
    for(int j = 0; j < geometries[i].getNumberOfTriangles(); j++)
    {
      fprintf(fp,"5\n"); //triangle
    }
  }

  fclose(fp);
}

void delta::core::io::writeGridGeometryToVTK(
    std::string 						path,
    int 								step,
    std::vector<std::array<iREAL, 6>> 	boundary)
{
  char iter[100];
  sprintf(iter, "%u.vtk", step);
  char base[100] = "grid_"; //care or buffer overflow
  strcat(base, iter);

  path += base;
  const char *filename = path.c_str();

  FILE *fp = fopen(filename, "w+");
  if( fp == NULL )
  {
    perror("Error while opening the file.\n");
    return;
  }

  int numberOfBoundaries = boundary.size();
  int numVertices = 8*numberOfBoundaries;

  fprintf(fp,"# vtk DataFile Version 2.0\n"
         "Output vtk file\n"
         "ASCII\n\n"
         "DATASET UNSTRUCTURED_GRID\n"
         "POINTS %i double\n", numVertices);


  for(int i=0; i<boundary.size(); i++)
  {
    iREAL lo[3], hi[3];
    lo[0] = boundary[i][0]; // lower corner
    lo[1] = boundary[i][1]; // lower corner
    lo[2] = boundary[i][2]; // lower corner

    hi[0] = boundary[i][3]; // upper corner
    hi[1] = boundary[i][4]; // upper corner
    hi[2] = boundary[i][5]; // upper corner

    //boundary vertices
    fprintf(fp, "%.5f %.5f %.5f\n"
            "%.5f %.5f %.5f\n"
            "%.5f %.5f %.5f\n"
            "%.5f %.5f %.5f\n", 
            lo[0], lo[1], lo[2], //0: A
            lo[0], hi[1], lo[2], //1: B
            lo[0], hi[1], hi[2], //2: E
            lo[0], lo[1], hi[2]);//3: F

    fprintf(fp, "%.5f %.5f %.5f\n"
            "%.5f %.5f %.5f\n"
            "%.5f %.5f %.5f\n"
            "%.5f %.5f %.5f\n", hi[0], hi[1], hi[2], //4: H
            hi[0], lo[1], hi[2], //5: G
            hi[0], lo[1], lo[2], //6: D
            hi[0], hi[1], lo[2]);//7: C
  }

  int numberOfLines     = numberOfBoundaries*12;
  int cellPointers      = numberOfLines*3;
  int cellNumber 		= numberOfLines;

  fprintf(fp,"\nCELLS %i %i\n", cellNumber, cellPointers);

  for(int i=0; i<boundary.size(); i++)
  {
    //AB | 0->1
    //AD | 0->3
    //AG | 0->6

    //EC | 4->2
    //EH | 4->7
    //EF | 4->5

    //BC | 1->C
    //BH | 1->H
    //CD | 2->3
    //DF | 3->5
    //GH | 6->7
    //GF | 6->5

    int lA = i*8 + 0;
    int lB = i*8 + 1;
    int lE = i*8 + 2;
    int lF = i*8 + 3;
    int lH = i*8 + 4;
    int lG = i*8 + 5;
    int lD = i*8 + 6;
    int lC = i*8 + 7;

    fprintf(fp, "2 %i %i\n", lA, lB);
    fprintf(fp, "2 %i %i\n", lA, lF);
    fprintf(fp, "2 %i %i\n", lA, lD);

    fprintf(fp, "2 %i %i\n", lB, lE);
    fprintf(fp, "2 %i %i\n", lB, lC);

    fprintf(fp, "2 %i %i\n", lE, lF);
    fprintf(fp, "2 %i %i\n", lF, lG);

    fprintf(fp, "2 %i %i\n", lD, lC);
    fprintf(fp, "2 %i %i\n", lD, lG);

    fprintf(fp, "2 %i %i\n", lH, lE);
    fprintf(fp, "2 %i %i\n", lH, lC);
    fprintf(fp, "2 %i %i\n", lH, lG);
  }

  fprintf(fp,"\nCELL_TYPES %i\n", cellNumber);
  //write line faces
  for(int j = 0; j < numberOfLines; j++)
  {
    fprintf(fp, "3\n"); //lines
  }

  fclose(fp);
}

void delta::core::io::writeScenarioSpecification(std::string fileName)
{

}

