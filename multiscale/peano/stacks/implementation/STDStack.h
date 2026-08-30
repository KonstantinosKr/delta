// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_STACKS_IMPLEMENTATION_STD_STACK_H_
#define _PEANO_STACKS_IMPLEMENTATION_STD_STACK_H_

#include <vector>
#include <iostream>
#include <sstream>


#include "tarch/logging/Log.h"
#include "tarch/Assertions.h"


namespace peano {
    namespace stacks {
      namespace implementation {
        template <class T>
        class STDStack;
      }
    }
}


/**
 * Stack based upon std::vector.
 *
 * Identifier is $std-vector$. This wrapper basically adds only the feature
 * that it keeps track of the maximum stack size ever obtained. While one
 * could think of an implementation in terms of push_back() and pop_back(), I
 * rely on an integer index representing the top of the stack. In terms of
 * runtime, this is fine, as std-stacks never shrink anyway.
 *
 * The integer allows me to use views on the stack, i.e. to extract
 * subpartitions of the stack en block.
 *
 * !!! Thread Safety
 *
 * The std stack implementation has to be thread-safe: If multiple threads decide
 * to write whole blocks to the stack, the STL implementation might decide to
 * reallocate the memory. Consequently, threads forked earlier might write to
 * invalid memory. I hence use the TBB vector variant that is thread-safe. As a
 * consequence, OpenMP is not supported for std stacks.
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.9 $
 */
template <class T>
class peano::stacks::implementation::STDStack {
  private:
    /**
     * Logging device
     */
    static tarch::logging::Log _log;

    /**
     * Wrapped type.
     */
    std::vector< T > _container;

    int _maxSize;

    int _currentElement;
  public:
    class PopBlockVertexStackView {
      private:
        /**
         * Parent is friend
         */
        friend class peano::stacks::implementation::STDStack<T>;

        int         _currentElement;

        /**
         * Should be const, but gcc had some issues with this.
         */
        int         _size;

        int         _remainingSize;

        peano::stacks::implementation::STDStack<T>* _stack;
      public:
        /**
         * The default constructor creates an empty stack view
         */
        PopBlockVertexStackView():
          _currentElement(0),
          _size(0),
          _remainingSize(0),
          _stack(0) {
        }

        /**
         * Constructor
         */
        PopBlockVertexStackView(int size, int currentElementBeforeViewIsOpened, peano::stacks::implementation::STDStack<T>* stack):
          _currentElement(currentElementBeforeViewIsOpened),
          _size(size),
          _remainingSize(size),
          _stack(stack) {
        }

        int getTotalViewSize() const {
          return _size;
        }

        int size() const {
          return _remainingSize;
        }

        bool isEmpty() const {
          return size()==0;
        }

        T pop() {
          assertion( _remainingSize>0 );
          _remainingSize--;
          _currentElement--;
          assertion( _currentElement>=0 );
          assertion( _stack!=0 );
          return _stack->_container[_currentElement];
        }

        PopBlockVertexStackView popBlockFromInputStack(int numberOfVertices) {
          PopBlockVertexStackView result(numberOfVertices, _currentElement, _stack);

          _remainingSize  -= numberOfVertices;
          _currentElement -= numberOfVertices;

          assertion( _remainingSize>=0 );
          assertion( _currentElement>=0 );

          return result;
        }

        std::string toString() const {
          std::ostringstream msg;
          msg << "(size=" << _size
              << ",currentElement=" << _currentElement
              << ",remaining-size=" << _remainingSize
              << ")";
          return msg.str();
        }
    };

    class PushBlockVertexStackView {
      private:
        /**
         * Parent is friend
         */
        friend class peano::stacks::implementation::STDStack<T>;

        int         _currentElement;

        /**
         * Should be const, but gcc had some issues with this.
         */
        int         _size;

        int         _remainingSize;

        peano::stacks::implementation::STDStack<T>*  _stack;
      public:
        /**
         * The default constructor creates an empty stack view
         */
        PushBlockVertexStackView():
          _currentElement(0),
          _size(0),
          _remainingSize(0),
          _stack(0) {
        }

        /**
         * Constructor
         */
        PushBlockVertexStackView(int size, int currentElementBeforeViewIsOpened, peano::stacks::implementation::STDStack<T>* stack):
          _currentElement(currentElementBeforeViewIsOpened),
          _size(size),
          _remainingSize(size),
          _stack(stack) {
        }

        int getTotalViewSize() const {
          return _size;
        }

        int size() const {
          return _remainingSize;
        }

        bool isOpen() const {
          return size()!=0;
        }

        void push(const T& value) {
          assertion( _remainingSize>0 );
          _remainingSize--;
          assertion( _stack!=0 );
          _stack->_container[_currentElement] = value;
          _currentElement++;
        }

        PushBlockVertexStackView pushBlockOnOutputStack(int numberOfVertices) {
          PushBlockVertexStackView result(numberOfVertices, _currentElement, _stack);

          _remainingSize  -= numberOfVertices;
          _currentElement += numberOfVertices;

          assertion3( _remainingSize>=0, numberOfVertices, _remainingSize, _currentElement );

          return result;
        }


        std::string toString() const {
          std::ostringstream msg;
          msg << "(size=" << _size
              << ",currentElement=" << _currentElement
              << ",remaining-size=" << _remainingSize
              << ")";
          return msg.str();
        }
    };

    /**
     * Default constructor.
     */
    STDStack():
      _maxSize(0),
      _currentElement(0) {
    }

    void clear() {
      _maxSize        = 0;
      _currentElement = 0;
      _container.clear();
    }

    /**
     * Default destructor.
     */
    ~STDStack() {
    }


    /**
     * Push element on stack.
     */
    void push( const T& element ) {
      if (_currentElement >= static_cast<int>(_container.size()) ) {
        assertion(_currentElement - static_cast<int>(_container.size()) <= 1 );
        _container.push_back(element);
        _maxSize = _maxSize < size() ? size(): _maxSize;
      }
      else {
        _container[_currentElement] = element;
      }
      _currentElement++;
    }


    long int getMaxSize() const {
      return _maxSize;
    }


    void clearMaxSize() {
      _maxSize = 0;
    }


    /**
     * Pop element from stack.
     */
    T pop() {
      assertion( !isEmpty() );
      _currentElement--;
      assertion( _currentElement>=0 );
      return _container[_currentElement];
    }

    /**
     * Pop element from stack.
     */
    T top() {
      assertion( !isEmpty() );
      assertion( _currentElement>=0 );
      return _container[_currentElement];
    }


    PopBlockVertexStackView  popBlockFromInputStack(int numberOfVertices) {
      PopBlockVertexStackView result(numberOfVertices, _currentElement, this);

      _currentElement-=numberOfVertices;
      assertion( _currentElement>=0 );

      return result;
    }


    /**
     * Push a block on the output stack
     *
     * Pushing a block on the output stack basically means that we move the
     * stack pointer by numberOfVertices entries. A block write stems from a
     * regular subgrid, i.e. the corresponding subgrid remains constant, but
     * it might happen that other grid parts processed before have added new
     * vertices. So, we might have to increase the stack size before we open
     * the push view on the stack. Also, the swapping of the stacks might
     * imply that the current output stack is not big enough - the input stack
     * might be, but we are using two distinguished stack data structures.
     *
     * @param numberOfVertices Size of the view
     */
    PushBlockVertexStackView  pushBlockOnOutputStack(int numberOfVertices) {
      PushBlockVertexStackView result(numberOfVertices, _currentElement, this);

      _currentElement+=numberOfVertices;

      assertion4( _currentElement < static_cast<int>(_container.size()), _currentElement, static_cast<int>(_container.size()), numberOfVertices, getMaxSize() );

      return result;
    }

    /**
     * @return Size of the stack.
     */
    int size() const {
      return _currentElement;
    }


    /**
     * @return Is the stack empty, i.e. does size equal zero.
     */
    bool isEmpty() const {
      return size()==0;
    }


    void growByAtLeastNElements(int n) {
      if ( static_cast<int>(_container.size()) < n + _currentElement) {
        _container.resize(n + _currentElement);
      }
    }

};


template <class T>
tarch::logging::Log peano::stacks::implementation::STDStack<T>::_log( "peano::stacks::implementation::STDStack" );


#endif
