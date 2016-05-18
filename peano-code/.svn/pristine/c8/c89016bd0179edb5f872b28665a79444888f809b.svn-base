// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_AUTOTUNING_KERNEL_GRAIN_SIZES_H_
#define _PEANO_DATA_TRAVERSAL_AUTOTUNING_KERNEL_GRAIN_SIZES_H_


namespace peano {
  namespace datatraversal {
    namespace autotuning {
      #ifndef UsePDESpecificKernelGrainSizes
      /**
       * Class holding grain sizes for all the kernel parallel section.
       * 0 means no parallelisation here.
       */
      class KernelGrainSizes {
        public:
          const static int SetCounterOnRefinedNode = 0;
          const static int ResetVertexRegularityStateInAscendForNopMappings = 64;
          const static int RunResetVertexRegularityStateInAscendForNopMappingsInParallelForIndividualLevels = 3;
      };
      #endif
    }
  }
}


#endif
