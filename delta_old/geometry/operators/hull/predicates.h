#ifndef __DELTA_HULL_predicates__
#define __DELTA_HULL_predicates__

namespace delta {
  namespace hull {
    /* initialize predicate constants and strict iREAL precision FPU mode */
    void exactinit();

    /* predicates themeslves */
    iREAL orient2d (iREAL * pa, iREAL * pb, iREAL * pc);
    iREAL orient3d (iREAL * pa, iREAL * pb, iREAL * pc, iREAL * pd);
    iREAL incircle (iREAL * pa, iREAL * pb, iREAL * pc, iREAL * pd);
    iREAL insphere (iREAL * pa, iREAL * pb, iREAL * pc, iREAL * pd, iREAL * pe);
  }
}

#endif
