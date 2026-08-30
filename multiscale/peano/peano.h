// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org

namespace peano {
  /**
   * Fill Lookup Tables
   *
   * Fill all the lookup tables used within the application. As lookup
   * tables are used by many operations, I suggest to call this operation
   * as soon as possible.
   *
   * There shall no error occur in this operation. Thus, it does not return
   * any code.
   */
  void fillLookupTables();

  /**
   * Releases all the cached data and all temporary data from lookup tables.
   */
  void releaseCachedData();

  /**
   * Init Parallel Environment
   *
   * Inits the parallel environment. If the parallel mode is not set, the
   * operation detoriates to nop. The function returns 0 if everything
   * is o.k., it returns -2 otherwise. Please call this operation before
   * you call any other operation that could result in an error. I suggest
   * to call it right after fillLookupTables().
   *
   * init might change the variables passed. If you want to parse the
   * command line arguments, use the values returned. If you use the
   * arguments directly without calling initParallelEnvironment() they
   * might contain MPI values not important to the program.
   *
   * !!! Rationale
   *
   * You may not use the trace macros before this operation has invoked the init
   * operation. Otherwise, the getRank() assertion fails, as the node has not
   * been configured correctly.
   */
  int initParallelEnvironment(int* argc, char*** argv);

  void shutdownParallelEnvironment();

  /**
   * Init shared memory environment.
   *
   * If you work in a hybrid environment, i.e. if you use MPI and shared memory
   * parallelisation, please invoke initParallelEnvironment() before.
   *
   * If an error occurs, it returns -3.
   */
  int initSharedMemoryEnvironment();
  void shutdownSharedMemoryEnvironment();
}
