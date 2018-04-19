#include <delta/geometry/defined/graphite.h>

//Intact Poly Brick
delta::geometry::mesh::Mesh *delta::geometry::defined::generateBrickFB(
  iREAL center[3], iREAL h)
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  char fileinput[100] = "input/bricksmall.vtk";
  delta::geometry::mesh::Mesh *mesh = delta::core::readSingleVTKGeometry(fileinput);

  iREAL centerOfGeometry[3];
  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->scaleXYZ(h, centerOfGeometry);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::defined::generateBrickFB()
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  char fileinput[100] = "input/bricksmall.vtk";
  delta::geometry::mesh::Mesh *mesh = delta::core::readSingleVTKGeometry(fileinput);

  iREAL center[] = {0,0,0};
  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::defined::generateBrickFB(
  iREAL center[3])
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  char fileinput[100] = "input/bricksmall.vtk";

  delta::geometry::mesh::Mesh *mesh = delta::core::readSingleVTKGeometry(fileinput);

  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->moveMeshFromOriginToPosition(center);

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

  delta::geometry::mesh::Mesh *mesh = delta::core::readSingleVTKGeometry(fileinput);

  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->scaleXYZ(h, centerOfGeometry);
  mesh->moveMeshFromOriginToPosition(center);

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

  delta::geometry::mesh::Mesh *mesh = delta::core::readSingleVTKGeometry(fileinput);

  iREAL centerOfGeometry[3];
  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->scaleXYZ(h, centerOfGeometry);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}
