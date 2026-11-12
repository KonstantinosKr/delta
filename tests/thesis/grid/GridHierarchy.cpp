// Broadphase grid morphologies (Krestenitis 2018, thesis chapter 6).
//
//   NoGrid   6.3 "deteriorated space grid": one mono-cell, N(N-1)/2 pairs.
//   Regular  6.3: every cell of every level 0..L_max exists; L_max is the level
//            of the smallest body.
//   Adaptive 6.3 + 6.4.1/6.4.2: only cells that hold (or must contain) a body
//            exist; coarse bodies are inherited downwards as virtual particles.
//   Reluctant 6.3 + ch.7: as Adaptive, but a cell is only refined when it holds
//            >= min_bodies_to_refine bodies and at least one pair in the cell
//            neighbourhood is approaching (v_BA < 0) and close
//            (gap < approach_gap * h_L).  Everything else stays coarse
//            (cells_refused), which is what keeps h_min - and therefore dt_max -
//            large.
//
// ---------------------------------------------------------------------------
// Implementation notes
// ---------------------------------------------------------------------------
// * Index space.  The hierarchy owns its own AMReX index space: level L is a
//   (refine_factor^L)^3 grid of cubic cells over the physical domain given by
//   geom_.  Level 0 is therefore exactly one cell spanning the domain, which is
//   the mono-cell of the thesis, and h_L = h_0 / rf^L with h_0 = domain edge.
//   geom_ supplies the physical domain (ProbLo/ProbHi) and nothing else; the
//   base AMR grid of geom_ (e.g. 32^3) is deliberately not used as level 0,
//   because 6.1/6.3 define the root cell as the whole domain.
//
// * Per level we keep
//       amrex::BoxArray              the cell grid (authoritative cell set)
//       amrex::DistributionMapping   ditto, for the MultiFab
//       amrex::iMultiFab             comp 0 = #real bodies, comp 1 = #virtual
//                                    bodies, comp 2 = refinement flag of the
//                                    last build (occupancy / flags live in AMReX
//                                    data, not in std::vector)
//       std::vector<IntVect> cells   lexicographically sorted cell lower
//                                    corners: the cell linearisation.  Its slot
//                                    index is arithmetic (i*n+j)*n+k for a
//                                    complete level and a binary search for a
//                                    sparse one (a CSR-style index on top of the
//                                    BoxArray; amrex::DenseBins would give the
//                                    same thing but with an ordering we cannot
//                                    pin down, and we need determinism)
//       std::vector<std::vector<int>> realBins / virtBins   CSR bins of body ids
//   Cell construction itself always goes through amrex::Box / IntVect
//   arithmetic (Box(IntVect,IntVect), amrex::coarsen, BoxArray::refine,
//   BoxArray::contains).
//
//   Two BoxArray layouts.  A *complete* level (Regular, and level 0 everywhere)
//   is the uniform grid over the level domain, block-partitioned into 8^3-cell
//   boxes; its MultiFab index space is the cell grid, one MultiFab cell per grid
//   cell, but a 19,683-cell level costs 64 FABs instead of 19,683.  A *sparse*
//   level (Adaptive/Reluctant) is the set of active cells as single-cell boxes,
//   because there is no box that covers them without covering empty space.
//
// * Sign convention of v_BA.  Thesis 6.3 writes
//       v_BA = (v_A . d_BA) - (v_B . d_BA),   d_BA = c_B - c_A
//   and states "v_BA < 0 means approaching" (section 3 of SPEC.md repeats this:
//   "separation when >= 0").  Those two statements are sign-inconsistent: with
//   d_BA = c_B - c_A, bodies closing in give (v_A - v_B).d_BA > 0.  We keep the
//   predicate (v_BA < 0 <=> approaching, v_BA >= 0 <=> separating) because that
//   is the semantic used by the dt scheme, and evaluate it on the distance line
//   measured from B to A, d_AB = c_A - c_B:
//       v_BA = (v_A . d_AB) - (v_B . d_AB) = -(v_A - v_B).(c_B - c_A),
//   i.e. v_BA < 0 exactly when the centre distance shrinks.  This is the only
//   interpretation under which "bodies moving apart are not refined" - the
//   property the reluctant variant exists for - actually holds.
//
// * No-false-negative argument (the hard contract).
//   (i)  Same level L: every real body on L satisfies d <= h_L (size_slack <= 1;
//        the stencil radius below is widened automatically if it does not), so
//        two overlapping spheres on L have centre distance <= r_a + r_b <= h_L
//        and hence live in the same or in face/diagonal neighbouring cells.
//        The forward half-stencil visits each unordered cell pair exactly once.
//   (ii) Different levels: a pair is emitted at the level of its *finest*
//        member.  Let A be real at La < Lb and B real at Lb in cell C.  If the
//        spheres overlap, |c_A - c_B| <= r_A + r_B <= r_A + m_C where m_C is the
//        largest real radius in C, so per axis the gap between c_A and C is at
//        most r_A + m_C: A's AABB grown by m_C always overlaps C, so A becomes a
//        virtual particle of C and A-vs-B is compared there.  B is never
//        inherited upwards, so the pair is emitted once.
//   (iii) Reluctant: bodies that are refused a refinement simply stay on the
//        coarser level, and (i)/(ii) apply at the level where they actually
//        live.  Refinement only ever *reduces* the work; it is never needed for
//        correctness.
//   Therefore candidatePairs() is a superset of the true contact set in every
//   mode, independent of the refinement criterion.
//
// * Statistics semantics.
//     cells_total        existing cells over all levels; a Regular grid owns the
//                        complete uniform grid at every level, empty or not
//     cells_occupied     cells holding >= 1 real body
//     cell_pair_checks   neighbour cell pairs examined *with content*; empty
//                        boxes are skipped, their cost is already visible in
//                        cells_total
//     raw_pairs          emissions before dedup (a pair can be emitted more than
//                        once across cells/levels, e.g. through the stencil)
//     unique_pairs       after sorting and dedup - the size of candidatePairs()
//     refinements/coarsenings  per-level cell-count deltas vs the previous
//                        build; zero on the first build
//     h_min / h_max      cellSize(levels-1) / cellSize(0), i.e. the finest level
//                        that actually exists vs the root.  A reluctant build
//                        that never refined reports h_min = h_max = domain edge,
//                        which is exactly the dt benefit the variant is after.
//
// * Reluctant rest tolerance.  A pair with |v_BA| <= 1e-12 (in particular a pair
//   of bodies at rest, where v_BA is exactly 0) is treated as not clearly
//   separating and may refine.  Without that, a static pile would never refine
//   and the variant would silently degrade to NoGrid.
// ---------------------------------------------------------------------------

