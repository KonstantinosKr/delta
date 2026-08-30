// Copyright (C) 2009 Technische Universitaet Muenchen
// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www5.in.tum.de/peano
#ifndef _TARCH_MULTICORE_RECURSIVE_LOCK_H_
#define _TARCH_MULTICORE_RECURSIVE_LOCK_H_


namespace tarch {
  namespace multicore {
    class RecursiveSemaphore;
    class RecursiveLock;
  }
}


/**
 * Create a lock around a boolean semaphore region
 *
 * @see tarch::multicore::BooleanSemaphore
 */
class tarch::multicore::RecursiveLock {
  private:
	RecursiveSemaphore&  _semaphore;
    bool                 _lockIsAquired;
  public:
    RecursiveLock( tarch::multicore::RecursiveSemaphore& semaphore, bool aquireLockImmediately = true );
    ~RecursiveLock();

    void lock();
    void free();
};

#endif
