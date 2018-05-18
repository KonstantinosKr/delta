/*
 * mesh.cpp
 *
 *  Created on: 10 May 2018
 *      Author: konstantinos
 */

#include <delta/geometry/operators/mesh.h>

namespace delta {
  namespace geometry {
	namespace operators {
	  namespace mesh {

		void delta::geometry::operators::mesh::moveMeshFromPositionToOrigin(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
			iREAL center[3])
		{
		  for(unsigned i=0;i<xCoordinates.size();i++)
		  {
			xCoordinates[i] = xCoordinates[i]-center[0];
			yCoordinates[i] = yCoordinates[i]-center[1];
			zCoordinates[i] = zCoordinates[i]-center[2];
		  }
		}

		void delta::geometry::operators::mesh::moveMeshFromOriginToPosition(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
			iREAL center[3])
		{
		  for(unsigned i=0;i<xCoordinates.size();i++)
		  {
			xCoordinates[i] = (xCoordinates[i])+center[0];
			yCoordinates[i] = (yCoordinates[i])+center[1];
			zCoordinates[i] = (zCoordinates[i])+center[2];
		  }
		}

		void delta::geometry::operators::mesh::scaleXYZ(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
		    iREAL scale,
		    iREAL position[3])
		{
		  delta::geometry::operators::mesh::moveMeshFromPositionToOrigin(xCoordinates, yCoordinates, zCoordinates, position);

		  for(unsigned i=0;i<xCoordinates.size();i++)
		  {
			  xCoordinates[i] = xCoordinates[i]*scale;
			  yCoordinates[i] = yCoordinates[i]*scale;
			  zCoordinates[i] = zCoordinates[i]*scale;
		  }
		  delta::geometry::operators::mesh::moveMeshFromOriginToPosition(xCoordinates, yCoordinates, zCoordinates, position);
		}

		void delta::geometry::operators::mesh::rotateX(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
			iREAL alphaX)
		{
		  const iREAL pi = std::acos(-1);
		  for (unsigned i=0;i<xCoordinates.size(); i++)
		  {
			  iREAL x = xCoordinates[i];
			  iREAL y = yCoordinates[i];
			  iREAL z = zCoordinates[i];

			  iREAL M[] = {
				 1.0,                 0.0,                   0.0,
				 0.0,  std::cos(2*pi*alphaX),  std::sin(2*pi*alphaX),
				 0.0, -std::sin(2*pi*alphaX),  std::cos(2*pi*alphaX)
			  };

			  xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
			  yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
			  zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
		  }
		}

		void delta::geometry::operators::mesh::rotateY(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
			iREAL alphaY)
		{
		  const iREAL pi = std::acos(-1);
		  for (unsigned i=0;i<xCoordinates.size(); i++) {
			  iREAL x = xCoordinates[i];
			  iREAL y = yCoordinates[i];
			  iREAL z = zCoordinates[i];

			  iREAL M[] = {
				std::cos(2*pi*alphaY),  0.0, std::sin(2*pi*alphaY),
				0.0,                    1.0,                   0.0,
			   -std::sin(2*pi*alphaY),  0.0, std::cos(2*pi*alphaY)
			  };

			  xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
			  yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
			  zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
		  }
		}

		void delta::geometry::operators::mesh::rotateZ(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
			iREAL alphaZ)
		{
		  const iREAL pi = std::acos(-1);
		  for (unsigned i=0;i<xCoordinates.size(); i++) {
			  iREAL x = xCoordinates[i];
			  iREAL y = yCoordinates[i];
			  iREAL z = zCoordinates[i];

			  iREAL M[] = {
				std::cos(2*pi*alphaZ),  std::sin(2*pi*alphaZ),  0.0,
			   -std::sin(2*pi*alphaZ),  std::cos(2*pi*alphaZ),  0.0,
								 0.0,                   0.0,  1.0
			  };

			  xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
			  yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
			  zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
		  }
		}
	  }
	} /* namespace operators */
  } /* namespace geometry */
} /* namespace delta */