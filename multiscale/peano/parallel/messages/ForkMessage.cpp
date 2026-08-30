#include "peano/parallel/messages/ForkMessage.h"

peano::parallel::messages::ForkMessage::PersistentRecords::PersistentRecords() {
   
}


peano::parallel::messages::ForkMessage::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& domainOffset, const tarch::la::Vector<DIMENSIONS,double>& h, const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& adjacencyData, const int& level, const tarch::la::Vector<DIMENSIONS,int>& positionOfFineGridCellRelativeToCoarseGridCell):
_domainOffset(domainOffset),
_h(h),
_adjacencyData(adjacencyData),
_level(level),
_positionOfFineGridCellRelativeToCoarseGridCell(positionOfFineGridCellRelativeToCoarseGridCell) {
   
}


 tarch::la::Vector<DIMENSIONS,double> peano::parallel::messages::ForkMessage::PersistentRecords::getDomainOffset() const  {
   return _domainOffset;
}



 void peano::parallel::messages::ForkMessage::PersistentRecords::setDomainOffset(const tarch::la::Vector<DIMENSIONS,double>& domainOffset)  {
   _domainOffset = (domainOffset);
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::messages::ForkMessage::PersistentRecords::getH() const  {
   return _h;
}



 void peano::parallel::messages::ForkMessage::PersistentRecords::setH(const tarch::la::Vector<DIMENSIONS,double>& h)  {
   _h = (h);
}



 tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int> peano::parallel::messages::ForkMessage::PersistentRecords::getAdjacencyData() const  {
   return _adjacencyData;
}



 void peano::parallel::messages::ForkMessage::PersistentRecords::setAdjacencyData(const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& adjacencyData)  {
   _adjacencyData = (adjacencyData);
}



 int peano::parallel::messages::ForkMessage::PersistentRecords::getLevel() const  {
   return _level;
}



 void peano::parallel::messages::ForkMessage::PersistentRecords::setLevel(const int& level)  {
   _level = level;
}



 tarch::la::Vector<DIMENSIONS,int> peano::parallel::messages::ForkMessage::PersistentRecords::getPositionOfFineGridCellRelativeToCoarseGridCell() const  {
   return _positionOfFineGridCellRelativeToCoarseGridCell;
}



 void peano::parallel::messages::ForkMessage::PersistentRecords::setPositionOfFineGridCellRelativeToCoarseGridCell(const tarch::la::Vector<DIMENSIONS,int>& positionOfFineGridCellRelativeToCoarseGridCell)  {
   _positionOfFineGridCellRelativeToCoarseGridCell = (positionOfFineGridCellRelativeToCoarseGridCell);
}


peano::parallel::messages::ForkMessage::ForkMessage() {
   
}


peano::parallel::messages::ForkMessage::ForkMessage(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._domainOffset, persistentRecords._h, persistentRecords._adjacencyData, persistentRecords._level, persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell) {
   
}


peano::parallel::messages::ForkMessage::ForkMessage(const tarch::la::Vector<DIMENSIONS,double>& domainOffset, const tarch::la::Vector<DIMENSIONS,double>& h, const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& adjacencyData, const int& level, const tarch::la::Vector<DIMENSIONS,int>& positionOfFineGridCellRelativeToCoarseGridCell):
_persistentRecords(domainOffset, h, adjacencyData, level, positionOfFineGridCellRelativeToCoarseGridCell) {
   
}


peano::parallel::messages::ForkMessage::~ForkMessage() { }


 tarch::la::Vector<DIMENSIONS,double> peano::parallel::messages::ForkMessage::getDomainOffset() const  {
   return _persistentRecords._domainOffset;
}



 void peano::parallel::messages::ForkMessage::setDomainOffset(const tarch::la::Vector<DIMENSIONS,double>& domainOffset)  {
   _persistentRecords._domainOffset = (domainOffset);
}



 double peano::parallel::messages::ForkMessage::getDomainOffset(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._domainOffset[elementIndex];
   
}



 void peano::parallel::messages::ForkMessage::setDomainOffset(int elementIndex, const double& domainOffset)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._domainOffset[elementIndex]= domainOffset;
   
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::messages::ForkMessage::getH() const  {
   return _persistentRecords._h;
}



 void peano::parallel::messages::ForkMessage::setH(const tarch::la::Vector<DIMENSIONS,double>& h)  {
   _persistentRecords._h = (h);
}



 double peano::parallel::messages::ForkMessage::getH(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._h[elementIndex];
   
}



 void peano::parallel::messages::ForkMessage::setH(int elementIndex, const double& h)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._h[elementIndex]= h;
   
}



 tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int> peano::parallel::messages::ForkMessage::getAdjacencyData() const  {
   return _persistentRecords._adjacencyData;
}



 void peano::parallel::messages::ForkMessage::setAdjacencyData(const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& adjacencyData)  {
   _persistentRecords._adjacencyData = (adjacencyData);
}



 int peano::parallel::messages::ForkMessage::getAdjacencyData(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<TWO_POWER_D_TIMES_TWO_POWER_D);
   return _persistentRecords._adjacencyData[elementIndex];
   
}



 void peano::parallel::messages::ForkMessage::setAdjacencyData(int elementIndex, const int& adjacencyData)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<TWO_POWER_D_TIMES_TWO_POWER_D);
   _persistentRecords._adjacencyData[elementIndex]= adjacencyData;
   
}



 int peano::parallel::messages::ForkMessage::getLevel() const  {
   return _persistentRecords._level;
}



 void peano::parallel::messages::ForkMessage::setLevel(const int& level)  {
   _persistentRecords._level = level;
}



 tarch::la::Vector<DIMENSIONS,int> peano::parallel::messages::ForkMessage::getPositionOfFineGridCellRelativeToCoarseGridCell() const  {
   return _persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell;
}



 void peano::parallel::messages::ForkMessage::setPositionOfFineGridCellRelativeToCoarseGridCell(const tarch::la::Vector<DIMENSIONS,int>& positionOfFineGridCellRelativeToCoarseGridCell)  {
   _persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell = (positionOfFineGridCellRelativeToCoarseGridCell);
}



 int peano::parallel::messages::ForkMessage::getPositionOfFineGridCellRelativeToCoarseGridCell(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[elementIndex];
   
}



 void peano::parallel::messages::ForkMessage::setPositionOfFineGridCellRelativeToCoarseGridCell(int elementIndex, const int& positionOfFineGridCellRelativeToCoarseGridCell)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[elementIndex]= positionOfFineGridCellRelativeToCoarseGridCell;
   
}




