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

#include "Dem2D.h"

#include <algorithm>
#include <cmath>
#include <map>

namespace delta {
  namespace dem2d {

    namespace {

      const iREAL PI = 3.14159265358979323846;

      inline Vec2 closestOnSegment(const Vec2& p, const Vec2& q, const Vec2& c)
      {
        const Vec2  d  = q - p;
        const iREAL dd = lengthSqr(d);
        if (dd <= 0.0) return p;
        iREAL t = dot(c - p, d) / dd;
        if (t < 0.0) t = 0.0;
        if (t > 1.0) t = 1.0;
        return p + t * d;
      }

      inline bool pointInTriangle(const std::array<Vec2, 3>& v, const Vec2& p)
      {
        const iREAL d1 = cross(v[1] - v[0], p - v[0]);
        const iREAL d2 = cross(v[2] - v[1], p - v[1]);
        const iREAL d3 = cross(v[0] - v[2], p - v[2]);
        const bool  hasNeg = (d1 < 0.0) || (d2 < 0.0) || (d3 < 0.0);
        const bool  hasPos = (d1 > 0.0) || (d2 > 0.0) || (d3 > 0.0);
        return !(hasNeg && hasPos);
      }

      /* Outward unit normal of the edge (v[i], v[j]), oriented away from the
       * centroid so it does not depend on the vertex winding. */
      inline Vec2 edgeOutwardNormal(const std::array<Vec2, 3>& v, int i, int j, const Vec2& centroid)
      {
        Vec2 n = normalized(perp(v[j] - v[i]));
        const Vec2 mid = 0.5 * (v[i] + v[j]);
        if (dot(n, centroid - mid) > 0.0) n = -n;
        return n;
      }

      inline int nearestEdge(const std::array<Vec2, 3>& v, const Vec2& p)
      {
        int    best  = 0;
        iREAL  bestD = 1e300;
        for (int i = 0; i < 3; ++i) {
          const Vec2 q = closestOnSegment(v[i], v[(i + 1) % 3], p);
          const iREAL d = lengthSqr(q - p);
          if (d < bestD) { bestD = d; best = i; }
        }
        return best;
      }

      struct Projection { iREAL lo; iREAL hi; };

      inline Projection projectPolygon(const std::array<Vec2, 3>& v, const Vec2& axis)
      {
        iREAL lo = dot(v[0], axis);
        iREAL hi = lo;
        for (int i = 1; i < 3; ++i) {
          const iREAL d = dot(v[i], axis);
          if (d < lo) lo = d;
          if (d > hi) hi = d;
        }
        Projection p; p.lo = lo; p.hi = hi;
        return p;
      }

      inline Vec2 farthestVertex(const std::array<Vec2, 3>& v, const Vec2& axis, bool maxSide)
      {
        Vec2   best  = v[0];
        iREAL  bestD = dot(v[0], axis);
        for (int i = 1; i < 3; ++i) {
          const iREAL d = dot(v[i], axis);
          if (maxSide ? (d > bestD) : (d < bestD)) { bestD = d; best = v[i]; }
        }
        return best;
      }

      inline Vec2 triangleCentroid(const std::array<Vec2, 3>& v)
      {
        return (v[0] + v[1] + v[2]) / 3.0;
      }

      /* Centroid of the intersection of two triangles.
       *
       * The contact point of a polygon pair has to sit inside the real overlap.
       * Taking the midpoint of the two polygons' extreme vertices along the
       * normal looks right for two similarly sized bodies, but against a large
       * static wall it lands far away and the resulting lever arm spins the
       * body up. Clipping A by B's three half planes gives the true overlap
       * region, whose centroid is local to the contact by construction.
       *
       * Falls back to A's extreme vertex along the normal if floating point
       * leaves no polygon, which is a vertex contact with negligible area. */
      inline Vec2 intersectionCentroid(const std::array<Vec2, 3>& A,
                                       const std::array<Vec2, 3>& B,
                                       const Vec2& normal)
      {
        Vec2 poly[8];
        int  np = 3;
        for (int i = 0; i < 3; ++i) poly[i] = A[i];

        const Vec2 centroidB = triangleCentroid(B);
        for (int e = 0; e < 3 && np > 0; ++e) {
          const Vec2  p = B[e];
          const Vec2  q = B[(e + 1) % 3];
          const iREAL s = cross(q - p, centroidB - p) >= 0.0 ? 1.0 : -1.0;

          Vec2 out[8];
          int  no = 0;
          for (int i = 0; i < np; ++i) {
            const Vec2  cur = poly[i];
            const Vec2  nxt = poly[(i + 1) % np];
            const iREAL dc  = s * cross(q - p, cur - p);
            const iREAL dn  = s * cross(q - p, nxt - p);
            if (dc >= 0.0 && no < 8) out[no++] = cur;
            if (((dc >= 0.0) != (dn >= 0.0)) && no < 8) {
              const iREAL t = dc / (dc - dn);
              out[no++] = cur + t * (nxt - cur);
            }
          }
          np = no;
          for (int i = 0; i < np; ++i) poly[i] = out[i];
        }

        if (np >= 3) {
          Vec2 c(0.0, 0.0);
          for (int i = 0; i < np; ++i) c += poly[i];
          return c / (iREAL)np;
        }
        return farthestVertex(A, normal, true);
      }

      inline bool withinReach(const Particle2D& a, const Particle2D& b)
      {
        const iREAL reach = a.boundingRadius + b.boundingRadius + a.epsilon + b.epsilon;
        return lengthSqr(b.position - a.position) <= reach * reach;
      }

    }  // namespace

    /* -----------------------------------------------------------------------
     * factories
     * --------------------------------------------------------------------- */

    Particle2D makeCircle(int id, const Vec2& centre, iREAL radius, iREAL density,
                          iREAL friction, iREAL epsilon)
    {
      Particle2D p;
      p.id             = id;
      p.shape          = ShapeType::Circle;
      p.radius         = radius;
      p.position       = centre;
      p.boundingRadius = radius;
      p.epsilon        = epsilon;
      p.friction       = friction;
      p.mass           = density * PI * radius * radius;
      p.inertia        = 0.5 * p.mass * radius * radius;
      p.invMass        = p.mass    > 0.0 ? 1.0 / p.mass    : 0.0;
      p.invInertia     = p.inertia > 0.0 ? 1.0 / p.inertia : 0.0;
      return p;
    }

    Particle2D makeTriangle(int id, const std::array<Vec2, 3>& vertices, iREAL density,
                            iREAL friction, iREAL epsilon)
    {
      Particle2D p;
      p.id            = id;
      p.shape         = ShapeType::Triangle;
      p.worldVertices = vertices;

      const Vec2 centroid = triangleCentroid(vertices);
      p.position = centroid;
      for (int i = 0; i < 3; ++i) p.localVertices[i] = vertices[i] - centroid;

      const iREAL area = 0.5 * std::fabs(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
      p.mass = density * area;
      /* Uniform triangular lamina about its centroid: I = m/36 (a^2 + b^2 + c^2). */
      const iREAL a2 = lengthSqr(vertices[1] - vertices[0]);
      const iREAL b2 = lengthSqr(vertices[2] - vertices[1]);
      const iREAL c2 = lengthSqr(vertices[0] - vertices[2]);
      p.inertia = p.mass / 36.0 * (a2 + b2 + c2);

      p.invMass    = p.mass    > 0.0 ? 1.0 / p.mass    : 0.0;
      p.invInertia = p.inertia > 0.0 ? 1.0 / p.inertia : 0.0;

      p.boundingRadius = 0.0;
      for (int i = 0; i < 3; ++i) p.boundingRadius = std::max(p.boundingRadius, length(p.localVertices[i]));

      p.epsilon  = epsilon;
      p.friction = friction;
      return p;
    }

    void makeStatic(Particle2D& particle)
    {
      particle.isObstacle = true;
      particle.invMass    = 0.0;
      particle.invInertia = 0.0;
      particle.velocity  = Vec2(0.0, 0.0);
      particle.angularVelocity = 0.0;
    }

    /* -----------------------------------------------------------------------
     * contact routines
     * --------------------------------------------------------------------- */

    bool circleCircle(const Particle2D& a, const Particle2D& b, Contact2D& out)
    {
      const Vec2  d    = b.position - a.position;
      const iREAL dist = length(d);
      const iREAL sum  = a.radius + b.radius;
      if (dist >= sum) return false;

      const Vec2 n = dist > 0.0 ? d / dist : Vec2(1.0, 0.0);
      out.normal = n;
      out.depth  = sum - dist;
      /* Midpoint of the two surface points: half way between the a surface
       * point and the b surface point along the normal. */
      out.point  = 0.5 * (a.position + b.position) + 0.5 * (a.radius - b.radius) * n;
      return true;
    }

    bool circleTriangle(const Particle2D& circle, const Particle2D& triangle, Contact2D& out)
    {
      const std::array<Vec2, 3>& v = triangle.worldVertices;
      const Vec2  c        = circle.position;
      const Vec2  centroid = triangleCentroid(v);
      const int   e        = nearestEdge(v, c);
      const int   j        = (e + 1) % 3;
      const Vec2  q        = closestOnSegment(v[e], v[j], c);
      const iREAL dist     = length(q - c);
      const Vec2  nOut     = edgeOutwardNormal(v, e, j, centroid);

      /* normal points from the circle towards the triangle so that the reaction
       * pushes the circle outwards: force on circle = -f * normal. */
      if (pointInTriangle(v, c)) {
        /* Centre inside: the separation distance is radius + distance to the
         * nearest edge, and the circle must leave through that edge. */
        out.normal = -nOut;
        out.depth  = circle.radius + dist;
      } else {
        if (dist >= circle.radius) return false;
        out.normal = dist > 0.0 ? (q - c) / dist : -nOut;
        out.depth  = circle.radius - dist;
      }
      out.point = q;
      return true;
    }

    bool triangleTriangle(const Particle2D& a, const Particle2D& b, Contact2D& out)
    {
      const std::array<Vec2, 3>& A = a.worldVertices;
      const std::array<Vec2, 3>& B = b.worldVertices;

      iREAL minOverlap = 1e300;
      Vec2  minAxis(1.0, 0.0);
      bool  found = false;

      const std::array<Vec2, 3>* polys[2] = { &A, &B };
      for (int s = 0; s < 2; ++s) {
        const std::array<Vec2, 3>& P = *polys[s];
        for (int i = 0; i < 3; ++i) {
          const Vec2 axis = normalized(perp(P[(i + 1) % 3] - P[i]));
          if (lengthSqr(axis) <= 0.0) continue;

          const Projection pa = projectPolygon(A, axis);
          const Projection pb = projectPolygon(B, axis);
          const iREAL overlap = std::min(pa.hi, pb.hi) - std::max(pa.lo, pb.lo);
          if (overlap <= 0.0) return false;   /* separating axis exists */

          if (!found || overlap < minOverlap) { minOverlap = overlap; minAxis = axis; found = true; }
        }
      }
      if (!found) return false;

      /* Orient the minimum axis from a to b. */
      const Vec2 cA = triangleCentroid(A);
      const Vec2 cB = triangleCentroid(B);
      if (dot(minAxis, cB - cA) < 0.0) minAxis = -minAxis;

      out.normal = minAxis;
      out.depth  = minOverlap;
      out.point  = intersectionCentroid(A, B, minAxis);
      return true;
    }

    bool collide(const Particle2D& a, const Particle2D& b, Contact2D& out)
    {
      if (a.shape == ShapeType::Circle && b.shape == ShapeType::Circle)
        return circleCircle(a, b, out);

      if (a.shape == ShapeType::Circle && b.shape == ShapeType::Triangle)
        return circleTriangle(a, b, out);

      if (a.shape == ShapeType::Triangle && b.shape == ShapeType::Circle) {
        if (!circleTriangle(b, a, out)) return false;
        out.normal = -out.normal;   /* circle -> triangle becomes triangle -> circle */
        return true;
      }

      return triangleTriangle(a, b, out);
    }

    /* -----------------------------------------------------------------------
     * engine
     * --------------------------------------------------------------------- */

    Engine2D::Engine2D() {}

    Engine2D::Engine2D(std::vector<Particle2D> particles, const ContactParams2D& params)
      : _particles(std::move(particles)), _params(params)
    {
      _force.resize(_particles.size());
      _torque.resize(_particles.size());
      refreshWorldVertices();
    }

    void Engine2D::refreshWorldVertices()
    {
      for (std::vector<Particle2D>::iterator it = _particles.begin(); it != _particles.end(); ++it) {
        if (it->shape != ShapeType::Triangle) continue;
        const iREAL c = std::cos(it->angle);
        const iREAL s = std::sin(it->angle);
        for (int i = 0; i < 3; ++i)
          it->worldVertices[i] = it->position + rotate(it->localVertices[i], c, s);
      }
    }

    std::vector<std::pair<int, int> > Engine2D::binnedCandidates() const
    {
      std::vector<std::pair<int, int> > out;
      const int n = (int)_particles.size();
      if (n == 0) return out;

      /* Cell size is the largest possible pair reach, so a 3x3 cell scan
       * cannot miss a candidate (two points within one cell width differ in
       * cell index by at most one). */
      iREAL maxUnit = 0.0;
      for (int i = 0; i < n; ++i)
        maxUnit = std::max(maxUnit, _particles[i].boundingRadius + _particles[i].epsilon);
      if (maxUnit <= 0.0) return out;
      const iREAL h = 2.0 * maxUnit;

      std::map<std::pair<int, int>, std::vector<int> > grid;
      for (int i = 0; i < n; ++i) {
        const int ix = (int)std::floor(_particles[i].position.x / h);
        const int iy = (int)std::floor(_particles[i].position.y / h);
        grid[std::make_pair(ix, iy)].push_back(i);
      }

      for (int i = 0; i < n; ++i) {
        const Particle2D& pi = _particles[i];
        const int ix = (int)std::floor(pi.position.x / h);
        const int iy = (int)std::floor(pi.position.y / h);
        for (int dx = -1; dx <= 1; ++dx) {
          for (int dy = -1; dy <= 1; ++dy) {
            const std::map<std::pair<int, int>, std::vector<int> >::const_iterator it =
              grid.find(std::make_pair(ix + dx, iy + dy));
            if (it == grid.end()) continue;
            for (size_t k = 0; k < it->second.size(); ++k) {
              const int j = it->second[k];
              if (j <= i) continue;
              if (!withinReach(pi, _particles[j])) continue;
              out.push_back(std::make_pair(i, j));
            }
          }
        }
      }
      return out;
    }

    std::vector<std::pair<int, int> > Engine2D::bruteForceCandidates() const
    {
      std::vector<std::pair<int, int> > out;
      const int n = (int)_particles.size();
      for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
          if (withinReach(_particles[i], _particles[j])) out.push_back(std::make_pair(i, j));
      return out;
    }

    void Engine2D::contactDetection()
    {
      _contacts.clear();
      const std::vector<std::pair<int, int> > pairs = binnedCandidates();
      for (size_t k = 0; k < pairs.size(); ++k) {
        Contact2D c;
        if (collide(_particles[pairs[k].first], _particles[pairs[k].second], c)) {
          c.a = pairs[k].first;
          c.b = pairs[k].second;
          _contacts.push_back(c);
        }
      }
    }

    void Engine2D::bruteForceContacts(std::vector<Contact2D>& out) const
    {
      out.clear();
      const int n = (int)_particles.size();
      for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
          Contact2D c;
          if (collide(_particles[i], _particles[j], c)) {
            c.a = i;
            c.b = j;
            out.push_back(c);
          }
        }
      }
    }

    void Engine2D::deriveForces()
    {
      const size_t n = _particles.size();
      for (size_t i = 0; i < n; ++i) { _force[i] = Vec2(0.0, 0.0); _torque[i] = 0.0; }

      for (size_t k = 0; k < _contacts.size(); ++k) {
        const Contact2D&  c = _contacts[k];
        const Particle2D& A = _particles[c.a];
        const Particle2D& B = _particles[c.b];

        const Vec2 rA = c.point - A.position;
        const Vec2 rB = c.point - B.position;
        const Vec2 vA = A.velocity + A.angularVelocity * perp(rA);
        const Vec2 vB = B.velocity + B.angularVelocity * perp(rB);
        const Vec2 vRel = vB - vA;

        const Vec2&  n  = c.normal;
        const iREAL  vn = dot(vRel, n);

        const iREAL invMassSum = A.invMass + B.invMass;
        if (invMassSum <= 0.0) continue;   /* obstacle against obstacle */
        const iREAL mEff = 1.0 / invMassSum;

        /* Damping opposes the approach: vRel along the a->b normal is negative
         * while the pair closes, so the correct damper is -c * vn. (Note the 3D
         * core's forces.cpp adds +c*vn here, which injects energy; not copied.) */
        iREAL fn = _params.stiffness * c.depth
                 - 2.0 * _params.dampingRatio * std::sqrt(_params.stiffness * mEff) * vn;
        if (fn < 0.0) fn = 0.0;   /* spring-dashpot carries no tension */

        Vec2 F = fn * n;

        const Vec2  vt    = vRel - vn * n;
        const iREAL vtLen = length(vt);
        if (vtLen > 0.0) {
          const iREAL mu  = 0.5 * (A.friction + B.friction);
          iREAL       ft  = _params.tangentialStiffness * vtLen;
          const iREAL cap = mu * fn;
          if (ft > cap) ft = cap;
          F -= (ft / vtLen) * vt;
        }

        _force[c.a]  -= F;
        _torque[c.a] -= cross(rA, F);
        _force[c.b]  += F;
        _torque[c.b] += cross(rB, F);
      }
    }

    void Engine2D::integrate(iREAL dt)
    {
      for (size_t i = 0; i < _particles.size(); ++i) {
        Particle2D& p = _particles[i];
        if (p.invMass <= 0.0) continue;

        p.velocity += dt * (_params.gravity + p.invMass * _force[i]);
        p.angularVelocity += dt * p.invInertia * _torque[i];
        p.position += dt * p.velocity;
        p.angle    += dt * p.angularVelocity;
      }
    }

    void Engine2D::step(iREAL dt)
    {
      refreshWorldVertices();
      contactDetection();
      deriveForces();
      integrate(dt);
      refreshWorldVertices();
    }

  }
}
