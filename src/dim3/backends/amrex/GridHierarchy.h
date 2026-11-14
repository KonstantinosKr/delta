/*
 * Adaptive broadphase hierarchy (thesis 6.3 / 6.4.2), ported from the
 * amrex_contact experiment tree onto delta's own types.
 *
 * What it does: pick a cell edge per level (h_L = h0 / refine^L), give every
 * body the deepest level that still resolves it (naturalLevel), and let a level
 * exist cells only where a body of that level or deeper sits. Bodies of coarser
 * levels are inherited downwards -- a coarse body occupies the fine cells it
 * overlaps -- which is the "virtual particle" of 6.4.2. Candidate pairs come
 * from the 3^3 neighbourhood of each occupied cell at every level, then a reach
 * test, then deduplicated.
 *
 * Why the level-0 grid alone is already complete: every body has nat >= 0, so
 * every body is present on level 0, and the stencil radius there is
 * ceil(maxReach / h0) >= 1. Finer levels only make the search local; they can
 * never find a pair level 0 would miss. That is what makes this safe to use as
 * a *replacement* for AMReX's neighbour list on a single rank, and why the
 * grid_modes test can demand an identical pair set.
 *
 * shortcut: single rank only. AMReX's neighbour list does the cross-rank ghost
 * exchange for the other modes; a hierarchy pair has no notion of a remote
 * body. Upgrade by asking AMReX for the multi-level pair list and keeping this
 * class only as the cell-set constructor.
 */
#ifndef DELTA_CORE_PARALLEL_GRIDHIERARCHY_H_
#define DELTA_CORE_PARALLEL_GRIDHIERARCHY_H_

#include <array>
#include <cstddef>
#include <map>
#include <utility>
#include <vector>

#include "core/delta.h"

namespace delta {
  namespace core {
    namespace parallel {

/* One body as the hierarchy sees it: where it is, how it moves and how far it
 * reaches. */
struct GridBody
{
  std::array<iREAL, 3> pos;
  std::array<iREAL, 3> vel;
  iREAL diameter;
  iREAL reach;      /* 0.5*diameter + epsilon, the pair reach contributed */
};

/* Adaptive: every body sits as deep as its size allows (6.3).
 * Reluctant: everything starts on the mono-cell and a cell is refined only when
 * it holds >= 2 bodies that are approaching (ch. 7), so h_min stays large. */
enum class HierarchyMode
{
  Adaptive,
  Reluctant
};

struct GridParams
{
  HierarchyMode mode = HierarchyMode::Adaptive;
  int refineFactor  = 3;      /* children per edge (thesis 6.1) */
  int maxLevel      = 4;      /* deepest level */
  double sizeSlack  = 1.0;    /* a level must satisfy h >= sizeSlack*diameter */
  int minBodiesToRefine = 2;  /* reluctant: bodies a cell needs to be refined */
  double approachGap    = 1.0;/* reluctant: refine when gap < approachGap*h */
};

struct GridStats
{
  int  levels        = 0;
  long cells         = 0;    /* occupied cells over all levels */
  long refused       = 0;    /* reluctant: cells left coarse on purpose */
  long cellPairs     = 0;    /* neighbour lookups performed */
  long rawPairs      = 0;    /* before dedup */
  long uniquePairs   = 0;
};

class GridHierarchy
{
public:
  explicit GridHierarchy(const GridParams& params = GridParams());

  /* Builds the levels for these bodies over the given domain. */
  void build(const std::vector<GridBody>& bodies,
             const std::array<iREAL, 3>& lo,
             const std::array<iREAL, 3>& hi,
             iREAL maxReach);

  /* Candidate pairs as indices into the body vector, ascending, unique. */
  const std::vector<std::pair<int, int> >& candidatePairs() const { return _pairs; }

  /* Level a body of this diameter belongs on. */
  int naturalLevel(iREAL diameter) const;

  const GridStats& stats() const { return _stats; }
  iREAL cellEdge(int level) const;

private:
  using Cell = std::array<int, 3>;

  struct Level
  {
    int  level = 0;
    iREAL h = 0.0;
    int  n = 1;
    int  stencil = 1;
    std::map<Cell, std::vector<int> > occupied;   /* cell -> body indices */
  };

  Cell cellOf(const Level& level, const std::array<iREAL, 3>& pos) const;

  void buildAdaptive(const std::vector<GridBody>& bodies, iREAL maxReach);
  void buildReluctant(const std::vector<GridBody>& bodies, iREAL maxReach);
  void collectPairs(const std::vector<GridBody>& bodies,
                    const std::vector<int>& levelOf,
                    int topLevel,
                    iREAL maxReach);

  GridParams _params;
  std::array<iREAL, 3> _lo{{0.0, 0.0, 0.0}};
  iREAL _h0 = 0.0;
  std::vector<Level> _levels;
  std::vector<std::pair<int, int> > _pairs;
  GridStats _stats;
};

    }
  }
}

#endif /* DELTA_CORE_PARALLEL_GRIDHIERARCHY_H_ */
