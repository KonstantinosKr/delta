#include "dem/records/Cell.h"

#if !defined(PersistentRegularSubtrees) && !defined(Parallel) && !defined(Debug) && !defined(SharedMemoryParallelisation)
   dem::records::Cell::PersistentRecords::PersistentRecords() {
      
   }
   
   
   dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
   _isInside(isInside),
   _state(state),
   _evenFlags(evenFlags),
   _accessNumber(accessNumber) {
      
   }
   
   dem::records::Cell::Cell() {
      
   }
   
   
   dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber) {
      
   }
   
   
   dem::records::Cell::Cell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
   _persistentRecords(isInside, state, evenFlags, accessNumber) {
      
   }
   
   
   dem::records::Cell::~Cell() { }
   
   std::string dem::records::Cell::toString(const State& param) {
      switch (param) {
         case Leaf: return "Leaf";
         case Refined: return "Refined";
         case Root: return "Root";
      }
      return "undefined";
   }
   
   std::string dem::records::Cell::getStateMapping() {
      return "State(Leaf=0,Refined=1,Root=2)";
   }
   
   
   std::string dem::records::Cell::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::Cell::toString (std::ostream& out) const {
      out << "("; 
      out << "isInside:" << getIsInside();
      out << ",";
      out << "state:" << toString(getState());
      out << ",";
      out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
      out << ",";
      out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
      out <<  ")";
   }
   
   
   dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::CellPacked dem::records::Cell::convert() const{
      return CellPacked(
         getIsInside(),
         getState(),
         getEvenFlags(),
         getAccessNumber()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
      
      MPI_Datatype dem::records::Cell::Datatype = 0;
      MPI_Datatype dem::records::Cell::FullDatatype = 0;
      
      
      void dem::records::Cell::initDatatype() {
         {
            Cell dummyCell[2];
            
            #ifdef MPI2
            const int Attributes = 2;
            #else
            const int Attributes = 3;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_CXX_BOOL		 //isInside
               , MPI_INT		 //state
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 1		 //isInside
               , 1		 //state
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[2] );
            disp[2] -= base;
            disp[2] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
            MPI_Type_commit( &Cell::Datatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
            MPI_Type_commit( &Cell::Datatype );
            #endif
            
         }
         {
            Cell dummyCell[2];
            
            #ifdef MPI2
            const int Attributes = 4;
            #else
            const int Attributes = 5;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_CXX_BOOL		 //isInside
               , MPI_INT		 //state
               , MPI_CXX_BOOL		 //evenFlags
               , MPI_SHORT		 //accessNumber
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 1		 //isInside
               , 1		 //state
               , DIMENSIONS		 //evenFlags
               , DIMENSIONS_TIMES_TWO		 //accessNumber
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[4] );
            disp[4] -= base;
            disp[4] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
            MPI_Type_commit( &Cell::FullDatatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
            MPI_Type_commit( &Cell::FullDatatype );
            #endif
            
         }
         
      }
      
      
      void dem::records::Cell::shutdownDatatype() {
         MPI_Type_free( &Cell::Datatype );
         MPI_Type_free( &Cell::FullDatatype );
         
      }
      
      void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         _senderDestinationRank = destination;
         
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message dem::records::Cell "
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
               msg << "was not able to send message dem::records::Cell "
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
                  msg << "testing for finished send task for dem::records::Cell "
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
                  "dem::records::Cell",
                  "send(int)", destination,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "dem::records::Cell",
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
      
      
      
      void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            MPI_Status  status;
            const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
            _senderDestinationRank = status.MPI_SOURCE;
            if ( result != MPI_SUCCESS ) {
               std::ostringstream msg;
               msg << "failed to start to receive dem::records::Cell from node "
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
               msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "testing for finished receive task for dem::records::Cell failed: "
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
                  "dem::records::Cell",
                  "receive(int)", source,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "dem::records::Cell",
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
      
      
      
      bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
      
      int dem::records::Cell::getSenderRank() const {
         assertion( _senderDestinationRank!=-1 );
         return _senderDestinationRank;
         
      }
   #endif
   
   
   dem::records::CellPacked::PersistentRecords::PersistentRecords() {
      if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
      
   }
   
   
   dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
   _accessNumber(accessNumber) {
      setIsInside(isInside);
      setState(state);
      setEvenFlags(evenFlags);
      if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
      
   }
   
   dem::records::CellPacked::CellPacked() {
      if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
      
   }
   
   
   dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber) {
      if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
      
   }
   
   
   dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
   _persistentRecords(isInside, state, evenFlags, accessNumber) {
      if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
      
   }
   
   
   dem::records::CellPacked::~CellPacked() { }
   
   std::string dem::records::CellPacked::toString(const State& param) {
      return dem::records::Cell::toString(param);
   }
   
   std::string dem::records::CellPacked::getStateMapping() {
      return dem::records::Cell::getStateMapping();
   }
   
   
   
   std::string dem::records::CellPacked::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::CellPacked::toString (std::ostream& out) const {
      out << "("; 
      out << "isInside:" << getIsInside();
      out << ",";
      out << "state:" << toString(getState());
      out << ",";
      out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
      out << ",";
      out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
      out <<  ")";
   }
   
   
   dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::Cell dem::records::CellPacked::convert() const{
      return Cell(
         getIsInside(),
         getState(),
         getEvenFlags(),
         getAccessNumber()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
      
      MPI_Datatype dem::records::CellPacked::Datatype = 0;
      MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
      
      
      void dem::records::CellPacked::initDatatype() {
         {
            CellPacked dummyCellPacked[2];
            
            #ifdef MPI2
            const int Attributes = 1;
            #else
            const int Attributes = 2;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_SHORT		 //_packedRecords0
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 1		 //_packedRecords0
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[1] );
            disp[1] -= base;
            disp[1] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
            MPI_Type_commit( &CellPacked::Datatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
            MPI_Type_commit( &CellPacked::Datatype );
            #endif
            
         }
         {
            CellPacked dummyCellPacked[2];
            
            #ifdef MPI2
            const int Attributes = 2;
            #else
            const int Attributes = 3;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_SHORT		 //accessNumber
               , MPI_SHORT		 //_packedRecords0
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 DIMENSIONS_TIMES_TWO		 //accessNumber
               , 1		 //_packedRecords0
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
            disp[2] -= base;
            disp[2] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
            MPI_Type_commit( &CellPacked::FullDatatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
            MPI_Type_commit( &CellPacked::FullDatatype );
            #endif
            
         }
         
      }
      
      
      void dem::records::CellPacked::shutdownDatatype() {
         MPI_Type_free( &CellPacked::Datatype );
         MPI_Type_free( &CellPacked::FullDatatype );
         
      }
      
      void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         _senderDestinationRank = destination;
         
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message dem::records::CellPacked "
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
               msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "testing for finished send task for dem::records::CellPacked "
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
                  "dem::records::CellPacked",
                  "send(int)", destination,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "dem::records::CellPacked",
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
      
      
      
      void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            MPI_Status  status;
            const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
            _senderDestinationRank = status.MPI_SOURCE;
            if ( result != MPI_SUCCESS ) {
               std::ostringstream msg;
               msg << "failed to start to receive dem::records::CellPacked from node "
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
               msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                  "dem::records::CellPacked",
                  "receive(int)", source,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "dem::records::CellPacked",
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
      
      
      
      bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
      
      int dem::records::CellPacked::getSenderRank() const {
         assertion( _senderDestinationRank!=-1 );
         return _senderDestinationRank;
         
      }
   #endif
   
   
   #elif defined(Parallel) && !defined(PersistentRegularSubtrees) && !defined(Debug) && !defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
      _isInside(isInside),
      _state(state),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _cellIsAForkCandidate(cellIsAForkCandidate) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
      _persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 7;
               #else
               const int Attributes = 8;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_CXX_BOOL		 //cellIsAForkCandidate
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //cellIsAForkCandidate
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[6] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[7] );
               disp[7] -= base;
               disp[7] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         setCellIsAForkCandidate(cellIsAForkCandidate);
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate()) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
      _persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 4;
               #else
               const int Attributes = 5;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[4] );
               disp[4] -= base;
               disp[4] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif !defined(PersistentRegularSubtrees) && !defined(Parallel) && !defined(Debug) && defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _isInside(isInside),
      _state(state),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _persistentRecords(isInside, state, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 6;
               #else
               const int Attributes = 7;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[6] );
               disp[6] -= base;
               disp[6] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _accessNumber(accessNumber),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _persistentRecords(isInside, state, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 1;
               #else
               const int Attributes = 2;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[1] );
               disp[1] -= base;
               disp[1] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 4;
               #else
               const int Attributes = 5;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //accessNumber
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[4] );
               disp[4] -= base;
               disp[4] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(PersistentRegularSubtrees) && !defined(Parallel) && !defined(Debug) && !defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& persistentRegularSubtreeIndex):
      _isInside(isInside),
      _state(state),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, evenFlags, accessNumber, persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 5;
               #else
               const int Attributes = 6;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[4] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[5] );
               disp[5] -= base;
               disp[5] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& persistentRegularSubtreeIndex):
      _accessNumber(accessNumber),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, evenFlags, accessNumber, persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 1;
               #else
               const int Attributes = 2;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[1] );
               disp[1] -= base;
               disp[1] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //accessNumber
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //persistentRegularSubtreeIndex
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif !defined(PersistentRegularSubtrees) && defined(Debug) && !defined(Parallel) && !defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
      _isInside(isInside),
      _state(state),
      _level(level),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 5;
               #else
               const int Attributes = 6;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[5] );
               disp[5] -= base;
               disp[5] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
      _level(level),
      _accessNumber(accessNumber) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //accessNumber
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(Parallel) && !defined(PersistentRegularSubtrees) && defined(Debug) && !defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
      _isInside(isInside),
      _state(state),
      _level(level),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _cellIsAForkCandidate(cellIsAForkCandidate) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 4;
               #else
               const int Attributes = 5;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , 1		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[4] );
               disp[4] -= base;
               disp[4] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 8;
               #else
               const int Attributes = 9;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_CXX_BOOL		 //cellIsAForkCandidate
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //cellIsAForkCandidate
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[7] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[8] );
               disp[8] -= base;
               disp[8] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
      _level(level),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         setCellIsAForkCandidate(cellIsAForkCandidate);
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate()) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , 1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 5;
               #else
               const int Attributes = 6;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[5] );
               disp[5] -= base;
               disp[5] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(PersistentRegularSubtrees) && !defined(Parallel) && !defined(Debug) && defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _isInside(isInside),
      _state(state),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream, persistentRecords._persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream, persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 7;
               #else
               const int Attributes = 8;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[6] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[7] );
               disp[7] -= base;
               disp[7] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _accessNumber(accessNumber),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream, persistentRecords._persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream, persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 1;
               #else
               const int Attributes = 2;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[1] );
               disp[1] -= base;
               disp[1] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 5;
               #else
               const int Attributes = 6;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //accessNumber
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //persistentRegularSubtreeIndex
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[5] );
               disp[5] -= base;
               disp[5] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif !defined(PersistentRegularSubtrees) && defined(Debug) && !defined(Parallel) && defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _isInside(isInside),
      _state(state),
      _level(level),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 7;
               #else
               const int Attributes = 8;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[6] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[7] );
               disp[7] -= base;
               disp[7] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _level(level),
      _accessNumber(accessNumber),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 5;
               #else
               const int Attributes = 6;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[5] );
               disp[5] -= base;
               disp[5] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(PersistentRegularSubtrees) && defined(Debug) && !defined(Parallel) && !defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& persistentRegularSubtreeIndex):
      _isInside(isInside),
      _state(state),
      _level(level),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 6;
               #else
               const int Attributes = 7;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[5] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[6] );
               disp[6] -= base;
               disp[6] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& persistentRegularSubtreeIndex):
      _level(level),
      _accessNumber(accessNumber),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 4;
               #else
               const int Attributes = 5;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //persistentRegularSubtreeIndex
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[4] );
               disp[4] -= base;
               disp[4] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(Parallel) && !defined(PersistentRegularSubtrees) && !defined(Debug) && defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _isInside(isInside),
      _state(state),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _cellIsAForkCandidate(cellIsAForkCandidate),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 9;
               #else
               const int Attributes = 10;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_CXX_BOOL		 //cellIsAForkCandidate
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //cellIsAForkCandidate
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[8] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[9] );
               disp[9] -= base;
               disp[9] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         setCellIsAForkCandidate(cellIsAForkCandidate);
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate(), persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 6;
               #else
               const int Attributes = 7;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[5] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[6] );
               disp[6] -= base;
               disp[6] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(Parallel) && defined(PersistentRegularSubtrees) && !defined(Debug) && !defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& persistentRegularSubtreeIndex):
      _isInside(isInside),
      _state(state),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _cellIsAForkCandidate(cellIsAForkCandidate),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate, persistentRecords._persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 8;
               #else
               const int Attributes = 9;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_CXX_BOOL		 //cellIsAForkCandidate
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //cellIsAForkCandidate
                  , 1		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[7] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[8] );
               disp[8] -= base;
               disp[8] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& persistentRegularSubtreeIndex):
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         setCellIsAForkCandidate(cellIsAForkCandidate);
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate(), persistentRecords._persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 5;
               #else
               const int Attributes = 6;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //persistentRegularSubtreeIndex
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[5] );
               disp[5] -= base;
               disp[5] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(PersistentRegularSubtrees) && defined(Debug) && !defined(Parallel) && defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _isInside(isInside),
      _state(state),
      _level(level),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream, persistentRecords._persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream, persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 8;
               #else
               const int Attributes = 9;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[7] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[8] );
               disp[8] -= base;
               disp[8] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _level(level),
      _accessNumber(accessNumber),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream, persistentRecords._persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream, persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+3 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+3 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 6;
               #else
               const int Attributes = 7;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //persistentRegularSubtreeIndex
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[5] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[6] );
               disp[6] -= base;
               disp[6] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(Parallel) && !defined(PersistentRegularSubtrees) && defined(Debug) && defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _isInside(isInside),
      _state(state),
      _level(level),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _cellIsAForkCandidate(cellIsAForkCandidate),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 4;
               #else
               const int Attributes = 5;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , 1		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[4] );
               disp[4] -= base;
               disp[4] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 10;
               #else
               const int Attributes = 11;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_CXX_BOOL		 //cellIsAForkCandidate
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //cellIsAForkCandidate
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[9] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[10] );
               disp[10] -= base;
               disp[10] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _level(level),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         setCellIsAForkCandidate(cellIsAForkCandidate);
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate(), persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , 1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 7;
               #else
               const int Attributes = 8;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[6] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[7] );
               disp[7] -= base;
               disp[7] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(Parallel) && defined(PersistentRegularSubtrees) && !defined(Debug) && defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _isInside(isInside),
      _state(state),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _cellIsAForkCandidate(cellIsAForkCandidate),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream, persistentRecords._persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream, persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 10;
               #else
               const int Attributes = 11;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_CXX_BOOL		 //cellIsAForkCandidate
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //cellIsAForkCandidate
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[9] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[10] );
               disp[10] -= base;
               disp[10] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         setCellIsAForkCandidate(cellIsAForkCandidate);
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate(), persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream, persistentRecords._persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream, persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 2;
               #else
               const int Attributes = 3;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[2] );
               disp[2] -= base;
               disp[2] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 7;
               #else
               const int Attributes = 8;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //persistentRegularSubtreeIndex
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[6] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[7] );
               disp[7] -= base;
               disp[7] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(Parallel) && defined(PersistentRegularSubtrees) && defined(Debug) && !defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& persistentRegularSubtreeIndex):
      _isInside(isInside),
      _state(state),
      _level(level),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _cellIsAForkCandidate(cellIsAForkCandidate),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate, persistentRecords._persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 4;
               #else
               const int Attributes = 5;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , 1		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[4] );
               disp[4] -= base;
               disp[4] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 9;
               #else
               const int Attributes = 10;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_CXX_BOOL		 //cellIsAForkCandidate
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //cellIsAForkCandidate
                  , 1		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[8] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[9] );
               disp[9] -= base;
               disp[9] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& persistentRegularSubtreeIndex):
      _level(level),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         setCellIsAForkCandidate(cellIsAForkCandidate);
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate(), persistentRecords._persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , 1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 6;
               #else
               const int Attributes = 7;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //persistentRegularSubtreeIndex
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[5] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[6] );
               disp[6] -= base;
               disp[6] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(Parallel) && defined(PersistentRegularSubtrees) && defined(Debug) && defined(SharedMemoryParallelisation)
      dem::records::Cell::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::Cell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _isInside(isInside),
      _state(state),
      _level(level),
      _evenFlags(evenFlags),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _cellIsAForkCandidate(cellIsAForkCandidate),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         
      }
      
      dem::records::Cell::Cell() {
         
      }
      
      
      dem::records::Cell::Cell(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream, persistentRecords._persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::Cell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream, persistentRegularSubtreeIndex) {
         
      }
      
      
      dem::records::Cell::~Cell() { }
      
      std::string dem::records::Cell::toString(const State& param) {
         switch (param) {
            case Leaf: return "Leaf";
            case Refined: return "Refined";
            case Root: return "Root";
         }
         return "undefined";
      }
      
      std::string dem::records::Cell::getStateMapping() {
         return "State(Leaf=0,Refined=1,Root=2)";
      }
      
      
      std::string dem::records::Cell::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::Cell::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::Cell::PersistentRecords dem::records::Cell::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::CellPacked dem::records::Cell::convert() const{
         return CellPacked(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::Cell::_log( "dem::records::Cell" );
         
         MPI_Datatype dem::records::Cell::Datatype = 0;
         MPI_Datatype dem::records::Cell::FullDatatype = 0;
         
         
         void dem::records::Cell::initDatatype() {
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 4;
               #else
               const int Attributes = 5;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , 1		 //subtreeHoldsWorker
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[4] );
               disp[4] -= base;
               disp[4] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::Datatype );
               MPI_Type_commit( &Cell::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::Datatype);
               MPI_Type_commit( &Cell::Datatype );
               #endif
               
            }
            {
               Cell dummyCell[2];
               
               #ifdef MPI2
               const int Attributes = 11;
               #else
               const int Attributes = 12;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_CXX_BOOL		 //isInside
                  , MPI_INT		 //state
                  , MPI_INT		 //level
                  , MPI_CXX_BOOL		 //evenFlags
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_CXX_BOOL		 //cellIsAForkCandidate
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //isInside
                  , 1		 //state
                  , 1		 //level
                  , DIMENSIONS		 //evenFlags
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //cellIsAForkCandidate
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //persistentRegularSubtreeIndex
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._isInside))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._state))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._level))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[10] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(Cell)), i, disp[i], Attributes, sizeof(Cell));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCell[1]))), 		&disp[11] );
               disp[11] -= base;
               disp[11] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Cell::FullDatatype );
               MPI_Type_commit( &Cell::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Cell::FullDatatype);
               MPI_Type_commit( &Cell::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::Cell::shutdownDatatype() {
            MPI_Type_free( &Cell::Datatype );
            MPI_Type_free( &Cell::FullDatatype );
            
         }
         
         void dem::records::Cell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::Cell "
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
                  msg << "was not able to send message dem::records::Cell "
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
                     msg << "testing for finished send task for dem::records::Cell "
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
                     "dem::records::Cell",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         void dem::records::Cell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::Cell from node "
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
                  msg << "failed to start to receive dem::records::Cell from node "
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
                     msg << "testing for finished receive task for dem::records::Cell failed: "
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
                     "dem::records::Cell",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::Cell",
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
         
         
         
         bool dem::records::Cell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::Cell::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _level(level),
      _accessNumber(accessNumber),
      _responsibleRank(responsibleRank),
      _subtreeHoldsWorker(subtreeHoldsWorker),
      _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
      _numberOfStoresToOutputStream(numberOfStoresToOutputStream),
      _persistentRegularSubtreeIndex(persistentRegularSubtreeIndex) {
         setIsInside(isInside);
         setState(state);
         setEvenFlags(evenFlags);
         setCellIsAForkCandidate(cellIsAForkCandidate);
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      dem::records::CellPacked::CellPacked() {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate(), persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream, persistentRecords._persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::CellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream, const int& persistentRegularSubtreeIndex):
      _persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream, persistentRegularSubtreeIndex) {
         if ((DIMENSIONS+4 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((DIMENSIONS+4 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::CellPacked::~CellPacked() { }
      
      std::string dem::records::CellPacked::toString(const State& param) {
         return dem::records::Cell::toString(param);
      }
      
      std::string dem::records::CellPacked::getStateMapping() {
         return dem::records::Cell::getStateMapping();
      }
      
      
      
      std::string dem::records::CellPacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::CellPacked::toString (std::ostream& out) const {
         out << "("; 
         out << "isInside:" << getIsInside();
         out << ",";
         out << "state:" << toString(getState());
         out << ",";
         out << "level:" << getLevel();
         out << ",";
         out << "evenFlags:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getEvenFlags(i) << ",";
   }
   out << getEvenFlags(DIMENSIONS-1) << "]";
         out << ",";
         out << "accessNumber:[";
   for (int i = 0; i < DIMENSIONS_TIMES_TWO-1; i++) {
      out << getAccessNumber(i) << ",";
   }
   out << getAccessNumber(DIMENSIONS_TIMES_TWO-1) << "]";
         out << ",";
         out << "responsibleRank:" << getResponsibleRank();
         out << ",";
         out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
         out << ",";
         out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
         out << ",";
         out << "numberOfLoadsFromInputStream:" << getNumberOfLoadsFromInputStream();
         out << ",";
         out << "numberOfStoresToOutputStream:" << getNumberOfStoresToOutputStream();
         out << ",";
         out << "persistentRegularSubtreeIndex:" << getPersistentRegularSubtreeIndex();
         out <<  ")";
      }
      
      
      dem::records::CellPacked::PersistentRecords dem::records::CellPacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::Cell dem::records::CellPacked::convert() const{
         return Cell(
            getIsInside(),
            getState(),
            getLevel(),
            getEvenFlags(),
            getAccessNumber(),
            getResponsibleRank(),
            getSubtreeHoldsWorker(),
            getCellIsAForkCandidate(),
            getNumberOfLoadsFromInputStream(),
            getNumberOfStoresToOutputStream(),
            getPersistentRegularSubtreeIndex()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::CellPacked::_log( "dem::records::CellPacked" );
         
         MPI_Datatype dem::records::CellPacked::Datatype = 0;
         MPI_Datatype dem::records::CellPacked::FullDatatype = 0;
         
         
         void dem::records::CellPacked::initDatatype() {
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 3;
               #else
               const int Attributes = 4;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , 1		 //subtreeHoldsWorker
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[3] );
               disp[3] -= base;
               disp[3] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::Datatype );
               MPI_Type_commit( &CellPacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::Datatype);
               MPI_Type_commit( &CellPacked::Datatype );
               #endif
               
            }
            {
               CellPacked dummyCellPacked[2];
               
               #ifdef MPI2
               const int Attributes = 8;
               #else
               const int Attributes = 9;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_INT		 //level
                  , MPI_SHORT		 //accessNumber
                  , MPI_INT		 //responsibleRank
                  , MPI_CXX_BOOL		 //subtreeHoldsWorker
                  , MPI_INT		 //numberOfLoadsFromInputStream
                  , MPI_INT		 //numberOfStoresToOutputStream
                  , MPI_INT		 //persistentRegularSubtreeIndex
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //level
                  , DIMENSIONS_TIMES_TWO		 //accessNumber
                  , 1		 //responsibleRank
                  , 1		 //subtreeHoldsWorker
                  , 1		 //numberOfLoadsFromInputStream
                  , 1		 //numberOfStoresToOutputStream
                  , 1		 //persistentRegularSubtreeIndex
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._level))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._persistentRegularSubtreeIndex))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[7] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(CellPacked)), i, disp[i], Attributes, sizeof(CellPacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyCellPacked[1]))), 		&disp[8] );
               disp[8] -= base;
               disp[8] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &CellPacked::FullDatatype );
               MPI_Type_commit( &CellPacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &CellPacked::FullDatatype);
               MPI_Type_commit( &CellPacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::CellPacked::shutdownDatatype() {
            MPI_Type_free( &CellPacked::Datatype );
            MPI_Type_free( &CellPacked::FullDatatype );
            
         }
         
         void dem::records::CellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::CellPacked "
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
                  msg << "was not able to send message dem::records::CellPacked "
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
                     msg << "testing for finished send task for dem::records::CellPacked "
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
                     "dem::records::CellPacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         void dem::records::CellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
               _senderDestinationRank = status.MPI_SOURCE;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                  msg << "failed to start to receive dem::records::CellPacked from node "
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
                     msg << "testing for finished receive task for dem::records::CellPacked failed: "
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
                     "dem::records::CellPacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::CellPacked",
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
         
         
         
         bool dem::records::CellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::CellPacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   
#endif