#include "contact/Grid.hpp"

#include <AMReX_Box.H>
#include <AMReX_BoxArray.H>
#include <AMReX_BoxList.H>
#include <AMReX_DistributionMapping.H>
#include <AMReX_MFIter.H>
#include <AMReX_iMultiFab.H>

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace contact {
namespace {

constexpr int kDim = 3;  // AMREX_SPACEDIM must be 3 for this build
static_assert(AMREX_SPACEDIM == 3, "contact::GridHierarchy is 3D-only");

[[nodiscard]] Real powInt(Real base, int e) noexcept {
    Real r = 1.0;
    for (int i = 0; i < e; ++i) r *= base;
    return r;
}

[[nodiscard]] bool iequals(const std::string& a, const char* b) {
    std::size_t n = 0;
    while (b[n] != '\0') ++n;
    if (a.size() != n) return false;
    for (std::size_t i = 0; i < n; ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

// Lexicographic (i,j,k) order; used for the deterministic cell linearisation.
[[nodiscard]] bool ivLess(const amrex::IntVect& a, const amrex::IntVect& b) noexcept {
    for (int d = 0; d < kDim; ++d) {
        if (a[d] != b[d]) return a[d] < b[d];
    }
    return false;
}

// Forward half of the (2R+1)^3 cell stencil: exactly one of {+o, -o} is kept, so
// every unordered pair of neighbouring cells is examined once.  R = 1 yields the
// 13 offsets of the 26-neighbourhood (thesis 6.2/6.3).
[[nodiscard]] std::vector<amrex::IntVect> forwardStencil(int R) {
    std::vector<amrex::IntVect> out;
    for (int i = -R; i <= R; ++i) {
        for (int j = -R; j <= R; ++j) {
            for (int k = -R; k <= R; ++k) {
                if (i == 0 && j == 0 && k == 0) continue;
                if (!(i > 0 || (i == 0 && (j > 0 || (j == 0 && k > 0))))) continue;
                out.push_back(amrex::IntVect(i, j, k));
            }
        }
    }
    return out;
}

// Thesis 6.1/6.3 body -> level rule ("drop", 6.4.1): the finest level whose cell
// edge is still at least size_slack * diameter; max_level if none qualifies.
[[nodiscard]] int naturalLevel(Real h0, const GridParams& p, Real diameter) noexcept {
    const Real rf = static_cast<Real>(p.refine_factor);
    for (int L = p.max_level; L >= 0; --L) {
        const Real h = h0 / powInt(rf, L);
        if (h >= p.size_slack * diameter) return L;
    }
    return p.max_level;
}

// Index of the cell of a level with edge h (n cells per direction) hosting a
// point; clamped into the level so that stray particles outside the domain
// cannot corrupt the bins.
[[nodiscard]] amrex::IntVect cellOfPoint(const std::array<Real, 3>& plo, Real h, int n,
                                        const Vec3& x) noexcept {
    amrex::IntVect iv;
    for (int d = 0; d < kDim; ++d) {
        const Real t = (x[d] - plo[d]) / h;
        int i = static_cast<int>(std::floor(t));
        if (i < 0) i = 0;
        if (i >= n) i = n - 1;
        iv[d] = i;
    }
    return iv;
}

}  // namespace

// ---------------------------------------------------------------------------
// Level
// ---------------------------------------------------------------------------
// Definition of the private nested type fixed by Grid.hpp.  It owns the AMReX
// cell grid of one refinement level plus the CSR bins and the pairs emitted on
// that level.

struct GridHierarchy::Level {
    int index{0};
    Real h{1.0};
    int n{1};
    bool dense{false};  // true: cells == the complete level grid (Regular)
    std::array<Real, 3> plo{{0.0, 0.0, 0.0}};
    const std::vector<Particle>* pop{nullptr};  // non-owning, valid during build

    amrex::Box domain;
    amrex::BoxArray boxes;
    amrex::DistributionMapping dm;
    amrex::iMultiFab flags;  // 0: #real, 1: #virtual, 2: refined flag

    std::vector<amrex::IntVect> cells;  // sorted lower corners, slot order
    std::vector<std::vector<int>> realBins;
    std::vector<std::vector<int>> virtBins;
    std::vector<Real> cellMaxR;  // largest real radius in the cell
    std::vector<int> refineFlag;

    std::vector<ContactPair> pairs;        // raw emissions of this level
    std::vector<ContactPair> mergedPairs;  // level 0 only: final sorted unique set

    int stencilRadius{1};
    long rawPairs{0};
    long cellPairChecks{0};
    long virtualLinks{0};

    // Build-to-build cell counts (level 0 only) for refinements/coarsenings.
    std::vector<long> bldCellCounts;
    std::vector<long> prevCellCounts;

    void define(int idx, Real hh, int nn, const std::array<Real, 3>& lo,
                const std::vector<amrex::Box>& cellBoxes,
                const std::vector<Particle>* bodies, bool denseGrid);
    [[nodiscard]] int slot(const amrex::IntVect& iv) const noexcept;
    void assignReal(int id, const amrex::IntVect& iv);
    void addPair(int a, int b) noexcept;
    void fillFlags();
    void inheritFrom(const Level& coarse, int ratio, bool materialise);
    void buildPairs();
    void flagRefinedCells(const std::vector<char>& mark);
    void setStencilRadius();

    void cellBounds(int c, Real* lo, Real* hi) const noexcept;
    [[nodiscard]] bool bodyOverlapsCell(int c, const Particle& p, Real margin) const noexcept;

    static void finalise(std::vector<std::unique_ptr<Level>>& lv,
                         const std::vector<long>& prevCounts, GridStats& st);
};

void GridHierarchy::Level::define(int idx, Real hh, int nn,
                                  const std::array<Real, 3>& lo,
                                  const std::vector<amrex::Box>& cellBoxes,
                                  const std::vector<Particle>* bodies, bool denseGrid) {
    index = idx;
    h = hh;
    n = nn;
    plo = lo;
    pop = bodies;
    dense = denseGrid;

    domain = amrex::Box(amrex::IntVect(0), amrex::IntVect(nn - 1));

    if (dense) {
        // Complete level grid (Regular, thesis 6.3): the BoxArray is the uniform
        // grid over the level domain, block-partitioned (8^3 cells per box) the
        // way AMReX represents a uniform mesh.  Its index space *is* the cell
        // grid, one cell per cell, so the occupancy MultiFab below is per cell
        // while the number of FABs stays O((n/8)^3) instead of O(n^3) - a
        // 19,683-cell level costs 64 boxes, not 19,683.
        if (!cellBoxes.empty()) {
            throw std::runtime_error("GridHierarchy: dense level takes no cell list");
        }
        const int block = (nn < 8) ? nn : 8;
        boxes = amrex::BoxArray(domain);
        boxes.maxSize(amrex::IntVect(block));
        dm = amrex::DistributionMapping(boxes);
        flags.define(boxes, dm, 3, 0);

        // Lexicographic (i,j,k) linearisation == cell slot index; O(1) lookup.
        const std::size_t nc = static_cast<std::size_t>(nn) * nn * nn;
        cells.resize(nc);
        std::size_t s = 0;
        for (int i = 0; i < nn; ++i) {
            for (int j = 0; j < nn; ++j) {
                for (int k = 0; k < nn; ++k) cells[s++] = amrex::IntVect(i, j, k);
            }
        }
        realBins.assign(nc, std::vector<int>{});
        virtBins.assign(nc, std::vector<int>{});
        cellMaxR.assign(nc, 0.0);
        refineFlag.assign(nc, 0);
    } else {
        std::vector<amrex::Box> bs = cellBoxes;
        std::sort(bs.begin(), bs.end(), [](const amrex::Box& a, const amrex::Box& b) {
            return ivLess(a.smallEnd(), b.smallEnd());
        });
        // Reject duplicates early: BoxArray would silently keep them and the cell
        // linearisation (and therefore the pair count) would be ambiguous.
        for (std::size_t i = 1; i < bs.size(); ++i) {
            if (bs[i].smallEnd() == bs[i - 1].smallEnd()) {
                throw std::runtime_error("GridHierarchy: duplicate cell at level " +
                                         std::to_string(idx));
            }
        }
        // amrex::Vector is the AMReX container BoxList wants; it derives from
        // std::vector, so the sorted cell list converts directly.
        amrex::Vector<amrex::Box> ambs(bs.begin(), bs.end());
        boxes.define(amrex::BoxList(std::move(ambs)));
        dm = amrex::DistributionMapping(boxes);
        flags.define(boxes, dm, 3, 0);

        cells.resize(bs.size());
        for (std::size_t i = 0; i < bs.size(); ++i) cells[i] = bs[i].smallEnd();

        realBins.assign(bs.size(), std::vector<int>{});
        virtBins.assign(bs.size(), std::vector<int>{});
        cellMaxR.assign(bs.size(), 0.0);
        refineFlag.assign(bs.size(), 0);
    }
    pairs.clear();
    mergedPairs.clear();
    rawPairs = 0;
    cellPairChecks = 0;
    virtualLinks = 0;
}

int GridHierarchy::Level::slot(const amrex::IntVect& iv) const noexcept {
    if (dense) {
        for (int d = 0; d < kDim; ++d) {
            if (iv[d] < 0 || iv[d] >= n) return -1;
        }
        return (iv[0] * n + iv[1]) * n + iv[2];
    }
    const auto it = std::lower_bound(cells.begin(), cells.end(), iv, ivLess);
    if (it == cells.end()) return -1;
    if ((*it)[0] != iv[0] || (*it)[1] != iv[1] || (*it)[2] != iv[2]) return -1;
    return static_cast<int>(it - cells.begin());
}

void GridHierarchy::Level::assignReal(int id, const amrex::IntVect& iv) {
    const int s = slot(iv);
    if (s < 0) {
        // Cannot happen: the cell sets are built from the very bodies binned
        // here.  A silent drop would be a false negative, so make it loud.
        throw std::runtime_error("GridHierarchy: body " + std::to_string(id) +
                                 " has no cell on level " + std::to_string(index));
    }
    realBins[s].push_back(id);
    const Real r = (*pop)[id].radius;
    if (r > cellMaxR[s]) cellMaxR[s] = r;
}

void GridHierarchy::Level::addPair(int a, int b) noexcept {
    if (a == b) return;
    ContactPair p;
    p.i = (a < b) ? a : b;
    p.j = (a < b) ? b : a;
    pairs.push_back(p);
    ++rawPairs;
}

void GridHierarchy::Level::cellBounds(int c, Real* lo, Real* hi) const noexcept {
    for (int d = 0; d < kDim; ++d) {
        lo[d] = plo[d] + static_cast<Real>(cells[c][d]) * h;
        hi[d] = lo[d] + h;
    }
}

// Conservative AABB test: body p (real radius) inflated by `margin` against the
// physical box of cell c.  Equality counts as overlap.
bool GridHierarchy::Level::bodyOverlapsCell(int c, const Particle& p, Real margin) const noexcept {
    Real lo[3];
    Real hi[3];
    cellBounds(c, lo, hi);
    for (int d = 0; d < kDim; ++d) {
        if (p.pos[d] + p.radius + margin < lo[d]) return false;
        if (p.pos[d] - p.radius - margin > hi[d]) return false;
    }
    return true;
}

void GridHierarchy::Level::fillFlags() {
    flags.setVal(0);
    for (amrex::MFIter mfi(flags); mfi.isValid(); ++mfi) {
        const amrex::Box& bx = mfi.validbox();
        auto f = flags[mfi].array();
        for (int k = bx.smallEnd(2); k <= bx.bigEnd(2); ++k) {
            for (int j = bx.smallEnd(1); j <= bx.bigEnd(1); ++j) {
                for (int i = bx.smallEnd(0); i <= bx.bigEnd(0); ++i) {
                    const int s = slot(amrex::IntVect(i, j, k));
                    if (s < 0) continue;
                    f(i, j, k, 0) = static_cast<int>(realBins[s].size());
                    f(i, j, k, 1) = static_cast<int>(virtBins[s].size());
                    f(i, j, k, 2) = refineFlag[s];
                }
            }
        }
    }
}

// Stencil half-width from the largest body that actually resides on this level:
// two overlapping spheres on L are at most 2*maxR apart, i.e. at most
// ceil(2*maxR/h) cells.  With size_slack = 1 this is the 13-offset
// 26-neighbourhood; it widens automatically for smaller size_slack.  A radius
// beyond 4 means the population is degenerate for this level (a body wider than
// several cells, which the level rule of 6.1 should have prevented): refuse it
// instead of silently walking a huge stencil.
void GridHierarchy::Level::setStencilRadius() {
    Real maxR = 0.0;
    for (const Real r : cellMaxR) maxR = std::max(maxR, r);
    const int rreq = static_cast<int>(std::ceil(2.0 * maxR / h));
    if (rreq > 4) {
        throw std::runtime_error(
            "GridHierarchy: level " + std::to_string(index) + " needs a cell stencil of " +
            std::to_string(rreq) + " (largest body diameter " + std::to_string(2.0 * maxR) +
            " vs cell edge " + std::to_string(h) +
            "); body does not fit its level - check size_slack / max_level");
    }
    stencilRadius = std::max(1, rreq);
}

void GridHierarchy::Level::flagRefinedCells(const std::vector<char>& mark) {
    refineFlag.assign(cells.size(), 0);
    for (std::size_t i = 0; i < mark.size() && i < refineFlag.size(); ++i) {
        refineFlag[i] = mark[i] ? 1 : 0;
    }
}

// Thesis 6.4.2: inherit the coarse level's real bodies into every cell of this
// (finer) level whose body AABB - inflated by the largest real radius in the
// cell - overlaps it.  `materialise` = false computes the same links on the fly
// and emits the real-vs-virtual pairs directly (GridParams::inherit_coarse).
void GridHierarchy::Level::inheritFrom(const Level& coarse, int ratio, bool materialise) {
    if (coarse.realBins.empty()) return;
    for (int c = 0; c < static_cast<int>(cells.size()); ++c) {
        if (realBins[c].empty()) continue;
        const Real m = cellMaxR[c];
        // One cell of margin more than the geometric requirement: the inflated
        // AABB has half width r + m, which covers any partner radius <= m.
        const int g = static_cast<int>(std::ceil(m / coarse.h)) + 1;
        amrex::Box reg = amrex::coarsen(amrex::Box(cells[c], cells[c]), ratio);
        reg.grow(g);
        reg &= coarse.domain;

        for (int k = reg.smallEnd(2); k <= reg.bigEnd(2); ++k) {
            for (int j = reg.smallEnd(1); j <= reg.bigEnd(1); ++j) {
                for (int i = reg.smallEnd(0); i <= reg.bigEnd(0); ++i) {
                    const int s = coarse.slot(amrex::IntVect(i, j, k));
                    if (s < 0) continue;  // cell not present at the coarse level
                    for (const int id : coarse.realBins[s]) {
                        if (!bodyOverlapsCell(c, (*pop)[id], m)) continue;
                        if (materialise) {
                            virtBins[c].push_back(id);
                            ++virtualLinks;
                        } else {
                            for (const int rb : realBins[c]) addPair(id, rb);
                        }
                    }
                }
            }
        }
    }
}

void GridHierarchy::Level::buildPairs() {
    const std::vector<amrex::IntVect> sten = forwardStencil(stencilRadius);
    // Rough upper bound on the emissions of this level (real bodies times the
    // number of cells they are compared against); avoids repeated reallocation.
    std::size_t nreal = 0;
    for (const std::vector<int>& b : realBins) nreal += b.size();
    pairs.reserve(nreal * (1 + sten.size() / 4));

    // Traversal is driven by the AMReX BoxArray / MultiFab (MFIter), not by the
    // std::vector bins, so a regular grid's empty boxes are still visited (and
    // skipped immediately) even though they can hold no pair.
    for (amrex::MFIter mfi(flags); mfi.isValid(); ++mfi) {
        const amrex::Box& bx = mfi.validbox();
        auto f = flags[mfi].array();
        // A sparse level has one cell per box; a dense (Regular) level packs
        // block^3 cells into one box, so walk the box's cells in both cases.
        for (int k0 = bx.smallEnd(2); k0 <= bx.bigEnd(2); ++k0) {
            for (int j0 = bx.smallEnd(1); j0 <= bx.bigEnd(1); ++j0) {
                for (int i0 = bx.smallEnd(0); i0 <= bx.bigEnd(0); ++i0) {
                    const int nr = f(i0, j0, k0, 0);
                    const int nv = f(i0, j0, k0, 1);

                    const int c = slot(amrex::IntVect(i0, j0, k0));
                    if (c < 0) continue;
                    // Empty cells are skipped: their existence is reported in
                    // cells_total (a Regular grid owns the whole uniform grid at
                    // every level), but a 3^3 stencil over ~20k empty boxes per
                    // step can produce no pair.  The thesis counts those boxes as
                    // grid administration cost (6.3, Fig. 6.10); keeping them in
                    // the BoxArray preserves exactly that.
                    if (nr + nv == 0) continue;
                    const std::vector<int>& rc = realBins[c];
                    const std::vector<int>& vc = virtBins[c];

                    // Intra-cell: real vs real (touchVertexLastTime, 6.2).
                    for (std::size_t a = 0; a < rc.size(); ++a) {
                        for (std::size_t b = a + 1; b < rc.size(); ++b) addPair(rc[a], rc[b]);
                    }
                    // Intra-cell: real vs inherited virtual (6.4.2).  Same cell is
                    // sufficient - see note (ii) in the file header.
                    for (const int rb : rc) {
                        for (const int vb : vc) addPair(rb, vb);
                    }

                    // Inter-cell: forward half-stencil, so each unordered cell
                    // pair is examined exactly once (single-touch policy, 6.2).
                    for (const amrex::IntVect& off : sten) {
                        const amrex::IntVect nbv = amrex::IntVect(i0, j0, k0) + off;
                        if (!domain.contains(nbv)) continue;
                        const int s = slot(nbv);
                        if (s < 0) continue;  // cell does not exist on this level
                        ++cellPairChecks;
                        if (realBins[s].empty() && virtBins[s].empty()) continue;
                        for (const int a : rc) {
                            for (const int b : realBins[s]) addPair(a, b);
                        }
                    }
                }
            }
        }
    }
}

void GridHierarchy::Level::finalise(std::vector<std::unique_ptr<Level>>& lv,
                                    const std::vector<long>& prevCounts, GridStats& st) {
    st.levels = static_cast<int>(lv.size());
    st.cells_total = 0;
    st.cells_occupied = 0;
    st.virtual_links = 0;
    st.cell_pair_checks = 0;
    st.raw_pairs = 0;

    std::vector<long> counts(lv.size(), 0);
    std::vector<ContactPair> all;
    for (std::size_t i = 0; i < lv.size(); ++i) {
        const Level& L = *lv[i];
        counts[i] = static_cast<long>(L.cells.size());
        st.cells_total += counts[i];
        for (std::size_t c = 0; c < L.realBins.size(); ++c) {
            if (!L.realBins[c].empty()) ++st.cells_occupied;
        }
        st.virtual_links += L.virtualLinks;
        st.cell_pair_checks += L.cellPairChecks;
        st.raw_pairs += L.rawPairs;
        all.insert(all.end(), L.pairs.begin(), L.pairs.end());
    }

    std::sort(all.begin(), all.end(), [](const ContactPair& a, const ContactPair& b) {
        if (a.i != b.i) return a.i < b.i;
        return a.j < b.j;
    });
    all.erase(std::unique(all.begin(), all.end(), [](const ContactPair& a, const ContactPair& b) {
                  return a.i == b.i && a.j == b.j;
              }),
              all.end());
    st.unique_pairs = static_cast<long>(all.size());

    // refinements / coarsenings: per-level cell-count deltas vs the previous
    // build (zero on the first build).
    st.refinements = 0;
    st.coarsenings = 0;
    if (!prevCounts.empty()) {
        const std::size_t nmax = std::max(prevCounts.size(), counts.size());
        for (std::size_t i = 0; i < nmax; ++i) {
            const long a = (i < prevCounts.size()) ? prevCounts[i] : 0;
            const long b = (i < counts.size()) ? counts[i] : 0;
            if (b > a) st.refinements += b - a;
            else st.coarsenings += a - b;
        }
    }

    if (!lv.empty()) {
        lv[0]->mergedPairs = std::move(all);
        lv[0]->bldCellCounts = counts;
        lv[0]->prevCellCounts = prevCounts;
    }
}

// ---------------------------------------------------------------------------
// GridHierarchy
// ---------------------------------------------------------------------------

const char* toString(GridMode m) noexcept {
    switch (m) {
        case GridMode::NoGrid: return "NoGrid";
        case GridMode::Regular: return "Regular";
        case GridMode::Adaptive: return "Adaptive";
        case GridMode::Reluctant: return "Reluctant";
    }
    return "Unknown";
}

bool gridModeFromString(const std::string& s, GridMode& out) noexcept {
    if (iequals(s, "NoGrid") || iequals(s, "no-grid") || iequals(s, "none")) {
        out = GridMode::NoGrid;
        return true;
    }
    if (iequals(s, "Regular")) {
        out = GridMode::Regular;
        return true;
    }
    if (iequals(s, "Adaptive")) {
        out = GridMode::Adaptive;
        return true;
    }
    if (iequals(s, "Reluctant") || iequals(s, "ReluctantAdaptive") ||
        iequals(s, "reluctant-adaptive")) {
        out = GridMode::Reluctant;
        return true;
    }
    return false;
}

GridHierarchy::GridHierarchy(const amrex::Geometry& geom, GridParams params, GridMode mode)
    : geom_(geom), params_(params), mode_(mode) {
    if (params_.refine_factor < 2) {
        throw std::invalid_argument("GridHierarchy: refine_factor must be >= 2");
    }
    if (params_.max_level < 0) {
        throw std::invalid_argument("GridHierarchy: max_level must be >= 0");
    }
    if (params_.size_slack <= 0.0) {
        throw std::invalid_argument("GridHierarchy: size_slack must be > 0");
    }
}

GridHierarchy::~GridHierarchy() = default;

void GridHierarchy::build(const std::vector<Particle>& bodies, Real dt) {
    stats_.reset();
    // Cell counts of the previous build drive refinements/coarsenings.  Level 0
    // always exists after a build, so it is the carrier of the history (the
    // member list of GridHierarchy is fixed by the header).
    std::vector<long> prev;
    if (!levels_.empty() && !levels_[0]->bldCellCounts.empty()) {
        prev = levels_[0]->bldCellCounts;
    }
    levels_.clear();

    switch (mode_) {
        case GridMode::NoGrid: buildNoGrid(bodies); break;
        case GridMode::Regular: buildRegular(bodies); break;
        case GridMode::Adaptive: buildAdaptive(bodies, dt); break;
        case GridMode::Reluctant: buildReluctant(bodies, dt); break;
    }

    stats_.particles_real = static_cast<long>(bodies.size());
    Level::finalise(levels_, prev, stats_);
}

void GridHierarchy::buildNoGrid(const std::vector<Particle>& bodies) {
    // "Deteriorated space grid": one cell of eight vertices, every body in it.
    std::array<Real, 3> plo{{0.0, 0.0, 0.0}};
    for (int d = 0; d < kDim; ++d) plo[d] = geom_.ProbLo(d);

    auto lv = std::unique_ptr<Level>(new Level());
    lv->define(0, geom_.ProbHi(0) - geom_.ProbLo(0), 1, plo, {}, &bodies, true);

    for (int i = 0; i < static_cast<int>(bodies.size()); ++i) {
        lv->realBins[0].push_back(i);
        const Real r = bodies[i].radius;
        if (r > lv->cellMaxR[0]) lv->cellMaxR[0] = r;
    }
    lv->refineFlag[0] = 0;
    lv->fillFlags();

    // All N(N-1)/2 pairs, the reference broadphase.
    for (int i = 0; i < static_cast<int>(bodies.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(bodies.size()); ++j) lv->addPair(i, j);
    }
    levels_.push_back(std::move(lv));
}

void GridHierarchy::buildRegular(const std::vector<Particle>& bodies) {
    // Regular grid: every cell of every level 0..L_max exists, L_max being the
    // level of the smallest body in the current population.
    const Real h0 = geom_.ProbHi(0) - geom_.ProbLo(0);
    std::array<Real, 3> plo{{0.0, 0.0, 0.0}};
    for (int d = 0; d < kDim; ++d) plo[d] = geom_.ProbLo(d);
    const int rf = params_.refine_factor;

    int lmax = 0;
    for (const Particle& p : bodies) {
        lmax = std::max(lmax, naturalLevel(h0, params_, 2.0 * p.radius));
    }

    // Cell budget guard: a uniformly refined 3D grid explodes quickly.
    const long nfine = static_cast<long>(powInt(static_cast<Real>(rf), lmax));
    if (nfine * nfine * nfine > 8000000L) {
        throw std::runtime_error("GridHierarchy: regular grid at level " +
                                 std::to_string(lmax) + " would need " +
                                 std::to_string(nfine * nfine * nfine) + " cells");
    }

    for (int L = 0; L <= lmax; ++L) {
        const Real h = h0 / powInt(static_cast<Real>(rf), L);
        const int n = static_cast<int>(powInt(static_cast<Real>(rf), L));

        // Every cell of the level exists: the level is the complete uniform grid,
        // which define() turns into one block-partitioned BoxArray (no per-cell
        // boxes, no per-cell FABs) with a per-cell occupancy MultiFab.
        auto lv = std::unique_ptr<Level>(new Level());
        lv->define(L, h, n, plo, {}, &bodies, true);
        levels_.push_back(std::move(lv));
    }

    // Drop every body to its natural level (6.4.1).
    for (int b = 0; b < static_cast<int>(bodies.size()); ++b) {
        const int L = naturalLevel(h0, params_, 2.0 * bodies[b].radius);
        levels_[L]->assignReal(b, cellOfPoint(plo, levels_[L]->h, levels_[L]->n, bodies[b].pos));
    }

    // Virtual inheritance of coarse bodies into the occupied fine cells (6.4.2).
    for (int L = 1; L < static_cast<int>(levels_.size()); ++L) {
        for (int La = 0; La < L; ++La) {
            const int ratio = static_cast<int>(powInt(static_cast<Real>(rf), L - La));
            levels_[L]->inheritFrom(*levels_[La], ratio, params_.inherit_coarse);
        }
    }

    for (auto& lv : levels_) {
        // Occupancy / virtual-count / refinement flags into the iMultiFab; the
        // pair pass reads them back through MFIter.
        lv->fillFlags();
        lv->setStencilRadius();
        lv->buildPairs();
    }
}

// Shared body of buildAdaptive: create the level objects for a given
// body -> level assignment, then inherit and compare.  (No extra members are
// allowed on GridHierarchy: the header is fixed, so the helpers live here as
// file-local types/functions.)

void GridHierarchy::buildAdaptive(const std::vector<Particle>& bodies, Real /*dt*/) {
    const Real h0 = geom_.ProbHi(0) - geom_.ProbLo(0);
    std::array<Real, 3> plo{{0.0, 0.0, 0.0}};
    for (int d = 0; d < kDim; ++d) plo[d] = geom_.ProbLo(d);
    const int rf = params_.refine_factor;

    if (bodies.empty()) {
        // No population: the mono-cell of level 0 still exists (6.3).
        auto lv = std::unique_ptr<Level>(new Level());
        lv->define(0, h0, 1, plo, {}, &bodies, true);
        lv->fillFlags();
        levels_.push_back(std::move(lv));
        return;
    }

    std::vector<int> nat(bodies.size(), 0);
    int lmax = 0;
    for (int b = 0; b < static_cast<int>(bodies.size()); ++b) {
        nat[b] = naturalLevel(h0, params_, 2.0 * bodies[b].radius);
        lmax = std::max(lmax, nat[b]);
    }

    // Level by level, only the cells that hold (or must contain) a body exist:
    // cell set of level L = { cell_L(c_b) : nat[b] >= L }.  This construction is
    // nested by definition (the parent of an existing level-L cell holds the same
    // body and therefore exists at L-1); the containment is checked below with
    // amrex::BoxArray::refine/contains, which is what makes it a valid AMR
    // hierarchy.
    for (int L = 0; L <= lmax; ++L) {
        const Real h = h0 / powInt(static_cast<Real>(rf), L);
        const int n = static_cast<int>(powInt(static_cast<Real>(rf), L));

        std::vector<amrex::IntVect> ivs;
        ivs.reserve(bodies.size());
        for (int b = 0; b < static_cast<int>(bodies.size()); ++b) {
            if (nat[b] < L) continue;
            ivs.push_back(cellOfPoint(plo, h, n, bodies[b].pos));
        }
        std::sort(ivs.begin(), ivs.end(), ivLess);
        ivs.erase(std::unique(ivs.begin(), ivs.end()), ivs.end());
        if (ivs.empty()) break;  // "if a level has no active cells it is not created"

        std::vector<amrex::Box> cellBoxes;
        cellBoxes.reserve(ivs.size());
        for (const amrex::IntVect& iv : ivs) cellBoxes.push_back(amrex::Box(iv, iv));

        auto lv = std::unique_ptr<Level>(new Level());
        lv->define(L, h, n, plo, cellBoxes, &bodies, false);
        levels_.push_back(std::move(lv));

        if (L > 0) {
            // AMR validity: the fine boxes must be covered by the refined coarse
            // boxes (they are, by construction; this asserts it).
            amrex::BoxArray coarseRefined = amrex::BoxArray(levels_[L - 1]->boxes);
            coarseRefined.refine(rf);
            if (!coarseRefined.contains(levels_[L]->boxes)) {
                throw std::runtime_error("GridHierarchy: level " + std::to_string(L) +
                                         " is not contained in the refined level " +
                                         std::to_string(L - 1));
            }
        }

        for (int b = 0; b < static_cast<int>(bodies.size()); ++b) {
            if (nat[b] != L) continue;
            levels_[L]->assignReal(b, cellOfPoint(plo, h, n, bodies[b].pos));
        }
    }

    for (int L = 1; L < static_cast<int>(levels_.size()); ++L) {
        for (int La = 0; La < L; ++La) {
            const int ratio = static_cast<int>(powInt(static_cast<Real>(rf), L - La));
            levels_[L]->inheritFrom(*levels_[La], ratio, params_.inherit_coarse);
        }
    }

    for (auto& lv : levels_) {
        lv->fillFlags();
        lv->setStencilRadius();
        lv->buildPairs();
    }
}

// v_BA on the distance line measured from B to A (see the sign discussion at the
// top of this file): negative <=> the centres are closing in.  A pair at rest
// (|v_BA| <= kRestEps, i.e. v_BA == 0 for a static pile) counts as NOT clearly
// separating, so a resting pile is still refined.  Documented deviation from the
// literal v_BA < 0 test: with bodies at rest every pair evaluates to exactly 0,
// the strict test would never refine anything and the reluctant mode would
// collapse to the mono-cell (NoGrid) for every static scenario, losing the whole
// point of the morphology.  Only pairs that are demonstrably moving apart
// (v_BA >= kRestEps) suppress refinement.
constexpr Real kRestEps = 1.0e-12;

[[nodiscard]] static bool nonSeparatingPair(const Particle& a, const Particle& b) noexcept {
    const Vec3 dAB = a.pos - b.pos;
    const Vec3 dv = a.vel - b.vel;
    return dot(dv, dAB) < kRestEps;
}

[[nodiscard]] static Real pairGap(const Particle& a, const Particle& b) noexcept {
    return norm(b.pos - a.pos) - (a.radius + b.radius);
}

void GridHierarchy::buildReluctant(const std::vector<Particle>& bodies, Real /*dt*/) {
    const Real h0 = geom_.ProbHi(0) - geom_.ProbLo(0);
    std::array<Real, 3> plo{{0.0, 0.0, 0.0}};
    for (int d = 0; d < kDim; ++d) plo[d] = geom_.ProbLo(d);
    const int rf = params_.refine_factor;
    const int nb = static_cast<int>(bodies.size());

    std::vector<int> nat(nb, 0);
    for (int b = 0; b < nb; ++b) nat[b] = naturalLevel(h0, params_, 2.0 * bodies[b].radius);

    // Start with everything on the mono-cell and refine top down, cell by cell,
    // only where the 6.3 criterion holds.
    std::vector<int> cur(nb, 0);         // current level of each body

    auto lv0 = std::unique_ptr<Level>(new Level());
    lv0->define(0, h0, 1, plo, {}, &bodies, true);
    for (int b = 0; b < nb; ++b) lv0->assignReal(b, amrex::IntVect(0));
    lv0->fillFlags();
    levels_.push_back(std::move(lv0));

    std::vector<std::vector<int>> cellBodies;  // per cell of the current level

    for (int L = 0; L < params_.max_level; ++L) {
        Level& lv = *levels_[L];
        const int nc = static_cast<int>(lv.cells.size());

        // bodies per cell on this level (all of them are real: they live here)
        cellBodies.assign(nc, std::vector<int>{});
        for (int b = 0; b < nb; ++b) {
            if (cur[b] != L) continue;
            const int s = lv.slot(cellOfPoint(plo, lv.h, lv.n, bodies[b].pos));
            if (s >= 0) cellBodies[s].push_back(b);
        }

        std::vector<char> mark(nc, 0);
        for (int c = 0; c < nc; ++c) {
            const std::vector<int>& here = cellBodies[c];
            if (here.empty()) continue;
            // Can anything in this cell legally go deeper?  If not the cell is not
            // "refused", it is simply as fine as its occupants allow.
            bool canRefine = false;
            for (const int b : here) {
                if (nat[b] > L) { canRefine = true; break; }
            }
            if (!canRefine) continue;

            bool refine = static_cast<int>(here.size()) >= params_.min_bodies_to_refine;
            if (refine) {
                // Approach test over the cell + its 26 neighbours (6.3: "they
                // exist in neighbouring cells").  Deterministic ordering: cells
                // in lexicographic order, bodies in bin order.
                refine = false;
                for (int dc = -1; dc <= 1 && !refine; ++dc) {
                    for (int dj = -1; dj <= 1 && !refine; ++dj) {
                        for (int di = -1; di <= 1 && !refine; ++di) {
                            const amrex::IntVect nv = lv.cells[c] + amrex::IntVect(di, dj, dc);
                            const int s = lv.slot(nv);
                            if (s < 0) continue;
                            const std::vector<int>& other = cellBodies[s];
                            for (std::size_t ia = 0; ia < here.size() && !refine; ++ia) {
                                for (std::size_t ib = 0; ib < other.size() && !refine; ++ib) {
                                    const int a = here[ia];
                                    const int b = other[ib];
                                    if (a == b) continue;
                                    if (s == c && b < a) continue;  // each pair once
                                    if (!nonSeparatingPair(bodies[a], bodies[b])) continue;
                                    if (pairGap(bodies[a], bodies[b]) >=
                                        params_.approach_gap * lv.h) {
                                        continue;
                                    }
                                    refine = true;
                                }
                            }
                        }
                    }
                }
            }
            mark[c] = refine ? 1 : 0;
            if (!refine) ++stats_.cells_refused;
        }
        lv.flagRefinedCells(mark);
        lv.fillFlags();

        // Drop the bodies of refined cells one level down, as far as their own
        // size permits (6.4.1).  Bodies that are refused stay where they are.
        std::vector<int> next(nb, -1);
        const Real hf = h0 / powInt(static_cast<Real>(rf), L + 1);
        const int nf = static_cast<int>(powInt(static_cast<Real>(rf), L + 1));
        std::vector<amrex::IntVect> ivs;
        for (int c = 0; c < nc; ++c) {
            if (!mark[c]) continue;
            for (const int b : cellBodies[c]) {
                if (nat[b] <= L) continue;
                next[b] = L + 1;
                ivs.push_back(cellOfPoint(plo, hf, nf, bodies[b].pos));
            }
        }
        if (ivs.empty()) break;  // nothing dropped: no finer level is created

        std::sort(ivs.begin(), ivs.end(), ivLess);
        ivs.erase(std::unique(ivs.begin(), ivs.end()), ivs.end());
        std::vector<amrex::Box> cellBoxes;
        cellBoxes.reserve(ivs.size());
        for (const amrex::IntVect& iv : ivs) cellBoxes.push_back(amrex::Box(iv, iv));

        auto fine = std::unique_ptr<Level>(new Level());
        fine->define(L + 1, hf, nf, plo, cellBoxes, &bodies, false);
        for (int b = 0; b < nb; ++b) {
            if (next[b] != L + 1) continue;
            fine->assignReal(b, cellOfPoint(plo, hf, nf, bodies[b].pos));
        }
        fine->fillFlags();
        levels_.push_back(std::move(fine));

        // AMR validity of the new level.
        amrex::BoxArray coarseRefined = amrex::BoxArray(levels_[L]->boxes);
        coarseRefined.refine(rf);
        if (!coarseRefined.contains(levels_[L + 1]->boxes)) {
            throw std::runtime_error("GridHierarchy: reluctant level " + std::to_string(L + 1) +
                                     " is not contained in the refined level " +
                                     std::to_string(L));
        }

        for (int b = 0; b < nb; ++b) {
            if (next[b] == L + 1) cur[b] = L + 1;
        }

        // Bodies that were dropped are no longer real links of level L (6.4.1:
        // the association is moved, not copied).  Rebuild the coarse bins so the
        // coarse level only compares what actually stayed there.
        lv.realBins.assign(nc, std::vector<int>{});
        lv.cellMaxR.assign(nc, 0.0);
        for (int b = 0; b < nb; ++b) {
            if (cur[b] != L) continue;
            lv.assignReal(b, cellOfPoint(plo, lv.h, lv.n, bodies[b].pos));
        }
    }

    // Coarse bodies that were refused must still be visible to the fine levels
    // (6.4.2), otherwise the pairs with fine bodies would be missed.
    for (int L = 1; L < static_cast<int>(levels_.size()); ++L) {
        for (int La = 0; La < L; ++La) {
            const int ratio = static_cast<int>(powInt(static_cast<Real>(rf), L - La));
            levels_[L]->inheritFrom(*levels_[La], ratio, params_.inherit_coarse);
        }
    }

    for (auto& lvp : levels_) {
        lvp->fillFlags();
        lvp->setStencilRadius();
        lvp->buildPairs();
    }
}

void GridHierarchy::candidatePairs(std::vector<ContactPair>& out) const {
    out.clear();
    if (levels_.empty()) return;
    const std::vector<ContactPair>& m = levels_[0]->mergedPairs;
    out.assign(m.begin(), m.end());
}

int GridHierarchy::levelOf(int body_id) const noexcept {
    // Bodies are binned exactly once (as a real link) at the level they reside
    // on; virtual links are not considered here.
    for (int L = static_cast<int>(levels_.size()) - 1; L >= 0; --L) {
        const Level& lv = *levels_[L];
        for (const std::vector<int>& bin : lv.realBins) {
            if (std::find(bin.begin(), bin.end(), body_id) != bin.end()) return L;
        }
    }
    return -1;
}

Real GridHierarchy::cellSize(int level) const noexcept {
    const Real h0 = geom_.ProbHi(0) - geom_.ProbLo(0);
    const int rf = (params_.refine_factor > 0) ? params_.refine_factor : 1;
    Real h = h0;
    if (level > 0) {
        for (int i = 0; i < level; ++i) h /= static_cast<Real>(rf);
    } else if (level < 0) {
        for (int i = 0; i < -level; ++i) h *= static_cast<Real>(rf);
    }
    return h;
}

Real GridHierarchy::minCellSize() const noexcept {
    if (levels_.empty()) return cellSize(0);
    return cellSize(static_cast<int>(levels_.size()) - 1);
}

Real GridHierarchy::maxCellSize() const noexcept { return cellSize(0); }

void GridHierarchy::printProfile(std::ostream& os) const {
    os << "GridHierarchy mode=" << toString(mode_)
       << " max_level=" << params_.max_level
       << " refine_factor=" << params_.refine_factor
       << " size_slack=" << params_.size_slack
       << " approach_gap=" << params_.approach_gap
       << " min_bodies_to_refine=" << params_.min_bodies_to_refine
       << " inherit_coarse=" << (params_.inherit_coarse ? "true" : "false") << '\n';
    for (const auto& lv : levels_) {
        long occ = 0;
        for (const std::vector<int>& b : lv->realBins) {
            if (!b.empty()) ++occ;
        }
        os << "  level " << lv->index << "  h=" << lv->h
           << "  cells=" << lv->cells.size()
           << "  occupied=" << occ
           << "  virtual_links=" << lv->virtualLinks
           << "  raw_pairs=" << lv->rawPairs
           << "  cell_pair_checks=" << lv->cellPairChecks
           << "  stencil_R=" << lv->stencilRadius << '\n';
    }
    os << "  stats: levels=" << stats_.levels
       << " cells_total=" << stats_.cells_total
       << " cells_occupied=" << stats_.cells_occupied
       << " cells_refused=" << stats_.cells_refused
       << " particles_real=" << stats_.particles_real
       << " virtual_links=" << stats_.virtual_links
       << " cell_pair_checks=" << stats_.cell_pair_checks
       << " raw_pairs=" << stats_.raw_pairs
       << " unique_pairs=" << stats_.unique_pairs
       << " refinements=" << stats_.refinements
       << " coarsenings=" << stats_.coarsenings
       << " h_min=" << minCellSize()
       << " h_max=" << maxCellSize() << '\n';
}

// ---------------------------------------------------------------------------
// Visualisation snapshot: every cell that currently exists, with its level and
// how many real / inherited bodies it holds. The regular grid therefore shows
// its full uniform mesh, the adaptive one only the occupied neighbourhood, and
// the reluctant one the cells it deliberately refused to refine.
// ---------------------------------------------------------------------------
void GridHierarchy::cellsForVisualisation(std::vector<CellViz>& out,
                                          std::size_t max_cells) const {
    out.clear();
    for (const auto& lp : levels_) {
        const Level& L = *lp;
        for (int c = 0; c < static_cast<int>(L.cells.size()); ++c) {
            if (out.size() >= max_cells) return;
            Real lo[3];
            Real hi[3];
            L.cellBounds(c, lo, hi);
            CellViz v;
            v.lo = makeVec3(lo[0], lo[1], lo[2]);
            v.hi = makeVec3(hi[0], hi[1], hi[2]);
            v.level = L.index;
            v.nreal = static_cast<int>(L.realBins[c].size());
            v.nvirt = static_cast<int>(L.virtBins[c].size());
            out.push_back(v);
        }
    }
}

}  // namespace contact
