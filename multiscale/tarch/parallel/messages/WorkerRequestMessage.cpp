#include "tarch/parallel/messages/WorkerRequestMessage.h"

tarch::parallel::messages::WorkerRequestMessage::PersistentRecords::PersistentRecords() {
   
}


tarch::parallel::messages::WorkerRequestMessage::PersistentRecords::PersistentRecords(const int& numberOfRequestedWorkers):
_numberOfRequestedWorkers(numberOfRequestedWorkers) {
   
}


 int tarch::parallel::messages::WorkerRequestMessage::PersistentRecords::getNumberOfRequestedWorkers() const  {
   return _numberOfRequestedWorkers;
}



 void tarch::parallel::messages::WorkerRequestMessage::PersistentRecords::setNumberOfRequestedWorkers(const int& numberOfRequestedWorkers)  {
   _numberOfRequestedWorkers = numberOfRequestedWorkers;
}


tarch::parallel::messages::WorkerRequestMessage::WorkerRequestMessage() {
   
}


tarch::parallel::messages::WorkerRequestMessage::WorkerRequestMessage(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfRequestedWorkers) {
   
}


tarch::parallel::messages::WorkerRequestMessage::WorkerRequestMessage(const int& numberOfRequestedWorkers):
_persistentRecords(numberOfRequestedWorkers) {
   
}


tarch::parallel::messages::WorkerRequestMessage::~WorkerRequestMessage() { }


 int tarch::parallel::messages::WorkerRequestMessage::getNumberOfRequestedWorkers() const  {
   return _persistentRecords._numberOfRequestedWorkers;
}



 void tarch::parallel::messages::WorkerRequestMessage::setNumberOfRequestedWorkers(const int& numberOfRequestedWorkers)  {
   _persistentRecords._numberOfRequestedWorkers = numberOfRequestedWorkers;
}




std::string tarch::parallel::messages::WorkerRequestMessage::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void tarch::parallel::messages::WorkerRequestMessage::toString (std::ostream& out) const {
   out << "("; 
   out << "numberOfRequestedWorkers:" << getNumberOfRequestedWorkers();
   out <<  ")";
}


tarch::parallel::messages::WorkerRequestMessage::PersistentRecords tarch::parallel::messages::WorkerRequestMessage::getPersistentRecords() const {
   return _persistentRecords;
}

tarch::parallel::messages::WorkerRequestMessagePacked tarch::parallel::messages::WorkerRequestMessage::convert() const{
   return WorkerRequestMessagePacked(
      getNumberOfRequestedWorkers()
   );
}

