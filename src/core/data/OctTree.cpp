/*
 * OctTree.cpp
 *
 *  Created on: 16 Jul 2018
 *      Author: konstantinos
 */

#include "OctTree.h"
#include <cmath>

namespace delta {
  namespace core {
	namespace data {

	OctTree::OctTree() {

	}

	OctTree::OctTree(
		iREAL maxMeshSize,
		std::array<iREAL, 6> domain)
	{
      std::array<iREAL, 3> minpoint = {domain[0], domain[1], domain[2]};
	  std::array<iREAL, 3> maxpoint = {domain[3], domain[4], domain[5]};

	  //compute coarse level dimensions widths
	  iREAL xw = std::fabs(maxpoint[0]-minpoint[0]);//source/parent width
	  iREAL yw = std::fabs(maxpoint[1]-minpoint[1]);
	  iREAL zw = std::fabs(maxpoint[2]-minpoint[2]);

	  _domain = domain;
	  //target refinement length cannot be smaller than then largest triangle (maybe 2 x bigger is ideal)
	  if(xw < maxMeshSize || yw < maxMeshSize || zw < maxMeshSize)
	  {//stop refinement if current cell width is smaller than one specified
		A = nullptr;
		B = nullptr;
		C = nullptr;
		D = nullptr;

		E = nullptr;
		F = nullptr;
		G = nullptr;
		H = nullptr;
		_isLeaf = true;
		return;
	  }
	  else //else refine
	  {
		_isLeaf = false;
	  }

	  ////////////////////////////////////////////////////////////////////////////////
	  std::array<iREAL, 3> midpoint = {minpoint[0]+xw/2, minpoint[1]+yw/2, minpoint[2]+zw/2};

	  //get fine level bounding boxes
	  //////////////////No1 -A- BACK LEFT BOX////////////////////////////////////////
	  std::array<iREAL, 3> minpointA = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]};
	  std::array<iREAL, 3> maxpointA = {midpoint[0], maxpoint[1], midpoint[2]};
	  std::array<iREAL, 6> boundaryA = {minpointA[0], minpointA[1], minpointA[2], maxpointA[0], maxpointA[1], maxpointA[2]};

	  std::array<iREAL, 3> minpointB = {minpoint[0], minpoint[1], minpoint[2]};
	  std::array<iREAL, 3> maxpointB = {midpoint[0], maxpoint[1]-(yw/2), midpoint[2]};
	  std::array<iREAL, 6> boundaryB = {minpointB[0], minpointB[1], minpointB[2], maxpointB[0], maxpointB[1], maxpointB[2]};

	  //get fine level bounding boxes
	  //////////////////No2 -B- FRONT LEFT BOX////////////////////////////////////////
	  std::array<iREAL, 3> minpointC = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
	  std::array<iREAL, 3> maxpointC = {midpoint[0], midpoint[1]+(yw/2), midpoint[2]+(zw/2)};
	  std::array<iREAL, 6> boundaryC = {minpointC[0], minpointC[1], minpointC[2], maxpointC[0], maxpointC[1], maxpointC[2]};

	  std::array<iREAL, 3> minpointD = {minpoint[0], minpoint[1], minpoint[2]+(zw/2)};
	  std::array<iREAL, 3> maxpointD = {midpoint[0], midpoint[1], midpoint[2]+(zw/2)};
	  std::array<iREAL, 6> boundaryD = {minpointD[0], minpointD[1], minpointD[2], maxpointD[0], maxpointD[1], maxpointD[2]};

	  //get fine level bounding boxes
	  //////////////////No3 -C- BACK RIGHT BOX////////////////////////////////////////
	  std::array<iREAL, 3> minpointE = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]};
	  std::array<iREAL, 3> maxpointE = {maxpoint[0], maxpoint[1], maxpoint[2]-(zw/2)};
	  std::array<iREAL, 6> boundaryE = {minpointE[0], minpointE[1], minpointE[2], maxpointE[0], maxpointE[1], maxpointE[2]};

	  std::array<iREAL, 3> minpointF = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]};
	  std::array<iREAL, 3> maxpointF = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]-(zw/2)};
	  std::array<iREAL, 6> boundaryF = {minpointF[0], minpointF[1], minpointF[2], maxpointF[0], maxpointF[1], maxpointF[2]};

	  //get fine level bounding boxes
	  /////////////////No4 -D- FRONT RIGHT BOX////////////////////////////////////////
	  std::array<iREAL, 3> minpointG = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
	  std::array<iREAL, 3> maxpointG = maxpoint;
	  std::array<iREAL, 6> boundaryG = {minpointG[0], minpointG[1], minpointG[2], maxpointG[0], maxpointG[1], maxpointG[2]};

	  std::array<iREAL, 3> minpointH = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]+(zw/2)};
	  std::array<iREAL, 3> maxpointH = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]};
	  std::array<iREAL, 6> boundaryH = {minpointH[0], minpointH[1], minpointH[2], maxpointH[0], maxpointH[1], maxpointH[2]};
	  ////////////////////////////////////////////////////////////////////////////////

	  A = new OctTree(maxMeshSize, boundaryA);
	  B = new OctTree(maxMeshSize, boundaryB);
	  C = new OctTree(maxMeshSize, boundaryC);
	  D = new OctTree(maxMeshSize, boundaryD);

	  E = new OctTree(maxMeshSize, boundaryE);
	  F = new OctTree(maxMeshSize, boundaryF);
	  G = new OctTree(maxMeshSize, boundaryG);
	  H = new OctTree(maxMeshSize, boundaryH);
	}

	OctTree::OctTree(
		iREAL maxMeshSize,
		std::array<iREAL, 6> domain,
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates)
	{
	  refine(maxMeshSize, domain,
			 xCoordinates, yCoordinates, zCoordinates);
	}

	void OctTree::refine(
		iREAL maxMeshSize,
		std::array<iREAL, 6> domain,
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates)
	{
	  std::array<iREAL, 3> minpoint = {domain[0], domain[1], domain[2]};
	  std::array<iREAL, 3> maxpoint = {domain[3], domain[4], domain[5]};

	  //compute coarse level dimensions widths
	  iREAL xw = std::fabs(maxpoint[0]-minpoint[0]);//parent width
	  iREAL yw = std::fabs(maxpoint[1]-minpoint[1]);
	  iREAL zw = std::fabs(maxpoint[2]-minpoint[2]);

	  _isLeaf = false;
	  _domain = domain;
	  //target refinement length cannot be smaller than then largest triangle (maybe 2 x bigger is ideal)
	  if(xw < maxMeshSize || yw < maxMeshSize || zw < maxMeshSize)
	  {//stop refinement if current cell width is smaller than one specified
		A = nullptr;
		B = nullptr;
		C = nullptr;
		D = nullptr;

		E = nullptr;
		F = nullptr;
		G = nullptr;
		H = nullptr;
		_isLeaf = true;
		return;
	  }

	  ////////////////////////////////////////////////////////////////////////////////
	  std::array<iREAL, 3> midpoint = {minpoint[0]+xw/2, minpoint[1]+yw/2, minpoint[2]+zw/2};

	  //get fine level bounding boxes
	  //////////////////No1 -A- BACK LEFT BOX////////////////////////////////////////
	  std::array<iREAL, 3> minpointA = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]};
	  std::array<iREAL, 3> maxpointA = {midpoint[0], maxpoint[1], midpoint[2]};
	  std::array<iREAL, 6> boundaryA = {minpointA[0], minpointA[1], minpointA[2], maxpointA[0], maxpointA[1], maxpointA[2]};

	  std::array<iREAL, 3> minpointB = {minpoint[0], minpoint[1], minpoint[2]};
	  std::array<iREAL, 3> maxpointB = {midpoint[0], maxpoint[1]-(yw/2), midpoint[2]};
	  std::array<iREAL, 6> boundaryB = {minpointB[0], minpointB[1], minpointB[2], maxpointB[0], maxpointB[1], maxpointB[2]};

	  //get fine level bounding boxes
	  //////////////////No2 -B- FRONT LEFT BOX////////////////////////////////////////
	  std::array<iREAL, 3> minpointC = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
	  std::array<iREAL, 3> maxpointC = {midpoint[0], midpoint[1]+(yw/2), midpoint[2]+(zw/2)};
	  std::array<iREAL, 6> boundaryC = {minpointC[0], minpointC[1], minpointC[2], maxpointC[0], maxpointC[1], maxpointC[2]};

	  std::array<iREAL, 3> minpointD = {minpoint[0], minpoint[1], minpoint[2]+(zw/2)};
	  std::array<iREAL, 3> maxpointD = {midpoint[0], midpoint[1], midpoint[2]+(zw/2)};
	  std::array<iREAL, 6> boundaryD = {minpointD[0], minpointD[1], minpointD[2], maxpointD[0], maxpointD[1], maxpointD[2]};

	  //get fine level bounding boxes
	  //////////////////No3 -C- BACK RIGHT BOX////////////////////////////////////////
	  std::array<iREAL, 3> minpointE = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]};
	  std::array<iREAL, 3> maxpointE = {maxpoint[0], maxpoint[1], maxpoint[2]-(zw/2)};
	  std::array<iREAL, 6> boundaryE = {minpointE[0], minpointE[1], minpointE[2], maxpointE[0], maxpointE[1], maxpointE[2]};

	  std::array<iREAL, 3> minpointF = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]};
	  std::array<iREAL, 3> maxpointF = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]-(zw/2)};
	  std::array<iREAL, 6> boundaryF = {minpointF[0], minpointF[1], minpointF[2], maxpointF[0], maxpointF[1], maxpointF[2]};

	  //get fine level bounding boxes
	  /////////////////No4 -D- FRONT RIGHT BOX////////////////////////////////////////
	  std::array<iREAL, 3> minpointG = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
	  std::array<iREAL, 3> maxpointG = maxpoint;
	  std::array<iREAL, 6> boundaryG = {minpointG[0], minpointG[1], minpointG[2], maxpointG[0], maxpointG[1], maxpointG[2]};

	  std::array<iREAL, 3> minpointH = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]+(zw/2)};
	  std::array<iREAL, 3> maxpointH = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]};
	  std::array<iREAL, 6> boundaryH = {minpointH[0], minpointH[1], minpointH[2], maxpointH[0], maxpointH[1], maxpointH[2]};
	  ////////////////////////////////////////////////////////////////////////////////

	  if(isPartiallyInDomain(boundaryA, xCoordinates, yCoordinates, zCoordinates))
	  {
		A = new OctTree(maxMeshSize, boundaryA, xCoordinates, yCoordinates, zCoordinates);
	  } else
	  {
		A = nullptr;
		_isLeaf = true;
	  }
	  if(isPartiallyInDomain(boundaryB, xCoordinates, yCoordinates, zCoordinates))
	  {
		B = new OctTree(maxMeshSize, boundaryB, xCoordinates, yCoordinates, zCoordinates);
	  } else
	  {
		B = nullptr;
		_isLeaf = true;
	  }
	  if(isPartiallyInDomain(boundaryC, xCoordinates, yCoordinates, zCoordinates))
	  {
		C = new OctTree(maxMeshSize, boundaryC, xCoordinates, yCoordinates, zCoordinates);
	  } else
	  {
		C = nullptr;
		_isLeaf = true;
	  }
	  if(isPartiallyInDomain(boundaryD, xCoordinates, yCoordinates, zCoordinates))
	  {
		D = new OctTree(maxMeshSize, boundaryD, xCoordinates, yCoordinates, zCoordinates);
	  } else
	  {
		D = nullptr;
		_isLeaf = true;
	  }


	  if(isPartiallyInDomain(boundaryE, xCoordinates, yCoordinates, zCoordinates))
	  {
		E = new OctTree(maxMeshSize, boundaryE, xCoordinates, yCoordinates, zCoordinates);
	  } else
	  {
		E = nullptr;
		_isLeaf = true;
	  }
	  if(isPartiallyInDomain(boundaryF, xCoordinates, yCoordinates, zCoordinates))
	  {
		F = new OctTree(maxMeshSize, boundaryF, xCoordinates, yCoordinates, zCoordinates);
	  } else
	  {
		F = nullptr;
		_isLeaf = true;
	  }
	  if(isPartiallyInDomain(boundaryG, xCoordinates, yCoordinates, zCoordinates))
	  {
		G = new OctTree(maxMeshSize, boundaryG, xCoordinates, yCoordinates, zCoordinates);
	  } else
	  {
		G = nullptr;
		_isLeaf = true;
	  }
	  if(isPartiallyInDomain(boundaryH, xCoordinates, yCoordinates, zCoordinates))
	  {
		H = new OctTree(maxMeshSize, boundaryH, xCoordinates, yCoordinates, zCoordinates);
	  } else
	  {
		H = nullptr;
		_isLeaf = true;
	  }
	}

	std::array<iREAL, 6> OctTree::getBoundary()
	{
	  return _domain;
	}

	void OctTree::getBoundary(
		iREAL x[3],
		std::array<iREAL, 6>& childDomain)
	{
	  if(OctTree::isInDomain(x) && !_isLeaf)
	  {
		A->getBoundary(x, childDomain);
		B->getBoundary(x, childDomain);
		C->getBoundary(x, childDomain);
		D->getBoundary(x, childDomain);

		E->getBoundary(x, childDomain);
		F->getBoundary(x, childDomain);
		G->getBoundary(x, childDomain);
		H->getBoundary(x, childDomain);
	  } else if(OctTree::isInDomain(x) && _isLeaf)
	  {
		childDomain =  _domain;
	  }
	}

	void OctTree::getBoundary(
		std::vector<std::array<iREAL, 6>> &childDomain)
	{
	  if(A != nullptr)
	  A->getBoundary(childDomain);
	  if(B != nullptr)
	  B->getBoundary(childDomain);
	  if(C != nullptr)
	  C->getBoundary(childDomain);
	  if(D != nullptr)
	  D->getBoundary(childDomain);

	  if(E != nullptr)
	  E->getBoundary(childDomain);
	  if(F != nullptr)
	  F->getBoundary(childDomain);
	  if(G != nullptr)
	  G->getBoundary(childDomain);
	  if(H != nullptr)
	  H->getBoundary(childDomain);
	  if(_isLeaf)
	  {
		childDomain.push_back(_domain);
	  }
	}

	bool OctTree::isInDomain(iREAL x[3])
	{
	  bool xInside = false;
	  bool yInside = false;
	  bool zInside = false;

	  if(x[0] > _domain[0] && x[0] < _domain[3])
	  {
		xInside = true;
	  }

	  if(x[1] > _domain[1] && x[1] < _domain[4])
	  {
		yInside = true;
	  }

	  if(x[2] > _domain[2] && x[2] < _domain[5])
	  {
		zInside = true;
	  }

	  bool inside = false;
	  if(xInside && yInside && zInside)
	  {
		inside = true;
	  }

	  return inside;
	}

	bool OctTree::isPartiallyInDomain(
		std::array<iREAL,6> boundingBox,
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates)
	{
	  for(unsigned i=0; i<xCoordinates.size(); i+=3)
	  {
		iREAL x[3] = {xCoordinates[i], yCoordinates[i], zCoordinates[i]};
		iREAL y[3] = {xCoordinates[i+1], yCoordinates[i+1], zCoordinates[i+1]};
		iREAL z[3] = {xCoordinates[i+2], yCoordinates[i+2], zCoordinates[i+2]};

		if(	delta::contact::detection::isInBox(x, boundingBox) == true ||
			delta::contact::detection::isInBox(y, boundingBox) == true ||
			delta::contact::detection::isInBox(z, boundingBox) == true)
		{
		  return true;
		}
	  }
	  return false;
	}

	void OctTree::getPartialMesh(
		std::array<iREAL,6> boundingBox,
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates,
		std::vector<iREAL>& xPartialCoordinates,
		std::vector<iREAL>& yPartialCoordinates,
		std::vector<iREAL>& zPartialCoordinates)
	{
	  for(unsigned i=0; i<xCoordinates.size(); i+=3)
	  {
		iREAL x[3] = {xCoordinates[i], yCoordinates[i], zCoordinates[i]};
		iREAL y[3] = {xCoordinates[i+1], yCoordinates[i+1], zCoordinates[i+1]};
		iREAL z[3] = {xCoordinates[i+2], yCoordinates[i+2], zCoordinates[i+2]};

		if(	delta::contact::detection::isInBox(x, boundingBox) == true ||
			delta::contact::detection::isInBox(y, boundingBox) == true ||
			delta::contact::detection::isInBox(z, boundingBox) == true)
		{
		  xPartialCoordinates.push_back(xCoordinates[i]);
		  yPartialCoordinates.push_back(yCoordinates[i]);
		  zPartialCoordinates.push_back(zCoordinates[i]);

		  xPartialCoordinates.push_back(xCoordinates[i+1]);
		  yPartialCoordinates.push_back(yCoordinates[i+1]);
		  zPartialCoordinates.push_back(zCoordinates[i+1]);

		  xPartialCoordinates.push_back(xCoordinates[i+2]);
		  yPartialCoordinates.push_back(yCoordinates[i+2]);
		  zPartialCoordinates.push_back(zCoordinates[i+2]);
		}
	  }
	}

	void OctTree::getBBOXOverlappedMesh(
		  double x[3], double epsilon,
		  std::vector<iREAL> &xCoordinates,
		  std::vector<iREAL> &yCoordinates,
		  std::vector<iREAL> &zCoordinates,
		  std::vector<iREAL> &xCoordinatesPartial,
		  std::vector<iREAL> &yCoordinatesPartial,
		  std::vector<iREAL> &zCoordinatesPartial)
	{
	  if(A != nullptr)
	  A->getBBOXOverlappedMesh(x, epsilon,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
			  xCoordinatesPartial,
			  yCoordinatesPartial,
			  zCoordinatesPartial);
	  if(B != nullptr)
	  B->getBBOXOverlappedMesh(x, epsilon,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
			  xCoordinatesPartial,
			  yCoordinatesPartial,
			  zCoordinatesPartial);
	  if(C != nullptr)
	  C->getBBOXOverlappedMesh(x, epsilon,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
			  xCoordinatesPartial,
			  yCoordinatesPartial,
			  zCoordinatesPartial);
	  if(D != nullptr)
	  D->getBBOXOverlappedMesh(x, epsilon,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
			  xCoordinatesPartial,
			  yCoordinatesPartial,
			  zCoordinatesPartial);
	  if(E != nullptr)
	  E->getBBOXOverlappedMesh(x, epsilon,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
			  xCoordinatesPartial,
			  yCoordinatesPartial,
			  zCoordinatesPartial);
	  if(F != nullptr)
	  F->getBBOXOverlappedMesh(x, epsilon,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
			  xCoordinatesPartial,
			  yCoordinatesPartial,
			  zCoordinatesPartial);
	  if(G != nullptr)
	  G->getBBOXOverlappedMesh(x, epsilon,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
			  xCoordinatesPartial,
			  yCoordinatesPartial,
			  zCoordinatesPartial);
	  if(H != nullptr)
	  H->getBBOXOverlappedMesh(x, epsilon,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
	  		  xCoordinatesPartial,
	  		  yCoordinatesPartial,
	  		  zCoordinatesPartial);
	  if(_isLeaf)
	  {
		if(isInDomain(x))
		{
		  getPartialMesh(_domain,
			  xCoordinates,
			  yCoordinates,
			  zCoordinates,
			  xCoordinatesPartial,
			  yCoordinatesPartial,
			  zCoordinatesPartial);
		}
	  }
	}

	iREAL OctTree::getDomainLength()
	{
	  return std::sqrt(
		  (_domain[3]-_domain[0])*(_domain[3]-_domain[0]) +
		  (_domain[4]-_domain[1])*(_domain[4]-_domain[1]) +
		  (_domain[5]-_domain[2])*(_domain[5]-_domain[2]));
	}

	OctTree::~OctTree() {

	}

	} /* namespace data */
  } /* namespace core */
} /* namespace delta */