std::string peano::parallel::messages::ForkMessage::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::parallel::messages::ForkMessage::toString (std::ostream& out) const {
   out << "("; 
   out << "domainOffset:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getDomainOffset(i) << ",";
   }
   out << getDomainOffset(DIMENSIONS-1) << "]";
   out << ",";
   out << "h:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getH(i) << ",";
   }
   out << getH(DIMENSIONS-1) << "]";
   out << ",";
   out << "adjacencyData:[";
   for (int i = 0; i < TWO_POWER_D_TIMES_TWO_POWER_D-1; i++) {
      out << getAdjacencyData(i) << ",";
   }
   out << getAdjacencyData(TWO_POWER_D_TIMES_TWO_POWER_D-1) << "]";
   out << ",";
   out << "level:" << getLevel();
   out << ",";
   out << "positionOfFineGridCellRelativeToCoarseGridCell:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getPositionOfFineGridCellRelativeToCoarseGridCell(i) << ",";
   }
   out << getPositionOfFineGridCellRelativeToCoarseGridCell(DIMENSIONS-1) << "]";
   out <<  ")";
}


peano::parallel::messages::ForkMessage::PersistentRecords peano::parallel::messages::ForkMessage::getPersistentRecords() const {
   return _persistentRecords;
}

peano::parallel::messages::ForkMessagePacked peano::parallel::messages::ForkMessage::convert() const{
   return ForkMessagePacked(
      getDomainOffset(),
      getH(),
      getAdjacencyData(),
      getLevel(),
      getPositionOfFineGridCellRelativeToCoarseGridCell()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::parallel::messages::ForkMessage::_log( "peano::parallel::messages::ForkMessage" );
   
   MPI_Datatype peano::parallel::messages::ForkMessage::Datatype = 0;
   MPI_Datatype peano::parallel::messages::ForkMessage::FullDatatype = 0;
   
   
   void peano::parallel::messages::ForkMessage::initDatatype() {
      {
         ForkMessage dummyForkMessage[2];
         
         #ifdef MPI2
         const int Attributes = 5;
         #else
         const int Attributes = 6;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_DOUBLE		 //domainOffset
            , MPI_DOUBLE		 //h
            , MPI_INT		 //adjacencyData
            , MPI_INT		 //level
            , MPI_INT		 //positionOfFineGridCellRelativeToCoarseGridCell
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              DIMENSIONS		 //domainOffset
            , DIMENSIONS		 //h
            , TWO_POWER_D_TIMES_TWO_POWER_D		 //adjacencyData
            , 1		 //level
            , DIMENSIONS		 //positionOfFineGridCellRelativeToCoarseGridCell
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._domainOffset[0]))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._domainOffset[0]))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._h[0]))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._h[0]))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._adjacencyData[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._adjacencyData[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._level))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._level))), 		&disp[3] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[0]))), 		&disp[4] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[0]))), 		&disp[4] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(ForkMessage)), i, disp[i], Attributes, sizeof(ForkMessage));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[1]))), 		&disp[5] );
         disp[5] -= base;
         disp[5] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &ForkMessage::Datatype );
         MPI_Type_commit( &ForkMessage::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ForkMessage::Datatype);
         MPI_Type_commit( &ForkMessage::Datatype );
         #endif
         
      }
      {
         ForkMessage dummyForkMessage[2];
         
         #ifdef MPI2
         const int Attributes = 5;
         #else
         const int Attributes = 6;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_DOUBLE		 //domainOffset
            , MPI_DOUBLE		 //h
            , MPI_INT		 //adjacencyData
            , MPI_INT		 //level
            , MPI_INT		 //positionOfFineGridCellRelativeToCoarseGridCell
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              DIMENSIONS		 //domainOffset
            , DIMENSIONS		 //h
            , TWO_POWER_D_TIMES_TWO_POWER_D		 //adjacencyData
            , 1		 //level
            , DIMENSIONS		 //positionOfFineGridCellRelativeToCoarseGridCell
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._domainOffset[0]))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._domainOffset[0]))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._h[0]))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._h[0]))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._adjacencyData[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._adjacencyData[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._level))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._level))), 		&disp[3] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[0]))), 		&disp[4] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[0]._persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[0]))), 		&disp[4] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(ForkMessage)), i, disp[i], Attributes, sizeof(ForkMessage));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessage[1]))), 		&disp[5] );
         disp[5] -= base;
         disp[5] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &ForkMessage::FullDatatype );
         MPI_Type_commit( &ForkMessage::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ForkMessage::FullDatatype);
         MPI_Type_commit( &ForkMessage::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void peano::parallel::messages::ForkMessage::shutdownDatatype() {
      MPI_Type_free( &ForkMessage::Datatype );
      MPI_Type_free( &ForkMessage::FullDatatype );
      
   }
   
   void peano::parallel::messages::ForkMessage::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message peano::parallel::messages::ForkMessage " 
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
         msg << "was not able to send message peano::parallel::messages::ForkMessage "  
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
           msg << "testing for finished send task for peano::parallel::messages::ForkMessage " 
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
             "peano::parallel::messages::ForkMessage", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "peano::parallel::messages::ForkMessage", 
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
   
   
   
   void peano::parallel::messages::ForkMessage::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive peano::parallel::messages::ForkMessage from node " 
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
        msg << "failed to start to receive peano::parallel::messages::ForkMessage from node " 
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
          msg << "testing for finished receive task for peano::parallel::messages::ForkMessage failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::parallel::messages::ForkMessage", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::parallel::messages::ForkMessage", 
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
        msg << "testing for finished receive task for peano::parallel::messages::ForkMessage failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::parallel::messages::ForkMessage failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::parallel::messages::ForkMessage", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::parallel::messages::ForkMessage", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive peano::parallel::messages::ForkMessage from node " 
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
   bool peano::parallel::messages::ForkMessage::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int peano::parallel::messages::ForkMessage::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif


peano::parallel::messages::ForkMessagePacked::PersistentRecords::PersistentRecords() {
   
}


peano::parallel::messages::ForkMessagePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& domainOffset, const tarch::la::Vector<DIMENSIONS,double>& h, const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& adjacencyData, const int& level, const tarch::la::Vector<DIMENSIONS,int>& positionOfFineGridCellRelativeToCoarseGridCell):
_domainOffset(domainOffset),
_h(h),
_adjacencyData(adjacencyData),
_level(level),
_positionOfFineGridCellRelativeToCoarseGridCell(positionOfFineGridCellRelativeToCoarseGridCell) {
   
}


 tarch::la::Vector<DIMENSIONS,double> peano::parallel::messages::ForkMessagePacked::PersistentRecords::getDomainOffset() const  {
   return _domainOffset;
}



 void peano::parallel::messages::ForkMessagePacked::PersistentRecords::setDomainOffset(const tarch::la::Vector<DIMENSIONS,double>& domainOffset)  {
   _domainOffset = (domainOffset);
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::messages::ForkMessagePacked::PersistentRecords::getH() const  {
   return _h;
}



 void peano::parallel::messages::ForkMessagePacked::PersistentRecords::setH(const tarch::la::Vector<DIMENSIONS,double>& h)  {
   _h = (h);
}



 tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int> peano::parallel::messages::ForkMessagePacked::PersistentRecords::getAdjacencyData() const  {
   return _adjacencyData;
}



 void peano::parallel::messages::ForkMessagePacked::PersistentRecords::setAdjacencyData(const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& adjacencyData)  {
   _adjacencyData = (adjacencyData);
}



 int peano::parallel::messages::ForkMessagePacked::PersistentRecords::getLevel() const  {
   return _level;
}



 void peano::parallel::messages::ForkMessagePacked::PersistentRecords::setLevel(const int& level)  {
   _level = level;
}



 tarch::la::Vector<DIMENSIONS,int> peano::parallel::messages::ForkMessagePacked::PersistentRecords::getPositionOfFineGridCellRelativeToCoarseGridCell() const  {
   return _positionOfFineGridCellRelativeToCoarseGridCell;
}



 void peano::parallel::messages::ForkMessagePacked::PersistentRecords::setPositionOfFineGridCellRelativeToCoarseGridCell(const tarch::la::Vector<DIMENSIONS,int>& positionOfFineGridCellRelativeToCoarseGridCell)  {
   _positionOfFineGridCellRelativeToCoarseGridCell = (positionOfFineGridCellRelativeToCoarseGridCell);
}


peano::parallel::messages::ForkMessagePacked::ForkMessagePacked() {
   
}


peano::parallel::messages::ForkMessagePacked::ForkMessagePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._domainOffset, persistentRecords._h, persistentRecords._adjacencyData, persistentRecords._level, persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell) {
   
}


