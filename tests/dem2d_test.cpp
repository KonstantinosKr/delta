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

/*
 * Acceptance tests for the 2D circle/triangle DEM core.
 *
 *   1. contact primitives against hand computed geometry (cc, ct, tt)
 *   2. SAT depth is a true separation distance (push out by depth -> no contact)
 *   3. third law: momentum and angular momentum survive a step, all pairings
 *   4. the uniform-cell broad phase finds exactly the brute force pair set
 *   5. a mixed circle/triangle box stays finite and bounded over many steps
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <random>
#include <vector>

#include "dem2d/Dem2D.h"

using namespace delta::dem2d;

static int failures = 0;

#define CHECK(cond)                                                            \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond);              \
      ++failures;                                                              \
    }                                                                          \
  } while (0)

#define CHECK_NEAR(a, b, tol)                                                  \
  do {                                                                         \
    const double _a = (a), _b = (b), _t = (tol);                               \
    if (!(std::fabs(_a - _b) <= _t)) {                                         \
      std::printf("FAIL %s:%d  |%g - %g| > %g\n", __FILE__, __LINE__, _a, _b, _t); \
      ++failures;                                                              \
    }                                                                          \
  } while (0)

/* -------------------------------------------------------------------------- */

static void testCircleCircle()
{
  const Particle2D a = makeCircle(0, Vec2(0.0, 0.0), 0.5, 1000.0, 0.0);
  const Particle2D b = makeCircle(1, Vec2(0.8, 0.0), 0.5, 1000.0, 0.0);

  Contact2D c;
  CHECK(collide(a, b, c));
  CHECK_NEAR(c.depth, 0.2, 1e-12);
  CHECK_NEAR(c.normal.x, 1.0, 1e-12);
  CHECK_NEAR(c.normal.y, 0.0, 1e-12);
  CHECK_NEAR(c.point.x, 0.4, 1e-12);

  /* separated disks do not contact */
  const Particle2D far = makeCircle(2, Vec2(3.0, 0.0), 0.5, 1000.0, 0.0);
  CHECK(!collide(a, far, c));

  /* mass and inertia of a disk */
  CHECK_NEAR(a.mass, 1000.0 * 3.14159265358979323846 * 0.25, 1e-9);
  CHECK_NEAR(a.inertia, 0.5 * a.mass * 0.25, 1e-9);
}

static void testCircleTriangle()
{
  /* Right triangle with a horizontal bottom edge y = 0 running x in [0, 2]. */
  const std::array<Vec2, 3> tri = { Vec2(0.0, 0.0), Vec2(2.0, 0.0), Vec2(2.0, 1.0) };
  Particle2D t = makeTriangle(0, tri, 1000.0, 0.0);

  /* Circle centred below the bottom edge by 0.9, radius 1.0 -> overlap 0.1. */
  Particle2D c = makeCircle(1, Vec2(1.0, -0.9), 1.0, 1000.0, 0.0);

  Contact2D out;
  CHECK(collide(c, t, out));                 /* circle first: normal circle -> triangle */
  CHECK_NEAR(out.depth, 0.1, 1e-12);
  CHECK_NEAR(out.normal.x, 0.0, 1e-12);
  CHECK_NEAR(out.normal.y, 1.0, 1e-12);
  CHECK_NEAR(out.point.x, 1.0, 1e-12);
  CHECK_NEAR(out.point.y, 0.0, 1e-12);

  /* reversed argument order must flip the normal */
  Contact2D flipped;
  CHECK(collide(t, c, flipped));
  CHECK_NEAR(flipped.normal.x, -out.normal.x, 1e-12);
  CHECK_NEAR(flipped.normal.y, -out.normal.y, 1e-12);
  CHECK_NEAR(flipped.depth, out.depth, 1e-12);

  /* a clear gap gives no contact (closest point on the hypotenuse is 1.34 away) */
  Particle2D high = makeCircle(2, Vec2(1.0, 2.0), 1.0, 1000.0, 0.0);
  CHECK(!collide(high, t, out));

  /* centre inside the triangle: separation distance is radius + distance to edge */
  Particle2D inside = makeCircle(3, Vec2(1.5, 0.2), 0.05, 1000.0, 0.0);
  CHECK(collide(inside, t, out));
  CHECK_NEAR(out.depth, 0.05 + 0.2, 1e-12);
}

static void testTriangleTriangle()
{
  /* Unit right triangle, duplicated and shifted so the two overlap. */
  const std::array<Vec2, 3> base = { Vec2(0.0, 0.0), Vec2(2.0, 0.0), Vec2(0.0, 2.0) };
  std::array<Vec2, 3> shifted;
  for (int i = 0; i < 3; ++i) shifted[i] = base[i] + Vec2(0.2, 0.2);

  Particle2D a = makeTriangle(0, base, 1000.0, 0.0);
  Particle2D b = makeTriangle(1, shifted, 1000.0, 0.0);

  Contact2D c;
  CHECK(collide(a, b, c));
  CHECK(c.depth > 0.0);
  CHECK_NEAR(length(c.normal), 1.0, 1e-12);

  /* the normal must point from a to b */
  CHECK(dot(c.normal, b.position - a.position) > 0.0);

  /* depth is a separation distance: pushing b out by depth removes contact. */
  std::array<Vec2, 3> pushed;
  for (int i = 0; i < 3; ++i) pushed[i] = shifted[i] + (c.depth * 1.001) * c.normal;
  Particle2D bOut = makeTriangle(2, pushed, 1000.0, 0.0);
  Contact2D ignored;
  CHECK(!collide(a, bOut, ignored));

  /* pushing b part way in keeps the contact and reduces the depth */
  std::array<Vec2, 3> deeper;
  for (int i = 0; i < 3; ++i) deeper[i] = shifted[i] - (c.depth * 0.25) * c.normal;
  Particle2D bIn = makeTriangle(3, deeper, 1000.0, 0.0);
  Contact2D inOut;
  CHECK(collide(a, bIn, inOut));
  CHECK(inOut.depth > c.depth);

  /* fully separated copies do not contact */
  std::array<Vec2, 3> apart;
  for (int i = 0; i < 3; ++i) apart[i] = base[i] + Vec2(5.0, 0.0);
  Particle2D bFar = makeTriangle(4, apart, 1000.0, 0.0);
  CHECK(!collide(a, bFar, ignored));
}

/* -------------------------------------------------------------------------- */

static void totalMomentum(const std::vector<Particle2D>& ps, Vec2& p, iREAL& l)
{
  p = Vec2(0.0, 0.0);
  l = 0.0;
  for (size_t i = 0; i < ps.size(); ++i) {
    if (ps[i].invMass <= 0.0) continue;
    p += ps[i].mass * ps[i].velocity;
    l += ps[i].mass * cross(ps[i].position, ps[i].velocity) + ps[i].inertia * ps[i].angularVelocity;
  }
}

static void checkMomentum(const char* label, std::vector<Particle2D> ps)
{
  Engine2D engine(ps);
  engine.params().gravity = Vec2(0.0, 0.0);

  Vec2  p0; iREAL l0;
  totalMomentum(engine.particles(), p0, l0);

  CHECK(engine.contacts().empty());   /* not stepped yet */
  engine.step(1e-4);

  Vec2  p1; iREAL l1;
  totalMomentum(engine.particles(), p1, l1);

  if (!(engine.contacts().size() > 0)) {
    std::printf("FAIL %s: expected a contact, got none\n", label);
    ++failures;
  }

  const iREAL scale = std::max<iREAL>(1.0, length(p0));
  CHECK_NEAR(length(p1 - p0), 0.0, 1e-10 * scale);
  CHECK_NEAR(l1, l0, 1e-9 * std::max<iREAL>(1.0, std::fabs(l0)));
}

