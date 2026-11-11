/*
 * ContactParams.h
 *
 * Runtime contact-law parameters, shared by the 2D and 3D solvers.
 *
 * The 3D core used to compile these in as SSPRING/SDAMPER macros, which meant a
 * scenario file's SPRING/DASHPOT block was read, printed, and then ignored. The
 * defaults below are exactly the old macro values, so a run that does not set
 * anything is bit-identical to before.
 *
 * Dimension-free on purpose: the 2D core keeps its own ContactParams2D because
 * it also carries a tangential stiffness and a Vec2 gravity, and forcing one
 * struct on both would mean a union of fields neither uses.
 */

#ifndef DELTA_CORE_CONTACTPARAMS_H_
#define DELTA_CORE_CONTACTPARAMS_H_

#include <array>

#if !defined(iREAL)
#define iREAL double
#endif

namespace delta {
  namespace core {

struct ContactParams
{
  /* Normal spring stiffness and the dimensionless damping ratio. The damper
   * coefficient is 2 * dampingRatio * sqrt(stiffness * m_effective), the same
   * form both solvers already used. */
  iREAL spring      = 2.0e5;   /* was #define SSPRING 2E5  */
  iREAL dashpot     = 0.5;     /* was #define SDAMPER 0.5 */

  /* Coulomb limit, applied to the tangential force as mu * |normal force|. */
  iREAL friction    = 0.5;     /* was #define FRICTION 0.5 */

  /* Body force; zero keeps gravity off, which is what the serial engine's
   * default was. */
  std::array<iREAL, 3> gravity = {{0.0, 0.0, 0.0}};
};

  }  /* namespace core */
}  /* namespace delta */

#endif /* DELTA_CORE_CONTACTPARAMS_H_ */
