// Standalone tests for contact::GridHierarchy (broadphase grid morphologies,
// thesis 6.1-6.4).  Links AMReX (3D, serial, CPU).
//
//   1. geometry smoke: Geometry over [0,1]^3, levelOf/cellSize/min/max
//   2. NoGrid: 60 bodies => exactly N(N-1)/2 pairs
//   3. Regular / Adaptive / Reluctant on 200 polydisperse bodies: sorted+unique,
//      no false negatives vs a brute-force bounding-sphere oracle, counts
//   4. Reluctant vs Adaptive on a strictly separating population
//   5. determinism: rebuild twice => identical stats and candidate vector

#include "contact/Grid.hpp"

#include <AMReX.H>
#include <AMReX_Geometry.H>
#include <AMReX_RealBox.H>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using contact::ContactPair;
using contact::GridHierarchy;
using contact::GridMode;
using contact::GridParams;
using contact::Particle;
using contact::Real;
using contact::Vec3;

namespace {

// Vec3 is a std::array, so ADL at global scope does not find contact's
// arithmetic operators; pull them in explicitly.
using contact::cross;
using contact::dot;
using contact::norm;
using contact::norm2;
using contact::normalized;
using contact::operator*;
using contact::operator+;
using contact::operator-;

int g_fail = 0;
int g_checks = 0;

void check(bool cond, const char* what, int line) {
    ++g_checks;
    if (!cond) {
        std::printf("  FAIL  line %d: %s\n", line, what);
        ++g_fail;
    }
}
#define CHECK(cond) check((cond), #cond, __LINE__)
#define CHECK_NEAR(a, b, tol)                                                      \
    do {                                                                           \
        const double aa = (a);                                                      \
        const double bb = (b);                                                      \
        check(std::fabs(aa - bb) <= (tol), #a " ~ " #b, __LINE__);                  \
    } while (0)

Particle makeBody(int id, const Vec3& pos, Real radius, const Vec3& vel) {
    Particle p;
    p.id = id;
    p.pos = pos;
    p.vel = vel;
    p.radius = radius;
    p.halo = 1.1 * radius;
    p.mass = 1.0;
    p.inv_mass = 1.0;
    return p;
}

bool sortedUnique(const std::vector<ContactPair>& v) {
    for (std::size_t k = 0; k < v.size(); ++k) {
        if (v[k].i >= v[k].j) return false;
        if (k > 0 && !(v[k - 1].i < v[k].i || (v[k - 1].i == v[k].i && v[k - 1].j < v[k].j))) {
            return false;
        }
    }
    return true;
}

bool samePairs(const std::vector<ContactPair>& a, const std::vector<ContactPair>& b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(),
                                              [](const ContactPair& x, const ContactPair& y) {
                                                  return x.i == y.i && x.j == y.j;
                                              });
}

bool containsPair(const std::vector<ContactPair>& v, int i, int j) {
    ContactPair k;
    k.i = std::min(i, j);
    k.j = std::max(i, j);
    return std::binary_search(v.begin(), v.end(), k, [](const ContactPair& a, const ContactPair& b) {
        return (a.i < b.i) || (a.i == b.i && a.j < b.j);
    });
}

// Brute-force bounding-sphere oracle: every pair whose real spheres overlap.
std::vector<ContactPair> overlapOracle(const std::vector<Particle>& b) {
    std::vector<ContactPair> out;
    for (std::size_t i = 0; i < b.size(); ++i) {
        for (std::size_t j = i + 1; j < b.size(); ++j) {
            const Real d = contact::norm(b[j].pos - b[i].pos);
            if (d <= b[i].radius + b[j].radius) out.push_back(ContactPair{int(i), int(j)});
        }
    }
    return out;
}

// Returns the number of oracle pairs missing from `cand`.
long checkNoFalseNegatives(const std::vector<ContactPair>& cand,
                           const std::vector<Particle>& bodies, const char* tag) {
    const std::vector<ContactPair> oracle = overlapOracle(bodies);
    long missing = 0;
    for (const ContactPair& p : oracle) {
        if (!containsPair(cand, p.i, p.j)) {
            if (missing < 5) {
                std::printf("  MISSING %s pair (%d,%d)  d=%.6f  ra+rb=%.6f\n", tag, p.i, p.j,
                            contact::norm(bodies[p.j].pos - bodies[p.i].pos),
                            bodies[p.i].radius + bodies[p.j].radius);
            }
            ++missing;
        }
    }
    std::printf("  %-9s candidates=%zu  oracle(overlapping spheres)=%zu  missing=%ld\n", tag,
                cand.size(), oracle.size(), missing);
    return missing;
}

void report(GridHierarchy& g, const std::vector<ContactPair>& cand, const char* tag) {
    const contact::GridStats& s = g.stats();
    std::printf(
        "  %-9s levels=%d cells_total=%ld cells_occupied=%ld raw=%ld unique=%ld "
        "virtual_links=%ld cell_pair_checks=%ld refused=%ld h_min=%.6f h_max=%.6f\n",
        tag, s.levels, s.cells_total, s.cells_occupied, s.raw_pairs, s.unique_pairs,
        s.virtual_links, s.cell_pair_checks, s.cells_refused, g.minCellSize(),
        g.maxCellSize());
    (void)cand;
}

// Polydisperse population: uniform positions, random diameters in [dmin,dmax].
std::vector<Particle> polydisperse(int n, unsigned seed, Real dmin, Real dmax) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<Real> pos(0.05, 0.95);
    std::uniform_real_distribution<Real> dia(dmin, dmax);
    std::uniform_real_distribution<Real> velr(-0.5, 0.5);
    std::vector<Particle> bodies;
    bodies.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        const Real r = 0.5 * dia(gen);
        bodies.push_back(makeBody(i, Vec3{pos(gen), pos(gen), pos(gen)}, r,
                                  Vec3{velr(gen), velr(gen), velr(gen)}));
    }
    return bodies;
}

}  // namespace

