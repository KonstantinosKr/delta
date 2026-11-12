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

/* Unnormalised gap between surfaces: negative when overlapping. */
inline iREAL pairGap(const GridBody& a, const GridBody& b)
{
  const iREAL dx = b.pos[0] - a.pos[0];
  const iREAL dy = b.pos[1] - a.pos[1];
  const iREAL dz = b.pos[2] - a.pos[2];
  return std::sqrt(dx*dx + dy*dy + dz*dz) - 0.5 * (a.diameter + b.diameter);
}

/* A pair counts as "approaching" unless it is demonstrably moving apart. At
 * rest the dot product is exactly 0, and the strict test (v < 0) would then
 * never refine a static pile, collapsing reluctant to the mono-cell for every
 * scenario in this repo. Rest therefore counts as approaching. */
constexpr iREAL kRestEps = 1.0e-12;

inline bool nonSeparating(const GridBody& a, const GridBody& b)
{
  const iREAL dx = a.pos[0] - b.pos[0];
  const iREAL dy = a.pos[1] - b.pos[1];
  const iREAL dz = a.pos[2] - b.pos[2];
  const iREAL dvx = a.vel[0] - b.vel[0];
  const iREAL dvy = a.vel[1] - b.vel[1];
  const iREAL dvz = a.vel[2] - b.vel[2];
  return (dvx*dx + dvy*dy + dvz*dz) < kRestEps;
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
  /* Deepest level whose cell still resolves the body (thesis 6.3). */
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
    /* Bodies outside the domain must not corrupt the bins. */
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

  if (_params.mode == HierarchyMode::Reluctant) {buildReluctant(bodies, maxReach);}
  else                                    {buildAdaptive(bodies, maxReach);}
}

/* Every body sits at its natural level; a coarser body is inherited downwards
 * into the finer levels it is covered by (6.4.2 virtual particles). */
void GridHierarchy::buildAdaptive(const std::vector<GridBody>& bodies, iREAL maxReach)
{
  const int count = static_cast<int>(bodies.size());
  std::vector<int> levelOf(static_cast<std::size_t>(count), 0);
  for (int b = 0; b < count; ++b) {levelOf[b] = naturalLevel(bodies[b].diameter);}
  collectPairs(bodies, levelOf, _params.maxLevel, maxReach);
}

void GridHierarchy::buildReluctant(const std::vector<GridBody>& bodies, iREAL maxReach)
{
  const int count = static_cast<int>(bodies.size());
  std::vector<int> natural(static_cast<std::size_t>(count), 0);
  std::vector<int> levelOf(static_cast<std::size_t>(count), 0);   /* all on the mono-cell */
  for (int b = 0; b < count; ++b) {natural[b] = naturalLevel(bodies[b].diameter);}

  for (int level = 0; level < _params.maxLevel; ++level) {
    Level current;
    current.level = level;
    current.h = cellEdge(level);
    current.n = powInt(_params.refineFactor, level);

    /* Only the bodies that actually live on this level are candidates for
     * refinement; inherited ones are already handled one level up. */
    std::map<Cell, std::vector<int> > real;
    for (int b = 0; b < count; ++b) {
      if (levelOf[b] != level) {continue;}
      real[cellOf(current, bodies[b].pos)].push_back(b);
    }
    if (real.empty()) {break;}

    std::map<Cell, std::vector<int> > cellBodies = real;
    std::vector<Cell> refined;

    for (std::map<Cell, std::vector<int> >::const_iterator it = cellBodies.begin();
         it != cellBodies.end(); ++it) {
      const std::vector<int>& here = it->second;

      bool canRefine = false;
      for (std::size_t k = 0; k < here.size(); ++k) {
        if (natural[here[k]] > level) {canRefine = true; break;}
      }
      if (!canRefine || static_cast<int>(here.size()) < _params.minBodiesToRefine) {
        ++_stats.refused;
        continue;
      }

      /* Refine only when two bodies in this cell or its 26 neighbours are
       * within approachGap cells and not separating: that keeps h_min -- and
       * with it the chapter-7 step bound -- as large as possible. */
      bool refine = false;
      for (int dx = -1; dx <= 1 && !refine; ++dx)
        for (int dy = -1; dy <= 1 && !refine; ++dy)
          for (int dz = -1; dz <= 1 && !refine; ++dz) {
            const Cell neighbour = {{it->first[0] + dx, it->first[1] + dy, it->first[2] + dz}};
            const std::map<Cell, std::vector<int> >::const_iterator found =
                cellBodies.find(neighbour);
            if (found == cellBodies.end()) {continue;}
            const std::vector<int>& other = found->second;
            for (std::size_t ia = 0; ia < here.size() && !refine; ++ia)
              for (std::size_t ib = 0; ib < other.size() && !refine; ++ib) {
                const int a = here[ia];
                const int b = other[ib];
                if (a == b) {continue;}
                if (neighbour == it->first && b < a) {continue;}   /* each pair once */
                if (!nonSeparating(bodies[a], bodies[b])) {continue;}
                if (pairGap(bodies[a], bodies[b]) >= _params.approachGap * current.h) {continue;}
                refine = true;
              }
          }

      if (!refine) {++_stats.refused; continue;}
      refined.push_back(it->first);
    }

    if (refined.empty()) {break;}

    /* Drop the bodies of refined cells one level down, as far as their own size
     * allows (6.4.1); the others stay where they are. */
    for (std::size_t r = 0; r < refined.size(); ++r) {
      const std::vector<int>& here = cellBodies[refined[r]];
      for (std::size_t k = 0; k < here.size(); ++k) {
        const int b = here[k];
        if (natural[b] > level) {levelOf[b] = level + 1;}
      }
    }
  }

  collectPairs(bodies, levelOf, _params.maxLevel, maxReach);
}

