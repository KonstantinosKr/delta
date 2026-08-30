// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PARALLEL_NODE_H_
#define _TARCH_PARALLEL_NODE_H_


#include "tarch/parallel/MPIConstants.h"
#include "tarch/logging/Log.h"

#include <ctime>


#ifdef Parallel
/**
 * I rely on the usleep() method within all messages. Newer versions of
 * Linux/GCC seem not to include the corresponding header automatically
 * through ctime. I thus added this new include.
 */
#include <unistd.h>
#endif


namespace tarch {
  namespace parallel {
    class Node;

    /**
     * Returns a string representation of the mpi status. For a detailed
     * description of the mpi status itself see the file mpi.h.
     */
    std::string MPIStatusToString( const MPI_Status& status );

    std::string MPIReturnValueToString( int result );
  }
}

/**
 * Parallel Node Representant
 *
 * Represents a program instance within a cluster. Thus, this class is a
 * singleton.
 *
 * The parallel concept is a client - server model (process 0 is the server),
 * where all active nodes act as servers deploying calculations on demand. So
 * the basic activities of a parallel node are
 *
 * - receive new root element to work on
 * - pass back space-tree
 * - perform additive cycle
 * - perform multiplicative cycle
 *
 * The two perform commands have to return several statistic records. Among them
 * are time needed, some residual characteristics and flags defining if the
 * domain has been refined. Furthermore the number of communication partners is
 * an interesting detail.
 *
 * In the near future, this class should become responsible for the error
 * handling. Right now, the error handle set is the fatal error handler. That
 * is the whole parallel application is shut down as soon as an mpi error
 * occurs.
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.51 $
 */
class tarch::parallel::Node {
  public:
    static const int DEADLOCK_EXIT_CODE = -2;
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    /**
     * Is set true if init() is called.
     */
    static bool _initIsCalled;

    /**
     * Rank (id) of this process.
     */
    int _rank;

    /**
     * Number of processors available.
     */
    int _numberOfProcessors;

    /**
     * MPI Communicator this process belongs to.
     */
    MPI_Comm _communicator;

    /**
     * How long shall the application wait until it writes a time-out warning
     */
    clock_t _timeOutWarning;

    clock_t _deadlockTimeOut;

    bool    _areTimeoutsEnabled;

    /**
     * The standard constructor assignes the attributes default values and
     * checks whether the program is compiled using the -DParallel option.
     * If this is not the case, a warning is logged.
     */
    Node();

    /**
     * The copy constructor is private.
     */
    Node( const Node& node );

    /**
     * Receive any Message Pending in the MPI/Receive Buffers
     *
     * We do poll MPI only every k iterations. Once we have found a message, we 
     * wait again k iterations. If the threshold is exceeded and no message is 
     * found we however do not reset k (I've tried this and it gives worse 
     * runtime): receiveDanglingMessages is typically called when we enter a 
     * critical phase of the simulation and we urgently wait for incoming MPI
     * messages. So once we are in that critical regime and have already 
     * exceeded the k it would be harakiri to reset this one to 0 again - we 
     * only do so if data drops in and we thus may assume that we'll leave the 
     * critical phase anyway.
     */
    void receiveDanglingMessagesFromReceiveBuffers();

  public:
    /**
     * Return a Free Tag
     *
     * Returns a free tag to be used for a new datatype. Each result is
     * delivered exactly once. The string argument is just for logging.
     *
     *
     * <h2> Implementation details </h2>
     *
     * This operation should write something to the log devices. However, it
     * is static and the class' log devices are static, too. C++ has no
     * mechanism to define which static entity has to be instantiated first.
     * On some systems, it hence happened that someone called this static
     * function while the static log attribute has not been initialised yet.
     * Consequently, the operation uses its own (local) log variable instead
     * of the log variable of the class. This is an important workaround!
     */
    static int reserveFreeTag(const std::string& fullQualifiedMessageName);
    static void releaseTag(int tag);

    /**
     * Logs the status of the process onto the log device.
     *
     */
    void logStatus() const;

    /**
     * This operation returns the singleton instance. Before using this
     * instance, one has to call the init() operation on the instance returned.
     *
     * @return The singleton instance
     */
    static Node& getInstance();

    /**
     * The standard destructor calls MPI_Finalize().
     */
    virtual ~Node();

    /**
     * This operation initializes the MPI environment and the program instance.
     * Not that the argv and argc parameters are both in and out parameters.
     * Before you pass them to the operation, they are set by the mpi
     * environment. Afterwards the original parameters from the user are stored
     * within them.
     *
     * !! Implementation details
     *
     * init never uses the log device to report any errors as the log device
     * usually in turn uses Node's getters. Furthermore, the _initIsCalled flag
     * has thus to be set before the log state operation is invoked.
     *
     * @return true if initialisation has been successful
     */
    bool init(int* argc, char*** argv);

    /**
     * Shuts down the application. Should be the last operation called by the
     * overall application.
     *
     * !!! Rationale
     *
     * Originally, I put the shutdown operation into the destructor of Node.
     * The MPI environment consequently was shut down as soon as the operating
     * system terminates the application. However, Scalasca complained on the
     * BlueGene/P that the
     * destruction happened after the return statement of the main method. To
     * make Peano work with Scalasca, I hence moved the MPI shutdown into a
     * method called explicitely before the final return statement.
     *
     * This seems to be a CLX effect, i.e. the Intel and the GNU compilers
     * worked fine with Scalasca. Hence, I assume that Intel and GNU
     * executables destroy all static objects (singletons) before they return
     * from the main functions. CLX destroys the static objects after the
     * return statement and thus makes Scalasca's instrumentation report an
     * error.
     */
    void shutdown();

    /**
     * Return rank of this node. In the serial version, i.e. without MPI, this
     * operation always returns 0.
     *
     * @return Rank of this node
     */
    int getRank() const;

    /**
     * Get the global master
     *
     * Peano sets up a logical tree topology on all the ranks. The root of this
     * logical tree, i.e. the rank that is reponsible for all other ranks, is
     * the global master. In contrast, every rank also has a local master that
     * tells him what to do. This is the master and the parent within the
     * topology tree. Use the NodePool to identify the rank of a rank's master.
     *
     * @return 0
     */
    static int getGlobalMasterRank();

    /**
     * @return Communicator of Peano
     */
    MPI_Comm getCommunicator() const;

    /**
     * @return Number of Nodes Available
     */
    int getNumberOfNodes() const;

    /**
     * Is this node the global master process, i.e. does its rank equal get
     * MasterProcessRank()? This operation returns always true if the code
     * is not compiled with -DParallel.
     *
     * @return Is this node the global master process?
     */
    bool isGlobalMaster() const;

    /**
     * Triggers a time out and shuts down the cluster:
     *
     * The implementation does not use MPI_Abort, since it seems that this
     * operation requires all nodes running. Instead of,
     * getDeadlockWarningTimeStamp() uses the system exit function passing it
     * DEADLOCK_EXIT_CODE as exit code.
     *
     * The operation should be called only if the deadlock time-out is switched
     * on ( isTimeOutDeadlockEnabled() ) and the deadlock time-out has expired.
     * Use getDeadlockWarningTimeStamp() and the system operation clock() to
     * check the second requirement.
     *
     * @param className  Name of the class that triggers the deadlock shutdown.
     * @param methodName Name of the method that triggers the deadlock shutdown.
     * @param communicationPartnerRank Rank of the node the operation that
     *          should have sent a message but did not.
     */
    void triggerDeadlockTimeOut(
      const std::string&  className,
      const std::string&  methodName,
      int                 communicationPartnerRank,
      int                 tag,
      int                 numberOfExpectedMessages,
      const std::string&  comment = ""
    );

    void writeTimeOutWarning(
      const std::string&  className,
      const std::string&  methodName,
      int                 communicationPartnerRank,
      int                 tag,
      int                 numberOfExpectedMessages
    );

    void plotMessageQueues();

    /**
     * Ensure that there are no messages anymore from the specified rank.
     */
    void ensureThatMessageQueuesAreEmpty( int fromRank, int tag );

    /**
     * If you want to make any receive operation to be able to recognize a
     * time-out, you have to insert the following code snipped before your
     * receive call.
     *
     * \code
     *   MPI_Status  status;
     *   int         flag = 0;
     *   clock_t     timeOutWarning   = Node::getInstance().getDeadlockWarningTimeStamp();
     *   clock_t     timeOutShutdown  = Node::getInstance().getDeadlockTimeOutTimeStamp();
     *   bool        triggeredTimeoutWarning = false;
     *   MPI_Iprobe( source, CommunicationTag, Node::getInstance().getCommunicator(), &flag, &status );
     *
     *   while (!flag) {
     *     if ( Node::getInstance().isTimeOutWarningEnabled() && (clock()>timeOutWarning) && (!triggeredTimeoutWarning)) {
     *       Node::getInstance().writeTimeOutWarning( "parallel::SendReceiveBuffer", "receiveAllMessages()", source );
     *       triggeredTimeoutWarning = true;
     *     }
     *     if ( Node::getInstance().isTimeOutDeadlockEnabled() && (clock()>timeOutShutdown)) {
     *       Node::getInstance().triggerDeadlockTimeOut( "parallel::SendReceiveBuffer", "receiveAllMessages()", source );
     *     }
     *     MPI_Iprobe( source, CommunicationTag, Node::getInstance().getCommunicator(), &flag, &status );
     *   }
     * \endcode
     *
     * @return Time stamp when next warning should be written if no message has
     *         been received meanwhile.
     */
    std::clock_t getDeadlockWarningTimeStamp() const;

    /**
     * @return Time stamp when next application should terminate because of a
     *         time out if no message has been received meanwhile.
     */
    std::clock_t getDeadlockTimeOutTimeStamp() const;

    bool isTimeOutDeadlockEnabled() const;
    bool isTimeOutWarningEnabled() const;

    bool isInitialised() const;

    /**
     * Set time out warning
     *
     * Set after how much time a node waiting for an MPI message shall write a
     * warning that it is likely that it ran into a deadlock. If you pass 0,
     * that switches off this feature.
     */
    void setTimeOutWarning( const std::clock_t & value );

    /**
     * Set deadlock time out
     *
     * Set after how much time a node waiting for an MPI message shall quit
     * and shutdown the whole application with an error report. If you pass 0,
     * that switches off this feature.
     */
    void setDeadlockTimeOut( const std::clock_t & value );

    /**
     * Set communicator to be used by Peano
     */
    void setCommunicator( MPI_Comm communicator );

    /**
     * Receive Dangling MPI Messages
     *
     * This operation tells all the services that they shall poll the MPI queues
     * for additional messages. Thus, it is questionable why such an operation
     * is assigned to this class. It only wraps the services. However, all these
     * 'hey poll the MPI queues' is always triggered by blocking sends and
     * receives, i.e. it is tied to the parallelisation. And, thus, it is part
     * of the node singleton.
     */
    void receiveDanglingMessages();

    void suspendTimeouts( bool timeoutsDisabled );
};

#endif
