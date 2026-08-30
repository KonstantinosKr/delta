// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_STACKS_CELL_ARRAY_STACK_H_
#define _PEANO_STACKS_CELL_ARRAY_STACK_H_


#include "peano/stacks/Stacks.h"
#include "peano/stacks/implementation/ArrayStack.h"

#include "peano/grid/Checkpoint.h"


namespace peano {
  namespace stacks {
    template <class Cell>
    class CellArrayStack;
  }
}


/**
 * Array Cell Stack
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.2 $
 */
template <class Cell>
class peano::stacks::CellArrayStack {
  private:
    typedef typename peano::stacks::implementation::ArrayStack<typename Cell::PersistentCell> Container;

    /**
     * Data structure for the two stacks.
     */
    Container _stack[2];

    int _inputStackNumber;

    /**
     * One is not allowed to clone a stack.
     */
    CellArrayStack<Cell>( const CellArrayStack<Cell>& stack ) {}

    /**
     * One is not allowed to clone a stack.
     */
    CellArrayStack<Cell>& operator=( const CellArrayStack<Cell>& stack ) { return *this; }
  public:
    /**
     * Constructor.
     */
    CellArrayStack(int size);

    ~CellArrayStack();

    /**
     * Pops element from input stack.
     *
     * @param stackNumber Always InOutStack for the time being
     */
    Cell pop(int stackNumber);
    void push( int stackNumber, const Cell& cell );

    int sizeOfInputStack() const;
    int sizeOfOutputStack() const;

    bool isInputStackEmpty() const;
    bool isOutputStackEmpty() const;

    void clear();

    /**
     * This operation flips input and output stack.
     */
    void flipInputAndOutputStack();

    template <class Vertex>
    void writeToCheckpoint( peano::grid::Checkpoint<Vertex,Cell>& checkpoint ) const;

    template <class Vertex>
    void readFromCheckpoint( const peano::grid::Checkpoint<Vertex,Cell>& checkpoint );

};


#include "peano/stacks/CellArrayStack.cpph"

#endif
