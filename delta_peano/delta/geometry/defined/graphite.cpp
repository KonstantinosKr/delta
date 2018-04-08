#include <delta/geometry/defined/graphite.h>

//Intact Poly Brick
delta::geometry::mesh::Mesh *delta::geometry::defined::generateBrickFB(
  iREAL center[3], iREAL h)
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  char fileinput[100] = "input/bricksmall.vtk";
  delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::mesh::Mesh *mesh =
	  new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);

  iREAL centerOfGeometry[3];
  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->scaleXYZ(h, centerOfGeometry);
  mesh->moveMeshFromOriginToPosition(center);

  mesh->flatten(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::defined::generateBrickFB()
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  char fileinput[100] = "input/bricksmall.vtk";
  delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::mesh::Mesh *mesh =
	  new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);

  iREAL center[] = {0,0,0};
  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->moveMeshFromOriginToPosition(center);

  mesh->flatten(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::defined::generateBrickFB(
  iREAL center[3])
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  char fileinput[100] = "input/bricksmall.vtk";
  delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::mesh::Mesh *mesh =
	  new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);

  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->moveMeshFromOriginToPosition(center);

  mesh->flatten(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

//Loose keys - orientation X-wise
delta::geometry::mesh::Mesh *delta::geometry::defined::generateKeyXK(
  iREAL center[3], iREAL h)
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  char fileinput[100] = "input/keysmall.vtk";
  delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::mesh::Mesh *mesh =
	  new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);

  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->scaleXYZ(h, centerOfGeometry);
  mesh->moveMeshFromOriginToPosition(center);

  mesh->flatten(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

//Loose keys - orientation Y-wise
delta::geometry::mesh::Mesh *delta::geometry::defined::generateKeyYK(
  iREAL center[3], iREAL h)
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  char fileinput[100] = "input/keysmall.vtk";
  delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::mesh::Mesh *mesh =
	  new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);

  iREAL centerOfGeometry[3];
  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->scaleXYZ(h, centerOfGeometry);
  mesh->moveMeshFromOriginToPosition(center);

  mesh->flatten(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}
