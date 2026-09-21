/*
 * Checks MeshBuffer::validate(), the guard that makes the shared VTU writer
 * fail loudly instead of emitting a silently mangled grid. Each case is a way
 * a .vtu gets corrupted:
 *
 *   - a cell id past the end of the point array (the bug that collapsed every
 *     AMReX box onto the last one),
 *   - a degenerate cell,
 *   - a field whose length does not match the geometry.
 *
 * Run: ctest --test-dir build  (or ./build/delta_meshbuffer_test)
 * CHECK() rather than assert() because a Release build defines -DNDEBUG.
 */

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include "core/io/MeshBuffer.h"

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      std::abort();                                                        \
    }                                                                      \
  } while (0)

#define CHECK_THROWS(expr)                                                  \
  do {                                                                      \
    bool threw = false;                                                     \
    try { (void)(expr); } catch (const std::logic_error&) { threw = true; }  \
    CHECK(threw);                                                           \
  } while (0)

using delta::core::io::MeshBuffer;

namespace {

// Two triangles sharing an edge, with consistent fields.
MeshBuffer validBuffer() {
  MeshBuffer b;
  b.points.push_back({{0.0, 0.0, 0.0}});
  b.points.push_back({{1.0, 0.0, 0.0}});
  b.points.push_back({{0.0, 1.0, 0.0}});
  b.points.push_back({{1.0, 1.0, 0.0}});
  b.cells.push_back({5, {0, 1, 2}});  // VTK_TRIANGLE
  b.cells.push_back({5, {1, 3, 2}});
  b.pointFields.push_back({"radius", 1, false, {0.1, 0.1, 0.1, 0.1}});
  b.cellFields.push_back({"global_id", 1, true, {0.0, 1.0}});
  return b;
}

}  // namespace

int main() {
  // A well-formed buffer passes the guard.
  {
    MeshBuffer b = validBuffer();
    b.validate();
  }

  // A cell id past the end of the point array is the offset bug.
  {
    MeshBuffer b = validBuffer();
    b.cells[1] = {5, {1, 3, 9}};
    CHECK_THROWS(b.validate());
  }

  // A cell with fewer than two points cannot be drawn.
  {
    MeshBuffer b = validBuffer();
    b.cells.push_back({1, {0}});
    CHECK_THROWS(b.validate());
  }

  // A point field that does not cover every point.
  {
    MeshBuffer b = validBuffer();
    b.pointFields.push_back({"velocity", 3, false, {0.0, 0.0, 0.0}});
    CHECK_THROWS(b.validate());
  }

  // A cell field that does not cover every cell.
  {
    MeshBuffer b = validBuffer();
    b.cellFields.push_back({"material", 1, true, {0.0}});
    CHECK_THROWS(b.validate());
  }

  std::printf("mesh_buffer_test OK: validate() rejects dangling, degenerate "
              "and length-mismatched buffers\n");
  return 0;
}
