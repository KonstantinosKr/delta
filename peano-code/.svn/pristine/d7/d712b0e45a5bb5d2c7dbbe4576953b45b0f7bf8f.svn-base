// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_HEAP_HEAP_VECTOR_H_
#define _PEANO_HEAP_HEAP_VECTOR_H_


#include "tarch/compiler/CompilerSpecificSettings.h"

#ifdef CompilerICC
#include <malloc.h>
//#include <mm_malloc.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif



namespace peano {
  namespace heap {
    /**
     * STL-compliant allocator that allocates aligned memory.
     *
     * This code is inspired by a version from
     *  https://code.google.com/archive/p/mastermind-strategy/source/default/source
     *  where
     * /mastermind-strategy/tags/release-1.0.0/src/util/aligned_allocator.hpp
     *
     * subject to the MIT License. We have however removed all the Windows
     * affinities and instead tailored it to GCC vs. Intel. The heart of the
     * class, the two functions allocate and deallocate are thus completely
     * rewritten.
     *
     * @param Alignment Alignment of the allocation, e.g. 16. If you set 0,
     *                  then we use the standard C heap management
     */
    template <class T, size_t Alignment>
    struct HeapAllocator: public std::allocator<T> {
      typedef typename std::allocator<T>::size_type size_type;
      typedef typename std::allocator<T>::pointer pointer;
      typedef typename std::allocator<T>::const_pointer const_pointer;


      template <class U>
      struct rebind {
        typedef HeapAllocator<U,Alignment> other;
      };


      HeapAllocator() throw() { }


      HeapAllocator(const HeapAllocator& other) throw():
        std::allocator<T>(other) {
      }


      template <class U>
      HeapAllocator(const HeapAllocator<U,Alignment>&) throw() { }


      ~HeapAllocator() throw() { }


      pointer allocate(size_type n) {
        return allocate(n, const_pointer(0));
      }


      pointer allocate(size_type n, const_pointer /* hint */) {
        void *p;

        if (Alignment==0) {
          p = malloc(n*sizeof(T));
        }
        else {
          #ifdef CompilerICC
          p = _mm_alloc(n*sizeof(T), Alignment);
          #else
          // The arguments here are permuted compared to _mm_alloc
          p = aligned_alloc(Alignment, n*sizeof(T));
          #endif
        }

        if (!p) {
          throw std::bad_alloc();
        }
        return static_cast<pointer>(p);
      }


      void deallocate(pointer p, size_type n) {
        if (Alignment==0) {
          free(p);
        }
        else {
          #ifdef CompilerICC
          _mm_free(p);
          #else
          free(p);
          #endif
        }
      }
    };
  }
}


/**
 * Checks whether two aligned allocators are equal. Two allocators are equal
 * if the memory allocated using one allocator can be deallocated by the other.
 * @returns Always @c true.
 * @ingroup AlignedAllocator
 */
template <class T1, size_t A1, class T2, size_t A2>
bool operator == (const peano::heap::HeapAllocator<T1,A1> &, const peano::heap::HeapAllocator<T2,A2> &) {
  return true;
}


/**
 * Checks whether two aligned allocators are not equal. Two allocators are equal
 * if the memory allocated using one allocator can be deallocated by the other.
 * @returns Always @c false.
 * @ingroup AlignedAllocator
 */
template <class T1, size_t A1, class T2, size_t A2>
bool operator != (const peano::heap::HeapAllocator<T1,A1> &, const peano::heap::HeapAllocator<T2,A2> &) {
  return false;
}


#endif
