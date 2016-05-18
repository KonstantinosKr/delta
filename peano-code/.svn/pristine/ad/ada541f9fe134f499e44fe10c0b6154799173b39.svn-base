#include "peano/heap/records/FloatHeapData.h"

peano::heap::records::FloatHeapData::PersistentRecords::PersistentRecords() {
   
}


peano::heap::records::FloatHeapData::PersistentRecords::PersistentRecords(const float& u):
_u(u) {
   
}


 float peano::heap::records::FloatHeapData::PersistentRecords::getU() const  {
   return _u;
}



 void peano::heap::records::FloatHeapData::PersistentRecords::setU(const float& u)  {
   _u = u;
}


peano::heap::records::FloatHeapData::FloatHeapData() {
   
}


peano::heap::records::FloatHeapData::FloatHeapData(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._u) {
   
}


peano::heap::records::FloatHeapData::FloatHeapData(const float& u):
_persistentRecords(u) {
   
}


peano::heap::records::FloatHeapData::~FloatHeapData() { }


 float peano::heap::records::FloatHeapData::getU() const  {
   return _persistentRecords._u;
}



 void peano::heap::records::FloatHeapData::setU(const float& u)  {
   _persistentRecords._u = u;
}




std::string peano::heap::records::FloatHeapData::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::heap::records::FloatHeapData::toString (std::ostream& out) const {
   out << "("; 
   out << "u:" << getU();
   out <<  ")";
}


peano::heap::records::FloatHeapData::PersistentRecords peano::heap::records::FloatHeapData::getPersistentRecords() const {
   return _persistentRecords;
}

peano::heap::records::FloatHeapDataPacked peano::heap::records::FloatHeapData::convert() const{
   return FloatHeapDataPacked(
      getU()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::heap::records::FloatHeapData::_log( "peano::heap::records::FloatHeapData" );
   
   MPI_Datatype peano::heap::records::FloatHeapData::Datatype = 0;
   MPI_Datatype peano::heap::records::FloatHeapData::FullDatatype = 0;
   
   
   void peano::heap::records::FloatHeapData::initDatatype() {
      {
         FloatHeapData dummyFloatHeapData[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_FLOAT,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapData[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapData[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapData[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &FloatHeapData::Datatype );
         MPI_Type_commit( &FloatHeapData::Datatype );
         
      }
      {
         FloatHeapData dummyFloatHeapData[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_FLOAT,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapData[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapData[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapData[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &FloatHeapData::FullDatatype );
         MPI_Type_commit( &FloatHeapData::FullDatatype );
         
      }
      
   }
   
   
   void peano::heap::records::FloatHeapData::shutdownDatatype() {
      MPI_Type_free( &FloatHeapData::Datatype );
      MPI_Type_free( &FloatHeapData::FullDatatype );
      
   }
   
   void peano::heap::records::FloatHeapData::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message peano::heap::records::FloatHeapData "
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
            msg << "was not able to send message peano::heap::records::FloatHeapData "
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
               msg << "testing for finished send task for peano::heap::records::FloatHeapData "
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
               "peano::heap::records::FloatHeapData",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::FloatHeapData",
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
   
   
   
   void peano::heap::records::FloatHeapData::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive peano::heap::records::FloatHeapData from node "
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
            msg << "failed to start to receive peano::heap::records::FloatHeapData from node "
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
               msg << "testing for finished receive task for peano::heap::records::FloatHeapData failed: "
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
               "peano::heap::records::FloatHeapData",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::FloatHeapData",
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
   
   
   
   bool peano::heap::records::FloatHeapData::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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


peano::heap::records::FloatHeapDataPacked::PersistentRecords::PersistentRecords() {
   
}


peano::heap::records::FloatHeapDataPacked::PersistentRecords::PersistentRecords(const float& u):
_u(u) {
   
}


 float peano::heap::records::FloatHeapDataPacked::PersistentRecords::getU() const  {
   return _u;
}



 void peano::heap::records::FloatHeapDataPacked::PersistentRecords::setU(const float& u)  {
   _u = u;
}


peano::heap::records::FloatHeapDataPacked::FloatHeapDataPacked() {
   
}


peano::heap::records::FloatHeapDataPacked::FloatHeapDataPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._u) {
   
}


peano::heap::records::FloatHeapDataPacked::FloatHeapDataPacked(const float& u):
_persistentRecords(u) {
   
}


peano::heap::records::FloatHeapDataPacked::~FloatHeapDataPacked() { }


 float peano::heap::records::FloatHeapDataPacked::getU() const  {
   return _persistentRecords._u;
}



 void peano::heap::records::FloatHeapDataPacked::setU(const float& u)  {
   _persistentRecords._u = u;
}




std::string peano::heap::records::FloatHeapDataPacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::heap::records::FloatHeapDataPacked::toString (std::ostream& out) const {
   out << "("; 
   out << "u:" << getU();
   out <<  ")";
}


peano::heap::records::FloatHeapDataPacked::PersistentRecords peano::heap::records::FloatHeapDataPacked::getPersistentRecords() const {
   return _persistentRecords;
}

peano::heap::records::FloatHeapData peano::heap::records::FloatHeapDataPacked::convert() const{
   return FloatHeapData(
      getU()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::heap::records::FloatHeapDataPacked::_log( "peano::heap::records::FloatHeapDataPacked" );
   
   MPI_Datatype peano::heap::records::FloatHeapDataPacked::Datatype = 0;
   MPI_Datatype peano::heap::records::FloatHeapDataPacked::FullDatatype = 0;
   
   
   void peano::heap::records::FloatHeapDataPacked::initDatatype() {
      {
         FloatHeapDataPacked dummyFloatHeapDataPacked[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_FLOAT,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapDataPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapDataPacked[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapDataPacked[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &FloatHeapDataPacked::Datatype );
         MPI_Type_commit( &FloatHeapDataPacked::Datatype );
         
      }
      {
         FloatHeapDataPacked dummyFloatHeapDataPacked[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_FLOAT,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapDataPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapDataPacked[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyFloatHeapDataPacked[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &FloatHeapDataPacked::FullDatatype );
         MPI_Type_commit( &FloatHeapDataPacked::FullDatatype );
         
      }
      
   }
   
   
   void peano::heap::records::FloatHeapDataPacked::shutdownDatatype() {
      MPI_Type_free( &FloatHeapDataPacked::Datatype );
      MPI_Type_free( &FloatHeapDataPacked::FullDatatype );
      
   }
   
   void peano::heap::records::FloatHeapDataPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message peano::heap::records::FloatHeapDataPacked "
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
            msg << "was not able to send message peano::heap::records::FloatHeapDataPacked "
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
               msg << "testing for finished send task for peano::heap::records::FloatHeapDataPacked "
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
               "peano::heap::records::FloatHeapDataPacked",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::FloatHeapDataPacked",
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
   
   
   
   void peano::heap::records::FloatHeapDataPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive peano::heap::records::FloatHeapDataPacked from node "
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
            msg << "failed to start to receive peano::heap::records::FloatHeapDataPacked from node "
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
               msg << "testing for finished receive task for peano::heap::records::FloatHeapDataPacked failed: "
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
               "peano::heap::records::FloatHeapDataPacked",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::FloatHeapDataPacked",
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
   
   
   
   bool peano::heap::records::FloatHeapDataPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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



