#include <dim3/geometry/loaders/graphite.h>

//Intact Poly Brick
delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateBrickFB(
  iREAL center[3], iREAL h)
{
  const std::string fileinput = delta::core::io::inputPath("bricksmall.stl");
  delta::geometry::mesh::Mesh *mesh = delta::core::io::readPartGeometry(fileinput);

  iREAL centerOfGeometry[3];
  mesh->computeCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->scaleXYZ(h);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateBrickFB()
{
  const std::string fileinput = delta::core::io::inputPath("bricksmall.stl");
  delta::geometry::mesh::Mesh *mesh = delta::core::io::readPartGeometry(fileinput);

  iREAL center[] = {0,0,0};
  iREAL centerOfGeometry[3];

  mesh->computeCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateBrickFB(
  iREAL center[3])
{
  const std::string fileinput = delta::core::io::inputPath("bricksmall.stl");

  delta::geometry::mesh::Mesh *mesh = delta::core::io::readPartGeometry(fileinput);

  iREAL centerOfGeometry[3];

  mesh->computeCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->rotateX(-0.25);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

//Loose keys - orientation X-wise
delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateKeyXK(
  iREAL center[3], iREAL h)
{
  const std::string fileinput = delta::core::io::inputPath("keysmall.stl");

  delta::geometry::mesh::Mesh *mesh = delta::core::io::readPartGeometry(fileinput);

  iREAL centerOfGeometry[3];

  mesh->computeCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->scaleXYZ(h);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}

//Loose keys - orientation Y-wise
delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateKeyYK(
  iREAL center[3], iREAL h)
{
  const std::string fileinput = delta::core::io::inputPath("keysmall.stl");

  delta::geometry::mesh::Mesh *mesh = delta::core::io::readPartGeometry(fileinput);

  iREAL centerOfGeometry[3];
  mesh->computeCenterOfGeometry(centerOfGeometry);
  mesh->moveMeshFromPositionToOrigin(centerOfGeometry);
  mesh->scaleXYZ(h);
  mesh->moveMeshFromOriginToPosition(center);

  return mesh;
}
