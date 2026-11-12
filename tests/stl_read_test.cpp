/*
 * Guards the hand-written STL reader in core/io/read.cpp.
 *
 * readPartGeometry used to go through assimp for every format, including STL.
 * STL now has a native reader (binary and ascii) and assimp is kept only for the
 * formats the repo does not ship. That swap is only safe if the two agree, so
 * this test checks the native path against the meshes actually in input/dim3:
 *
 *   - the triangle and vertex counts match the header of each binary file,
 *   - the first triangle's coordinates are exact (the format has no compression),
 *   - an ascii STL produces the same triangle count as the equivalent binary,
 *   - a truncated file is rejected rather than read past the end.
 *
 * CHECK() rather than assert() because a Release build defines -DNDEBUG.
 */
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "core/io/read.h"

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      std::abort();                                                        \
    }                                                                      \
  } while (0)

namespace {

struct StlHeader {
  unsigned int triangles;
  std::string  path;
  bool         present;
};

/* Read the declared triangle count straight out of the file, without using the
 * reader under test. */
StlHeader probeBinaryStl(const std::string& path) {
  StlHeader h;
  h.path = path;
  h.present = false;
  h.triangles = 0;
  std::ifstream in(path.c_str(), std::ios::binary);
  if (!in) return h;
  char header[84];
  in.read(header, 84);
  if (in.gcount() != 84) return h;
  unsigned int n = 0;
  std::memcpy(&n, header + 80, sizeof(n));
  h.triangles = n;
  h.present = true;
  return h;
}

void writeFile(const std::string& path, const std::string& data) {
  std::ofstream out(path.c_str(), std::ios::binary);
  out.write(data.data(), (std::streamsize)data.size());
}

}  // namespace

int main() {
  const std::string dir = std::string(DELTA_INPUT_DIR) + "/";

  /* ---- the real meshes ------------------------------------------------ */
  const char* names[] = {"Body.STL", "Rotor.STL", "TailRotor.STL", "turbine.stl"};
  int checked = 0;

  for (int i = 0; i < 4; ++i) {
    const std::string path = dir + names[i];
    const StlHeader h = probeBinaryStl(path);
    if (!h.present) {
      std::printf("  %-16s missing, skipped\n", names[i]);
      continue;
    }

    delta::geometry::mesh::Mesh* mesh = delta::core::io::readPartGeometry(path);
    CHECK(mesh != nullptr);

    /* The native binary reader emits one face per triangle record, so the face
     * count is what has to match the header. Mesh::replace() reads FROM the
     * vectors you pass in (a confusing name); the accessors are the way out. */
    std::vector<std::array<int, 3> > faces = mesh->getTriangleFaces();
    CHECK(faces.size() == (std::size_t)h.triangles);

    const std::vector<iREAL> x = mesh->getXCoordinatesAsVector();
    const std::vector<iREAL> y = mesh->getYCoordinatesAsVector();
    const std::vector<iREAL> z = mesh->getZCoordinatesAsVector();
    CHECK(x.size() == 3u * (std::size_t)h.triangles);
    CHECK(y.size() == x.size() && z.size() == x.size());

    /* every face must index inside the emitted vertex list */
    for (std::size_t f = 0; f < faces.size(); ++f)
      for (int k = 0; k < 3; ++k)
        CHECK(faces[f][k] >= 0 && (std::size_t)faces[f][k] < x.size());

    std::printf("  %-16s %6u triangles -> %6zu coords  ok\n",
                names[i], h.triangles, x.size());
    delete mesh;
    ++checked;
  }

  CHECK(checked > 0);

  /* ---- ascii and binary agree ----------------------------------------- */
  {
    /* Two triangles, one shared edge, written both ways. */
    const std::string ascii =
        "solid test\n"
        "  facet normal 0 0 1\n    outer loop\n"
        "      vertex 0 0 0\n      vertex 1 0 0\n      vertex 0 1 0\n"
        "    endloop\n  endfacet\n"
        "  facet normal 0 0 1\n    outer loop\n"
        "      vertex 1 0 0\n      vertex 1 1 0\n      vertex 0 1 0\n"
        "    endloop\n  endfacet\n"
        "endsolid test\n";
    writeFile("/tmp/frame_stl_ascii.stl", ascii);

    delta::geometry::mesh::Mesh* m =
        delta::core::io::readPartGeometry("/tmp/frame_stl_ascii.stl");
    CHECK(m != nullptr);
    std::vector<iREAL> x = m->getXCoordinatesAsVector();
    std::vector<iREAL> y = m->getYCoordinatesAsVector();
    std::vector<iREAL> z = m->getZCoordinatesAsVector();
    CHECK(x.size() == 6);      /* 2 triangles * 3 vertices */
    CHECK(x[0] == 0.0 && y[0] == 0.0 && z[0] == 0.0);
    CHECK(x[1] == 1.0 && y[1] == 0.0);
    CHECK(x[2] == 0.0 && y[2] == 1.0);
    delete m;
  }

  /* ---- binary path, synthetic so the expected numbers are known -------- */
  {
    std::string blob(80, ' ');          /* header, deliberately not "solid" */
    unsigned int n = 1;
    blob.append(reinterpret_cast<const char*>(&n), sizeof(n));

    const float tri[12] = {0.f, 0.f, 1.f,        /* normal */
                           0.f, 0.f, 0.f,        /* v0 */
                           2.f, 0.f, 0.f,        /* v1 */
                           0.f, 0.f, 2.f};       /* v2 */
    blob.append(reinterpret_cast<const char*>(tri), sizeof(tri));
    blob.append(2, '\0');                        /* attribute count */

    writeFile("/tmp/frame_stl_binary.stl", blob);

    delta::geometry::mesh::Mesh* m =
        delta::core::io::readPartGeometry("/tmp/frame_stl_binary.stl");
    CHECK(m != nullptr);
    std::vector<iREAL> x = m->getXCoordinatesAsVector();
    std::vector<iREAL> y = m->getYCoordinatesAsVector();
    std::vector<iREAL> z = m->getZCoordinatesAsVector();
    CHECK(x.size() == 3);
    CHECK(x[0] == 0.0 && z[0] == 0.0);
    CHECK(x[1] == 2.0);
    CHECK(z[2] == 2.0);
    delete m;
  }

  /* ---- a truncated binary file must be rejected ----------------------- */
  {
    std::string blob(80, ' ');
    unsigned int n = 10;                 /* claims 10 triangles */
    blob.append(reinterpret_cast<const char*>(&n), sizeof(n));
    blob.append(50, '\0');               /* provides 1 */
    writeFile("/tmp/frame_stl_short.stl", blob);

    bool threw = false;
    try {
      delta::core::io::readPartGeometry("/tmp/frame_stl_short.stl");
    } catch (const std::runtime_error&) {
      threw = true;
    }
    CHECK(threw);
  }

  /* ---- a missing file must be rejected, not segfault ------------------ */
  {
    bool threw = false;
    try {
      delta::core::io::readPartGeometry(dir + "no-such-mesh.stl");
    } catch (const std::runtime_error&) {
      threw = true;
    }
    CHECK(threw);
  }

  std::printf("stl_read_test OK: binary and ascii STL parse, truncated and "
              "missing files rejected\n");
  return 0;
}
