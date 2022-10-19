/*
 * write.cpp
 *
 *  Created on: 20 Oct 2022
 *      Author: konstantinos
 */

#include "write.h"

void delta::core::io::writeGeometryToVTKVTK(
    std::string                                                             path,
    int                                                                     step,
    std::vector<delta::core::data::ParticleRecord>& geometries) {

    std::string filename = path + "geometry_" + std::to_string(step) + ".vtu";

    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

    //Properties initialisation
    auto direction = vtkSmartPointer<vtkDoubleArray>::New();
    direction->SetNumberOfComponents(3);
    direction->SetName("direction");

    auto radius = vtkSmartPointer<vtkDoubleArray>::New();
    radius->SetNumberOfComponents(1);
    radius->SetName("radius");
    
    auto epsilon = vtkSmartPointer<vtkDoubleArray>::New();
    epsilon->SetNumberOfComponents(1);
    epsilon->SetName("epsilon");

    /////////////////////////////////////

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    for(auto &geometry: geometries) {
        //Center
        iREAL x = geometry._centre[0];
        iREAL y = geometry._centre[1];
        iREAL z = geometry._centre[2];

        points->InsertNextPoint(x, y, z);


        double dnorm[3] = { geometry._linearVelocity[0],
                            geometry._linearVelocity[1],
                            geometry._linearVelocity[2]};

        direction->InsertNextTuple(dnorm);

        double rad = geometry.getRad();
        radius->InsertNextTuple(&rad);

        double influence = geometry.getEpsilon();
        epsilon->InsertNextTuple(&influence);
    }

    //Post initialisation, add to scene
    unstructuredGrid->SetPoints(points);
    unstructuredGrid->GetPointData()->SetVectors(direction);
    unstructuredGrid->GetPointData()->SetScalars(radius);
    unstructuredGrid->GetPointData()->SetScalars(epsilon);

    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(filename.c_str());
    writer->SetInputData(unstructuredGrid);
    writer->Write();

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

    for(const auto& value: boundary) {
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

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

        int lA = 0;
        int lB = 1;
        int lE = 2;
        int lF = 3;
        int lH = 4;
        int lG = 5;
        int lD = 6;
        int lC = 7;

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
    }

    unstructuredGrid->GetPointData()->SetVectors(normals);


    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(filename.c_str());
    writer->SetInputData(unstructuredGrid);
    writer->Write();

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

