// Scenario construction: bodies + shapes for the replication experiments.
//
// Insertion is a lattice-jittered rejection sampler:
//   * the domain is cut into nside^3 equal cells; `count` bodies are pre-assigned
//     to distinct cells (round-robin over a deterministically shuffled cell list,
//     so a cell may hold several bodies when count > nside^3),
//   * each attempt draws new random positions inside the assigned cell (the body
//     is kept fully inside its cell, hence inside the domain),
//   * an attempt is accepted only when no pair of bodies overlaps in the
//     bounding-sphere sense: |c_i - c_j| >= r_i + r_j.  Overlaps are found with a
//     uniform hash grid of cell size 2*max_radius.
//   * after 200 rejected attempts the lattice is grown (nside += 1), which gives
//     every body more room; after 8 growth steps the scenario is reported as
//     infeasible instead of silently emitting overlapping bodies.
//
// Everything is a pure function of ScenarioSpec::seed: positions, sizes, shapes
// and initial velocities are bit-identical across runs and platforms (the RNG is
// splitmix64, not std:: distributions).
//
// Conventions (the headers are fixed, so they are documented here):
//   * ScenarioSpec::size_min / size_max are DIAMETERS, matching the thesis
//     vocabulary (particle diameters 0.02 vs 0.2 in 7.3, and the grid rule
//     h_L >= size_slack * diameter).  radius = 0.5 * diameter.
//   * ScenarioSpec::fill_fraction is advisory: the achieved solid fraction is
//     sum(V_i)/V_domain, determined by count and the size range.  It is not
//     enforced, because the insertion is a hard-sphere rejection sampler.
//   * ScenarioSpec::gravity / gravity_g / ground_plane are informational flags
//     only; buildScenario never writes Particle::force.  The driver applies
//     gravity (and the ground plane) during the simulation.

#include "contact/Scenario.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace contact {
namespace {

constexpr Real kDensity = 1.0;        // mass = density * mesh volume
constexpr Real kHaloEpsilon = 0.1;    // halo = radius * (1 + epsilon), 7.2
constexpr int kAttemptsPerLattice = 200;
constexpr int kMaxLatticeGrowth = 8;
constexpr int kBodyTriesPerAttempt = 24;

// Deterministic splitmix64 (kept local: the scenario code must not depend on
// std:: distribution implementations).
struct Rng {
    std::uint64_t s;

    explicit Rng(std::uint64_t seed) noexcept : s(seed + 0x9E3779B97F4A7C15ULL) {}

