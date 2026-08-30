#include "peano/parallel/loadbalancing/WorkerEntry.h"

peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::PersistentRecords() {
   
}


peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::PersistentRecords(const int& rank, const int& level, const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset, const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize):
_rank(rank),
_level(level),
_boundingBoxOffset(boundingBoxOffset),
_boundingBoxSize(boundingBoxSize) {
   
}


 int peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::getRank() const  {
   return _rank;
}



 void peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::setRank(const int& rank)  {
   _rank = rank;
}



 int peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::getLevel() const  {
   return _level;
}



 void peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::setLevel(const int& level)  {
   _level = level;
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::getBoundingBoxOffset() const  {
   return _boundingBoxOffset;
}



 void peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::setBoundingBoxOffset(const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset)  {
   _boundingBoxOffset = (boundingBoxOffset);
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::getBoundingBoxSize() const  {
   return _boundingBoxSize;
}



 void peano::parallel::loadbalancing::WorkerEntry::PersistentRecords::setBoundingBoxSize(const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize)  {
   _boundingBoxSize = (boundingBoxSize);
}


peano::parallel::loadbalancing::WorkerEntry::WorkerEntry() {
   
}


peano::parallel::loadbalancing::WorkerEntry::WorkerEntry(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._rank, persistentRecords._level, persistentRecords._boundingBoxOffset, persistentRecords._boundingBoxSize) {
   
}


peano::parallel::loadbalancing::WorkerEntry::WorkerEntry(const int& rank, const int& level, const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset, const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize):
_persistentRecords(rank, level, boundingBoxOffset, boundingBoxSize) {
   
}


peano::parallel::loadbalancing::WorkerEntry::~WorkerEntry() { }


 int peano::parallel::loadbalancing::WorkerEntry::getRank() const  {
   return _persistentRecords._rank;
}



 void peano::parallel::loadbalancing::WorkerEntry::setRank(const int& rank)  {
   _persistentRecords._rank = rank;
}



 int peano::parallel::loadbalancing::WorkerEntry::getLevel() const  {
   return _persistentRecords._level;
}



 void peano::parallel::loadbalancing::WorkerEntry::setLevel(const int& level)  {
   _persistentRecords._level = level;
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::loadbalancing::WorkerEntry::getBoundingBoxOffset() const  {
   return _persistentRecords._boundingBoxOffset;
}



 void peano::parallel::loadbalancing::WorkerEntry::setBoundingBoxOffset(const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset)  {
   _persistentRecords._boundingBoxOffset = (boundingBoxOffset);
}



 double peano::parallel::loadbalancing::WorkerEntry::getBoundingBoxOffset(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._boundingBoxOffset[elementIndex];
   
}



 void peano::parallel::loadbalancing::WorkerEntry::setBoundingBoxOffset(int elementIndex, const double& boundingBoxOffset)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._boundingBoxOffset[elementIndex]= boundingBoxOffset;
   
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::loadbalancing::WorkerEntry::getBoundingBoxSize() const  {
   return _persistentRecords._boundingBoxSize;
}



 void peano::parallel::loadbalancing::WorkerEntry::setBoundingBoxSize(const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize)  {
   _persistentRecords._boundingBoxSize = (boundingBoxSize);
}



 double peano::parallel::loadbalancing::WorkerEntry::getBoundingBoxSize(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._boundingBoxSize[elementIndex];
   
}



 void peano::parallel::loadbalancing::WorkerEntry::setBoundingBoxSize(int elementIndex, const double& boundingBoxSize)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._boundingBoxSize[elementIndex]= boundingBoxSize;
   
}




std::string peano::parallel::loadbalancing::WorkerEntry::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::parallel::loadbalancing::WorkerEntry::toString (std::ostream& out) const {
   out << "("; 
   out << "rank:" << getRank();
   out << ",";
   out << "level:" << getLevel();
   out << ",";
   out << "boundingBoxOffset:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getBoundingBoxOffset(i) << ",";
   }
   out << getBoundingBoxOffset(DIMENSIONS-1) << "]";
   out << ",";
   out << "boundingBoxSize:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getBoundingBoxSize(i) << ",";
   }
   out << getBoundingBoxSize(DIMENSIONS-1) << "]";
   out <<  ")";
}


peano::parallel::loadbalancing::WorkerEntry::PersistentRecords peano::parallel::loadbalancing::WorkerEntry::getPersistentRecords() const {
   return _persistentRecords;
}

peano::parallel::loadbalancing::WorkerEntryPacked peano::parallel::loadbalancing::WorkerEntry::convert() const{
   return WorkerEntryPacked(
      getRank(),
      getLevel(),
      getBoundingBoxOffset(),
      getBoundingBoxSize()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::parallel::loadbalancing::WorkerEntry::_log( "peano::parallel::loadbalancing::WorkerEntry" );
   
   MPI_Datatype peano::parallel::loadbalancing::WorkerEntry::Datatype = 0;
   MPI_Datatype peano::parallel::loadbalancing::WorkerEntry::FullDatatype = 0;
   
   
   void peano::parallel::loadbalancing::WorkerEntry::initDatatype() {
      {
         WorkerEntry dummyWorkerEntry[2];
         
         #ifdef MPI2
         const int Attributes = 4;
         #else
         const int Attributes = 5;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //rank
            , MPI_INT		 //level
            , MPI_DOUBLE		 //boundingBoxOffset
            , MPI_DOUBLE		 //boundingBoxSize
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //rank
            , 1		 //level
            , DIMENSIONS		 //boundingBoxOffset
            , DIMENSIONS		 //boundingBoxSize
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._rank))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._rank))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._level))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._level))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._boundingBoxOffset[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._boundingBoxOffset[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._boundingBoxSize[0]))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._boundingBoxSize[0]))), 		&disp[3] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(WorkerEntry)), i, disp[i], Attributes, sizeof(WorkerEntry));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[1]))), 		&disp[4] );
         disp[4] -= base;
         disp[4] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &WorkerEntry::Datatype );
         MPI_Type_commit( &WorkerEntry::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &WorkerEntry::Datatype);
         MPI_Type_commit( &WorkerEntry::Datatype );
         #endif
         
      }
      {
         WorkerEntry dummyWorkerEntry[2];
         
         #ifdef MPI2
         const int Attributes = 4;
         #else
         const int Attributes = 5;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //rank
            , MPI_INT		 //level
            , MPI_DOUBLE		 //boundingBoxOffset
            , MPI_DOUBLE		 //boundingBoxSize
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //rank
            , 1		 //level
            , DIMENSIONS		 //boundingBoxOffset
            , DIMENSIONS		 //boundingBoxSize
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._rank))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._rank))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._level))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._level))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._boundingBoxOffset[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._boundingBoxOffset[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._boundingBoxSize[0]))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[0]._persistentRecords._boundingBoxSize[0]))), 		&disp[3] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(WorkerEntry)), i, disp[i], Attributes, sizeof(WorkerEntry));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntry[1]))), 		&disp[4] );
         disp[4] -= base;
         disp[4] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &WorkerEntry::FullDatatype );
         MPI_Type_commit( &WorkerEntry::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &WorkerEntry::FullDatatype);
         MPI_Type_commit( &WorkerEntry::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void peano::parallel::loadbalancing::WorkerEntry::shutdownDatatype() {
      MPI_Type_free( &WorkerEntry::Datatype );
      MPI_Type_free( &WorkerEntry::FullDatatype );
      
   }
   
   void peano::parallel::loadbalancing::WorkerEntry::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message peano::parallel::loadbalancing::WorkerEntry " 
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
         msg << "was not able to send message peano::parallel::loadbalancing::WorkerEntry "  
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
           msg << "testing for finished send task for peano::parallel::loadbalancing::WorkerEntry " 
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
             "peano::parallel::loadbalancing::WorkerEntry", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "peano::parallel::loadbalancing::WorkerEntry", 
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
   
   
   
   void peano::parallel::loadbalancing::WorkerEntry::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive peano::parallel::loadbalancing::WorkerEntry from node " 
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
        msg << "failed to start to receive peano::parallel::loadbalancing::WorkerEntry from node " 
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
          msg << "testing for finished receive task for peano::parallel::loadbalancing::WorkerEntry failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::parallel::loadbalancing::WorkerEntry", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::parallel::loadbalancing::WorkerEntry", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
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
        msg << "testing for finished receive task for peano::parallel::loadbalancing::WorkerEntry failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::parallel::loadbalancing::WorkerEntry failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::parallel::loadbalancing::WorkerEntry", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::parallel::loadbalancing::WorkerEntry", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive peano::parallel::loadbalancing::WorkerEntry from node " 
            << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
    }
    break; 
  } 
  _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source; 
 // =========================== 
// end injected snippet/aspect 
// =========================== 

      
   
   
   
   }
   bool peano::parallel::loadbalancing::WorkerEntry::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int peano::parallel::loadbalancing::WorkerEntry::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif


peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::PersistentRecords() {
   
}


peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::PersistentRecords(const int& rank, const int& level, const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset, const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize):
_rank(rank),
_level(level),
_boundingBoxOffset(boundingBoxOffset),
_boundingBoxSize(boundingBoxSize) {
   
}


 int peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::getRank() const  {
   return _rank;
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::setRank(const int& rank)  {
   _rank = rank;
}



 int peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::getLevel() const  {
   return _level;
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::setLevel(const int& level)  {
   _level = level;
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::getBoundingBoxOffset() const  {
   return _boundingBoxOffset;
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::setBoundingBoxOffset(const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset)  {
   _boundingBoxOffset = (boundingBoxOffset);
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::getBoundingBoxSize() const  {
   return _boundingBoxSize;
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords::setBoundingBoxSize(const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize)  {
   _boundingBoxSize = (boundingBoxSize);
}


peano::parallel::loadbalancing::WorkerEntryPacked::WorkerEntryPacked() {
   
}


peano::parallel::loadbalancing::WorkerEntryPacked::WorkerEntryPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._rank, persistentRecords._level, persistentRecords._boundingBoxOffset, persistentRecords._boundingBoxSize) {
   
}


peano::parallel::loadbalancing::WorkerEntryPacked::WorkerEntryPacked(const int& rank, const int& level, const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset, const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize):
_persistentRecords(rank, level, boundingBoxOffset, boundingBoxSize) {
   
}


peano::parallel::loadbalancing::WorkerEntryPacked::~WorkerEntryPacked() { }


 int peano::parallel::loadbalancing::WorkerEntryPacked::getRank() const  {
   return _persistentRecords._rank;
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::setRank(const int& rank)  {
   _persistentRecords._rank = rank;
}



 int peano::parallel::loadbalancing::WorkerEntryPacked::getLevel() const  {
   return _persistentRecords._level;
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::setLevel(const int& level)  {
   _persistentRecords._level = level;
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::loadbalancing::WorkerEntryPacked::getBoundingBoxOffset() const  {
   return _persistentRecords._boundingBoxOffset;
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::setBoundingBoxOffset(const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset)  {
   _persistentRecords._boundingBoxOffset = (boundingBoxOffset);
}



 double peano::parallel::loadbalancing::WorkerEntryPacked::getBoundingBoxOffset(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._boundingBoxOffset[elementIndex];
   
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::setBoundingBoxOffset(int elementIndex, const double& boundingBoxOffset)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._boundingBoxOffset[elementIndex]= boundingBoxOffset;
   
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::loadbalancing::WorkerEntryPacked::getBoundingBoxSize() const  {
   return _persistentRecords._boundingBoxSize;
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::setBoundingBoxSize(const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize)  {
   _persistentRecords._boundingBoxSize = (boundingBoxSize);
}



 double peano::parallel::loadbalancing::WorkerEntryPacked::getBoundingBoxSize(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._boundingBoxSize[elementIndex];
   
}



 void peano::parallel::loadbalancing::WorkerEntryPacked::setBoundingBoxSize(int elementIndex, const double& boundingBoxSize)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._boundingBoxSize[elementIndex]= boundingBoxSize;
   
}




std::string peano::parallel::loadbalancing::WorkerEntryPacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::parallel::loadbalancing::WorkerEntryPacked::toString (std::ostream& out) const {
   out << "("; 
   out << "rank:" << getRank();
   out << ",";
   out << "level:" << getLevel();
   out << ",";
   out << "boundingBoxOffset:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getBoundingBoxOffset(i) << ",";
   }
   out << getBoundingBoxOffset(DIMENSIONS-1) << "]";
   out << ",";
   out << "boundingBoxSize:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getBoundingBoxSize(i) << ",";
   }
   out << getBoundingBoxSize(DIMENSIONS-1) << "]";
   out <<  ")";
}


peano::parallel::loadbalancing::WorkerEntryPacked::PersistentRecords peano::parallel::loadbalancing::WorkerEntryPacked::getPersistentRecords() const {
   return _persistentRecords;
}

peano::parallel::loadbalancing::WorkerEntry peano::parallel::loadbalancing::WorkerEntryPacked::convert() const{
   return WorkerEntry(
      getRank(),
      getLevel(),
      getBoundingBoxOffset(),
      getBoundingBoxSize()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::parallel::loadbalancing::WorkerEntryPacked::_log( "peano::parallel::loadbalancing::WorkerEntryPacked" );
   
   MPI_Datatype peano::parallel::loadbalancing::WorkerEntryPacked::Datatype = 0;
   MPI_Datatype peano::parallel::loadbalancing::WorkerEntryPacked::FullDatatype = 0;
   
   
   void peano::parallel::loadbalancing::WorkerEntryPacked::initDatatype() {
      {
         WorkerEntryPacked dummyWorkerEntryPacked[2];
         
         #ifdef MPI2
         const int Attributes = 4;
         #else
         const int Attributes = 5;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //rank
            , MPI_INT		 //level
            , MPI_DOUBLE		 //boundingBoxOffset
            , MPI_DOUBLE		 //boundingBoxSize
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //rank
            , 1		 //level
            , DIMENSIONS		 //boundingBoxOffset
            , DIMENSIONS		 //boundingBoxSize
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._rank))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._rank))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._level))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._level))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._boundingBoxOffset[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._boundingBoxOffset[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._boundingBoxSize[0]))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._boundingBoxSize[0]))), 		&disp[3] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(WorkerEntryPacked)), i, disp[i], Attributes, sizeof(WorkerEntryPacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[1]))), 		&disp[4] );
         disp[4] -= base;
         disp[4] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &WorkerEntryPacked::Datatype );
         MPI_Type_commit( &WorkerEntryPacked::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &WorkerEntryPacked::Datatype);
         MPI_Type_commit( &WorkerEntryPacked::Datatype );
         #endif
         
      }
      {
         WorkerEntryPacked dummyWorkerEntryPacked[2];
         
         #ifdef MPI2
         const int Attributes = 4;
         #else
         const int Attributes = 5;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_INT		 //rank
            , MPI_INT		 //level
            , MPI_DOUBLE		 //boundingBoxOffset
            , MPI_DOUBLE		 //boundingBoxSize
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              1		 //rank
            , 1		 //level
            , DIMENSIONS		 //boundingBoxOffset
            , DIMENSIONS		 //boundingBoxSize
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._rank))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._rank))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._level))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._level))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._boundingBoxOffset[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._boundingBoxOffset[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._boundingBoxSize[0]))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[0]._persistentRecords._boundingBoxSize[0]))), 		&disp[3] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(WorkerEntryPacked)), i, disp[i], Attributes, sizeof(WorkerEntryPacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyWorkerEntryPacked[1]))), 		&disp[4] );
         disp[4] -= base;
         disp[4] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &WorkerEntryPacked::FullDatatype );
         MPI_Type_commit( &WorkerEntryPacked::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &WorkerEntryPacked::FullDatatype);
         MPI_Type_commit( &WorkerEntryPacked::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void peano::parallel::loadbalancing::WorkerEntryPacked::shutdownDatatype() {
      MPI_Type_free( &WorkerEntryPacked::Datatype );
      MPI_Type_free( &WorkerEntryPacked::FullDatatype );
      
   }
   
   void peano::parallel::loadbalancing::WorkerEntryPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message peano::parallel::loadbalancing::WorkerEntryPacked " 
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
         msg << "was not able to send message peano::parallel::loadbalancing::WorkerEntryPacked "  
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
           msg << "testing for finished send task for peano::parallel::loadbalancing::WorkerEntryPacked " 
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
             "peano::parallel::loadbalancing::WorkerEntryPacked", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "peano::parallel::loadbalancing::WorkerEntryPacked", 
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
   
   
   
   void peano::parallel::loadbalancing::WorkerEntryPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive peano::parallel::loadbalancing::WorkerEntryPacked from node " 
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
        msg << "failed to start to receive peano::parallel::loadbalancing::WorkerEntryPacked from node " 
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
          msg << "testing for finished receive task for peano::parallel::loadbalancing::WorkerEntryPacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::parallel::loadbalancing::WorkerEntryPacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::parallel::loadbalancing::WorkerEntryPacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
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
        msg << "testing for finished receive task for peano::parallel::loadbalancing::WorkerEntryPacked failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::parallel::loadbalancing::WorkerEntryPacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::parallel::loadbalancing::WorkerEntryPacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::parallel::loadbalancing::WorkerEntryPacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive peano::parallel::loadbalancing::WorkerEntryPacked from node " 
            << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
    }
    break; 
  } 
  _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source; 
 // =========================== 
// end injected snippet/aspect 
// =========================== 

      
   
   
   
   }
   bool peano::parallel::loadbalancing::WorkerEntryPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int peano::parallel::loadbalancing::WorkerEntryPacked::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif



