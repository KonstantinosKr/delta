#include "dem/records/RepositoryState.h"

dem::records::RepositoryState::PersistentRecords::PersistentRecords() {
   
}


dem::records::RepositoryState::PersistentRecords::PersistentRecords(const Action& action, const int& numberOfIterations, const bool& exchangeBoundaryVertices):
_action(action),
_numberOfIterations(numberOfIterations),
_exchangeBoundaryVertices(exchangeBoundaryVertices) {
   
}

dem::records::RepositoryState::RepositoryState() {
   
}


dem::records::RepositoryState::RepositoryState(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._action, persistentRecords._numberOfIterations, persistentRecords._exchangeBoundaryVertices) {
   
}


dem::records::RepositoryState::RepositoryState(const Action& action, const int& numberOfIterations, const bool& exchangeBoundaryVertices):
_persistentRecords(action, numberOfIterations, exchangeBoundaryVertices) {
   
}


dem::records::RepositoryState::~RepositoryState() { }

std::string dem::records::RepositoryState::toString(const Action& param) {
   switch (param) {
      case WriteCheckpoint: return "WriteCheckpoint";
      case ReadCheckpoint: return "ReadCheckpoint";
      case Terminate: return "Terminate";
      case RunOnAllNodes: return "RunOnAllNodes";
      case UseAdapterCreateGrid: return "UseAdapterCreateGrid";
      case UseAdapterCreateGridAndPlot: return "UseAdapterCreateGridAndPlot";
      case UseAdapterTimeStep: return "UseAdapterTimeStep";
      case UseAdapterTimeStepAndPlot: return "UseAdapterTimeStepAndPlot";
      case UseAdapterCollision: return "UseAdapterCollision";
      case UseAdapterMoveParticles: return "UseAdapterMoveParticles";
      case UseAdapterAdopt: return "UseAdapterAdopt";
      case UseAdapterAdoptReluctantly: return "UseAdapterAdoptReluctantly";
      case UseAdapterPlotData: return "UseAdapterPlotData";
      case UseAdapterTimeStepOnDynamicGrid: return "UseAdapterTimeStepOnDynamicGrid";
      case UseAdapterTimeStepOnDynamicGridMerged: return "UseAdapterTimeStepOnDynamicGridMerged";
      case UseAdapterTimeStepAndPlotOnDynamicGrid: return "UseAdapterTimeStepAndPlotOnDynamicGrid";
      case UseAdapterTimeStepOnReluctantDynamicGrid: return "UseAdapterTimeStepOnReluctantDynamicGrid";
      case UseAdapterTimeStepOnReluctantDynamicGridMerged: return "UseAdapterTimeStepOnReluctantDynamicGridMerged";
      case UseAdapterTimeStepAndPlotOnReluctantDynamicGrid: return "UseAdapterTimeStepAndPlotOnReluctantDynamicGrid";
      case NumberOfAdapters: return "NumberOfAdapters";
   }
   return "undefined";
}

std::string dem::records::RepositoryState::getActionMapping() {
   return "Action(WriteCheckpoint=0,ReadCheckpoint=1,Terminate=2,RunOnAllNodes=3,UseAdapterCreateGrid=4,UseAdapterCreateGridAndPlot=5,UseAdapterTimeStep=6,UseAdapterTimeStepAndPlot=7,UseAdapterCollision=8,UseAdapterMoveParticles=9,UseAdapterAdopt=10,UseAdapterAdoptReluctantly=11,UseAdapterPlotData=12,UseAdapterTimeStepOnDynamicGrid=13,UseAdapterTimeStepOnDynamicGridMerged=14,UseAdapterTimeStepAndPlotOnDynamicGrid=15,UseAdapterTimeStepOnReluctantDynamicGrid=16,UseAdapterTimeStepOnReluctantDynamicGridMerged=17,UseAdapterTimeStepAndPlotOnReluctantDynamicGrid=18,NumberOfAdapters=19)";
}


std::string dem::records::RepositoryState::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void dem::records::RepositoryState::toString (std::ostream& out) const {
   out << "("; 
   out << "action:" << toString(getAction());
   out << ",";
   out << "numberOfIterations:" << getNumberOfIterations();
   out << ",";
   out << "exchangeBoundaryVertices:" << getExchangeBoundaryVertices();
   out <<  ")";
}


dem::records::RepositoryState::PersistentRecords dem::records::RepositoryState::getPersistentRecords() const {
   return _persistentRecords;
}

dem::records::RepositoryStatePacked dem::records::RepositoryState::convert() const{
   return RepositoryStatePacked(
      getAction(),
      getNumberOfIterations(),
      getExchangeBoundaryVertices()
   );
}

#ifdef Parallel
   tarch::logging::Log dem::records::RepositoryState::_log( "dem::records::RepositoryState" );
   
   MPI_Datatype dem::records::RepositoryState::Datatype = 0;
   MPI_Datatype dem::records::RepositoryState::FullDatatype = 0;
   
   
   void dem::records::RepositoryState::initDatatype() {
      {
         RepositoryState dummyRepositoryState[2];
         
         #ifdef MPI2
         const int Attributes = 3;
         #else
         const int Attributes = 4;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //action
            , MPI_INT		 //numberOfIterations
            , MPI_CXX_BOOL		 //exchangeBoundaryVertices
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //action
            , 1		 //numberOfIterations
            , 1		 //exchangeBoundaryVertices
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._action))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._action))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._numberOfIterations))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._numberOfIterations))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._exchangeBoundaryVertices))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._exchangeBoundaryVertices))), 		&disp[2] );
         #endif
         #ifdef MPI2
         for (int i=1; i<Attributes; i++) {
         #else
         for (int i=1; i<Attributes-1; i++) {
         #endif
            assertion1( disp[i] > disp[i-1], i );
         }
         #ifdef MPI2
         for (int i=0; i<Attributes; i++) {
         #else
         for (int i=0; i<Attributes-1; i++) {
         #endif
            disp[i] = disp[i] - base; // should be MPI_Aint_diff(disp[i], base); but this is not supported by most MPI-2 implementations
            assertion4(disp[i]<static_cast<int>(sizeof(RepositoryState)), i, disp[i], Attributes, sizeof(RepositoryState));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[1]))), 		&disp[3] );
         disp[3] -= base;
         disp[3] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &RepositoryState::Datatype );
         MPI_Type_commit( &RepositoryState::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &RepositoryState::Datatype);
         MPI_Type_commit( &RepositoryState::Datatype );
         #endif
         
      }
      {
         RepositoryState dummyRepositoryState[2];
         
         #ifdef MPI2
         const int Attributes = 3;
         #else
         const int Attributes = 4;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //action
            , MPI_INT		 //numberOfIterations
            , MPI_CXX_BOOL		 //exchangeBoundaryVertices
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //action
            , 1		 //numberOfIterations
            , 1		 //exchangeBoundaryVertices
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._action))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._action))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._numberOfIterations))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._numberOfIterations))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._exchangeBoundaryVertices))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[0]._persistentRecords._exchangeBoundaryVertices))), 		&disp[2] );
         #endif
         #ifdef MPI2
         for (int i=1; i<Attributes; i++) {
         #else
         for (int i=1; i<Attributes-1; i++) {
         #endif
            assertion1( disp[i] > disp[i-1], i );
         }
         #ifdef MPI2
         for (int i=0; i<Attributes; i++) {
         #else
         for (int i=0; i<Attributes-1; i++) {
         #endif
            disp[i] = disp[i] - base; // should be MPI_Aint_diff(disp[i], base); but this is not supported by most MPI-2 implementations
            assertion4(disp[i]<static_cast<int>(sizeof(RepositoryState)), i, disp[i], Attributes, sizeof(RepositoryState));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryState[1]))), 		&disp[3] );
         disp[3] -= base;
         disp[3] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &RepositoryState::FullDatatype );
         MPI_Type_commit( &RepositoryState::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &RepositoryState::FullDatatype);
         MPI_Type_commit( &RepositoryState::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void dem::records::RepositoryState::shutdownDatatype() {
      MPI_Type_free( &RepositoryState::Datatype );
      MPI_Type_free( &RepositoryState::FullDatatype );
      
   }
   
   void dem::records::RepositoryState::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      _senderDestinationRank = destination;
      
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message dem::records::RepositoryState "
            << toString()
            << " to node " << destination
            << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "send(int)",msg.str() );
         }
         
      }
      else {
      
         MPI_Request* sendRequestHandle = new MPI_Request();
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
            msg << "was not able to send message dem::records::RepositoryState "
            << toString()
            << " to node " << destination
            << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "send(int)",msg.str() );
         }
         result = MPI_Test( sendRequestHandle, &flag, MPI_STATUS_IGNORE );
         while (!flag) {
            if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
            if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
            result = MPI_Test( sendRequestHandle, &flag, MPI_STATUS_IGNORE );
            if (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "testing for finished send task for dem::records::RepositoryState "
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
               "dem::records::RepositoryState",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::RepositoryState",
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
   
   
   
   void dem::records::RepositoryState::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
         _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive dem::records::RepositoryState from node "
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
            msg << "failed to start to receive dem::records::RepositoryState from node "
            << source << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "receive(int)", msg.str() );
         }
         
         result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
         while (!flag) {
            if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
            if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
            result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
            if (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "testing for finished receive task for dem::records::RepositoryState failed: "
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
               "dem::records::RepositoryState",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::RepositoryState",
               "receive(int)", source,tag,1
               );
            }
            tarch::parallel::Node::getInstance().receiveDanglingMessages();
            usleep(communicateSleep);
            
         }
         
         delete sendRequestHandle;
         
         _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
         #ifdef Debug
         _log.debug("receive(int,int)", "received " + toString() ); 
         #endif
         
      }
      
   }
   
   
   
   bool dem::records::RepositoryState::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int dem::records::RepositoryState::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif


dem::records::RepositoryStatePacked::PersistentRecords::PersistentRecords() {
   
}


dem::records::RepositoryStatePacked::PersistentRecords::PersistentRecords(const Action& action, const int& numberOfIterations, const bool& exchangeBoundaryVertices):
_action(action),
_numberOfIterations(numberOfIterations),
_exchangeBoundaryVertices(exchangeBoundaryVertices) {
   
}

dem::records::RepositoryStatePacked::RepositoryStatePacked() {
   
}


dem::records::RepositoryStatePacked::RepositoryStatePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._action, persistentRecords._numberOfIterations, persistentRecords._exchangeBoundaryVertices) {
   
}


dem::records::RepositoryStatePacked::RepositoryStatePacked(const Action& action, const int& numberOfIterations, const bool& exchangeBoundaryVertices):
_persistentRecords(action, numberOfIterations, exchangeBoundaryVertices) {
   
}


dem::records::RepositoryStatePacked::~RepositoryStatePacked() { }

std::string dem::records::RepositoryStatePacked::toString(const Action& param) {
   return dem::records::RepositoryState::toString(param);
}

std::string dem::records::RepositoryStatePacked::getActionMapping() {
   return dem::records::RepositoryState::getActionMapping();
}



std::string dem::records::RepositoryStatePacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void dem::records::RepositoryStatePacked::toString (std::ostream& out) const {
   out << "("; 
   out << "action:" << toString(getAction());
   out << ",";
   out << "numberOfIterations:" << getNumberOfIterations();
   out << ",";
   out << "exchangeBoundaryVertices:" << getExchangeBoundaryVertices();
   out <<  ")";
}


dem::records::RepositoryStatePacked::PersistentRecords dem::records::RepositoryStatePacked::getPersistentRecords() const {
   return _persistentRecords;
}

dem::records::RepositoryState dem::records::RepositoryStatePacked::convert() const{
   return RepositoryState(
      getAction(),
      getNumberOfIterations(),
      getExchangeBoundaryVertices()
   );
}

#ifdef Parallel
   tarch::logging::Log dem::records::RepositoryStatePacked::_log( "dem::records::RepositoryStatePacked" );
   
   MPI_Datatype dem::records::RepositoryStatePacked::Datatype = 0;
   MPI_Datatype dem::records::RepositoryStatePacked::FullDatatype = 0;
   
   
   void dem::records::RepositoryStatePacked::initDatatype() {
      {
         RepositoryStatePacked dummyRepositoryStatePacked[2];
         
         #ifdef MPI2
         const int Attributes = 3;
         #else
         const int Attributes = 4;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //action
            , MPI_INT		 //numberOfIterations
            , MPI_CXX_BOOL		 //exchangeBoundaryVertices
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //action
            , 1		 //numberOfIterations
            , 1		 //exchangeBoundaryVertices
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._action))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._action))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._numberOfIterations))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._numberOfIterations))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._exchangeBoundaryVertices))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._exchangeBoundaryVertices))), 		&disp[2] );
         #endif
         #ifdef MPI2
         for (int i=1; i<Attributes; i++) {
         #else
         for (int i=1; i<Attributes-1; i++) {
         #endif
            assertion1( disp[i] > disp[i-1], i );
         }
         #ifdef MPI2
         for (int i=0; i<Attributes; i++) {
         #else
         for (int i=0; i<Attributes-1; i++) {
         #endif
            disp[i] = disp[i] - base; // should be MPI_Aint_diff(disp[i], base); but this is not supported by most MPI-2 implementations
            assertion4(disp[i]<static_cast<int>(sizeof(RepositoryStatePacked)), i, disp[i], Attributes, sizeof(RepositoryStatePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[1]))), 		&disp[3] );
         disp[3] -= base;
         disp[3] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &RepositoryStatePacked::Datatype );
         MPI_Type_commit( &RepositoryStatePacked::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &RepositoryStatePacked::Datatype);
         MPI_Type_commit( &RepositoryStatePacked::Datatype );
         #endif
         
      }
      {
         RepositoryStatePacked dummyRepositoryStatePacked[2];
         
         #ifdef MPI2
         const int Attributes = 3;
         #else
         const int Attributes = 4;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //action
            , MPI_INT		 //numberOfIterations
            , MPI_CXX_BOOL		 //exchangeBoundaryVertices
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //action
            , 1		 //numberOfIterations
            , 1		 //exchangeBoundaryVertices
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._action))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._action))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._numberOfIterations))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._numberOfIterations))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._exchangeBoundaryVertices))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[0]._persistentRecords._exchangeBoundaryVertices))), 		&disp[2] );
         #endif
         #ifdef MPI2
         for (int i=1; i<Attributes; i++) {
         #else
         for (int i=1; i<Attributes-1; i++) {
         #endif
            assertion1( disp[i] > disp[i-1], i );
         }
         #ifdef MPI2
         for (int i=0; i<Attributes; i++) {
         #else
         for (int i=0; i<Attributes-1; i++) {
         #endif
            disp[i] = disp[i] - base; // should be MPI_Aint_diff(disp[i], base); but this is not supported by most MPI-2 implementations
            assertion4(disp[i]<static_cast<int>(sizeof(RepositoryStatePacked)), i, disp[i], Attributes, sizeof(RepositoryStatePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyRepositoryStatePacked[1]))), 		&disp[3] );
         disp[3] -= base;
         disp[3] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &RepositoryStatePacked::FullDatatype );
         MPI_Type_commit( &RepositoryStatePacked::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &RepositoryStatePacked::FullDatatype);
         MPI_Type_commit( &RepositoryStatePacked::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void dem::records::RepositoryStatePacked::shutdownDatatype() {
      MPI_Type_free( &RepositoryStatePacked::Datatype );
      MPI_Type_free( &RepositoryStatePacked::FullDatatype );
      
   }
   
   void dem::records::RepositoryStatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      _senderDestinationRank = destination;
      
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message dem::records::RepositoryStatePacked "
            << toString()
            << " to node " << destination
            << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "send(int)",msg.str() );
         }
         
      }
      else {
      
         MPI_Request* sendRequestHandle = new MPI_Request();
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
            msg << "was not able to send message dem::records::RepositoryStatePacked "
            << toString()
            << " to node " << destination
            << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "send(int)",msg.str() );
         }
         result = MPI_Test( sendRequestHandle, &flag, MPI_STATUS_IGNORE );
         while (!flag) {
            if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
            if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
            result = MPI_Test( sendRequestHandle, &flag, MPI_STATUS_IGNORE );
            if (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "testing for finished send task for dem::records::RepositoryStatePacked "
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
               "dem::records::RepositoryStatePacked",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::RepositoryStatePacked",
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
   
   
   
   void dem::records::RepositoryStatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
         _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive dem::records::RepositoryStatePacked from node "
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
            msg << "failed to start to receive dem::records::RepositoryStatePacked from node "
            << source << ": " << tarch::parallel::MPIReturnValueToString(result);
            _log.error( "receive(int)", msg.str() );
         }
         
         result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
         while (!flag) {
            if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
            if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
            result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
            if (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "testing for finished receive task for dem::records::RepositoryStatePacked failed: "
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
               "dem::records::RepositoryStatePacked",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::RepositoryStatePacked",
               "receive(int)", source,tag,1
               );
            }
            tarch::parallel::Node::getInstance().receiveDanglingMessages();
            usleep(communicateSleep);
            
         }
         
         delete sendRequestHandle;
         
         _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
         #ifdef Debug
         _log.debug("receive(int,int)", "received " + toString() ); 
         #endif
         
      }
      
   }
   
   
   
   bool dem::records::RepositoryStatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int dem::records::RepositoryStatePacked::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif



