// This file originally was created by pdt (Peano Development Toolkit) as part
// of a code based upon the Peano project by Tobias Weinzierl. For conditions 
// of distribution and use of this project, please see the copyright notice at
// www.peano-framework.org. Feel free to adopt the license and authorship of 
// this file and your project to your needs as long as the license is in 
// agreement with the original Peano user constraints. A reference to/citation  
// of  Peano and its author is highly appreciated.
#ifndef _DEM_VERTEX_H_ 
#define _DEM_VERTEX_H_

#include "dem/records/Vertex.h"
#include "dem/records/Particle.h"

#include "peano/grid/Vertex.h"
#include "peano/grid/VertexEnumerator.h"
#include "peano/utils/Globals.h"
#include "peano/heap/DoubleHeap.h"

#include "dem/mappings/Collision.h"
#include "delta/geometry/material.h"
#include <iomanip>


namespace dem { 
  class Vertex;
      
  /**
   * Forward declaration
   */
  class VertexOperations;

  typedef peano::heap::PlainHeap< dem::records::Particle >      ParticleHeap;

  //typedef peano::heap::PlainDoubleHeapAlignment32               DEMDoubleHeap;
  typedef peano::heap::PlainDoubleHeap                          DEMDoubleHeap;
}


/**
 * Blueprint for grid vertex.
 * 
 * This file has originally been created by the PDT and may be manually extended to 
 * the needs of your application. We do not recommend to remove anything!
 */
class dem::Vertex: public peano::grid::Vertex< dem::records::Vertex > { 
  private: 
    typedef class peano::grid::Vertex< dem::records::Vertex >  Base;

    friend class VertexOperations;
    static tarch::multicore::BooleanSemaphore        _VertexSemaphore;
  public:

    /**
     * Default Constructor
     *
     * This constructor is required by the framework's data container. Do not 
     * remove it.
     */
    Vertex();
    
    /**
     * This constructor should not set any attributes. It is used by the 
     * traversal algorithm whenever it allocates an array whose elements 
     * will be overwritten later anyway.  
     */
    Vertex(const Base::DoNotCallStandardConstructor&);
    
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
    Vertex(const Base::PersistentVertex& argument);

    /**
     * Assigns the vertex an index where to store its triangle sets.
     */
    void init();
    void destroy() const;

    /**
     * Add a new triangle set for this vertex.
     *
     * @param particleId  Each particle has a unique id and we have to know to
     *                    which particle a triangle belongs to
     */
    int createParticle(
        const tarch::la::Vector<DIMENSIONS,double>& center,
  		  std::vector<double>&  xCoordinates,
  		  std::vector<double>&  yCoordinates,
  		  std::vector<double>&  zCoordinates,
  		  double epsilon, bool friction,
  		  delta::geometry::material::MaterialType material,
  		  bool isObstacle,
  		  int particleId,
  		  int localparticleId);

    int createSubParticle(
        const tarch::la::Vector<DIMENSIONS,double>& center,
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates,
        double centerOfMass[3],
        double inertia[9],
        double inverse[9],
        double mass,
        double epsilon,
        bool friction,
        delta::geometry::material::MaterialType material,
        bool isObstacle,
        int particleId,
        int localparticleId);

    int createSphereParticle(
        const tarch::la::Vector<DIMENSIONS,double>&  center,
    		double radius,
    		double epsilon,
    		bool friction,
    		delta::geometry::material::MaterialType material,
    		bool isObstacle,
    		int particleId);

    int getNumberOfParticles() const;
    int getNumberOfTriangles( int particleNumber ) const;

    records::Particle& getParticle( int particleNumber );
    const records::Particle& getParticle( int particleNumber ) const;

    int getNumberOfRealAndVirtualParticles() const;

    /**
     * Appends this particle to the vertex's particle list.
     */
    void appendParticle(const records::Particle& particle);

    /**
     * Removes the particleNumberth particle from the particle list. It does
     * however not delete it .Therefore I call this operation release instead
     * of delete. Notably, it does nothing with the triangle lists.
     */
    void releaseParticle(int particleNumber);

    double* getXCoordinates( int particleNumber );
    double* getYCoordinates( int particleNumber );
    double* getZCoordinates( int particleNumber );

    double* getXRefCoordinates( int particleNumber );
    double* getYRefCoordinates( int particleNumber );
    double* getZRefCoordinates( int particleNumber );

    DEMDoubleHeap::HeapEntries&  getXCoordinatesAsVector( int particleNumber );
    DEMDoubleHeap::HeapEntries&  getYCoordinatesAsVector( int particleNumber );
    DEMDoubleHeap::HeapEntries&  getZCoordinatesAsVector( int particleNumber );

    DEMDoubleHeap::HeapEntries&  getXRefCoordinatesAsVector( int particleNumber );
    DEMDoubleHeap::HeapEntries&  getYRefCoordinatesAsVector( int particleNumber );
    DEMDoubleHeap::HeapEntries&  getZRefCoordinatesAsVector( int particleNumber );

    const DEMDoubleHeap::HeapEntries&  getXCoordinatesAsVector( int particleNumber ) const;
    const DEMDoubleHeap::HeapEntries&  getYCoordinatesAsVector( int particleNumber ) const;
    const DEMDoubleHeap::HeapEntries&  getZCoordinatesAsVector( int particleNumber ) const;

    const DEMDoubleHeap::HeapEntries&  getXRefCoordinatesAsVector( int particleNumber ) const;
    const DEMDoubleHeap::HeapEntries&  getYRefCoordinatesAsVector( int particleNumber ) const;
    const DEMDoubleHeap::HeapEntries&  getZRefCoordinatesAsVector( int particleNumber ) const;

    /**
     * Part of the dynamic AMR.
     *
     * Is called in touchVertexFirstTime()
     */
    void clearGridRefinementAnalysisData();

    /**
     * Part of the dynamic AMR.
     *
     * Is called in touchVertexLastTime() and restricts all refinement control
     * data.
     *
     * We have a boolean veto flag per vertex that avoids that a refined vertex
     * is erased. If it is not set, any refined vertex is unrefined, i.e. we
     * realise a rather aggressive coarsening. The bool is set if it is set for
     * one of the children or if one of the children holds particles. Please
     * note that is does not matter wheather the refined vertex itself holds
     * particles. This information is of relevance only for the next coarser
     * level. erase() on a refined vertex does remove all finer vertices. If we
     * would veto an erase if a refined vertex holds particles, every vertex
     * holding any particle would yield a next finer level with grid cells even
     * though there might be no particles held in there at all.
     */
    void restrictParticleResponsibilityData(const Vertex& fineGridVertex);

    /**
     * Part of the dynamic AMR.
     *
     * Is called in touchVertexLastTime() and makes a refined vertex unrefined,
     * i.e. invokes coarse(), if no refinement veto is set. See
     * restrictParticleResponsibilityData() for a discussion on this veto.
     */
    void eraseIfParticleDistributionPermits();


    /**
     * Clear the list of particles held on any coarser level
     */
    void clearInheritedCoarseGridParticles();

    /**
     * Take the real particles stored on the coarser levels and append them to
     * the local list of virtual particles. Do the same with the coarser list
     * of virtual particles such that this link information propagates through
     * all levels.
     *
     * <h1>Optimisation</h1>
     *
     * If we have a coarse grid vertex that is not refined yet, i.e. has state
     * refining for example, we know that his vertex's particles already have
     * been compared to all the surrounding ones. It would be stupid to propagate
     * its information downwards as we would do all the stuff again (and
     * introduce contact points that we throw away later anyway). This is
     * usually not a big deal. It however hurts in the very first iteration
     * where the grid really changes dramatically. We cannot afford to have 10
     * or 100 times the original number of comparisons in this sweep.
     */
    void inheritCoarseGridParticles( const Vertex&  vertex );
};


#endif
