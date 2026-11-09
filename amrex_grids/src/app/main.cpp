// Driver for the AMReX replication of
//   K. Krestenitis, "Parallel Multiscale Contact Dynamics for Rigid
//   Non-spherical Bodies", PhD thesis, Durham University, 2018.
//
// One binary, four broadphase morphologies (thesis 6.3), three narrowphase
// strategies (5.4-5.6), two contact laws (2.3) and the dynamic time step of
// chapter 7.

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <AMReX.H>
#include <AMReX_Box.H>
#include <AMReX_Geometry.H>
#include <AMReX_ParallelDescriptor.H>
#include <AMReX_RealBox.H>

#include "contact/Grid.hpp"
#include "contact/Narrowphase.hpp"
#include "contact/Physics.hpp"
#include "contact/Scenario.hpp"
#include "contact/Types.hpp"

namespace {

using namespace contact;

struct Options {
    std::string scenario{"pack"};
    std::string modes{"all"};          // all | nogrid | regular | adaptive | reluctant
    GridMode single_mode{GridMode::Adaptive};
    bool single_mode_set{false};
    NarrowphaseKind narrow{NarrowphaseKind::Hybrid};
    ContactModel model{ContactModel::Penalty};
    ShapeKind shape{ShapeKind::Icosa};
    int count{200};
    int steps{300};
    Real size_min{0.02};
    Real size_max{0.10};
    Real dt0{1.0e-3};
    Real velocity{0.0};
    Real domain{1.0};
    Real fill{0.25};
    Real kn{0.0};        // 0 = auto (stiffness scaled to the body mass)
    Real mu{0.5};
    Real restitution{0.5};
    std::string dt_source{"stiffness"};  // stiffness | grid (thesis ch.7 regime)
    bool dynamic_dt{true};
    bool verify{false};
    bool gravity{false};
    unsigned seed{12345};
    std::string csv{};
    bool quiet{false};
    GridParams grid{};
};

void usage() {
    std::cout <<
        "usage: contact_dem [options]\n"
        "  --scenario pack|two-body      scenario (default pack)\n"
        "  --mode all|nogrid|regular|adaptive|reluctant\n"
        "  --narrow brute|iterative|hybrid|hybrid-fast\n"
        "  --model penalty|springdashpot\n"
        "  --shape tetra|octa|icosa|hull\n"
        "  --n <bodies>  --steps <n>  --seed <n>\n"
        "  --size-min <r>  --size-max <r>\n"
        "  --dt <dt0>  --velocity <v>  --fixed-dt   (disable the chapter-7 controller)\n"
        "  --domain <L>  --fill <frac>  --kn <k>  --mu <m>  --restitution <e>\n"
        "  --dt-source stiffness|grid   (grid = thesis ch.7, dt limited by h_min only)\n"
        "  --gravity  --verify             (compare contacts against nogrid+brute)\n"
        "  --max-level <n>  --refine <r>   (grid refinement factor)\n"
        "  --csv <path>  --quiet\n";
}

bool parseArgs(int argc, char** argv, Options& o) {
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        auto next = [&](Real& dst) { if (i + 1 < argc) dst = std::atof(argv[++i]); };
        auto nexti = [&](int& dst) { if (i + 1 < argc) dst = std::atoi(argv[++i]); };
        if (a == "--help" || a == "-h") { usage(); return false; }
        else if (a == "--scenario") { if (++i < argc) o.scenario = argv[i]; }
        else if (a == "--mode") {
            if (++i < argc) {
                o.modes = argv[i];
                if (o.modes != "all") {
                    GridMode m;
                    if (!gridModeFromString(o.modes, m)) { std::cerr << "bad --mode\n"; return false; }
                    o.single_mode = m;
                    o.single_mode_set = true;
                }
            }
        }
        else if (a == "--narrow") {
            if (++i < argc) {
                const std::string s = argv[i];
                if (s == "brute") o.narrow = NarrowphaseKind::Brute;
                else if (s == "iterative") o.narrow = NarrowphaseKind::Iterative;
                else if (s == "hybrid") o.narrow = NarrowphaseKind::Hybrid;
                else if (s == "hybrid-fast") o.narrow = NarrowphaseKind::HybridFast;
                else { std::cerr << "bad --narrow\n"; return false; }
            }
        }
        else if (a == "--model") {
            if (++i < argc) {
                if (!contactModelFromString(argv[i], o.model)) { std::cerr << "bad --model\n"; return false; }
            }
        }
        else if (a == "--shape") {
            if (++i < argc) {
                if (!shapeFromString(argv[i], o.shape)) { std::cerr << "bad --shape\n"; return false; }
            }
        }
        else if (a == "--n") nexti(o.count);
        else if (a == "--steps") nexti(o.steps);
        else if (a == "--seed") { if (++i < argc) o.seed = static_cast<unsigned>(std::strtoul(argv[i], nullptr, 10)); }
        else if (a == "--size-min") next(o.size_min);
        else if (a == "--size-max") next(o.size_max);
        else if (a == "--dt") next(o.dt0);
        else if (a == "--velocity") next(o.velocity);
        else if (a == "--domain") next(o.domain);
        else if (a == "--fill") next(o.fill);
        else if (a == "--kn") next(o.kn);
        else if (a == "--mu") next(o.mu);
        else if (a == "--restitution") next(o.restitution);
        else if (a == "--dt-source") { if (++i < argc) o.dt_source = argv[i]; }
        else if (a == "--fixed-dt") o.dynamic_dt = false;
        else if (a == "--gravity") o.gravity = true;
        else if (a == "--verify") o.verify = true;
        else if (a == "--max-level") nexti(o.grid.max_level);
        else if (a == "--refine") nexti(o.grid.refine_factor);
        else if (a == "--csv") { if (++i < argc) o.csv = argv[i]; }
        else if (a == "--quiet") o.quiet = true;
        else { std::cerr << "unknown option: " << a << "\n"; usage(); return false; }
    }
    return true;
}

std::string pairKey(int i, int j) {
    std::ostringstream s;
    s << i << ':' << j;
    return s.str();
}

struct RunResult {
    std::string mode;
    long steps{0};
    long contacts{0};
    long candidate_pairs{0};
    long unique_pairs{0};
    long tri_pairs{0};
    long fallbacks{0};
    Real time{0.0};
    Real dt_mean{0.0};
    Real dt_max{0.0};
    Real dt_min{0.0};
    double wall_ms{0.0};
    GridStats grid{};
    long reference_missing{0};
    long reference_extra{0};
    bool verify_ok{true};
};

std::vector<ContactPoint> findContacts(const std::vector<Particle>& bodies,
                                       const std::vector<ContactPair>& pairs,
                                       Narrowphase& np,
                                       Real margin) {
    std::vector<ContactPoint> out;
    out.reserve(pairs.size() / 4 + 1);
    for (const auto& pr : pairs) {
        ContactPoint cp;
        if (np.query(bodies[pr.i], bodies[pr.j], margin, cp)) out.push_back(cp);
    }
    return out;
}

RunResult runMode(GridMode mode, const Options& opt, const amrex::Geometry& geom,
                  const std::vector<Particle>& initial, std::ostream* csv) {
    const amrex::RealBox domain_rb = geom.ProbDomain();
    Aabb box;
    box.lo = makeVec3(domain_rb.lo(0), domain_rb.lo(1), domain_rb.lo(2));
    box.hi = makeVec3(domain_rb.hi(0), domain_rb.hi(1), domain_rb.hi(2));

    std::vector<Particle> bodies = initial;
    GridHierarchy grid(geom, opt.grid, mode);
    Narrowphase np(opt.narrow, 0.0);
    ContactDynamics dyn;
    Material mat;
    mat.mu = opt.mu;
    mat.restitution = opt.restitution;
    Real m_min = 1.0;
    {
        Real m_sum = 0.0;
        m_min = 1.0e30;
        for (const auto& b : bodies) { m_sum += b.mass; m_min = std::min(m_min, b.mass); }
        if (bodies.empty()) m_min = 1.0;
        const Real m_mean = (bodies.empty() ? 1.0 : m_sum / static_cast<Real>(bodies.size()));
        (void)m_mean;
        // Stiffness scaled to the *lightest* body: the explicit integrator is
        // limited by the highest natural frequency, omega = sqrt(kn/m_min).
        mat.kn = (opt.kn > 0.0) ? opt.kn : 1.0e5 * m_min;
        mat.kt = 2.0 * mat.kn;
        const Real c_crit = 2.0 * std::sqrt(mat.kn * m_min);
        mat.gamma_n = 0.2 * c_crit;
        mat.gamma_t = 0.2 * c_crit;
    }
    DtState dts;
    dts.dynamic = opt.dynamic_dt;
    if (opt.dt_source == "grid") {
        // Thesis ch. 7 regime: the step is limited by the grid (h_min) and the
        // maximum velocity only, not by the contact stiffness.
        dts.dt = opt.dt0;
        dts.dt_max = 1.0e30;
    } else {
        // Explicit stability limit of the penalty spring (0.2 * period).
        const Real omega = std::sqrt(mat.kn / m_min);
        const Real dt_stable = (omega > 0.0) ? 0.2 / omega : opt.dt0;
        dts.dt_max = std::min(opt.dt0 * 100.0, dt_stable);
        dts.dt = std::min(opt.dt0, 0.5 * dt_stable);
    }
    DtController dtc(dts);

    // Reference oracle for --verify: mono-cell broadphase + brute force.
    std::unique_ptr<GridHierarchy> ref_grid;
    std::unique_ptr<Narrowphase> ref_np;
    if (opt.verify) {
        ref_grid = std::make_unique<GridHierarchy>(geom, opt.grid, GridMode::NoGrid);
        ref_np = std::make_unique<Narrowphase>(NarrowphaseKind::Brute, 0.0);
    }

    RunResult res;
    res.mode = toString(mode);

    Real dt = opt.dt0;
    Real t = 0.0;
    double wall = 0.0;
    Real dt_sum = 0.0;
    Real dt_hi = 0.0;
    Real dt_lo = 1.0e30;

    std::vector<ContactPair> pairs;
    std::vector<ContactPoint> contacts;

    for (int step = 0; step < opt.steps; ++step) {
        const auto t0 = std::chrono::steady_clock::now();
        double step_ms = 0.0;   // per-step wall time, the CSV column the bench sums

        grid.build(bodies, dt);
        pairs.clear();
        grid.candidatePairs(pairs);

        np.resetStats();
        contacts = findContacts(bodies, pairs, np, 0.0);

        // ---- validation against the no-grid + brute-force oracle -------------
        if (opt.verify) {
            std::vector<ContactPair> ref_pairs;
            ref_grid->build(bodies, dt);
            ref_grid->candidatePairs(ref_pairs);
            auto ref_contacts = findContacts(bodies, ref_pairs, *ref_np, 0.0);

            std::set<std::string> mine, ref;
            for (const auto& c : contacts) mine.insert(pairKey(c.i, c.j));
            for (const auto& c : ref_contacts) ref.insert(pairKey(c.i, c.j));
            for (const auto& k : ref) if (!mine.count(k)) ++res.reference_missing;
            for (const auto& k : mine) if (!ref.count(k)) ++res.reference_extra;
            if (res.reference_missing != 0) res.verify_ok = false;
        }

        // ---- forces ---------------------------------------------------------
        for (auto& b : bodies) { b.force = Vec3{0, 0, 0}; b.torque = Vec3{0, 0, 0}; }
        dyn.computeForces(bodies, contacts, mat, opt.model, dt);
        if (opt.gravity) {
            for (auto& b : bodies) b.force[2] -= b.mass * 9.81;
        }

        const auto diag = computeDiagnostics(bodies, contacts);
        res.contacts += static_cast<long>(contacts.size());
        res.candidate_pairs += static_cast<long>(pairs.size());
        res.tri_pairs += np.stats().tri_pairs;
        res.fallbacks += np.stats().fallbacks;
        res.unique_pairs += grid.stats().unique_pairs;

        // Snapshot for the CSV row (written at the end of the step, once the
        // wall time of the whole step is known).
        const int step_index = step;
        const Real step_t = t;
        const Real step_dt = dt;
        const long step_contacts = static_cast<long>(contacts.size());
        const long step_pairs = static_cast<long>(pairs.size());
        const long step_tri = np.stats().tri_pairs;
        const Real step_ke = diag.kinetic_linear + diag.kinetic_angular;
        const Real step_pen = diag.max_penetration;
        const GridStats step_grid = grid.stats();
        const Real step_hmin = grid.minCellSize();

        if (!opt.quiet && (step % 50 == 0 || step == opt.steps - 1)) {
            std::cout << "  [" << res.mode << "] step " << std::setw(5) << step
                      << "  t=" << std::fixed << std::setprecision(4) << t
                      << "  dt=" << std::scientific << dt
                      << "  pairs=" << pairs.size()
                      << "  contacts=" << contacts.size() << '\n';
        }

        // ---- advance --------------------------------------------------------
        dyn.integrate(bodies, dt);
        dyn.applyWalls(bodies, box, -1.0);

        Real h_min = grid.minCellSize();
        Real h_max = grid.maxCellSize();
        if (mode == GridMode::NoGrid) {
            // The mono-cell has no meaningful grid cutoff (thesis 6.3), so the
            // no-tunnelling bound of 7.1(iii) falls back to the particle scale.
            Real d_min = 1.0e30;
            for (const auto& b : bodies) d_min = std::min(d_min, 2.0 * b.radius);
            h_min = d_min;
            h_max = d_min;
        }
        const auto decision = dtc.update(bodies, pairs, h_min, h_max);
        const Real dt_next = dtc.state().dynamic ? decision.dt_next : dt;

        dt_sum += dt;
        dt_hi = std::max(dt_hi, dt);
        dt_lo = std::min(dt_lo, dt);
        t += dt;
        dt = std::max(dt_next, 1.0e-12);

        const auto t1 = std::chrono::steady_clock::now();
        step_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        wall += step_ms;

        if (csv) {
            *csv << res.mode << ',' << toString(opt.narrow) << ',' << step_index << ','
                 << step_t << ',' << step_dt << ',' << step_contacts << ',' << step_pairs << ','
                 << step_tri << ',' << step_pairs << ',' << step_ke << ',' << step_pen << ','
                 << step_ms << ',' << step_grid.levels << ',' << step_grid.cells_total << ','
                 << step_grid.cells_occupied << ',' << step_grid.cell_pair_checks << ','
                 << step_grid.virtual_links << ',' << step_grid.cells_refused << ','
                 << step_hmin << '\n';
        }
        res.steps = step + 1;
    }

    res.time = t;
    res.wall_ms = wall;
    res.dt_mean = (res.steps > 0) ? dt_sum / res.steps : 0.0;
    res.dt_max = dt_hi;
    res.dt_min = (dt_lo < 1.0e29) ? dt_lo : 0.0;
    res.grid = grid.stats();
    if (!opt.quiet) {
        std::cout << "---- " << res.mode << " ----\n";
        grid.printProfile(std::cout);
    }
    return res;
}

}  // namespace

int main(int argc, char** argv) {
    Options opt;
    if (!parseArgs(argc, argv, opt)) return 1;

    // AMReX gets a minimal argv; our own options were already consumed.
    int amrex_argc = 1;
    char* amrex_argv_storage[2] = {argv[0], nullptr};
    char** amrex_argv = amrex_argv_storage;
    // AMReX chatter (banner, TinyProfiler dump) goes to a null stream: the
    // driver prints its own, parseable report.
    std::ostringstream amrex_devnull;
    amrex::Initialize(amrex_argc, amrex_argv, std::function<void()>{},
                      amrex_devnull, amrex_devnull);

    int rc = 0;
    {
        const amrex::IntVect lo(AMREX_D_DECL(0, 0, 0));
        const amrex::IntVect hi(AMREX_D_DECL(31, 31, 31));
        const amrex::Box domain(lo, hi);
        const amrex::RealBox rb({AMREX_D_DECL(0.0, 0.0, 0.0)},
                                {AMREX_D_DECL(opt.domain, opt.domain, opt.domain)});
        const amrex::Array<int, AMREX_SPACEDIM> periodic{AMREX_D_DECL(0, 0, 0)};
        const amrex::Geometry geom(domain, rb, 0, periodic);

        Aabb box;
        box.lo = makeVec3(0.0, 0.0, 0.0);
        box.hi = makeVec3(opt.domain, opt.domain, opt.domain);

        std::vector<Particle> initial;
        if (opt.scenario == "two-body") {
            initial = buildTwoBodyCollision(box);
        } else {
            ScenarioSpec spec;
            spec.shape = opt.shape;
            spec.count = opt.count;
            spec.size_min = opt.size_min;
            spec.size_max = opt.size_max;
            spec.seed = opt.seed;
            spec.gravity = opt.gravity;
            spec.fill_fraction = opt.fill;
            spec.velocity = opt.velocity;
            initial = buildScenario(spec, box);
        }

        std::vector<GridMode> modes;
        if (opt.single_mode_set) {
            modes.push_back(opt.single_mode);
        } else {
            modes = {GridMode::NoGrid, GridMode::Regular, GridMode::Adaptive, GridMode::Reluctant};
        }

        std::vector<RunResult> results;
        for (GridMode m : modes) {
            std::ofstream csv_file;
            std::ostream* csv = nullptr;
            if (!opt.csv.empty()) {
                const std::string path = (modes.size() > 1)
                    ? opt.csv + "." + toString(m) + ".csv" : opt.csv;
                csv_file.open(path);
                if (csv_file) {
                    csv_file << "mode,narrowphase,step,time,dt,contacts,candidate_pairs,"
                                "tri_pairs,contact_checks,kinetic,penetration,wall_time_ms,"
                                "levels,cells_total,cells_occupied,cell_pair_checks,"
                                "virtual_links,cells_refused,h_min\n";
                    csv = &csv_file;
                }
            }
            results.push_back(runMode(m, opt, geom, initial, csv));
        }

        // ---- summary --------------------------------------------------------
        std::cout << "\n" << std::left << std::setw(12) << "mode"
                  << std::right << std::setw(8) << "steps"
                  << std::setw(10) << "contacts"
                  << std::setw(12) << "cand.pairs"
                  << std::setw(12) << "tri.pairs"
                  << std::setw(12) << "mean dt"
                  << std::setw(12) << "max dt"
                  << std::setw(12) << "wall[ms]"
                  << ((opt.verify) ? "  verify" : "") << '\n';
        for (const auto& r : results) {
            std::cout << std::left << std::setw(12) << r.mode << std::right
                      << std::setw(8) << r.steps
                      << std::setw(10) << r.contacts
                      << std::setw(12) << r.candidate_pairs
                      << std::setw(12) << r.tri_pairs
                      << std::setw(12) << std::scientific << std::setprecision(3) << r.dt_mean
                      << std::setw(12) << r.dt_max
                      << std::setw(12) << std::fixed << std::setprecision(1) << r.wall_ms;
            if (opt.verify) {
                std::cout << "  " << (r.verify_ok ? "PASS" : "FAIL")
                          << " (missing=" << r.reference_missing
                          << ", extra=" << r.reference_extra << ')';
                if (!r.verify_ok) rc = 2;
            }
            std::cout << '\n';
        }
        std::cout << std::defaultfloat;
    }
    amrex::Finalize();
    return rc;
}
