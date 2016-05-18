// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_STACKS_VERTEX_STD_STACK_H_
#define _PEANO_STACKS_VERTEX_STD_STACK_H_


#include "peano/stacks/Stacks.h"
#include "peano/stacks/implementation/STDStack.h"

#include "peano/utils/Globals.h"

#include "peano/grid/Checkpoint.h"


namespace peano {
    namespace stacks {
      template <class Vertex>
      class VertexSTDStack;
    }
}


/**
 * Vertex Stack Based upon C++'s STD Stack
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.2 $
 */
template <class Vertex>
class peano::stacks::VertexSTDStack {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    typedef peano::stacks::implementation::STDStack< typename Vertex::PersistentVertex >  PersistentContainer;
    typedef peano::stacks::implementation::STDStack< Vertex >                             TemporaryContainer;

    /**
     * Number of input/output stacks.
     */
    static const int InOutStacks = 2;

    /**
     * This is the attribute holding all the temporary stacks.
     */
    TemporaryContainer _temporaryStack[NUMBER_OF_TEMPORARY_STACKS];

    /**
     * There's an input and one output stack one can flip. So they are realized
     * using an array of cardinality 2 and this integer indicates which one is
     * the input stream right now.
     */
    int _currentInputStack;

    /**
     * Input output stacks. Stack 0 and 1 are the upper stacks, i.e. the stacks
     * that are needed for the next iteration, 2 and 3 are the lower ones
     * belonging to the grid levels not processed right now.
     */
    PersistentContainer _inputOutputStack[InOutStacks];

    /**
     * One is not allowed to clone a stack.
     */
    VertexSTDStack<Vertex>( const VertexSTDStack<Vertex>& stack ) {}

    /**
     * One is not allowed to clone a stack.
     */
    VertexSTDStack<Vertex>& operator=( const VertexSTDStack<Vertex>& stack ) { return *this; }

  public:
    typedef typename PersistentContainer::PopBlockVertexStackView   PopBlockVertexStackView;
    typedef typename PersistentContainer::PushBlockVertexStackView  PushBlockVertexStackView;

    /**
     * Constructor.
     *
     * @see EventStack::EventStack()
     */
    VertexSTDStack();

    ~VertexSTDStack();

    /**
     * Pops element from a stack.
     *
     * @param stackNumber Either peano::stacks::Constants::InOutStack
     *                    or a number greater than or equal to zero.
     */
    Vertex pop(int stackNumber);

    /**
     * Pushes element to a stack.
     *
     * @param stackNumber Either peano::stacks::Constants::InOutStack
     *                    or a number greater than or equal to zero.
     */
    void push( int stackNumber, const Vertex& vertex );

    /**
     * This operation grabs numberOfVertices from the input stack en block and
     * returns a view to it. Subsequent pops do not affect this block anymore,
     * i.e. the stack is reduced immediately.
     *
     * @return Pointer to block. Your are responsible to delete this view afterwards.
     */
    PopBlockVertexStackView  popBlockFromInputStack(int numberOfVertices);

    PushBlockVertexStackView  pushBlockOnOutputStack(int numberOfVertices);

    int sizeOfInputStack() const;
    int sizeOfOutputStack() const;

    bool isInputStackEmpty() const;
    bool isOutputStackEmpty() const;

    void clear();

    /**
     * This operation flips input and output stack.
     */
    void flipInputAndOutputStack();

    /**
     * @todo
     */
    void growOutputStackByAtLeastNElements(int n);

    template <class Cell>
    void writeToCheckpoint( peano::grid::Checkpoint<Vertex,Cell>& checkpoint ) const;

    template <class Cell>
    void readFromCheckpoint( const peano::grid::Checkpoint<Vertex,Cell>& checkpoint );
};


#include "peano/stacks/VertexSTDStack.cpph"


#endif
