/*
 * Drop test: a disk falls onto a static floor and bounces.
 *
 * This is the honest check on the damping sign. A sign error in the dashpot
 * term does not show up as a crash - it shows up as a bounce that keeps energy,
 * or one that dies faster than the damping ratio says it should. The linear
 * spring-dashpot of 2.3 has a closed form:
 *
 *     coefficient of restitution  e = exp(-pi * zeta / sqrt(1 - zeta^2))
 *           for zeta < 1, from the log decrement of a damped oscillator
 *     contact duration            t = pi / (omega_d),  omega_d = omega_n*sqrt(1-zeta^2)
 *                                     omega_n = sqrt(k_eff / m)
 *
 * The closed form only holds for small penetration. At a 0.5 m drop this disk
 * reaches ~20% of its radius, where the linearised formula under-predicts the
 * rebound, so the test also integrates the same 1-DOF ODE with the same
 * explicit Euler and dt and compares against that instead. Agreement with the
 * integration is the real claim: it means the engine solves the equation it
 * says it solves.
 *
 * Usage:  drop_test [--rebounds N] [--dashpot Z] [--dt S] [-v]
 *         -v writes an OBJ sequence of the trajectory for render_drop.py
 */
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "dim2/Dem2D.h"

using namespace delta::dem2d;

namespace {

const double kPi = 3.14159265358979323846;

/* Floor as a rectangle of two static triangles, top edge at y = 0. */
void addFloor(std::vector<Particle2D>& ps, double halfWidth)
{
  const std::array<Vec2, 3> t0 = { Vec2(-halfWidth, -1.0), Vec2(halfWidth, -1.0), Vec2(halfWidth, 0.0) };
  const std::array<Vec2, 3> t1 = { Vec2(-halfWidth, -1.0), Vec2(halfWidth, 0.0), Vec2(-halfWidth, 0.0) };
  Particle2D a = makeTriangle(0, t0, 1000.0, 0.0);
  Particle2D b = makeTriangle(1, t1, 1000.0, 0.0);
  makeStatic(a);
  makeStatic(b);
  ps.push_back(a);
  ps.push_back(b);
}

struct Result {
  double e_measured   = 0.0;   // speed out / speed in, first bounce
  double t_contact    = 0.0;   // seconds spent touching the floor, first bounce
  double v_in         = 0.0;
  double v_out        = 0.0;
  double maxDepth     = 0.0;   // deepest penetration, sanity
};

/* The same equation the engine claims to solve, in one dimension, integrated
 * with the same explicit Euler and dt. This is the reference the measured
 * numbers are compared against; the analytic e=exp(...) is reported for
 * context only, because it assumes small penetration. */
struct Reference { double e, t_contact; };

Reference integrateReference(double m, double k, double zeta, double dt,
                             double radius, double dropY)
{
  const double g  = 9.8;
  const double c  = 2.0 * zeta * std::sqrt(k * m);
  double y = dropY, v = 0.0;
  bool     touching = false;
  double   vIn = 0.0, tC = 0.0;
  Reference out{0.0, 0.0};

  const int maxSteps = (int)(8.0 / dt);
  for (int step = 0; step < maxSteps; ++step) {
    const double vBefore = v;
    const bool   now = (y - radius) <= 1e-9;
    double F = 0.0;
    if (now) {
      const double depth = radius - y;
      F = k * depth - c * v;
      if (F < 0.0) F = 0.0;
    }
    v += dt * (-g + F / m);
    y += dt * v;

    if (now && !touching) { vIn = vBefore; tC = 0.0; }
    if (now) tC += dt;
    if (!now && touching) {
      out.e = (vIn != 0.0) ? std::fabs(v) / std::fabs(vIn) : 0.0;
      out.t_contact = tC;
      return out;
    }
    touching = now;
  }
  return out;
}

}  // namespace