    std::uint64_t next() noexcept {
        std::uint64_t z = (s += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    Real uniform() noexcept {
        return static_cast<Real>(next() >> 11) * (1.0 / 9007199254740992.0);
    }

    Real uniform(Real lo, Real hi) noexcept { return lo + (hi - lo) * uniform(); }

    Vec3 unitVector() noexcept {
        for (;;) {
            const Real x = 2.0 * uniform() - 1.0;
            const Real y = 2.0 * uniform() - 1.0;
            const Real z = 2.0 * uniform() - 1.0;
            const Real r2 = x * x + y * y + z * z;
            if (r2 > 1.0e-6 && r2 <= 1.0) {
                const Real inv = 1.0 / std::sqrt(r2);
                return Vec3{x * inv, y * inv, z * inv};
            }
        }
    }

    std::uint64_t below(std::uint64_t n) noexcept {   // n > 0
        return next() % n;
    }
};

std::uint64_t mixSeed(unsigned seed, std::uint64_t salt) noexcept {
    std::uint64_t z = static_cast<std::uint64_t>(seed) + salt * 0x9E3779B97F4A7C15ULL;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

// Mesh volume through the signed tetrahedron sum (|V|; makeShape guarantees the
// winding is outward so the sum is positive).
Real meshVolume(const Mesh& mesh) {
    Real sum6 = 0.0;
    for (const Triangle& t : mesh) {
        sum6 += dot(t.v[0], cross(t.v[1], t.v[2]));
    }
    return std::abs(sum6) / 6.0;
}

void finalizeParticle(Particle& p, Real radius, Mesh mesh) {
    p.radius = radius;
    p.halo = radius * (1.0 + kHaloEpsilon);
    p.mesh = std::move(mesh);
    p.mass = kDensity * meshVolume(p.mesh);
    // Scalar (solid sphere) inertia approximation I = 2/5 m r^2.
    p.inertia = 0.4 * p.mass * radius * radius;
    p.inv_mass = (p.mass > 0.0) ? 1.0 / p.mass : 0.0;
    p.inv_inertia = (p.inertia > 0.0) ? 1.0 / p.inertia : 0.0;
    p.quat = Quat{};
    p.level = 0;
    p.bin = 0;
    p.force = Vec3{0.0, 0.0, 0.0};
    p.torque = Vec3{0.0, 0.0, 0.0};
}

void checkDomain(const Aabb& domain) {
    for (int d = 0; d < 3; ++d) {
        if (!(domain.hi[d] > domain.lo[d])) {
            throw std::invalid_argument("buildScenario: empty domain");
        }
        if (!std::isfinite(domain.lo[d]) || !std::isfinite(domain.hi[d])) {
            throw std::invalid_argument("buildScenario: non-finite domain bounds");
        }
    }
}

// Uniform hash grid used only for the rejection test.
class HashGrid {
  public:
    HashGrid(const Aabb& box, Real cell)
        : lo_(box.lo), cell_(cell > 0.0 ? cell : 1.0) {
        for (int d = 0; d < 3; ++d) {
            n_[d] = std::max(1, static_cast<int>(std::ceil((box.hi[d] - box.lo[d]) / cell_)));
        }
    }

    void insert(int id, const Vec3& p, Real r) {
        const auto key = keyOf(p);
        buckets_[hash(key)].push_back(Entry{id, p, r});
    }

    bool overlaps(const Vec3& p, Real r) const {
        const auto c = keyOf(p);
        for (int dz = -1; dz <= 1; ++dz) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    const std::array<int, 3> k{c[0] + dx, c[1] + dy, c[2] + dz};
                    const auto it = buckets_.find(hash(k));
                    if (it == buckets_.end()) continue;
                    for (const Entry& e : it->second) {
                        const Real rr = r + e.r;
                        if (norm2(p - e.p) < rr * rr) return true;
                    }
                }
            }
        }
        return false;
    }

  private:
    struct Entry {
        int id;
        Vec3 p;
        Real r;
    };

    std::array<int, 3> keyOf(const Vec3& p) const noexcept {
        std::array<int, 3> k{};
        for (int d = 0; d < 3; ++d) {
            k[d] = static_cast<int>(std::floor((p[d] - lo_[d]) / cell_));
        }
        return k;
    }

    static std::uint64_t hash(const std::array<int, 3>& k) noexcept {
        std::uint64_t h = 1469598103934665603ULL;
        for (int d = 0; d < 3; ++d) {
            h ^= static_cast<std::uint64_t>(static_cast<std::int64_t>(k[d]) + 0x9E3779B9LL);
            h *= 1099511628211ULL;
            h ^= h >> 29;
        }
        return h;
    }

    Vec3 lo_{};
    Real cell_{1.0};
    std::array<int, 3> n_{{1, 1, 1}};
    std::unordered_map<std::uint64_t, std::vector<Entry>> buckets_;
};

}  // namespace

std::vector<Particle> buildScenario(const ScenarioSpec& spec, const Aabb& domain) {
    checkDomain(domain);

    const int count = spec.count;
    if (count < 0) throw std::invalid_argument("buildScenario: negative count");
    if (count == 0) return {};

    const Real d_lo = std::min(spec.size_min, spec.size_max);
    const Real d_hi = std::max(spec.size_min, spec.size_max);
    if (!(d_lo > 0.0)) throw std::invalid_argument("buildScenario: size_min must be positive");

    Vec3 extent{domain.hi[0] - domain.lo[0], domain.hi[1] - domain.lo[1],
                domain.hi[2] - domain.lo[2]};

    Rng rng(mixSeed(spec.seed, 0x5CE7A710ULL));

    // ---- sizes, shapes, velocities (drawn before placement so that a rejected
    // attempt re-uses the same bodies, only the jitter changes) ----------------
    std::vector<Real> radii(static_cast<std::size_t>(count));
    std::vector<Vec3> dirs(static_cast<std::size_t>(count));
    std::vector<Mesh> meshes(static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        const Real diameter = d_lo + (d_hi - d_lo) * rng.uniform();
        const Real r = 0.5 * diameter;
        radii[static_cast<std::size_t>(i)] = r;
        dirs[static_cast<std::size_t>(i)] = rng.unitVector();
        const unsigned shape_seed =
            static_cast<unsigned>(mixSeed(spec.seed, 0x1000ULL + static_cast<std::uint64_t>(i)));
        meshes[static_cast<std::size_t>(i)] = makeShape(spec.shape, r, shape_seed);
    }

    Real r_max = 0.0;
    for (Real r : radii) r_max = std::max(r_max, r);
    if (r_max <= 0.0) throw std::runtime_error("buildScenario: zero radius");

    // ---- lattice ------------------------------------------------------------
    // Start coarse enough that cells hold ~2 bodies; rejections then refine it.
    int nside = std::max(1, static_cast<int>(std::floor(std::cbrt(static_cast<double>(count) / 2.0))));
    if (static_cast<long>(nside) * nside * nside < count && nside < 2) nside = 2;

    std::vector<Vec3> pos(static_cast<std::size_t>(count));

    for (int growth = 0; growth <= kMaxLatticeGrowth; ++growth, ++nside) {
        const long ncells = static_cast<long>(nside) * nside * nside;
        if (ncells < count) continue;   // cannot host the bodies at all

        // Cell edge per axis; the jitter box is clipped so that a body stays
        // inside its cell (a body in a cell smaller than itself simply cannot be
        // jittered, it is placed at the cell centre and validated like any other).
        std::array<Real, 3> cell{};
        for (int d = 0; d < 3; ++d) cell[d] = extent[d] / static_cast<Real>(nside);

        // Deterministically shuffled cell list (Fisher-Yates with splitmix64).
        std::vector<long> cells(static_cast<std::size_t>(ncells));
        for (long c = 0; c < ncells; ++c) cells[static_cast<std::size_t>(c)] = c;
        for (long c = ncells - 1; c > 0; --c) {
            const long j = static_cast<long>(rng.below(static_cast<std::uint64_t>(c + 1)));
            std::swap(cells[static_cast<std::size_t>(c)], cells[static_cast<std::size_t>(j)]);
        }

        std::vector<std::array<int, 3>> cell_of(static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) {
            const long c = cells[static_cast<std::size_t>(i)];
            cell_of[static_cast<std::size_t>(i)] = {
                static_cast<int>(c % nside),
                static_cast<int>((c / nside) % nside),
                static_cast<int>(c / (static_cast<long>(nside) * nside)),
            };
        }

        for (int attempt = 0; attempt < kAttemptsPerLattice; ++attempt) {
            bool ok = true;
            HashGrid grid(domain, 2.0 * r_max);

            for (int i = 0; i < count && ok; ++i) {
                const Real r = radii[static_cast<std::size_t>(i)];
                const std::array<int, 3>& ci = cell_of[static_cast<std::size_t>(i)];

                // Box inside the cell that keeps the whole body in the cell.
                std::array<Real, 3> lo{};
                std::array<Real, 3> hi{};
                for (int d = 0; d < 3; ++d) {
                    const Real c0 = domain.lo[d] + static_cast<Real>(ci[d]) * cell[d];
                    const Real c1 = c0 + cell[d];
                    lo[d] = c0 + r;
                    hi[d] = c1 - r;
                    if (hi[d] < lo[d]) {   // cell narrower than the body
                        lo[d] = hi[d] = 0.5 * (c0 + c1);
                    }
                    // never leave the domain, whatever the lattice says
                    lo[d] = std::max(lo[d], domain.lo[d] + r);
                    hi[d] = std::min(hi[d], domain.hi[d] - r);
                    if (hi[d] < lo[d]) {
                        lo[d] = hi[d] = 0.5 * (domain.lo[d] + domain.hi[d]);
                    }
                }

                bool placed = false;
                for (int t = 0; t < kBodyTriesPerAttempt; ++t) {
                    Vec3 p{rng.uniform(lo[0], hi[0]), rng.uniform(lo[1], hi[1]),
                           rng.uniform(lo[2], hi[2])};
                    if (!grid.overlaps(p, r)) {
                        pos[static_cast<std::size_t>(i)] = p;
                        grid.insert(i, p, r);
                        placed = true;
                        break;
                    }
                }
                if (!placed) ok = false;
            }

            if (!ok) continue;

            // Final independent verification of the whole set (defensive: the
            // hash grid is approximate at its borders, this one is exact).
            bool clean = true;
            for (int i = 0; i < count && clean; ++i) {
                for (int j = i + 1; j < count; ++j) {
                    const Real rr = radii[static_cast<std::size_t>(i)] +
                                    radii[static_cast<std::size_t>(j)];
                    if (norm2(pos[static_cast<std::size_t>(i)] -
                              pos[static_cast<std::size_t>(j)]) < rr * rr) {
                        clean = false;
                        break;
                    }
                }
            }
            if (!clean) continue;

            // ---- accept ------------------------------------------------------
            std::vector<Particle> bodies(static_cast<std::size_t>(count));
            for (int i = 0; i < count; ++i) {
                Particle& p = bodies[static_cast<std::size_t>(i)];
                p.id = i;
                p.pos = pos[static_cast<std::size_t>(i)];
                p.vel = (spec.velocity != 0.0)
                            ? dirs[static_cast<std::size_t>(i)] * spec.velocity
                            : Vec3{0.0, 0.0, 0.0};
                p.omega = Vec3{0.0, 0.0, 0.0};
                finalizeParticle(p, radii[static_cast<std::size_t>(i)],
                                 meshes[static_cast<std::size_t>(i)]);
            }
            return bodies;
        }
    }

    throw std::runtime_error(
        "buildScenario: could not insert " + std::to_string(count) +
        " non-overlapping bodies of diameter [" + std::to_string(d_lo) + ", " +
        std::to_string(d_hi) + "] into the domain (lattice exhausted)");
}

std::vector<Particle> buildTwoBodyCollision(const Aabb& domain, Real diameter_a,
                                            Real diameter_b) {
    checkDomain(domain);
    if (!(diameter_a > 0.0) || !(diameter_b > 0.0)) {
        throw std::invalid_argument("buildTwoBodyCollision: diameters must be positive");
    }

    // Thesis 7.3: unit cube, centres (0.2,0.2,0.2) and (0.8,0.8,0.8), velocities
    // (0.1,0.1,0.1) and (-0.1,-0.1,-0.1) -- 0.1 per axis, i.e. 0.1*sqrt(3) speed
    // along the body diagonal, collision at t ~ 2.7.
    const Vec3 ca{0.2, 0.2, 0.2};
    const Vec3 cb{0.8, 0.8, 0.8};
    const Vec3 va{0.1, 0.1, 0.1};
    const Vec3 vb{-0.1, -0.1, -0.1};

    for (int d = 0; d < 3; ++d) {
        if (ca[d] < domain.lo[d] || ca[d] > domain.hi[d] || cb[d] < domain.lo[d] ||
            cb[d] > domain.hi[d]) {
            throw std::invalid_argument(
                "buildTwoBodyCollision: thesis positions (0.2..0.8) are outside the domain");
        }
    }

    const Real ra = 0.5 * diameter_a;
    const Real rb = 0.5 * diameter_b;

    std::vector<Particle> bodies(2);
    bodies[0].id = 0;
    bodies[0].pos = ca;
    bodies[0].vel = va;
    finalizeParticle(bodies[0], ra, makeShape(ShapeKind::Icosa, ra, 0u));

    bodies[1].id = 1;
    bodies[1].pos = cb;
    bodies[1].vel = vb;
    finalizeParticle(bodies[1], rb, makeShape(ShapeKind::Icosa, rb, 1u));

    return bodies;
}

}  // namespace contact
