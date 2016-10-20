#include "delta/primitives/properties.h"
#include <stdio.h>

void delta::primitives::centerOfMass(
      const std::vector<double>&  xCoordinates,
      const std::vector<double>&  yCoordinates,
      double&                     mass,
      double&                     centreOfMassX,
      double&                     centreOfMassY
) {
  mass          = 1.0;
  centreOfMassX = 0.0;
  centreOfMassY = 0.0;
  
  int nVertices = xCoordinates.size();
  for(int i=0;i<nVertices;i++)
  {
    centreOfMassX += xCoordinates[i];
    centreOfMassY += yCoordinates[i];
  }
  centreOfMassX = centreOfMassX/(nVertices*3);
  centreOfMassY = centreOfMassY/(nVertices*3);
}

void delta::primitives::centerOfMass(
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates,
  double&               centreOfMassX,
  double&               centreOfMassY,
  double&               centreOfMassZ,
  double&               refcentreOfMassX,
  double&               refcentreOfMassY,
  double&               refcentreOfMassZ
) {

  centreOfMassX = 0.0;
  centreOfMassY = 0.0;
  centreOfMassZ = 0.0;
  
  int nVertices = xCoordinates.size();

  for(int i=0;i<nVertices;i++)
  {
    centreOfMassX += xCoordinates[i];
    centreOfMassY += yCoordinates[i];
    centreOfMassZ += zCoordinates[i];
  }

  centreOfMassX = centreOfMassX/(nVertices*3);
  centreOfMassY = centreOfMassY/(nVertices*3);
  centreOfMassZ = centreOfMassZ/(nVertices*3);

  refcentreOfMassX = centreOfMassX;
  refcentreOfMassY = centreOfMassY;
  refcentreOfMassZ = centreOfMassZ;
}

double delta::primitives::computeHMin(
  const std::vector<double>&  xCoordinates,
  const std::vector<double>&  yCoordinates,
  const std::vector<double>&  zCoordinates
)
{
  double min = 1E99;

  for(int i=0; i < xCoordinates.size(); i+=3)
  {
	double A[3], B[3], C[3];
	A[0] = xCoordinates[i];
	A[1] = yCoordinates[i];
	A[2] = zCoordinates[i];

	B[0] = xCoordinates[i+1];
	B[1] = yCoordinates[i+1];
	B[2] = zCoordinates[i+1];

	C[0] = xCoordinates[i+2];
	C[1] = yCoordinates[i+2];
	C[2] = zCoordinates[i+2];

	double AB = sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1])+(A[2]-B[2])*(A[2]-B[2]));
	double BC = sqrt((B[0]-C[0])*(B[0]-C[0])+(B[1]-C[1])*(B[1]-C[1])+(B[2]-C[2])*(B[2]-C[2]));
	double CA = sqrt((C[0]-A[0])*(C[0]-A[0])+(C[1]-A[1])*(C[1]-A[1])+(C[2]-A[2])*(C[2]-A[2]));

	if (std::min(std::min(AB, BC), CA) < min)
	{
		min = std::min(std::min(AB, BC), CA);
	}
  }
  if(min == 1E99) min = 0.0;
  return min;
}
