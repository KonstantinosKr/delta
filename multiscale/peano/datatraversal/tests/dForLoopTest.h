// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef PEANO_DATATRAVERSAL_TESTS_DFORLOOPTEST_H_
#define PEANO_DATATRAVERSAL_TESTS_DFORLOOPTEST_H_

#include "tarch/tests/TestCase.h"
#include "tarch/la/Vector.h"
#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"

namespace peano {
    namespace datatraversal {
      namespace tests {
        class dForLoopTest;
      }
    }
}

namespace peano
{
    namespace datatraversal
    {
      namespace tests
      {
        class dForLoopTest : public tarch::tests::TestCase
        {
          private:
          /**
           * Logging device
           */
            static tarch::logging::Log _log;

          /**
           * This test divides a 4x4 patch of vertices into the appropriate ranges, using a grain size of 1.
           * Thus, the result should be to have 16 ranges, each holding one vertex.
           */
            void testCreateRangesVectorGrainSize1();

            /**
             * This test checks wether the reduction at the end of a parallel for is performed
             * correctly.
             */
            void testParallelReduction();

          public:
            dForLoopTest();
            virtual ~dForLoopTest();

            virtual void run();
            virtual void setUp();
        };

        class TestLoopBody {
          public:
            static int _constructorCounter;
            static int _operatorCounter;
            static int _destructorCounter;

            static tarch::multicore::BooleanSemaphore _semaphore;

            TestLoopBody();
            TestLoopBody(const TestLoopBody& copy);
            ~TestLoopBody();
            void mergeWithWorkerThread(const TestLoopBody& worker);
            void mergeIntoMasterThread(TestLoopBody& master) const;
            void operator() (const tarch::la::Vector<2,int>& i);
            void operator() (const tarch::la::Vector<3,int>& i);

            static void resetGlobalCounter();
        };
      }
    }
}

#endif
