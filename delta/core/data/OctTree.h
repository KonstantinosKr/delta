/*
 * OctTree.h
 *
 *  Created on: 16 Jul 2018
 *      Author: konstantinos
 */

#ifndef DELTA_CORE_DATA_OCTTREE_H_
#define DELTA_CORE_DATA_OCTTREE_H_

#include <vector>
#include <array>
#include <contact/detection/box.h>

namespace delta {
  namespace core {
	namespace data {

	  class OctTree;

	} /* namespace data */
  } /* namespace core */
} /* namespace delta */

class delta::core::data::OctTree {

  public:
	OctTree();
	OctTree(iREAL maxMeshSize, std::array<iREAL, 6> domain);
	OctTree(
		iREAL maxMeshSize,
		std::array<iREAL, 6> domain,
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	void refine(
		iREAL maxMeshSize,
		std::array<iREAL, 6> domain,
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	std::array<iREAL, 6> getBoundary();
	void getBoundBoxes(std::vector<std::array<iREAL, 6>> &boxes);
	void getBoundary(iREAL x[3], std::array<iREAL, 6>& childDomain);
	void getBoundary(std::vector<std::array<iREAL, 6>> &childDomain);
	bool isInDomain(iREAL x[3]);

	void getPartialMesh(
		std::array<iREAL,6> boundingBox,
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates,
		std::vector<iREAL>& xPartialCoordinates,
		std::vector<iREAL>& yPartialCoordinates,
		std::vector<iREAL>& zPartialCoordinates);

	void getBBOXOverlappedMesh(
		  double x[3], double epsilon,
		  std::vector<iREAL> &xCoordinates,
		  std::vector<iREAL> &yCoordinates,
		  std::vector<iREAL> &zCoordinates,
		  std::vector<iREAL> &xCoordinatesPartial,
		  std::vector<iREAL> &yCoordinatesPartial,
		  std::vector<iREAL> &zCoordinatesPartial);

	iREAL getDomainLength();

	virtual ~OctTree();
  private:

	bool isPartiallyInDomain(
		std::array<iREAL,6> boundingBox,
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	std::array<iREAL, 6> _domain;
	bool _isLeaf;

	std::vector<iREAL> _xcluster;
	std::vector<iREAL> _ycluster;
	std::vector<iREAL> _zcluster;

	OctTree *A;
	OctTree *B;
	OctTree *C;
	OctTree *D;

	OctTree *E;
	OctTree *F;
	OctTree *G;
	OctTree *H;
};
#endif /* DELTA_CORE_DATA_OCTTREE_H_ */
