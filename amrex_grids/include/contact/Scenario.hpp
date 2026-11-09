#pragma once
// Body shapes and scenarios for the replication experiments.

#include <string>
#include <vector>

#include "contact/Types.hpp"

namespace contact {

enum class ShapeKind { Tetra, Octa, Icosa, RandomHull };

[[nodiscard]] const char* toString(ShapeKind s) noexcept;
[[nodiscard]] bool shapeFromString(const std::string& s, ShapeKind& out) noexcept;

// Convex, closed, outward-oriented triangle meshes, centred on the centroid
// and scaled so that the bounding-sphere radius equals `radius`.
Mesh makeShape(ShapeKind kind, Real radius, unsigned seed);

struct ScenarioSpec {
    std::string name{"random-packing"};
    ShapeKind shape{ShapeKind::Icosa};
    int count{200};
    Real size_min{0.02};
    Real size_max{0.06};
    Real fill_fraction{0.25};
    Real velocity{0.0};          // initial speed scale
    bool gravity{false};
    Real gravity_g{9.81};
    unsigned seed{12345};
    bool ground_plane{true};
};

// Lattice-jittered non-overlapping insertion into `domain`.
[[nodiscard]] std::vector<Particle> buildScenario(const ScenarioSpec& spec, const Aabb& domain);

// Thesis 7.3 experiment: two bodies of diameter 0.02 and 0.2 in the unit cube,
// at (0.2,0.2,0.2) and (0.8,0.8,0.8), velocities +-0.1 along the diagonal.
[[nodiscard]] std::vector<Particle> buildTwoBodyCollision(const Aabb& domain,
                                                          Real diameter_a = 0.02,
                                                          Real diameter_b = 0.20);

}  // namespace contact