#ifdef Parallel
   tarch::logging::Log tarch::parallel::messages::WorkerRequestMessage::_log( "tarch::parallel::messages::WorkerRequestMessage" );
   
   MPI_Datatype tarch::parallel::messages::WorkerRequestMessage::Datatype = 0;
   MPI_Datatype tarch::parallel::messages::WorkerRequestMessage::FullDatatype = 0;
   
   
   void tarch::parallel::messages::WorkerRequestMessage::initDatatype() {
      {
         WorkerRequestMessage dummyWorkerRequestMessage[2];
         
         #ifdef MPI2
         const int Attributes = 1;
         #else
         const int Attributes = 2;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //numberOfRequestedWorkers
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //numberOfRequestedWorkers
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage[0]._persistentRecords._numberOfRequestedWorkers))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage[0]._persistentRecords._numberOfRequestedWorkers))), 		&disp[0] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(WorkerRequestMessage)), i, disp[i], Attributes, sizeof(WorkerRequestMessage));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage[1]))), 		&disp[1] );
         disp[1] -= base;
         disp[1] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &WorkerRequestMessage::Datatype );
         MPI_Type_commit( &WorkerRequestMessage::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &WorkerRequestMessage::Datatype);
         MPI_Type_commit( &WorkerRequestMessage::Datatype );
         #endif
         
      }
      {
         WorkerRequestMessage dummyWorkerRequestMessage[2];
         
         #ifdef MPI2
         const int Attributes = 1;
         #else
         const int Attributes = 2;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //numberOfRequestedWorkers
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //numberOfRequestedWorkers
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage[0]._persistentRecords._numberOfRequestedWorkers))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage[0]._persistentRecords._numberOfRequestedWorkers))), 		&disp[0] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(WorkerRequestMessage)), i, disp[i], Attributes, sizeof(WorkerRequestMessage));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessage[1]))), 		&disp[1] );
         disp[1] -= base;
         disp[1] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &WorkerRequestMessage::FullDatatype );
         MPI_Type_commit( &WorkerRequestMessage::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &WorkerRequestMessage::FullDatatype);
         MPI_Type_commit( &WorkerRequestMessage::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void tarch::parallel::messages::WorkerRequestMessage::shutdownDatatype() {
      MPI_Type_free( &WorkerRequestMessage::Datatype );
      MPI_Type_free( &WorkerRequestMessage::FullDatatype );
      
   }
   
   void tarch::parallel::messages::WorkerRequestMessage::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message tarch::parallel::messages::WorkerRequestMessage " 
             << toString() 
             << " to node " << destination 
             << ": " << tarch::parallel::MPIReturnValueToString(result); 
         _log.error( "send(int)",msg.str() ); 
       } 
    } 
    break; 
   case ExchangeMode::NonblockingWithPollingLoopOverTests: 
    {
      MPI_Request* sendRequestHandle = new MPI_Request(); 
      int          flag = 0; 
       int          result; 
       clock_t      timeOutWarning   = -1; 
       clock_t      timeOutShutdown  = -1; 
       bool         triggeredTimeoutWarning = false;  
       result = MPI_Isend(  
         this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination,  
         tag, tarch::parallel::Node::getInstance().getCommunicator(), 
         sendRequestHandle  
       ); 
       if  (result!=MPI_SUCCESS) {  
         std::ostringstream msg;  
         msg << "was not able to send message tarch::parallel::messages::WorkerRequestMessage "  
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
           msg << "testing for finished send task for tarch::parallel::messages::WorkerRequestMessage " 
               << toString() 
               << " sent to node " << destination 
               << " failed: " << tarch::parallel::MPIReturnValueToString(result); 
           _log.error("send(int)", msg.str() ); 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
           (clock()>timeOutWarning) && 
           (!triggeredTimeoutWarning) 
         ) { 
           tarch::parallel::Node::getInstance().writeTimeOutWarning( 
             "tarch::parallel::messages::WorkerRequestMessage", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "tarch::parallel::messages::WorkerRequestMessage", 
             "send(int)", destination,tag,1 
           ); 
         } 
 	       tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
       } 
       delete sendRequestHandle; 
     }  
     break; 
   case ExchangeMode::LoopOverProbeWithBlockingReceive: 
    assertionMsg(false,"should not be called"); 
    break; 
} 
 // ============================= 
// end injected snippet/aspect 
// ============================= 

      
   }
   
   
   
   void tarch::parallel::messages::WorkerRequestMessage::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
MPI_Status status; 
switch (mode) { 
  case ExchangeMode::Blocking: 
    { 
      const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive tarch::parallel::messages::WorkerRequestMessage from node " 
            << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
    } 
    break; 
  case ExchangeMode::NonblockingWithPollingLoopOverTests: 
    { 
      int          flag = 0; 
      int          result; 
      clock_t      timeOutWarning   = -1; 
      clock_t      timeOutShutdown  = -1; 
      bool         triggeredTimeoutWarning = false; 
      MPI_Request* sendRequestHandle = new MPI_Request(); 
       result = MPI_Irecv( 
        this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, 
        tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle 
      ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive tarch::parallel::messages::WorkerRequestMessage from node " 
             << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
      result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "tarch::parallel::messages::WorkerRequestMessage", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "tarch::parallel::messages::WorkerRequestMessage", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for tarch::parallel::messages::WorkerRequestMessage failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
      } 
      delete sendRequestHandle; 
    }    break; 
  case ExchangeMode::LoopOverProbeWithBlockingReceive: 
    {
      int flag; 
      clock_t      timeOutWarning   = -1; 
      clock_t      timeOutShutdown  = -1; 
      bool         triggeredTimeoutWarning = false; 
      int result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
       if (result!=MPI_SUCCESS) { 
        std::ostringstream msg; 
        msg << "testing for finished receive task for tarch::parallel::messages::WorkerRequestMessage failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "tarch::parallel::messages::WorkerRequestMessage", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "tarch::parallel::messages::WorkerRequestMessage", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for tarch::parallel::messages::WorkerRequestMessage failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive tarch::parallel::messages::WorkerRequestMessage from node " 
            << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
    }
    break; 
  } 
// =========================== 
// end injected snippet/aspect 
// =========================== 

      
     _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
   }
   
   
   
   bool tarch::parallel::messages::WorkerRequestMessage::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int tarch::parallel::messages::WorkerRequestMessage::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif


tarch::parallel::messages::WorkerRequestMessagePacked::PersistentRecords::PersistentRecords() {
   
}


tarch::parallel::messages::WorkerRequestMessagePacked::PersistentRecords::PersistentRecords(const int& numberOfRequestedWorkers):
_numberOfRequestedWorkers(numberOfRequestedWorkers) {
   
}


 int tarch::parallel::messages::WorkerRequestMessagePacked::PersistentRecords::getNumberOfRequestedWorkers() const  {
   return _numberOfRequestedWorkers;
}



 void tarch::parallel::messages::WorkerRequestMessagePacked::PersistentRecords::setNumberOfRequestedWorkers(const int& numberOfRequestedWorkers)  {
   _numberOfRequestedWorkers = numberOfRequestedWorkers;
}


tarch::parallel::messages::WorkerRequestMessagePacked::WorkerRequestMessagePacked() {
   
}


tarch::parallel::messages::WorkerRequestMessagePacked::WorkerRequestMessagePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._numberOfRequestedWorkers) {
   
}


tarch::parallel::messages::WorkerRequestMessagePacked::WorkerRequestMessagePacked(const int& numberOfRequestedWorkers):
_persistentRecords(numberOfRequestedWorkers) {
   
}


tarch::parallel::messages::WorkerRequestMessagePacked::~WorkerRequestMessagePacked() { }


 int tarch::parallel::messages::WorkerRequestMessagePacked::getNumberOfRequestedWorkers() const  {
   return _persistentRecords._numberOfRequestedWorkers;
}



 void tarch::parallel::messages::WorkerRequestMessagePacked::setNumberOfRequestedWorkers(const int& numberOfRequestedWorkers)  {
   _persistentRecords._numberOfRequestedWorkers = numberOfRequestedWorkers;
}




std::string tarch::parallel::messages::WorkerRequestMessagePacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void tarch::parallel::messages::WorkerRequestMessagePacked::toString (std::ostream& out) const {
   out << "("; 
   out << "numberOfRequestedWorkers:" << getNumberOfRequestedWorkers();
   out <<  ")";
}


tarch::parallel::messages::WorkerRequestMessagePacked::PersistentRecords tarch::parallel::messages::WorkerRequestMessagePacked::getPersistentRecords() const {
   return _persistentRecords;
}

tarch::parallel::messages::WorkerRequestMessage tarch::parallel::messages::WorkerRequestMessagePacked::convert() const{
   return WorkerRequestMessage(
      getNumberOfRequestedWorkers()
   );
}

#ifdef Parallel
   tarch::logging::Log tarch::parallel::messages::WorkerRequestMessagePacked::_log( "tarch::parallel::messages::WorkerRequestMessagePacked" );
   
   MPI_Datatype tarch::parallel::messages::WorkerRequestMessagePacked::Datatype = 0;
   MPI_Datatype tarch::parallel::messages::WorkerRequestMessagePacked::FullDatatype = 0;
   
   
   void tarch::parallel::messages::WorkerRequestMessagePacked::initDatatype() {
      {
         WorkerRequestMessagePacked dummyWorkerRequestMessagePacked[2];
         
         #ifdef MPI2
         const int Attributes = 1;
         #else
         const int Attributes = 2;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //numberOfRequestedWorkers
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //numberOfRequestedWorkers
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked[0]._persistentRecords._numberOfRequestedWorkers))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked[0]._persistentRecords._numberOfRequestedWorkers))), 		&disp[0] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(WorkerRequestMessagePacked)), i, disp[i], Attributes, sizeof(WorkerRequestMessagePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked[1]))), 		&disp[1] );
         disp[1] -= base;
         disp[1] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &WorkerRequestMessagePacked::Datatype );
         MPI_Type_commit( &WorkerRequestMessagePacked::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &WorkerRequestMessagePacked::Datatype);
         MPI_Type_commit( &WorkerRequestMessagePacked::Datatype );
         #endif
         
      }
      {
         WorkerRequestMessagePacked dummyWorkerRequestMessagePacked[2];
         
         #ifdef MPI2
         const int Attributes = 1;
         #else
         const int Attributes = 2;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //numberOfRequestedWorkers
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //numberOfRequestedWorkers
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked[0]._persistentRecords._numberOfRequestedWorkers))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked[0]._persistentRecords._numberOfRequestedWorkers))), 		&disp[0] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(WorkerRequestMessagePacked)), i, disp[i], Attributes, sizeof(WorkerRequestMessagePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerRequestMessagePacked[1]))), 		&disp[1] );
         disp[1] -= base;
         disp[1] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &WorkerRequestMessagePacked::FullDatatype );
         MPI_Type_commit( &WorkerRequestMessagePacked::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &WorkerRequestMessagePacked::FullDatatype);
         MPI_Type_commit( &WorkerRequestMessagePacked::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void tarch::parallel::messages::WorkerRequestMessagePacked::shutdownDatatype() {
      MPI_Type_free( &WorkerRequestMessagePacked::Datatype );
      MPI_Type_free( &WorkerRequestMessagePacked::FullDatatype );
      
   }
   
   void tarch::parallel::messages::WorkerRequestMessagePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message tarch::parallel::messages::WorkerRequestMessagePacked " 
             << toString() 
             << " to node " << destination 
             << ": " << tarch::parallel::MPIReturnValueToString(result); 
         _log.error( "send(int)",msg.str() ); 
       } 
    } 
    break; 
   case ExchangeMode::NonblockingWithPollingLoopOverTests: 
    {
      MPI_Request* sendRequestHandle = new MPI_Request(); 
      int          flag = 0; 
       int          result; 
       clock_t      timeOutWarning   = -1; 
       clock_t      timeOutShutdown  = -1; 
       bool         triggeredTimeoutWarning = false;  
       result = MPI_Isend(  
         this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination,  
         tag, tarch::parallel::Node::getInstance().getCommunicator(), 
         sendRequestHandle  
       ); 
       if  (result!=MPI_SUCCESS) {  
         std::ostringstream msg;  
         msg << "was not able to send message tarch::parallel::messages::WorkerRequestMessagePacked "  
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
           msg << "testing for finished send task for tarch::parallel::messages::WorkerRequestMessagePacked " 
               << toString() 
               << " sent to node " << destination 
               << " failed: " << tarch::parallel::MPIReturnValueToString(result); 
           _log.error("send(int)", msg.str() ); 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
           (clock()>timeOutWarning) && 
           (!triggeredTimeoutWarning) 
         ) { 
           tarch::parallel::Node::getInstance().writeTimeOutWarning( 
             "tarch::parallel::messages::WorkerRequestMessagePacked", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "tarch::parallel::messages::WorkerRequestMessagePacked", 
             "send(int)", destination,tag,1 
           ); 
         } 
 	       tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
       } 
       delete sendRequestHandle; 
     }  
     break; 
   case ExchangeMode::LoopOverProbeWithBlockingReceive: 
    assertionMsg(false,"should not be called"); 
    break; 
} 
 // ============================= 
// end injected snippet/aspect 
// ============================= 

      
   }
   
   
   
   void tarch::parallel::messages::WorkerRequestMessagePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
MPI_Status status; 
switch (mode) { 
  case ExchangeMode::Blocking: 
    { 
      const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive tarch::parallel::messages::WorkerRequestMessagePacked from node " 
            << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
    } 
    break; 
  case ExchangeMode::NonblockingWithPollingLoopOverTests: 
    { 
      int          flag = 0; 
      int          result; 
      clock_t      timeOutWarning   = -1; 
      clock_t      timeOutShutdown  = -1; 
      bool         triggeredTimeoutWarning = false; 
      MPI_Request* sendRequestHandle = new MPI_Request(); 
       result = MPI_Irecv( 
        this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, 
        tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle 
      ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive tarch::parallel::messages::WorkerRequestMessagePacked from node " 
             << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
      result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "tarch::parallel::messages::WorkerRequestMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "tarch::parallel::messages::WorkerRequestMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for tarch::parallel::messages::WorkerRequestMessagePacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
      } 
      delete sendRequestHandle; 
    }    break; 
  case ExchangeMode::LoopOverProbeWithBlockingReceive: 
    {
      int flag; 
      clock_t      timeOutWarning   = -1; 
      clock_t      timeOutShutdown  = -1; 
      bool         triggeredTimeoutWarning = false; 
      int result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
       if (result!=MPI_SUCCESS) { 
        std::ostringstream msg; 
        msg << "testing for finished receive task for tarch::parallel::messages::WorkerRequestMessagePacked failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "tarch::parallel::messages::WorkerRequestMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "tarch::parallel::messages::WorkerRequestMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for tarch::parallel::messages::WorkerRequestMessagePacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive tarch::parallel::messages::WorkerRequestMessagePacked from node " 
            << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
    }
    break; 
  } 
// =========================== 
// end injected snippet/aspect 
// =========================== 

      
     _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
   }
   
   
   
   bool tarch::parallel::messages::WorkerRequestMessagePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int tarch::parallel::messages::WorkerRequestMessagePacked::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif



