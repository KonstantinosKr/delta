// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_STACKS_VERTEX_ARRAY_STACK_H_
#define _PEANO_STACKS_VERTEX_ARRAY_STACK_H_


#include "peano/stacks/Stacks.h"
#include "peano/stacks/implementation/ArrayStack.h"

#include "peano/utils/Globals.h"

#include "peano/grid/Checkpoint.h"


namespace peano {
    namespace stacks {
      template <class Vertex>
      class VertexArrayStack;
    }
}


/**
 * Array Cell Stack
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.2 $
 */
template <class Vertex>
class peano::stacks::VertexArrayStack {
  private:
    typedef peano::stacks::implementation::ArrayStack< typename Vertex::PersistentVertex >  PersistentContainer;
    typedef peano::stacks::implementation::ArrayStack< Vertex >                             TemporaryContainer;

    /**
     * Number of input/output stacks.
     */
    static const int InOutStacks = 2;

    /**
     * Data structure for the two stacks.
     */
    PersistentContainer _stack[InOutStacks];

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
    VertexArrayStack<Vertex>( const VertexArrayStack<Vertex>& stack ) {}

    /**
     * One is not allowed to clone a stack.
     */
    VertexArrayStack<Vertex>& operator=( const VertexArrayStack<Vertex>& stack ) { return *this; }
  public:
    class PopBlockVertexStackView {
      private:
      public:
      PopBlockVertexStackView() {}
      PopBlockVertexStackView(int size) {}

      Vertex pop() {return Vertex();}
      PopBlockVertexStackView  popBlockFromInputStack(int numberOfVertices) {std::cout << "not implemented yet" << std::endl; exit(-3); return 0; }

      int getTotalViewSize() const {return -1;}
      int size() const {return -1;}

      bool isEmpty() const { return true; }

      std::string toString() const { return "<undef>"; }
    };

    class PushBlockVertexStackView {
      private:
      public:
      PushBlockVertexStackView() {}
      PushBlockVertexStackView(int size) {}
      int getTotalViewSize() const {return -1;}
      int size() const {return -1;}
      bool isOpen() const { return true; }

      void push(const typename Vertex::PersistentVertex& x) {}
      PushBlockVertexStackView  pushBlockOnOutputStack(int numberOfVertices) {std::cout << "not implemented yet" << std::endl; exit(-3); return 0; }

      std::string toString() const { return "<undef>"; }
    };

    /**
     * Constructor.
     *
     * @see EventStack::EventStack()
     */
    VertexArrayStack(int maxSizeOfPersistentStacks, int maxSizeOfTemporaryStacks);

    ~VertexArrayStack();

    /**
     * Pops element from input stack.
     *
     * @param stackNumber Either InOutStack or a number greater than or equal
     *                    to zero.
     */
    Vertex pop(int stackNumber);
    void push( int stackNumber, const Vertex& vertex );

    /**
     * This operation grabs numberOfVertices from the input stack en block and
     * returns a view to it. Subsequent pops do not affect this block anymore,
     * i.e. the stack is reduced immediately.
     *
     * @return Pointer to block. Your are responsible to delete this view afterwards.
     */
    PopBlockVertexStackView popBlockFromInputStack(int numberOfVertices) { std::cerr << "not implemented yet"; exit(-1); return PopBlockVertexStackView(); }
    PushBlockVertexStackView  pushBlockOnOutputStack(int numberOfVertices)  { std::cerr << "not implemented yet"; exit(-1); return PushBlockVertexStackView(); }

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


#include "peano/stacks/VertexArrayStack.cpph"

#endif