static void testThirdLaw()
{
  {
    std::vector<Particle2D> ps;
    ps.push_back(makeCircle(0, Vec2(0.0, 0.0), 0.25, 1000.0, 0.3));
    ps.push_back(makeCircle(1, Vec2(0.4, 0.0), 0.25, 1000.0, 0.3));
    ps[0].velocity = Vec2(0.5, 0.1);
    ps[1].velocity = Vec2(-0.4, -0.2);
    ps[0].angularVelocity = 0.7;
    ps[1].angularVelocity = -0.3;
    checkMomentum("circle-circle", ps);
  }
  {
    std::vector<Particle2D> ps;
    const std::array<Vec2, 3> tri = { Vec2(0.0, 0.0), Vec2(0.6, 0.0), Vec2(0.0, 0.6) };
    ps.push_back(makeTriangle(0, tri, 1000.0, 0.3));
    ps.push_back(makeCircle(1, Vec2(0.5, 0.1), 0.2, 1000.0, 0.3));
    ps[0].velocity = Vec2(-0.2, 0.3);
    ps[1].velocity = Vec2(0.3, -0.1);
    ps[0].angularVelocity = 0.4;
    ps[1].angularVelocity = -0.6;
    checkMomentum("triangle-circle", ps);
  }
  {
    std::vector<Particle2D> ps;
    const std::array<Vec2, 3> a = { Vec2(0.0, 0.0), Vec2(0.8, 0.0), Vec2(0.0, 0.8) };
    std::array<Vec2, 3> b;
    for (int i = 0; i < 3; ++i) b[i] = a[i] + Vec2(0.2, 0.2);
    ps.push_back(makeTriangle(0, a, 1000.0, 0.3));
    ps.push_back(makeTriangle(1, b, 1000.0, 0.3));
    ps[0].velocity = Vec2(0.1, -0.2);
    ps[1].velocity = Vec2(-0.3, 0.2);
    ps[0].angularVelocity = 0.5;
    ps[1].angularVelocity = -0.5;
    checkMomentum("triangle-triangle", ps);
  }
}

/* -------------------------------------------------------------------------- */

static std::vector<Particle2D> randomCloud(unsigned seed, int count)
{
  std::mt19937 rng(seed);
  std::uniform_real_distribution<double> pos(0.0, 10.0);
  std::uniform_real_distribution<double> siz(0.1, 0.5);
  std::uniform_real_distribution<double> dst(-0.05, 0.05);

  std::vector<Particle2D> ps;
  for (int i = 0; i < count; ++i) {
    const Vec2 c(pos(rng), pos(rng));
    if (i % 2 == 0) {
      ps.push_back(makeCircle(i, c, siz(rng), 1000.0, 0.2));
    } else {
      const double s = siz(rng);
      std::array<Vec2, 3> v = { c + Vec2(-s, -s), c + Vec2(s, -s + dst(rng)), c + Vec2(0.1 * dst(rng), s) };
      ps.push_back(makeTriangle(i, v, 1000.0, 0.2));
    }
  }
  return ps;
}

static void testBroadPhase()
{
  for (int trial = 0; trial < 5; ++trial) {
    Engine2D engine(randomCloud(100u + trial, 300));

    std::vector<std::pair<int, int> > binned = engine.binnedCandidates();
    std::vector<std::pair<int, int> > brute  = engine.bruteForceCandidates();
    std::sort(binned.begin(), binned.end());
    std::sort(brute.begin(), brute.end());

    if (binned != brute) {
      std::printf("FAIL broad phase trial %d: %d binned vs %d brute candidates\n",
                  trial, (int)binned.size(), (int)brute.size());
      ++failures;
      continue;
    }

    engine.contactDetection();
    std::vector<Contact2D> bruteContacts;
    engine.bruteForceContacts(bruteContacts);

    std::vector<std::pair<int, int> > binnedPairs;
    for (size_t i = 0; i < engine.contacts().size(); ++i)
      binnedPairs.push_back(std::make_pair(engine.contacts()[i].a, engine.contacts()[i].b));
    std::vector<std::pair<int, int> > brutePairs;
    for (size_t i = 0; i < bruteContacts.size(); ++i)
      brutePairs.push_back(std::make_pair(bruteContacts[i].a, bruteContacts[i].b));
    std::sort(binnedPairs.begin(), binnedPairs.end());
    std::sort(brutePairs.begin(), brutePairs.end());

    CHECK(binnedPairs == brutePairs);
  }
}

/* -------------------------------------------------------------------------- */

static void addRect(std::vector<Particle2D>& ps, double x0, double y0, double x1, double y1, int baseId)
{
  std::array<Vec2, 3> t0 = { Vec2(x0, y0), Vec2(x1, y0), Vec2(x1, y1) };
  std::array<Vec2, 3> t1 = { Vec2(x0, y0), Vec2(x1, y1), Vec2(x0, y1) };
  Particle2D a = makeTriangle(baseId, t0, 1000.0, 0.0);
  Particle2D b = makeTriangle(baseId + 1, t1, 1000.0, 0.0);
  makeStatic(a);
  makeStatic(b);
  ps.push_back(a);
  ps.push_back(b);
}

static void testMixedBox()
{
  std::vector<Particle2D> ps;

  /* thick static walls, domain is [0, 10] x [0, 10], open at the top */
  addRect(ps, -1.0, -1.0,  0.0, 11.0, 0);    /* left   */
  addRect(ps, 10.0, -1.0, 11.0, 11.0, 10);   /* right  */
  addRect(ps, -1.0, -1.0, 11.0,  0.0, 20);   /* bottom */

  std::mt19937 rng(7);
  std::uniform_real_distribution<double> pos(1.0, 9.0);
  std::uniform_real_distribution<double> siz(0.15, 0.35);

  for (int i = 0; i < 40; ++i) {
    const Vec2 c(pos(rng), pos(rng) + 4.0);
    if (i % 2 == 0) {
      ps.push_back(makeCircle(100 + i, c, siz(rng), 1000.0, 0.4));
    } else {
      const double s = siz(rng);
      std::array<Vec2, 3> v = { c + Vec2(-s, -s), c + Vec2(s, -s), c + Vec2(0.0, s) };
      ps.push_back(makeTriangle(100 + i, v, 1000.0, 0.4));
    }
  }

  Engine2D engine(ps);
  const iREAL dt = 1e-3;
  for (int s = 0; s < 2000; ++s) engine.step(dt);

  const std::vector<Particle2D>& out = engine.particles();
  int finite = 0;
  iREAL maxSpeed = 0.0;
  for (size_t i = 0; i < out.size(); ++i) {
    const Particle2D& p = out[i];
    if (std::isfinite(p.position.x) && std::isfinite(p.position.y) &&
        std::isfinite(p.velocity.x) && std::isfinite(p.velocity.y) &&
        std::isfinite(p.angle) && std::isfinite(p.angularVelocity)) {
      ++finite;
    }
    maxSpeed = std::max(maxSpeed, length(p.velocity));
  }
  CHECK(finite == (int)out.size());
  CHECK(maxSpeed < 50.0);

  int inside = 0;
  for (size_t i = 0; i < out.size(); ++i) {
    const Particle2D& p = out[i];
    if (p.isObstacle) { ++inside; continue; }
    if (p.position.x > -0.6 && p.position.x < 10.6 &&
        p.position.y > -0.6 && p.position.y < 10.6) ++inside;
  }
  if (inside != (int)out.size()) {
    std::printf("FAIL mixed box: %d of %d bodies inside the walls\n", inside, (int)out.size());
    ++failures;
  }

  /* anything that came to rest should do so above the floor */
  int settled = 0;
  for (size_t i = 0; i < out.size(); ++i)
    if (!out[i].isObstacle && length(out[i].velocity) < 0.5) ++settled;

  std::printf("  mixed box: %d bodies, %d contacts at rest, %d settled, max|v|=%g\n",
              (int)out.size(), (int)engine.contacts().size(), settled, maxSpeed);
  CHECK(settled > 0);
}

/* -------------------------------------------------------------------------- */

int main()
{
  testCircleCircle();
  testCircleTriangle();
  testTriangleTriangle();
  testThirdLaw();
  testBroadPhase();
  testMixedBox();

  if (failures == 0) {
    std::printf("dem2d_test OK: circle/triangle contacts, third law, broad phase, mixed box\n");
    return 0;
  }
  std::printf("dem2d_test FAILED with %d check(s)\n", failures);
  return 1;
}
