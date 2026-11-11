/*
 * apps/dem2d — entry point for the 2D solver.
 *
 * Counterpart of apps/dem3d. Fills a box with circles and triangles, drops them
 * under gravity and writes VTU frames through the shared MeshBuffer writer, so
 * the output convention matches the 3D path.
 *
 * The engine has no scenario-file format of its own, so the scene is a flag
 * set. Everything has a default that runs.
 */

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>
#include <vector>

#include "core/ContactParams.h"
#include "dim2/Dem2D.h"
#include "dim2/Dem2DWrite.h"

using namespace delta::dem2d;

namespace {

struct Options
{
  int    steps      = 3000;
  int    bodies     = 60;
  int    saveEvery  = 25;      /* 0 disables frames */
  double dt         = 1e-3;
  double domain     = 10.0;    /* square box [0,domain]^2, open at the top */
  std::string out   = "dem2d_out";
  unsigned seed     = 12345;
  delta::core::ContactParams contact;
};

void usage()
{
  std::printf(
    "usage: dem2d [options]\n"
    "  --steps N        integration steps          (default 3000)\n"
    "  --bodies N       number of moving bodies    (default 60)\n"
    "  --dt S           step size                  (default 1e-3)\n"
    "  --domain S       box side, open at the top  (default 10)\n"
    "  --out DIR        VTU output directory       (default dem2d_out)\n"
    "  --save-every N   frames written every N steps, 0 = none  (default 25)\n"
    "  --seed N         placement seed             (default 12345)\n"
    "  --spring S       normal stiffness           (default %.3g)\n"
    "  --dashpot S      damping ratio              (default %.3g)\n"
    "  --friction S     Coulomb coefficient        (default %.3g)\n",
    Options{}.contact.spring, Options{}.contact.dashpot, Options{}.contact.friction);
}

/* Two triangles per rectangle, both static: floor, left wall, right wall. */
void addWall(std::vector<Particle2D>& ps, double x0, double y0,
             double x1, double y1, int baseId)
{
  const std::array<Vec2, 3> t0 = { Vec2(x0, y0), Vec2(x1, y0), Vec2(x1, y1) };
  const std::array<Vec2, 3> t1 = { Vec2(x0, y0), Vec2(x1, y1), Vec2(x0, y1) };
  Particle2D a = makeTriangle(baseId,     t0, 1000.0, 0.0);
  Particle2D b = makeTriangle(baseId + 1, t1, 1000.0, 0.0);
  makeStatic(a);
  makeStatic(b);
  ps.push_back(a);
  ps.push_back(b);
}

bool finiteState(const std::vector<Particle2D>& ps, int& badId, const char*& field)
{
  for (size_t i = 0; i < ps.size(); ++i) {
    const Particle2D& p = ps[i];
    if (!std::isfinite(p.position.x) || !std::isfinite(p.position.y)) { badId = p.id; field = "position"; return false; }
    if (!std::isfinite(p.velocity.x) || !std::isfinite(p.velocity.y)) { badId = p.id; field = "velocity"; return false; }
    if (!std::isfinite(p.angle))                                      { badId = p.id; field = "angle";    return false; }
    if (!std::isfinite(p.angularVelocity))                            { badId = p.id; field = "angularVelocity"; return false; }
  }
  badId = -1;
  field = "";
  return true;
}

}  // namespace

int main(int argc, const char** argv)
{
  Options opt;

  for (int a = 1; a < argc; ++a) {
    const std::string flag = argv[a];
    const char* value = (a + 1 < argc) ? argv[a + 1] : nullptr;

    if (flag == "-h" || flag == "--help") { usage(); return 0; }

    /* Every flag below takes a value. */
    if (!value) { std::fprintf(stderr, "%s needs a value\n", flag.c_str()); return 2; }

    if      (flag == "--steps")      opt.steps     = std::atoi(value);
    else if (flag == "--bodies")     opt.bodies    = std::atoi(value);
    else if (flag == "--save-every") opt.saveEvery = std::atoi(value);
    else if (flag == "--dt")         opt.dt        = std::atof(value);
    else if (flag == "--domain")     opt.domain    = std::atof(value);
    else if (flag == "--out")        opt.out       = value;
    else if (flag == "--seed")       opt.seed      = (unsigned)std::strtoul(value, nullptr, 10);
    else if (flag == "--spring")     opt.contact.spring   = std::atof(value);
    else if (flag == "--dashpot")    opt.contact.dashpot  = std::atof(value);
    else if (flag == "--friction")   opt.contact.friction = std::atof(value);
    else { std::fprintf(stderr, "unknown flag %s\n", flag.c_str()); usage(); return 2; }
    ++a;   /* consumed the value */
  }

  if (opt.steps < 0 || opt.bodies < 0 || !(opt.dt > 0.0) || !(opt.domain > 0.0)) {
    std::fprintf(stderr, "steps, bodies must be >= 0 and dt, domain > 0\n");
    return 2;
  }

  const double D = opt.domain;
  std::vector<Particle2D> ps;
  addWall(ps, -1.0, -1.0,  D + 1.0, 0.0,   0);   /* floor      */
  addWall(ps, -1.0, -1.0,  0.0,     D + 1.0, 10); /* left wall  */
  addWall(ps,  D,   -1.0,  D + 1.0, D + 1.0, 20); /* right wall */

  /* Bodies start inside the box. The top is open, so a body placed above D
   * would free-fall from outside and pass through the floor without ever
   * touching a wall from the inside. */
  std::mt19937 rng(opt.seed);
  std::uniform_real_distribution<double> xy(1.0, D - 1.0);
  std::uniform_real_distribution<double> siz(0.15, 0.35);

  for (int i = 0; i < opt.bodies; ++i) {
    const double size = siz(rng);
    const Vec2   c(xy(rng), xy(rng));

    if (i % 2 == 0) {
      ps.push_back(makeCircle(100 + i, c, size, 1000.0, (iREAL)opt.contact.friction));
    } else {
      const std::array<Vec2, 3> v = { c + Vec2(-size, -size),
                                      c + Vec2(size, -size),
                                      c + Vec2(0.0, size) };
      ps.push_back(makeTriangle(100 + i, v, 1000.0, (iREAL)opt.contact.friction));
    }
  }

  Engine2D engine(ps);
  engine.params().stiffness    = opt.contact.spring;
  engine.params().dampingRatio = opt.contact.dashpot;

  std::printf("dem2d: bodies=%d (moving %d) domain=%.3g steps=%d dt=%.3g seed=%u\n",
              (int)ps.size(), opt.bodies, D, opt.steps, opt.dt, opt.seed);

  int frame = 0;
  if (opt.saveEvery > 0) {
    writeDem2DToVTK(opt.out + "/", frame++, engine.particles());
  }

  for (int s = 0; s < opt.steps; ++s) {
    engine.step((iREAL)opt.dt);

    int badId = -1;
    const char* field = "";
    if (!finiteState(engine.particles(), badId, field)) {
      std::fprintf(stderr, "step %d: non-finite %s on body %d\n", s, field, badId);
      return 1;
    }

    if (opt.saveEvery > 0 && (s + 1) % opt.saveEvery == 0) {
      writeDem2DToVTK(opt.out + "/", frame++, engine.particles());
    }
  }

  /* One summary line, matching what dem3d reports per step. */
  iREAL maxSpeed = 0.0;
  int   settled  = 0;
  for (size_t i = 0; i < engine.particles().size(); ++i) {
    const Particle2D& p = engine.particles()[i];
    if (p.isObstacle) continue;
    const iREAL v = length(p.velocity);
    if (v > maxSpeed) maxSpeed = v;
    if (v < 0.5) ++settled;
  }

  std::printf("contacts=%d settled=%d/%d max|v|=%g frames=%d out=%s\n",
              (int)engine.contacts().size(), settled, opt.bodies,
              maxSpeed, frame, opt.out.c_str());
  return 0;
}
