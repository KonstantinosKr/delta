// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TESTS_LOAD_VERTICES_ON_REGULAR_REFINED_PATCH_TEST_H_
#define _PEANO_GRID_TESTS_LOAD_VERTICES_ON_REGULAR_REFINED_PATCH_TEST_H_


#include "tarch/tests/TestCase.h"
#include "peano/grid/tests/records/TestVertex.h"
#include "peano/grid/Vertex.h"




namespace peano {
  namespace grid {
    namespace tests {
      class LoadVerticesOnRegularRefinedPatchTest;
    }
  }
}


class peano::grid::tests::LoadVerticesOnRegularRefinedPatchTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log _log;

    void test2D();
    void test3D();
  public:
    LoadVerticesOnRegularRefinedPatchTest();

    virtual ~LoadVerticesOnRegularRefinedPatchTest();

    virtual void run();

    void virtual setUp();
};

#endif
