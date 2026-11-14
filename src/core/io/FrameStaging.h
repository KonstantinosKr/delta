/*
 * FrameStaging.h
 *
 * Staging container for one VTK XML unstructured grid (.vtu), deliberately
 * free of any VTK type so the code that fills it stays independent of the
 * writer.
 *
 * Points are always three-dimensional because VTK is; a plane model simply
 * writes z = 0. Dimension is therefore a property of whatever fills the
 * staging container, never of the container itself - there is no "2D staging"
 * to keep in sync with a "3D staging".
 *
 * Cell ids are absolute indices into `points`, so a body appended after
 * another must offset its ids by the running point count. Getting that wrong
 * is what collapsed every AMReX box onto the last one, so validate() checks it.
 */

#ifndef DELTA_CORE_IO_FRAMESTAGING_H_
#define DELTA_CORE_IO_FRAMESTAGING_H_

#include <array>
#include <string>
#include <utility>
#include <vector>

namespace delta {
namespace core {
namespace io {

/*
 * The cell type tags of the .vtu format (VTK cell types, section 5 of the
 * legacy format spec). They are fixed integers, part of the file format rather
 * than of the library, so the header can name them without including VTK - that
 * is what lets a producer fill a FrameStaging on a machine with no VTK at all.
 */
enum VtuCellType
{
  VTU_VERTEX     = 1,
  VTU_POLYLINE   = 4,
  VTU_TRIANGLE   = 5,
  VTU_POLYGON    = 7,
  VTU_TETRA      = 10,
  VTU_HEXAHEDRON = 12,
  VTU_LINE       = 3
};

struct FrameStaging
{
  struct Field
  {
    std::string         name;
    int                 components;
    bool                integer;  // emitted as int rather than double
    std::vector<double> data;     // interleaved, length == n * components
  };

  std::vector<std::array<double, 3> > points;

  /* (VTK cell type, absolute point ids). The type is stored as a plain int so
   * this header stays VTK-free; writers hand it back to VTK unchanged. */
  std::vector<std::pair<int, std::vector<long long> > > cells;

  std::vector<Field> pointFields;  // one tuple per point
  std::vector<Field> cellFields;   // one tuple per cell

  std::string activeScalars;  // optional point field to colour by
  std::string activeVectors;  // optional point field to use as glyph vectors

  /* Throws std::logic_error on geometry the writer would silently mangle:
   * dangling cell ids, degenerate cells, or field lengths that do not match
   * the geometry. One pass over the cells, so writers call it always. */
  void validate() const;
};

/* Emit the staging as path + name + "_" + step + ".vtu" and rewrite
 * path + name + ".pvd" so the frames load as one time series.
 *
 * Two emitters for the same file format:
 *
 *   writeVTU       through the VTK library. Only compiled when VTK was found
 *                  (DELTA_HAVE_VTK); the reference implementation.
 *   writeVTUAscii  hand-written XML, no VTK. Always available, so a machine
 *                  without VTK can still produce frames and run the tests.
 *
 * Both validate() the staging first and call writePVD, so the two cannot drift
 * on anything except the bytes in the file - which tests/frame_staging_test.cpp
 * checks by parsing both back.
 */
void writeVTU(const std::string& path, const std::string& name, int step,
              const FrameStaging& buffer);

void writeVTUAscii(const std::string& path, const std::string& name, int step,
                   const FrameStaging& buffer);

/* True when writeVTU() is compiled in; false means only the ascii emitter
 * exists, and callers that want a file should use writeVTUAscii(). */
bool hasVtkWriter();

/* Rewrite the ParaView collection beside the .vtu files. Stateless: rescans
 * the directory on every call. */
void writePVD(const std::string& path, const std::string& name);

}  // namespace io
}  // namespace core
}  // namespace delta

#endif /* DELTA_CORE_IO_FRAMESTAGING_H_ */