int main(int argc, char** argv) {
    amrex::Initialize(argc, argv, false);

    // ---------------------------------------------------------------- test 1
    std::printf("[1] geometry smoke (Box 0..31 over [0,1]^3, Adaptive, max_level=4)\n");
    {
        amrex::Box dom(amrex::IntVect(0), amrex::IntVect(31));
        amrex::Real lo[3] = {0.0, 0.0, 0.0};
        amrex::Real hi[3] = {1.0, 1.0, 1.0};
        amrex::RealBox rb(lo, hi);
        amrex::Geometry geom(dom, &rb, 0, nullptr);

        GridParams gp;
        gp.max_level = 4;
        gp.refine_factor = 3;
        GridHierarchy g(geom, gp, GridMode::Adaptive);

        // one body per natural level: 0, 1, 2, 4
        std::vector<Particle> bodies;
        bodies.push_back(makeBody(0, Vec3{0.10, 0.10, 0.10}, 0.500, Vec3{0, 0, 0}));  // d=1.00
        bodies.push_back(makeBody(1, Vec3{0.60, 0.10, 0.10}, 0.100, Vec3{0, 0, 0}));  // d=0.20
        bodies.push_back(makeBody(2, Vec3{0.10, 0.60, 0.10}, 0.020, Vec3{0, 0, 0}));  // d=0.04
        bodies.push_back(makeBody(3, Vec3{0.60, 0.60, 0.10}, 0.005, Vec3{0, 0, 0}));  // d=0.01
        g.build(bodies, 0.0);

        CHECK_NEAR(g.cellSize(0), 1.0, 1e-15);
        CHECK_NEAR(g.cellSize(1), 1.0 / 3.0, 1e-15);
        CHECK_NEAR(g.cellSize(2), 1.0 / 9.0, 1e-15);
        CHECK_NEAR(g.cellSize(3), 1.0 / 27.0, 1e-15);
        CHECK_NEAR(g.cellSize(4), 1.0 / 81.0, 1e-15);
        CHECK_NEAR(g.maxCellSize(), 1.0, 1e-15);
        CHECK_NEAR(g.minCellSize(), 1.0 / 81.0, 1e-15);
        CHECK(g.levelOf(0) == 0);
        CHECK(g.levelOf(1) == 1);
        CHECK(g.levelOf(2) == 2);
        CHECK(g.levelOf(3) == 4);
        CHECK(g.stats().levels == 5);
        CHECK(g.stats().cells_total == 8);  // 1 + 3 + 2 + 1 + 1
        CHECK(g.stats().cells_occupied == 4);
        CHECK(std::string(contact::toString(GridMode::NoGrid)) == "NoGrid");
        CHECK(std::string(contact::toString(GridMode::Regular)) == "Regular");
        CHECK(std::string(contact::toString(GridMode::Adaptive)) == "Adaptive");
        CHECK(std::string(contact::toString(GridMode::Reluctant)) == "Reluctant");
        GridMode m = GridMode::NoGrid;
        CHECK(contact::gridModeFromString("Reluctant", m) && m == GridMode::Reluctant);
        CHECK(contact::gridModeFromString("adaptive", m) && m == GridMode::Adaptive);
        CHECK(!contact::gridModeFromString("bogus", m));
        g.printProfile(std::cout);
    }

    // ---------------------------------------------------------------- test 2
    std::printf("\n[2] NoGrid with 60 bodies\n");
    {
        amrex::Box dom(amrex::IntVect(0), amrex::IntVect(31));
        amrex::Real lo[3] = {0.0, 0.0, 0.0};
        amrex::Real hi[3] = {1.0, 1.0, 1.0};
        amrex::RealBox rb(lo, hi);
        amrex::Geometry geom(dom, &rb, 0, nullptr);

        GridHierarchy g(geom, GridParams{}, GridMode::NoGrid);
        std::vector<Particle> bodies = polydisperse(60, 7u, 0.02, 0.12);
        g.build(bodies, 1e-3);
        std::vector<ContactPair> cand;
        g.candidatePairs(cand);

        CHECK(g.mode() == GridMode::NoGrid);
        CHECK(std::string(contact::toString(g.mode())) == "NoGrid");
        CHECK(g.stats().unique_pairs == 60 * 59 / 2);
        CHECK(g.stats().raw_pairs == 60 * 59 / 2);
        CHECK(cand.size() == static_cast<std::size_t>(60 * 59 / 2));
        CHECK(sortedUnique(cand));
        CHECK(g.stats().levels == 1);
        CHECK(g.stats().cells_total == 1);
        CHECK(g.stats().virtual_links == 0);
        report(g, cand, "NoGrid");
    }

    // ---------------------------------------------------------------- test 3
    std::printf("\n[3] 200 polydisperse bodies (d in [0.02,0.12]), fixed seed\n");
    std::vector<Particle> bodies200 = polydisperse(200, 12345u, 0.02, 0.12);
    {
        amrex::Box dom(amrex::IntVect(0), amrex::IntVect(31));
        amrex::Real lo[3] = {0.0, 0.0, 0.0};
        amrex::Real hi[3] = {1.0, 1.0, 1.0};
        amrex::RealBox rb(lo, hi);
        amrex::Geometry geom(dom, &rb, 0, nullptr);

        GridParams gp;
        gp.max_level = 4;
        gp.refine_factor = 3;
        gp.size_slack = 1.0;

        const GridMode modes[4] = {GridMode::NoGrid, GridMode::Regular, GridMode::Adaptive,
                                   GridMode::Reluctant};
        long uniq[4] = {0, 0, 0, 0};
        for (int k = 0; k < 4; ++k) {
            GridHierarchy g(geom, gp, modes[k]);
            g.build(bodies200, 1e-3);
            std::vector<ContactPair> cand;
            g.candidatePairs(cand);
            CHECK(sortedUnique(cand));
            CHECK(static_cast<long>(cand.size()) == g.stats().unique_pairs);
            const long missing = checkNoFalseNegatives(cand, bodies200, contact::toString(modes[k]));
            CHECK(missing == 0);
            report(g, cand, contact::toString(modes[k]));
            uniq[k] = g.stats().unique_pairs;
        }
        CHECK(uniq[2] <= uniq[0]);
        CHECK(uniq[3] <= uniq[0]);
        std::printf("  measured unique: NoGrid=%ld Regular=%ld Adaptive=%ld Reluctant=%ld\n",
                    uniq[0], uniq[1], uniq[2], uniq[3]);
    }

    // -------------------------------------------------------------- test 3b
    // Dense cluster + size_slack = 0.5: bodies are wider than half a cell, so
    // the cell stencil has to widen (R = 2) and the virtual inheritance margin
    // is exercised hard.  Same no-false-negative oracle.
    std::printf("\n[3b] dense cluster, 120 bodies, size_slack=0.5 (wide stencil)\n");
    {
        amrex::Box dom(amrex::IntVect(0), amrex::IntVect(31));
        amrex::Real lo[3] = {0.0, 0.0, 0.0};
        amrex::Real hi[3] = {1.0, 1.0, 1.0};
        amrex::RealBox rb(lo, hi);
        amrex::Geometry geom(dom, &rb, 0, nullptr);

        GridParams gp;
        gp.max_level = 4;
        gp.size_slack = 0.5;

        std::mt19937 gen(999u);
        std::uniform_real_distribution<Real> pos(0.30, 0.70);
        std::uniform_real_distribution<Real> dia(0.05, 0.20);
        std::uniform_real_distribution<Real> velr(-1.0, 1.0);
        std::vector<Particle> dense;
        for (int i = 0; i < 120; ++i) {
            const Real r = 0.5 * dia(gen);
            dense.push_back(makeBody(i, Vec3{pos(gen), pos(gen), pos(gen)}, r,
                                     Vec3{velr(gen), velr(gen), velr(gen)}));
        }

        const GridMode modes[4] = {GridMode::NoGrid, GridMode::Regular, GridMode::Adaptive,
                                   GridMode::Reluctant};
        long uniq[4] = {0, 0, 0, 0};
        for (int k = 0; k < 4; ++k) {
            GridHierarchy g(geom, gp, modes[k]);
            g.build(dense, 1e-3);
            std::vector<ContactPair> cand;
            g.candidatePairs(cand);
            CHECK(sortedUnique(cand));
            CHECK(checkNoFalseNegatives(cand, dense, contact::toString(modes[k])) == 0);
            report(g, cand, contact::toString(modes[k]));
            uniq[k] = g.stats().unique_pairs;
            if (modes[k] == GridMode::Adaptive) {
                std::ostringstream os;
                g.printProfile(os);
                CHECK(os.str().find("stencil_R=2") != std::string::npos);
            }
        }
        CHECK(uniq[2] <= uniq[0]);
        CHECK(uniq[3] <= uniq[0]);
        std::printf("  measured unique: NoGrid=%ld Regular=%ld Adaptive=%ld Reluctant=%ld\n",
                    uniq[0], uniq[1], uniq[2], uniq[3]);
    }

    // ---------------------------------------------------------------- test 4
    std::printf("\n[4] Reluctant vs Adaptive, strictly separating population (6 bodies)\n");
    {
        amrex::Box dom(amrex::IntVect(0), amrex::IntVect(31));
        amrex::Real lo[3] = {0.0, 0.0, 0.0};
        amrex::Real hi[3] = {1.0, 1.0, 1.0};
        amrex::RealBox rb(lo, hi);
        amrex::Geometry geom(dom, &rb, 0, nullptr);

        GridParams gp;
        gp.max_level = 4;
        gp.refine_factor = 3;

        // All six centres inside one level-2 cell (h=1/9) around x=0.5, radii 0.03
        // (d=0.06 -> natural level 2), velocities strictly radially outward, so
        // every pair separates: v_BA >= 0 for all of them.
        const Vec3 c{0.5, 0.5, 0.5};
        const Real off[6][3] = {{0.000, 0.000, 0.000}, {0.020, 0.005, -0.010},
                                {-0.018, 0.012, 0.008}, {0.006, -0.020, 0.014},
                                {-0.008, -0.006, -0.020}, {0.014, 0.016, 0.018}};
        std::vector<Particle> bodies;
        for (int i = 0; i < 6; ++i) {
            const Vec3 d{off[i][0], off[i][1], off[i][2]};
            const Vec3 p = c + d;
            bodies.push_back(makeBody(i, p, 0.03, contact::normalized(d) * 1.5));
        }

        GridHierarchy ga(geom, gp, GridMode::Adaptive);
        ga.build(bodies, 1e-3);
        std::vector<ContactPair> ca;
        ga.candidatePairs(ca);
        CHECK(sortedUnique(ca));
        CHECK(checkNoFalseNegatives(ca, bodies, "Adaptive") == 0);
        report(ga, ca, "Adaptive");

        GridHierarchy gr(geom, gp, GridMode::Reluctant);
        gr.build(bodies, 1e-3);
        std::vector<ContactPair> cr;
        gr.candidatePairs(cr);
        CHECK(sortedUnique(cr));
        CHECK(gr.stats().cells_refused > 0);
        CHECK(cr.size() <= ca.size());
        CHECK(checkNoFalseNegatives(cr, bodies, "Reluctant") == 0);
        report(gr, cr, "Reluctant");
        std::printf("  separating: adaptive=%zu reluctant=%zu cells_refused=%ld\n", ca.size(),
                    cr.size(), gr.stats().cells_refused);

        // Sanity in the other direction: the same cluster but approaching must
        // refine (refused cells then only where the criterion cannot be met).
        std::vector<Particle> inward = bodies;
        for (int i = 0; i < 6; ++i) {
            const Vec3 d{off[i][0], off[i][1], off[i][2]};
            inward[i].vel = contact::normalized(d) * -1.5;
        }
        GridHierarchy gr2(geom, gp, GridMode::Reluctant);
        gr2.build(inward, 1e-3);
        std::vector<ContactPair> cr2;
        gr2.candidatePairs(cr2);
        report(gr2, cr2, "Reluctant*");
        CHECK(gr2.stats().levels > gr.stats().levels);  // approaching => refines deeper

        // Refinement/coarsening deltas vs the previous build: shrink the bodies,
        // which drop them to deeper levels and thus grow the grid.
        std::vector<Particle> smaller = inward;
        for (Particle& p : smaller) {
            p.radius *= 0.34;
            p.halo = 1.1 * p.radius;
        }
        const long lv_before = gr2.stats().levels;
        gr2.build(smaller, 1e-3);
        std::printf("  rebuild with 0.34x radii: levels %ld -> %d, refinements=%ld "
                    "coarsenings=%ld\n",
                    lv_before, gr2.stats().levels, gr2.stats().refinements,
                    gr2.stats().coarsenings);
        CHECK(gr2.stats().refinements + gr2.stats().coarsenings > 0);
    }

    // --------------------------------------------------------------- test 4b
    // Resting pile: all velocities exactly zero.  v_BA == 0 must not be read as
    // separation, otherwise the reluctant grid would never refine and a static
    // pile would fall back to NoGrid.
    std::printf("\n[4b] reluctant on a resting pile (60 bodies, v == 0)\n");
    {
        amrex::Box dom(amrex::IntVect(0), amrex::IntVect(31));
        amrex::Real lo[3] = {0.0, 0.0, 0.0};
        amrex::Real hi[3] = {1.0, 1.0, 1.0};
        amrex::RealBox rb(lo, hi);
        amrex::Geometry geom(dom, &rb, 0, nullptr);

        GridParams gp;
        gp.max_level = 4;
        std::vector<Particle> pile = polydisperse(60, 7u, 0.02, 0.12);
        for (Particle& p : pile) p.vel = Vec3{0.0, 0.0, 0.0};

        GridHierarchy ga(geom, gp, GridMode::Adaptive);
        ga.build(pile, 1e-3);
        std::vector<ContactPair> ca;
        ga.candidatePairs(ca);

        GridHierarchy gr(geom, gp, GridMode::Reluctant);
        gr.build(pile, 1e-3);
        std::vector<ContactPair> cr;
        gr.candidatePairs(cr);
        CHECK(sortedUnique(cr));
        CHECK(checkNoFalseNegatives(cr, pile, "Reluctant@rest") == 0);
        CHECK(gr.stats().levels > 1);  // a resting pile must still be refined
        CHECK(gr.minCellSize() < gr.maxCellSize());  // it really went below level 0
        CHECK(cr.size() < static_cast<std::size_t>(1770 / 2));  // far from NoGrid
        (void)ca;
        report(ga, ca, "Adaptive");
        report(gr, cr, "Reluctant");
        std::printf("  resting: adaptive=%zu reluctant=%zu levels=%d h_min=%.6f\n", ca.size(),
                    cr.size(), gr.stats().levels, gr.minCellSize());
    }

    // ---------------------------------------------------------------- test 5
    std::printf("\n[5] determinism (all modes, 200 bodies, two builds)\n");
    {
        amrex::Box dom(amrex::IntVect(0), amrex::IntVect(31));
        amrex::Real lo[3] = {0.0, 0.0, 0.0};
        amrex::Real hi[3] = {1.0, 1.0, 1.0};
        amrex::RealBox rb(lo, hi);
        amrex::Geometry geom(dom, &rb, 0, nullptr);

        GridParams gp;
        gp.max_level = 4;
        const GridMode modes[4] = {GridMode::NoGrid, GridMode::Regular, GridMode::Adaptive,
                                   GridMode::Reluctant};
        for (const GridMode m : modes) {
            GridHierarchy g(geom, gp, m);
            g.build(bodies200, 2.5e-4);
            std::vector<ContactPair> a;
            g.candidatePairs(a);
            const contact::GridStats s1 = g.stats();

            g.build(bodies200, 2.5e-4);
            std::vector<ContactPair> b;
            g.candidatePairs(b);
            const contact::GridStats s2 = g.stats();

            CHECK(samePairs(a, b));
            CHECK(s1.levels == s2.levels);
            CHECK(s1.cells_total == s2.cells_total);
            CHECK(s1.cells_occupied == s2.cells_occupied);
            CHECK(s1.cells_refused == s2.cells_refused);
            CHECK(s1.virtual_links == s2.virtual_links);
            CHECK(s1.cell_pair_checks == s2.cell_pair_checks);
            CHECK(s1.raw_pairs == s2.raw_pairs);
            CHECK(s1.unique_pairs == s2.unique_pairs);
            CHECK(s2.refinements == 0 && s2.coarsenings == 0);  // same population

            // Fresh hierarchy on the same input must agree as well.
            GridHierarchy g2(geom, gp, m);
            g2.build(bodies200, 2.5e-4);
            std::vector<ContactPair> c;
            g2.candidatePairs(c);
            CHECK(samePairs(a, c));
            CHECK(g2.stats().unique_pairs == s1.unique_pairs);
            CHECK(g2.stats().raw_pairs == s1.raw_pairs);
            std::printf("  %-9s deterministic: pairs=%zu raw=%ld cells=%ld\n",
                        contact::toString(m), a.size(), s1.raw_pairs, s1.cells_total);
        }
    }

    // ---------------------------------------------------------------- test 6
    std::printf("\n[6] edge cases: empty population, single body, over-sized body\n");
    {
        amrex::Box dom(amrex::IntVect(0), amrex::IntVect(31));
        amrex::Real lo[3] = {0.0, 0.0, 0.0};
        amrex::Real hi[3] = {1.0, 1.0, 1.0};
        amrex::RealBox rb(lo, hi);
        amrex::Geometry geom(dom, &rb, 0, nullptr);

        GridParams gp;
        gp.max_level = 3;
        const GridMode modes[4] = {GridMode::NoGrid, GridMode::Regular, GridMode::Adaptive,
                                   GridMode::Reluctant};
        for (const GridMode m : modes) {
            GridHierarchy g(geom, gp, m);
            std::vector<Particle> none;
            g.build(none, 1e-3);
            std::vector<ContactPair> cand;
            g.candidatePairs(cand);
            CHECK(cand.empty());
            CHECK(g.stats().unique_pairs == 0);
            CHECK(g.stats().levels == 1);
            CHECK(g.stats().cells_total == 1);
            CHECK(g.minCellSize() > 0.0);
            const int empty_levels = g.stats().levels;
            const long empty_cells = g.stats().cells_total;

            std::vector<Particle> one{makeBody(0, Vec3{0.4, 0.4, 0.4}, 0.05, Vec3{0, 0, 0})};
            g.build(one, 1e-3);
            g.candidatePairs(cand);
            CHECK(cand.empty());
            CHECK(g.levelOf(0) >= 0);
            std::printf("  %-9s empty: levels=%d cells=%ld; single body on level %d\n",
                        contact::toString(m), empty_levels, empty_cells, g.levelOf(0));
        }

        // A body larger than the domain hits the literal "else max_level" branch of
        // the 6.1 rule and cannot be binned in a cell smaller than itself; the
        // hierarchy refuses instead of walking an unbounded stencil.
        bool threw = false;
        try {
            GridHierarchy g(geom, gp, GridMode::Adaptive);
            std::vector<Particle> giant{makeBody(0, Vec3{0.5, 0.5, 0.5}, 0.6, Vec3{0, 0, 0})};
            g.build(giant, 1e-3);
        } catch (const std::runtime_error& e) {
            threw = true;
            std::printf("  over-sized body rejected: %s\n", e.what());
        }
        CHECK(threw);
    }

    std::printf("\n%d checks, %d failures\n", g_checks, g_fail);
    amrex::Finalize();
    return (g_fail == 0) ? 0 : 1;
}
