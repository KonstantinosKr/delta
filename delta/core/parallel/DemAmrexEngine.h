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

#ifndef DELTA_CORE_PARALLEL_DEM_AMREX_ENGINE_H_
#define DELTA_CORE_PARALLEL_DEM_AMREX_ENGINE_H_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <AMReX_BoxArray.H>
#include <AMReX_DistributionMapping.H>
#include <AMReX_Geometry.H>

#include "../data/ParticleRecord.h"
#include "DemParticleContainer.h"

namespace delta {
  namespace core {
    namespace parallel {
      class DemAmrexEngine;
      struct DemParallelConfig;
    }
  }
}

/*
 * Grid/ghost/bin configuration of the parallel engine.
 *
 *   maxGridSize     BoxArray::maxSize() in cells; <= 0 keeps one box, i.e. a
 *                   single tile per rank with no internal boundaries.
 *   nneighbor       AMReX ghost halo width in cells (m_num_neighbor_cells).
 *   cellsPerReach   grid resolution: dx is aimed at reach/cellsPerReach. 1.0 is
 *                   the sound minimum for nneighbor=1; larger values are a
 *                   deliberately under-sized halo (used by the sizing test).
 *   margin          domain padding around the particle bounding box; < 0 picks
 *                   2*max halo diameter, which is where particles can reach
 *                   without leaving the domain in one step at test time scales.
 *   enforceSizing   abort on unsound nneighbor*dx / bin_size; false only for
 *                   diagnostics that need to run an unsound grid on purpose.
 */
namespace delta {
  namespace core {
    namespace parallel {

struct DemParallelConfig
{
  int maxGridSize = 32;
  int nneighbor = 1;
  double cellsPerReach = 1.0;
  double margin = -1.0;
  bool enforceSizing = true;
};

    }
  }
}

/*
 * Parallel AMReX-backed counterpart of delta::core::Engine.
 *
 * Per step it runs the serial physics in the serial order:
 *   clearNeighbors -> Redistribute -> fillNeighbors -> updateNeighbors ->
 *   buildNeighborList(CheckPair, reach) ->
 *   per tile: resolve contacts against the neighbour list and accumulate
 *   force/torque for every real particle, then apply them ->
 *   gravity + position + orientation update.
 *
 * Roles: the local real particle is always the A argument of the detection and
 * force kernels and the partner (real, ghost or obstacle) is B. Serial stores
 * each contact twice, once per endpoint, with the normal negated for the second
 * endpoint; sphere(A,B) is the exact bit-level mirror of sphere(B,A), so running
 * the local particle as A with the normal un-negated reproduces serial's
 * endpoint force bit for bit and no ghost force exchange is needed.
 *
 * Obstacles are not put in the container: the cross-container pair path in AMReX
 * requires compatible iterators, and the serial obstacle loop is a local
 * sphere x mesh scan. Their ParticleRecords are replicated read-only on every
 * rank and consulted by the per-particle loop.
 */
class delta::core::parallel::DemAmrexEngine
{
public:
  typedef delta::core::data::ParticleRecord Record;

  DemAmrexEngine(const std::vector<Record>& records,
                 const DemParallelConfig& config = DemParallelConfig());
  ~DemAmrexEngine();

  DemAmrexEngine(const DemAmrexEngine&) = delete;
  DemAmrexEngine& operator=(const DemAmrexEngine&) = delete;

  /* Builds grid, container and initial particle state (collective). */
  void initFrom(const std::vector<Record>& records,
                const DemParallelConfig& config = DemParallelConfig());

  /* Contact detection + forces + gravity + position/orientation integration. */
  void step(iREAL dt, const std::array<iREAL, 3>& gravity);

  /* Serial Engine::contactDetection()+deriveForces(): one force pass, no move. */
  void detectAndDeriveForces(iREAL dt);

  /* Serial Engine::updatePosition() (gravity, centre, rotation matrix). */
  void integrate(iREAL dt, const std::array<iREAL, 3>& gravity);

  /* Writes the current state back into records in initFrom() order (collective);
   * every rank ends up with the identical full set. */
  void syncTo(std::vector<Record>& out) const;

  /* ---- diagnostics ----------------------------------------------------- */
  /* Global number of valid particles (collective). */
  long long particleCount() const;
  long long expectedParticleCount() const { return _expectedParticles; }
  /* Global number of unique contacting pairs of the last force pass. */
  long long contactPairCount() const;
  /* Unique contacting (globalId,globalId) pairs resolved by this rank. */
  void contactPairs(std::vector<std::pair<long long, long long> >& out) const;
  /* Global max |force| of the last force pass. */
  iREAL maxForceMagnitude() const;
  /* Global max |torque| of the last force pass. */
  iREAL maxTorqueMagnitude() const;
  /* nullptr when nneighbor*dx >= reach and bin_size >= 2*reach. */
  const char* sizingDiagnostic() const
  {
    return _sizingDiagnostic.empty() ? nullptr : _sizingDiagnostic.c_str();
  }
  bool sizingIsSound() const { return sizingDiagnostic() == nullptr; }

  /* Local lookup of a delta global id in this rank's own tiles (false when the
   * rank does not own the particle). Every particle is owned by exactly one
   * rank, which is what syncTo() relies on. */
  bool findParticle(int globalId, int& lev, int& grid, int& tile, int& index) const;

  iREAL reach() const { return _reach; }
  iREAL dx() const { return _dxMin; }
  int nneighbor() const { return _config.nneighbor; }
  const DemParticleContainer& container() const { return *_pc; }
  const std::vector<Record>& obstacles() const { return _obstacles; }

  /* Writes this engine's AMReX domain-decomposition boxes (the grid the
   * particles actually live on) as ParaView XML: <path>grid_<step>.vtu plus a
   * grid.pvd time series. The writer is rank-serial, so only the I/O rank
   * writes; the call itself is safe from every rank. */
  void writeToVTK(const std::string& path, int step) const;

private:
  void buildGrid(const std::vector<const Record*>& freeParticles);
  void seedParticles(const std::vector<const Record*>& freeParticles);
  void checkParticleCount(const char* where) const;
  void forcePass(iREAL dt);

  DemParallelConfig          _config;
  amrex::Geometry            _geom;
  amrex::BoxArray            _ba;
  amrex::DistributionMapping _dmap;
  std::unique_ptr<DemParticleContainer> _pc;

  std::vector<Record> _obstacles;
  std::map<int, int>  _indexOfGlobalId;   /* delta global id -> position in input */

  long long _expectedParticles = 0;
  iREAL     _reach = 0.0;
  iREAL     _dxMin = 0.0;
  iREAL     _maxHalo = 0.0;
  iREAL     _maxForce = 0.0;
  iREAL     _maxTorque = 0.0;
  long long _contactPairs = 0;
  std::vector<std::pair<long long, long long> > _pairList;
  /* Accumulated (force, torque) per tile between the two force-pass loops. */
  std::map<std::pair<int, int>, std::vector<iREAL> > _forceCache;
  std::string _sizingDiagnostic;
};

#endif /* DELTA_CORE_PARALLEL_DEM_AMREX_ENGINE_H_ */
