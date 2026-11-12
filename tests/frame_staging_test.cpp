/*
 * Checks FrameStaging::validate(), the guard that makes the shared VTU writer
 * fail loudly instead of emitting a silently mangled grid. Each case is a way
 * a .vtu gets corrupted:
 *
 *   - a cell id past the end of the point array (the bug that collapsed every
 *     AMReX box onto the last one),
 *   - a degenerate cell,
 *   - a field whose length does not match the geometry.
 *
 * Then it writes the same staging through writeVTUAscii and reads the file back,
 * which is the only thing keeping the hand-written XML honest against the
 * library emitter.
 *
 * Run: ctest --test-dir build  (or ./build/delta_frame_staging_test)
 * CHECK() rather than assert() because a Release build defines -DNDEBUG.
 */

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "core/io/FrameStaging.h"

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

using delta::core::io::FrameStaging;

namespace {

// Two triangles sharing an edge, with consistent fields.
FrameStaging validBuffer() {
  FrameStaging b;
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

/* Read one <DataArray> body back out of the ascii file and parse the numbers.
 * Deliberately dumb: find the Name, take everything up to </DataArray>.
 * An empty name means "the one inside <Points>", which is the only unnamed
 * array in the file. */
std::vector<double> readArray(const std::string& xml, const std::string& name) {
  std::size_t at;
  if (name.empty()) {
    at = xml.find("<Points>");
    if (at == std::string::npos) return std::vector<double>();
    at = xml.find("<DataArray", at);
  } else {
    at = xml.find("Name=\"" + name + "\"");
    if (at == std::string::npos) return std::vector<double>();
    at = xml.rfind("<DataArray", at);
  }
  if (at == std::string::npos) return std::vector<double>();
  at = xml.find('>', at);
  const std::size_t end = xml.find("</DataArray>", at);
  if (end == std::string::npos) return std::vector<double>();
  std::istringstream body(xml.substr(at + 1, end - at - 1));

  std::vector<double> out;
  double v;
  while (body >> v) out.push_back(v);
  return out;
}

std::string slurp(const std::string& path) {
  std::ifstream in(path.c_str());
  std::ostringstream ss;
  ss << in.rdbuf();
  return ss.str();
}

}  // namespace

int main() {
  // A well-formed buffer passes the guard.
  {
    FrameStaging b = validBuffer();
    b.validate();
  }

  // A cell id past the end of the point array is the offset bug.
  {
    FrameStaging b = validBuffer();
    b.cells[1] = {5, {1, 3, 9}};
    CHECK_THROWS(b.validate());
  }

  // A cell with fewer than two points cannot be drawn.
  {
    FrameStaging b = validBuffer();
    b.cells.push_back({1, {0}});
    CHECK_THROWS(b.validate());
  }

  // A point field that does not cover every point.
  {
    FrameStaging b = validBuffer();
    b.pointFields.push_back({"velocity", 3, false, {0.0, 0.0, 0.0}});
    CHECK_THROWS(b.validate());
  }

  // A cell field that does not cover every cell.
  {
    FrameStaging b = validBuffer();
    b.cellFields.push_back({"material", 1, true, {0.0}});
    CHECK_THROWS(b.validate());
  }

  /* ---- the ascii emitter ------------------------------------------------
   * Write the same staging, read it back, compare every number. This is what
   * stops the hand-written XML from drifting away from the VTK emitter. */
  {
    FrameStaging b = validBuffer();
    const std::string dir = "frame_staging_out/";
    std::string mk = "mkdir -p " + dir;
    if (std::system(mk.c_str()) != 0) { std::fprintf(stderr, "mkdir failed\n"); return 1; }

    delta::core::io::writeVTUAscii(dir, "check", 7, b);

    const std::string xml = slurp(dir + "check_7.vtu");
    CHECK(!xml.empty());

    /* well-formed enough to be recognised */
    CHECK(xml.find("<VTKFile type=\"UnstructuredGrid\"") != std::string::npos);
    CHECK(xml.find("NumberOfPoints=\"4\"") != std::string::npos);
    CHECK(xml.find("NumberOfCells=\"2\"") != std::string::npos);

    /* points survive exactly: %.17g is round-trip exact for a double */
    const std::vector<double> pts = readArray(xml, "");
    CHECK(pts.size() == 4 * 3);
    for (std::size_t i = 0; i < b.points.size(); ++i)
      for (int d = 0; d < 3; ++d)
        CHECK(pts[i * 3 + d] == b.points[i][d]);

    /* connectivity, offsets and types */
    const std::vector<double> conn = readArray(xml, "connectivity");
    CHECK(conn.size() == 6);
    CHECK(conn[0] == 0 && conn[1] == 1 && conn[2] == 2);
    CHECK(conn[3] == 1 && conn[4] == 3 && conn[5] == 2);

    const std::vector<double> offs = readArray(xml, "offsets");
    CHECK(offs.size() == 2);
    CHECK(offs[0] == 3 && offs[1] == 6);   /* prefix sums, not per-cell counts */

    const std::vector<double> types = readArray(xml, "types");
    CHECK(types.size() == 2);
    CHECK(types[0] == 5 && types[1] == 5);

    /* point field with its name, and the active-scalar hint */
    const std::vector<double> rad = readArray(xml, "radius");
    CHECK(rad.size() == 4);
    for (std::size_t i = 0; i < rad.size(); ++i) CHECK(rad[i] == 0.1);

    const std::vector<double> gid = readArray(xml, "global_id");
    CHECK(gid.size() == 2);
    CHECK(gid[0] == 0.0 && gid[1] == 1.0);

    /* the rule: a bad staging must not reach the file at all */
    FrameStaging bad = validBuffer();
    bad.cells[1] = {5, {1, 3, 99}};
    bool threw = false;
    try { delta::core::io::writeVTUAscii(dir, "bad", 0, bad); }
    catch (const std::logic_error&) { threw = true; }
    CHECK(threw);

    std::printf("  ascii emitter: %s\n",
                delta::core::io::hasVtkWriter() ? "VTK also available"
                                                : "only emitter (no VTK)");
  }

  std::printf("frame_staging_test OK: validate() rejects dangling, degenerate "
              "and length-mismatched buffers; writeVTUAscii round-trips\n");
  return 0;
}
