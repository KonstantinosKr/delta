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

#ifndef DELTA_CORE_PARALLEL_DEM_PARTICLE_CONTAINER_H_
#define DELTA_CORE_PARALLEL_DEM_PARTICLE_CONTAINER_H_

#include <cstdio>
#include <cstdlib>

#include <AMReX_NeighborParticles.H>
#include <AMReX_Particles.H>

namespace delta {
  namespace core {
    namespace parallel {
      class DemParticleContainer;
    }
  }
}

/*
 * Per-particle component layout (docs/recon/amrex-fit.md 4). 45 reals + 3 ints:
 *   linVel3 angVel3 refAngVel3 force3 torque3 orientation9 inverse9 inertia9
 *   mass1 epsilon1 diameter1                                      = 45 reals
 *   material friction globalId                                    = 3  ints
 *
 * The blocks are contiguous runs of single-component SoA arrays, so component
 * k of e.g. linVel is ptd.rdata(DemRealComp::linVel + k).
 *
 * Namespaces: ParticleTileData::rdata(k)/idata(k) are 0-based over the array
 * components (ParticleTile::getParticleTileData fills m_rdata[i] from
 * SoA::GetRealData(i)). AoS particles are the other namespace and are offset by
 * AMREX_SPACEDIM + NStructReal, i.e. p.rdata(3 + k) for this container.
 *
 * globalId carries the delta global particle id; AMReX's own p.id() stays
 * rank-assigned (NextID) and is not used to identify delta particles.
 */
namespace delta {
  namespace core {
    namespace parallel {

struct DemRealComp
{
  enum {
    linVel = 0, angVel = 3, refAngVel = 6, force = 9, torque = 12,
    orientation = 15, inverse = 24, inertia = 33,
    mass = 42, epsilon = 43, diameter = 44,
    ncomp = 45
  };
};

struct DemIntComp
{
  enum { material = 0, friction = 1, globalId = 2, ncomp = 3 };
};

    }
  }
}

class delta::core::parallel::DemParticleContainer
    : public amrex::NeighborParticleContainer<0, 0, DemRealComp::ncomp, DemIntComp::ncomp>
{
public:
  using Base = amrex::NeighborParticleContainer<0, 0, DemRealComp::ncomp, DemIntComp::ncomp>;
  using Base::Base;

  /*
   * m_neighbor_list / neighbor_list are protected in AMReX; a derived class is
   * the supported way to read them (AMReX's own MDParticleContainer test does
   * the same). neighbor_list is the packed [count, ids...] form and only exists
   * on the CPU build.
   */
  const amrex::NeighborList<typename Base::ParticleType>& neighborList(
      int lev, const PairIndex& key) const
  {
    return m_neighbor_list[lev].at(key);
  }

  /*
   * True iff buildNeighborList() produced an entry for this tile. It only
   * creates entries for tiles that exist in the particle data, and a BoxArray
   * usually has far more boxes than populated tiles, so callers must not assume
   * every MakeMFIter() tile has a list.
   */
  bool hasNeighborList(int lev, const PairIndex& key) const
  {
    const auto& levpairs = m_neighbor_list[lev];
    return levpairs.find(key) != levpairs.end();
  }

#ifndef AMREX_USE_GPU
  const Base::IntVector& packedNeighborList(int lev, const PairIndex& key) const
  {
    return neighbor_list[lev].at(key);
  }
#endif

  /*
   * Ghost/bin sizing soundness.
   *
   * Every partner within `reach` of a tile particle is found iff
   *   (a) nneighbor * dx >= reach   - the exchanged halo really contains it, and
   *   (b) bin_size    >= reach      - the +/-1 bin search reaches it.
   * (a) is the silent one: AMReX's own guard is AMREX_ASSERT(numParticles-
   * OutOfRange(...)) and this AMReX install is configured with
   * AMReX_ASSERTIONS=OFF, so an under-sized halo drops contacts without any
   * diagnostic. Delta therefore checks both conditions itself and refuses to
   * run an unsound configuration unless the caller explicitly asks for it.
   *
   * Returns nullptr when the sizing is sound, otherwise a static message
   * naming the violated condition.
   */
  static const char* sizingDiagnostic(amrex::Real reach, amrex::Real dx,
                                     int nneighbor, amrex::Real bin_size)
  {
    if (!(reach > 0.0)) {
      return "contact reach is not positive (every particle has zero radius+epsilon)";
    }
    if (!(dx > 0.0)) {
      return "mesh spacing dx is not positive";
    }
    if (nneighbor < 1) {
      return "nneighbor must be at least one ghost cell";
    }
    if (!(bin_size >= 2.0 * reach)) {
      return "bin_size < 2*reach: AMReX shrinks the requested bin width to "
             "extent/ceil(extent/bin_size), so the +/-1 bin search stops covering reach";
    }
    if (static_cast<amrex::Real>(nneighbor) * dx < reach) {
      return "nneighbor*dx < reach: the ghost halo does not contain every partner, "
             "contacts would be dropped silently";
    }
    return nullptr;
  }

  /* Same check, aborting instead of returning. */
  static void assertSizing(amrex::Real reach, amrex::Real dx, int nneighbor,
                           amrex::Real bin_size)
  {
    const char* why = sizingDiagnostic(reach, dx, nneighbor, bin_size);
    if (why != nullptr) {
      std::fprintf(stderr, "delta AMReX particle container: unsound neighbour sizing: %s\n", why);
      std::abort();
    }
  }
};

#endif /* DELTA_CORE_PARALLEL_DEM_PARTICLE_CONTAINER_H_ */
