#pragma once
// Broadphase grid morphologies, thesis chapter 6.
//
//   NoGrid   6.3 "deteriorated space grid": one mono-cell (8 vertices), i.e.
//            every body is compared against every other body.
//   Regular  6.3: uniformly refine every cell of every level until the cell
//            edge is just larger than the smallest particle diameter.
//   Adaptive 6.3: refine only around bodies; empty regions stay coarse,
//            particles are dropped/lifted across levels (6.4.1) and coarse
//            bodies are inherited as "virtual particles" (6.4.2).
//   Reluctant 6.3 + chapter 7: refine only when a cell holds >= 2 bodies that
//            are approaching (v_BA < 0), so the grid stays as coarse as
//            possible and the admissible dt stays large.
//
// Implemented with AMReX BoxArray / DistributionMapping / MultiFab / Geometry.

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <AMReX_BoxArray.H>
#include <AMReX_DistributionMapping.H>
#include <AMReX_Geometry.H>
#include <AMReX_MultiFab.H>
#include <AMReX_RealBox.H>

#include "contact/Types.hpp"

namespace contact {

enum class GridMode { NoGrid, Regular, Adaptive, Reluctant };

[[nodiscard]] const char* toString(GridMode m) noexcept;
[[nodiscard]] bool gridModeFromString(const std::string& s, GridMode& out) noexcept;

struct GridParams {
    int max_level{4};          // deepest refinement level (level 0 = root)
    int refine_factor{3};      // 3^d children per cell (thesis 6.1)
    Real size_slack{1.0};      // particle level: cell_edge >= size_slack * diameter
    Real approach_gap{1.0};    // reluctant: refine when gap < approach_gap * cell
    int min_bodies_to_refine{2};
    bool inherit_coarse{true}; // virtual particles (6.4.2)
    Real halo_epsilon{0.1};    // e = 0.1 * radius, halo = radius * (1 + e)
};

struct GridStats {
    int levels{0};
    long cells_total{0};        // all cells over all levels
    long cells_occupied{0};     // cells that hold at least one body
    long cells_refused{0};      // reluctant: cells left coarse on purpose
    long particles_real{0};
    long virtual_links{0};
    long cell_pair_checks{0};
    long raw_pairs{0};          // before dedup
    long unique_pairs{0};
    long refinements{0};
    long coarsenings{0};

    void reset() noexcept { *this = GridStats{}; }
};

class GridHierarchy {
  public:
    GridHierarchy(const amrex::Geometry& geom, GridParams params, GridMode mode);

    // Out-of-line: Level is an incomplete type here.
    ~GridHierarchy();
    GridHierarchy(const GridHierarchy&) = delete;
    GridHierarchy& operator=(const GridHierarchy&) = delete;

    // Rebuild the morphology from the current body positions. Called once per
    // step for Adaptive / Reluctant; the regular grid only rebuilds when the
    // particle size range changes; NoGrid is a single cell.
    void build(const std::vector<Particle>& bodies, Real dt);

    // Unique candidate pairs (i < j) for the narrowphase.
    void candidatePairs(std::vector<ContactPair>& out) const;

    [[nodiscard]] GridMode mode() const noexcept { return mode_; }
    [[nodiscard]] const GridParams& params() const noexcept { return params_; }
    [[nodiscard]] const GridStats& stats() const noexcept { return stats_; }
    [[nodiscard]] int levelOf(int body_id) const noexcept;
    [[nodiscard]] Real cellSize(int level) const noexcept;
    // Smallest cell edge over all levels: h_min of the dt formula (thesis 7.2).
    [[nodiscard]] Real minCellSize() const noexcept;
    [[nodiscard]] Real maxCellSize() const noexcept;

    void printProfile(std::ostream& os) const;

  private:
    struct Level;

    void buildNoGrid(const std::vector<Particle>& bodies);
    void buildRegular(const std::vector<Particle>& bodies);
    void buildAdaptive(const std::vector<Particle>& bodies, Real dt);
    void buildReluctant(const std::vector<Particle>& bodies, Real dt);

    amrex::Geometry geom_;
    GridParams params_;
    GridMode mode_;
    GridStats stats_;
    std::vector<std::unique_ptr<Level>> levels_;
};

}  // namespace contact
