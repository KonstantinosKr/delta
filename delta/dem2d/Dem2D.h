/*
 The MIT License (MIT)

 Copyright (c) 2022 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef DELTA_DEM2D_DEM2D_H_
#define DELTA_DEM2D_DEM2D_H_

#include <array>
#include <utility>
#include <vector>

#include "Vec2.h"

namespace delta {
  namespace dem2d {

/*
 * A plane DEM core with two rigid element kinds:
 *
 *   Circle   - a disk, described by a radius.
 *   Triangle - a rigid convex polygon with three vertices, described in the
 *              body frame relative to its centroid.
 *
 * Everything is rigid-body soft-contact: the same spring-dashpot normal law
 * and Coulomb-limited tangential law used by the 3D core, with a scalar
 * moment of inertia instead of a 3x3 tensor and a scalar angle instead of a
 * rotation matrix.
 */
enum class ShapeType { Circle, Triangle };

struct Particle2D
{
  int        id      = 0;
  ShapeType  shape   = ShapeType::Circle;

  /* Circle geometry */
  iREAL      radius  = 0.0;

  /* Triangle geometry, body frame, centred on the centroid. */
  std::array<Vec2, 3> localVertices;

  /* Cached world frame triangle, refreshed by the engine before detection. */
  std::array<Vec2, 3> worldVertices;

  /* Bounding radius about the centroid: radius for a disk, circumradius for a
   * triangle. The broad phase reach of a pair is boundingRadiusA + boundingRadiusB. */
  iREAL      boundingRadius = 0.0;

  /* Broad-phase skin, lets a pair be collected slightly before it overlaps.
   * It never enters the force law: force is zero at zero overlap. */
  iREAL      epsilon = 0.0;

  /* Rigid body state. position is the disk centre / triangle centroid. */
  Vec2       position;
  Vec2       velocity;
  iREAL      angle           = 0.0;
  iREAL      angularVelocity = 0.0;

  /* Inertia. invMass == 0 marks a static obstacle. */
  iREAL      mass       = 1.0;
  iREAL      inertia    = 1.0;
  iREAL      invMass    = 1.0;
  iREAL      invInertia = 1.0;

  iREAL      friction   = 0.0;
  bool       isObstacle = false;
};

/* A resolved overlap. normal is unit length and points from a towards b;
 * depth is the overlap, positive while the elements interpenetrate. */
struct Contact2D
{
  int    a      = -1;
  int    b      = -1;
  Vec2   normal;
  iREAL  depth  = 0.0;
  Vec2   point;
};

struct ContactParams2D
{
  iREAL stiffness          = 2.0e5;   /* normal spring, matches the 3D SSPRING */
  iREAL dampingRatio       = 0.5;     /* matches the 3D SDAMPER */
  iREAL tangentialStiffness = 1.0e5;  /* viscous tangent, capped by Coulomb */
  Vec2  gravity            = Vec2(0.0, -9.8);
};

/* ---------------------------------------------------------------------------
 * factories
 * ------------------------------------------------------------------------- */

Particle2D makeCircle(int id, const Vec2& centre, iREAL radius, iREAL density,
                      iREAL friction, iREAL epsilon = 0.0);

/* Vertices are given in the world frame; the centroid becomes the body
 * position and the vertices are re-expressed relative to it. */
Particle2D makeTriangle(int id, const std::array<Vec2, 3>& vertices, iREAL density,
                        iREAL friction, iREAL epsilon = 0.0);

/* Freeze a particle in place: infinite mass and inertia, obstacle flag set. */
void makeStatic(Particle2D& particle);

/* ---------------------------------------------------------------------------
 * contact routines, one per element pairing, exposed for direct testing.
 * Each returns true and fills `out` when the pair overlaps. Normals always
 * follow the a -> b convention of the argument order.
 * ------------------------------------------------------------------------- */

bool circleCircle(const Particle2D& a, const Particle2D& b, Contact2D& out);
bool circleTriangle(const Particle2D& circle, const Particle2D& triangle, Contact2D& out);
bool triangleTriangle(const Particle2D& a, const Particle2D& b, Contact2D& out);

/* Shape dispatch. Note that circleTriangle is defined for (circle, triangle)
 * order, so a (triangle, circle) argument pair flips the normal. */
bool collide(const Particle2D& a, const Particle2D& b, Contact2D& out);

/* ---------------------------------------------------------------------------
 * engine
 * ------------------------------------------------------------------------- */

class Engine2D
{
public:
  Engine2D();
  explicit Engine2D(std::vector<Particle2D> particles,
                    const ContactParams2D& params = ContactParams2D());

  void step(iREAL dt);

  std::vector<Particle2D>&       particles()       { return _particles; }
  const std::vector<Particle2D>& particles() const { return _particles; }
  const std::vector<Contact2D>&  contacts()  const { return _contacts; }

  ContactParams2D&       params()       { return _params; }
  const ContactParams2D& params() const { return _params; }

  /* Uniform-cell broad phase, the 2D analogue of the 3D binnedCandidates(). */
  std::vector<std::pair<int, int> > binnedCandidates() const;

  /* O(N^2) reference used by the tests to prove the broad phase loses nothing. */
  std::vector<std::pair<int, int> > bruteForceCandidates() const;

  /* Resolve contacts using the broad phase (what step() does). */
  void contactDetection();

  /* Resolve contacts by exhaustive scanning, reference oracle for the tests. */
  void bruteForceContacts(std::vector<Contact2D>& out) const;

  /* Refresh worldVertices from position/angle. */
  void refreshWorldVertices();

private:
  void deriveForces();
  void integrate(iREAL dt);

  std::vector<Particle2D> _particles;
  std::vector<Contact2D>  _contacts;
  ContactParams2D         _params;
  std::vector<Vec2>       _force;
  std::vector<iREAL>      _torque;
};

    }
  }

#endif /* DELTA_DEM2D_DEM2D_H_ */
