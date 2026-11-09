#pragma once
// Minimal ASCII VTU (XML UnstructuredGrid) writer for the two-body and
// packing experiments: rigid triangles in world frame plus the broadphase
// cells of the current grid morphology, so ParaView shows what the grid is
// actually doing (regular / adaptive / reluctant refinement) next to the bodies.

#include <string>
#include <vector>

#include "contact/Grid.hpp"
#include "contact/Types.hpp"

namespace contact {

struct Frame {
    Real time{0.0};
    int step{0};
    std::string file;  // basename, for the .pvd collection
};

// Writes <prefix>_<step:05d>.vtu plus a <prefix>.pvd time series.
// CellData: kind (0 = body triangle, 1 = grid cell), body, level, occupied.
class VtuWriter {
  public:
    explicit VtuWriter(std::string prefix) : prefix_(std::move(prefix)) {}

    void write(int step, Real time, const std::vector<Particle>& bodies,
               const std::vector<CellViz>& cells);

    // Call once at the end: writes the ParaView collection file.
    void writeCollection() const;

    [[nodiscard]] const std::string& prefix() const noexcept { return prefix_; }
    [[nodiscard]] std::size_t frames() const noexcept { return frames_.size(); }

  private:
    std::string prefix_;
    std::vector<Frame> frames_;
};

}  // namespace contact
