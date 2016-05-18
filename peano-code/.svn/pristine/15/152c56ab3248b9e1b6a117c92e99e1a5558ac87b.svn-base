#include "peano/heap/records/IntegerHeapData.h"

peano::heap::records::IntegerHeapData::PersistentRecords::PersistentRecords() {
   
}


peano::heap::records::IntegerHeapData::PersistentRecords::PersistentRecords(const int& u):
_u(u) {
   
}


 int peano::heap::records::IntegerHeapData::PersistentRecords::getU() const  {
   return _u;
}



 void peano::heap::records::IntegerHeapData::PersistentRecords::setU(const int& u)  {
   _u = u;
}


peano::heap::records::IntegerHeapData::IntegerHeapData() {
   
}


peano::heap::records::IntegerHeapData::IntegerHeapData(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._u) {
   
}


peano::heap::records::IntegerHeapData::IntegerHeapData(const int& u):
_persistentRecords(u) {
   
}


peano::heap::records::IntegerHeapData::~IntegerHeapData() { }


 int peano::heap::records::IntegerHeapData::getU() const  {
   return _persistentRecords._u;
}



 void peano::heap::records::IntegerHeapData::setU(const int& u)  {
   _persistentRecords._u = u;
}




std::string peano::heap::records::IntegerHeapData::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::heap::records::IntegerHeapData::toString (std::ostream& out) const {
   out << "("; 
   out << "u:" << getU();
   out <<  ")";
}


peano::heap::records::IntegerHeapData::PersistentRecords peano::heap::records::IntegerHeapData::getPersistentRecords() const {
   return _persistentRecords;
}

peano::heap::records::IntegerHeapDataPacked peano::heap::records::IntegerHeapData::convert() const{
   return IntegerHeapDataPacked(
      getU()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::heap::records::IntegerHeapData::_log( "peano::heap::records::IntegerHeapData" );
   
   MPI_Datatype peano::heap::records::IntegerHeapData::Datatype = 0;
   MPI_Datatype peano::heap::records::IntegerHeapData::FullDatatype = 0;
   
   
   void peano::heap::records::IntegerHeapData::initDatatype() {
      {
         IntegerHeapData dummyIntegerHeapData[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_INT,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapData[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapData[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapData[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &IntegerHeapData::Datatype );
         MPI_Type_commit( &IntegerHeapData::Datatype );
         
      }
      {
         IntegerHeapData dummyIntegerHeapData[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_INT,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapData[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapData[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapData[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &IntegerHeapData::FullDatatype );
         MPI_Type_commit( &IntegerHeapData::FullDatatype );
         
      }
      
   }
   
   
   void peano::heap::records::IntegerHeapData::shutdownDatatype() {
      MPI_Type_free( &IntegerHeapData::Datatype );
      MPI_Type_free( &IntegerHeapData::FullDatatype );
      
   }
   
   void peano::heap::records::IntegerHeapData::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message peano::heap::records::IntegerHeapData "
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
            msg << "was not able to send message peano::heap::records::IntegerHeapData "
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
               msg << "testing for finished send task for peano::heap::records::IntegerHeapData "
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
               "peano::heap::records::IntegerHeapData",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::IntegerHeapData",
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
   
   
   
   void peano::heap::records::IntegerHeapData::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive peano::heap::records::IntegerHeapData from node "
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
            msg << "failed to start to receive peano::heap::records::IntegerHeapData from node "
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
               msg << "testing for finished receive task for peano::heap::records::IntegerHeapData failed: "
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
               "peano::heap::records::IntegerHeapData",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::IntegerHeapData",
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
   
   
   
   bool peano::heap::records::IntegerHeapData::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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


peano::heap::records::IntegerHeapDataPacked::PersistentRecords::PersistentRecords() {
   
}


peano::heap::records::IntegerHeapDataPacked::PersistentRecords::PersistentRecords(const int& u):
_u(u) {
   
}


 int peano::heap::records::IntegerHeapDataPacked::PersistentRecords::getU() const  {
   return _u;
}



 void peano::heap::records::IntegerHeapDataPacked::PersistentRecords::setU(const int& u)  {
   _u = u;
}


peano::heap::records::IntegerHeapDataPacked::IntegerHeapDataPacked() {
   
}


peano::heap::records::IntegerHeapDataPacked::IntegerHeapDataPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._u) {
   
}


peano::heap::records::IntegerHeapDataPacked::IntegerHeapDataPacked(const int& u):
_persistentRecords(u) {
   
}


peano::heap::records::IntegerHeapDataPacked::~IntegerHeapDataPacked() { }


 int peano::heap::records::IntegerHeapDataPacked::getU() const  {
   return _persistentRecords._u;
}



 void peano::heap::records::IntegerHeapDataPacked::setU(const int& u)  {
   _persistentRecords._u = u;
}




std::string peano::heap::records::IntegerHeapDataPacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::heap::records::IntegerHeapDataPacked::toString (std::ostream& out) const {
   out << "("; 
   out << "u:" << getU();
   out <<  ")";
}


peano::heap::records::IntegerHeapDataPacked::PersistentRecords peano::heap::records::IntegerHeapDataPacked::getPersistentRecords() const {
   return _persistentRecords;
}

peano::heap::records::IntegerHeapData peano::heap::records::IntegerHeapDataPacked::convert() const{
   return IntegerHeapData(
      getU()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::heap::records::IntegerHeapDataPacked::_log( "peano::heap::records::IntegerHeapDataPacked" );
   
   MPI_Datatype peano::heap::records::IntegerHeapDataPacked::Datatype = 0;
   MPI_Datatype peano::heap::records::IntegerHeapDataPacked::FullDatatype = 0;
   
   
   void peano::heap::records::IntegerHeapDataPacked::initDatatype() {
      {
         IntegerHeapDataPacked dummyIntegerHeapDataPacked[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_INT,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapDataPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapDataPacked[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapDataPacked[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &IntegerHeapDataPacked::Datatype );
         MPI_Type_commit( &IntegerHeapDataPacked::Datatype );
         
      }
      {
         IntegerHeapDataPacked dummyIntegerHeapDataPacked[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_INT,		 //u
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //u
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapDataPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapDataPacked[0]._persistentRecords._u))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyIntegerHeapDataPacked[1]._persistentRecords._u))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &IntegerHeapDataPacked::FullDatatype );
         MPI_Type_commit( &IntegerHeapDataPacked::FullDatatype );
         
      }
      
   }
   
   
   void peano::heap::records::IntegerHeapDataPacked::shutdownDatatype() {
      MPI_Type_free( &IntegerHeapDataPacked::Datatype );
      MPI_Type_free( &IntegerHeapDataPacked::FullDatatype );
      
   }
   
   void peano::heap::records::IntegerHeapDataPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message peano::heap::records::IntegerHeapDataPacked "
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
            msg << "was not able to send message peano::heap::records::IntegerHeapDataPacked "
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
               msg << "testing for finished send task for peano::heap::records::IntegerHeapDataPacked "
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
               "peano::heap::records::IntegerHeapDataPacked",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::IntegerHeapDataPacked",
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
   
   
   
   void peano::heap::records::IntegerHeapDataPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive peano::heap::records::IntegerHeapDataPacked from node "
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
            msg << "failed to start to receive peano::heap::records::IntegerHeapDataPacked from node "
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
               msg << "testing for finished receive task for peano::heap::records::IntegerHeapDataPacked failed: "
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
               "peano::heap::records::IntegerHeapDataPacked",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::heap::records::IntegerHeapDataPacked",
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
   
   
   
   bool peano::heap::records::IntegerHeapDataPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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



