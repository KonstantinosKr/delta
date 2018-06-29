#include <delta/geometry/hardcoded/graphite.h>

//Intact Poly Brick
delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateBrickFB(
  iREAL center[3], iREAL h)
{
  char fileinput[100] = "input/bricksmall.vtk";
  delta::geometry::mesh::Mesh *mesh = delta::core::readVTKGeometry(fileinput);

  iREAL centerOfGeometry[3];
  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->scaleXYZ(h);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateBrickFB()
{
  char fileinput[100] = "input/bricksmall.vtk";
  delta::geometry::mesh::Mesh *mesh = delta::core::readVTKGeometry(fileinput);

  iREAL center[] = {0,0,0};
  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateBrickFB(
  iREAL center[3])
{
  char fileinput[100] = "input/bricksmall.vtk";

  delta::geometry::mesh::Mesh *mesh = delta::core::readVTKGeometry(fileinput);

  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

//Loose keys - orientation X-wise
delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateKeyXK(
  iREAL center[3], iREAL h)
{
  char fileinput[100] = "input/keysmall.vtk";

  delta::geometry::mesh::Mesh *mesh = delta::core::readVTKGeometry(fileinput);

  iREAL centerOfGeometry[3];

  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->scaleXYZ(h);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

//Loose keys - orientation Y-wise
delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateKeyYK(
  iREAL center[3], iREAL h)
{
  char fileinput[100] = "input/keysmall.vtk";

  delta::geometry::mesh::Mesh *mesh = delta::core::readVTKGeometry(fileinput);

  iREAL centerOfGeometry[3];
  mesh->getCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->scaleXYZ(h);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}
