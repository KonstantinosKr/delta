/*
 * BodySpec.h - a named, validated way to describe one body.
 *
 * Why this exists: the Object constructors take ten positional arguments, three
 * of which are booleans (obstacle, friction, convex) and three of which are
 * std::array<iREAL,3> in a row (centre, linear, angular). A call site reads
 *
 *     Object("sphere", r, id, centre, WOOD, false, true, true, eps, lin, ang)
 *
 * which gives the compiler nothing to complain about if `lin` and `ang` are
 * swapped, and no way at all to tell which boolean is which. A BodySpec is
 * built by name and converts to Object once, at the end.
 *
 * On initial forces: an impulse applied once at t=0 is exactly a velocity
 * increment, so `impulse()` is the mechanism - there is no force field to seed,
 * because the integrator never stores one (deriveForces recomputes from contacts
 * every step). Sustained body-fixed forces are a different feature and are not
 * represented here.
 */

#ifndef DELTA_CORE_BODYSPEC_H_
#define DELTA_CORE_BODYSPEC_H_

#include <array>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/delta.h"
#include "core/io/read.h"
#include "dim3/geometry/material.h"
#include "dim3/geometry/structure/Mesh.h"
#include "dim3/world/structure/Object.h"

namespace delta {
namespace core {

class BodySpec
{
public:
  enum Shape { Sphere, Mesh };

  /* ---- factories: the two ways a body is described in practice ---- */

  /* A sphere. Its mass comes from the density of a solid sphere of `radius`. */
  static BodySpec sphere(std::array<iREAL, 3> centre, iREAL radius)
  {
    BodySpec s;
    s._shape  = Sphere;
    s._centre = centre;
    s._radius = radius;
    return s;
  }

  /* An arbitrary mesh, scaled so its bounding diagonal is `size`.
   * The file is not read until build(), so a bad path fails there, once. */
  static BodySpec mesh (const std::string& path, std::array<iREAL, 3> centre,
                        iREAL size)
  {
    BodySpec s;
    s._shape  = Mesh;
    s._centre = centre;
    s._radius = size;
    s._meshPath = path;
    return s;
  }

  /* A pre-built mesh the caller owns the lifetime of. */
  static BodySpec withMesh(delta::geometry::mesh::Mesh* mesh,
                           std::array<iREAL, 3> centre)
  {
    BodySpec s;
    s._shape = Mesh;
    s._centre = centre;
    s._mesh = mesh;
    return s;
  }

  /* ---- by-name setters ------------------------------------------- */

  BodySpec& id(int v)            { _id = v; return *this; }
  BodySpec& density(iREAL v)     { _density = v; return *this; }

  /* Freeze in place: infinite mass, no integration. */
  BodySpec& obstacle()           { _obstacle = true; return *this; }
  BodySpec& frictionless()       { _friction = false; return *this; }
  BodySpec& concave()            { _convex = false; return *this; }

  /* Contact reach beyond the geometry. Two bodies touch when the gap between
   * their surfaces is under epsilonA + epsilonB. */
  BodySpec& epsilon(iREAL v)     { _epsilon = v; return *this; }

  /* Initial velocity. Use this directly when you know the velocity. */
  BodySpec& velocity(std::array<iREAL, 3> v)        { _velocity = v; return *this; }
  BodySpec& angularVelocity(std::array<iREAL, 3> w) { _omega = w; return *this; }

  /* Initial velocity from a one-shot impulse over `duration`.
   * v0 = F * duration / mass. This is the same thing as velocity(): the
   * integrator has no force accumulator, so an impulse only exists as the
   * velocity it leaves behind. Throws if the mass would be zero. */
  BodySpec& impulse(std::array<iREAL, 3> f, iREAL duration)
  {
    /* An obstacle is integrated to zero mass, so nothing moves it and a velocity
     * set here would be silently discarded. Say so rather than pretend. */
    if (_obstacle)
    {
      throw std::logic_error("BodySpec::impulse: body is an obstacle "
                             "(infinite mass); an impulse cannot move it");
    }
    const iREAL m = mass();
    if (!(m > 0.0))
    {
      throw std::logic_error("BodySpec::impulse: body has zero mass");
    }
    _velocity = {{ f[0] * duration / m,
                   f[1] * duration / m,
                   f[2] * duration / m }};
    return *this;
  }

  /* Rotational kick from an angular impulse: w0 = tau * duration / inertia. */
  BodySpec& angularImpulse(std::array<iREAL, 3> tau, iREAL duration)
  {
    if (_obstacle)
    {
      throw std::logic_error("BodySpec::angularImpulse: body is an obstacle");
    }
    const iREAL I = inertia();
    if (!(I > 0.0))
    {
      throw std::logic_error("BodySpec::angularImpulse: body has zero inertia");
    }
    _omega = {{ tau[0] * duration / I,
                tau[1] * duration / I,
                tau[2] * duration / I }};
    return *this;
  }

  /* ---- read-back, also used by the tests ---- */

  Shape  shape()  const { return _shape; }
  int    id()     const { return _id; }
  iREAL  radius() const { return _radius; }
  bool   isObstacle() const { return _obstacle; }
  const std::array<iREAL, 3>& velocity() const { return _velocity; }

  /* Solid-sphere mass from the density. Independent of shape: a mesh body's
   * mass is the sphere of its bounding radius, which is the convention the
   * existing scenarios use (see apps/dem3d/main.cpp). */
  iREAL mass() const
  {
    const iREAL pi = 3.14159265358979323846;
    return _density * (4.0 / 3.0) * pi * _radius * _radius * _radius;
  }

  /* Solid-sphere inertia, 2/5 m r^2. */
  iREAL inertia() const { return 0.4 * mass() * _radius * _radius; }

  /* Convert to the type the engine consumes. Reads the mesh file here. */
  delta::world::structure::Object build() const
  {
    if (_density <= 0.0)
    {
      throw std::logic_error("BodySpec: density must be positive");
    }
    if (!(_radius > 0.0) && _shape == Sphere)
    {
      throw std::logic_error("BodySpec: sphere needs a positive radius");
    }

    delta::world::structure::Object o =
        (_shape == Sphere)
          ? delta::world::structure::Object(
                "sphere", _radius, _id, _centre, _material,
                _obstacle, _friction, _convex, _epsilon, _velocity, _omega)
          : delta::world::structure::Object(
                "mesh", _id, meshPointer(), _centre, _material,
                _obstacle, _friction, _convex, _epsilon, _velocity, _omega);

    o.setMass(_obstacle ? 0.0 : mass());
    return o;
  }

private:
  BodySpec()
  {
    _velocity = {{0.0, 0.0, 0.0}};
    _omega    = {{0.0, 0.0, 0.0}};
    _centre   = {{0.0, 0.0, 0.0}};
  }

  delta::geometry::mesh::Mesh* meshPointer() const
  {
    if (_mesh) return _mesh;
    if (_meshPath.empty())
    {
      throw std::logic_error("BodySpec: mesh body without a path or a mesh");
    }
    return delta::core::io::readPartGeometry(_meshPath);
  }

  Shape         _shape   = Sphere;
  int           _id      = 0;
  std::array<iREAL, 3> _centre   = {{0.0, 0.0, 0.0}};
  std::array<iREAL, 3> _velocity = {{0.0, 0.0, 0.0}};
  std::array<iREAL, 3> _omega    = {{0.0, 0.0, 0.0}};

  iREAL         _radius  = 0.0;
  iREAL         _density = 1000.0;      /* WOOD, the scenarios' default */
  iREAL         _epsilon = 0.0;
  bool          _obstacle = false;
  bool          _friction = true;
  bool          _convex   = true;

  std::string   _meshPath;
  delta::geometry::mesh::Mesh* _mesh = nullptr;
  delta::geometry::material::MaterialType _material =
      delta::geometry::material::MaterialType::WOOD;
};

}  /* namespace core */
}  /* namespace delta */

#endif /* DELTA_CORE_BODYSPEC_H_ */
