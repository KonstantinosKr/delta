#include "dem/records/State.h"

#if !defined(TrackGridStatistics) && !defined(Parallel)
   dem::records::State::PersistentRecords::PersistentRecords() {
      
   }
   
   
   dem::records::State::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
   _numberOfContactPoints(numberOfContactPoints),
   _numberOfParticleReassignments(numberOfParticleReassignments),
   _numberOfTriangleComparisons(numberOfTriangleComparisons),
   _timeStepSize(timeStepSize),
   _currentTime(currentTime),
   _twoParticlesAreClose(twoParticlesAreClose),
   _hasRefined(hasRefined),
   _hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
   _hasErased(hasErased),
   _hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
   _hasChangedVertexOrCellState(hasChangedVertexOrCellState),
   _hasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration),
   _isTraversalInverted(isTraversalInverted) {
      
   }
   
   dem::records::State::State() {
      
   }
   
   
   dem::records::State::State(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._timeStepSize, persistentRecords._currentTime, persistentRecords._twoParticlesAreClose, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._hasModifiedGridInPreviousIteration, persistentRecords._isTraversalInverted) {
      
   }
   
   
   dem::records::State::State(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
   _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, timeStepSize, currentTime, twoParticlesAreClose, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted) {
      
   }
   
   
   dem::records::State::~State() { }
   
   
   
   std::string dem::records::State::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::State::toString (std::ostream& out) const {
      out << "("; 
      out << "numberOfContactPoints:" << getNumberOfContactPoints();
      out << ",";
      out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
      out << ",";
      out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
      out << ",";
      out << "timeStepSize:" << getTimeStepSize();
      out << ",";
      out << "currentTime:" << getCurrentTime();
      out << ",";
      out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
      out << ",";
      out << "hasRefined:" << getHasRefined();
      out << ",";
      out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
      out << ",";
      out << "hasErased:" << getHasErased();
      out << ",";
      out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
      out << ",";
      out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
      out << ",";
      out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
      out << ",";
      out << "isTraversalInverted:" << getIsTraversalInverted();
      out <<  ")";
   }
   
   
   dem::records::State::PersistentRecords dem::records::State::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::StatePacked dem::records::State::convert() const{
      return StatePacked(
         getNumberOfContactPoints(),
         getNumberOfParticleReassignments(),
         getNumberOfTriangleComparisons(),
         getTimeStepSize(),
         getCurrentTime(),
         getTwoParticlesAreClose(),
         getHasRefined(),
         getHasTriggeredRefinementForNextIteration(),
         getHasErased(),
         getHasTriggeredEraseForNextIteration(),
         getHasChangedVertexOrCellState(),
         getHasModifiedGridInPreviousIteration(),
         getIsTraversalInverted()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::State::_log( "dem::records::State" );
      
      MPI_Datatype dem::records::State::Datatype = 0;
      MPI_Datatype dem::records::State::FullDatatype = 0;
      
      
      void dem::records::State::initDatatype() {
         {
            State dummyState[2];
            
            const int Attributes = 14;
            MPI_Datatype subtypes[Attributes] = {
               MPI_DOUBLE,		 //numberOfContactPoints
               MPI_DOUBLE,		 //numberOfParticleReassignments
               MPI_DOUBLE,		 //numberOfTriangleComparisons
               MPI_DOUBLE,		 //timeStepSize
               MPI_DOUBLE,		 //currentTime
               MPI_CHAR,		 //twoParticlesAreClose
               MPI_CHAR,		 //hasRefined
               MPI_CHAR,		 //hasTriggeredRefinementForNextIteration
               MPI_CHAR,		 //hasErased
               MPI_CHAR,		 //hasTriggeredEraseForNextIteration
               MPI_CHAR,		 //hasChangedVertexOrCellState
               MPI_CHAR,		 //hasModifiedGridInPreviousIteration
               MPI_CHAR,		 //isTraversalInverted
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //numberOfContactPoints
               1,		 //numberOfParticleReassignments
               1,		 //numberOfTriangleComparisons
               1,		 //timeStepSize
               1,		 //currentTime
               1,		 //twoParticlesAreClose
               1,		 //hasRefined
               1,		 //hasTriggeredRefinementForNextIteration
               1,		 //hasErased
               1,		 //hasTriggeredEraseForNextIteration
               1,		 //hasChangedVertexOrCellState
               1,		 //hasModifiedGridInPreviousIteration
               1,		 //isTraversalInverted
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[6] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[7] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[8] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[9] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[10] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[11] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[12] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]._persistentRecords._numberOfContactPoints))), 		&disp[13] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::Datatype );
            MPI_Type_commit( &State::Datatype );
            
         }
         {
            State dummyState[2];
            
            const int Attributes = 14;
            MPI_Datatype subtypes[Attributes] = {
               MPI_DOUBLE,		 //numberOfContactPoints
               MPI_DOUBLE,		 //numberOfParticleReassignments
               MPI_DOUBLE,		 //numberOfTriangleComparisons
               MPI_DOUBLE,		 //timeStepSize
               MPI_DOUBLE,		 //currentTime
               MPI_CHAR,		 //twoParticlesAreClose
               MPI_CHAR,		 //hasRefined
               MPI_CHAR,		 //hasTriggeredRefinementForNextIteration
               MPI_CHAR,		 //hasErased
               MPI_CHAR,		 //hasTriggeredEraseForNextIteration
               MPI_CHAR,		 //hasChangedVertexOrCellState
               MPI_CHAR,		 //hasModifiedGridInPreviousIteration
               MPI_CHAR,		 //isTraversalInverted
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //numberOfContactPoints
               1,		 //numberOfParticleReassignments
               1,		 //numberOfTriangleComparisons
               1,		 //timeStepSize
               1,		 //currentTime
               1,		 //twoParticlesAreClose
               1,		 //hasRefined
               1,		 //hasTriggeredRefinementForNextIteration
               1,		 //hasErased
               1,		 //hasTriggeredEraseForNextIteration
               1,		 //hasChangedVertexOrCellState
               1,		 //hasModifiedGridInPreviousIteration
               1,		 //isTraversalInverted
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[6] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[7] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[8] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[9] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[10] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[11] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[12] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]._persistentRecords._numberOfContactPoints))), 		&disp[13] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::FullDatatype );
            MPI_Type_commit( &State::FullDatatype );
            
         }
         
      }
      
      
      void dem::records::State::shutdownDatatype() {
         MPI_Type_free( &State::Datatype );
         MPI_Type_free( &State::FullDatatype );
         
      }
      
      void dem::records::State::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         _senderDestinationRank = destination;
         
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message dem::records::State "
               << toString()
               << " to node " << destination
               << ": " << tarch::parallel::MPIReturnValueToString(result);
               _log.error( "send(int)",msg.str() );
            }
            
         }
         else {
         
         MPI_Request* sendRequestHandle = new MPI_Request();
         MPI_Status   status;
         int          flag = 0;
         int          result;
         
         clock_t      timeOutWarning   = -1;
         clock_t      timeOutShutdown  = -1;
         bool         triggeredTimeoutWarning = false;
         
         if (exchangeOnlyAttributesMarkedWithParallelise) {
            result = MPI_Isend(
               this, 1, Datatype, destination,
               tag, tarch::parallel::Node::getInstance().getCommunicator(),
               sendRequestHandle
            );
            
         }
         else {
            result = MPI_Isend(
               this, 1, FullDatatype, destination,
               tag, tarch::parallel::Node::getInstance().getCommunicator(),
               sendRequestHandle
            );
            
         }
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message dem::records::State "
            << toString()
            << " to node " << destination
            << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "send(int)",msg.str() );
         }
         result = MPI_Test( sendRequestHandle, &flag, &status );
         while (!flag) {
            if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
            if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
            result = MPI_Test( sendRequestHandle, &flag, &status );
            if (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "testing for finished send task for dem::records::State "
               << toString()
               << " sent to node " << destination
               << " failed: " << tarch::parallel::MPIReturnValueToString(result);
               _log.error("send(int)", msg.str() );
            }
            
            // deadlock aspect
            if (
               tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
               (clock()>timeOutWarning) &&
               (!triggeredTimeoutWarning)
            ) {
               tarch::parallel::Node::getInstance().writeTimeOutWarning(
               "dem::records::State",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::State",
               "send(int)", destination,tag,1
               );
            }
            tarch::parallel::Node::getInstance().receiveDanglingMessages();
            usleep(communicateSleep);
            
         }
         
         delete sendRequestHandle;
         #ifdef Debug
         _log.debug("send(int,int)", "sent " + toString() );
         #endif
         
      }
      
   }
   
   
   
   void dem::records::State::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         _senderDestinationRank = status.MPI_SOURCE;
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive dem::records::State from node "
            << source << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "receive(int)", msg.str() );
         }
         
      }
      else {
      
         MPI_Request* sendRequestHandle = new MPI_Request();
         MPI_Status   status;
         int          flag = 0;
         int          result;
         
         clock_t      timeOutWarning   = -1;
         clock_t      timeOutShutdown  = -1;
         bool         triggeredTimeoutWarning = false;
         
         if (exchangeOnlyAttributesMarkedWithParallelise) {
            result = MPI_Irecv(
               this, 1, Datatype, source, tag,
               tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
            );
            
         }
         else {
            result = MPI_Irecv(
               this, 1, FullDatatype, source, tag,
               tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
            );
            
         }
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive dem::records::State from node "
            << source << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "receive(int)", msg.str() );
         }
         
         result = MPI_Test( sendRequestHandle, &flag, &status );
         while (!flag) {
            if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
            if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
            result = MPI_Test( sendRequestHandle, &flag, &status );
            if (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "testing for finished receive task for dem::records::State failed: "
               << tarch::parallel::MPIReturnValueToString(result);
               _log.error("receive(int)", msg.str() );
            }
            
            // deadlock aspect
            if (
               tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
               (clock()>timeOutWarning) &&
               (!triggeredTimeoutWarning)
            ) {
               tarch::parallel::Node::getInstance().writeTimeOutWarning(
               "dem::records::State",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::State",
               "receive(int)", source,tag,1
               );
            }
            tarch::parallel::Node::getInstance().receiveDanglingMessages();
            usleep(communicateSleep);
            
         }
         
         delete sendRequestHandle;
         
         _senderDestinationRank = status.MPI_SOURCE;
         #ifdef Debug
         _log.debug("receive(int,int)", "received " + toString() ); 
         #endif
         
      }
      
   }
   
   
   
   bool dem::records::State::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
      MPI_Status status;
      int  flag        = 0;
      MPI_Iprobe(
         MPI_ANY_SOURCE, tag,
         tarch::parallel::Node::getInstance().getCommunicator(), &flag, &status
      );
      if (flag) {
         int  messageCounter;
         if (exchangeOnlyAttributesMarkedWithParallelise) {
            MPI_Get_count(&status, Datatype, &messageCounter);
         }
         else {
            MPI_Get_count(&status, FullDatatype, &messageCounter);
         }
         return messageCounter > 0;
      }
      else return false;
      
   }
   
   int dem::records::State::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif


dem::records::StatePacked::PersistentRecords::PersistentRecords() {
   if ((6 >= (8 * sizeof(short int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(short int))));
   
}


dem::records::StatePacked::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
_numberOfContactPoints(numberOfContactPoints),
_numberOfParticleReassignments(numberOfParticleReassignments),
_numberOfTriangleComparisons(numberOfTriangleComparisons),
_timeStepSize(timeStepSize),
_currentTime(currentTime),
_twoParticlesAreClose(twoParticlesAreClose),
_isTraversalInverted(isTraversalInverted) {
   setHasRefined(hasRefined);
   setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
   setHasErased(hasErased);
   setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
   setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
   setHasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration);
   if ((6 >= (8 * sizeof(short int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(short int))));
   
}

dem::records::StatePacked::StatePacked() {
   if ((6 >= (8 * sizeof(short int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(short int))));
   
}


dem::records::StatePacked::StatePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._timeStepSize, persistentRecords._currentTime, persistentRecords._twoParticlesAreClose, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords.getHasModifiedGridInPreviousIteration(), persistentRecords._isTraversalInverted) {
   if ((6 >= (8 * sizeof(short int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(short int))));
   
}


dem::records::StatePacked::StatePacked(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
_persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, timeStepSize, currentTime, twoParticlesAreClose, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted) {
   if ((6 >= (8 * sizeof(short int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(short int))));
   
}


dem::records::StatePacked::~StatePacked() { }



std::string dem::records::StatePacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void dem::records::StatePacked::toString (std::ostream& out) const {
   out << "("; 
   out << "numberOfContactPoints:" << getNumberOfContactPoints();
   out << ",";
   out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
   out << ",";
   out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
   out << ",";
   out << "timeStepSize:" << getTimeStepSize();
   out << ",";
   out << "currentTime:" << getCurrentTime();
   out << ",";
   out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
   out << ",";
   out << "hasRefined:" << getHasRefined();
   out << ",";
   out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
   out << ",";
   out << "hasErased:" << getHasErased();
   out << ",";
   out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
   out << ",";
   out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
   out << ",";
   out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
   out << ",";
   out << "isTraversalInverted:" << getIsTraversalInverted();
   out <<  ")";
}


dem::records::StatePacked::PersistentRecords dem::records::StatePacked::getPersistentRecords() const {
   return _persistentRecords;
}

dem::records::State dem::records::StatePacked::convert() const{
   return State(
      getNumberOfContactPoints(),
      getNumberOfParticleReassignments(),
      getNumberOfTriangleComparisons(),
      getTimeStepSize(),
      getCurrentTime(),
      getTwoParticlesAreClose(),
      getHasRefined(),
      getHasTriggeredRefinementForNextIteration(),
      getHasErased(),
      getHasTriggeredEraseForNextIteration(),
      getHasChangedVertexOrCellState(),
      getHasModifiedGridInPreviousIteration(),
      getIsTraversalInverted()
   );
}

#ifdef Parallel
   tarch::logging::Log dem::records::StatePacked::_log( "dem::records::StatePacked" );
   
   MPI_Datatype dem::records::StatePacked::Datatype = 0;
   MPI_Datatype dem::records::StatePacked::FullDatatype = 0;
   
   
   void dem::records::StatePacked::initDatatype() {
      {
         StatePacked dummyStatePacked[2];
         
         const int Attributes = 9;
         MPI_Datatype subtypes[Attributes] = {
            MPI_DOUBLE,		 //numberOfContactPoints
            MPI_DOUBLE,		 //numberOfParticleReassignments
            MPI_DOUBLE,		 //numberOfTriangleComparisons
            MPI_DOUBLE,		 //timeStepSize
            MPI_DOUBLE,		 //currentTime
            MPI_CHAR,		 //twoParticlesAreClose
            MPI_CHAR,		 //isTraversalInverted
            MPI_SHORT,		 //_packedRecords0
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //numberOfContactPoints
            1,		 //numberOfParticleReassignments
            1,		 //numberOfTriangleComparisons
            1,		 //timeStepSize
            1,		 //currentTime
            1,		 //twoParticlesAreClose
            1,		 //isTraversalInverted
            1,		 //_packedRecords0
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[3] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[4] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[6] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[7] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]._persistentRecords._numberOfContactPoints))), 		&disp[8] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::Datatype );
         MPI_Type_commit( &StatePacked::Datatype );
         
      }
      {
         StatePacked dummyStatePacked[2];
         
         const int Attributes = 9;
         MPI_Datatype subtypes[Attributes] = {
            MPI_DOUBLE,		 //numberOfContactPoints
            MPI_DOUBLE,		 //numberOfParticleReassignments
            MPI_DOUBLE,		 //numberOfTriangleComparisons
            MPI_DOUBLE,		 //timeStepSize
            MPI_DOUBLE,		 //currentTime
            MPI_CHAR,		 //twoParticlesAreClose
            MPI_CHAR,		 //isTraversalInverted
            MPI_SHORT,		 //_packedRecords0
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //numberOfContactPoints
            1,		 //numberOfParticleReassignments
            1,		 //numberOfTriangleComparisons
            1,		 //timeStepSize
            1,		 //currentTime
            1,		 //twoParticlesAreClose
            1,		 //isTraversalInverted
            1,		 //_packedRecords0
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[3] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[4] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[6] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[7] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]._persistentRecords._numberOfContactPoints))), 		&disp[8] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::FullDatatype );
         MPI_Type_commit( &StatePacked::FullDatatype );
         
      }
      
   }
   
   
   void dem::records::StatePacked::shutdownDatatype() {
      MPI_Type_free( &StatePacked::Datatype );
      MPI_Type_free( &StatePacked::FullDatatype );
      
   }
   
   void dem::records::StatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      _senderDestinationRank = destination;
      
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message dem::records::StatePacked "
            << toString()
            << " to node " << destination
            << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "send(int)",msg.str() );
         }
         
      }
      else {
      
      MPI_Request* sendRequestHandle = new MPI_Request();
      MPI_Status   status;
      int          flag = 0;
      int          result;
      
      clock_t      timeOutWarning   = -1;
      clock_t      timeOutShutdown  = -1;
      bool         triggeredTimeoutWarning = false;
      
      if (exchangeOnlyAttributesMarkedWithParallelise) {
         result = MPI_Isend(
            this, 1, Datatype, destination,
            tag, tarch::parallel::Node::getInstance().getCommunicator(),
            sendRequestHandle
         );
         
      }
      else {
         result = MPI_Isend(
            this, 1, FullDatatype, destination,
            tag, tarch::parallel::Node::getInstance().getCommunicator(),
            sendRequestHandle
         );
         
      }
      if  (result!=MPI_SUCCESS) {
         std::ostringstream msg;
         msg << "was not able to send message dem::records::StatePacked "
         << toString()
         << " to node " << destination
         << ": " << tarch::parallel::MPIReturnValueToString(result);
         _log.error( "send(int)",msg.str() );
      }
      result = MPI_Test( sendRequestHandle, &flag, &status );
      while (!flag) {
         if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
         if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
         result = MPI_Test( sendRequestHandle, &flag, &status );
         if (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "testing for finished send task for dem::records::StatePacked "
            << toString()
            << " sent to node " << destination
            << " failed: " << tarch::parallel::MPIReturnValueToString(result);
            _log.error("send(int)", msg.str() );
         }
         
         // deadlock aspect
         if (
            tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
            (clock()>timeOutWarning) &&
            (!triggeredTimeoutWarning)
         ) {
            tarch::parallel::Node::getInstance().writeTimeOutWarning(
            "dem::records::StatePacked",
            "send(int)", destination,tag,1
            );
            triggeredTimeoutWarning = true;
         }
         if (
            tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
            (clock()>timeOutShutdown)
         ) {
            tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
            "dem::records::StatePacked",
            "send(int)", destination,tag,1
            );
         }
         tarch::parallel::Node::getInstance().receiveDanglingMessages();
         usleep(communicateSleep);
         
      }
      
      delete sendRequestHandle;
      #ifdef Debug
      _log.debug("send(int,int)", "sent " + toString() );
      #endif
      
   }
   
}



void dem::records::StatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
   if (communicateSleep<0) {
   
      MPI_Status  status;
      const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
      _senderDestinationRank = status.MPI_SOURCE;
      if ( result != MPI_SUCCESS ) {
         std::ostringstream msg;
         msg << "failed to start to receive dem::records::StatePacked from node "
         << source << ": " << tarch::parallel::MPIReturnValueToString(result);
         _log.error( "receive(int)", msg.str() );
      }
      
   }
   else {
   
      MPI_Request* sendRequestHandle = new MPI_Request();
      MPI_Status   status;
      int          flag = 0;
      int          result;
      
      clock_t      timeOutWarning   = -1;
      clock_t      timeOutShutdown  = -1;
      bool         triggeredTimeoutWarning = false;
      
      if (exchangeOnlyAttributesMarkedWithParallelise) {
         result = MPI_Irecv(
            this, 1, Datatype, source, tag,
            tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
         );
         
      }
      else {
         result = MPI_Irecv(
            this, 1, FullDatatype, source, tag,
            tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
         );
         
      }
      if ( result != MPI_SUCCESS ) {
         std::ostringstream msg;
         msg << "failed to start to receive dem::records::StatePacked from node "
         << source << ": " << tarch::parallel::MPIReturnValueToString(result);
         _log.error( "receive(int)", msg.str() );
      }
      
      result = MPI_Test( sendRequestHandle, &flag, &status );
      while (!flag) {
         if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
         if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
         result = MPI_Test( sendRequestHandle, &flag, &status );
         if (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "testing for finished receive task for dem::records::StatePacked failed: "
            << tarch::parallel::MPIReturnValueToString(result);
            _log.error("receive(int)", msg.str() );
         }
         
         // deadlock aspect
         if (
            tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
            (clock()>timeOutWarning) &&
            (!triggeredTimeoutWarning)
         ) {
            tarch::parallel::Node::getInstance().writeTimeOutWarning(
            "dem::records::StatePacked",
            "receive(int)", source,tag,1
            );
            triggeredTimeoutWarning = true;
         }
         if (
            tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
            (clock()>timeOutShutdown)
         ) {
            tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
            "dem::records::StatePacked",
            "receive(int)", source,tag,1
            );
         }
         tarch::parallel::Node::getInstance().receiveDanglingMessages();
         usleep(communicateSleep);
         
      }
      
      delete sendRequestHandle;
      
      _senderDestinationRank = status.MPI_SOURCE;
      #ifdef Debug
      _log.debug("receive(int,int)", "received " + toString() ); 
      #endif
      
   }
   
}



bool dem::records::StatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
   MPI_Status status;
   int  flag        = 0;
   MPI_Iprobe(
      MPI_ANY_SOURCE, tag,
      tarch::parallel::Node::getInstance().getCommunicator(), &flag, &status
   );
   if (flag) {
      int  messageCounter;
      if (exchangeOnlyAttributesMarkedWithParallelise) {
         MPI_Get_count(&status, Datatype, &messageCounter);
      }
      else {
         MPI_Get_count(&status, FullDatatype, &messageCounter);
      }
      return messageCounter > 0;
   }
   else return false;
   
}

int dem::records::StatePacked::getSenderRank() const {
   assertion( _senderDestinationRank!=-1 );
   return _senderDestinationRank;
   
}
#endif



#elif defined(TrackGridStatistics) && defined(Parallel)
dem::records::State::PersistentRecords::PersistentRecords() {

}


dem::records::State::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
_numberOfContactPoints(numberOfContactPoints),
_numberOfParticleReassignments(numberOfParticleReassignments),
_numberOfTriangleComparisons(numberOfTriangleComparisons),
_timeStepSize(timeStepSize),
_currentTime(currentTime),
_twoParticlesAreClose(twoParticlesAreClose),
_minMeshWidth(minMeshWidth),
_maxMeshWidth(maxMeshWidth),
_numberOfInnerVertices(numberOfInnerVertices),
_numberOfBoundaryVertices(numberOfBoundaryVertices),
_numberOfOuterVertices(numberOfOuterVertices),
_numberOfInnerCells(numberOfInnerCells),
_numberOfOuterCells(numberOfOuterCells),
_numberOfInnerLeafVertices(numberOfInnerLeafVertices),
_numberOfBoundaryLeafVertices(numberOfBoundaryLeafVertices),
_numberOfOuterLeafVertices(numberOfOuterLeafVertices),
_numberOfInnerLeafCells(numberOfInnerLeafCells),
_numberOfOuterLeafCells(numberOfOuterLeafCells),
_maxLevel(maxLevel),
_hasRefined(hasRefined),
_hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
_hasErased(hasErased),
_hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
_hasChangedVertexOrCellState(hasChangedVertexOrCellState),
_hasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration),
_isTraversalInverted(isTraversalInverted),
_reduceStateAndCell(reduceStateAndCell),
_couldNotEraseDueToDecompositionFlag(couldNotEraseDueToDecompositionFlag),
_subWorkerIsInvolvedInJoinOrFork(subWorkerIsInvolvedInJoinOrFork) {

}

dem::records::State::State() {

}


dem::records::State::State(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._timeStepSize, persistentRecords._currentTime, persistentRecords._twoParticlesAreClose, persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._numberOfInnerLeafVertices, persistentRecords._numberOfBoundaryLeafVertices, persistentRecords._numberOfOuterLeafVertices, persistentRecords._numberOfInnerLeafCells, persistentRecords._numberOfOuterLeafCells, persistentRecords._maxLevel, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._hasModifiedGridInPreviousIteration, persistentRecords._isTraversalInverted, persistentRecords._reduceStateAndCell, persistentRecords._couldNotEraseDueToDecompositionFlag, persistentRecords._subWorkerIsInvolvedInJoinOrFork) {

}


dem::records::State::State(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
_persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, timeStepSize, currentTime, twoParticlesAreClose, minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, numberOfInnerLeafVertices, numberOfBoundaryLeafVertices, numberOfOuterLeafVertices, numberOfInnerLeafCells, numberOfOuterLeafCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted, reduceStateAndCell, couldNotEraseDueToDecompositionFlag, subWorkerIsInvolvedInJoinOrFork) {

}


dem::records::State::~State() { }



std::string dem::records::State::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void dem::records::State::toString (std::ostream& out) const {
out << "("; 
out << "numberOfContactPoints:" << getNumberOfContactPoints();
out << ",";
out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
out << ",";
out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
out << ",";
out << "timeStepSize:" << getTimeStepSize();
out << ",";
out << "currentTime:" << getCurrentTime();
out << ",";
out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
out << ",";
out << "minMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMinMeshWidth(i) << ",";
   }
   out << getMinMeshWidth(DIMENSIONS-1) << "]";
out << ",";
out << "maxMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMaxMeshWidth(i) << ",";
   }
   out << getMaxMeshWidth(DIMENSIONS-1) << "]";
out << ",";
out << "numberOfInnerVertices:" << getNumberOfInnerVertices();
out << ",";
out << "numberOfBoundaryVertices:" << getNumberOfBoundaryVertices();
out << ",";
out << "numberOfOuterVertices:" << getNumberOfOuterVertices();
out << ",";
out << "numberOfInnerCells:" << getNumberOfInnerCells();
out << ",";
out << "numberOfOuterCells:" << getNumberOfOuterCells();
out << ",";
out << "numberOfInnerLeafVertices:" << getNumberOfInnerLeafVertices();
out << ",";
out << "numberOfBoundaryLeafVertices:" << getNumberOfBoundaryLeafVertices();
out << ",";
out << "numberOfOuterLeafVertices:" << getNumberOfOuterLeafVertices();
out << ",";
out << "numberOfInnerLeafCells:" << getNumberOfInnerLeafCells();
out << ",";
out << "numberOfOuterLeafCells:" << getNumberOfOuterLeafCells();
out << ",";
out << "maxLevel:" << getMaxLevel();
out << ",";
out << "hasRefined:" << getHasRefined();
out << ",";
out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
out << ",";
out << "hasErased:" << getHasErased();
out << ",";
out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
out << ",";
out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
out << ",";
out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
out << ",";
out << "isTraversalInverted:" << getIsTraversalInverted();
out << ",";
out << "reduceStateAndCell:" << getReduceStateAndCell();
out << ",";
out << "couldNotEraseDueToDecompositionFlag:" << getCouldNotEraseDueToDecompositionFlag();
out << ",";
out << "subWorkerIsInvolvedInJoinOrFork:" << getSubWorkerIsInvolvedInJoinOrFork();
out <<  ")";
}


dem::records::State::PersistentRecords dem::records::State::getPersistentRecords() const {
return _persistentRecords;
}

dem::records::StatePacked dem::records::State::convert() const{
return StatePacked(
   getNumberOfContactPoints(),
   getNumberOfParticleReassignments(),
   getNumberOfTriangleComparisons(),
   getTimeStepSize(),
   getCurrentTime(),
   getTwoParticlesAreClose(),
   getMinMeshWidth(),
   getMaxMeshWidth(),
   getNumberOfInnerVertices(),
   getNumberOfBoundaryVertices(),
   getNumberOfOuterVertices(),
   getNumberOfInnerCells(),
   getNumberOfOuterCells(),
   getNumberOfInnerLeafVertices(),
   getNumberOfBoundaryLeafVertices(),
   getNumberOfOuterLeafVertices(),
   getNumberOfInnerLeafCells(),
   getNumberOfOuterLeafCells(),
   getMaxLevel(),
   getHasRefined(),
   getHasTriggeredRefinementForNextIteration(),
   getHasErased(),
   getHasTriggeredEraseForNextIteration(),
   getHasChangedVertexOrCellState(),
   getHasModifiedGridInPreviousIteration(),
   getIsTraversalInverted(),
   getReduceStateAndCell(),
   getCouldNotEraseDueToDecompositionFlag(),
   getSubWorkerIsInvolvedInJoinOrFork()
);
}

#ifdef Parallel
tarch::logging::Log dem::records::State::_log( "dem::records::State" );

MPI_Datatype dem::records::State::Datatype = 0;
MPI_Datatype dem::records::State::FullDatatype = 0;


void dem::records::State::initDatatype() {
   {
      State dummyState[2];
      
      const int Attributes = 30;
      MPI_Datatype subtypes[Attributes] = {
         MPI_DOUBLE,		 //numberOfContactPoints
         MPI_DOUBLE,		 //numberOfParticleReassignments
         MPI_DOUBLE,		 //numberOfTriangleComparisons
         MPI_DOUBLE,		 //timeStepSize
         MPI_DOUBLE,		 //currentTime
         MPI_CHAR,		 //twoParticlesAreClose
         MPI_DOUBLE,		 //minMeshWidth
         MPI_DOUBLE,		 //maxMeshWidth
         MPI_DOUBLE,		 //numberOfInnerVertices
         MPI_DOUBLE,		 //numberOfBoundaryVertices
         MPI_DOUBLE,		 //numberOfOuterVertices
         MPI_DOUBLE,		 //numberOfInnerCells
         MPI_DOUBLE,		 //numberOfOuterCells
         MPI_DOUBLE,		 //numberOfInnerLeafVertices
         MPI_DOUBLE,		 //numberOfBoundaryLeafVertices
         MPI_DOUBLE,		 //numberOfOuterLeafVertices
         MPI_DOUBLE,		 //numberOfInnerLeafCells
         MPI_DOUBLE,		 //numberOfOuterLeafCells
         MPI_INT,		 //maxLevel
         MPI_CHAR,		 //hasRefined
         MPI_CHAR,		 //hasTriggeredRefinementForNextIteration
         MPI_CHAR,		 //hasErased
         MPI_CHAR,		 //hasTriggeredEraseForNextIteration
         MPI_CHAR,		 //hasChangedVertexOrCellState
         MPI_CHAR,		 //hasModifiedGridInPreviousIteration
         MPI_CHAR,		 //isTraversalInverted
         MPI_CHAR,		 //reduceStateAndCell
         MPI_CHAR,		 //couldNotEraseDueToDecompositionFlag
         MPI_CHAR,		 //subWorkerIsInvolvedInJoinOrFork
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         1,		 //numberOfContactPoints
         1,		 //numberOfParticleReassignments
         1,		 //numberOfTriangleComparisons
         1,		 //timeStepSize
         1,		 //currentTime
         1,		 //twoParticlesAreClose
         DIMENSIONS,		 //minMeshWidth
         DIMENSIONS,		 //maxMeshWidth
         1,		 //numberOfInnerVertices
         1,		 //numberOfBoundaryVertices
         1,		 //numberOfOuterVertices
         1,		 //numberOfInnerCells
         1,		 //numberOfOuterCells
         1,		 //numberOfInnerLeafVertices
         1,		 //numberOfBoundaryLeafVertices
         1,		 //numberOfOuterLeafVertices
         1,		 //numberOfInnerLeafCells
         1,		 //numberOfOuterLeafCells
         1,		 //maxLevel
         1,		 //hasRefined
         1,		 //hasTriggeredRefinementForNextIteration
         1,		 //hasErased
         1,		 //hasTriggeredEraseForNextIteration
         1,		 //hasChangedVertexOrCellState
         1,		 //hasModifiedGridInPreviousIteration
         1,		 //isTraversalInverted
         1,		 //reduceStateAndCell
         1,		 //couldNotEraseDueToDecompositionFlag
         1,		 //subWorkerIsInvolvedInJoinOrFork
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]))), &base);
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[3] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[4] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[6] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[7] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[8] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[9] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[10] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[11] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[12] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[13] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[14] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[15] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[16] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[17] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[18] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[19] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[20] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[21] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[22] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[23] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[24] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[25] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[26] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[27] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[28] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]._persistentRecords._numberOfContactPoints))), 		&disp[29] );
      
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::Datatype );
      MPI_Type_commit( &State::Datatype );
      
   }
   {
      State dummyState[2];
      
      const int Attributes = 30;
      MPI_Datatype subtypes[Attributes] = {
         MPI_DOUBLE,		 //numberOfContactPoints
         MPI_DOUBLE,		 //numberOfParticleReassignments
         MPI_DOUBLE,		 //numberOfTriangleComparisons
         MPI_DOUBLE,		 //timeStepSize
         MPI_DOUBLE,		 //currentTime
         MPI_CHAR,		 //twoParticlesAreClose
         MPI_DOUBLE,		 //minMeshWidth
         MPI_DOUBLE,		 //maxMeshWidth
         MPI_DOUBLE,		 //numberOfInnerVertices
         MPI_DOUBLE,		 //numberOfBoundaryVertices
         MPI_DOUBLE,		 //numberOfOuterVertices
         MPI_DOUBLE,		 //numberOfInnerCells
         MPI_DOUBLE,		 //numberOfOuterCells
         MPI_DOUBLE,		 //numberOfInnerLeafVertices
         MPI_DOUBLE,		 //numberOfBoundaryLeafVertices
         MPI_DOUBLE,		 //numberOfOuterLeafVertices
         MPI_DOUBLE,		 //numberOfInnerLeafCells
         MPI_DOUBLE,		 //numberOfOuterLeafCells
         MPI_INT,		 //maxLevel
         MPI_CHAR,		 //hasRefined
         MPI_CHAR,		 //hasTriggeredRefinementForNextIteration
         MPI_CHAR,		 //hasErased
         MPI_CHAR,		 //hasTriggeredEraseForNextIteration
         MPI_CHAR,		 //hasChangedVertexOrCellState
         MPI_CHAR,		 //hasModifiedGridInPreviousIteration
         MPI_CHAR,		 //isTraversalInverted
         MPI_CHAR,		 //reduceStateAndCell
         MPI_CHAR,		 //couldNotEraseDueToDecompositionFlag
         MPI_CHAR,		 //subWorkerIsInvolvedInJoinOrFork
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         1,		 //numberOfContactPoints
         1,		 //numberOfParticleReassignments
         1,		 //numberOfTriangleComparisons
         1,		 //timeStepSize
         1,		 //currentTime
         1,		 //twoParticlesAreClose
         DIMENSIONS,		 //minMeshWidth
         DIMENSIONS,		 //maxMeshWidth
         1,		 //numberOfInnerVertices
         1,		 //numberOfBoundaryVertices
         1,		 //numberOfOuterVertices
         1,		 //numberOfInnerCells
         1,		 //numberOfOuterCells
         1,		 //numberOfInnerLeafVertices
         1,		 //numberOfBoundaryLeafVertices
         1,		 //numberOfOuterLeafVertices
         1,		 //numberOfInnerLeafCells
         1,		 //numberOfOuterLeafCells
         1,		 //maxLevel
         1,		 //hasRefined
         1,		 //hasTriggeredRefinementForNextIteration
         1,		 //hasErased
         1,		 //hasTriggeredEraseForNextIteration
         1,		 //hasChangedVertexOrCellState
         1,		 //hasModifiedGridInPreviousIteration
         1,		 //isTraversalInverted
         1,		 //reduceStateAndCell
         1,		 //couldNotEraseDueToDecompositionFlag
         1,		 //subWorkerIsInvolvedInJoinOrFork
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]))), &base);
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[3] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[4] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[6] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[7] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[8] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[9] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[10] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[11] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[12] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[13] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[14] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[15] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[16] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[17] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[18] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[19] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[20] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[21] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[22] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[23] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[24] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[25] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[26] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[27] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[28] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]._persistentRecords._numberOfContactPoints))), 		&disp[29] );
      
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::FullDatatype );
      MPI_Type_commit( &State::FullDatatype );
      
   }
   
}


void dem::records::State::shutdownDatatype() {
   MPI_Type_free( &State::Datatype );
   MPI_Type_free( &State::FullDatatype );
   
}

void dem::records::State::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
   _senderDestinationRank = destination;
   
   if (communicateSleep<0) {
   
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
      if  (result!=MPI_SUCCESS) {
         std::ostringstream msg;
         msg << "was not able to send message dem::records::State "
         << toString()
         << " to node " << destination
         << ": " << tarch::parallel::MPIReturnValueToString(result);
         _log.error( "send(int)",msg.str() );
      }
      
   }
   else {
   
   MPI_Request* sendRequestHandle = new MPI_Request();
   MPI_Status   status;
   int          flag = 0;
   int          result;
   
   clock_t      timeOutWarning   = -1;
   clock_t      timeOutShutdown  = -1;
   bool         triggeredTimeoutWarning = false;
   
   if (exchangeOnlyAttributesMarkedWithParallelise) {
      result = MPI_Isend(
         this, 1, Datatype, destination,
         tag, tarch::parallel::Node::getInstance().getCommunicator(),
         sendRequestHandle
      );
      
   }
   else {
      result = MPI_Isend(
         this, 1, FullDatatype, destination,
         tag, tarch::parallel::Node::getInstance().getCommunicator(),
         sendRequestHandle
      );
      
   }
   if  (result!=MPI_SUCCESS) {
      std::ostringstream msg;
      msg << "was not able to send message dem::records::State "
      << toString()
      << " to node " << destination
      << ": " << tarch::parallel::MPIReturnValueToString(result);
      _log.error( "send(int)",msg.str() );
   }
   result = MPI_Test( sendRequestHandle, &flag, &status );
   while (!flag) {
      if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
      if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
      result = MPI_Test( sendRequestHandle, &flag, &status );
      if (result!=MPI_SUCCESS) {
         std::ostringstream msg;
         msg << "testing for finished send task for dem::records::State "
         << toString()
         << " sent to node " << destination
         << " failed: " << tarch::parallel::MPIReturnValueToString(result);
         _log.error("send(int)", msg.str() );
      }
      
      // deadlock aspect
      if (
         tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
         (clock()>timeOutWarning) &&
         (!triggeredTimeoutWarning)
      ) {
         tarch::parallel::Node::getInstance().writeTimeOutWarning(
         "dem::records::State",
         "send(int)", destination,tag,1
         );
         triggeredTimeoutWarning = true;
      }
      if (
         tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
         (clock()>timeOutShutdown)
      ) {
         tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
         "dem::records::State",
         "send(int)", destination,tag,1
         );
      }
      tarch::parallel::Node::getInstance().receiveDanglingMessages();
      usleep(communicateSleep);
      
   }
   
   delete sendRequestHandle;
   #ifdef Debug
   _log.debug("send(int,int)", "sent " + toString() );
   #endif
   
}

}



void dem::records::State::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

   MPI_Status  status;
   const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
   _senderDestinationRank = status.MPI_SOURCE;
   if ( result != MPI_SUCCESS ) {
      std::ostringstream msg;
      msg << "failed to start to receive dem::records::State from node "
      << source << ": " << tarch::parallel::MPIReturnValueToString(result);
      _log.error( "receive(int)", msg.str() );
   }
   
}
else {

   MPI_Request* sendRequestHandle = new MPI_Request();
   MPI_Status   status;
   int          flag = 0;
   int          result;
   
   clock_t      timeOutWarning   = -1;
   clock_t      timeOutShutdown  = -1;
   bool         triggeredTimeoutWarning = false;
   
   if (exchangeOnlyAttributesMarkedWithParallelise) {
      result = MPI_Irecv(
         this, 1, Datatype, source, tag,
         tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
      );
      
   }
   else {
      result = MPI_Irecv(
         this, 1, FullDatatype, source, tag,
         tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
      );
      
   }
   if ( result != MPI_SUCCESS ) {
      std::ostringstream msg;
      msg << "failed to start to receive dem::records::State from node "
      << source << ": " << tarch::parallel::MPIReturnValueToString(result);
      _log.error( "receive(int)", msg.str() );
   }
   
   result = MPI_Test( sendRequestHandle, &flag, &status );
   while (!flag) {
      if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
      if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
      result = MPI_Test( sendRequestHandle, &flag, &status );
      if (result!=MPI_SUCCESS) {
         std::ostringstream msg;
         msg << "testing for finished receive task for dem::records::State failed: "
         << tarch::parallel::MPIReturnValueToString(result);
         _log.error("receive(int)", msg.str() );
      }
      
      // deadlock aspect
      if (
         tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
         (clock()>timeOutWarning) &&
         (!triggeredTimeoutWarning)
      ) {
         tarch::parallel::Node::getInstance().writeTimeOutWarning(
         "dem::records::State",
         "receive(int)", source,tag,1
         );
         triggeredTimeoutWarning = true;
      }
      if (
         tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
         (clock()>timeOutShutdown)
      ) {
         tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
         "dem::records::State",
         "receive(int)", source,tag,1
         );
      }
      tarch::parallel::Node::getInstance().receiveDanglingMessages();
      usleep(communicateSleep);
      
   }
   
   delete sendRequestHandle;
   
   _senderDestinationRank = status.MPI_SOURCE;
   #ifdef Debug
   _log.debug("receive(int,int)", "received " + toString() ); 
   #endif
   
}

}



bool dem::records::State::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Status status;
int  flag        = 0;
MPI_Iprobe(
   MPI_ANY_SOURCE, tag,
   tarch::parallel::Node::getInstance().getCommunicator(), &flag, &status
);
if (flag) {
   int  messageCounter;
   if (exchangeOnlyAttributesMarkedWithParallelise) {
      MPI_Get_count(&status, Datatype, &messageCounter);
   }
   else {
      MPI_Get_count(&status, FullDatatype, &messageCounter);
   }
   return messageCounter > 0;
}
else return false;

}

int dem::records::State::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


dem::records::StatePacked::PersistentRecords::PersistentRecords() {
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}


dem::records::StatePacked::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
_numberOfContactPoints(numberOfContactPoints),
_numberOfParticleReassignments(numberOfParticleReassignments),
_numberOfTriangleComparisons(numberOfTriangleComparisons),
_timeStepSize(timeStepSize),
_currentTime(currentTime),
_twoParticlesAreClose(twoParticlesAreClose),
_minMeshWidth(minMeshWidth),
_maxMeshWidth(maxMeshWidth),
_numberOfInnerVertices(numberOfInnerVertices),
_numberOfBoundaryVertices(numberOfBoundaryVertices),
_numberOfOuterVertices(numberOfOuterVertices),
_numberOfInnerCells(numberOfInnerCells),
_numberOfOuterCells(numberOfOuterCells),
_numberOfInnerLeafVertices(numberOfInnerLeafVertices),
_numberOfBoundaryLeafVertices(numberOfBoundaryLeafVertices),
_numberOfOuterLeafVertices(numberOfOuterLeafVertices),
_numberOfInnerLeafCells(numberOfInnerLeafCells),
_numberOfOuterLeafCells(numberOfOuterLeafCells),
_maxLevel(maxLevel),
_isTraversalInverted(isTraversalInverted) {
setHasRefined(hasRefined);
setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
setHasErased(hasErased);
setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
setHasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration);
setReduceStateAndCell(reduceStateAndCell);
setCouldNotEraseDueToDecompositionFlag(couldNotEraseDueToDecompositionFlag);
setSubWorkerIsInvolvedInJoinOrFork(subWorkerIsInvolvedInJoinOrFork);
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}

dem::records::StatePacked::StatePacked() {
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}


dem::records::StatePacked::StatePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._timeStepSize, persistentRecords._currentTime, persistentRecords._twoParticlesAreClose, persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._numberOfInnerLeafVertices, persistentRecords._numberOfBoundaryLeafVertices, persistentRecords._numberOfOuterLeafVertices, persistentRecords._numberOfInnerLeafCells, persistentRecords._numberOfOuterLeafCells, persistentRecords._maxLevel, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords.getHasModifiedGridInPreviousIteration(), persistentRecords._isTraversalInverted, persistentRecords.getReduceStateAndCell(), persistentRecords.getCouldNotEraseDueToDecompositionFlag(), persistentRecords.getSubWorkerIsInvolvedInJoinOrFork()) {
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}


dem::records::StatePacked::StatePacked(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
_persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, timeStepSize, currentTime, twoParticlesAreClose, minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, numberOfInnerLeafVertices, numberOfBoundaryLeafVertices, numberOfOuterLeafVertices, numberOfInnerLeafCells, numberOfOuterLeafCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted, reduceStateAndCell, couldNotEraseDueToDecompositionFlag, subWorkerIsInvolvedInJoinOrFork) {
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}


dem::records::StatePacked::~StatePacked() { }



std::string dem::records::StatePacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void dem::records::StatePacked::toString (std::ostream& out) const {
out << "("; 
out << "numberOfContactPoints:" << getNumberOfContactPoints();
out << ",";
out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
out << ",";
out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
out << ",";
out << "timeStepSize:" << getTimeStepSize();
out << ",";
out << "currentTime:" << getCurrentTime();
out << ",";
out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
out << ",";
out << "minMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMinMeshWidth(i) << ",";
   }
   out << getMinMeshWidth(DIMENSIONS-1) << "]";
out << ",";
out << "maxMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMaxMeshWidth(i) << ",";
   }
   out << getMaxMeshWidth(DIMENSIONS-1) << "]";
out << ",";
out << "numberOfInnerVertices:" << getNumberOfInnerVertices();
out << ",";
out << "numberOfBoundaryVertices:" << getNumberOfBoundaryVertices();
out << ",";
out << "numberOfOuterVertices:" << getNumberOfOuterVertices();
out << ",";
out << "numberOfInnerCells:" << getNumberOfInnerCells();
out << ",";
out << "numberOfOuterCells:" << getNumberOfOuterCells();
out << ",";
out << "numberOfInnerLeafVertices:" << getNumberOfInnerLeafVertices();
out << ",";
out << "numberOfBoundaryLeafVertices:" << getNumberOfBoundaryLeafVertices();
out << ",";
out << "numberOfOuterLeafVertices:" << getNumberOfOuterLeafVertices();
out << ",";
out << "numberOfInnerLeafCells:" << getNumberOfInnerLeafCells();
out << ",";
out << "numberOfOuterLeafCells:" << getNumberOfOuterLeafCells();
out << ",";
out << "maxLevel:" << getMaxLevel();
out << ",";
out << "hasRefined:" << getHasRefined();
out << ",";
out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
out << ",";
out << "hasErased:" << getHasErased();
out << ",";
out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
out << ",";
out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
out << ",";
out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
out << ",";
out << "isTraversalInverted:" << getIsTraversalInverted();
out << ",";
out << "reduceStateAndCell:" << getReduceStateAndCell();
out << ",";
out << "couldNotEraseDueToDecompositionFlag:" << getCouldNotEraseDueToDecompositionFlag();
out << ",";
out << "subWorkerIsInvolvedInJoinOrFork:" << getSubWorkerIsInvolvedInJoinOrFork();
out <<  ")";
}


dem::records::StatePacked::PersistentRecords dem::records::StatePacked::getPersistentRecords() const {
return _persistentRecords;
}

dem::records::State dem::records::StatePacked::convert() const{
return State(
getNumberOfContactPoints(),
getNumberOfParticleReassignments(),
getNumberOfTriangleComparisons(),
getTimeStepSize(),
getCurrentTime(),
getTwoParticlesAreClose(),
getMinMeshWidth(),
getMaxMeshWidth(),
getNumberOfInnerVertices(),
getNumberOfBoundaryVertices(),
getNumberOfOuterVertices(),
getNumberOfInnerCells(),
getNumberOfOuterCells(),
getNumberOfInnerLeafVertices(),
getNumberOfBoundaryLeafVertices(),
getNumberOfOuterLeafVertices(),
getNumberOfInnerLeafCells(),
getNumberOfOuterLeafCells(),
getMaxLevel(),
getHasRefined(),
getHasTriggeredRefinementForNextIteration(),
getHasErased(),
getHasTriggeredEraseForNextIteration(),
getHasChangedVertexOrCellState(),
getHasModifiedGridInPreviousIteration(),
getIsTraversalInverted(),
getReduceStateAndCell(),
getCouldNotEraseDueToDecompositionFlag(),
getSubWorkerIsInvolvedInJoinOrFork()
);
}

#ifdef Parallel
tarch::logging::Log dem::records::StatePacked::_log( "dem::records::StatePacked" );

MPI_Datatype dem::records::StatePacked::Datatype = 0;
MPI_Datatype dem::records::StatePacked::FullDatatype = 0;


void dem::records::StatePacked::initDatatype() {
{
   StatePacked dummyStatePacked[2];
   
   const int Attributes = 22;
   MPI_Datatype subtypes[Attributes] = {
      MPI_DOUBLE,		 //numberOfContactPoints
      MPI_DOUBLE,		 //numberOfParticleReassignments
      MPI_DOUBLE,		 //numberOfTriangleComparisons
      MPI_DOUBLE,		 //timeStepSize
      MPI_DOUBLE,		 //currentTime
      MPI_CHAR,		 //twoParticlesAreClose
      MPI_DOUBLE,		 //minMeshWidth
      MPI_DOUBLE,		 //maxMeshWidth
      MPI_DOUBLE,		 //numberOfInnerVertices
      MPI_DOUBLE,		 //numberOfBoundaryVertices
      MPI_DOUBLE,		 //numberOfOuterVertices
      MPI_DOUBLE,		 //numberOfInnerCells
      MPI_DOUBLE,		 //numberOfOuterCells
      MPI_DOUBLE,		 //numberOfInnerLeafVertices
      MPI_DOUBLE,		 //numberOfBoundaryLeafVertices
      MPI_DOUBLE,		 //numberOfOuterLeafVertices
      MPI_DOUBLE,		 //numberOfInnerLeafCells
      MPI_DOUBLE,		 //numberOfOuterLeafCells
      MPI_INT,		 //maxLevel
      MPI_CHAR,		 //isTraversalInverted
      MPI_SHORT,		 //_packedRecords0
      MPI_UB		 // end/displacement flag
   };
   
   int blocklen[Attributes] = {
      1,		 //numberOfContactPoints
      1,		 //numberOfParticleReassignments
      1,		 //numberOfTriangleComparisons
      1,		 //timeStepSize
      1,		 //currentTime
      1,		 //twoParticlesAreClose
      DIMENSIONS,		 //minMeshWidth
      DIMENSIONS,		 //maxMeshWidth
      1,		 //numberOfInnerVertices
      1,		 //numberOfBoundaryVertices
      1,		 //numberOfOuterVertices
      1,		 //numberOfInnerCells
      1,		 //numberOfOuterCells
      1,		 //numberOfInnerLeafVertices
      1,		 //numberOfBoundaryLeafVertices
      1,		 //numberOfOuterLeafVertices
      1,		 //numberOfInnerLeafCells
      1,		 //numberOfOuterLeafCells
      1,		 //maxLevel
      1,		 //isTraversalInverted
      1,		 //_packedRecords0
      1		 // end/displacement flag
   };
   
   MPI_Aint     disp[Attributes];
   
   MPI_Aint base;
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]))), &base);
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[3] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[4] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[6] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[7] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[8] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[9] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[10] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[11] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[12] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[13] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[14] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[15] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[16] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[17] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[18] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[19] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[20] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]._persistentRecords._numberOfContactPoints))), 		&disp[21] );
   
   for (int i=1; i<Attributes; i++) {
      assertion1( disp[i] > disp[i-1], i );
   }
   for (int i=0; i<Attributes; i++) {
      disp[i] -= base;
   }
   MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::Datatype );
   MPI_Type_commit( &StatePacked::Datatype );
   
}
{
   StatePacked dummyStatePacked[2];
   
   const int Attributes = 22;
   MPI_Datatype subtypes[Attributes] = {
      MPI_DOUBLE,		 //numberOfContactPoints
      MPI_DOUBLE,		 //numberOfParticleReassignments
      MPI_DOUBLE,		 //numberOfTriangleComparisons
      MPI_DOUBLE,		 //timeStepSize
      MPI_DOUBLE,		 //currentTime
      MPI_CHAR,		 //twoParticlesAreClose
      MPI_DOUBLE,		 //minMeshWidth
      MPI_DOUBLE,		 //maxMeshWidth
      MPI_DOUBLE,		 //numberOfInnerVertices
      MPI_DOUBLE,		 //numberOfBoundaryVertices
      MPI_DOUBLE,		 //numberOfOuterVertices
      MPI_DOUBLE,		 //numberOfInnerCells
      MPI_DOUBLE,		 //numberOfOuterCells
      MPI_DOUBLE,		 //numberOfInnerLeafVertices
      MPI_DOUBLE,		 //numberOfBoundaryLeafVertices
      MPI_DOUBLE,		 //numberOfOuterLeafVertices
      MPI_DOUBLE,		 //numberOfInnerLeafCells
      MPI_DOUBLE,		 //numberOfOuterLeafCells
      MPI_INT,		 //maxLevel
      MPI_CHAR,		 //isTraversalInverted
      MPI_SHORT,		 //_packedRecords0
      MPI_UB		 // end/displacement flag
   };
   
   int blocklen[Attributes] = {
      1,		 //numberOfContactPoints
      1,		 //numberOfParticleReassignments
      1,		 //numberOfTriangleComparisons
      1,		 //timeStepSize
      1,		 //currentTime
      1,		 //twoParticlesAreClose
      DIMENSIONS,		 //minMeshWidth
      DIMENSIONS,		 //maxMeshWidth
      1,		 //numberOfInnerVertices
      1,		 //numberOfBoundaryVertices
      1,		 //numberOfOuterVertices
      1,		 //numberOfInnerCells
      1,		 //numberOfOuterCells
      1,		 //numberOfInnerLeafVertices
      1,		 //numberOfBoundaryLeafVertices
      1,		 //numberOfOuterLeafVertices
      1,		 //numberOfInnerLeafCells
      1,		 //numberOfOuterLeafCells
      1,		 //maxLevel
      1,		 //isTraversalInverted
      1,		 //_packedRecords0
      1		 // end/displacement flag
   };
   
   MPI_Aint     disp[Attributes];
   
   MPI_Aint base;
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]))), &base);
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[3] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[4] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[6] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[7] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[8] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[9] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[10] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[11] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[12] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[13] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[14] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[15] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[16] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[17] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[18] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[19] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[20] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]._persistentRecords._numberOfContactPoints))), 		&disp[21] );
   
   for (int i=1; i<Attributes; i++) {
      assertion1( disp[i] > disp[i-1], i );
   }
   for (int i=0; i<Attributes; i++) {
      disp[i] -= base;
   }
   MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::FullDatatype );
   MPI_Type_commit( &StatePacked::FullDatatype );
   
}

}


void dem::records::StatePacked::shutdownDatatype() {
MPI_Type_free( &StatePacked::Datatype );
MPI_Type_free( &StatePacked::FullDatatype );

}

void dem::records::StatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

   const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
   if  (result!=MPI_SUCCESS) {
      std::ostringstream msg;
      msg << "was not able to send message dem::records::StatePacked "
      << toString()
      << " to node " << destination
      << ": " << tarch::parallel::MPIReturnValueToString(result);
      _log.error( "send(int)",msg.str() );
   }
   
}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
   result = MPI_Isend(
      this, 1, Datatype, destination,
      tag, tarch::parallel::Node::getInstance().getCommunicator(),
      sendRequestHandle
   );
   
}
else {
   result = MPI_Isend(
      this, 1, FullDatatype, destination,
      tag, tarch::parallel::Node::getInstance().getCommunicator(),
      sendRequestHandle
   );
   
}
if  (result!=MPI_SUCCESS) {
   std::ostringstream msg;
   msg << "was not able to send message dem::records::StatePacked "
   << toString()
   << " to node " << destination
   << ": " << tarch::parallel::MPIReturnValueToString(result);
   _log.error( "send(int)",msg.str() );
}
result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
   if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
   if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
   result = MPI_Test( sendRequestHandle, &flag, &status );
   if (result!=MPI_SUCCESS) {
      std::ostringstream msg;
      msg << "testing for finished send task for dem::records::StatePacked "
      << toString()
      << " sent to node " << destination
      << " failed: " << tarch::parallel::MPIReturnValueToString(result);
      _log.error("send(int)", msg.str() );
   }
   
   // deadlock aspect
   if (
      tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
      (clock()>timeOutWarning) &&
      (!triggeredTimeoutWarning)
   ) {
      tarch::parallel::Node::getInstance().writeTimeOutWarning(
      "dem::records::StatePacked",
      "send(int)", destination,tag,1
      );
      triggeredTimeoutWarning = true;
   }
   if (
      tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
      (clock()>timeOutShutdown)
   ) {
      tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
      "dem::records::StatePacked",
      "send(int)", destination,tag,1
      );
   }
   tarch::parallel::Node::getInstance().receiveDanglingMessages();
   usleep(communicateSleep);
   
}

delete sendRequestHandle;
#ifdef Debug
_log.debug("send(int,int)", "sent " + toString() );
#endif

}

}



void dem::records::StatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
   std::ostringstream msg;
   msg << "failed to start to receive dem::records::StatePacked from node "
   << source << ": " << tarch::parallel::MPIReturnValueToString(result);
   _log.error( "receive(int)", msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
   result = MPI_Irecv(
      this, 1, Datatype, source, tag,
      tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
   );
   
}
else {
   result = MPI_Irecv(
      this, 1, FullDatatype, source, tag,
      tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
   );
   
}
if ( result != MPI_SUCCESS ) {
   std::ostringstream msg;
   msg << "failed to start to receive dem::records::StatePacked from node "
   << source << ": " << tarch::parallel::MPIReturnValueToString(result);
   _log.error( "receive(int)", msg.str() );
}

result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
   if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
   if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
   result = MPI_Test( sendRequestHandle, &flag, &status );
   if (result!=MPI_SUCCESS) {
      std::ostringstream msg;
      msg << "testing for finished receive task for dem::records::StatePacked failed: "
      << tarch::parallel::MPIReturnValueToString(result);
      _log.error("receive(int)", msg.str() );
   }
   
   // deadlock aspect
   if (
      tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
      (clock()>timeOutWarning) &&
      (!triggeredTimeoutWarning)
   ) {
      tarch::parallel::Node::getInstance().writeTimeOutWarning(
      "dem::records::StatePacked",
      "receive(int)", source,tag,1
      );
      triggeredTimeoutWarning = true;
   }
   if (
      tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
      (clock()>timeOutShutdown)
   ) {
      tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
      "dem::records::StatePacked",
      "receive(int)", source,tag,1
      );
   }
   tarch::parallel::Node::getInstance().receiveDanglingMessages();
   usleep(communicateSleep);
   
}

delete sendRequestHandle;

_senderDestinationRank = status.MPI_SOURCE;
#ifdef Debug
_log.debug("receive(int,int)", "received " + toString() ); 
#endif

}

}



bool dem::records::StatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Status status;
int  flag        = 0;
MPI_Iprobe(
MPI_ANY_SOURCE, tag,
tarch::parallel::Node::getInstance().getCommunicator(), &flag, &status
);
if (flag) {
int  messageCounter;
if (exchangeOnlyAttributesMarkedWithParallelise) {
   MPI_Get_count(&status, Datatype, &messageCounter);
}
else {
   MPI_Get_count(&status, FullDatatype, &messageCounter);
}
return messageCounter > 0;
}
else return false;

}

int dem::records::StatePacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif defined(TrackGridStatistics) && !defined(Parallel)
dem::records::State::PersistentRecords::PersistentRecords() {

}


dem::records::State::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
_numberOfContactPoints(numberOfContactPoints),
_numberOfParticleReassignments(numberOfParticleReassignments),
_numberOfTriangleComparisons(numberOfTriangleComparisons),
_timeStepSize(timeStepSize),
_currentTime(currentTime),
_twoParticlesAreClose(twoParticlesAreClose),
_minMeshWidth(minMeshWidth),
_maxMeshWidth(maxMeshWidth),
_numberOfInnerVertices(numberOfInnerVertices),
_numberOfBoundaryVertices(numberOfBoundaryVertices),
_numberOfOuterVertices(numberOfOuterVertices),
_numberOfInnerCells(numberOfInnerCells),
_numberOfOuterCells(numberOfOuterCells),
_numberOfInnerLeafVertices(numberOfInnerLeafVertices),
_numberOfBoundaryLeafVertices(numberOfBoundaryLeafVertices),
_numberOfOuterLeafVertices(numberOfOuterLeafVertices),
_numberOfInnerLeafCells(numberOfInnerLeafCells),
_numberOfOuterLeafCells(numberOfOuterLeafCells),
_maxLevel(maxLevel),
_hasRefined(hasRefined),
_hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
_hasErased(hasErased),
_hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
_hasChangedVertexOrCellState(hasChangedVertexOrCellState),
_hasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration),
_isTraversalInverted(isTraversalInverted) {

}

dem::records::State::State() {

}


dem::records::State::State(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._timeStepSize, persistentRecords._currentTime, persistentRecords._twoParticlesAreClose, persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._numberOfInnerLeafVertices, persistentRecords._numberOfBoundaryLeafVertices, persistentRecords._numberOfOuterLeafVertices, persistentRecords._numberOfInnerLeafCells, persistentRecords._numberOfOuterLeafCells, persistentRecords._maxLevel, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._hasModifiedGridInPreviousIteration, persistentRecords._isTraversalInverted) {

}


dem::records::State::State(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
_persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, timeStepSize, currentTime, twoParticlesAreClose, minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, numberOfInnerLeafVertices, numberOfBoundaryLeafVertices, numberOfOuterLeafVertices, numberOfInnerLeafCells, numberOfOuterLeafCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted) {

}


dem::records::State::~State() { }



std::string dem::records::State::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void dem::records::State::toString (std::ostream& out) const {
out << "("; 
out << "numberOfContactPoints:" << getNumberOfContactPoints();
out << ",";
out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
out << ",";
out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
out << ",";
out << "timeStepSize:" << getTimeStepSize();
out << ",";
out << "currentTime:" << getCurrentTime();
out << ",";
out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
out << ",";
out << "minMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMinMeshWidth(i) << ",";
   }
   out << getMinMeshWidth(DIMENSIONS-1) << "]";
out << ",";
out << "maxMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMaxMeshWidth(i) << ",";
   }
   out << getMaxMeshWidth(DIMENSIONS-1) << "]";
out << ",";
out << "numberOfInnerVertices:" << getNumberOfInnerVertices();
out << ",";
out << "numberOfBoundaryVertices:" << getNumberOfBoundaryVertices();
out << ",";
out << "numberOfOuterVertices:" << getNumberOfOuterVertices();
out << ",";
out << "numberOfInnerCells:" << getNumberOfInnerCells();
out << ",";
out << "numberOfOuterCells:" << getNumberOfOuterCells();
out << ",";
out << "numberOfInnerLeafVertices:" << getNumberOfInnerLeafVertices();
out << ",";
out << "numberOfBoundaryLeafVertices:" << getNumberOfBoundaryLeafVertices();
out << ",";
out << "numberOfOuterLeafVertices:" << getNumberOfOuterLeafVertices();
out << ",";
out << "numberOfInnerLeafCells:" << getNumberOfInnerLeafCells();
out << ",";
out << "numberOfOuterLeafCells:" << getNumberOfOuterLeafCells();
out << ",";
out << "maxLevel:" << getMaxLevel();
out << ",";
out << "hasRefined:" << getHasRefined();
out << ",";
out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
out << ",";
out << "hasErased:" << getHasErased();
out << ",";
out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
out << ",";
out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
out << ",";
out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
out << ",";
out << "isTraversalInverted:" << getIsTraversalInverted();
out <<  ")";
}


dem::records::State::PersistentRecords dem::records::State::getPersistentRecords() const {
return _persistentRecords;
}

dem::records::StatePacked dem::records::State::convert() const{
return StatePacked(
getNumberOfContactPoints(),
getNumberOfParticleReassignments(),
getNumberOfTriangleComparisons(),
getTimeStepSize(),
getCurrentTime(),
getTwoParticlesAreClose(),
getMinMeshWidth(),
getMaxMeshWidth(),
getNumberOfInnerVertices(),
getNumberOfBoundaryVertices(),
getNumberOfOuterVertices(),
getNumberOfInnerCells(),
getNumberOfOuterCells(),
getNumberOfInnerLeafVertices(),
getNumberOfBoundaryLeafVertices(),
getNumberOfOuterLeafVertices(),
getNumberOfInnerLeafCells(),
getNumberOfOuterLeafCells(),
getMaxLevel(),
getHasRefined(),
getHasTriggeredRefinementForNextIteration(),
getHasErased(),
getHasTriggeredEraseForNextIteration(),
getHasChangedVertexOrCellState(),
getHasModifiedGridInPreviousIteration(),
getIsTraversalInverted()
);
}

#ifdef Parallel
tarch::logging::Log dem::records::State::_log( "dem::records::State" );

MPI_Datatype dem::records::State::Datatype = 0;
MPI_Datatype dem::records::State::FullDatatype = 0;


void dem::records::State::initDatatype() {
{
State dummyState[2];

const int Attributes = 27;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //numberOfContactPoints
MPI_DOUBLE,		 //numberOfParticleReassignments
MPI_DOUBLE,		 //numberOfTriangleComparisons
MPI_DOUBLE,		 //timeStepSize
MPI_DOUBLE,		 //currentTime
MPI_CHAR,		 //twoParticlesAreClose
MPI_DOUBLE,		 //minMeshWidth
MPI_DOUBLE,		 //maxMeshWidth
MPI_DOUBLE,		 //numberOfInnerVertices
MPI_DOUBLE,		 //numberOfBoundaryVertices
MPI_DOUBLE,		 //numberOfOuterVertices
MPI_DOUBLE,		 //numberOfInnerCells
MPI_DOUBLE,		 //numberOfOuterCells
MPI_DOUBLE,		 //numberOfInnerLeafVertices
MPI_DOUBLE,		 //numberOfBoundaryLeafVertices
MPI_DOUBLE,		 //numberOfOuterLeafVertices
MPI_DOUBLE,		 //numberOfInnerLeafCells
MPI_DOUBLE,		 //numberOfOuterLeafCells
MPI_INT,		 //maxLevel
MPI_CHAR,		 //hasRefined
MPI_CHAR,		 //hasTriggeredRefinementForNextIteration
MPI_CHAR,		 //hasErased
MPI_CHAR,		 //hasTriggeredEraseForNextIteration
MPI_CHAR,		 //hasChangedVertexOrCellState
MPI_CHAR,		 //hasModifiedGridInPreviousIteration
MPI_CHAR,		 //isTraversalInverted
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //numberOfContactPoints
1,		 //numberOfParticleReassignments
1,		 //numberOfTriangleComparisons
1,		 //timeStepSize
1,		 //currentTime
1,		 //twoParticlesAreClose
DIMENSIONS,		 //minMeshWidth
DIMENSIONS,		 //maxMeshWidth
1,		 //numberOfInnerVertices
1,		 //numberOfBoundaryVertices
1,		 //numberOfOuterVertices
1,		 //numberOfInnerCells
1,		 //numberOfOuterCells
1,		 //numberOfInnerLeafVertices
1,		 //numberOfBoundaryLeafVertices
1,		 //numberOfOuterLeafVertices
1,		 //numberOfInnerLeafCells
1,		 //numberOfOuterLeafCells
1,		 //maxLevel
1,		 //hasRefined
1,		 //hasTriggeredRefinementForNextIteration
1,		 //hasErased
1,		 //hasTriggeredEraseForNextIteration
1,		 //hasChangedVertexOrCellState
1,		 //hasModifiedGridInPreviousIteration
1,		 //isTraversalInverted
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[13] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[14] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[15] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[16] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[17] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[18] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[19] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[20] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[21] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[22] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[23] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[24] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[25] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]._persistentRecords._numberOfContactPoints))), 		&disp[26] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::Datatype );
MPI_Type_commit( &State::Datatype );

}
{
State dummyState[2];

const int Attributes = 27;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //numberOfContactPoints
MPI_DOUBLE,		 //numberOfParticleReassignments
MPI_DOUBLE,		 //numberOfTriangleComparisons
MPI_DOUBLE,		 //timeStepSize
MPI_DOUBLE,		 //currentTime
MPI_CHAR,		 //twoParticlesAreClose
MPI_DOUBLE,		 //minMeshWidth
MPI_DOUBLE,		 //maxMeshWidth
MPI_DOUBLE,		 //numberOfInnerVertices
MPI_DOUBLE,		 //numberOfBoundaryVertices
MPI_DOUBLE,		 //numberOfOuterVertices
MPI_DOUBLE,		 //numberOfInnerCells
MPI_DOUBLE,		 //numberOfOuterCells
MPI_DOUBLE,		 //numberOfInnerLeafVertices
MPI_DOUBLE,		 //numberOfBoundaryLeafVertices
MPI_DOUBLE,		 //numberOfOuterLeafVertices
MPI_DOUBLE,		 //numberOfInnerLeafCells
MPI_DOUBLE,		 //numberOfOuterLeafCells
MPI_INT,		 //maxLevel
MPI_CHAR,		 //hasRefined
MPI_CHAR,		 //hasTriggeredRefinementForNextIteration
MPI_CHAR,		 //hasErased
MPI_CHAR,		 //hasTriggeredEraseForNextIteration
MPI_CHAR,		 //hasChangedVertexOrCellState
MPI_CHAR,		 //hasModifiedGridInPreviousIteration
MPI_CHAR,		 //isTraversalInverted
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //numberOfContactPoints
1,		 //numberOfParticleReassignments
1,		 //numberOfTriangleComparisons
1,		 //timeStepSize
1,		 //currentTime
1,		 //twoParticlesAreClose
DIMENSIONS,		 //minMeshWidth
DIMENSIONS,		 //maxMeshWidth
1,		 //numberOfInnerVertices
1,		 //numberOfBoundaryVertices
1,		 //numberOfOuterVertices
1,		 //numberOfInnerCells
1,		 //numberOfOuterCells
1,		 //numberOfInnerLeafVertices
1,		 //numberOfBoundaryLeafVertices
1,		 //numberOfOuterLeafVertices
1,		 //numberOfInnerLeafCells
1,		 //numberOfOuterLeafCells
1,		 //maxLevel
1,		 //hasRefined
1,		 //hasTriggeredRefinementForNextIteration
1,		 //hasErased
1,		 //hasTriggeredEraseForNextIteration
1,		 //hasChangedVertexOrCellState
1,		 //hasModifiedGridInPreviousIteration
1,		 //isTraversalInverted
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[13] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[14] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[15] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[16] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[17] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[18] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[19] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[20] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[21] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[22] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[23] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[24] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[25] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]._persistentRecords._numberOfContactPoints))), 		&disp[26] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::FullDatatype );
MPI_Type_commit( &State::FullDatatype );

}

}


void dem::records::State::shutdownDatatype() {
MPI_Type_free( &State::Datatype );
MPI_Type_free( &State::FullDatatype );

}

void dem::records::State::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message dem::records::State "
<< toString()
<< " to node " << destination
<< ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "send(int)",msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
result = MPI_Isend(
this, 1, Datatype, destination,
tag, tarch::parallel::Node::getInstance().getCommunicator(),
sendRequestHandle
);

}
else {
result = MPI_Isend(
this, 1, FullDatatype, destination,
tag, tarch::parallel::Node::getInstance().getCommunicator(),
sendRequestHandle
);

}
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message dem::records::State "
<< toString()
<< " to node " << destination
<< ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "send(int)",msg.str() );
}
result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
result = MPI_Test( sendRequestHandle, &flag, &status );
if (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "testing for finished send task for dem::records::State "
<< toString()
<< " sent to node " << destination
<< " failed: " << tarch::parallel::MPIReturnValueToString(result);
_log.error("send(int)", msg.str() );
}

// deadlock aspect
if (
tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
(clock()>timeOutWarning) &&
(!triggeredTimeoutWarning)
) {
tarch::parallel::Node::getInstance().writeTimeOutWarning(
"dem::records::State",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"dem::records::State",
"send(int)", destination,tag,1
);
}
tarch::parallel::Node::getInstance().receiveDanglingMessages();
usleep(communicateSleep);

}

delete sendRequestHandle;
#ifdef Debug
_log.debug("send(int,int)", "sent " + toString() );
#endif

}

}



void dem::records::State::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive dem::records::State from node "
<< source << ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "receive(int)", msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
result = MPI_Irecv(
this, 1, Datatype, source, tag,
tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
);

}
else {
result = MPI_Irecv(
this, 1, FullDatatype, source, tag,
tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
);

}
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive dem::records::State from node "
<< source << ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "receive(int)", msg.str() );
}

result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
result = MPI_Test( sendRequestHandle, &flag, &status );
if (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "testing for finished receive task for dem::records::State failed: "
<< tarch::parallel::MPIReturnValueToString(result);
_log.error("receive(int)", msg.str() );
}

// deadlock aspect
if (
tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
(clock()>timeOutWarning) &&
(!triggeredTimeoutWarning)
) {
tarch::parallel::Node::getInstance().writeTimeOutWarning(
"dem::records::State",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"dem::records::State",
"receive(int)", source,tag,1
);
}
tarch::parallel::Node::getInstance().receiveDanglingMessages();
usleep(communicateSleep);

}

delete sendRequestHandle;

_senderDestinationRank = status.MPI_SOURCE;
#ifdef Debug
_log.debug("receive(int,int)", "received " + toString() ); 
#endif

}

}



bool dem::records::State::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Status status;
int  flag        = 0;
MPI_Iprobe(
MPI_ANY_SOURCE, tag,
tarch::parallel::Node::getInstance().getCommunicator(), &flag, &status
);
if (flag) {
int  messageCounter;
if (exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Get_count(&status, Datatype, &messageCounter);
}
else {
MPI_Get_count(&status, FullDatatype, &messageCounter);
}
return messageCounter > 0;
}
else return false;

}

int dem::records::State::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


dem::records::StatePacked::PersistentRecords::PersistentRecords() {
if ((6 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(short int))));

}


dem::records::StatePacked::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
_numberOfContactPoints(numberOfContactPoints),
_numberOfParticleReassignments(numberOfParticleReassignments),
_numberOfTriangleComparisons(numberOfTriangleComparisons),
_timeStepSize(timeStepSize),
_currentTime(currentTime),
_twoParticlesAreClose(twoParticlesAreClose),
_minMeshWidth(minMeshWidth),
_maxMeshWidth(maxMeshWidth),
_numberOfInnerVertices(numberOfInnerVertices),
_numberOfBoundaryVertices(numberOfBoundaryVertices),
_numberOfOuterVertices(numberOfOuterVertices),
_numberOfInnerCells(numberOfInnerCells),
_numberOfOuterCells(numberOfOuterCells),
_numberOfInnerLeafVertices(numberOfInnerLeafVertices),
_numberOfBoundaryLeafVertices(numberOfBoundaryLeafVertices),
_numberOfOuterLeafVertices(numberOfOuterLeafVertices),
_numberOfInnerLeafCells(numberOfInnerLeafCells),
_numberOfOuterLeafCells(numberOfOuterLeafCells),
_maxLevel(maxLevel),
_isTraversalInverted(isTraversalInverted) {
setHasRefined(hasRefined);
setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
setHasErased(hasErased);
setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
setHasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration);
if ((6 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(short int))));

}

dem::records::StatePacked::StatePacked() {
if ((6 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(short int))));

}


dem::records::StatePacked::StatePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._timeStepSize, persistentRecords._currentTime, persistentRecords._twoParticlesAreClose, persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._numberOfInnerLeafVertices, persistentRecords._numberOfBoundaryLeafVertices, persistentRecords._numberOfOuterLeafVertices, persistentRecords._numberOfInnerLeafCells, persistentRecords._numberOfOuterLeafCells, persistentRecords._maxLevel, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords.getHasModifiedGridInPreviousIteration(), persistentRecords._isTraversalInverted) {
if ((6 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(short int))));

}


dem::records::StatePacked::StatePacked(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
_persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, timeStepSize, currentTime, twoParticlesAreClose, minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, numberOfInnerLeafVertices, numberOfBoundaryLeafVertices, numberOfOuterLeafVertices, numberOfInnerLeafCells, numberOfOuterLeafCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted) {
if ((6 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(short int))));

}


dem::records::StatePacked::~StatePacked() { }



std::string dem::records::StatePacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void dem::records::StatePacked::toString (std::ostream& out) const {
out << "("; 
out << "numberOfContactPoints:" << getNumberOfContactPoints();
out << ",";
out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
out << ",";
out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
out << ",";
out << "timeStepSize:" << getTimeStepSize();
out << ",";
out << "currentTime:" << getCurrentTime();
out << ",";
out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
out << ",";
out << "minMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMinMeshWidth(i) << ",";
   }
   out << getMinMeshWidth(DIMENSIONS-1) << "]";
out << ",";
out << "maxMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMaxMeshWidth(i) << ",";
   }
   out << getMaxMeshWidth(DIMENSIONS-1) << "]";
out << ",";
out << "numberOfInnerVertices:" << getNumberOfInnerVertices();
out << ",";
out << "numberOfBoundaryVertices:" << getNumberOfBoundaryVertices();
out << ",";
out << "numberOfOuterVertices:" << getNumberOfOuterVertices();
out << ",";
out << "numberOfInnerCells:" << getNumberOfInnerCells();
out << ",";
out << "numberOfOuterCells:" << getNumberOfOuterCells();
out << ",";
out << "numberOfInnerLeafVertices:" << getNumberOfInnerLeafVertices();
out << ",";
out << "numberOfBoundaryLeafVertices:" << getNumberOfBoundaryLeafVertices();
out << ",";
out << "numberOfOuterLeafVertices:" << getNumberOfOuterLeafVertices();
out << ",";
out << "numberOfInnerLeafCells:" << getNumberOfInnerLeafCells();
out << ",";
out << "numberOfOuterLeafCells:" << getNumberOfOuterLeafCells();
out << ",";
out << "maxLevel:" << getMaxLevel();
out << ",";
out << "hasRefined:" << getHasRefined();
out << ",";
out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
out << ",";
out << "hasErased:" << getHasErased();
out << ",";
out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
out << ",";
out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
out << ",";
out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
out << ",";
out << "isTraversalInverted:" << getIsTraversalInverted();
out <<  ")";
}


dem::records::StatePacked::PersistentRecords dem::records::StatePacked::getPersistentRecords() const {
return _persistentRecords;
}

dem::records::State dem::records::StatePacked::convert() const{
return State(
getNumberOfContactPoints(),
getNumberOfParticleReassignments(),
getNumberOfTriangleComparisons(),
getTimeStepSize(),
getCurrentTime(),
getTwoParticlesAreClose(),
getMinMeshWidth(),
getMaxMeshWidth(),
getNumberOfInnerVertices(),
getNumberOfBoundaryVertices(),
getNumberOfOuterVertices(),
getNumberOfInnerCells(),
getNumberOfOuterCells(),
getNumberOfInnerLeafVertices(),
getNumberOfBoundaryLeafVertices(),
getNumberOfOuterLeafVertices(),
getNumberOfInnerLeafCells(),
getNumberOfOuterLeafCells(),
getMaxLevel(),
getHasRefined(),
getHasTriggeredRefinementForNextIteration(),
getHasErased(),
getHasTriggeredEraseForNextIteration(),
getHasChangedVertexOrCellState(),
getHasModifiedGridInPreviousIteration(),
getIsTraversalInverted()
);
}

#ifdef Parallel
tarch::logging::Log dem::records::StatePacked::_log( "dem::records::StatePacked" );

MPI_Datatype dem::records::StatePacked::Datatype = 0;
MPI_Datatype dem::records::StatePacked::FullDatatype = 0;


void dem::records::StatePacked::initDatatype() {
{
StatePacked dummyStatePacked[2];

const int Attributes = 22;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //numberOfContactPoints
MPI_DOUBLE,		 //numberOfParticleReassignments
MPI_DOUBLE,		 //numberOfTriangleComparisons
MPI_DOUBLE,		 //timeStepSize
MPI_DOUBLE,		 //currentTime
MPI_CHAR,		 //twoParticlesAreClose
MPI_DOUBLE,		 //minMeshWidth
MPI_DOUBLE,		 //maxMeshWidth
MPI_DOUBLE,		 //numberOfInnerVertices
MPI_DOUBLE,		 //numberOfBoundaryVertices
MPI_DOUBLE,		 //numberOfOuterVertices
MPI_DOUBLE,		 //numberOfInnerCells
MPI_DOUBLE,		 //numberOfOuterCells
MPI_DOUBLE,		 //numberOfInnerLeafVertices
MPI_DOUBLE,		 //numberOfBoundaryLeafVertices
MPI_DOUBLE,		 //numberOfOuterLeafVertices
MPI_DOUBLE,		 //numberOfInnerLeafCells
MPI_DOUBLE,		 //numberOfOuterLeafCells
MPI_INT,		 //maxLevel
MPI_CHAR,		 //isTraversalInverted
MPI_SHORT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //numberOfContactPoints
1,		 //numberOfParticleReassignments
1,		 //numberOfTriangleComparisons
1,		 //timeStepSize
1,		 //currentTime
1,		 //twoParticlesAreClose
DIMENSIONS,		 //minMeshWidth
DIMENSIONS,		 //maxMeshWidth
1,		 //numberOfInnerVertices
1,		 //numberOfBoundaryVertices
1,		 //numberOfOuterVertices
1,		 //numberOfInnerCells
1,		 //numberOfOuterCells
1,		 //numberOfInnerLeafVertices
1,		 //numberOfBoundaryLeafVertices
1,		 //numberOfOuterLeafVertices
1,		 //numberOfInnerLeafCells
1,		 //numberOfOuterLeafCells
1,		 //maxLevel
1,		 //isTraversalInverted
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[13] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[14] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[15] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[16] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[17] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[18] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[19] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[20] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]._persistentRecords._numberOfContactPoints))), 		&disp[21] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::Datatype );
MPI_Type_commit( &StatePacked::Datatype );

}
{
StatePacked dummyStatePacked[2];

const int Attributes = 22;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //numberOfContactPoints
MPI_DOUBLE,		 //numberOfParticleReassignments
MPI_DOUBLE,		 //numberOfTriangleComparisons
MPI_DOUBLE,		 //timeStepSize
MPI_DOUBLE,		 //currentTime
MPI_CHAR,		 //twoParticlesAreClose
MPI_DOUBLE,		 //minMeshWidth
MPI_DOUBLE,		 //maxMeshWidth
MPI_DOUBLE,		 //numberOfInnerVertices
MPI_DOUBLE,		 //numberOfBoundaryVertices
MPI_DOUBLE,		 //numberOfOuterVertices
MPI_DOUBLE,		 //numberOfInnerCells
MPI_DOUBLE,		 //numberOfOuterCells
MPI_DOUBLE,		 //numberOfInnerLeafVertices
MPI_DOUBLE,		 //numberOfBoundaryLeafVertices
MPI_DOUBLE,		 //numberOfOuterLeafVertices
MPI_DOUBLE,		 //numberOfInnerLeafCells
MPI_DOUBLE,		 //numberOfOuterLeafCells
MPI_INT,		 //maxLevel
MPI_CHAR,		 //isTraversalInverted
MPI_SHORT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //numberOfContactPoints
1,		 //numberOfParticleReassignments
1,		 //numberOfTriangleComparisons
1,		 //timeStepSize
1,		 //currentTime
1,		 //twoParticlesAreClose
DIMENSIONS,		 //minMeshWidth
DIMENSIONS,		 //maxMeshWidth
1,		 //numberOfInnerVertices
1,		 //numberOfBoundaryVertices
1,		 //numberOfOuterVertices
1,		 //numberOfInnerCells
1,		 //numberOfOuterCells
1,		 //numberOfInnerLeafVertices
1,		 //numberOfBoundaryLeafVertices
1,		 //numberOfOuterLeafVertices
1,		 //numberOfInnerLeafCells
1,		 //numberOfOuterLeafCells
1,		 //maxLevel
1,		 //isTraversalInverted
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[13] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[14] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[15] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[16] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[17] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[18] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[19] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[20] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]._persistentRecords._numberOfContactPoints))), 		&disp[21] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::FullDatatype );
MPI_Type_commit( &StatePacked::FullDatatype );

}

}


void dem::records::StatePacked::shutdownDatatype() {
MPI_Type_free( &StatePacked::Datatype );
MPI_Type_free( &StatePacked::FullDatatype );

}

void dem::records::StatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message dem::records::StatePacked "
<< toString()
<< " to node " << destination
<< ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "send(int)",msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
result = MPI_Isend(
this, 1, Datatype, destination,
tag, tarch::parallel::Node::getInstance().getCommunicator(),
sendRequestHandle
);

}
else {
result = MPI_Isend(
this, 1, FullDatatype, destination,
tag, tarch::parallel::Node::getInstance().getCommunicator(),
sendRequestHandle
);

}
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message dem::records::StatePacked "
<< toString()
<< " to node " << destination
<< ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "send(int)",msg.str() );
}
result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
result = MPI_Test( sendRequestHandle, &flag, &status );
if (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "testing for finished send task for dem::records::StatePacked "
<< toString()
<< " sent to node " << destination
<< " failed: " << tarch::parallel::MPIReturnValueToString(result);
_log.error("send(int)", msg.str() );
}

// deadlock aspect
if (
tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
(clock()>timeOutWarning) &&
(!triggeredTimeoutWarning)
) {
tarch::parallel::Node::getInstance().writeTimeOutWarning(
"dem::records::StatePacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"dem::records::StatePacked",
"send(int)", destination,tag,1
);
}
tarch::parallel::Node::getInstance().receiveDanglingMessages();
usleep(communicateSleep);

}

delete sendRequestHandle;
#ifdef Debug
_log.debug("send(int,int)", "sent " + toString() );
#endif

}

}



void dem::records::StatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive dem::records::StatePacked from node "
<< source << ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "receive(int)", msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
result = MPI_Irecv(
this, 1, Datatype, source, tag,
tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
);

}
else {
result = MPI_Irecv(
this, 1, FullDatatype, source, tag,
tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
);

}
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive dem::records::StatePacked from node "
<< source << ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "receive(int)", msg.str() );
}

result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
result = MPI_Test( sendRequestHandle, &flag, &status );
if (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "testing for finished receive task for dem::records::StatePacked failed: "
<< tarch::parallel::MPIReturnValueToString(result);
_log.error("receive(int)", msg.str() );
}

// deadlock aspect
if (
tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
(clock()>timeOutWarning) &&
(!triggeredTimeoutWarning)
) {
tarch::parallel::Node::getInstance().writeTimeOutWarning(
"dem::records::StatePacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"dem::records::StatePacked",
"receive(int)", source,tag,1
);
}
tarch::parallel::Node::getInstance().receiveDanglingMessages();
usleep(communicateSleep);

}

delete sendRequestHandle;

_senderDestinationRank = status.MPI_SOURCE;
#ifdef Debug
_log.debug("receive(int,int)", "received " + toString() ); 
#endif

}

}



bool dem::records::StatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Status status;
int  flag        = 0;
MPI_Iprobe(
MPI_ANY_SOURCE, tag,
tarch::parallel::Node::getInstance().getCommunicator(), &flag, &status
);
if (flag) {
int  messageCounter;
if (exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Get_count(&status, Datatype, &messageCounter);
}
else {
MPI_Get_count(&status, FullDatatype, &messageCounter);
}
return messageCounter > 0;
}
else return false;

}

int dem::records::StatePacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif !defined(TrackGridStatistics) && defined(Parallel)
dem::records::State::PersistentRecords::PersistentRecords() {

}


dem::records::State::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
_numberOfContactPoints(numberOfContactPoints),
_numberOfParticleReassignments(numberOfParticleReassignments),
_numberOfTriangleComparisons(numberOfTriangleComparisons),
_timeStepSize(timeStepSize),
_currentTime(currentTime),
_twoParticlesAreClose(twoParticlesAreClose),
_hasRefined(hasRefined),
_hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
_hasErased(hasErased),
_hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
_hasChangedVertexOrCellState(hasChangedVertexOrCellState),
_hasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration),
_isTraversalInverted(isTraversalInverted),
_reduceStateAndCell(reduceStateAndCell),
_couldNotEraseDueToDecompositionFlag(couldNotEraseDueToDecompositionFlag),
_subWorkerIsInvolvedInJoinOrFork(subWorkerIsInvolvedInJoinOrFork) {

}

dem::records::State::State() {

}


dem::records::State::State(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._timeStepSize, persistentRecords._currentTime, persistentRecords._twoParticlesAreClose, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._hasModifiedGridInPreviousIteration, persistentRecords._isTraversalInverted, persistentRecords._reduceStateAndCell, persistentRecords._couldNotEraseDueToDecompositionFlag, persistentRecords._subWorkerIsInvolvedInJoinOrFork) {

}


dem::records::State::State(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
_persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, timeStepSize, currentTime, twoParticlesAreClose, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted, reduceStateAndCell, couldNotEraseDueToDecompositionFlag, subWorkerIsInvolvedInJoinOrFork) {

}


dem::records::State::~State() { }



std::string dem::records::State::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void dem::records::State::toString (std::ostream& out) const {
out << "("; 
out << "numberOfContactPoints:" << getNumberOfContactPoints();
out << ",";
out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
out << ",";
out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
out << ",";
out << "timeStepSize:" << getTimeStepSize();
out << ",";
out << "currentTime:" << getCurrentTime();
out << ",";
out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
out << ",";
out << "hasRefined:" << getHasRefined();
out << ",";
out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
out << ",";
out << "hasErased:" << getHasErased();
out << ",";
out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
out << ",";
out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
out << ",";
out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
out << ",";
out << "isTraversalInverted:" << getIsTraversalInverted();
out << ",";
out << "reduceStateAndCell:" << getReduceStateAndCell();
out << ",";
out << "couldNotEraseDueToDecompositionFlag:" << getCouldNotEraseDueToDecompositionFlag();
out << ",";
out << "subWorkerIsInvolvedInJoinOrFork:" << getSubWorkerIsInvolvedInJoinOrFork();
out <<  ")";
}


dem::records::State::PersistentRecords dem::records::State::getPersistentRecords() const {
return _persistentRecords;
}

dem::records::StatePacked dem::records::State::convert() const{
return StatePacked(
getNumberOfContactPoints(),
getNumberOfParticleReassignments(),
getNumberOfTriangleComparisons(),
getTimeStepSize(),
getCurrentTime(),
getTwoParticlesAreClose(),
getHasRefined(),
getHasTriggeredRefinementForNextIteration(),
getHasErased(),
getHasTriggeredEraseForNextIteration(),
getHasChangedVertexOrCellState(),
getHasModifiedGridInPreviousIteration(),
getIsTraversalInverted(),
getReduceStateAndCell(),
getCouldNotEraseDueToDecompositionFlag(),
getSubWorkerIsInvolvedInJoinOrFork()
);
}

#ifdef Parallel
tarch::logging::Log dem::records::State::_log( "dem::records::State" );

MPI_Datatype dem::records::State::Datatype = 0;
MPI_Datatype dem::records::State::FullDatatype = 0;


void dem::records::State::initDatatype() {
{
State dummyState[2];

const int Attributes = 17;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //numberOfContactPoints
MPI_DOUBLE,		 //numberOfParticleReassignments
MPI_DOUBLE,		 //numberOfTriangleComparisons
MPI_DOUBLE,		 //timeStepSize
MPI_DOUBLE,		 //currentTime
MPI_CHAR,		 //twoParticlesAreClose
MPI_CHAR,		 //hasRefined
MPI_CHAR,		 //hasTriggeredRefinementForNextIteration
MPI_CHAR,		 //hasErased
MPI_CHAR,		 //hasTriggeredEraseForNextIteration
MPI_CHAR,		 //hasChangedVertexOrCellState
MPI_CHAR,		 //hasModifiedGridInPreviousIteration
MPI_CHAR,		 //isTraversalInverted
MPI_CHAR,		 //reduceStateAndCell
MPI_CHAR,		 //couldNotEraseDueToDecompositionFlag
MPI_CHAR,		 //subWorkerIsInvolvedInJoinOrFork
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //numberOfContactPoints
1,		 //numberOfParticleReassignments
1,		 //numberOfTriangleComparisons
1,		 //timeStepSize
1,		 //currentTime
1,		 //twoParticlesAreClose
1,		 //hasRefined
1,		 //hasTriggeredRefinementForNextIteration
1,		 //hasErased
1,		 //hasTriggeredEraseForNextIteration
1,		 //hasChangedVertexOrCellState
1,		 //hasModifiedGridInPreviousIteration
1,		 //isTraversalInverted
1,		 //reduceStateAndCell
1,		 //couldNotEraseDueToDecompositionFlag
1,		 //subWorkerIsInvolvedInJoinOrFork
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[13] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[14] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[15] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]._persistentRecords._numberOfContactPoints))), 		&disp[16] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::Datatype );
MPI_Type_commit( &State::Datatype );

}
{
State dummyState[2];

const int Attributes = 17;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //numberOfContactPoints
MPI_DOUBLE,		 //numberOfParticleReassignments
MPI_DOUBLE,		 //numberOfTriangleComparisons
MPI_DOUBLE,		 //timeStepSize
MPI_DOUBLE,		 //currentTime
MPI_CHAR,		 //twoParticlesAreClose
MPI_CHAR,		 //hasRefined
MPI_CHAR,		 //hasTriggeredRefinementForNextIteration
MPI_CHAR,		 //hasErased
MPI_CHAR,		 //hasTriggeredEraseForNextIteration
MPI_CHAR,		 //hasChangedVertexOrCellState
MPI_CHAR,		 //hasModifiedGridInPreviousIteration
MPI_CHAR,		 //isTraversalInverted
MPI_CHAR,		 //reduceStateAndCell
MPI_CHAR,		 //couldNotEraseDueToDecompositionFlag
MPI_CHAR,		 //subWorkerIsInvolvedInJoinOrFork
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //numberOfContactPoints
1,		 //numberOfParticleReassignments
1,		 //numberOfTriangleComparisons
1,		 //timeStepSize
1,		 //currentTime
1,		 //twoParticlesAreClose
1,		 //hasRefined
1,		 //hasTriggeredRefinementForNextIteration
1,		 //hasErased
1,		 //hasTriggeredEraseForNextIteration
1,		 //hasChangedVertexOrCellState
1,		 //hasModifiedGridInPreviousIteration
1,		 //isTraversalInverted
1,		 //reduceStateAndCell
1,		 //couldNotEraseDueToDecompositionFlag
1,		 //subWorkerIsInvolvedInJoinOrFork
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[13] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[14] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[15] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]._persistentRecords._numberOfContactPoints))), 		&disp[16] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::FullDatatype );
MPI_Type_commit( &State::FullDatatype );

}

}


void dem::records::State::shutdownDatatype() {
MPI_Type_free( &State::Datatype );
MPI_Type_free( &State::FullDatatype );

}

void dem::records::State::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message dem::records::State "
<< toString()
<< " to node " << destination
<< ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "send(int)",msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
result = MPI_Isend(
this, 1, Datatype, destination,
tag, tarch::parallel::Node::getInstance().getCommunicator(),
sendRequestHandle
);

}
else {
result = MPI_Isend(
this, 1, FullDatatype, destination,
tag, tarch::parallel::Node::getInstance().getCommunicator(),
sendRequestHandle
);

}
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message dem::records::State "
<< toString()
<< " to node " << destination
<< ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "send(int)",msg.str() );
}
result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
result = MPI_Test( sendRequestHandle, &flag, &status );
if (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "testing for finished send task for dem::records::State "
<< toString()
<< " sent to node " << destination
<< " failed: " << tarch::parallel::MPIReturnValueToString(result);
_log.error("send(int)", msg.str() );
}

// deadlock aspect
if (
tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
(clock()>timeOutWarning) &&
(!triggeredTimeoutWarning)
) {
tarch::parallel::Node::getInstance().writeTimeOutWarning(
"dem::records::State",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"dem::records::State",
"send(int)", destination,tag,1
);
}
tarch::parallel::Node::getInstance().receiveDanglingMessages();
usleep(communicateSleep);

}

delete sendRequestHandle;
#ifdef Debug
_log.debug("send(int,int)", "sent " + toString() );
#endif

}

}



void dem::records::State::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive dem::records::State from node "
<< source << ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "receive(int)", msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
result = MPI_Irecv(
this, 1, Datatype, source, tag,
tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
);

}
else {
result = MPI_Irecv(
this, 1, FullDatatype, source, tag,
tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
);

}
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive dem::records::State from node "
<< source << ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "receive(int)", msg.str() );
}

result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
result = MPI_Test( sendRequestHandle, &flag, &status );
if (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "testing for finished receive task for dem::records::State failed: "
<< tarch::parallel::MPIReturnValueToString(result);
_log.error("receive(int)", msg.str() );
}

// deadlock aspect
if (
tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
(clock()>timeOutWarning) &&
(!triggeredTimeoutWarning)
) {
tarch::parallel::Node::getInstance().writeTimeOutWarning(
"dem::records::State",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"dem::records::State",
"receive(int)", source,tag,1
);
}
tarch::parallel::Node::getInstance().receiveDanglingMessages();
usleep(communicateSleep);

}

delete sendRequestHandle;

_senderDestinationRank = status.MPI_SOURCE;
#ifdef Debug
_log.debug("receive(int,int)", "received " + toString() ); 
#endif

}

}



bool dem::records::State::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Status status;
int  flag        = 0;
MPI_Iprobe(
MPI_ANY_SOURCE, tag,
tarch::parallel::Node::getInstance().getCommunicator(), &flag, &status
);
if (flag) {
int  messageCounter;
if (exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Get_count(&status, Datatype, &messageCounter);
}
else {
MPI_Get_count(&status, FullDatatype, &messageCounter);
}
return messageCounter > 0;
}
else return false;

}

int dem::records::State::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


dem::records::StatePacked::PersistentRecords::PersistentRecords() {
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}


dem::records::StatePacked::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
_numberOfContactPoints(numberOfContactPoints),
_numberOfParticleReassignments(numberOfParticleReassignments),
_numberOfTriangleComparisons(numberOfTriangleComparisons),
_timeStepSize(timeStepSize),
_currentTime(currentTime),
_twoParticlesAreClose(twoParticlesAreClose),
_isTraversalInverted(isTraversalInverted) {
setHasRefined(hasRefined);
setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
setHasErased(hasErased);
setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
setHasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration);
setReduceStateAndCell(reduceStateAndCell);
setCouldNotEraseDueToDecompositionFlag(couldNotEraseDueToDecompositionFlag);
setSubWorkerIsInvolvedInJoinOrFork(subWorkerIsInvolvedInJoinOrFork);
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}

dem::records::StatePacked::StatePacked() {
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}


dem::records::StatePacked::StatePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._timeStepSize, persistentRecords._currentTime, persistentRecords._twoParticlesAreClose, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords.getHasModifiedGridInPreviousIteration(), persistentRecords._isTraversalInverted, persistentRecords.getReduceStateAndCell(), persistentRecords.getCouldNotEraseDueToDecompositionFlag(), persistentRecords.getSubWorkerIsInvolvedInJoinOrFork()) {
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}


dem::records::StatePacked::StatePacked(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& timeStepSize, const double& currentTime, const bool& twoParticlesAreClose, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
_persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, timeStepSize, currentTime, twoParticlesAreClose, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted, reduceStateAndCell, couldNotEraseDueToDecompositionFlag, subWorkerIsInvolvedInJoinOrFork) {
if ((9 >= (8 * sizeof(short int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((9 < (8 * sizeof(short int))));

}


dem::records::StatePacked::~StatePacked() { }



std::string dem::records::StatePacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void dem::records::StatePacked::toString (std::ostream& out) const {
out << "("; 
out << "numberOfContactPoints:" << getNumberOfContactPoints();
out << ",";
out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
out << ",";
out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
out << ",";
out << "timeStepSize:" << getTimeStepSize();
out << ",";
out << "currentTime:" << getCurrentTime();
out << ",";
out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
out << ",";
out << "hasRefined:" << getHasRefined();
out << ",";
out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
out << ",";
out << "hasErased:" << getHasErased();
out << ",";
out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
out << ",";
out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
out << ",";
out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
out << ",";
out << "isTraversalInverted:" << getIsTraversalInverted();
out << ",";
out << "reduceStateAndCell:" << getReduceStateAndCell();
out << ",";
out << "couldNotEraseDueToDecompositionFlag:" << getCouldNotEraseDueToDecompositionFlag();
out << ",";
out << "subWorkerIsInvolvedInJoinOrFork:" << getSubWorkerIsInvolvedInJoinOrFork();
out <<  ")";
}


dem::records::StatePacked::PersistentRecords dem::records::StatePacked::getPersistentRecords() const {
return _persistentRecords;
}

dem::records::State dem::records::StatePacked::convert() const{
return State(
getNumberOfContactPoints(),
getNumberOfParticleReassignments(),
getNumberOfTriangleComparisons(),
getTimeStepSize(),
getCurrentTime(),
getTwoParticlesAreClose(),
getHasRefined(),
getHasTriggeredRefinementForNextIteration(),
getHasErased(),
getHasTriggeredEraseForNextIteration(),
getHasChangedVertexOrCellState(),
getHasModifiedGridInPreviousIteration(),
getIsTraversalInverted(),
getReduceStateAndCell(),
getCouldNotEraseDueToDecompositionFlag(),
getSubWorkerIsInvolvedInJoinOrFork()
);
}

#ifdef Parallel
tarch::logging::Log dem::records::StatePacked::_log( "dem::records::StatePacked" );

MPI_Datatype dem::records::StatePacked::Datatype = 0;
MPI_Datatype dem::records::StatePacked::FullDatatype = 0;


void dem::records::StatePacked::initDatatype() {
{
StatePacked dummyStatePacked[2];

const int Attributes = 9;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //numberOfContactPoints
MPI_DOUBLE,		 //numberOfParticleReassignments
MPI_DOUBLE,		 //numberOfTriangleComparisons
MPI_DOUBLE,		 //timeStepSize
MPI_DOUBLE,		 //currentTime
MPI_CHAR,		 //twoParticlesAreClose
MPI_CHAR,		 //isTraversalInverted
MPI_SHORT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //numberOfContactPoints
1,		 //numberOfParticleReassignments
1,		 //numberOfTriangleComparisons
1,		 //timeStepSize
1,		 //currentTime
1,		 //twoParticlesAreClose
1,		 //isTraversalInverted
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]._persistentRecords._numberOfContactPoints))), 		&disp[8] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::Datatype );
MPI_Type_commit( &StatePacked::Datatype );

}
{
StatePacked dummyStatePacked[2];

const int Attributes = 9;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //numberOfContactPoints
MPI_DOUBLE,		 //numberOfParticleReassignments
MPI_DOUBLE,		 //numberOfTriangleComparisons
MPI_DOUBLE,		 //timeStepSize
MPI_DOUBLE,		 //currentTime
MPI_CHAR,		 //twoParticlesAreClose
MPI_CHAR,		 //isTraversalInverted
MPI_SHORT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //numberOfContactPoints
1,		 //numberOfParticleReassignments
1,		 //numberOfTriangleComparisons
1,		 //timeStepSize
1,		 //currentTime
1,		 //twoParticlesAreClose
1,		 //isTraversalInverted
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]._persistentRecords._numberOfContactPoints))), 		&disp[8] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::FullDatatype );
MPI_Type_commit( &StatePacked::FullDatatype );

}

}


void dem::records::StatePacked::shutdownDatatype() {
MPI_Type_free( &StatePacked::Datatype );
MPI_Type_free( &StatePacked::FullDatatype );

}

void dem::records::StatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message dem::records::StatePacked "
<< toString()
<< " to node " << destination
<< ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "send(int)",msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
result = MPI_Isend(
this, 1, Datatype, destination,
tag, tarch::parallel::Node::getInstance().getCommunicator(),
sendRequestHandle
);

}
else {
result = MPI_Isend(
this, 1, FullDatatype, destination,
tag, tarch::parallel::Node::getInstance().getCommunicator(),
sendRequestHandle
);

}
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message dem::records::StatePacked "
<< toString()
<< " to node " << destination
<< ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "send(int)",msg.str() );
}
result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
result = MPI_Test( sendRequestHandle, &flag, &status );
if (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "testing for finished send task for dem::records::StatePacked "
<< toString()
<< " sent to node " << destination
<< " failed: " << tarch::parallel::MPIReturnValueToString(result);
_log.error("send(int)", msg.str() );
}

// deadlock aspect
if (
tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
(clock()>timeOutWarning) &&
(!triggeredTimeoutWarning)
) {
tarch::parallel::Node::getInstance().writeTimeOutWarning(
"dem::records::StatePacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"dem::records::StatePacked",
"send(int)", destination,tag,1
);
}
tarch::parallel::Node::getInstance().receiveDanglingMessages();
usleep(communicateSleep);

}

delete sendRequestHandle;
#ifdef Debug
_log.debug("send(int,int)", "sent " + toString() );
#endif

}

}



void dem::records::StatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive dem::records::StatePacked from node "
<< source << ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "receive(int)", msg.str() );
}

}
else {

MPI_Request* sendRequestHandle = new MPI_Request();
MPI_Status   status;
int          flag = 0;
int          result;

clock_t      timeOutWarning   = -1;
clock_t      timeOutShutdown  = -1;
bool         triggeredTimeoutWarning = false;

if (exchangeOnlyAttributesMarkedWithParallelise) {
result = MPI_Irecv(
this, 1, Datatype, source, tag,
tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
);

}
else {
result = MPI_Irecv(
this, 1, FullDatatype, source, tag,
tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
);

}
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive dem::records::StatePacked from node "
<< source << ": " << tarch::parallel::MPIReturnValueToString(result);
_log.error( "receive(int)", msg.str() );
}

result = MPI_Test( sendRequestHandle, &flag, &status );
while (!flag) {
if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
result = MPI_Test( sendRequestHandle, &flag, &status );
if (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "testing for finished receive task for dem::records::StatePacked failed: "
<< tarch::parallel::MPIReturnValueToString(result);
_log.error("receive(int)", msg.str() );
}

// deadlock aspect
if (
tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
(clock()>timeOutWarning) &&
(!triggeredTimeoutWarning)
) {
tarch::parallel::Node::getInstance().writeTimeOutWarning(
"dem::records::StatePacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"dem::records::StatePacked",
"receive(int)", source,tag,1
);
}
tarch::parallel::Node::getInstance().receiveDanglingMessages();
usleep(communicateSleep);

}

delete sendRequestHandle;

_senderDestinationRank = status.MPI_SOURCE;
#ifdef Debug
_log.debug("receive(int,int)", "received " + toString() ); 
#endif

}

}



bool dem::records::StatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Status status;
int  flag        = 0;
MPI_Iprobe(
MPI_ANY_SOURCE, tag,
tarch::parallel::Node::getInstance().getCommunicator(), &flag, &status
);
if (flag) {
int  messageCounter;
if (exchangeOnlyAttributesMarkedWithParallelise) {
MPI_Get_count(&status, Datatype, &messageCounter);
}
else {
MPI_Get_count(&status, FullDatatype, &messageCounter);
}
return messageCounter > 0;
}
else return false;

}

int dem::records::StatePacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#endif