peano::parallel::messages::ForkMessagePacked::ForkMessagePacked(const tarch::la::Vector<DIMENSIONS,double>& domainOffset, const tarch::la::Vector<DIMENSIONS,double>& h, const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& adjacencyData, const int& level, const tarch::la::Vector<DIMENSIONS,int>& positionOfFineGridCellRelativeToCoarseGridCell):
_persistentRecords(domainOffset, h, adjacencyData, level, positionOfFineGridCellRelativeToCoarseGridCell) {
   
}


peano::parallel::messages::ForkMessagePacked::~ForkMessagePacked() { }


 tarch::la::Vector<DIMENSIONS,double> peano::parallel::messages::ForkMessagePacked::getDomainOffset() const  {
   return _persistentRecords._domainOffset;
}



 void peano::parallel::messages::ForkMessagePacked::setDomainOffset(const tarch::la::Vector<DIMENSIONS,double>& domainOffset)  {
   _persistentRecords._domainOffset = (domainOffset);
}



 double peano::parallel::messages::ForkMessagePacked::getDomainOffset(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._domainOffset[elementIndex];
   
}



 void peano::parallel::messages::ForkMessagePacked::setDomainOffset(int elementIndex, const double& domainOffset)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._domainOffset[elementIndex]= domainOffset;
   
}



 tarch::la::Vector<DIMENSIONS,double> peano::parallel::messages::ForkMessagePacked::getH() const  {
   return _persistentRecords._h;
}



 void peano::parallel::messages::ForkMessagePacked::setH(const tarch::la::Vector<DIMENSIONS,double>& h)  {
   _persistentRecords._h = (h);
}



 double peano::parallel::messages::ForkMessagePacked::getH(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._h[elementIndex];
   
}



 void peano::parallel::messages::ForkMessagePacked::setH(int elementIndex, const double& h)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._h[elementIndex]= h;
   
}



 tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int> peano::parallel::messages::ForkMessagePacked::getAdjacencyData() const  {
   return _persistentRecords._adjacencyData;
}



 void peano::parallel::messages::ForkMessagePacked::setAdjacencyData(const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& adjacencyData)  {
   _persistentRecords._adjacencyData = (adjacencyData);
}



 int peano::parallel::messages::ForkMessagePacked::getAdjacencyData(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<TWO_POWER_D_TIMES_TWO_POWER_D);
   return _persistentRecords._adjacencyData[elementIndex];
   
}



 void peano::parallel::messages::ForkMessagePacked::setAdjacencyData(int elementIndex, const int& adjacencyData)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<TWO_POWER_D_TIMES_TWO_POWER_D);
   _persistentRecords._adjacencyData[elementIndex]= adjacencyData;
   
}



 int peano::parallel::messages::ForkMessagePacked::getLevel() const  {
   return _persistentRecords._level;
}



 void peano::parallel::messages::ForkMessagePacked::setLevel(const int& level)  {
   _persistentRecords._level = level;
}



 tarch::la::Vector<DIMENSIONS,int> peano::parallel::messages::ForkMessagePacked::getPositionOfFineGridCellRelativeToCoarseGridCell() const  {
   return _persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell;
}



 void peano::parallel::messages::ForkMessagePacked::setPositionOfFineGridCellRelativeToCoarseGridCell(const tarch::la::Vector<DIMENSIONS,int>& positionOfFineGridCellRelativeToCoarseGridCell)  {
   _persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell = (positionOfFineGridCellRelativeToCoarseGridCell);
}



 int peano::parallel::messages::ForkMessagePacked::getPositionOfFineGridCellRelativeToCoarseGridCell(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[elementIndex];
   
}



 void peano::parallel::messages::ForkMessagePacked::setPositionOfFineGridCellRelativeToCoarseGridCell(int elementIndex, const int& positionOfFineGridCellRelativeToCoarseGridCell)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[elementIndex]= positionOfFineGridCellRelativeToCoarseGridCell;
   
}




std::string peano::parallel::messages::ForkMessagePacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::parallel::messages::ForkMessagePacked::toString (std::ostream& out) const {
   out << "("; 
   out << "domainOffset:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getDomainOffset(i) << ",";
   }
   out << getDomainOffset(DIMENSIONS-1) << "]";
   out << ",";
   out << "h:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getH(i) << ",";
   }
   out << getH(DIMENSIONS-1) << "]";
   out << ",";
   out << "adjacencyData:[";
   for (int i = 0; i < TWO_POWER_D_TIMES_TWO_POWER_D-1; i++) {
      out << getAdjacencyData(i) << ",";
   }
   out << getAdjacencyData(TWO_POWER_D_TIMES_TWO_POWER_D-1) << "]";
   out << ",";
   out << "level:" << getLevel();
   out << ",";
   out << "positionOfFineGridCellRelativeToCoarseGridCell:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getPositionOfFineGridCellRelativeToCoarseGridCell(i) << ",";
   }
   out << getPositionOfFineGridCellRelativeToCoarseGridCell(DIMENSIONS-1) << "]";
   out <<  ")";
}


peano::parallel::messages::ForkMessagePacked::PersistentRecords peano::parallel::messages::ForkMessagePacked::getPersistentRecords() const {
   return _persistentRecords;
}

peano::parallel::messages::ForkMessage peano::parallel::messages::ForkMessagePacked::convert() const{
   return ForkMessage(
      getDomainOffset(),
      getH(),
      getAdjacencyData(),
      getLevel(),
      getPositionOfFineGridCellRelativeToCoarseGridCell()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::parallel::messages::ForkMessagePacked::_log( "peano::parallel::messages::ForkMessagePacked" );
   
   MPI_Datatype peano::parallel::messages::ForkMessagePacked::Datatype = 0;
   MPI_Datatype peano::parallel::messages::ForkMessagePacked::FullDatatype = 0;
   
   
   void peano::parallel::messages::ForkMessagePacked::initDatatype() {
      {
         ForkMessagePacked dummyForkMessagePacked[2];
         
         #ifdef MPI2
         const int Attributes = 5;
         #else
         const int Attributes = 6;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_DOUBLE		 //domainOffset
            , MPI_DOUBLE		 //h
            , MPI_INT		 //adjacencyData
            , MPI_INT		 //level
            , MPI_INT		 //positionOfFineGridCellRelativeToCoarseGridCell
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              DIMENSIONS		 //domainOffset
            , DIMENSIONS		 //h
            , TWO_POWER_D_TIMES_TWO_POWER_D		 //adjacencyData
            , 1		 //level
            , DIMENSIONS		 //positionOfFineGridCellRelativeToCoarseGridCell
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._domainOffset[0]))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._domainOffset[0]))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._h[0]))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._h[0]))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._adjacencyData[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._adjacencyData[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._level))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._level))), 		&disp[3] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[0]))), 		&disp[4] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[0]))), 		&disp[4] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(ForkMessagePacked)), i, disp[i], Attributes, sizeof(ForkMessagePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[1]))), 		&disp[5] );
         disp[5] -= base;
         disp[5] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &ForkMessagePacked::Datatype );
         MPI_Type_commit( &ForkMessagePacked::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ForkMessagePacked::Datatype);
         MPI_Type_commit( &ForkMessagePacked::Datatype );
         #endif
         
      }
      {
         ForkMessagePacked dummyForkMessagePacked[2];
         
         #ifdef MPI2
         const int Attributes = 5;
         #else
         const int Attributes = 6;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_DOUBLE		 //domainOffset
            , MPI_DOUBLE		 //h
            , MPI_INT		 //adjacencyData
            , MPI_INT		 //level
            , MPI_INT		 //positionOfFineGridCellRelativeToCoarseGridCell
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              DIMENSIONS		 //domainOffset
            , DIMENSIONS		 //h
            , TWO_POWER_D_TIMES_TWO_POWER_D		 //adjacencyData
            , 1		 //level
            , DIMENSIONS		 //positionOfFineGridCellRelativeToCoarseGridCell
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._domainOffset[0]))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._domainOffset[0]))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._h[0]))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._h[0]))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._adjacencyData[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._adjacencyData[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._level))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._level))), 		&disp[3] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[0]))), 		&disp[4] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[0]._persistentRecords._positionOfFineGridCellRelativeToCoarseGridCell[0]))), 		&disp[4] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(ForkMessagePacked)), i, disp[i], Attributes, sizeof(ForkMessagePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyForkMessagePacked[1]))), 		&disp[5] );
         disp[5] -= base;
         disp[5] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &ForkMessagePacked::FullDatatype );
         MPI_Type_commit( &ForkMessagePacked::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ForkMessagePacked::FullDatatype);
         MPI_Type_commit( &ForkMessagePacked::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void peano::parallel::messages::ForkMessagePacked::shutdownDatatype() {
      MPI_Type_free( &ForkMessagePacked::Datatype );
      MPI_Type_free( &ForkMessagePacked::FullDatatype );
      
   }
   
   void peano::parallel::messages::ForkMessagePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message peano::parallel::messages::ForkMessagePacked " 
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
         msg << "was not able to send message peano::parallel::messages::ForkMessagePacked "  
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
           msg << "testing for finished send task for peano::parallel::messages::ForkMessagePacked " 
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
             "peano::parallel::messages::ForkMessagePacked", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "peano::parallel::messages::ForkMessagePacked", 
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
   
   
   
   void peano::parallel::messages::ForkMessagePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive peano::parallel::messages::ForkMessagePacked from node " 
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
        msg << "failed to start to receive peano::parallel::messages::ForkMessagePacked from node " 
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
          msg << "testing for finished receive task for peano::parallel::messages::ForkMessagePacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::parallel::messages::ForkMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::parallel::messages::ForkMessagePacked", 
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
        msg << "testing for finished receive task for peano::parallel::messages::ForkMessagePacked failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::parallel::messages::ForkMessagePacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::parallel::messages::ForkMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::parallel::messages::ForkMessagePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive peano::parallel::messages::ForkMessagePacked from node " 
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
   bool peano::parallel::messages::ForkMessagePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int peano::parallel::messages::ForkMessagePacked::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif



