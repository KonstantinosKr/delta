// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_COMMUNICATION_SPECIFICATION_H_
#define _PEANO_COMMUNICATION_SPECIFICATION_H_

#include <string>


namespace peano {
  struct CommunicationSpecification;
}



/**
 * Combine two specifications.
 *
 * Forwards the arguments to peano::CommunicationSpecification::combine().
 */
peano::CommunicationSpecification operator&(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs);

bool operator==(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs);
bool operator!=(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs);



/**
 * Specification of communication behaviour of a mapping
 *
 * Peano applies a clear broadcast/reduction paradigm in principle. Prior to
 * the actual traversal on any rank, the state is broadcasted, vertices and
 * cells are exchanged and then every rank runs. In return, the vertices and
 * cells from a rank are reduced when this rank has finished.
 *
 * This has a fundamental drawback: No rank can start to work before it has
 * received the master's state and its vertices. And it can send back the state
 * and the vertices not before the last touchVertexLastTime() has been called.
 * Unfortunately, each local tree is embedded into 3^d-1 other trees to preserve
 * the global Peano order on all grid entities, i.e. the worker receives data,
 * then runs through a part of this ghost layer, runs through the local partition,
 * through the remaining ghost cells, and then sends state, vertices, and cell
 * data up to the master again. Otherwise, even simple things such as the count
 * of the number of vertices would become invalid. However, sometimes it is
 * speed you need while you can live with inaccurate results. This specification
 * allows you to indicate this.
 *
 * This is important in particular if you perform several operations in a row.
 * In iterative solvers, e.g. There, global reduction typically is not required
 * in each individual step though you might require reduction in the tree to
 * couple different grid hierarchies.
 *
 * !!! Reduction
 *
 * Please note that the reduction also interplays with the specification of the
 * worker-master communication. If the reduction is switched off, the
 * specification is ignored.
 *
 * !!! Optimistic tuning
 *
 * The most aggressive optimisations work if and only if you perform multiple
 * iterations which means in turn that you switch of the load balancing.
 * Otherwise, the runner should/will complain. If lb were active, you cannot
 * delay or even skip the exchange of the state, and thus these two things
 * interfere. As such, the specification is a hint what Peano might do to
 * tune the code. Sending the state late or even skipping whole data exchange
 * then is up to Peano. It might optimise, it might also stick with the old
 * version.
 *
 * !!! Heaps, boundary data exchange and MPI communication tuning
 *
 * If you are exchanging data through the boundary via heaps, the MPI
 * communication tuning soon becomes challenging. Heaps require the user to open
 * communication windows explicitly and to close them again. This is typically
 * done by the user in beginIteration() and endIteration(). If you decide to use a
 * more 'aggressive' communication schemes such as
 * SendDataAndStateAfterProcessingOfLocalSubtree, communication windows might close
 * too early or be opened too late.
 */
struct peano::CommunicationSpecification {
  public:
    enum class ExchangeMasterWorkerData {
      /**
       * All data has to be available on the worker before it does anything.
       * Should be default. Is typically slow as the worker cannot start to
       * traverse its ghost layer until the master has accessed its actual
       * tree.
       */
      SendDataAndStateBeforeFirstTouchVertexFirstTime,
      /**
       * The state is sent to the worker before we do anything. This is
       * important if the state object encodes statistics such as whether the grid has changed or
       * coarsen calls went through. But the data, i.e. the vertices and the
       * cell, may last a little bit longer to run through the network if this
       * flag is set.
       *
       * Please note that the operation receiveDataFromMaster() on the worker
       * is called late now because it plugs into the data movement not into
       * state movement.
       */
      SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime,
      /**
       * Wait for the master's state and its data not before the code has entered
       * the local subtree. Besides masking out communication completely (cf.
       * MaskOutMasterWorkerDataAndStateExchange) this is by far the fastest
       * variant of master-worker communication as workers can immediately start
       * to traverse the grid again after one sweep has finished. However, it has
       * severe implications:
       *
       * - The right state is not available when touchVertexFirstTime() for vertices
       *   along the parallel boundary is called.
       * - The statistics held by the state (such as "have vertices refined") might
       *   be corumpted as we lose the information of these boundary vertices.
       * - The semantics beginIteration() changes: It might be called after lots
       *   of touchVertexFirstTime() events have been triggered. Please note that
       *   touchVertexFirstTime() are for MPI directly followed by mergeWithNeighbour()
       *   calls which consequently also might prelude beginIteration().
       */
      SendDataAndStateBeforeDescendIntoLocalSubtree,
      /**
       * Do not receive any data from the master.
       */
      MaskOutMasterWorkerDataAndStateExchange
    };

    enum class ExchangeWorkerMasterData {
      /**
       * Send back both data and state after everything has completed locally.
       *
       * Typically, all data of the coarsest level are sent back to the master after
       * the very last touchVertexLastTime() call. If you do not depend on this
       * touchVertexLastTime(), you may decide that the cell and the 2^d vertices on
       * the coarsest level are sent back immediately when all local cells of a rank
       * have been processed. Then, use another flag.
       */
      SendDataAndStateAfterLastTouchVertexLastTime,
      /**
       * Send data local data back as soon as possible. The state in turn is
       * sent back when all operations on the worker have terminated. This
       * flag anticipates that often the data of the vertices, cells (and maybe
       * additional heap data) have a big cardinality and hang around in the
       * network for some time. In contrast, states typically are small and can
       * be exchanged later.
       */
      SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime,
      /**
       * Send data and state up to the master as soon as the local spacetree is
       * processed. As a consequence, we call endIteration() and the
       * prepareSendToMaster() at that time as well, i.e. the traversal is not
       * yet finished when we do that. Be aware that this changes the semantics
       * of endIteration().
       */
      SendDataAndStateAfterProcessingOfLocalSubtree,
      /**
       * Do not send any data up to the master. As a consequence, we call
       * endIteration() and the prepareSendToMaster() as soon as we leave the
       * local subtree but not at the end of the traversal.
       */
      MaskOutWorkerMasterDataAndStateExchange
    };

    enum class Action {
      Early,
      Late,
      Skip
    };

    static CommunicationSpecification getMinimalSpecification(bool handleHeapInKernel=false);

    /**
     * This is the maximum specification where everyting is absolutely
     * synchronised and in order. The plotter mappings, e.g., require
     * such a specification as they have to open an output stream prior
     * to any other operation and they may not close it before everything
     * has been written.
     */
    static CommunicationSpecification getPessimisticSpecification(bool handleHeapInKernel=false);

    /**
     * !!! Kernel controls the heap
     *
     * One of the fundamental ideas of the heap is that the mappings are
     * responsible to notify when data will be exchanged. This allows
     * communication patterns that overlap multiple iterations and thus
     * are fast: you may send away data and receive it ten iterations
     * later.
     *
     * In practice, many applications cannot use this feature. They require
     * data exchanged via heap in each iteration. Also, many applications
     * prefer to have aggressive exchanges (read: multiple sweeps over the
     * grid with beginIteration() as late as possible) and thus do not fit
     * to the heap signature. As a solution, you may delegate the heap
     * management to the kernel which frees you from calling the start and
     * finished operations yourself. In turn, you have to implement a
     * Jacobi-style data exchange pattern, i.e. if you send data through the
     * heap in one iteration, you have to receive it right in the next one.
     *
     * If one mapping of an adapter sets this flag, it holds for the whole
     * adapter. Please note that you may not open any communication channel
     * manually then.
     *
     * @param exchangeMasterWorkerData  See enum description for variants
     * @param exchangeWorkerMasterData  See enum description for variants
     * @param controlHeapInKernel       See documentation above
     */
    CommunicationSpecification( ExchangeMasterWorkerData  exchangeMasterWorkerData_, ExchangeWorkerMasterData  exchangeWorkerMasterData_, bool controlHeapInKernel_ );


    Action sendStateBackToMaster() const;
    Action sendDataBackToMaster() const;

    Action receiveDataFromMaster(bool stateMayUseLazyStateAndDataReceives) const;
    Action receiveStateFromMaster(bool stateMayUseLazyStateAndDataReceives) const;

    std::string toString() const;

    bool shallKernelControlHeap() const;

    /**
     * Realisation of the & operator.
     *
     *
     * !!! Heap control
     *
     * For the heap control, we try to ensure that all mappings either deploy
     * the heap control to the kernel or do it manually. While the result is
     * a simple boolean or, we write a warning if the specs do not agree. This
     * is just a security check: it is in general not a good idea if parts of
     * the application control the heap manually and others not - that might
     * lead to conflicts.
     *
     * There's only one exception: the adapters use getMinimalSpecification()
     * and then combine all the mappings' specs with combine(). The minimal
     * spec by default does not control the heap in the kernel, so if a
     * mapping selects true for the heap control flag both specs do not
     * agree. In this case, we do not write a warning.
     */
    static peano::CommunicationSpecification combine(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs);
  private:
    friend bool ::operator==(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs);
    friend bool ::operator!=(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs);

    ExchangeMasterWorkerData  exchangeMasterWorkerData;
    ExchangeWorkerMasterData  exchangeWorkerMasterData;

    bool                      controlHeapInKernel;
};


#endif
