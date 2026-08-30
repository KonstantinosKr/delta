#include "tarch/parallel/messages/NodePoolAnswerMessage.h"

tarch::parallel::messages::NodePoolAnswerMessage::PersistentRecords::PersistentRecords() {
   
}


tarch::parallel::messages::NodePoolAnswerMessage::PersistentRecords::PersistentRecords(const int& newWorker):
_newWorker(newWorker) {
   
}


 int tarch::parallel::messages::NodePoolAnswerMessage::PersistentRecords::getNewWorker() const  {
   return _newWorker;
}



 void tarch::parallel::messages::NodePoolAnswerMessage::PersistentRecords::setNewWorker(const int& newWorker)  {
   _newWorker = newWorker;
}


tarch::parallel::messages::NodePoolAnswerMessage::NodePoolAnswerMessage() {
   
}


tarch::parallel::messages::NodePoolAnswerMessage::NodePoolAnswerMessage(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._newWorker) {
   
}


tarch::parallel::messages::NodePoolAnswerMessage::NodePoolAnswerMessage(const int& newWorker):
_persistentRecords(newWorker) {
   
}


tarch::parallel::messages::NodePoolAnswerMessage::~NodePoolAnswerMessage() { }


 int tarch::parallel::messages::NodePoolAnswerMessage::getNewWorker() const  {
   return _persistentRecords._newWorker;
}



 void tarch::parallel::messages::NodePoolAnswerMessage::setNewWorker(const int& newWorker)  {
   _persistentRecords._newWorker = newWorker;
}




std::string tarch::parallel::messages::NodePoolAnswerMessage::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void tarch::parallel::messages::NodePoolAnswerMessage::toString (std::ostream& out) const {
   out << "("; 
   out << "newWorker:" << getNewWorker();
   out <<  ")";
}


tarch::parallel::messages::NodePoolAnswerMessage::PersistentRecords tarch::parallel::messages::NodePoolAnswerMessage::getPersistentRecords() const {
   return _persistentRecords;
}

tarch::parallel::messages::NodePoolAnswerMessagePacked tarch::parallel::messages::NodePoolAnswerMessage::convert() const{
   return NodePoolAnswerMessagePacked(
      getNewWorker()
   );
}

