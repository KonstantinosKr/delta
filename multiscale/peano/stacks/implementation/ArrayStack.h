// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_STACKS_IMPLEMENTATION_ARRAY_STACK_H_
#define _PEANO_STACKS_IMPLEMENTATION_ARRAY_STACK_H_


#include "tarch/logging/Log.h"
#include "tarch/Assertions.h"


namespace peano {
    namespace stacks {
      namespace implementation {
        template <class T>
        class ArrayStack;
      }
    }
}


/**
 * Stack based upon a simple C array.
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.10 $
 */
template <class T>
class peano::stacks::implementation::ArrayStack {
  private:
    /**
     * Log device.
     */
    static tarch::logging::Log _log;

    /**
     * This is the actual container.
     */
    T*   _array;

    /**
     * Counter holding the number of elements, i.e. it points to the next free
     * element.
     */
    long int  _currentIndex;

    /**
     * Holds the size of the array.
     */
    long int  _arraySize;

    /**
     * Holds the maximum size actually used.
     */
    long int  _maxSize;

    void freeDataStructure() {
      if ( _array != 0 ) {
        delete[] _array;
        _array = 0;
      }
    }

  public:
    /**
     * Default constructor.
     */
    ArrayStack():
      _array(0),
      _currentIndex(0),
      _arraySize(0),
      _maxSize(0) {
    }


    /**
     * Default destructor.
     */
    ~ArrayStack() {
      freeDataStructure();
    }


    void init(int blockSize) {
      assertion( _array==0 );
      assertion( _currentIndex == 0 );
      assertion( _maxSize == 0 );
      _arraySize = blockSize;
      _array = 0;
      _array = new (std::nothrow) T[blockSize];
      if (_array==0) {
        _log.error(
          "configure(configuration)",
          "failed to alloc stack memory. Pointer returned by OS was 0"
        );
        exit(-1);
      }
      _currentIndex   = 0;
      assertion( _array != 0 );

      std::ostringstream message;
      message << "created stack based upon a C array: "
              << blockSize << " entries available, "
              << sizeof( T ) << " byte(s) per entry";
      _log.debug( "configure(StackConfiguration)", message.str() );
    }


    void clear() {
      _currentIndex = 0;
      _maxSize      = 0;
    }


    /**
     * Pushes an element onto the stack.
     */
    void push( const T& element ) {
      assertion( _array != 0 );
      assertion( _currentIndex <  _arraySize-1 );

      _array[_currentIndex]=element;
      _currentIndex++;
      _maxSize = _maxSize < _currentIndex ? _currentIndex: _maxSize;
    }


    /**
     * Takes one element from the stack.
     */
    T pop() {
      assertion( _array != 0 );
      assertion( _currentIndex > 0 );
      _currentIndex--;
      return _array[_currentIndex];
    }


    long int getMaxSize() const {
      return _maxSize;
    }


    void clearMaxSize() {
      _maxSize = 0;
    }


    /**
     * Returns the current size of the stack.
     */
    long int size() const {
      return _currentIndex;
    }


    /**
     * Is the stack empty.
     */
    bool isEmpty() const {
      return _currentIndex==0;
    }
};


template <class T>
tarch::logging::Log peano::stacks::implementation::ArrayStack<T>::_log( "peano::stacks::implementation::ArrayStack" );


#endif
