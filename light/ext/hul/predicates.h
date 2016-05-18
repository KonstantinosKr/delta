#ifndef __DELTA_HULL_predicates__
#define __DELTA_HULL_predicates__

namespace delta {
  namespace hull {
    /* initialize predicate constants and strict double precision FPU mode */
    void exactinit();

    /* predicates themeslves */
    double orient2d (double * pa, double * pb, double * pc);
    double orient3d (double * pa, double * pb, double * pc, double * pd);
    double incircle (double * pa, double * pb, double * pc, double * pd);
    double insphere (double * pa, double * pb, double * pc, double * pd, double * pe);
  }
}

#endif
