// This file originally was created by pdt (Peano Development Toolkit) as part
// of a code based upon the Peano project by Tobias Weinzierl. For conditions 
// of distribution and use of this project, please see the copyright notice at
// www.peano-framework.org. Feel free to adopt the license and authorship of 
// this file and your project to your needs as long as the license is in 
// agreement with the original Peano user constraints. A reference to/citation  
// of  Peano and its author is highly appreciated.
#ifndef MULTIGRID_MAPPINGS_AdditiveMG_H_
#define MULTIGRID_MAPPINGS_AdditiveMG_H_


#include "tarch/logging/Log.h"
#include "tarch/la/Vector.h"

#include "peano/grid/VertexEnumerator.h"
#include "peano/MappingSpecification.h"
#include "peano/CommunicationSpecification.h"

#include "tarch/multicore/MulticoreDefinitions.h"

#include "multigrid/Vertex.h"
#include "multigrid/Cell.h"
#include "multigrid/State.h"

#include "matrixfree/solver/Multigrid.h"


namespace multigrid {
  namespace mappings {
    class AdditiveMG;
  }
}


/**
 * This is a mapping from the spacetree traversal events to your user-defined activities.
 * The latter are realised within the mappings. 
 * 
 * @author Peano Development Toolkit (PDT) by  Tobias Weinzierl
 * @version $Revision: 1.10 $
 */
class multigrid::mappings::AdditiveMG {
  private:
    /**
     * Logging device for the trace macros.
     */
    static tarch::logging::Log  _log;

    State                          _state;
    matrixfree::solver::Multigrid  _multigrid;
  public:
    /**
     * These flags are used to inform Peano about your operation. It tells the 
     * framework whether the operation is empty, whether it works only on the 
     * spacetree leaves, whether the operation can restart if the thread 
     * crashes (resiliency), and so forth. This information allows Peano to
     * optimise the code.
     *
     * @see peano::MappingSpecification for information on thread safety.
     */
    static peano::MappingSpecification   touchVertexLastTimeSpecification();
    static peano::MappingSpecification   touchVertexFirstTimeSpecification();
    static peano::MappingSpecification   enterCellSpecification();
    static peano::MappingSpecification   leaveCellSpecification();
    static peano::MappingSpecification   ascendSpecification();
    static peano::MappingSpecification   descendSpecification();

    static peano::CommunicationSpecification   communicationSpecification();


    /**
     * Mapping constructor.
     *
     * Mappings have to have a standard constructor and, typically, no other 
     * constructor does exist. While the constructor may initialise a mapping, 
     * Peano's concept requires the mapping to be semi-stateless:
     *
     * - At construction time the mapping has no well-defined state, i.e. the 
     *   values set by the constructor are meaningless.
     * - Whenever the mapping's beginIteration() operation is called, the 
     *   mapping has to initialise itself. To do this, it has to analyse the 
     *   passed state object. The beginIteration() operation may set attributes 
     *   of the mapping and these attributes now have a valid state.
     * - All the subsequent calls on the mapping can rely on valid mapping 
     *   attributes until
     * - The operation endIteration() is invoked. Afterwards, all the mapping's 
     *   attributes have an undefined state.
     *
     * With this concept, you cannot ensure a consistent mapping state 
     * in-between two iterations: While the first iteration might set some 
     * mapping attributes, the attributes become invalid after the first 
     * endIteration() call and might be changed from outside before the next 
     * beginIteration() is invoked.
     *
     * To implement persistent attributes, you have to write back all these  
     * attributes at endIteration() and reload them at the next beginIteration() 
     * call. With this sometimes confusing persistency concept, we can ensure 
     * that your code works on a parallel machine and for any mapping/algorithm 
     * modification.
     */
    AdditiveMG();

    #if defined(SharedMemoryParallelisation)
    /**
     * Copy constructor for multithreaded code
     *
     * If Peano uses several cores, the mappings are duplicated due to this 
     * copy constructor.
     *
     * This operation is thread-safe, i.e. you need no semaphores here. 
     *
     * @see mergeWithWorkerThread()
     */
    AdditiveMG(const AdditiveMG& masterThread);
    #endif

    /**
     * Destructor. Typically does not implement any operation.
     */
    virtual ~AdditiveMG();
  
    #if defined(SharedMemoryParallelisation)
    /**
     * Merge with worker thread
     *
     * In Peano's multithreaded mode, each mapping is duplicated among the 
     * threads. This duplication is done via the copy constructor. At the end 
     * of a parallel section, the individual copies of the mappings are 
     * merged together into one the global state instance again. 
     * 
     * If there are t threads, Peano takes the original mapping and makes t 
     * copies. Then, the t copies are ran in parallel. In the end, the t copies 
     * are merged into the original step by step. If your mapping holds 
     * variables, also these variables are copied per thread. If you have 
     * pointers, be sure that those are copied in the constructor as well or 
     * handled correctly. This place then is the place to merge all the copies 
     * back into one mapping instance.
     *
     * This operation is thread-safe, i.e. you need no semaphores here.
     *
     * While this is the right place to merge parallel data, and while you 
     * do not need any semaphores at all here, there are also cases where this 
     * implementation pattern doesn't work. The best example is a plotter: 
     * You may not copy a plotter, and it does not make sense to merge a plotter. 
     * So, in this case, I recommend to hold a pointer to the plotter in the 
     * original mapping and to create the plotter on the heap. This pointer then 
     * is copied to all thread-local mappings and all threads use the same object
     * on the heap. However, you should protect this object by a BooleanSemaphore 
     * and a lock to serialise all accesses to the plotter.    
     */   
    void mergeWithWorkerThread(const AdditiveMG& workerThread);
    #endif

    /**
     * Create an inner vertex.
     *
     * You receive a vertex, its position, and the size of its @f$ 2^d @f$ 
     * surrounding cells. The grid already has found out that this will be an 
     * inner vertex, however, you have to set valid vertices.
     *
     * The vertices have an attribute position or x, too. However, this 
     * attribute is available in Debug mode only and it should be used solely 
     * to implement assertions. To work with the vertex's position, use the 
     * attribute x instead.
     *
     * @image html peano/grid/geometry-vertex-inside-outside.png
     *
     * If you are working with moving geometries, an inner vertex is either 
     * created at startup time or it is a former boundary vertex becoming an 
     * inner vertex now. To recognise the latter case is up to you, as every
     * PDE reacts differently.
     *
     * It might seem to make sense to add an assertion such as 
     * \code 
 assertion1( isInside() || isBoundary(), *this );
 \endcode
     * at the very beginning of your implementation. However, such an assertion 
     * fails as Peano switches the vertex type after the initialisation routine. 
     * The reason for this behaviour is simple: For moving boundaries, often 
     * you'd like to know what type the vertex had before (was it outside or 
     * was it an inner point becoming a boundary/boundary point becoming an 
     * inner point). This is possible, if the type is switched after the 
     * initialisation.
     *
     * The refinement process cuts a coarse grid cell into pieces. You have 
     * access to this coarse grid data via coarseGridCell (coarse grid's cell
     * data), coarseGridVertices, and the corresponding 
     * coarseGridVerticesEnumerator. The coarse grid cell always is cut into 
     * @f$ 3^d @f$ subcubes, i.e. two cuts along each coordinate axis. The 
     * integer vector fineGridPositionOfVertex (each entry is in-between zero
     * and 4) tells you where within this local fine grid the new vertex is 
     * located. You may modify both the fine grid vertex and the coarse grid
     * vertices and cells. However, only the initialisation of fineGridVertex
     * is mandatory.
     *
     * !!! Vertex and cell lifecycle
     * 
     * Please consult peano/grid/vertex-lifecycle.doxys for details on the 
     * vertex and cell lifecycle.
     *
     * !!! Addendum to fine grid vertex position and vertex event order
     *
     * One fundamental principle of Peano is that you don't know exact event 
     * orders. It is up to Peano to decide in which order it triggers the 
     * operations of your mappings as long as certain partial orders are 
     * preserved (enterCell() after touchVertexFirstTime(), e.g.). For a 
     * discussion of the partial orders, please confer the page vertex 
     * lifecycle of the peano::grid component.
     *
     * If a patch is created, you consequently don't know in which order the 
     * vertices are created. Neither do you know later in which order touch 
     * first or last is called. There are many reasons for this besides the 
     * fact that there is no need to expose traversal orders. The most 
     * important reason is that the freedom for the Peano kernel allows the 
     * latter to reorder events as long as logical constraints represented by 
     * the partial order are preserved. Reordering might yield a speedup. More 
     * important is that the kernel even might decide to trigger events in 
     * parallel in shared memory environments. The parallel execution however 
     * is disabled for creational events and works only for touch first/last.
     *
     * We know that all vertices adjacent to a cell are created 
     * before the cell itself is created. We also know that all vertices and 
     * cells in the spacetree that are coarser than the current vertex have 
     * been created before. Again, there are nondeterministic effects: Study
     * a @f$ 6\times 3 @f$ grid in 2d, i.e. a grid with 18 fine grid cells 
     * and two coarse cells. We refer to the coarse grid as left and right 
     * cell. When the vertex (3,1) in the patch is created (we start to count 
     * with zero), you do not know a priori whether the corresponding coarse 
     * cell is the left one or the right one.
     *
     * However, Peano passes the integer array fineGridPositionOfVertex to 
     * the events (same holds for touch first/last and destroy). That one 
     * tells you within the current patch the position of the vertex. If this 
     * argument in the previous example is (3,1), then the code currently is 
     * descending in the left coarse grid cell. If the argument is (0,1) it 
     * has been descending in the right. You can access all @f$ 2^d @f$ coarse 
     * grid vertices and you can be sure that they have been initialised 
     * before. With fineGridPositionOfVertex you can interpolate data from 
     * the coarse grid as you know where the vertex is located relative to the 
     * coarse grid vertices.
     *
     * @see peano::MappingSpecification for information on thread safety.
     *
     * @param fineGridVertex  Vertex that is to be initialised. This is the 
     *                        central object you should set values to.
     * @param fineGridX       Position of vertex.
     * @param fineGridH       Size of the surrounding cells of fineGridVertex.
     * @param coarseGridVertices Vertices of the coarser grid. These vertices 
     *                        have to be accessed using the enumerator.
     * @param coarseGridVerticesEnumerator Enumerator for coarseGridVertices. 
     *                        It also holds the coarse grid's cell size.
     * @param coarseGridCell  This it the cell of the coarser grid into which 
     *                        the new vertex is embedded.
     * @param fineGridPositionOfVertex  Position of new vertex within the 
     *                        fine grid.
     */
    void createInnerVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
    );


    /**
     * Create a boundary vertex.
     *
     * You receive a vertex, its position, and the size of its @f$ 2^d @f$ 
     * surrounding cells. The grid already has found out that this will be a 
     * boundary vertex, however, you have to set valid vertices. Boundary 
     * implies that this vertex has less than @f$ 2^d @f$ cells that lay 
     * completely with in the computational domain.
     *
     * @image html peano/grid/geometry-vertex-inside-outside.png
     *
     * The vertices have an attribute position or x, too. However, this 
     * attribute is available in Debug mode only and it should be used solely 
     * to implement assertions. To work with the vertex's position, use the 
     * attribute x instead. 
     *
     * A boundary vertex is 
     * - either created at startup time,
     * - a former outer vertex, or
     * - a former inner vertex.
     *      
     * The latter two cases happen if and only if you are working with moving 
     * geometries. To recognise the latter case is up to you, as every
     * PDE reacts differently, i.e. there's no boundary flag for the vertices 
     * a priori.
     *
     * It might seem to make sense to add an assertion such as 
     * \code 
 assertion1( isInside() || isBoundary(), *this );
 \endcode
     * at the very beginning of your implementation. However, such an assertion 
     * fails as Peano switches the vertex type after the initialisation routine. 
     * The reason for this behaviour is simple: For moving boundaries, often 
     * you'd like to know what type the vertex had before (was it outside or 
     * was it an inner point becoming a boundary/boundary point becoming an 
     * inner point). This is possible, if the type is switched after the 
     * initialisation.     
     *
     * The refinement process cuts a coarse grid cell into pieces. You have 
     * access to this coarse grid data via coarseGridCell (coarse grid's cell
     * data), coarseGridVertices, and the corresponding 
     * coarseGridVerticesEnumerator. The coarse grid cell always is cut into 
     * @f$ 3^d @f$ subcubes, i.e. two cuts along each coordinate axis. The 
     * integer vector fineGridPositionOfVertex (each entry is in-between zero
     * and 4) tells you where within this local fine grid the new vertex is 
     * located. You may modify both the fine grid vertex and the coarse grid
     * vertices and cells. However, only the initialisation of fineGridVertex
     * is mandatory.
     *
     * !!! Vertex and cell lifecycle
     * 
     * Please consult peano/grid/vertex-lifecycle.doxys for details on the 
     * vertex and cell lifecycle.
     *
     * !!! Boundary points on the unit square
     *
     * If your computational domain is the unit square coinciding with the 
     * Peano root, then the @f$ 2^d @f$ vertices of the coarsest level are 
     * boundary vertices. As a consequence, the first adapter specified in 
     * the specification is asked to create this boundary vertex. Usually 
     * this is totally irrelevant, as there will be finer grids anyway. 
     * However, you have to call refine(). As a consequence, I recommend to 
     * add a statement such as
     * \code
       if ( tarch::la::volume(fineGridH)>0.5) {
         fineGridVertex.refine();
       }
     \endcode
     * to the event in any case.
     *
     * @see peano::MappingSpecification for information on thread safety.
     * 
     * @param fineGridVertex  Vertex that is to be initialised. This is the 
     *                        central object you should set values to.
     * @param fineGridX       Position of vertex.
     * @param fineGridH       Size of the surrounding cells of fineGridVertex.
     * @param coarseGridVertices Vertices of the coarser grid. These vertices 
     *                        have to be accessed using the enumerator.
     * @param coarseGridVerticesEnumerator Enumerator for coarseGridVertices. 
     *                        It also holds the coarse grid's cell size.
     * @param coarseGridCell  This it the cell of the coarser grid into which 
     *                        the new vertex is embedded.
     * @param fineGridPositionOfVertex  Position of new vertex within the 
     *                        fine grid.
     */
    void createBoundaryVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
    );


    /**
     * Create Hanging Vertex
     *
     * Peano treats hanging vertices as not persistent, i.e. they are not kept
     * in-between two iterations. Even more, it might happen that a hanging 
     * vertex is created and destroyed several times throughout an iteration. 
     * Each hanging vertex create call is accompanied by a destroy call. If a 
     * cell has hanging vertices, you can be sure that all adjacent vertices 
     * have been initialised before, i.e. for each adjacent vertex either 
     * touchVertexFirstTime() or createHangingVertex() has been called. 
     *
     * !!! Vertex and cell lifecycle
     * 
     * Please consult peano/grid/vertex-lifecycle.doxys for details on the 
     * vertex and cell lifecycle.
     *
     * !!! Hanging nodes outside the computational domain
     *
     * For performance reasons, Peano does not check whether a hanging node 
     * is outside of the computational domain or (in the parallel case) does 
     * not belong to the current rank's subdomain: Peano calls the create 
     * method for each hanging node independent of its state. If you have to 
     * filter out outer and remote hanging vertices, you have to do this 
     * manually.
     *
     * To check the geometric property, you have to add the corresponding 
     * if checks in your mapping. If you run your code with mpi, you have to 
     * wrap your code into 
     * \code
 if ( fineGridVertex.isAdjacentToDomainOf( tarch::parallel::Node::getInstance().getRank() ) ) {
   ...
     \endcode  
     *  
     * @see peano::MappingSpecification for information on thread safety.
     */
    void createHangingVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
    );


    /**
     * Counterpart of create operation
     *
     * !!! Thread-safety
     *
     * This operation is not thread safe with respect to the coarse grid 
     * vertices and the coarse grid cell. With respect to the fine grid 
     * vertices, it is thread safe.
     *
     * !!! Vertex and cell lifecycle
     * 
     * Please consult peano/grid/vertex-lifecycle.doxys for details on the 
     * vertex and cell lifecycle.
     */
    void destroyHangingVertex(
      const multigrid::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
    );


    /**
     * Destroy a vertex.
     *
     * For the regular grid, a vertex typically is destroyed due to moving
     * boundaries. The operation does not distinguish between boundary and
     * inner vertices, i.e. if you wanna treat them differently, you have to
     * implement this manually. The destory operation also is called after a 
     * vertex has been moved to a different node due to dynamic load balancing.
     *
     * !!! Thread-safety
     *
     * This operation is not thread safe with respect to the coarse grid 
     * vertices and the coarse grid cell. With respect to the fine grid 
     * vertices, it is thread safe.
     * 
     * !!! Vertex and cell lifecycle
     * 
     * Please consult peano/grid/vertex-lifecycle.doxys for details on the 
     * vertex and cell lifecycle.
     *
     * @see createBoundaryVertex() for a argument description.
     */
    void destroyVertex(
      const multigrid::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
    );


    /**
     * Create an inner cell.
     *
     * Whenever the grid management has created a new cell (either at startup or
     * due to moving boundaries), it afterwards invokes this operation. Here you 
     * can add your PDE-specific initialisation. The grid management already has 
     * found out that this cell will be an inner cell, so you don't have to 
     * doublecheck this again. Instead, you can focuse on PDE-specific stuff.
     *
     * @image html peano/grid/geometry-cell-inside-outside.png
     *
     * The vertices surrounding the cell already are initialised, i.e. you can 
     * rely on them having a valid state. However, they are arranged in an array 
     * and you don't know how the vertices are ordered in this array. 
     * createInnerCell() just receives a pointer to this array. To access the 
     * individual elements, you have to use the vertex enumerator. This functor 
     * encapsulates the vertex enumeration and is initialised by the grid 
     * properly. See the enumerator's documentation for more details.
     * 
     * If you need the position of the vertices of the cell or its size, use the 
     * enumerator.
     *
     * !!! Thread-safety
     *
     * This operation is not thread safe with respect to the coarse grid 
     * vertices and the coarse grid cell. With respect to the fine grid 
     * vertices and fine grid cell, it is thread safe. 
     *
     * !!! Vertex and cell lifecycle
     * 
     * Please consult peano/grid/vertex-lifecycle.doxys for details on the 
     * vertex and cell lifecycle.
     * 
     * @param fineGridCell      Fine grid cell that you should fill with data.
     * @param fineGridVertices  Adjacent vertices of fineGridCell. To access 
     *                          elements of this array, use the enumerator.
     * @param fineGridVerticesEnumerator Enumerator for fineGridVertices.
     * @param coarseGridVertices Vertices of the next coarser level. Use the 
     *                          enumerator to access them.
     * @param coarseGridVerticesEnumerator Enumerator for coarseGridVertices.
     * @param coarseGridCell    Cell of the next coarser grid, i.e. the parent
     *                          cell within the spacetree.
     * @param fineGridPositionOfCell Position of fineGridCell within the parent 
     *                          cell. This is a d-dimensional vector where each 
     *                          entry is either 0,1, or 2.
     */
    void createCell(
      multigrid::Cell&                 fineGridCell,
      multigrid::Vertex * const         fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
    );


    /**
     * Destroy a cell
     *
     * This operation is called whenever the grid decides to destroy a cell due 
     * to moving boundaries or a load rebalancing. Here, the grid does not 
     * distinguish between inner and boundary cells, i.e. if you want to react 
     * differently, you have to implement this manually.
     * 
     * Remarks:  
     * - If you need the position of the vertices of the cell or its size, use the 
     *   enumerator.
     * - If the destory is invoked due to load balancing, it is called after the 
     *   cell has been sent to another node. You can identify this case by asking
     *   cell.isRemote(). 
     *
     * !!! Thread-safety
     *
     * This operation is not thread safe with respect to the coarse grid 
     * vertices and the coarse grid cell. With respect to the fine grid 
     * vertices and fine grid cell, it is thread safe. 
     *
     * !!! Vertex and cell lifecycle
     * 
     * Please consult peano/grid/vertex-lifecycle.doxys for details on the 
     * vertex and cell lifecycle.
     * 
     * @param fineGridCell      Fine grid cell that you should fill with data.
     * @param fineGridVertices  Adjacent vertices of fineGridCell. To access 
     *                          elements of this array, use the enumerator.
     * @param fineGridVerticesEnumerator Enumerator for fineGridVertices.
     * @param coarseGridVertices Vertices of the next coarser level. Use the 
     *                          enumerator to access them.
     * @param coarseGridVerticesEnumerator Enumerator for coarseGridVertices.
     * @param coarseGridCell    Cell of the next coarser grid, i.e. the parent
     *                          cell within the spacetree.
     * @param fineGridPositionOfCell Position of fineGridCell within the parent 
     *                          cell. This is a d-dimensional vector where each 
     *                          entry is either 0,1, or 2.
     */
    void destroyCell(
      const multigrid::Cell&           fineGridCell,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
    );
        
        
    #ifdef Parallel
    /**
     * Merge vertex with the incoming vertex from a neighbouring computation node.
     * 
     * When Peano is running in parallel the data exchange is done vertex-wise 
     * between two grid iterations. Thus, before the touchVertexFirstTime-event
     * the vertex, sent by the computation node, which shares this vertex, is 
     * merged with the local copy of this vertex.
     *
     * !!! Heap data
     *
     * If you are working with a heap data structure, your vertices or cells, 
     * respectively, hold pointers to the heap. The received records hold 
     * pointer indices as well. However, these pointers are copies from the 
     * remote ranks, i.e. the pointers are invalid though seem to be set.
     * Receive heap data instead separately without taking the pointers in 
     * the arguments into account. 
     *
     * @param vertex    Local copy of the vertex.
     * @param neighbour Remote copy of the vertex.
     * @param fromRank  See prepareSendToNeighbour()
     * @param isForkOrJoin See preareSendToNeighbour()
     */
    void mergeWithNeighbour(
      multigrid::Vertex&  vertex,
      const multigrid::Vertex&  neighbour,
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
    );


    /**
     * Prepare a vertex that is sent to the neighbour
     * 
     * When Peano is running in parallel the data exchange is done vertex-wise 
     * between two grid iterations. Thus, when the touchVertexLastTime event
     * has been called, the current vertex can be prepared for being sent to
     * the neighbouring computation node in this method. 
     *
     * @param vertex        Local vertex.
     * @param toRank        Rank of the neighbour if isForkOrJoin is unset.  
     *                      Otherwise, it is the rank of the master.
     */
    void prepareSendToNeighbour(
      multigrid::Vertex&  vertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
    );


    /**
     * Move data to neighbour
     *
     * Throughout the joins or forks of subdomains, data has to be moved 
     * between the nodes. This operation allows the user to plug into these 
     * movements. Different to the neighbour communciation, a move always 
     * implies that Peano is copying the data structure bit-wise to the 
     * remote node. However, if you have heap data and pointers, e.g., 
     * associated to your vertices/cells you have to take care yourself that 
     * this data is moved as well.
     *
     * If data is inside your computational domain, you fork, and this data 
     * now is located at the new boundary, Peano moves the data as well, i.e.
     * the move also can be a (global) copy operation.
     *
     * @param localVertex The local vertex. This is not a copy, i.e. you may 
     *                    modify the vertex before a copy of it is sent away.  
     */
    void prepareCopyToRemoteNode(
      multigrid::Vertex&  localVertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
    );
    

    /**
     * @see other prepareCopyToRemoteNode() operation for the vertices.
     */
    void prepareCopyToRemoteNode(
      multigrid::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
      int                                           level
    );


    /**
     * Merge with remote data due to fork or join
     *
     * This operation takes remote data and merges it into the local copy, as  
     * data is moved from one rank to another, e.g. Do not use an assignment 
     * operator on the whole record, as you may overwrite only PDE-specific 
     * fields in localVertex.
     *
     * @param localVertex  Local vertex data. Some information here is already 
     *                     set: Adjacency information from the master, e.g., 
     *                     already is merged. Thus, do not overwrite 
     *                     non-PDE-specific data.
     */
    void mergeWithRemoteDataDueToForkOrJoin(
      multigrid::Vertex&  localVertex,
      const multigrid::Vertex&  masterOrWorkerVertex,
      int                                          fromRank,
      const tarch::la::Vector<DIMENSIONS,double>&  x,
      const tarch::la::Vector<DIMENSIONS,double>&  h,
      int                                          level
    );


    /**
     * Merge with remote data due to fork or join
     *
     * This operation takes remote data and merges it into the local copy, as  
     * data is moved from one rank to another, e.g. Do not use an assignment 
     * operator on the whole record, as you may overwrite only PDE-specific 
     * fields in localVertex.
     *
     * @param localCell  Local cell data. Some information here is already 
     *                   set: Adjacency information from the master, e.g., 
     *                   already is merged. Thus, do not overwrite 
     *                   non-PDE-specific data.
     */
    void mergeWithRemoteDataDueToForkOrJoin(
      multigrid::Cell&  localCell,
      const multigrid::Cell&  masterOrWorkerCell,
      int                                          fromRank,
      const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
      int                                          level
    );


    /**
     * Prepare startup send to worker
     *
     * This operation is called always when we send data to a worker. It is not 
     * called when we are right in a join or fork. The operation is kind of the 
     * replacement of enterCell() on the master, i.e. called for this one instead 
     * of.
     *
     * !!! Reduction
     *
     * With the result, you can control whether the worker shall send back its 
     * data from the master rank or not. In accordance, prepareSendToMaster() and 
     * corresponding receive are then not called if reduction is switched off.  
     * However, the result is only a recommendation: On the one hand, the 
     * results of all the active mappings is combined. If one of them requires 
     * reduction, Peano does reduce data to the master. On the other hand, Peano 
     * itself might decide that it reduces nevertheless. The latter case 
     * happens if the master decides that load balancing should be made. 
     *
     *
     * @see peano::kernel::spacetreegrid::nodes::Node::updateCellsParallelStateAfterLoad()
     * @return Whether this node needs to send back data to its master.
     */
    bool prepareSendToWorker(
      multigrid::Cell&                       fineGridCell,
      multigrid::Vertex * const              fineGridVertices,
      const peano::grid::VertexEnumerator&       fineGridVerticesEnumerator,
      multigrid::Vertex * const              coarseGridVertices,
      const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
      multigrid::Cell&                       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell,
      int                                        worker
    );
    
    
    /**
     * Merge data from the worker into the master records. This operation is 
     * called on the master, i.e. the const arguments are received copies from 
     * the worker. It is invoked only if reduction along the spacetree is 
     * switched on.
     *
     * !!! Heap data
     *
     * If you are working with a heap data structure, your vertices or cells, 
     * respectively, hold pointers to the heap. The received records hold 
     * pointer indices as well. However, these pointers are copies from the 
     * remote ranks, i.e. the pointers are invalid though seem to be set.
     * Receive heap data instead separately without taking the pointers in 
     * the arguments into account.      
     *
     * @param workerGridCell      Valid object iff data reduction is switched on. See State::reduceDataToMaster().
     * @param workerGridVertices  Valid object iff data reduction is switched on. See State::reduceDataToMaster().
     * @param workerEnumerator    Valid object iff data reduction is switched on. See State::reduceDataToMaster().
     * @param workerState         Valid object iff data reduction is switched on. See State::reduceDataToMaster().
     */
    void mergeWithMaster(
      const multigrid::Cell&                       workerGridCell,
      multigrid::Vertex * const                    workerGridVertices,
      const peano::grid::VertexEnumerator&             workerEnumerator,
      multigrid::Cell&                             fineGridCell,
      multigrid::Vertex * const                    fineGridVertices,
      const peano::grid::VertexEnumerator&             fineGridVerticesEnumerator,
      multigrid::Vertex * const                    coarseGridVertices,
      const peano::grid::VertexEnumerator&             coarseGridVerticesEnumerator,
      multigrid::Cell&                             coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&         fineGridPositionOfCell,
      int                                              worker,
      const multigrid::State&                      workerState,
      multigrid::State&                            masterState
    );


    /**
     * Prepare send to master
     *
     * Counterpart of prepareSendToWorker() that is called at the end of each 
     * iteration if data reduction is switched on. At the moment this function 
     * is called, all the data on the local worker are already streamed to the 
     * stacks, i.e. you basically receive copies of the local cell and the 
     * local vertices. If you modify them, these changes will become undone in 
     * the subsequent iteration.
     *
     * !!! Data Consistency
     *
     * Multilevel data consistency in Peano can be tricky. Most codes thus 
     * introduce a rigorous master-owns pattern. In this case, always the 
     * vertex and cell state on the master is valid, i.e. prepareSendToMaster() 
     * informs the master about state changes. In return, prepareSendToWorker() 
     * feeds the worker with new valid state of vertices and cells. 
     * Receive from master operations thus overwrite the worker's local 
     * records with the master's data, as the master always rules.  
     *
     * The coarse data is a copy from the master and may not be modified.
     *
     * !!! Special case: Fork process
     * 
     * In the very first iteration on the worker, the receive process accepting 
     * data from the master is not followed by the corresponding send back even 
     * if the reduction is switched on. Hence, you have two times receive data 
     * from master before this operation is invoked for the first time. See 
     * Node::updateCellsParallelStateBeforeStore() for details on the technical 
     * reasons for this assymetry.
     *
     * !!! Skip sends to master
     *
     * If you switch off the global reduction, i.e. if you call iterate() on 
     * the repository with the parameter false, Peano does not send any data 
     * up along the spacetree. It does not reduce the cell, state, and vertex 
     * data. See State::reduceDataToMaster(). 
     */
    void prepareSendToMaster(
      multigrid::Cell&                       localCell,
      multigrid::Vertex *                    vertices,
      const peano::grid::VertexEnumerator&       verticesEnumerator, 
      const multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
      const multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
    );


    /**
     * Counterpart of prepareSendToWorker(). This operation is called once when 
     * we receive data from the master node. You can manipulate the received 
     * records - get additional data from the heap, e.g. Afterwards, the 
     * received records are merged into the worker's local data due to 
     * mergeWithWorker(). While this operation gives you access to both the 
     * cell and its adjacent vertices in the same order as prepareSendToWorker(),
     * the mergeWithWorker() operations are called on-the-fly in a (probably) 
     * different order, as their order depends on the global space-filling curve.
     *
     * If you manipulate receivedCell or receivedVertices(), respectively, 
     * these modified values will be passed to mergeWithWorker(). The classical 
     * use case is that prepareSendToWorker() sends away some heap data. This 
     * heap data then should be received in this operation, as you have all data 
     * available here in the same order as prepareSendToWorker(). Distribute the 
     * received heap data among the @f$ 2^d +1 @f$ out arguments you have 
     * available. The modified arguments then are forwarded by Peano to 
     * mergeWithWorker().
     *
     * !!! Rationale
     * 
     * The split of the receive process into two stages seems to be artificial 
     * and not very much straightforward. However, two constraints make it 
     * necessary:
     * - Some applications need a single receive point where the data received 
     *   has the same order as prepareSendToWorker().
     * - Some applications need well-initialised vertices when they descend 
     *   somewhere in the worker tree. This descend usually not in the central 
     *   element but an outer part of the tree, i.e. some vertices have to be 
     *   merged before we can also merge the cell and all other vertices.
     *
     * !!! Heap data
     *
     * If you are working with a heap data structure, your vertices or cells, 
     * respectively, hold pointers to the heap. The received records hold 
     * pointer indices as well. However, these pointers are copies from the 
     * remote ranks, i.e. the pointers are invalid though seem to be set.
     * Receive heap data instead separately without taking the pointers in 
     * the arguments into account. 
     *
     * If you receive heap data and need it in the actual merge operation, 
     * i.e. in mergeWithWorker(), we recommend to follow the subsequent steps: 
     * 
     * - Create an entry on the heap here for each vertex.
     * - Store the received data within these heap entries.
     * - Merge the heap data within mergeWithWorker().
     *
     * - Remove the heap entries created in this operation within mergeWithWorker().
     *
     * @param coarseGridVertices  Copy of the coarse vertices of the master 
     *                            node as well worker's records. As you receive 
     *                            the copy, you can alter the local ones, but 
     *                            any change to this data will remain local on this 
     *                            worker, i.e. changes are not committed back to the 
     *                            master. Also, the changes might be lost from adapter
     *                            run to adapter run, i.e. if you switch the adapter
     *                            you wanna use, Peano might come up with different
     *                            coarse vertices and you have to reset data there 
     *                            again. 
     * @param coarseGridCell      Copy fo the coarse cell of the master.
     * @param fineGridPositionOfCell Position of receivedCell relative to coarse 
     *                            cell on master.
     */
    void receiveDataFromMaster(
      multigrid::Cell&                        receivedCell, 
      multigrid::Vertex *                     receivedVertices,
      const peano::grid::VertexEnumerator&        receivedVerticesEnumerator,
      multigrid::Vertex * const               receivedCoarseGridVertices,
      const peano::grid::VertexEnumerator&        receivedCoarseGridVerticesEnumerator,
      multigrid::Cell&                        receivedCoarseGridCell,
      multigrid::Vertex * const               workersCoarseGridVertices,
      const peano::grid::VertexEnumerator&        workersCoarseGridVerticesEnumerator,
      multigrid::Cell&                        workersCoarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&    fineGridPositionOfCell
    );


    /**
     * Counterpart of mergeWithMaster()
     * 
     * Remark: If your grid exhibits sufficient regularity beyond the domain 
     *         boundaries, the received cell might be an outside cell. 
     */
    void mergeWithWorker(
      multigrid::Cell&           localCell, 
      const multigrid::Cell&     receivedMasterCell,
      const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
      int                                          level
    );


    /**
     * Counterpart of mergeWithMaster()
     * 
     * Remark: If your grid exhibits sufficient regularity beyond the domain 
     *         boundaries, the received vertex might be an outside vertex. 
     */
    void mergeWithWorker(
      multigrid::Vertex&        localVertex,
      const multigrid::Vertex&  receivedMasterVertex,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
    );
    #endif


    /**
     * Read vertex the first time throughout one iteration
     *
     * In the Peano world, an algorithm tells the grid that the grid should be 
     * traversed. The grid then decides how to do this and runs over all cells
     * and vertices. Whenever the grid traversal reads a vertex the very first 
     * time throughout an iteration, it invokes touchVertexFirstTime() for this 
     * vertex. Then, it calls handleCell up to @f$ 2^d @f$ times for the 
     * adjacent cells and passes this vertex to these calls. Finally, the grid
     * traversal invokes touchVertexLastTime(), i.e. the counterpart of this 
     * operation.
     *
     * @image html peano/grid/geometry-vertex-inside-outside.png
     *
     * The operation is called for both each inner and boundary vertices.
     * These vertices have an attribute position, too. However, this 
     * attribute is available in Debug mode only and it should be used solely 
     * to implement assertions. To work with the vertex's position, use the 
     * attribute fineGridX instead. The latter is availble all the time (the 
     * vertex's attribute is a redundant information that is just to be used
     * for correctness and consistency checks). 
     *
     * Vertices may have persistent and non-persistent attributes (see the 
     * documentation of the DaStGen tool). Attributes that are not persistent 
     * are not stored in-between two iterations, i.e. whenever 
     * touchVertexFirstTime() is called, these attributes contain garbage. So,
     * this operation is the right place to initialise the non-persistent 
     * attributes of a vertex.
     *
     * !!! Optimisation
     * 
     * This operation is invoked if and only if the corresponding specification 
     * flag does not hold NOP. Due to this specification flag you also can define 
     * whether this operation works on the leaves only, whether it may be 
     * called in parallel by multiple threads, and whether it is fail-safe or 
     * can at least be called multiple times if a thread crashes.
     *
     * If this operation works only on leaves, the operation is sometimes not 
     * called if Peano can be sure that all adjacent cells are refined. The 
     * sometimes implies that this specification induces and optimisation - it 
     * does not enforce that the operation is not called under certain 
     * circumstances.
     *
     *
     * @see peano::MappingSpecification for information on thread safety.
     * @see createInnerVertex() for a description of the arguments. 
     */
    void touchVertexFirstTime(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
    );



    /**
     * Read vertex the last time throughout one iteration
     *
     * In the Peano world, an algorithm tells the grid that the grid should be 
     * traversed. The grid then decides how to do this and runs over all cells
     * and vertices. Whenever the grid traversal reads a vertex the very first 
     * time throughout an iteration, it invokes touchVertexFirstTime() for this 
     * vertex. Then, it calls handleCell up to @f$ 2^d @f$ times for the 
     * adjacent cells and passes this vertex to these calls. Finally, the grid
     * traversal invokes touchVertexLastTime().
     *
     * @image html peano/grid/geometry-vertex-inside-outside.png
     *
     * The operation is called for both each inner and boundary vertices.
     * These vertices have an attribute position or x, too. However, this 
     * attribute is available in Debug mode only and it should be used solely 
     * to implement assertions. To work with the vertex's position, use the 
     * attribute x instead.
     *
     * Vertices may have persistent and non-persistent attributes (see the 
     * documentation of the DaStGen tool). Attributes that are not persistent 
     * are not stored in-between two iterations, i.e. whenever 
     * touchVertexFirstTime() is called, these attributes contain garbage. So,
     * this operation is the right place to do something with the non-persistent 
     * attributes. As soon as this operation terminates, these attributes are 
     * lost.
     *
     * !!! Optimisation
     * 
     * This operation is invoked if and only if the corresponding specification 
     * flag does not holds NOP. Due to this specification flag you also can define 
     * whether this operation works on the leaves only, whether it may be 
     * called in parallel by multiple threads, and whether it is fail-safe or 
     * can at least be called multiple times if a thread crashes.
     *
     * If this operation works only on leaves, the operation is sometimes not 
     * called if Peano can be sure that all adjacent cells are refined. The 
     * sometimes implies that this specification induces and optimisation - it 
     * does not enforce that the operation is not called under certain 
     * circumstances.
     *
     * @see peano::MappingSpecification for information on thread safety.
     *
     * @see createInnerVertex() for a description of the arguments. 
     */
    void touchVertexLastTime(
      multigrid::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
    );
    

    /**
     * Enter a cell
     *
     * In the Peano world, an algorithm tells the grid that the grid should be 
     * traversed. The grid then decides how to do this and runs over all cells
     * (and vertices). For each cell, it calls handleCell(), i.e. if you want 
     * your algorithm to do somethin on a cell, you should implement this 
     * operation.
     *
     * @image html peano/grid/geometry-cell-inside-outside.png
     *
     * The operation is called for each inner and boundary element and, again, 
     * you may not access the cell's adjacent vertices directly. Instead, you 
     * have to use the enumerator. For all adjacent vertices of this cell, 
     * touchVertexFirstTime() already has been called. touchVertexLastTime() has 
     * not been called yet. 
     * More information on the interplay with ascend() and 
     * descend() can be found in peano::grid::nodes::Refined.
     * 
     * If you need the position of the vertices of the cell or its size, use the 
     * enumerator.
     *
     * !!! Optimisation
     * 
     * This operation is invoked if and only if the corresponding specification 
     * flag does not hold NOP. Due to this specification flag you also can define 
     * whether this operation works on the leaves only, whether it may be 
     * called in parallel by multiple threads, and whether it is fail-safe or 
     * can at least be called multiple times if a thread crashes.
     *     
     * @see createCell() for a description of the arguments. 
     *
     * @see peano::MappingSpecification for information on thread safety.
     */
    void enterCell(
      multigrid::Cell&                 fineGridCell,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
    );


    /**
     * This is the counterpart of enterCell(). See this operation for a 
     * description of the arguments.
     */
    void leaveCell(
      multigrid::Cell&                          fineGridCell,
      multigrid::Vertex * const                 fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      multigrid::Vertex * const                 coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&                          coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfCell
    );


    /**
     * Begin an iteration
     * 
     * This operation is called whenever the algorithm tells Peano that the grid 
     * is to be traversed, i.e. this operation is called before any creational 
     * mapping operation or touchVertexFirstTime() or handleCell() is called.
     * The operation receives a solver state that has to 
     * encode the solver's state. Take this attribute to set the mapping's 
     * attributes. This class' attributes will remain valid until endIteration()
     * is called. Afterwards they might contain garbage.
     *
     * !!! Parallelisation
     *
     * If you run your code in parallel, beginIteration() and endIteration() 
     * realise the following lifecycle together with the state object:
     *
     * - Receive the state from the master if there is a master.
     * - beginIteration()
     * - Distribute the state among the workers if there are workers.
     * - Merge the states from the workers (if they exist) into the own state. 
     * - endIteration()
     * - Send the state to the master if there is a master.
     *
     * Please note that the beginIteration() time constraint is weakened in the 
     * parallel case if you choose to receive data on the worker late. Then, 
     * beginIteration() might not be called prior to any other event. See the 
     * documentation of CommunicationSpecification for details.
     *
     * @see AdditiveMG()
     */
    void beginIteration(
      multigrid::State&  solverState
    );


    /**
     * Iteration is done
     * 
     * This operation is called at the very end, i.e. after all the handleCell() 
     * and touchVertexLastTime() operations have been invoked. In this 
     * operation, you have to write all the data you will need later on back to 
     * the state object passed. Afterwards, the attributes of your mapping 
     * object (as well as global static fields) might be overwritten.  
     *
     * !!! Parallelisation
     *
     * If you run your code in parallel, beginIteration() and endIteration() 
     * realise the following lifecycle together with the state object:
     *
     * - Receive the state from the master if there is a master.
     * - beginIteration()
     * - Distribute the state among the workers if there are workers.
     * - Merge the states from the workers (if they exist) into the own state. 
     * - endIteration()
     * - Send the state to the master if there is a master.
     *
     * Please note that the endIteration() time constraint is weakened in the 
     * parallel case if you choose to send back data eagerly. Then, endIteration()
     * might not be called after all other events. See the documentation 
     * of CommunicationSpecification for details.
     *
     * @see AdditiveMG()
     */
    void endIteration(
      multigrid::State&  solverState
    );
    
    
    /**
     * Descend in the spacetree
     *
     * This operation is invoked right after Peano has loaded all fine grid 
     * cells but before any enterCell() is called on any of the fine grid 
     * cells, i.e. here you have access to all @f$ 3^d @f$ fine grid cells 
     * en block. The adjacent vertices on the finer grid already are loaded 
     * and touchVertexFirstTime() has been called for them.
     *
     * Hence, enterCell() and descend() are somehow redundant. However, there 
     * are cases (restriction and prolongation in multigrid methods, e.g.) 
     * where only descend() is convenient. In all other cases, it is better to 
     * use enterCell, as enterCell can be multithreaded. Also, if a fine grid 
     * cell is outside but its parent is not, descend is not invoked for this 
     * pair of cells, i.e. here you have to do something in enterCell anyway.
     *
     * To access the fine grid cells, please use the enumerator as you do for
     * the vertices in any case. In particular the VertexEnumerator's cell()
     * function is of relevance. It allows you to access the individual array
     * elements of the cell pointer.
     *
     * More information on the interplay of ascend() and 
     * descend() with enterCell() and leaveCell() can be found in peano::grid::nodes::Refined.
     *
     * @see VertexEnumerator::cell()
     *
     * !!! Optimisation
     * 
     * This operation is invoked if and only if the corresponding specification 
     * flag does not hold NOP. Due to this specification flag you also can define 
     * whether this operation works on the leaves only, whether it may be 
     * called in parallel by multiple threads, and whether it is fail-safe or 
     * can at least be called multiple times if a thread crashes.
     *
     * If the operation shall be called only on leaves, Peano skips the call 
     * sometimes if all fine grid cells are refined. The sometimes indicates 
     * that such a specification is an optimisation but not a valid constraint 
     * or precondition on the input arguments. 
     *     
     * @pre coarseGridCell.isInside()
     *
     * @see peano::MappingSpecification for information on thread safety.
     */
    void descend(
      multigrid::Cell * const          fineGridCells,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell
    );


    /**
     * Ascend in the spacetree
     *
     * Counterpart of descend(). Is called as soon as all leaveCell() events 
     * have terminated.
     *
     * There is always one ascend per descend call. Usually ascend is the last 
     * operation called for a cell. However, if a cell is gonna be destroyed, 
     * destroyCell is called afterwards.
     */
    void ascend(
      multigrid::Cell * const    fineGridCells,
      multigrid::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell
    );    
};


#endif
