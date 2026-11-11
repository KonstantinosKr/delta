/*
 * MeshBuffer.h
 *
 * Staging container for one VTK XML unstructured grid (.vtu), deliberately
 * free of any VTK type so the code that fills it stays independent of the
 * writer.
 *
 * Points are always three-dimensional because VTK is; a plane model simply
 * writes z = 0. Dimension is therefore a property of whatever fills the
 * buffer, never of the buffer itself - there is no "2D buffer" to keep in
 * sync with a "3D buffer".
 *
 * Cell ids are absolute indices into `points`, so a body appended after
 * another must offset its ids by the running point count. Getting that wrong
 * is what collapsed every AMReX box onto the last one, so validate() checks it.
 */

#ifndef DELTA_CORE_IO_MESHBUFFER_H_
#define DELTA_CORE_IO_MESHBUFFER_H_

#include <array>
#include <string>
#include <utility>
#include <vector>

namespace delta {
namespace core {
namespace io {

struct MeshBuffer
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

/* Emit `buffer` as path + name + "_" + step + ".vtu" and rewrite
 * path + name + ".pvd" so the frames load as one time series. */
void writeVTU(const std::string& path, const std::string& name, int step,
              const MeshBuffer& buffer);

/* Rewrite the ParaView collection beside the .vtu files. Stateless: rescans
 * the directory on every call. */
void writePVD(const std::string& path, const std::string& name);

}  // namespace io
}  // namespace core
}  // namespace delta

#endif /* DELTA_CORE_IO_MESHBUFFER_H_ */