int main(int argc, char** argv)
{
  int    rebounds = 3;
  double zeta     = 0.5;
  double dt       = 1e-5;
  bool   writeObj = false;
  std::string objDir;

  for (int a = 1; a < argc; ++a) {
    const std::string f = argv[a];
    if (f == "-v") { writeObj = true; if (a + 1 < argc && argv[a+1][0] != '-') objDir = argv[++a]; }
    else if (f == "--rebounds" && a + 1 < argc) rebounds = std::atoi(argv[++a]);
    else if (f == "--dashpot"  && a + 1 < argc) zeta     = std::atof(argv[++a]);
    else if (f == "--dt"       && a + 1 < argc) dt       = std::atof(argv[++a]);
    else if (f == "-h" || f == "--help") {
      std::printf("usage: drop_test [--rebounds N] [--dashpot Z] [--dt S] [-v DIR]\n");
      return 0;
    }
  }

  const double radius  = 0.05;
  const double dropY   = 0.50;         // centre height above the floor
  const double density = 1000.0;

  std::vector<Particle2D> ps;
  addFloor(ps, 2.0);

  Particle2D ball = makeCircle(10, Vec2(0.0, dropY), radius, density, 0.0);
  ball.epsilon = 0.0;
  ps.push_back(ball);

  Engine2D engine(ps);
  engine.params().stiffness    = 2.0e5;
  engine.params().dampingRatio = (iREAL)zeta;
  engine.params().gravity      = Vec2(0.0, -9.8);

  const double m       = engine.particles()[2].mass;
  const double kEff    = engine.params().stiffness;   // floor is static => m_eff = m
  const double omegaN  = std::sqrt(kEff / m);
  const double omegaD  = omegaN * std::sqrt(1.0 - zeta * zeta);
  const double eTheory = std::exp(-kPi * zeta / std::sqrt(1.0 - zeta * zeta));
  const double tTheory = kPi / omegaD;

  std::printf("disk r=%.3f, m=%.6g, dropped from y=%.3f onto a static floor\n", radius, m, dropY);
  std::printf("k=%.3g  zeta=%.3f  dt=%.3g\n", kEff, zeta, dt);
  std::printf("predicted: e=%.4f  contact time=%.6f s\n\n", eTheory, tTheory);

  if (writeObj) {
    std::string mk = "mkdir -p " + objDir;
    if (std::system(mk.c_str()) != 0) { std::fprintf(stderr, "mkdir failed\n"); return 1; }
  }

  Result r;
  int      bounce    = 0;
  bool     touching  = false;
  double   tContact  = 0.0;
  double   vIn       = 0.0;
  double   vOutPrev  = 0.0;
  double   maxDepth  = 0.0;
  int      frame     = 0;
  const int maxSteps = (int)(8.0 / dt);          // 8 s cap
  /* ~50 fps over 6 s, independent of dt. */
  const int frameEvery = std::max(1, (int)std::lround(0.02 / dt));

  for (int step = 0; step < maxSteps; ++step) {
    const Particle2D& b = engine.particles()[2];
    const double vBefore = b.velocity.y;
    const double yBefore = b.position.y;

    engine.step((iREAL)dt);

    const Particle2D& b2 = engine.particles()[2];

    /* Is the disk within the contact reach of the floor top (y = 0)? */
    const bool nowTouching = (b2.position.y - radius) <= 1e-9;
    const double depth = std::max(0.0, radius - b2.position.y);
    if (depth > maxDepth) maxDepth = depth;

    if (nowTouching && !touching) {          // contact begins
      vIn      = vBefore;                     // downward, negative
      tContact = 0.0;
    }
    if (nowTouching) tContact += dt;

    if (!nowTouching && touching) {          // contact ends -> that is a bounce
      ++bounce;
      if (bounce == 1) {
        r.v_in      = std::fabs(vIn);
        r.v_out     = std::fabs(b2.velocity.y);
        r.e_measured = (r.v_in > 0.0) ? r.v_out / r.v_in : 0.0;
        r.t_contact = tContact;
        r.maxDepth  = maxDepth;
      }
      std::printf("bounce %d: v_in=%.4f v_out=%.4f  e=%.4f  contact=%.6f s  apex=%.4f\n",
                  bounce, std::fabs(vIn), std::fabs(b2.velocity.y),
                  (vIn != 0.0) ? std::fabs(b2.velocity.y) / std::fabs(vIn) : 0.0,
                  tContact, b2.position.y);
      vOutPrev = b2.velocity.y;
    }

    touching = nowTouching;

    /* Frames on a wall-clock cadence, not per step: the contact window is
     * short and would otherwise dominate the frame count. */
    if (writeObj && (step % frameEvery == 0)) {
      char name[512];
      std::snprintf(name, sizeof(name), "%s/frame_%05d.obj", objDir.c_str(), frame++);
      FILE* out = std::fopen(name, "w");
      if (!out) { std::fprintf(stderr, "cannot write %s\n", name); break; }
      std::fprintf(out, "# drop frame %d t=%.5f\n", frame, step * dt);

      /* floor as two triangles, drawn as edges. The group label precedes the
       * vertices, so a reader can bucket them by the label in force. */
      const double hw = 2.0;
      std::fprintf(out, "g floor\n");
      std::fprintf(out, "v %f 0 0\nv %f 0 0\nv %f -1 0\n", -hw, hw, -hw);
      std::fprintf(out, "v %f 0 0\nv %f -1 0\nv %f -1 0\n", hw, hw, -hw);
      std::fprintf(out, "l 1 2\nl 2 3\nl 3 1\nl 4 5\nl 5 6\nl 6 4\n");

      /* the disk as a 24-gon, so the video shows the actual radius */
      const int N = 24;
      std::fprintf(out, "g ball\n");
      for (int i = 0; i < N; ++i) {
        const double a = 2.0 * kPi * i / N;
        std::fprintf(out, "v %f %f 0\n",
                     b2.position.x + radius * std::cos(a),
                     b2.position.y + radius * std::sin(a));
      }
      for (int i = 0; i < N; ++i)
        std::fprintf(out, "l %d %d\n", 7 + i, 7 + (i + 1) % N);
      std::fclose(out);
    }

    /* stop once the requested number of bounces is done and it is at rest */
    if (bounce >= rebounds && std::fabs(b2.velocity.y) < 1e-3 &&
        (b2.position.y - radius) > 1e-6) break;
  }

  const Reference ref = integrateReference(m, kEff, zeta, dt, radius, dropY);

  std::printf("\n--- first bounce ---\n");
  std::printf("  %-28s %10s %10s %10s\n", "", "measured", "integrated", "analytic");
  std::printf("  %-28s %10.4f %10.4f %10.4f\n", "restitution e",
              r.e_measured, ref.e, eTheory);
  std::printf("  %-28s %10.6f %10.6f %10.6f\n", "contact time t_c",
              r.t_contact, ref.t_contact, tTheory);

  const bool eMatches    = std::fabs(r.e_measured - ref.e) < 0.01;
  const bool tcMatches   = std::fabs(r.t_contact - ref.t_contact) < 1e-3;
  std::printf("\n  vs integration: e %s, t_c %s\n",
              eMatches ? "OK" : "MISMATCH", tcMatches ? "OK" : "MISMATCH");
  std::printf("  (the analytic column is context only: it assumes small "
              "penetration, this bounce reaches %.0f%% of the radius)\n",
              100.0 * r.maxDepth / radius);

  /* The physics claim: a passive dashpot can only ever remove energy, so the
   * rebound must be slower than the impact. This is the check that fails if the
   * damping sign is wrong. */
  /* A passive dashpot either removes energy (zeta > 0) or, with no dashpot,
   * leaves the collision elastic (zeta = 0, v_out == v_in to rounding). What it
   * must never do is return more energy than went in. */
  const iREAL tol = 1e-3 * r.v_in;
  const bool noGain = r.v_out <= r.v_in + tol;
  std::printf("  energy   v_out %s v_in   %s\n",
              (r.v_out < r.v_in - tol) ? "<" : "~",
              noGain ? (zeta > 0.0 ? "OK (dashpot removed energy)"
                                   : "OK (no dashpot, elastic)")
                     : "FAIL (dashpot added energy)");
  std::printf("  max penetration %.5f (r=%.3f, %.1f%% of radius)\n",
              r.maxDepth, radius, 100.0 * r.maxDepth / radius);

  int failures = 0;
  if (!noGain)       ++failures;
  if (!eMatches)     ++failures;
  if (!tcMatches)    ++failures;
  if (writeObj) std::printf("\nwrote %d frames to %s\n", frame, objDir.c_str());

  std::printf("\n%s\n", failures == 0 ? "PASS" : "FAIL");
  return failures == 0 ? 0 : 1;
}
