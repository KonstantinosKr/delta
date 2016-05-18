#include "peano/heap/records/BooleanHeapData.h"

peano::heap::records::BooleanHeapData::PersistentRecords::PersistentRecords() {
   
}


peano::heap::records::BooleanHeapData::PersistentRecords::PersistentRecords(const bool& u):
_u(u) {
   
}


 bool peano::heap::records::BooleanHeapData::PersistentRecords::getU() const  {
   return _u;
}



 void peano::heap::records::BooleanHeapData::PersistentRecords::setU(const bool& u)  {
   _u = u;
}


peano::heap::records::BooleanHeapData::BooleanHeapData() {
   
}


peano::heap::records::BooleanHeapData::BooleanHeapData(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._u) {
   
}


peano::heap::records::BooleanHeapData::BooleanHeapData(const bool& u):
_persistentRecords(u) {
   
}


peano::heap::records::BooleanHeapData::~BooleanHeapData() { }


 bool peano::heap::records::BooleanHeapData::getU() const  {
   return _persistentRecords._u;
}



 void peano::heap::records::BooleanHeapData::setU(const bool& u)  {
   _persistentRecords._u = u;
}




std::string peano::heap::records::BooleanHeapData::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::heap::records::BooleanHeapData::toString (std::ostream& out) const {
   out << "("; 
   out << "u:" << getU();
   out <<  ")";
}


peano::heap::records::BooleanHeapData::PersistentRecords peano::heap::records::BooleanHeapData::getPersistentRecords() const {
   return _persistentRecords;
}

