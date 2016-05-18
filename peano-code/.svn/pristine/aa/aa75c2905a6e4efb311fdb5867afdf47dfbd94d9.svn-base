// This file originally was created by pdt (Peano Development Toolkit) as part
// of a code based upon the Peano project by Tobias Weinzierl. For conditions 
// of distribution and use of this project, please see the copyright notice at
// www.peano-framework.org. Feel free to adopt the license and authorship of 
// this file and your project to your needs as long as the license is in 
// agreement with the original Peano user constraints. A reference to/citation  
// of  Peano and its author is highly appreciated.
#ifndef _MYPROJECT_STATE_H_ 
#define _MYPROJECT_STATE_H_

#include "myproject/records/State.h"
#include "peano/grid/State.h"

#include "peano/grid/Checkpoint.h"

namespace myproject { 
      class State;
      /**
       * Forward declaration
       */
      class Vertex;
      /**
       * Forward declaration
       */
      class Cell;
      
      namespace repositories {
        /** 
         * Forward declaration
         */
        class RepositoryArrayStack;
        class RepositorySTDStack;
      }
}


/**
 * Blueprint for solver state.
 * 
 * This file has originally been created by the PDT and may be manually extended to 
 * the needs of your application. We do not recommend to remove anything!
 */
class myproject::State: public peano::grid::State< myproject::records::State > { 
  private: 
    typedef class peano::grid::State< myproject::records::State >  Base;

    /**
     * Needed for checkpointing.
     */
    friend class myproject::repositories::RepositoryArrayStack;
    friend class myproject::repositories::RepositorySTDStack;
  
    void writeToCheckpoint( peano::grid::Checkpoint<Vertex,Cell>&  checkpoint ) const;    
    void readFromCheckpoint( const peano::grid::Checkpoint<Vertex,Cell>&  checkpoint );    
  
  public:
    /**
     * Default Constructor
     *
     * This constructor is required by the framework's data container. Do not 
     * remove it.
     */
    State();

    /**
     * Constructor
     *
     * This constructor is required by the framework's data container. Do not 
     * remove it. It is kind of a copy constructor that converts an object which 
     * comprises solely persistent attributes into a full attribute. This very 
     * functionality is implemented within the super type, i.e. this constructor 
     * has to invoke the correponsing super type's constructor and not the super 
     * type standard constructor.
     */
    State(const Base::PersistentState& argument);

    void setTimeStepSize(double dt);
    double getTimeStepSize() const;
};


#endif
