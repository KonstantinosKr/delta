# cvxlb
Convex primitives library (e.g. hull, gjk, intersection) in plain C.

This is an extract of geometrical tools used in [Solfec] (https://github.com/tkoziara/solfec). The code includes:

* convex polytope intersection (cvi.h, tri.h)
* convex hull calculation (hul.h)
* GJK proximity tests for polytopes and ellipsoids (gjk.h)
* simplex integration (spx.h)
* approximate triangle-sphere intersection (tsi.h)
* axis aligned bounding box overlap detection (hyb.h)
* kd-tree (kdt.h)
* rb-tree based maps and sets (map.h, set.h)
* linked list sorting (lis.h)
* memory pool (mem.h)