peano::heap::records::BooleanHeapDataPacked peano::heap::records::BooleanHeapData::convert() const{
   return BooleanHeapDataPacked(
      getU()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::heap::records::BooleanHeapData::_log( "peano::heap::records::BooleanHeapData" );
   
   MPI_Datatype peano::heap::records::BooleanHeapData::Datatype = 0;
   MPI_Datatype peano::heap::records::BooleanHeapData::FullDatatype = 0;
   
   
   void peano::heap::records::BooleanHeapData::initDatatype() {
      {
         BooleanHeapData dummyBooleanHeapData[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_CHAR,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapData[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapData[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapData[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &BooleanHeapData::Datatype );
         MPI_Type_commit( &BooleanHeapData::Datatype );
         
      }
      {
         BooleanHeapData dummyBooleanHeapData[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_CHAR,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapData[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapData[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapData[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &BooleanHeapData::FullDatatype );
         MPI_Type_commit( &BooleanHeapData::FullDatatype );
         
      }
      
   }
   
   
   void peano::heap::records::BooleanHeapData::shutdownDatatype() {
      MPI_Type_free( &BooleanHeapData::Datatype );
      MPI_Type_free( &BooleanHeapData::FullDatatype );
      
   }
   
   void peano::heap::records::BooleanHeapData::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message peano::heap::records::BooleanHeapData "
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
            msg << "was not able to send message peano::heap::records::BooleanHeapData "
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
               msg << "testing for finished send task for peano::heap::records::BooleanHeapData "
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
               "peano::heap::records::BooleanHeapData",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::BooleanHeapData",
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
   
   
   
   void peano::heap::records::BooleanHeapData::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive peano::heap::records::BooleanHeapData from node "
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
            msg << "failed to start to receive peano::heap::records::BooleanHeapData from node "
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
               msg << "testing for finished receive task for peano::heap::records::BooleanHeapData failed: "
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
               "peano::heap::records::BooleanHeapData",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::BooleanHeapData",
               "receive(int)", source,tag,1
               );
            }
            tarch::parallel::Node::getInstance().receiveDanglingMessages();
            usleep(communicateSleep);
            
         }
         
         delete sendRequestHandle;
         
         #ifdef Debug
         _log.debug("receive(int,int)", "received " + toString() ); 
         #endif
         
      }
      
   }
   
   
   
   bool peano::heap::records::BooleanHeapData::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   
#endif


peano::heap::records::BooleanHeapDataPacked::PersistentRecords::PersistentRecords() {
   
}


peano::heap::records::BooleanHeapDataPacked::PersistentRecords::PersistentRecords(const bool& u):
_u(u) {
   
}


 bool peano::heap::records::BooleanHeapDataPacked::PersistentRecords::getU() const  {
   return _u;
}



 void peano::heap::records::BooleanHeapDataPacked::PersistentRecords::setU(const bool& u)  {
   _u = u;
}


peano::heap::records::BooleanHeapDataPacked::BooleanHeapDataPacked() {
   
}


peano::heap::records::BooleanHeapDataPacked::BooleanHeapDataPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._u) {
   
}


peano::heap::records::BooleanHeapDataPacked::BooleanHeapDataPacked(const bool& u):
_persistentRecords(u) {
   
}


peano::heap::records::BooleanHeapDataPacked::~BooleanHeapDataPacked() { }


 bool peano::heap::records::BooleanHeapDataPacked::getU() const  {
   return _persistentRecords._u;
}



 void peano::heap::records::BooleanHeapDataPacked::setU(const bool& u)  {
   _persistentRecords._u = u;
}




std::string peano::heap::records::BooleanHeapDataPacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::heap::records::BooleanHeapDataPacked::toString (std::ostream& out) const {
   out << "("; 
   out << "u:" << getU();
   out <<  ")";
}


peano::heap::records::BooleanHeapDataPacked::PersistentRecords peano::heap::records::BooleanHeapDataPacked::getPersistentRecords() const {
   return _persistentRecords;
}

peano::heap::records::BooleanHeapData peano::heap::records::BooleanHeapDataPacked::convert() const{
   return BooleanHeapData(
      getU()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::heap::records::BooleanHeapDataPacked::_log( "peano::heap::records::BooleanHeapDataPacked" );
   
   MPI_Datatype peano::heap::records::BooleanHeapDataPacked::Datatype = 0;
   MPI_Datatype peano::heap::records::BooleanHeapDataPacked::FullDatatype = 0;
   
   
   void peano::heap::records::BooleanHeapDataPacked::initDatatype() {
      {
         BooleanHeapDataPacked dummyBooleanHeapDataPacked[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_CHAR,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapDataPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapDataPacked[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapDataPacked[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &BooleanHeapDataPacked::Datatype );
         MPI_Type_commit( &BooleanHeapDataPacked::Datatype );
         
      }
      {
         BooleanHeapDataPacked dummyBooleanHeapDataPacked[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_CHAR,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapDataPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapDataPacked[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyBooleanHeapDataPacked[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &BooleanHeapDataPacked::FullDatatype );
         MPI_Type_commit( &BooleanHeapDataPacked::FullDatatype );
         
      }
      
   }
   
   
   void peano::heap::records::BooleanHeapDataPacked::shutdownDatatype() {
      MPI_Type_free( &BooleanHeapDataPacked::Datatype );
      MPI_Type_free( &BooleanHeapDataPacked::FullDatatype );
      
   }
   
   void peano::heap::records::BooleanHeapDataPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message peano::heap::records::BooleanHeapDataPacked "
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
            msg << "was not able to send message peano::heap::records::BooleanHeapDataPacked "
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
               msg << "testing for finished send task for peano::heap::records::BooleanHeapDataPacked "
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
               "peano::heap::records::BooleanHeapDataPacked",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::BooleanHeapDataPacked",
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
   
   
   
   void peano::heap::records::BooleanHeapDataPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive peano::heap::records::BooleanHeapDataPacked from node "
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
            msg << "failed to start to receive peano::heap::records::BooleanHeapDataPacked from node "
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
               msg << "testing for finished receive task for peano::heap::records::BooleanHeapDataPacked failed: "
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
               "peano::heap::records::BooleanHeapDataPacked",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::BooleanHeapDataPacked",
               "receive(int)", source,tag,1
               );
            }
            tarch::parallel::Node::getInstance().receiveDanglingMessages();
            usleep(communicateSleep);
            
         }
         
         delete sendRequestHandle;
         
         #ifdef Debug
         _log.debug("receive(int,int)", "received " + toString() ); 
         #endif
         
      }
      
   }
   
   
   
   bool peano::heap::records::BooleanHeapDataPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   
#endif



