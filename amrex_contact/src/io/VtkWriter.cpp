// Minimal VTU writer. ASCII XML on purpose: no VTK dependency, the output opens
// in ParaView/VisIt as-is and diffs cleanly in git if a small frame is committed.

#include "contact/VtkWriter.hpp"

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace contact {
namespace {

void pushPoint(std::vector<Real>& pts, const Vec3& p) {
    pts.push_back(p[0]);
    pts.push_back(p[1]);
    pts.push_back(p[2]);
}

std::string frameName(const std::string& prefix, int step) {
    std::ostringstream s;
    s << prefix << '_' << std::setw(5) << std::setfill('0') << step << ".vtu";
    return s.str();
}

std::string basename(const std::string& path) {
    const std::size_t slash = path.find_last_of('/');
    return (slash == std::string::npos) ? path : path.substr(slash + 1);
}

}  // namespace

void VtuWriter::write(int step, Real time, const std::vector<Particle>& bodies,
                      const std::vector<CellViz>& cells) {
    std::vector<Real> pts;
    std::vector<long> conn;
    std::vector<long> offsets;
    std::vector<int> types;
    std::vector<int> kind;      // 0 = body triangle, 1 = grid cell
    std::vector<int> body_id;   // per cell
    std::vector<int> level;     // per cell
    std::vector<int> occupied;  // per cell (grid only)

    long npoint = 0;

    for (const Particle& b : bodies) {
        for (const Triangle& t : b.mesh) {
            const long base = npoint;
            for (int k = 0; k < 3; ++k) pushPoint(pts, b.worldVertex(t, k));
            npoint += 3;
            conn.push_back(base);
            conn.push_back(base + 1);
            conn.push_back(base + 2);
            offsets.push_back(static_cast<long>(conn.size()));
            types.push_back(7);  // VTK_POLYGON
            kind.push_back(0);
            body_id.push_back(b.id);
            level.push_back(b.level);
            occupied.push_back(0);
        }
    }

    for (const CellViz& c : cells) {
        const long base = npoint;
        const Real xs[2] = {c.lo[0], c.hi[0]};
        const Real ys[2] = {c.lo[1], c.hi[1]};
        const Real zs[2] = {c.lo[2], c.hi[2]};
        for (int k = 0; k < 2; ++k) {
            for (int j = 0; j < 2; ++j) {
                for (int i = 0; i < 2; ++i) {
                    pushPoint(pts, makeVec3(xs[i], ys[j], zs[k]));
                }
            }
        }
        npoint += 8;
        for (int v = 0; v < 8; ++v) conn.push_back(base + v);
        offsets.push_back(static_cast<long>(conn.size()));
        types.push_back(11);  // VTK_VOXEL
        kind.push_back(1);
        body_id.push_back(-1);
        level.push_back(c.level);
        occupied.push_back(c.nreal + c.nvirt);
    }

    const std::string name = frameName(prefix_, step);
    std::ofstream f(name);
    if (!f) return;

    f << "<?xml version=\"1.0\"?>\n";
    f << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    f << "  <UnstructuredGrid>\n";
    f << "    <Piece NumberOfPoints=\"" << npoint << "\" NumberOfCells=\"" << types.size()
      << "\">\n";
    f << std::setprecision(9);

    f << "      <Points>\n        <DataArray type=\"Float64\" NumberOfComponents=\"3\" "
         "format=\"ascii\">\n";
    for (std::size_t i = 0; i < pts.size(); ++i) {
        f << pts[i];
        f << ((i % 3 == 2) ? '\n' : ' ');
    }
    f << "        </DataArray>\n      </Points>\n";

    f << "      <Cells>\n";
    f << "        <DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">\n";
    for (std::size_t i = 0; i < conn.size(); ++i) {
        f << conn[i];
        f << (((i + 1) % 12 == 0) ? '\n' : ' ');
    }
    f << "\n        </DataArray>\n";
    f << "        <DataArray type=\"Int64\" Name=\"offsets\" format=\"ascii\">\n";
    for (std::size_t i = 0; i < offsets.size(); ++i) {
        f << offsets[i] << (((i + 1) % 16 == 0) ? '\n' : ' ');
    }
    f << "\n        </DataArray>\n";
    f << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
    for (std::size_t i = 0; i < types.size(); ++i) {
        f << types[i] << (((i + 1) % 32 == 0) ? '\n' : ' ');
    }
    f << "\n        </DataArray>\n      </Cells>\n";

    f << "      <CellData>\n";
    auto writeInt = [&f](const char* name, const std::vector<int>& v) {
        f << "        <DataArray type=\"Int32\" Name=\"" << name << "\" format=\"ascii\">\n";
        for (std::size_t i = 0; i < v.size(); ++i) {
            f << v[i] << (((i + 1) % 24 == 0) ? '\n' : ' ');
        }
        f << "\n        </DataArray>\n";
    };
    writeInt("kind", kind);
    writeInt("body", body_id);
    writeInt("level", level);
    writeInt("occupied", occupied);
    f << "      </CellData>\n";

    f << "    </Piece>\n  </UnstructuredGrid>\n</VTKFile>\n";

    Frame fr;
    fr.time = time;
    fr.step = step;
    fr.file = basename(name);
    frames_.push_back(fr);
}

void VtuWriter::writeCollection() const {
    std::ofstream f(prefix_ + ".pvd");
    if (!f) return;
    f << "<?xml version=\"1.0\"?>\n";
    f << "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    f << "  <Collection>\n";
    f << std::setprecision(9);
    for (const Frame& fr : frames_) {
        f << "    <DataSet timestep=\"" << fr.time << "\" group=\"\" part=\"0\" file=\""
          << fr.file << "\"/>\n";
    }
    f << "  </Collection>\n</VTKFile>\n";
}

}  // namespace contact