#ifdef Parallel
   tarch::logging::Log tarch::parallel::messages::NodePoolAnswerMessage::_log( "tarch::parallel::messages::NodePoolAnswerMessage" );
   
   MPI_Datatype tarch::parallel::messages::NodePoolAnswerMessage::Datatype = 0;
   MPI_Datatype tarch::parallel::messages::NodePoolAnswerMessage::FullDatatype = 0;
   
   
   void tarch::parallel::messages::NodePoolAnswerMessage::initDatatype() {
      {
         NodePoolAnswerMessage dummyNodePoolAnswerMessage[2];
         
         #ifdef MPI2
         const int Attributes = 1;
         #else
         const int Attributes = 2;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //newWorker
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //newWorker
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage[0]._persistentRecords._newWorker))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage[0]._persistentRecords._newWorker))), 		&disp[0] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(NodePoolAnswerMessage)), i, disp[i], Attributes, sizeof(NodePoolAnswerMessage));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage[1]))), 		&disp[1] );
         disp[1] -= base;
         disp[1] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &NodePoolAnswerMessage::Datatype );
         MPI_Type_commit( &NodePoolAnswerMessage::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &NodePoolAnswerMessage::Datatype);
         MPI_Type_commit( &NodePoolAnswerMessage::Datatype );
         #endif
         
      }
      {
         NodePoolAnswerMessage dummyNodePoolAnswerMessage[2];
         
         #ifdef MPI2
         const int Attributes = 1;
         #else
         const int Attributes = 2;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //newWorker
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //newWorker
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage[0]._persistentRecords._newWorker))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage[0]._persistentRecords._newWorker))), 		&disp[0] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(NodePoolAnswerMessage)), i, disp[i], Attributes, sizeof(NodePoolAnswerMessage));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessage[1]))), 		&disp[1] );
         disp[1] -= base;
         disp[1] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &NodePoolAnswerMessage::FullDatatype );
         MPI_Type_commit( &NodePoolAnswerMessage::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &NodePoolAnswerMessage::FullDatatype);
         MPI_Type_commit( &NodePoolAnswerMessage::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void tarch::parallel::messages::NodePoolAnswerMessage::shutdownDatatype() {
      MPI_Type_free( &NodePoolAnswerMessage::Datatype );
      MPI_Type_free( &NodePoolAnswerMessage::FullDatatype );
      
   }
   
   void tarch::parallel::messages::NodePoolAnswerMessage::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message tarch::parallel::messages::NodePoolAnswerMessage " 
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
         msg << "was not able to send message tarch::parallel::messages::NodePoolAnswerMessage "  
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
           msg << "testing for finished send task for tarch::parallel::messages::NodePoolAnswerMessage " 
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
             "tarch::parallel::messages::NodePoolAnswerMessage", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "tarch::parallel::messages::NodePoolAnswerMessage", 
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
   
   
   
   void tarch::parallel::messages::NodePoolAnswerMessage::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive tarch::parallel::messages::NodePoolAnswerMessage from node " 
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
        msg << "failed to start to receive tarch::parallel::messages::NodePoolAnswerMessage from node " 
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
            "tarch::parallel::messages::NodePoolAnswerMessage", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "tarch::parallel::messages::NodePoolAnswerMessage", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for tarch::parallel::messages::NodePoolAnswerMessage failed: " 
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
        msg << "testing for finished receive task for tarch::parallel::messages::NodePoolAnswerMessage failed: " 
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
            "tarch::parallel::messages::NodePoolAnswerMessage", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "tarch::parallel::messages::NodePoolAnswerMessage", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for tarch::parallel::messages::NodePoolAnswerMessage failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive tarch::parallel::messages::NodePoolAnswerMessage from node " 
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
   
   
   
   bool tarch::parallel::messages::NodePoolAnswerMessage::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int tarch::parallel::messages::NodePoolAnswerMessage::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif


tarch::parallel::messages::NodePoolAnswerMessagePacked::PersistentRecords::PersistentRecords() {
   
}


tarch::parallel::messages::NodePoolAnswerMessagePacked::PersistentRecords::PersistentRecords(const int& newWorker):
_newWorker(newWorker) {
   
}


 int tarch::parallel::messages::NodePoolAnswerMessagePacked::PersistentRecords::getNewWorker() const  {
   return _newWorker;
}



 void tarch::parallel::messages::NodePoolAnswerMessagePacked::PersistentRecords::setNewWorker(const int& newWorker)  {
   _newWorker = newWorker;
}


tarch::parallel::messages::NodePoolAnswerMessagePacked::NodePoolAnswerMessagePacked() {
   
}


tarch::parallel::messages::NodePoolAnswerMessagePacked::NodePoolAnswerMessagePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._newWorker) {
   
}


tarch::parallel::messages::NodePoolAnswerMessagePacked::NodePoolAnswerMessagePacked(const int& newWorker):
_persistentRecords(newWorker) {
   
}


tarch::parallel::messages::NodePoolAnswerMessagePacked::~NodePoolAnswerMessagePacked() { }


 int tarch::parallel::messages::NodePoolAnswerMessagePacked::getNewWorker() const  {
   return _persistentRecords._newWorker;
}



 void tarch::parallel::messages::NodePoolAnswerMessagePacked::setNewWorker(const int& newWorker)  {
   _persistentRecords._newWorker = newWorker;
}




std::string tarch::parallel::messages::NodePoolAnswerMessagePacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void tarch::parallel::messages::NodePoolAnswerMessagePacked::toString (std::ostream& out) const {
   out << "("; 
   out << "newWorker:" << getNewWorker();
   out <<  ")";
}


tarch::parallel::messages::NodePoolAnswerMessagePacked::PersistentRecords tarch::parallel::messages::NodePoolAnswerMessagePacked::getPersistentRecords() const {
   return _persistentRecords;
}

tarch::parallel::messages::NodePoolAnswerMessage tarch::parallel::messages::NodePoolAnswerMessagePacked::convert() const{
   return NodePoolAnswerMessage(
      getNewWorker()
   );
}

#ifdef Parallel
   tarch::logging::Log tarch::parallel::messages::NodePoolAnswerMessagePacked::_log( "tarch::parallel::messages::NodePoolAnswerMessagePacked" );
   
   MPI_Datatype tarch::parallel::messages::NodePoolAnswerMessagePacked::Datatype = 0;
   MPI_Datatype tarch::parallel::messages::NodePoolAnswerMessagePacked::FullDatatype = 0;
   
   
   void tarch::parallel::messages::NodePoolAnswerMessagePacked::initDatatype() {
      {
         NodePoolAnswerMessagePacked dummyNodePoolAnswerMessagePacked[2];
         
         #ifdef MPI2
         const int Attributes = 1;
         #else
         const int Attributes = 2;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //newWorker
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //newWorker
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked[0]._persistentRecords._newWorker))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked[0]._persistentRecords._newWorker))), 		&disp[0] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(NodePoolAnswerMessagePacked)), i, disp[i], Attributes, sizeof(NodePoolAnswerMessagePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked[1]))), 		&disp[1] );
         disp[1] -= base;
         disp[1] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &NodePoolAnswerMessagePacked::Datatype );
         MPI_Type_commit( &NodePoolAnswerMessagePacked::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &NodePoolAnswerMessagePacked::Datatype);
         MPI_Type_commit( &NodePoolAnswerMessagePacked::Datatype );
         #endif
         
      }
      {
         NodePoolAnswerMessagePacked dummyNodePoolAnswerMessagePacked[2];
         
         #ifdef MPI2
         const int Attributes = 1;
         #else
         const int Attributes = 2;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //newWorker
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //newWorker
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked[0]._persistentRecords._newWorker))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked[0]._persistentRecords._newWorker))), 		&disp[0] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(NodePoolAnswerMessagePacked)), i, disp[i], Attributes, sizeof(NodePoolAnswerMessagePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyNodePoolAnswerMessagePacked[1]))), 		&disp[1] );
         disp[1] -= base;
         disp[1] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &NodePoolAnswerMessagePacked::FullDatatype );
         MPI_Type_commit( &NodePoolAnswerMessagePacked::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &NodePoolAnswerMessagePacked::FullDatatype);
         MPI_Type_commit( &NodePoolAnswerMessagePacked::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void tarch::parallel::messages::NodePoolAnswerMessagePacked::shutdownDatatype() {
      MPI_Type_free( &NodePoolAnswerMessagePacked::Datatype );
      MPI_Type_free( &NodePoolAnswerMessagePacked::FullDatatype );
      
   }
   
   void tarch::parallel::messages::NodePoolAnswerMessagePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message tarch::parallel::messages::NodePoolAnswerMessagePacked " 
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
         msg << "was not able to send message tarch::parallel::messages::NodePoolAnswerMessagePacked "  
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
           msg << "testing for finished send task for tarch::parallel::messages::NodePoolAnswerMessagePacked " 
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
             "tarch::parallel::messages::NodePoolAnswerMessagePacked", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "tarch::parallel::messages::NodePoolAnswerMessagePacked", 
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
   
   
   
   void tarch::parallel::messages::NodePoolAnswerMessagePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive tarch::parallel::messages::NodePoolAnswerMessagePacked from node " 
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
        msg << "failed to start to receive tarch::parallel::messages::NodePoolAnswerMessagePacked from node " 
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
            "tarch::parallel::messages::NodePoolAnswerMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "tarch::parallel::messages::NodePoolAnswerMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for tarch::parallel::messages::NodePoolAnswerMessagePacked failed: " 
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
        msg << "testing for finished receive task for tarch::parallel::messages::NodePoolAnswerMessagePacked failed: " 
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
            "tarch::parallel::messages::NodePoolAnswerMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "tarch::parallel::messages::NodePoolAnswerMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for tarch::parallel::messages::NodePoolAnswerMessagePacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive tarch::parallel::messages::NodePoolAnswerMessagePacked from node " 
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
   
   
   
   bool tarch::parallel::messages::NodePoolAnswerMessagePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int tarch::parallel::messages::NodePoolAnswerMessagePacked::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif



