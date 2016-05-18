// This file originally was created by pdt (Peano Development Toolkit) as part
// of a code based upon the Peano project by Tobias Weinzierl. For conditions 
// of distribution and use of this project, please see the copyright notice at
// www.peano-framework.org. Feel free to adopt the license and authorship of 
// this file and your project to your needs as long as the license is in 
// agreement with the original Peano user constraints. A reference to/citation  
// of  Peano and its author is highly appreciated.
#ifndef _MULTIGRID_STATE_H_ 
#define _MULTIGRID_STATE_H_

#include "multigrid/records/State.h"
#include "peano/grid/State.h"

#include "peano/grid/Checkpoint.h"

namespace multigrid { 
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
class multigrid::State: public peano::grid::State< multigrid::records::State > { 
  private: 
    typedef class peano::grid::State< multigrid::records::State >  Base;

    /**
     * Needed for checkpointing.
     */
    friend class multigrid::repositories::RepositoryArrayStack;
    friend class multigrid::repositories::RepositorySTDStack;
  
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

    void clearAccumulatedAttributes();
    void merge(const State& state);

    double getResidualIn2Norm() const;
    double getResidualInMaxNorm() const;
    double getSolutionIn2Norm() const;
    double getSolutionInMaxNorm() const;
    double getNumberOfStencilUpdates() const;

    void notifyAboutFineGridVertexUpdate( double res, double newU, const tarch::la::Vector<DIMENSIONS,double>&  h);
    void incNumberOfStencilEvaluations();

    enum MultigridPhase {
      Init,
      Smooth,
      SmoothAndRestrict,
      SmoothAndProlong
    };

    void setMultiplicativeMultigridPhase(MultigridPhase phase);

    int getActiveSmoothingLevel() const;
    int getOldActiveSmoothingLevel() const;
};


#endif