/*
 * Shared pair generation. A level's body set is every body that lives on that
 * level or coarser (coarse bodies are inherited downwards), binned at the
 * level's own resolution; pairs come from the 3^3 stencil and are filtered by
 * reach, then deduplicated globally.
 *
 * Completeness: a pair whose two bodies live on different levels is found on
 * the finer of the two, because the coarser body is present there. A pair on
 * the same level is found there. Nothing is lost by not putting a fine body on
 * a coarser level, which is what makes the search local.
 */
void GridHierarchy::collectPairs(const std::vector<GridBody>& bodies,
                                 const std::vector<int>& levelOf,
                                 int topLevel,
                                 iREAL maxReach)
{
  const int count = static_cast<int>(bodies.size());
  int deepest = 0;
  for (int b = 0; b < count; ++b) {deepest = std::max(deepest, levelOf[b]);}
  if (topLevel < deepest) {deepest = topLevel;}

  std::set<std::pair<int, int> > unique;
  int created = 0;

  for (int level = 0; level <= deepest; ++level) {
    Level current;
    current.level = level;
    current.h = cellEdge(level);
    current.n = powInt(_params.refineFactor, level);

    for (int b = 0; b < count; ++b) {
      if (levelOf[b] > level) {continue;}     /* finer bodies are not inherited up */
      current.occupied[cellOf(current, bodies[b].pos)].push_back(b);
    }
    if (current.occupied.empty()) {continue;}

    current.stencil = static_cast<int>(std::ceil(maxReach / current.h));
    if (current.stencil < 1) {current.stencil = 1;}
    if (current.stencil > current.n) {current.stencil = current.n;}

    for (std::map<Cell, std::vector<int> >::const_iterator it = current.occupied.begin();
         it != current.occupied.end(); ++it) {
      for (int dx = -current.stencil; dx <= current.stencil; ++dx)
        for (int dy = -current.stencil; dy <= current.stencil; ++dy)
          for (int dz = -current.stencil; dz <= current.stencil; ++dz) {
            const Cell neighbour = {{it->first[0] + dx, it->first[1] + dy, it->first[2] + dz}};
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

    ++created;
    _stats.cells += static_cast<long>(current.occupied.size());
    _levels.push_back(current);
  }

  _stats.levels = created;
  _pairs.assign(unique.begin(), unique.end());
  _stats.uniquePairs = static_cast<long>(_pairs.size());
}

    }
  }
}
