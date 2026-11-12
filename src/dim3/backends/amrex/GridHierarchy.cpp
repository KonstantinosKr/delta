#include "GridHierarchy.h"

#include <algorithm>
#include <cmath>
#include <set>

namespace delta {
  namespace core {
    namespace parallel {

namespace {

inline iREAL sqr(iREAL v) {return v * v;}

inline int powInt(int base, int exponent)
{
  int value = 1;
  for (int k = 0; k < exponent; ++k) {value *= base;}
  return value;
}

}  // namespace

GridHierarchy::GridHierarchy(const GridParams& params)
  : _params(params)
{
}

iREAL GridHierarchy::cellEdge(int level) const
{
  return _h0 / static_cast<iREAL>(powInt(_params.refineFactor, level));
}

int GridHierarchy::naturalLevel(iREAL diameter) const
{
  /* Deepest level whose cell still resolves the body (thesis 6.3): the edge
   * must stay at or above sizeSlack*diameter. */
  for (int level = _params.maxLevel; level >= 0; --level) {
    if (cellEdge(level) >= _params.sizeSlack * diameter) {return level;}
  }
  return _params.maxLevel;
}

GridHierarchy::Cell GridHierarchy::cellOf(const Level& level,
                                          const std::array<iREAL, 3>& pos) const
{
  Cell cell{{0, 0, 0}};
  for (int d = 0; d < 3; ++d) {
    int index = static_cast<int>(std::floor((pos[d] - _lo[d]) / level.h));
    /* Bodies outside the domain must not corrupt the bins; clamp, matching the
     * experiment tree's cellOfPoint. */
    if (index < 0) {index = 0;}
    if (index > level.n - 1) {index = level.n - 1;}
    cell[d] = index;
  }
  return cell;
}

void GridHierarchy::build(const std::vector<GridBody>& bodies,
                          const std::array<iREAL, 3>& lo,
                          const std::array<iREAL, 3>& hi,
                          iREAL maxReach)
{
  _levels.clear();
  _pairs.clear();
  _stats = GridStats();
  _lo = lo;

  iREAL extent = 0.0;
  for (int d = 0; d < 3; ++d) {extent = std::max(extent, hi[d] - lo[d]);}
  _h0 = (extent > 0.0) ? extent : 1.0;

  _stats.levels = 1;
  if (bodies.empty()) {return;}

  const int count = static_cast<int>(bodies.size());
  std::vector<int> natural(count, 0);
  int deepest = 0;
  for (int b = 0; b < count; ++b) {
    natural[b] = naturalLevel(bodies[b].diameter);
    deepest = std::max(deepest, natural[b]);
  }

  std::set<std::pair<int, int> > unique;

  for (int level = 0; level <= deepest; ++level) {
    Level current;
    current.level = level;
    current.h = cellEdge(level);
    current.n = powInt(_params.refineFactor, level);

    /* A level exists cells only where a body of this level or deeper sits.
     * Bodies of deeper levels are inherited here implicitly (nat >= level), so
     * a coarse level always contains every body that lives below it. */
    for (int b = 0; b < count; ++b) {
      if (natural[b] < level) {continue;}
      current.occupied[cellOf(current, bodies[b].pos)].push_back(b);
    }
    if (current.occupied.empty()) {break;}

    /* One stencil radius covers every body that could be within reach: bodies
     * further than reach apart cannot touch, and a body in a cell R steps away
     * is at least (R-1)*h away. */
    current.stencil = static_cast<int>(std::ceil(maxReach / current.h));
    if (current.stencil < 1) {current.stencil = 1;}
    if (current.stencil > current.n) {current.stencil = current.n;}

    for (std::map<Cell, std::vector<int> >::const_iterator it = current.occupied.begin();
         it != current.occupied.end(); ++it) {
      const Cell& home = it->first;
      for (int dx = -current.stencil; dx <= current.stencil; ++dx)
        for (int dy = -current.stencil; dy <= current.stencil; ++dy)
          for (int dz = -current.stencil; dz <= current.stencil; ++dz) {
            const Cell neighbour = {{home[0] + dx, home[1] + dy, home[2] + dz}};
            const std::map<Cell, std::vector<int> >::const_iterator found =
                current.occupied.find(neighbour);
            if (found == current.occupied.end()) {continue;}
            ++_stats.cellPairs;

            for (std::size_t a = 0; a < it->second.size(); ++a)
              for (std::size_t b = 0; b < found->second.size(); ++b) {
                const int i = it->second[a];
                const int j = found->second[b];
                if (i == j) {continue;}
                ++_stats.rawPairs;

                const GridBody& bodyA = bodies[i];
                const GridBody& bodyB = bodies[j];
                const iREAL reach = bodyA.reach + bodyB.reach;
                const iREAL distance2 = sqr(bodyA.pos[0] - bodyB.pos[0]) +
                                        sqr(bodyA.pos[1] - bodyB.pos[1]) +
                                        sqr(bodyA.pos[2] - bodyB.pos[2]);
                if (distance2 > reach * reach) {continue;}
                unique.insert(i < j ? std::make_pair(i, j) : std::make_pair(j, i));
              }
          }
    }

    _stats.levels = level + 1;
    _stats.cells += static_cast<long>(current.occupied.size());
    _levels.push_back(current);
  }

  _pairs.assign(unique.begin(), unique.end());
  _stats.uniquePairs = static_cast<long>(_pairs.size());
}

    }
  }
}
