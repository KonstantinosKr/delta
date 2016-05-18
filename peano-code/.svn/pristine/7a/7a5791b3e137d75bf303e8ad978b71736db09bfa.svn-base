#include "peano/grid/tests/records/TestCell.h"

#if !defined(Debug) && !defined(Parallel) && defined(SharedMemoryParallelisation)
   peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords() {
      
   }
   
   
   peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
   _isInside(isInside),
   _state(state),
   _evenFlags(evenFlags),
   _accessNumber(accessNumber),
   _numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
   _numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
      
   }
   
   
    bool peano::grid::tests::records::TestCell::PersistentRecords::getIsInside() const  {
      return _isInside;
   }
   
   
   
    void peano::grid::tests::records::TestCell::PersistentRecords::setIsInside(const bool& isInside)  {
      _isInside = isInside;
   }
   
   
   
    peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::PersistentRecords::getState() const  {
      return _state;
   }
   
   
   
    void peano::grid::tests::records::TestCell::PersistentRecords::setState(const State& state)  {
      _state = state;
   }
   
   
   
    std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::PersistentRecords::getEvenFlags() const  {
      return _evenFlags;
   }
   
   
   
    void peano::grid::tests::records::TestCell::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
      _evenFlags = (evenFlags);
   }
   
   
   
    tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::PersistentRecords::getAccessNumber() const  {
      return _accessNumber;
   }
   
   
   
    void peano::grid::tests::records::TestCell::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
      _accessNumber = (accessNumber);
   }
   
   
   
    int peano::grid::tests::records::TestCell::PersistentRecords::getNumberOfLoadsFromInputStream() const  {
      return _numberOfLoadsFromInputStream;
   }
   
   
   
    void peano::grid::tests::records::TestCell::PersistentRecords::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
      _numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
   }
   
   
   
    int peano::grid::tests::records::TestCell::PersistentRecords::getNumberOfStoresToOutputStream() const  {
      return _numberOfStoresToOutputStream;
   }
   
   
   
    void peano::grid::tests::records::TestCell::PersistentRecords::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
      _numberOfStoresToOutputStream = numberOfStoresToOutputStream;
   }
   
   
   peano::grid::tests::records::TestCell::TestCell() {
      
   }
   
   
   peano::grid::tests::records::TestCell::TestCell(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
      
   }
   
   
   peano::grid::tests::records::TestCell::TestCell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
   _persistentRecords(isInside, state, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
      
   }
   
   
   peano::grid::tests::records::TestCell::~TestCell() { }
   
   
    bool peano::grid::tests::records::TestCell::getIsInside() const  {
      return _persistentRecords._isInside;
   }
   
   
   
    void peano::grid::tests::records::TestCell::setIsInside(const bool& isInside)  {
      _persistentRecords._isInside = isInside;
   }
   
   
   
    peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::getState() const  {
      return _persistentRecords._state;
   }
   
   
   
    void peano::grid::tests::records::TestCell::setState(const State& state)  {
      _persistentRecords._state = state;
   }
   
   
   
    std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::getEvenFlags() const  {
      return _persistentRecords._evenFlags;
   }
   
   
   
    void peano::grid::tests::records::TestCell::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
      _persistentRecords._evenFlags = (evenFlags);
   }
   
   
   
    bool peano::grid::tests::records::TestCell::getEvenFlags(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      return _persistentRecords._evenFlags[elementIndex];
      
   }
   
   
   
    void peano::grid::tests::records::TestCell::setEvenFlags(int elementIndex, const bool& evenFlags)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._evenFlags[elementIndex]= evenFlags;
      
   }
   
   
   
    void peano::grid::tests::records::TestCell::flipEvenFlags(int elementIndex)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._evenFlags.flip(elementIndex);
   }
   
   
   
    tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::getAccessNumber() const  {
      return _persistentRecords._accessNumber;
   }
   
   
   
    void peano::grid::tests::records::TestCell::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
      _persistentRecords._accessNumber = (accessNumber);
   }
   
   
   
    short int peano::grid::tests::records::TestCell::getAccessNumber(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS_TIMES_TWO);
      return _persistentRecords._accessNumber[elementIndex];
      
   }
   
   
   
    void peano::grid::tests::records::TestCell::setAccessNumber(int elementIndex, const short int& accessNumber)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS_TIMES_TWO);
      _persistentRecords._accessNumber[elementIndex]= accessNumber;
      
   }
   
   
   
    int peano::grid::tests::records::TestCell::getNumberOfLoadsFromInputStream() const  {
      return _persistentRecords._numberOfLoadsFromInputStream;
   }
   
   
   
    void peano::grid::tests::records::TestCell::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
      _persistentRecords._numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
   }
   
   
   
    int peano::grid::tests::records::TestCell::getNumberOfStoresToOutputStream() const  {
      return _persistentRecords._numberOfStoresToOutputStream;
   }
   
   
   
    void peano::grid::tests::records::TestCell::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
      _persistentRecords._numberOfStoresToOutputStream = numberOfStoresToOutputStream;
   }
   
   
   std::string peano::grid::tests::records::TestCell::toString(const State& param) {
      switch (param) {
         case Leaf: return "Leaf";
         case Refined: return "Refined";
         case Root: return "Root";
      }
      return "undefined";
   }
   
   std::string peano::grid::tests::records::TestCell::getStateMapping() {
      return "State(Leaf=0,Refined=1,Root=2)";
   }
   
   
   std::string peano::grid::tests::records::TestCell::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void peano::grid::tests::records::TestCell::toString (std::ostream& out) const {
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
   
   
   peano::grid::tests::records::TestCell::PersistentRecords peano::grid::tests::records::TestCell::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   peano::grid::tests::records::TestCellPacked peano::grid::tests::records::TestCell::convert() const{
      return TestCellPacked(
         getIsInside(),
         getState(),
         getEvenFlags(),
         getAccessNumber(),
         getNumberOfLoadsFromInputStream(),
         getNumberOfStoresToOutputStream()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log peano::grid::tests::records::TestCell::_log( "peano::grid::tests::records::TestCell" );
      
      MPI_Datatype peano::grid::tests::records::TestCell::Datatype = 0;
      MPI_Datatype peano::grid::tests::records::TestCell::FullDatatype = 0;
      
      
      void peano::grid::tests::records::TestCell::initDatatype() {
         {
            TestCell dummyTestCell[2];
            
            const int Attributes = 2;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //state
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //state
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._state))), 		&disp[1] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::Datatype );
            MPI_Type_commit( &TestCell::Datatype );
            
         }
         {
            TestCell dummyTestCell[2];
            
            const int Attributes = 7;
            MPI_Datatype subtypes[Attributes] = {
               MPI_CHAR,		 //isInside
               MPI_INT,		 //state
               MPI_INT,		 //evenFlags
               MPI_SHORT,		 //accessNumber
               MPI_INT,		 //numberOfLoadsFromInputStream
               MPI_INT,		 //numberOfStoresToOutputStream
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //isInside
               1,		 //state
               DIMENSIONS,		 //evenFlags
               DIMENSIONS_TIMES_TWO,		 //accessNumber
               1,		 //numberOfLoadsFromInputStream
               1,		 //numberOfStoresToOutputStream
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._isInside))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._isInside))), 		&disp[6] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::FullDatatype );
            MPI_Type_commit( &TestCell::FullDatatype );
            
         }
         
      }
      
      
      void peano::grid::tests::records::TestCell::shutdownDatatype() {
         MPI_Type_free( &TestCell::Datatype );
         MPI_Type_free( &TestCell::FullDatatype );
         
      }
      
      void peano::grid::tests::records::TestCell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         _senderDestinationRank = destination;
         
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message peano::grid::tests::records::TestCell "
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
            msg << "was not able to send message peano::grid::tests::records::TestCell "
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
               msg << "testing for finished send task for peano::grid::tests::records::TestCell "
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
               "peano::grid::tests::records::TestCell",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::grid::tests::records::TestCell",
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
   
   
   
   void peano::grid::tests::records::TestCell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         _senderDestinationRank = status.MPI_SOURCE;
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
            msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
               msg << "testing for finished receive task for peano::grid::tests::records::TestCell failed: "
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
               "peano::grid::tests::records::TestCell",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::grid::tests::records::TestCell",
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
   
   
   
   bool peano::grid::tests::records::TestCell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int peano::grid::tests::records::TestCell::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords() {
   if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((DIMENSIONS+3 < (8 * sizeof(int))));
   
}


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_accessNumber(accessNumber),
_numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
_numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
   setIsInside(isInside);
   setState(state);
   setEvenFlags(evenFlags);
   if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((DIMENSIONS+3 < (8 * sizeof(int))));
   
}


 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getIsInside() const  {
   int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setIsInside(const bool& isInside)  {
   int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isInside ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::PersistentRecords::getState() const  {
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setState(const State& state)  {
   assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(state) << (1));
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::PersistentRecords::getEvenFlags() const  {
   int mask = (int) (1 << (DIMENSIONS)) - 1 ;
   mask = static_cast<int>(mask << (3));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (3));
   std::bitset<DIMENSIONS> result = tmp;
   return result;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
   int mask = (int) (1 << (DIMENSIONS)) - 1 ;
   mask = static_cast<int>(mask << (3));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | evenFlags.to_ulong() << (3));
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::PersistentRecords::getAccessNumber() const  {
   return _accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
   _accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getNumberOfLoadsFromInputStream() const  {
   return _numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
   _numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getNumberOfStoresToOutputStream() const  {
   return _numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
   _numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


peano::grid::tests::records::TestCellPacked::TestCellPacked() {
   if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((DIMENSIONS+3 < (8 * sizeof(int))));
   
}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
   if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((DIMENSIONS+3 < (8 * sizeof(int))));
   
}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_persistentRecords(isInside, state, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
   if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((DIMENSIONS+3 < (8 * sizeof(int))));
   
}


peano::grid::tests::records::TestCellPacked::~TestCellPacked() { }


 bool peano::grid::tests::records::TestCellPacked::getIsInside() const  {
   int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setIsInside(const bool& isInside)  {
   int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isInside ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::getState() const  {
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::setState(const State& state)  {
   assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(state) << (1));
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::getEvenFlags() const  {
   int mask = (int) (1 << (DIMENSIONS)) - 1 ;
   mask = static_cast<int>(mask << (3));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (3));
   std::bitset<DIMENSIONS> result = tmp;
   return result;
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
   int mask = (int) (1 << (DIMENSIONS)) - 1 ;
   mask = static_cast<int>(mask << (3));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | evenFlags.to_ulong() << (3));
}



 bool peano::grid::tests::records::TestCellPacked::getEvenFlags(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   int mask = 1 << (3);
   mask = mask << elementIndex;
   return (_persistentRecords._packedRecords0& mask);
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(int elementIndex, const bool& evenFlags)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   assertion(!evenFlags || evenFlags==1);
   int shift        = 3 + elementIndex; 
   int mask         = 1     << (shift);
   int shiftedValue = evenFlags << (shift);
   _persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 & ~mask;
   _persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 |  shiftedValue;
}



 void peano::grid::tests::records::TestCellPacked::flipEvenFlags(int elementIndex)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   int mask = 1 << (3);
   mask = mask << elementIndex;
   _persistentRecords._packedRecords0^= mask;
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::getAccessNumber() const  {
   return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
   _persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCellPacked::getAccessNumber(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS_TIMES_TWO);
   return _persistentRecords._accessNumber[elementIndex];
   
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(int elementIndex, const short int& accessNumber)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS_TIMES_TWO);
   _persistentRecords._accessNumber[elementIndex]= accessNumber;
   
}



 int peano::grid::tests::records::TestCellPacked::getNumberOfLoadsFromInputStream() const  {
   return _persistentRecords._numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCellPacked::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
   _persistentRecords._numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCellPacked::getNumberOfStoresToOutputStream() const  {
   return _persistentRecords._numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCellPacked::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
   _persistentRecords._numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


std::string peano::grid::tests::records::TestCellPacked::toString(const State& param) {
   return peano::grid::tests::records::TestCell::toString(param);
}

std::string peano::grid::tests::records::TestCellPacked::getStateMapping() {
   return peano::grid::tests::records::TestCell::getStateMapping();
}



std::string peano::grid::tests::records::TestCellPacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::grid::tests::records::TestCellPacked::toString (std::ostream& out) const {
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


peano::grid::tests::records::TestCellPacked::PersistentRecords peano::grid::tests::records::TestCellPacked::getPersistentRecords() const {
   return _persistentRecords;
}

peano::grid::tests::records::TestCell peano::grid::tests::records::TestCellPacked::convert() const{
   return TestCell(
      getIsInside(),
      getState(),
      getEvenFlags(),
      getAccessNumber(),
      getNumberOfLoadsFromInputStream(),
      getNumberOfStoresToOutputStream()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::grid::tests::records::TestCellPacked::_log( "peano::grid::tests::records::TestCellPacked" );
   
   MPI_Datatype peano::grid::tests::records::TestCellPacked::Datatype = 0;
   MPI_Datatype peano::grid::tests::records::TestCellPacked::FullDatatype = 0;
   
   
   void peano::grid::tests::records::TestCellPacked::initDatatype() {
      {
         TestCellPacked dummyTestCellPacked[2];
         
         const int Attributes = 2;
         MPI_Datatype subtypes[Attributes] = {
            MPI_INT,		 //_packedRecords0
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //_packedRecords0
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._packedRecords0))), 		&disp[1] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::Datatype );
         MPI_Type_commit( &TestCellPacked::Datatype );
         
      }
      {
         TestCellPacked dummyTestCellPacked[2];
         
         const int Attributes = 5;
         MPI_Datatype subtypes[Attributes] = {
            MPI_SHORT,		 //accessNumber
            MPI_INT,		 //numberOfLoadsFromInputStream
            MPI_INT,		 //numberOfStoresToOutputStream
            MPI_INT,		 //_packedRecords0
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            DIMENSIONS_TIMES_TWO,		 //accessNumber
            1,		 //numberOfLoadsFromInputStream
            1,		 //numberOfStoresToOutputStream
            1,		 //_packedRecords0
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[1] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[2] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyTestCellPacked[1]._persistentRecords._accessNumber[0])), 		&disp[4] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::FullDatatype );
         MPI_Type_commit( &TestCellPacked::FullDatatype );
         
      }
      
   }
   
   
   void peano::grid::tests::records::TestCellPacked::shutdownDatatype() {
      MPI_Type_free( &TestCellPacked::Datatype );
      MPI_Type_free( &TestCellPacked::FullDatatype );
      
   }
   
   void peano::grid::tests::records::TestCellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      _senderDestinationRank = destination;
      
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
         msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
            msg << "testing for finished send task for peano::grid::tests::records::TestCellPacked "
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
            "peano::grid::tests::records::TestCellPacked",
            "send(int)", destination,tag,1
            );
            triggeredTimeoutWarning = true;
         }
         if (
            tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
            (clock()>timeOutShutdown)
         ) {
            tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
            "peano::grid::tests::records::TestCellPacked",
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



void peano::grid::tests::records::TestCellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
   if (communicateSleep<0) {
   
      MPI_Status  status;
      const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
      _senderDestinationRank = status.MPI_SOURCE;
      if ( result != MPI_SUCCESS ) {
         std::ostringstream msg;
         msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
         msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
            msg << "testing for finished receive task for peano::grid::tests::records::TestCellPacked failed: "
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
            "peano::grid::tests::records::TestCellPacked",
            "receive(int)", source,tag,1
            );
            triggeredTimeoutWarning = true;
         }
         if (
            tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
            (clock()>timeOutShutdown)
         ) {
            tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
            "peano::grid::tests::records::TestCellPacked",
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



bool peano::grid::tests::records::TestCellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCellPacked::getSenderRank() const {
   assertion( _senderDestinationRank!=-1 );
   return _senderDestinationRank;
   
}
#endif



#elif !defined(Parallel) && defined(Debug) && !defined(SharedMemoryParallelisation)
peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
_isInside(isInside),
_state(state),
_level(level),
_evenFlags(evenFlags),
_accessNumber(accessNumber) {

}


 bool peano::grid::tests::records::TestCell::PersistentRecords::getIsInside() const  {
return _isInside;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setIsInside(const bool& isInside)  {
_isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::PersistentRecords::getState() const  {
return _state;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setState(const State& state)  {
_state = state;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setLevel(const int& level)  {
_level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::PersistentRecords::getEvenFlags() const  {
return _evenFlags;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_evenFlags = (evenFlags);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}


peano::grid::tests::records::TestCell::TestCell() {

}


peano::grid::tests::records::TestCell::TestCell(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber) {

}


peano::grid::tests::records::TestCell::TestCell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
_persistentRecords(isInside, state, level, evenFlags, accessNumber) {

}


peano::grid::tests::records::TestCell::~TestCell() { }


 bool peano::grid::tests::records::TestCell::getIsInside() const  {
return _persistentRecords._isInside;
}



 void peano::grid::tests::records::TestCell::setIsInside(const bool& isInside)  {
_persistentRecords._isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::getState() const  {
return _persistentRecords._state;
}



 void peano::grid::tests::records::TestCell::setState(const State& state)  {
_persistentRecords._state = state;
}



 int peano::grid::tests::records::TestCell::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestCell::setLevel(const int& level)  {
_persistentRecords._level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::getEvenFlags() const  {
return _persistentRecords._evenFlags;
}



 void peano::grid::tests::records::TestCell::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_persistentRecords._evenFlags = (evenFlags);
}



 bool peano::grid::tests::records::TestCell::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._evenFlags[elementIndex];

}



 void peano::grid::tests::records::TestCell::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags[elementIndex]= evenFlags;

}



 void peano::grid::tests::records::TestCell::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags.flip(elementIndex);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCell::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCell::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCell::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}


std::string peano::grid::tests::records::TestCell::toString(const State& param) {
switch (param) {
   case Leaf: return "Leaf";
   case Refined: return "Refined";
   case Root: return "Root";
}
return "undefined";
}

std::string peano::grid::tests::records::TestCell::getStateMapping() {
return "State(Leaf=0,Refined=1,Root=2)";
}


std::string peano::grid::tests::records::TestCell::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCell::toString (std::ostream& out) const {
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


peano::grid::tests::records::TestCell::PersistentRecords peano::grid::tests::records::TestCell::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCellPacked peano::grid::tests::records::TestCell::convert() const{
return TestCellPacked(
   getIsInside(),
   getState(),
   getLevel(),
   getEvenFlags(),
   getAccessNumber()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCell::_log( "peano::grid::tests::records::TestCell" );

MPI_Datatype peano::grid::tests::records::TestCell::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCell::FullDatatype = 0;


void peano::grid::tests::records::TestCell::initDatatype() {
   {
      TestCell dummyTestCell[2];
      
      const int Attributes = 3;
      MPI_Datatype subtypes[Attributes] = {
         MPI_INT,		 //state
         MPI_INT,		 //level
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         1,		 //state
         1,		 //level
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[0] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._level))), 		&disp[1] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._state))), 		&disp[2] );
      
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::Datatype );
      MPI_Type_commit( &TestCell::Datatype );
      
   }
   {
      TestCell dummyTestCell[2];
      
      const int Attributes = 6;
      MPI_Datatype subtypes[Attributes] = {
         MPI_CHAR,		 //isInside
         MPI_INT,		 //state
         MPI_INT,		 //level
         MPI_INT,		 //evenFlags
         MPI_SHORT,		 //accessNumber
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         1,		 //isInside
         1,		 //state
         1,		 //level
         DIMENSIONS,		 //evenFlags
         DIMENSIONS_TIMES_TWO,		 //accessNumber
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._isInside))), 		&disp[0] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[1] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._level))), 		&disp[2] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._isInside))), 		&disp[5] );
      
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::FullDatatype );
      MPI_Type_commit( &TestCell::FullDatatype );
      
   }
   
}


void peano::grid::tests::records::TestCell::shutdownDatatype() {
   MPI_Type_free( &TestCell::Datatype );
   MPI_Type_free( &TestCell::FullDatatype );
   
}

void peano::grid::tests::records::TestCell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
   _senderDestinationRank = destination;
   
   if (communicateSleep<0) {
   
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
      if  (result!=MPI_SUCCESS) {
         std::ostringstream msg;
         msg << "was not able to send message peano::grid::tests::records::TestCell "
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
      msg << "was not able to send message peano::grid::tests::records::TestCell "
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
         msg << "testing for finished send task for peano::grid::tests::records::TestCell "
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
         "peano::grid::tests::records::TestCell",
         "send(int)", destination,tag,1
         );
         triggeredTimeoutWarning = true;
      }
      if (
         tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
         (clock()>timeOutShutdown)
      ) {
         tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
         "peano::grid::tests::records::TestCell",
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



void peano::grid::tests::records::TestCell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

   MPI_Status  status;
   const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
   _senderDestinationRank = status.MPI_SOURCE;
   if ( result != MPI_SUCCESS ) {
      std::ostringstream msg;
      msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
      msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
         msg << "testing for finished receive task for peano::grid::tests::records::TestCell failed: "
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
         "peano::grid::tests::records::TestCell",
         "receive(int)", source,tag,1
         );
         triggeredTimeoutWarning = true;
      }
      if (
         tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
         (clock()>timeOutShutdown)
      ) {
         tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
         "peano::grid::tests::records::TestCell",
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



bool peano::grid::tests::records::TestCell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCell::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords() {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
_level(level),
_accessNumber(accessNumber) {
setIsInside(isInside);
setState(state);
setEvenFlags(evenFlags);
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isInside ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::PersistentRecords::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(state) << (1));
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setLevel(const int& level)  {
_level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::PersistentRecords::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
_packedRecords0 = static_cast<int>(_packedRecords0 | evenFlags.to_ulong() << (3));
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}


peano::grid::tests::records::TestCellPacked::TestCellPacked() {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber) {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
_persistentRecords(isInside, state, level, evenFlags, accessNumber) {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::~TestCellPacked() { }


 bool peano::grid::tests::records::TestCellPacked::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isInside ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(state) << (1));
}



 int peano::grid::tests::records::TestCellPacked::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestCellPacked::setLevel(const int& level)  {
_persistentRecords._level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | evenFlags.to_ulong() << (3));
}



 bool peano::grid::tests::records::TestCellPacked::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
return (_persistentRecords._packedRecords0& mask);
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
assertion(!evenFlags || evenFlags==1);
int shift        = 3 + elementIndex; 
int mask         = 1     << (shift);
int shiftedValue = evenFlags << (shift);
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 & ~mask;
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 |  shiftedValue;
}



 void peano::grid::tests::records::TestCellPacked::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
_persistentRecords._packedRecords0^= mask;
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCellPacked::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}


std::string peano::grid::tests::records::TestCellPacked::toString(const State& param) {
return peano::grid::tests::records::TestCell::toString(param);
}

std::string peano::grid::tests::records::TestCellPacked::getStateMapping() {
return peano::grid::tests::records::TestCell::getStateMapping();
}



std::string peano::grid::tests::records::TestCellPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCellPacked::toString (std::ostream& out) const {
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


peano::grid::tests::records::TestCellPacked::PersistentRecords peano::grid::tests::records::TestCellPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCell peano::grid::tests::records::TestCellPacked::convert() const{
return TestCell(
getIsInside(),
getState(),
getLevel(),
getEvenFlags(),
getAccessNumber()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCellPacked::_log( "peano::grid::tests::records::TestCellPacked" );

MPI_Datatype peano::grid::tests::records::TestCellPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCellPacked::FullDatatype = 0;


void peano::grid::tests::records::TestCellPacked::initDatatype() {
{
   TestCellPacked dummyTestCellPacked[2];
   
   const int Attributes = 3;
   MPI_Datatype subtypes[Attributes] = {
      MPI_INT,		 //level
      MPI_INT,		 //_packedRecords0
      MPI_UB		 // end/displacement flag
   };
   
   int blocklen[Attributes] = {
      1,		 //level
      1,		 //_packedRecords0
      1		 // end/displacement flag
   };
   
   MPI_Aint     disp[Attributes];
   
   MPI_Aint base;
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._level))), 		&disp[0] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._level))), 		&disp[2] );
   
   for (int i=1; i<Attributes; i++) {
      assertion1( disp[i] > disp[i-1], i );
   }
   for (int i=0; i<Attributes; i++) {
      disp[i] -= base;
   }
   MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::Datatype );
   MPI_Type_commit( &TestCellPacked::Datatype );
   
}
{
   TestCellPacked dummyTestCellPacked[2];
   
   const int Attributes = 4;
   MPI_Datatype subtypes[Attributes] = {
      MPI_INT,		 //level
      MPI_SHORT,		 //accessNumber
      MPI_INT,		 //_packedRecords0
      MPI_UB		 // end/displacement flag
   };
   
   int blocklen[Attributes] = {
      1,		 //level
      DIMENSIONS_TIMES_TWO,		 //accessNumber
      1,		 //_packedRecords0
      1		 // end/displacement flag
   };
   
   MPI_Aint     disp[Attributes];
   
   MPI_Aint base;
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._level))), 		&disp[0] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._level))), 		&disp[3] );
   
   for (int i=1; i<Attributes; i++) {
      assertion1( disp[i] > disp[i-1], i );
   }
   for (int i=0; i<Attributes; i++) {
      disp[i] -= base;
   }
   MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::FullDatatype );
   MPI_Type_commit( &TestCellPacked::FullDatatype );
   
}

}


void peano::grid::tests::records::TestCellPacked::shutdownDatatype() {
MPI_Type_free( &TestCellPacked::Datatype );
MPI_Type_free( &TestCellPacked::FullDatatype );

}

void peano::grid::tests::records::TestCellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

   const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
   if  (result!=MPI_SUCCESS) {
      std::ostringstream msg;
      msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
   msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
      msg << "testing for finished send task for peano::grid::tests::records::TestCellPacked "
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
      "peano::grid::tests::records::TestCellPacked",
      "send(int)", destination,tag,1
      );
      triggeredTimeoutWarning = true;
   }
   if (
      tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
      (clock()>timeOutShutdown)
   ) {
      tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
      "peano::grid::tests::records::TestCellPacked",
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



void peano::grid::tests::records::TestCellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
   std::ostringstream msg;
   msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
   msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
      msg << "testing for finished receive task for peano::grid::tests::records::TestCellPacked failed: "
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
      "peano::grid::tests::records::TestCellPacked",
      "receive(int)", source,tag,1
      );
      triggeredTimeoutWarning = true;
   }
   if (
      tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
      (clock()>timeOutShutdown)
   ) {
      tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
      "peano::grid::tests::records::TestCellPacked",
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



bool peano::grid::tests::records::TestCellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCellPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif defined(Parallel) && !defined(Debug) && !defined(SharedMemoryParallelisation)
peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
_isInside(isInside),
_state(state),
_evenFlags(evenFlags),
_accessNumber(accessNumber),
_responsibleRank(responsibleRank),
_nodeWorkload(nodeWorkload),
_localWorkload(localWorkload),
_totalWorkload(totalWorkload),
_maxWorkload(maxWorkload),
_minWorkload(minWorkload),
_subtreeHoldsWorker(subtreeHoldsWorker),
_cellIsAForkCandidate(cellIsAForkCandidate) {

}


 bool peano::grid::tests::records::TestCell::PersistentRecords::getIsInside() const  {
return _isInside;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setIsInside(const bool& isInside)  {
_isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::PersistentRecords::getState() const  {
return _state;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setState(const State& state)  {
_state = state;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::PersistentRecords::getEvenFlags() const  {
return _evenFlags;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_evenFlags = (evenFlags);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getResponsibleRank() const  {
return _responsibleRank;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setResponsibleRank(const int& responsibleRank)  {
_responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getNodeWorkload() const  {
return _nodeWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNodeWorkload(const double& nodeWorkload)  {
_nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getLocalWorkload() const  {
return _localWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setLocalWorkload(const double& localWorkload)  {
_localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getTotalWorkload() const  {
return _totalWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setTotalWorkload(const double& totalWorkload)  {
_totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getMaxWorkload() const  {
return _maxWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setMaxWorkload(const double& maxWorkload)  {
_maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getMinWorkload() const  {
return _minWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setMinWorkload(const double& minWorkload)  {
_minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCell::PersistentRecords::getSubtreeHoldsWorker() const  {
return _subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCell::PersistentRecords::getCellIsAForkCandidate() const  {
return _cellIsAForkCandidate;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
_cellIsAForkCandidate = cellIsAForkCandidate;
}


peano::grid::tests::records::TestCell::TestCell() {

}


peano::grid::tests::records::TestCell::TestCell(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._nodeWorkload, persistentRecords._localWorkload, persistentRecords._totalWorkload, persistentRecords._maxWorkload, persistentRecords._minWorkload, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate) {

}


peano::grid::tests::records::TestCell::TestCell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
_persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, nodeWorkload, localWorkload, totalWorkload, maxWorkload, minWorkload, subtreeHoldsWorker, cellIsAForkCandidate) {

}


peano::grid::tests::records::TestCell::~TestCell() { }


 bool peano::grid::tests::records::TestCell::getIsInside() const  {
return _persistentRecords._isInside;
}



 void peano::grid::tests::records::TestCell::setIsInside(const bool& isInside)  {
_persistentRecords._isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::getState() const  {
return _persistentRecords._state;
}



 void peano::grid::tests::records::TestCell::setState(const State& state)  {
_persistentRecords._state = state;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::getEvenFlags() const  {
return _persistentRecords._evenFlags;
}



 void peano::grid::tests::records::TestCell::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_persistentRecords._evenFlags = (evenFlags);
}



 bool peano::grid::tests::records::TestCell::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._evenFlags[elementIndex];

}



 void peano::grid::tests::records::TestCell::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags[elementIndex]= evenFlags;

}



 void peano::grid::tests::records::TestCell::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags.flip(elementIndex);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCell::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCell::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCell::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCell::getResponsibleRank() const  {
return _persistentRecords._responsibleRank;
}



 void peano::grid::tests::records::TestCell::setResponsibleRank(const int& responsibleRank)  {
_persistentRecords._responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCell::getNodeWorkload() const  {
return _persistentRecords._nodeWorkload;
}



 void peano::grid::tests::records::TestCell::setNodeWorkload(const double& nodeWorkload)  {
_persistentRecords._nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCell::getLocalWorkload() const  {
return _persistentRecords._localWorkload;
}



 void peano::grid::tests::records::TestCell::setLocalWorkload(const double& localWorkload)  {
_persistentRecords._localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCell::getTotalWorkload() const  {
return _persistentRecords._totalWorkload;
}



 void peano::grid::tests::records::TestCell::setTotalWorkload(const double& totalWorkload)  {
_persistentRecords._totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCell::getMaxWorkload() const  {
return _persistentRecords._maxWorkload;
}



 void peano::grid::tests::records::TestCell::setMaxWorkload(const double& maxWorkload)  {
_persistentRecords._maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCell::getMinWorkload() const  {
return _persistentRecords._minWorkload;
}



 void peano::grid::tests::records::TestCell::setMinWorkload(const double& minWorkload)  {
_persistentRecords._minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCell::getSubtreeHoldsWorker() const  {
return _persistentRecords._subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCell::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_persistentRecords._subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCell::getCellIsAForkCandidate() const  {
return _persistentRecords._cellIsAForkCandidate;
}



 void peano::grid::tests::records::TestCell::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
_persistentRecords._cellIsAForkCandidate = cellIsAForkCandidate;
}


std::string peano::grid::tests::records::TestCell::toString(const State& param) {
switch (param) {
case Leaf: return "Leaf";
case Refined: return "Refined";
case Root: return "Root";
}
return "undefined";
}

std::string peano::grid::tests::records::TestCell::getStateMapping() {
return "State(Leaf=0,Refined=1,Root=2)";
}


std::string peano::grid::tests::records::TestCell::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCell::toString (std::ostream& out) const {
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
out << "nodeWorkload:" << getNodeWorkload();
out << ",";
out << "localWorkload:" << getLocalWorkload();
out << ",";
out << "totalWorkload:" << getTotalWorkload();
out << ",";
out << "maxWorkload:" << getMaxWorkload();
out << ",";
out << "minWorkload:" << getMinWorkload();
out << ",";
out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
out << ",";
out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
out <<  ")";
}


peano::grid::tests::records::TestCell::PersistentRecords peano::grid::tests::records::TestCell::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCellPacked peano::grid::tests::records::TestCell::convert() const{
return TestCellPacked(
getIsInside(),
getState(),
getEvenFlags(),
getAccessNumber(),
getResponsibleRank(),
getNodeWorkload(),
getLocalWorkload(),
getTotalWorkload(),
getMaxWorkload(),
getMinWorkload(),
getSubtreeHoldsWorker(),
getCellIsAForkCandidate()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCell::_log( "peano::grid::tests::records::TestCell" );

MPI_Datatype peano::grid::tests::records::TestCell::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCell::FullDatatype = 0;


void peano::grid::tests::records::TestCell::initDatatype() {
{
TestCell dummyTestCell[2];

const int Attributes = 5;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //state
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //state
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._maxWorkload))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._minWorkload))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._state))), 		&disp[4] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::Datatype );
MPI_Type_commit( &TestCell::Datatype );

}
{
TestCell dummyTestCell[2];

const int Attributes = 13;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isInside
MPI_INT,		 //state
MPI_INT,		 //evenFlags
MPI_SHORT,		 //accessNumber
MPI_INT,		 //responsibleRank
MPI_DOUBLE,		 //nodeWorkload
MPI_DOUBLE,		 //localWorkload
MPI_DOUBLE,		 //totalWorkload
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_CHAR,		 //cellIsAForkCandidate
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isInside
1,		 //state
DIMENSIONS,		 //evenFlags
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //responsibleRank
1,		 //nodeWorkload
1,		 //localWorkload
1,		 //totalWorkload
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //cellIsAForkCandidate
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._isInside))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._nodeWorkload))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._localWorkload))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._totalWorkload))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._maxWorkload))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._minWorkload))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._isInside))), 		&disp[12] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::FullDatatype );
MPI_Type_commit( &TestCell::FullDatatype );

}

}


void peano::grid::tests::records::TestCell::shutdownDatatype() {
MPI_Type_free( &TestCell::Datatype );
MPI_Type_free( &TestCell::FullDatatype );

}

void peano::grid::tests::records::TestCell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCell "
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
"peano::grid::tests::records::TestCell",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



void peano::grid::tests::records::TestCell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCell failed: "
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
"peano::grid::tests::records::TestCell",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



bool peano::grid::tests::records::TestCell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCell::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords() {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
_accessNumber(accessNumber),
_responsibleRank(responsibleRank),
_nodeWorkload(nodeWorkload),
_localWorkload(localWorkload),
_totalWorkload(totalWorkload),
_maxWorkload(maxWorkload),
_minWorkload(minWorkload),
_subtreeHoldsWorker(subtreeHoldsWorker) {
setIsInside(isInside);
setState(state);
setEvenFlags(evenFlags);
setCellIsAForkCandidate(cellIsAForkCandidate);
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isInside ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::PersistentRecords::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(state) << (1));
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::PersistentRecords::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
_packedRecords0 = static_cast<int>(_packedRecords0 | evenFlags.to_ulong() << (3));
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getResponsibleRank() const  {
return _responsibleRank;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setResponsibleRank(const int& responsibleRank)  {
_responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getNodeWorkload() const  {
return _nodeWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNodeWorkload(const double& nodeWorkload)  {
_nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getLocalWorkload() const  {
return _localWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setLocalWorkload(const double& localWorkload)  {
_localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getTotalWorkload() const  {
return _totalWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setTotalWorkload(const double& totalWorkload)  {
_totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getMaxWorkload() const  {
return _maxWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setMaxWorkload(const double& maxWorkload)  {
_maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getMinWorkload() const  {
return _minWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setMinWorkload(const double& minWorkload)  {
_minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getSubtreeHoldsWorker() const  {
return _subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getCellIsAForkCandidate() const  {
int mask = 1 << (DIMENSIONS + 3);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
int mask = 1 << (DIMENSIONS + 3);
   _packedRecords0 = static_cast<int>( cellIsAForkCandidate ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}


peano::grid::tests::records::TestCellPacked::TestCellPacked() {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._nodeWorkload, persistentRecords._localWorkload, persistentRecords._totalWorkload, persistentRecords._maxWorkload, persistentRecords._minWorkload, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate()) {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
_persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, nodeWorkload, localWorkload, totalWorkload, maxWorkload, minWorkload, subtreeHoldsWorker, cellIsAForkCandidate) {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::~TestCellPacked() { }


 bool peano::grid::tests::records::TestCellPacked::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isInside ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(state) << (1));
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | evenFlags.to_ulong() << (3));
}



 bool peano::grid::tests::records::TestCellPacked::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
return (_persistentRecords._packedRecords0& mask);
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
assertion(!evenFlags || evenFlags==1);
int shift        = 3 + elementIndex; 
int mask         = 1     << (shift);
int shiftedValue = evenFlags << (shift);
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 & ~mask;
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 |  shiftedValue;
}



 void peano::grid::tests::records::TestCellPacked::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
_persistentRecords._packedRecords0^= mask;
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCellPacked::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCellPacked::getResponsibleRank() const  {
return _persistentRecords._responsibleRank;
}



 void peano::grid::tests::records::TestCellPacked::setResponsibleRank(const int& responsibleRank)  {
_persistentRecords._responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCellPacked::getNodeWorkload() const  {
return _persistentRecords._nodeWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setNodeWorkload(const double& nodeWorkload)  {
_persistentRecords._nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getLocalWorkload() const  {
return _persistentRecords._localWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setLocalWorkload(const double& localWorkload)  {
_persistentRecords._localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getTotalWorkload() const  {
return _persistentRecords._totalWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setTotalWorkload(const double& totalWorkload)  {
_persistentRecords._totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getMaxWorkload() const  {
return _persistentRecords._maxWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setMaxWorkload(const double& maxWorkload)  {
_persistentRecords._maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getMinWorkload() const  {
return _persistentRecords._minWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setMinWorkload(const double& minWorkload)  {
_persistentRecords._minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCellPacked::getSubtreeHoldsWorker() const  {
return _persistentRecords._subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCellPacked::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_persistentRecords._subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCellPacked::getCellIsAForkCandidate() const  {
int mask = 1 << (DIMENSIONS + 3);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
int mask = 1 << (DIMENSIONS + 3);
   _persistentRecords._packedRecords0 = static_cast<int>( cellIsAForkCandidate ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}


std::string peano::grid::tests::records::TestCellPacked::toString(const State& param) {
return peano::grid::tests::records::TestCell::toString(param);
}

std::string peano::grid::tests::records::TestCellPacked::getStateMapping() {
return peano::grid::tests::records::TestCell::getStateMapping();
}



std::string peano::grid::tests::records::TestCellPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCellPacked::toString (std::ostream& out) const {
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
out << "nodeWorkload:" << getNodeWorkload();
out << ",";
out << "localWorkload:" << getLocalWorkload();
out << ",";
out << "totalWorkload:" << getTotalWorkload();
out << ",";
out << "maxWorkload:" << getMaxWorkload();
out << ",";
out << "minWorkload:" << getMinWorkload();
out << ",";
out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
out << ",";
out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
out <<  ")";
}


peano::grid::tests::records::TestCellPacked::PersistentRecords peano::grid::tests::records::TestCellPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCell peano::grid::tests::records::TestCellPacked::convert() const{
return TestCell(
getIsInside(),
getState(),
getEvenFlags(),
getAccessNumber(),
getResponsibleRank(),
getNodeWorkload(),
getLocalWorkload(),
getTotalWorkload(),
getMaxWorkload(),
getMinWorkload(),
getSubtreeHoldsWorker(),
getCellIsAForkCandidate()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCellPacked::_log( "peano::grid::tests::records::TestCellPacked" );

MPI_Datatype peano::grid::tests::records::TestCellPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCellPacked::FullDatatype = 0;


void peano::grid::tests::records::TestCellPacked::initDatatype() {
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 5;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._maxWorkload))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._minWorkload))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._maxWorkload))), 		&disp[4] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::Datatype );
MPI_Type_commit( &TestCellPacked::Datatype );

}
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 10;
MPI_Datatype subtypes[Attributes] = {
MPI_SHORT,		 //accessNumber
MPI_INT,		 //responsibleRank
MPI_DOUBLE,		 //nodeWorkload
MPI_DOUBLE,		 //localWorkload
MPI_DOUBLE,		 //totalWorkload
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //responsibleRank
1,		 //nodeWorkload
1,		 //localWorkload
1,		 //totalWorkload
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._nodeWorkload))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._localWorkload))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._totalWorkload))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._maxWorkload))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._minWorkload))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyTestCellPacked[1]._persistentRecords._accessNumber[0])), 		&disp[9] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::FullDatatype );
MPI_Type_commit( &TestCellPacked::FullDatatype );

}

}


void peano::grid::tests::records::TestCellPacked::shutdownDatatype() {
MPI_Type_free( &TestCellPacked::Datatype );
MPI_Type_free( &TestCellPacked::FullDatatype );

}

void peano::grid::tests::records::TestCellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCellPacked "
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
"peano::grid::tests::records::TestCellPacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



void peano::grid::tests::records::TestCellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCellPacked failed: "
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
"peano::grid::tests::records::TestCellPacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



bool peano::grid::tests::records::TestCellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCellPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif !defined(Debug) && !defined(Parallel) && !defined(SharedMemoryParallelisation)
peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
_isInside(isInside),
_state(state),
_evenFlags(evenFlags),
_accessNumber(accessNumber) {

}


 bool peano::grid::tests::records::TestCell::PersistentRecords::getIsInside() const  {
return _isInside;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setIsInside(const bool& isInside)  {
_isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::PersistentRecords::getState() const  {
return _state;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setState(const State& state)  {
_state = state;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::PersistentRecords::getEvenFlags() const  {
return _evenFlags;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_evenFlags = (evenFlags);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}


peano::grid::tests::records::TestCell::TestCell() {

}


peano::grid::tests::records::TestCell::TestCell(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber) {

}


peano::grid::tests::records::TestCell::TestCell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
_persistentRecords(isInside, state, evenFlags, accessNumber) {

}


peano::grid::tests::records::TestCell::~TestCell() { }


 bool peano::grid::tests::records::TestCell::getIsInside() const  {
return _persistentRecords._isInside;
}



 void peano::grid::tests::records::TestCell::setIsInside(const bool& isInside)  {
_persistentRecords._isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::getState() const  {
return _persistentRecords._state;
}



 void peano::grid::tests::records::TestCell::setState(const State& state)  {
_persistentRecords._state = state;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::getEvenFlags() const  {
return _persistentRecords._evenFlags;
}



 void peano::grid::tests::records::TestCell::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_persistentRecords._evenFlags = (evenFlags);
}



 bool peano::grid::tests::records::TestCell::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._evenFlags[elementIndex];

}



 void peano::grid::tests::records::TestCell::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags[elementIndex]= evenFlags;

}



 void peano::grid::tests::records::TestCell::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags.flip(elementIndex);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCell::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCell::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCell::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}


std::string peano::grid::tests::records::TestCell::toString(const State& param) {
switch (param) {
case Leaf: return "Leaf";
case Refined: return "Refined";
case Root: return "Root";
}
return "undefined";
}

std::string peano::grid::tests::records::TestCell::getStateMapping() {
return "State(Leaf=0,Refined=1,Root=2)";
}


std::string peano::grid::tests::records::TestCell::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCell::toString (std::ostream& out) const {
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


peano::grid::tests::records::TestCell::PersistentRecords peano::grid::tests::records::TestCell::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCellPacked peano::grid::tests::records::TestCell::convert() const{
return TestCellPacked(
getIsInside(),
getState(),
getEvenFlags(),
getAccessNumber()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCell::_log( "peano::grid::tests::records::TestCell" );

MPI_Datatype peano::grid::tests::records::TestCell::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCell::FullDatatype = 0;


void peano::grid::tests::records::TestCell::initDatatype() {
{
TestCell dummyTestCell[2];

const int Attributes = 2;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //state
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //state
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._state))), 		&disp[1] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::Datatype );
MPI_Type_commit( &TestCell::Datatype );

}
{
TestCell dummyTestCell[2];

const int Attributes = 5;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isInside
MPI_INT,		 //state
MPI_INT,		 //evenFlags
MPI_SHORT,		 //accessNumber
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isInside
1,		 //state
DIMENSIONS,		 //evenFlags
DIMENSIONS_TIMES_TWO,		 //accessNumber
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._isInside))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._isInside))), 		&disp[4] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::FullDatatype );
MPI_Type_commit( &TestCell::FullDatatype );

}

}


void peano::grid::tests::records::TestCell::shutdownDatatype() {
MPI_Type_free( &TestCell::Datatype );
MPI_Type_free( &TestCell::FullDatatype );

}

void peano::grid::tests::records::TestCell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCell "
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
"peano::grid::tests::records::TestCell",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



void peano::grid::tests::records::TestCell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCell failed: "
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
"peano::grid::tests::records::TestCell",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



bool peano::grid::tests::records::TestCell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCell::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords() {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
_accessNumber(accessNumber) {
setIsInside(isInside);
setState(state);
setEvenFlags(evenFlags);
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isInside ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::PersistentRecords::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(state) << (1));
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::PersistentRecords::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
_packedRecords0 = static_cast<int>(_packedRecords0 | evenFlags.to_ulong() << (3));
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}


peano::grid::tests::records::TestCellPacked::TestCellPacked() {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber) {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber):
_persistentRecords(isInside, state, evenFlags, accessNumber) {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::~TestCellPacked() { }


 bool peano::grid::tests::records::TestCellPacked::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isInside ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(state) << (1));
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | evenFlags.to_ulong() << (3));
}



 bool peano::grid::tests::records::TestCellPacked::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
return (_persistentRecords._packedRecords0& mask);
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
assertion(!evenFlags || evenFlags==1);
int shift        = 3 + elementIndex; 
int mask         = 1     << (shift);
int shiftedValue = evenFlags << (shift);
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 & ~mask;
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 |  shiftedValue;
}



 void peano::grid::tests::records::TestCellPacked::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
_persistentRecords._packedRecords0^= mask;
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCellPacked::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}


std::string peano::grid::tests::records::TestCellPacked::toString(const State& param) {
return peano::grid::tests::records::TestCell::toString(param);
}

std::string peano::grid::tests::records::TestCellPacked::getStateMapping() {
return peano::grid::tests::records::TestCell::getStateMapping();
}



std::string peano::grid::tests::records::TestCellPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCellPacked::toString (std::ostream& out) const {
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


peano::grid::tests::records::TestCellPacked::PersistentRecords peano::grid::tests::records::TestCellPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCell peano::grid::tests::records::TestCellPacked::convert() const{
return TestCell(
getIsInside(),
getState(),
getEvenFlags(),
getAccessNumber()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCellPacked::_log( "peano::grid::tests::records::TestCellPacked" );

MPI_Datatype peano::grid::tests::records::TestCellPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCellPacked::FullDatatype = 0;


void peano::grid::tests::records::TestCellPacked::initDatatype() {
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 2;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._packedRecords0))), 		&disp[1] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::Datatype );
MPI_Type_commit( &TestCellPacked::Datatype );

}
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 3;
MPI_Datatype subtypes[Attributes] = {
MPI_SHORT,		 //accessNumber
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyTestCellPacked[1]._persistentRecords._accessNumber[0])), 		&disp[2] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::FullDatatype );
MPI_Type_commit( &TestCellPacked::FullDatatype );

}

}


void peano::grid::tests::records::TestCellPacked::shutdownDatatype() {
MPI_Type_free( &TestCellPacked::Datatype );
MPI_Type_free( &TestCellPacked::FullDatatype );

}

void peano::grid::tests::records::TestCellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCellPacked "
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
"peano::grid::tests::records::TestCellPacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



void peano::grid::tests::records::TestCellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCellPacked failed: "
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
"peano::grid::tests::records::TestCellPacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



bool peano::grid::tests::records::TestCellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCellPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif defined(Parallel) && defined(SharedMemoryParallelisation) && defined(Debug)
peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_isInside(isInside),
_state(state),
_level(level),
_evenFlags(evenFlags),
_accessNumber(accessNumber),
_responsibleRank(responsibleRank),
_nodeWorkload(nodeWorkload),
_localWorkload(localWorkload),
_totalWorkload(totalWorkload),
_maxWorkload(maxWorkload),
_minWorkload(minWorkload),
_subtreeHoldsWorker(subtreeHoldsWorker),
_cellIsAForkCandidate(cellIsAForkCandidate),
_numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
_numberOfStoresToOutputStream(numberOfStoresToOutputStream) {

}


 bool peano::grid::tests::records::TestCell::PersistentRecords::getIsInside() const  {
return _isInside;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setIsInside(const bool& isInside)  {
_isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::PersistentRecords::getState() const  {
return _state;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setState(const State& state)  {
_state = state;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setLevel(const int& level)  {
_level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::PersistentRecords::getEvenFlags() const  {
return _evenFlags;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_evenFlags = (evenFlags);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getResponsibleRank() const  {
return _responsibleRank;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setResponsibleRank(const int& responsibleRank)  {
_responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getNodeWorkload() const  {
return _nodeWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNodeWorkload(const double& nodeWorkload)  {
_nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getLocalWorkload() const  {
return _localWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setLocalWorkload(const double& localWorkload)  {
_localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getTotalWorkload() const  {
return _totalWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setTotalWorkload(const double& totalWorkload)  {
_totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getMaxWorkload() const  {
return _maxWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setMaxWorkload(const double& maxWorkload)  {
_maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getMinWorkload() const  {
return _minWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setMinWorkload(const double& minWorkload)  {
_minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCell::PersistentRecords::getSubtreeHoldsWorker() const  {
return _subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCell::PersistentRecords::getCellIsAForkCandidate() const  {
return _cellIsAForkCandidate;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
_cellIsAForkCandidate = cellIsAForkCandidate;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getNumberOfLoadsFromInputStream() const  {
return _numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getNumberOfStoresToOutputStream() const  {
return _numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


peano::grid::tests::records::TestCell::TestCell() {

}


peano::grid::tests::records::TestCell::TestCell(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._nodeWorkload, persistentRecords._localWorkload, persistentRecords._totalWorkload, persistentRecords._maxWorkload, persistentRecords._minWorkload, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {

}


peano::grid::tests::records::TestCell::TestCell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, nodeWorkload, localWorkload, totalWorkload, maxWorkload, minWorkload, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {

}


peano::grid::tests::records::TestCell::~TestCell() { }


 bool peano::grid::tests::records::TestCell::getIsInside() const  {
return _persistentRecords._isInside;
}



 void peano::grid::tests::records::TestCell::setIsInside(const bool& isInside)  {
_persistentRecords._isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::getState() const  {
return _persistentRecords._state;
}



 void peano::grid::tests::records::TestCell::setState(const State& state)  {
_persistentRecords._state = state;
}



 int peano::grid::tests::records::TestCell::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestCell::setLevel(const int& level)  {
_persistentRecords._level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::getEvenFlags() const  {
return _persistentRecords._evenFlags;
}



 void peano::grid::tests::records::TestCell::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_persistentRecords._evenFlags = (evenFlags);
}



 bool peano::grid::tests::records::TestCell::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._evenFlags[elementIndex];

}



 void peano::grid::tests::records::TestCell::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags[elementIndex]= evenFlags;

}



 void peano::grid::tests::records::TestCell::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags.flip(elementIndex);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCell::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCell::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCell::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCell::getResponsibleRank() const  {
return _persistentRecords._responsibleRank;
}



 void peano::grid::tests::records::TestCell::setResponsibleRank(const int& responsibleRank)  {
_persistentRecords._responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCell::getNodeWorkload() const  {
return _persistentRecords._nodeWorkload;
}



 void peano::grid::tests::records::TestCell::setNodeWorkload(const double& nodeWorkload)  {
_persistentRecords._nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCell::getLocalWorkload() const  {
return _persistentRecords._localWorkload;
}



 void peano::grid::tests::records::TestCell::setLocalWorkload(const double& localWorkload)  {
_persistentRecords._localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCell::getTotalWorkload() const  {
return _persistentRecords._totalWorkload;
}



 void peano::grid::tests::records::TestCell::setTotalWorkload(const double& totalWorkload)  {
_persistentRecords._totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCell::getMaxWorkload() const  {
return _persistentRecords._maxWorkload;
}



 void peano::grid::tests::records::TestCell::setMaxWorkload(const double& maxWorkload)  {
_persistentRecords._maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCell::getMinWorkload() const  {
return _persistentRecords._minWorkload;
}



 void peano::grid::tests::records::TestCell::setMinWorkload(const double& minWorkload)  {
_persistentRecords._minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCell::getSubtreeHoldsWorker() const  {
return _persistentRecords._subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCell::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_persistentRecords._subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCell::getCellIsAForkCandidate() const  {
return _persistentRecords._cellIsAForkCandidate;
}



 void peano::grid::tests::records::TestCell::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
_persistentRecords._cellIsAForkCandidate = cellIsAForkCandidate;
}



 int peano::grid::tests::records::TestCell::getNumberOfLoadsFromInputStream() const  {
return _persistentRecords._numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCell::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_persistentRecords._numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCell::getNumberOfStoresToOutputStream() const  {
return _persistentRecords._numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCell::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_persistentRecords._numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


std::string peano::grid::tests::records::TestCell::toString(const State& param) {
switch (param) {
case Leaf: return "Leaf";
case Refined: return "Refined";
case Root: return "Root";
}
return "undefined";
}

std::string peano::grid::tests::records::TestCell::getStateMapping() {
return "State(Leaf=0,Refined=1,Root=2)";
}


std::string peano::grid::tests::records::TestCell::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCell::toString (std::ostream& out) const {
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
out << "nodeWorkload:" << getNodeWorkload();
out << ",";
out << "localWorkload:" << getLocalWorkload();
out << ",";
out << "totalWorkload:" << getTotalWorkload();
out << ",";
out << "maxWorkload:" << getMaxWorkload();
out << ",";
out << "minWorkload:" << getMinWorkload();
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


peano::grid::tests::records::TestCell::PersistentRecords peano::grid::tests::records::TestCell::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCellPacked peano::grid::tests::records::TestCell::convert() const{
return TestCellPacked(
getIsInside(),
getState(),
getLevel(),
getEvenFlags(),
getAccessNumber(),
getResponsibleRank(),
getNodeWorkload(),
getLocalWorkload(),
getTotalWorkload(),
getMaxWorkload(),
getMinWorkload(),
getSubtreeHoldsWorker(),
getCellIsAForkCandidate(),
getNumberOfLoadsFromInputStream(),
getNumberOfStoresToOutputStream()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCell::_log( "peano::grid::tests::records::TestCell" );

MPI_Datatype peano::grid::tests::records::TestCell::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCell::FullDatatype = 0;


void peano::grid::tests::records::TestCell::initDatatype() {
{
TestCell dummyTestCell[2];

const int Attributes = 6;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //state
MPI_INT,		 //level
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //state
1,		 //level
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._level))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._maxWorkload))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._minWorkload))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._state))), 		&disp[5] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::Datatype );
MPI_Type_commit( &TestCell::Datatype );

}
{
TestCell dummyTestCell[2];

const int Attributes = 16;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isInside
MPI_INT,		 //state
MPI_INT,		 //level
MPI_INT,		 //evenFlags
MPI_SHORT,		 //accessNumber
MPI_INT,		 //responsibleRank
MPI_DOUBLE,		 //nodeWorkload
MPI_DOUBLE,		 //localWorkload
MPI_DOUBLE,		 //totalWorkload
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_CHAR,		 //cellIsAForkCandidate
MPI_INT,		 //numberOfLoadsFromInputStream
MPI_INT,		 //numberOfStoresToOutputStream
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isInside
1,		 //state
1,		 //level
DIMENSIONS,		 //evenFlags
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //responsibleRank
1,		 //nodeWorkload
1,		 //localWorkload
1,		 //totalWorkload
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //cellIsAForkCandidate
1,		 //numberOfLoadsFromInputStream
1,		 //numberOfStoresToOutputStream
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._isInside))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._level))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._nodeWorkload))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._localWorkload))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._totalWorkload))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._maxWorkload))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._minWorkload))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[13] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[14] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._isInside))), 		&disp[15] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::FullDatatype );
MPI_Type_commit( &TestCell::FullDatatype );

}

}


void peano::grid::tests::records::TestCell::shutdownDatatype() {
MPI_Type_free( &TestCell::Datatype );
MPI_Type_free( &TestCell::FullDatatype );

}

void peano::grid::tests::records::TestCell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCell "
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
"peano::grid::tests::records::TestCell",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



void peano::grid::tests::records::TestCell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCell failed: "
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
"peano::grid::tests::records::TestCell",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



bool peano::grid::tests::records::TestCell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCell::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords() {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_level(level),
_accessNumber(accessNumber),
_responsibleRank(responsibleRank),
_nodeWorkload(nodeWorkload),
_localWorkload(localWorkload),
_totalWorkload(totalWorkload),
_maxWorkload(maxWorkload),
_minWorkload(minWorkload),
_subtreeHoldsWorker(subtreeHoldsWorker),
_numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
_numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
setIsInside(isInside);
setState(state);
setEvenFlags(evenFlags);
setCellIsAForkCandidate(cellIsAForkCandidate);
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isInside ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::PersistentRecords::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(state) << (1));
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setLevel(const int& level)  {
_level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::PersistentRecords::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
_packedRecords0 = static_cast<int>(_packedRecords0 | evenFlags.to_ulong() << (3));
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getResponsibleRank() const  {
return _responsibleRank;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setResponsibleRank(const int& responsibleRank)  {
_responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getNodeWorkload() const  {
return _nodeWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNodeWorkload(const double& nodeWorkload)  {
_nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getLocalWorkload() const  {
return _localWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setLocalWorkload(const double& localWorkload)  {
_localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getTotalWorkload() const  {
return _totalWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setTotalWorkload(const double& totalWorkload)  {
_totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getMaxWorkload() const  {
return _maxWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setMaxWorkload(const double& maxWorkload)  {
_maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getMinWorkload() const  {
return _minWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setMinWorkload(const double& minWorkload)  {
_minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getSubtreeHoldsWorker() const  {
return _subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getCellIsAForkCandidate() const  {
int mask = 1 << (DIMENSIONS + 3);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
int mask = 1 << (DIMENSIONS + 3);
   _packedRecords0 = static_cast<int>( cellIsAForkCandidate ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getNumberOfLoadsFromInputStream() const  {
return _numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getNumberOfStoresToOutputStream() const  {
return _numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


peano::grid::tests::records::TestCellPacked::TestCellPacked() {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._nodeWorkload, persistentRecords._localWorkload, persistentRecords._totalWorkload, persistentRecords._maxWorkload, persistentRecords._minWorkload, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate(), persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, nodeWorkload, localWorkload, totalWorkload, maxWorkload, minWorkload, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::~TestCellPacked() { }


 bool peano::grid::tests::records::TestCellPacked::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isInside ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(state) << (1));
}



 int peano::grid::tests::records::TestCellPacked::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestCellPacked::setLevel(const int& level)  {
_persistentRecords._level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | evenFlags.to_ulong() << (3));
}



 bool peano::grid::tests::records::TestCellPacked::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
return (_persistentRecords._packedRecords0& mask);
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
assertion(!evenFlags || evenFlags==1);
int shift        = 3 + elementIndex; 
int mask         = 1     << (shift);
int shiftedValue = evenFlags << (shift);
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 & ~mask;
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 |  shiftedValue;
}



 void peano::grid::tests::records::TestCellPacked::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
_persistentRecords._packedRecords0^= mask;
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCellPacked::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCellPacked::getResponsibleRank() const  {
return _persistentRecords._responsibleRank;
}



 void peano::grid::tests::records::TestCellPacked::setResponsibleRank(const int& responsibleRank)  {
_persistentRecords._responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCellPacked::getNodeWorkload() const  {
return _persistentRecords._nodeWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setNodeWorkload(const double& nodeWorkload)  {
_persistentRecords._nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getLocalWorkload() const  {
return _persistentRecords._localWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setLocalWorkload(const double& localWorkload)  {
_persistentRecords._localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getTotalWorkload() const  {
return _persistentRecords._totalWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setTotalWorkload(const double& totalWorkload)  {
_persistentRecords._totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getMaxWorkload() const  {
return _persistentRecords._maxWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setMaxWorkload(const double& maxWorkload)  {
_persistentRecords._maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getMinWorkload() const  {
return _persistentRecords._minWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setMinWorkload(const double& minWorkload)  {
_persistentRecords._minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCellPacked::getSubtreeHoldsWorker() const  {
return _persistentRecords._subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCellPacked::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_persistentRecords._subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCellPacked::getCellIsAForkCandidate() const  {
int mask = 1 << (DIMENSIONS + 3);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
int mask = 1 << (DIMENSIONS + 3);
   _persistentRecords._packedRecords0 = static_cast<int>( cellIsAForkCandidate ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 int peano::grid::tests::records::TestCellPacked::getNumberOfLoadsFromInputStream() const  {
return _persistentRecords._numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCellPacked::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_persistentRecords._numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCellPacked::getNumberOfStoresToOutputStream() const  {
return _persistentRecords._numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCellPacked::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_persistentRecords._numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


std::string peano::grid::tests::records::TestCellPacked::toString(const State& param) {
return peano::grid::tests::records::TestCell::toString(param);
}

std::string peano::grid::tests::records::TestCellPacked::getStateMapping() {
return peano::grid::tests::records::TestCell::getStateMapping();
}



std::string peano::grid::tests::records::TestCellPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCellPacked::toString (std::ostream& out) const {
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
out << "nodeWorkload:" << getNodeWorkload();
out << ",";
out << "localWorkload:" << getLocalWorkload();
out << ",";
out << "totalWorkload:" << getTotalWorkload();
out << ",";
out << "maxWorkload:" << getMaxWorkload();
out << ",";
out << "minWorkload:" << getMinWorkload();
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


peano::grid::tests::records::TestCellPacked::PersistentRecords peano::grid::tests::records::TestCellPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCell peano::grid::tests::records::TestCellPacked::convert() const{
return TestCell(
getIsInside(),
getState(),
getLevel(),
getEvenFlags(),
getAccessNumber(),
getResponsibleRank(),
getNodeWorkload(),
getLocalWorkload(),
getTotalWorkload(),
getMaxWorkload(),
getMinWorkload(),
getSubtreeHoldsWorker(),
getCellIsAForkCandidate(),
getNumberOfLoadsFromInputStream(),
getNumberOfStoresToOutputStream()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCellPacked::_log( "peano::grid::tests::records::TestCellPacked" );

MPI_Datatype peano::grid::tests::records::TestCellPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCellPacked::FullDatatype = 0;


void peano::grid::tests::records::TestCellPacked::initDatatype() {
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 6;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //level
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //level
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._level))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._maxWorkload))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._minWorkload))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._level))), 		&disp[5] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::Datatype );
MPI_Type_commit( &TestCellPacked::Datatype );

}
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 13;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //level
MPI_SHORT,		 //accessNumber
MPI_INT,		 //responsibleRank
MPI_DOUBLE,		 //nodeWorkload
MPI_DOUBLE,		 //localWorkload
MPI_DOUBLE,		 //totalWorkload
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_INT,		 //numberOfLoadsFromInputStream
MPI_INT,		 //numberOfStoresToOutputStream
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //level
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //responsibleRank
1,		 //nodeWorkload
1,		 //localWorkload
1,		 //totalWorkload
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //numberOfLoadsFromInputStream
1,		 //numberOfStoresToOutputStream
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._level))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._nodeWorkload))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._localWorkload))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._totalWorkload))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._maxWorkload))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._minWorkload))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._level))), 		&disp[12] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::FullDatatype );
MPI_Type_commit( &TestCellPacked::FullDatatype );

}

}


void peano::grid::tests::records::TestCellPacked::shutdownDatatype() {
MPI_Type_free( &TestCellPacked::Datatype );
MPI_Type_free( &TestCellPacked::FullDatatype );

}

void peano::grid::tests::records::TestCellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCellPacked "
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
"peano::grid::tests::records::TestCellPacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



void peano::grid::tests::records::TestCellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCellPacked failed: "
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
"peano::grid::tests::records::TestCellPacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



bool peano::grid::tests::records::TestCellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCellPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif defined(Parallel) && defined(Debug) && !defined(SharedMemoryParallelisation)
peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
_isInside(isInside),
_state(state),
_level(level),
_evenFlags(evenFlags),
_accessNumber(accessNumber),
_responsibleRank(responsibleRank),
_nodeWorkload(nodeWorkload),
_localWorkload(localWorkload),
_totalWorkload(totalWorkload),
_maxWorkload(maxWorkload),
_minWorkload(minWorkload),
_subtreeHoldsWorker(subtreeHoldsWorker),
_cellIsAForkCandidate(cellIsAForkCandidate) {

}


 bool peano::grid::tests::records::TestCell::PersistentRecords::getIsInside() const  {
return _isInside;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setIsInside(const bool& isInside)  {
_isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::PersistentRecords::getState() const  {
return _state;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setState(const State& state)  {
_state = state;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setLevel(const int& level)  {
_level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::PersistentRecords::getEvenFlags() const  {
return _evenFlags;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_evenFlags = (evenFlags);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getResponsibleRank() const  {
return _responsibleRank;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setResponsibleRank(const int& responsibleRank)  {
_responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getNodeWorkload() const  {
return _nodeWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNodeWorkload(const double& nodeWorkload)  {
_nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getLocalWorkload() const  {
return _localWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setLocalWorkload(const double& localWorkload)  {
_localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getTotalWorkload() const  {
return _totalWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setTotalWorkload(const double& totalWorkload)  {
_totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getMaxWorkload() const  {
return _maxWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setMaxWorkload(const double& maxWorkload)  {
_maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getMinWorkload() const  {
return _minWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setMinWorkload(const double& minWorkload)  {
_minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCell::PersistentRecords::getSubtreeHoldsWorker() const  {
return _subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCell::PersistentRecords::getCellIsAForkCandidate() const  {
return _cellIsAForkCandidate;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
_cellIsAForkCandidate = cellIsAForkCandidate;
}


peano::grid::tests::records::TestCell::TestCell() {

}


peano::grid::tests::records::TestCell::TestCell(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._nodeWorkload, persistentRecords._localWorkload, persistentRecords._totalWorkload, persistentRecords._maxWorkload, persistentRecords._minWorkload, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate) {

}


peano::grid::tests::records::TestCell::TestCell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
_persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, nodeWorkload, localWorkload, totalWorkload, maxWorkload, minWorkload, subtreeHoldsWorker, cellIsAForkCandidate) {

}


peano::grid::tests::records::TestCell::~TestCell() { }


 bool peano::grid::tests::records::TestCell::getIsInside() const  {
return _persistentRecords._isInside;
}



 void peano::grid::tests::records::TestCell::setIsInside(const bool& isInside)  {
_persistentRecords._isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::getState() const  {
return _persistentRecords._state;
}



 void peano::grid::tests::records::TestCell::setState(const State& state)  {
_persistentRecords._state = state;
}



 int peano::grid::tests::records::TestCell::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestCell::setLevel(const int& level)  {
_persistentRecords._level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::getEvenFlags() const  {
return _persistentRecords._evenFlags;
}



 void peano::grid::tests::records::TestCell::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_persistentRecords._evenFlags = (evenFlags);
}



 bool peano::grid::tests::records::TestCell::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._evenFlags[elementIndex];

}



 void peano::grid::tests::records::TestCell::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags[elementIndex]= evenFlags;

}



 void peano::grid::tests::records::TestCell::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags.flip(elementIndex);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCell::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCell::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCell::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCell::getResponsibleRank() const  {
return _persistentRecords._responsibleRank;
}



 void peano::grid::tests::records::TestCell::setResponsibleRank(const int& responsibleRank)  {
_persistentRecords._responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCell::getNodeWorkload() const  {
return _persistentRecords._nodeWorkload;
}



 void peano::grid::tests::records::TestCell::setNodeWorkload(const double& nodeWorkload)  {
_persistentRecords._nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCell::getLocalWorkload() const  {
return _persistentRecords._localWorkload;
}



 void peano::grid::tests::records::TestCell::setLocalWorkload(const double& localWorkload)  {
_persistentRecords._localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCell::getTotalWorkload() const  {
return _persistentRecords._totalWorkload;
}



 void peano::grid::tests::records::TestCell::setTotalWorkload(const double& totalWorkload)  {
_persistentRecords._totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCell::getMaxWorkload() const  {
return _persistentRecords._maxWorkload;
}



 void peano::grid::tests::records::TestCell::setMaxWorkload(const double& maxWorkload)  {
_persistentRecords._maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCell::getMinWorkload() const  {
return _persistentRecords._minWorkload;
}



 void peano::grid::tests::records::TestCell::setMinWorkload(const double& minWorkload)  {
_persistentRecords._minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCell::getSubtreeHoldsWorker() const  {
return _persistentRecords._subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCell::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_persistentRecords._subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCell::getCellIsAForkCandidate() const  {
return _persistentRecords._cellIsAForkCandidate;
}



 void peano::grid::tests::records::TestCell::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
_persistentRecords._cellIsAForkCandidate = cellIsAForkCandidate;
}


std::string peano::grid::tests::records::TestCell::toString(const State& param) {
switch (param) {
case Leaf: return "Leaf";
case Refined: return "Refined";
case Root: return "Root";
}
return "undefined";
}

std::string peano::grid::tests::records::TestCell::getStateMapping() {
return "State(Leaf=0,Refined=1,Root=2)";
}


std::string peano::grid::tests::records::TestCell::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCell::toString (std::ostream& out) const {
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
out << "nodeWorkload:" << getNodeWorkload();
out << ",";
out << "localWorkload:" << getLocalWorkload();
out << ",";
out << "totalWorkload:" << getTotalWorkload();
out << ",";
out << "maxWorkload:" << getMaxWorkload();
out << ",";
out << "minWorkload:" << getMinWorkload();
out << ",";
out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
out << ",";
out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
out <<  ")";
}


peano::grid::tests::records::TestCell::PersistentRecords peano::grid::tests::records::TestCell::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCellPacked peano::grid::tests::records::TestCell::convert() const{
return TestCellPacked(
getIsInside(),
getState(),
getLevel(),
getEvenFlags(),
getAccessNumber(),
getResponsibleRank(),
getNodeWorkload(),
getLocalWorkload(),
getTotalWorkload(),
getMaxWorkload(),
getMinWorkload(),
getSubtreeHoldsWorker(),
getCellIsAForkCandidate()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCell::_log( "peano::grid::tests::records::TestCell" );

MPI_Datatype peano::grid::tests::records::TestCell::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCell::FullDatatype = 0;


void peano::grid::tests::records::TestCell::initDatatype() {
{
TestCell dummyTestCell[2];

const int Attributes = 6;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //state
MPI_INT,		 //level
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //state
1,		 //level
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._level))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._maxWorkload))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._minWorkload))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._state))), 		&disp[5] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::Datatype );
MPI_Type_commit( &TestCell::Datatype );

}
{
TestCell dummyTestCell[2];

const int Attributes = 14;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isInside
MPI_INT,		 //state
MPI_INT,		 //level
MPI_INT,		 //evenFlags
MPI_SHORT,		 //accessNumber
MPI_INT,		 //responsibleRank
MPI_DOUBLE,		 //nodeWorkload
MPI_DOUBLE,		 //localWorkload
MPI_DOUBLE,		 //totalWorkload
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_CHAR,		 //cellIsAForkCandidate
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isInside
1,		 //state
1,		 //level
DIMENSIONS,		 //evenFlags
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //responsibleRank
1,		 //nodeWorkload
1,		 //localWorkload
1,		 //totalWorkload
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //cellIsAForkCandidate
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._isInside))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._level))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._responsibleRank))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._nodeWorkload))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._localWorkload))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._totalWorkload))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._maxWorkload))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._minWorkload))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._isInside))), 		&disp[13] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::FullDatatype );
MPI_Type_commit( &TestCell::FullDatatype );

}

}


void peano::grid::tests::records::TestCell::shutdownDatatype() {
MPI_Type_free( &TestCell::Datatype );
MPI_Type_free( &TestCell::FullDatatype );

}

void peano::grid::tests::records::TestCell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCell "
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
"peano::grid::tests::records::TestCell",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



void peano::grid::tests::records::TestCell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCell failed: "
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
"peano::grid::tests::records::TestCell",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



bool peano::grid::tests::records::TestCell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCell::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords() {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
_level(level),
_accessNumber(accessNumber),
_responsibleRank(responsibleRank),
_nodeWorkload(nodeWorkload),
_localWorkload(localWorkload),
_totalWorkload(totalWorkload),
_maxWorkload(maxWorkload),
_minWorkload(minWorkload),
_subtreeHoldsWorker(subtreeHoldsWorker) {
setIsInside(isInside);
setState(state);
setEvenFlags(evenFlags);
setCellIsAForkCandidate(cellIsAForkCandidate);
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isInside ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::PersistentRecords::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(state) << (1));
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setLevel(const int& level)  {
_level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::PersistentRecords::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
_packedRecords0 = static_cast<int>(_packedRecords0 | evenFlags.to_ulong() << (3));
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getResponsibleRank() const  {
return _responsibleRank;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setResponsibleRank(const int& responsibleRank)  {
_responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getNodeWorkload() const  {
return _nodeWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNodeWorkload(const double& nodeWorkload)  {
_nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getLocalWorkload() const  {
return _localWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setLocalWorkload(const double& localWorkload)  {
_localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getTotalWorkload() const  {
return _totalWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setTotalWorkload(const double& totalWorkload)  {
_totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getMaxWorkload() const  {
return _maxWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setMaxWorkload(const double& maxWorkload)  {
_maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getMinWorkload() const  {
return _minWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setMinWorkload(const double& minWorkload)  {
_minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getSubtreeHoldsWorker() const  {
return _subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getCellIsAForkCandidate() const  {
int mask = 1 << (DIMENSIONS + 3);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
int mask = 1 << (DIMENSIONS + 3);
   _packedRecords0 = static_cast<int>( cellIsAForkCandidate ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}


peano::grid::tests::records::TestCellPacked::TestCellPacked() {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._nodeWorkload, persistentRecords._localWorkload, persistentRecords._totalWorkload, persistentRecords._maxWorkload, persistentRecords._minWorkload, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate()) {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate):
_persistentRecords(isInside, state, level, evenFlags, accessNumber, responsibleRank, nodeWorkload, localWorkload, totalWorkload, maxWorkload, minWorkload, subtreeHoldsWorker, cellIsAForkCandidate) {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::~TestCellPacked() { }


 bool peano::grid::tests::records::TestCellPacked::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isInside ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(state) << (1));
}



 int peano::grid::tests::records::TestCellPacked::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestCellPacked::setLevel(const int& level)  {
_persistentRecords._level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | evenFlags.to_ulong() << (3));
}



 bool peano::grid::tests::records::TestCellPacked::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
return (_persistentRecords._packedRecords0& mask);
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
assertion(!evenFlags || evenFlags==1);
int shift        = 3 + elementIndex; 
int mask         = 1     << (shift);
int shiftedValue = evenFlags << (shift);
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 & ~mask;
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 |  shiftedValue;
}



 void peano::grid::tests::records::TestCellPacked::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
_persistentRecords._packedRecords0^= mask;
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCellPacked::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCellPacked::getResponsibleRank() const  {
return _persistentRecords._responsibleRank;
}



 void peano::grid::tests::records::TestCellPacked::setResponsibleRank(const int& responsibleRank)  {
_persistentRecords._responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCellPacked::getNodeWorkload() const  {
return _persistentRecords._nodeWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setNodeWorkload(const double& nodeWorkload)  {
_persistentRecords._nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getLocalWorkload() const  {
return _persistentRecords._localWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setLocalWorkload(const double& localWorkload)  {
_persistentRecords._localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getTotalWorkload() const  {
return _persistentRecords._totalWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setTotalWorkload(const double& totalWorkload)  {
_persistentRecords._totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getMaxWorkload() const  {
return _persistentRecords._maxWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setMaxWorkload(const double& maxWorkload)  {
_persistentRecords._maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getMinWorkload() const  {
return _persistentRecords._minWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setMinWorkload(const double& minWorkload)  {
_persistentRecords._minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCellPacked::getSubtreeHoldsWorker() const  {
return _persistentRecords._subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCellPacked::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_persistentRecords._subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCellPacked::getCellIsAForkCandidate() const  {
int mask = 1 << (DIMENSIONS + 3);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
int mask = 1 << (DIMENSIONS + 3);
   _persistentRecords._packedRecords0 = static_cast<int>( cellIsAForkCandidate ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}


std::string peano::grid::tests::records::TestCellPacked::toString(const State& param) {
return peano::grid::tests::records::TestCell::toString(param);
}

std::string peano::grid::tests::records::TestCellPacked::getStateMapping() {
return peano::grid::tests::records::TestCell::getStateMapping();
}



std::string peano::grid::tests::records::TestCellPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCellPacked::toString (std::ostream& out) const {
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
out << "nodeWorkload:" << getNodeWorkload();
out << ",";
out << "localWorkload:" << getLocalWorkload();
out << ",";
out << "totalWorkload:" << getTotalWorkload();
out << ",";
out << "maxWorkload:" << getMaxWorkload();
out << ",";
out << "minWorkload:" << getMinWorkload();
out << ",";
out << "subtreeHoldsWorker:" << getSubtreeHoldsWorker();
out << ",";
out << "cellIsAForkCandidate:" << getCellIsAForkCandidate();
out <<  ")";
}


peano::grid::tests::records::TestCellPacked::PersistentRecords peano::grid::tests::records::TestCellPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCell peano::grid::tests::records::TestCellPacked::convert() const{
return TestCell(
getIsInside(),
getState(),
getLevel(),
getEvenFlags(),
getAccessNumber(),
getResponsibleRank(),
getNodeWorkload(),
getLocalWorkload(),
getTotalWorkload(),
getMaxWorkload(),
getMinWorkload(),
getSubtreeHoldsWorker(),
getCellIsAForkCandidate()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCellPacked::_log( "peano::grid::tests::records::TestCellPacked" );

MPI_Datatype peano::grid::tests::records::TestCellPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCellPacked::FullDatatype = 0;


void peano::grid::tests::records::TestCellPacked::initDatatype() {
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 6;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //level
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //level
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._level))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._maxWorkload))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._minWorkload))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._level))), 		&disp[5] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::Datatype );
MPI_Type_commit( &TestCellPacked::Datatype );

}
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 11;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //level
MPI_SHORT,		 //accessNumber
MPI_INT,		 //responsibleRank
MPI_DOUBLE,		 //nodeWorkload
MPI_DOUBLE,		 //localWorkload
MPI_DOUBLE,		 //totalWorkload
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //level
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //responsibleRank
1,		 //nodeWorkload
1,		 //localWorkload
1,		 //totalWorkload
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._level))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._nodeWorkload))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._localWorkload))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._totalWorkload))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._maxWorkload))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._minWorkload))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._level))), 		&disp[10] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::FullDatatype );
MPI_Type_commit( &TestCellPacked::FullDatatype );

}

}


void peano::grid::tests::records::TestCellPacked::shutdownDatatype() {
MPI_Type_free( &TestCellPacked::Datatype );
MPI_Type_free( &TestCellPacked::FullDatatype );

}

void peano::grid::tests::records::TestCellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCellPacked "
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
"peano::grid::tests::records::TestCellPacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



void peano::grid::tests::records::TestCellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCellPacked failed: "
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
"peano::grid::tests::records::TestCellPacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



bool peano::grid::tests::records::TestCellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCellPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif defined(Parallel) && !defined(Debug) && defined(SharedMemoryParallelisation)
peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_isInside(isInside),
_state(state),
_evenFlags(evenFlags),
_accessNumber(accessNumber),
_responsibleRank(responsibleRank),
_nodeWorkload(nodeWorkload),
_localWorkload(localWorkload),
_totalWorkload(totalWorkload),
_maxWorkload(maxWorkload),
_minWorkload(minWorkload),
_subtreeHoldsWorker(subtreeHoldsWorker),
_cellIsAForkCandidate(cellIsAForkCandidate),
_numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
_numberOfStoresToOutputStream(numberOfStoresToOutputStream) {

}


 bool peano::grid::tests::records::TestCell::PersistentRecords::getIsInside() const  {
return _isInside;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setIsInside(const bool& isInside)  {
_isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::PersistentRecords::getState() const  {
return _state;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setState(const State& state)  {
_state = state;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::PersistentRecords::getEvenFlags() const  {
return _evenFlags;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_evenFlags = (evenFlags);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getResponsibleRank() const  {
return _responsibleRank;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setResponsibleRank(const int& responsibleRank)  {
_responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getNodeWorkload() const  {
return _nodeWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNodeWorkload(const double& nodeWorkload)  {
_nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getLocalWorkload() const  {
return _localWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setLocalWorkload(const double& localWorkload)  {
_localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getTotalWorkload() const  {
return _totalWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setTotalWorkload(const double& totalWorkload)  {
_totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getMaxWorkload() const  {
return _maxWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setMaxWorkload(const double& maxWorkload)  {
_maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCell::PersistentRecords::getMinWorkload() const  {
return _minWorkload;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setMinWorkload(const double& minWorkload)  {
_minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCell::PersistentRecords::getSubtreeHoldsWorker() const  {
return _subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCell::PersistentRecords::getCellIsAForkCandidate() const  {
return _cellIsAForkCandidate;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
_cellIsAForkCandidate = cellIsAForkCandidate;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getNumberOfLoadsFromInputStream() const  {
return _numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getNumberOfStoresToOutputStream() const  {
return _numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


peano::grid::tests::records::TestCell::TestCell() {

}


peano::grid::tests::records::TestCell::TestCell(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._nodeWorkload, persistentRecords._localWorkload, persistentRecords._totalWorkload, persistentRecords._maxWorkload, persistentRecords._minWorkload, persistentRecords._subtreeHoldsWorker, persistentRecords._cellIsAForkCandidate, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {

}


peano::grid::tests::records::TestCell::TestCell(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, nodeWorkload, localWorkload, totalWorkload, maxWorkload, minWorkload, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {

}


peano::grid::tests::records::TestCell::~TestCell() { }


 bool peano::grid::tests::records::TestCell::getIsInside() const  {
return _persistentRecords._isInside;
}



 void peano::grid::tests::records::TestCell::setIsInside(const bool& isInside)  {
_persistentRecords._isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::getState() const  {
return _persistentRecords._state;
}



 void peano::grid::tests::records::TestCell::setState(const State& state)  {
_persistentRecords._state = state;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::getEvenFlags() const  {
return _persistentRecords._evenFlags;
}



 void peano::grid::tests::records::TestCell::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_persistentRecords._evenFlags = (evenFlags);
}



 bool peano::grid::tests::records::TestCell::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._evenFlags[elementIndex];

}



 void peano::grid::tests::records::TestCell::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags[elementIndex]= evenFlags;

}



 void peano::grid::tests::records::TestCell::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags.flip(elementIndex);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCell::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCell::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCell::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCell::getResponsibleRank() const  {
return _persistentRecords._responsibleRank;
}



 void peano::grid::tests::records::TestCell::setResponsibleRank(const int& responsibleRank)  {
_persistentRecords._responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCell::getNodeWorkload() const  {
return _persistentRecords._nodeWorkload;
}



 void peano::grid::tests::records::TestCell::setNodeWorkload(const double& nodeWorkload)  {
_persistentRecords._nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCell::getLocalWorkload() const  {
return _persistentRecords._localWorkload;
}



 void peano::grid::tests::records::TestCell::setLocalWorkload(const double& localWorkload)  {
_persistentRecords._localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCell::getTotalWorkload() const  {
return _persistentRecords._totalWorkload;
}



 void peano::grid::tests::records::TestCell::setTotalWorkload(const double& totalWorkload)  {
_persistentRecords._totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCell::getMaxWorkload() const  {
return _persistentRecords._maxWorkload;
}



 void peano::grid::tests::records::TestCell::setMaxWorkload(const double& maxWorkload)  {
_persistentRecords._maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCell::getMinWorkload() const  {
return _persistentRecords._minWorkload;
}



 void peano::grid::tests::records::TestCell::setMinWorkload(const double& minWorkload)  {
_persistentRecords._minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCell::getSubtreeHoldsWorker() const  {
return _persistentRecords._subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCell::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_persistentRecords._subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCell::getCellIsAForkCandidate() const  {
return _persistentRecords._cellIsAForkCandidate;
}



 void peano::grid::tests::records::TestCell::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
_persistentRecords._cellIsAForkCandidate = cellIsAForkCandidate;
}



 int peano::grid::tests::records::TestCell::getNumberOfLoadsFromInputStream() const  {
return _persistentRecords._numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCell::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_persistentRecords._numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCell::getNumberOfStoresToOutputStream() const  {
return _persistentRecords._numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCell::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_persistentRecords._numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


std::string peano::grid::tests::records::TestCell::toString(const State& param) {
switch (param) {
case Leaf: return "Leaf";
case Refined: return "Refined";
case Root: return "Root";
}
return "undefined";
}

std::string peano::grid::tests::records::TestCell::getStateMapping() {
return "State(Leaf=0,Refined=1,Root=2)";
}


std::string peano::grid::tests::records::TestCell::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCell::toString (std::ostream& out) const {
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
out << "nodeWorkload:" << getNodeWorkload();
out << ",";
out << "localWorkload:" << getLocalWorkload();
out << ",";
out << "totalWorkload:" << getTotalWorkload();
out << ",";
out << "maxWorkload:" << getMaxWorkload();
out << ",";
out << "minWorkload:" << getMinWorkload();
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


peano::grid::tests::records::TestCell::PersistentRecords peano::grid::tests::records::TestCell::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCellPacked peano::grid::tests::records::TestCell::convert() const{
return TestCellPacked(
getIsInside(),
getState(),
getEvenFlags(),
getAccessNumber(),
getResponsibleRank(),
getNodeWorkload(),
getLocalWorkload(),
getTotalWorkload(),
getMaxWorkload(),
getMinWorkload(),
getSubtreeHoldsWorker(),
getCellIsAForkCandidate(),
getNumberOfLoadsFromInputStream(),
getNumberOfStoresToOutputStream()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCell::_log( "peano::grid::tests::records::TestCell" );

MPI_Datatype peano::grid::tests::records::TestCell::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCell::FullDatatype = 0;


void peano::grid::tests::records::TestCell::initDatatype() {
{
TestCell dummyTestCell[2];

const int Attributes = 5;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //state
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //state
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._maxWorkload))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._minWorkload))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._state))), 		&disp[4] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::Datatype );
MPI_Type_commit( &TestCell::Datatype );

}
{
TestCell dummyTestCell[2];

const int Attributes = 15;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isInside
MPI_INT,		 //state
MPI_INT,		 //evenFlags
MPI_SHORT,		 //accessNumber
MPI_INT,		 //responsibleRank
MPI_DOUBLE,		 //nodeWorkload
MPI_DOUBLE,		 //localWorkload
MPI_DOUBLE,		 //totalWorkload
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_CHAR,		 //cellIsAForkCandidate
MPI_INT,		 //numberOfLoadsFromInputStream
MPI_INT,		 //numberOfStoresToOutputStream
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isInside
1,		 //state
DIMENSIONS,		 //evenFlags
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //responsibleRank
1,		 //nodeWorkload
1,		 //localWorkload
1,		 //totalWorkload
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //cellIsAForkCandidate
1,		 //numberOfLoadsFromInputStream
1,		 //numberOfStoresToOutputStream
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._isInside))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._evenFlags))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._accessNumber[0]))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._responsibleRank))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._nodeWorkload))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._localWorkload))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._totalWorkload))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._maxWorkload))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._minWorkload))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._cellIsAForkCandidate))), 		&disp[11] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[12] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[13] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._isInside))), 		&disp[14] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::FullDatatype );
MPI_Type_commit( &TestCell::FullDatatype );

}

}


void peano::grid::tests::records::TestCell::shutdownDatatype() {
MPI_Type_free( &TestCell::Datatype );
MPI_Type_free( &TestCell::FullDatatype );

}

void peano::grid::tests::records::TestCell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCell "
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
"peano::grid::tests::records::TestCell",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



void peano::grid::tests::records::TestCell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCell failed: "
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
"peano::grid::tests::records::TestCell",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



bool peano::grid::tests::records::TestCell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCell::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords() {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_accessNumber(accessNumber),
_responsibleRank(responsibleRank),
_nodeWorkload(nodeWorkload),
_localWorkload(localWorkload),
_totalWorkload(totalWorkload),
_maxWorkload(maxWorkload),
_minWorkload(minWorkload),
_subtreeHoldsWorker(subtreeHoldsWorker),
_numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
_numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
setIsInside(isInside);
setState(state);
setEvenFlags(evenFlags);
setCellIsAForkCandidate(cellIsAForkCandidate);
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isInside ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::PersistentRecords::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(state) << (1));
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::PersistentRecords::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
_packedRecords0 = static_cast<int>(_packedRecords0 | evenFlags.to_ulong() << (3));
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getResponsibleRank() const  {
return _responsibleRank;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setResponsibleRank(const int& responsibleRank)  {
_responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getNodeWorkload() const  {
return _nodeWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNodeWorkload(const double& nodeWorkload)  {
_nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getLocalWorkload() const  {
return _localWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setLocalWorkload(const double& localWorkload)  {
_localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getTotalWorkload() const  {
return _totalWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setTotalWorkload(const double& totalWorkload)  {
_totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getMaxWorkload() const  {
return _maxWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setMaxWorkload(const double& maxWorkload)  {
_maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCellPacked::PersistentRecords::getMinWorkload() const  {
return _minWorkload;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setMinWorkload(const double& minWorkload)  {
_minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getSubtreeHoldsWorker() const  {
return _subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getCellIsAForkCandidate() const  {
int mask = 1 << (DIMENSIONS + 3);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
int mask = 1 << (DIMENSIONS + 3);
   _packedRecords0 = static_cast<int>( cellIsAForkCandidate ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getNumberOfLoadsFromInputStream() const  {
return _numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getNumberOfStoresToOutputStream() const  {
return _numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


peano::grid::tests::records::TestCellPacked::TestCellPacked() {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._responsibleRank, persistentRecords._nodeWorkload, persistentRecords._localWorkload, persistentRecords._totalWorkload, persistentRecords._maxWorkload, persistentRecords._minWorkload, persistentRecords._subtreeHoldsWorker, persistentRecords.getCellIsAForkCandidate(), persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const bool& isInside, const State& state, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& responsibleRank, const double& nodeWorkload, const double& localWorkload, const double& totalWorkload, const double& maxWorkload, const double& minWorkload, const bool& subtreeHoldsWorker, const bool& cellIsAForkCandidate, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_persistentRecords(isInside, state, evenFlags, accessNumber, responsibleRank, nodeWorkload, localWorkload, totalWorkload, maxWorkload, minWorkload, subtreeHoldsWorker, cellIsAForkCandidate, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
if ((DIMENSIONS+4 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+4 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::~TestCellPacked() { }


 bool peano::grid::tests::records::TestCellPacked::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isInside ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(state) << (1));
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | evenFlags.to_ulong() << (3));
}



 bool peano::grid::tests::records::TestCellPacked::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
return (_persistentRecords._packedRecords0& mask);
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
assertion(!evenFlags || evenFlags==1);
int shift        = 3 + elementIndex; 
int mask         = 1     << (shift);
int shiftedValue = evenFlags << (shift);
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 & ~mask;
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 |  shiftedValue;
}



 void peano::grid::tests::records::TestCellPacked::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
_persistentRecords._packedRecords0^= mask;
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCellPacked::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCellPacked::getResponsibleRank() const  {
return _persistentRecords._responsibleRank;
}



 void peano::grid::tests::records::TestCellPacked::setResponsibleRank(const int& responsibleRank)  {
_persistentRecords._responsibleRank = responsibleRank;
}



 double peano::grid::tests::records::TestCellPacked::getNodeWorkload() const  {
return _persistentRecords._nodeWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setNodeWorkload(const double& nodeWorkload)  {
_persistentRecords._nodeWorkload = nodeWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getLocalWorkload() const  {
return _persistentRecords._localWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setLocalWorkload(const double& localWorkload)  {
_persistentRecords._localWorkload = localWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getTotalWorkload() const  {
return _persistentRecords._totalWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setTotalWorkload(const double& totalWorkload)  {
_persistentRecords._totalWorkload = totalWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getMaxWorkload() const  {
return _persistentRecords._maxWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setMaxWorkload(const double& maxWorkload)  {
_persistentRecords._maxWorkload = maxWorkload;
}



 double peano::grid::tests::records::TestCellPacked::getMinWorkload() const  {
return _persistentRecords._minWorkload;
}



 void peano::grid::tests::records::TestCellPacked::setMinWorkload(const double& minWorkload)  {
_persistentRecords._minWorkload = minWorkload;
}



 bool peano::grid::tests::records::TestCellPacked::getSubtreeHoldsWorker() const  {
return _persistentRecords._subtreeHoldsWorker;
}



 void peano::grid::tests::records::TestCellPacked::setSubtreeHoldsWorker(const bool& subtreeHoldsWorker)  {
_persistentRecords._subtreeHoldsWorker = subtreeHoldsWorker;
}



 bool peano::grid::tests::records::TestCellPacked::getCellIsAForkCandidate() const  {
int mask = 1 << (DIMENSIONS + 3);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setCellIsAForkCandidate(const bool& cellIsAForkCandidate)  {
int mask = 1 << (DIMENSIONS + 3);
   _persistentRecords._packedRecords0 = static_cast<int>( cellIsAForkCandidate ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 int peano::grid::tests::records::TestCellPacked::getNumberOfLoadsFromInputStream() const  {
return _persistentRecords._numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCellPacked::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_persistentRecords._numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCellPacked::getNumberOfStoresToOutputStream() const  {
return _persistentRecords._numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCellPacked::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_persistentRecords._numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


std::string peano::grid::tests::records::TestCellPacked::toString(const State& param) {
return peano::grid::tests::records::TestCell::toString(param);
}

std::string peano::grid::tests::records::TestCellPacked::getStateMapping() {
return peano::grid::tests::records::TestCell::getStateMapping();
}



std::string peano::grid::tests::records::TestCellPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCellPacked::toString (std::ostream& out) const {
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
out << "nodeWorkload:" << getNodeWorkload();
out << ",";
out << "localWorkload:" << getLocalWorkload();
out << ",";
out << "totalWorkload:" << getTotalWorkload();
out << ",";
out << "maxWorkload:" << getMaxWorkload();
out << ",";
out << "minWorkload:" << getMinWorkload();
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


peano::grid::tests::records::TestCellPacked::PersistentRecords peano::grid::tests::records::TestCellPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCell peano::grid::tests::records::TestCellPacked::convert() const{
return TestCell(
getIsInside(),
getState(),
getEvenFlags(),
getAccessNumber(),
getResponsibleRank(),
getNodeWorkload(),
getLocalWorkload(),
getTotalWorkload(),
getMaxWorkload(),
getMinWorkload(),
getSubtreeHoldsWorker(),
getCellIsAForkCandidate(),
getNumberOfLoadsFromInputStream(),
getNumberOfStoresToOutputStream()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestCellPacked::_log( "peano::grid::tests::records::TestCellPacked" );

MPI_Datatype peano::grid::tests::records::TestCellPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCellPacked::FullDatatype = 0;


void peano::grid::tests::records::TestCellPacked::initDatatype() {
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 5;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._maxWorkload))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._minWorkload))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._maxWorkload))), 		&disp[4] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::Datatype );
MPI_Type_commit( &TestCellPacked::Datatype );

}
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 12;
MPI_Datatype subtypes[Attributes] = {
MPI_SHORT,		 //accessNumber
MPI_INT,		 //responsibleRank
MPI_DOUBLE,		 //nodeWorkload
MPI_DOUBLE,		 //localWorkload
MPI_DOUBLE,		 //totalWorkload
MPI_DOUBLE,		 //maxWorkload
MPI_DOUBLE,		 //minWorkload
MPI_CHAR,		 //subtreeHoldsWorker
MPI_INT,		 //numberOfLoadsFromInputStream
MPI_INT,		 //numberOfStoresToOutputStream
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //responsibleRank
1,		 //nodeWorkload
1,		 //localWorkload
1,		 //totalWorkload
1,		 //maxWorkload
1,		 //minWorkload
1,		 //subtreeHoldsWorker
1,		 //numberOfLoadsFromInputStream
1,		 //numberOfStoresToOutputStream
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._responsibleRank))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._nodeWorkload))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._localWorkload))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._totalWorkload))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._maxWorkload))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._minWorkload))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._subtreeHoldsWorker))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[8] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[9] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[10] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyTestCellPacked[1]._persistentRecords._accessNumber[0])), 		&disp[11] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::FullDatatype );
MPI_Type_commit( &TestCellPacked::FullDatatype );

}

}


void peano::grid::tests::records::TestCellPacked::shutdownDatatype() {
MPI_Type_free( &TestCellPacked::Datatype );
MPI_Type_free( &TestCellPacked::FullDatatype );

}

void peano::grid::tests::records::TestCellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCellPacked "
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
"peano::grid::tests::records::TestCellPacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



void peano::grid::tests::records::TestCellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCellPacked failed: "
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
"peano::grid::tests::records::TestCellPacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



bool peano::grid::tests::records::TestCellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCellPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif !defined(Parallel) && defined(SharedMemoryParallelisation) && defined(Debug)
peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestCell::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_isInside(isInside),
_state(state),
_level(level),
_evenFlags(evenFlags),
_accessNumber(accessNumber),
_numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
_numberOfStoresToOutputStream(numberOfStoresToOutputStream) {

}


 bool peano::grid::tests::records::TestCell::PersistentRecords::getIsInside() const  {
return _isInside;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setIsInside(const bool& isInside)  {
_isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::PersistentRecords::getState() const  {
return _state;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setState(const State& state)  {
_state = state;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setLevel(const int& level)  {
_level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::PersistentRecords::getEvenFlags() const  {
return _evenFlags;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_evenFlags = (evenFlags);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getNumberOfLoadsFromInputStream() const  {
return _numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCell::PersistentRecords::getNumberOfStoresToOutputStream() const  {
return _numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCell::PersistentRecords::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


peano::grid::tests::records::TestCell::TestCell() {

}


peano::grid::tests::records::TestCell::TestCell(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isInside, persistentRecords._state, persistentRecords._level, persistentRecords._evenFlags, persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {

}


peano::grid::tests::records::TestCell::TestCell(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_persistentRecords(isInside, state, level, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {

}


peano::grid::tests::records::TestCell::~TestCell() { }


 bool peano::grid::tests::records::TestCell::getIsInside() const  {
return _persistentRecords._isInside;
}



 void peano::grid::tests::records::TestCell::setIsInside(const bool& isInside)  {
_persistentRecords._isInside = isInside;
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCell::getState() const  {
return _persistentRecords._state;
}



 void peano::grid::tests::records::TestCell::setState(const State& state)  {
_persistentRecords._state = state;
}



 int peano::grid::tests::records::TestCell::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestCell::setLevel(const int& level)  {
_persistentRecords._level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCell::getEvenFlags() const  {
return _persistentRecords._evenFlags;
}



 void peano::grid::tests::records::TestCell::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
_persistentRecords._evenFlags = (evenFlags);
}



 bool peano::grid::tests::records::TestCell::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._evenFlags[elementIndex];

}



 void peano::grid::tests::records::TestCell::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags[elementIndex]= evenFlags;

}



 void peano::grid::tests::records::TestCell::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._evenFlags.flip(elementIndex);
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCell::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCell::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCell::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCell::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCell::getNumberOfLoadsFromInputStream() const  {
return _persistentRecords._numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCell::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_persistentRecords._numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCell::getNumberOfStoresToOutputStream() const  {
return _persistentRecords._numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCell::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_persistentRecords._numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


std::string peano::grid::tests::records::TestCell::toString(const State& param) {
switch (param) {
case Leaf: return "Leaf";
case Refined: return "Refined";
case Root: return "Root";
}
return "undefined";
}

std::string peano::grid::tests::records::TestCell::getStateMapping() {
return "State(Leaf=0,Refined=1,Root=2)";
}


std::string peano::grid::tests::records::TestCell::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCell::toString (std::ostream& out) const {
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


peano::grid::tests::records::TestCell::PersistentRecords peano::grid::tests::records::TestCell::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCellPacked peano::grid::tests::records::TestCell::convert() const{
return TestCellPacked(
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
tarch::logging::Log peano::grid::tests::records::TestCell::_log( "peano::grid::tests::records::TestCell" );

MPI_Datatype peano::grid::tests::records::TestCell::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCell::FullDatatype = 0;


void peano::grid::tests::records::TestCell::initDatatype() {
{
TestCell dummyTestCell[2];

const int Attributes = 3;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //state
MPI_INT,		 //level
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //state
1,		 //level
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._level))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._state))), 		&disp[2] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::Datatype );
MPI_Type_commit( &TestCell::Datatype );

}
{
TestCell dummyTestCell[2];

const int Attributes = 8;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isInside
MPI_INT,		 //state
MPI_INT,		 //level
MPI_INT,		 //evenFlags
MPI_SHORT,		 //accessNumber
MPI_INT,		 //numberOfLoadsFromInputStream
MPI_INT,		 //numberOfStoresToOutputStream
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isInside
1,		 //state
1,		 //level
DIMENSIONS,		 //evenFlags
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //numberOfLoadsFromInputStream
1,		 //numberOfStoresToOutputStream
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._isInside))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._state))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._level))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._evenFlags))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._accessNumber[0]))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCell[1]._persistentRecords._isInside))), 		&disp[7] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCell::FullDatatype );
MPI_Type_commit( &TestCell::FullDatatype );

}

}


void peano::grid::tests::records::TestCell::shutdownDatatype() {
MPI_Type_free( &TestCell::Datatype );
MPI_Type_free( &TestCell::FullDatatype );

}

void peano::grid::tests::records::TestCell::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "was not able to send message peano::grid::tests::records::TestCell "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCell "
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
"peano::grid::tests::records::TestCell",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



void peano::grid::tests::records::TestCell::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCell from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCell failed: "
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
"peano::grid::tests::records::TestCell",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCell",
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



bool peano::grid::tests::records::TestCell::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCell::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords() {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::PersistentRecords::PersistentRecords(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_level(level),
_accessNumber(accessNumber),
_numberOfLoadsFromInputStream(numberOfLoadsFromInputStream),
_numberOfStoresToOutputStream(numberOfStoresToOutputStream) {
setIsInside(isInside);
setState(state);
setEvenFlags(evenFlags);
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestCellPacked::PersistentRecords::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isInside ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::PersistentRecords::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(state) << (1));
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setLevel(const int& level)  {
_level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::PersistentRecords::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
_packedRecords0 = static_cast<int>(_packedRecords0 | evenFlags.to_ulong() << (3));
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::PersistentRecords::getAccessNumber() const  {
return _accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_accessNumber = (accessNumber);
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getNumberOfLoadsFromInputStream() const  {
return _numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCellPacked::PersistentRecords::getNumberOfStoresToOutputStream() const  {
return _numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCellPacked::PersistentRecords::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


peano::grid::tests::records::TestCellPacked::TestCellPacked() {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsInside(), persistentRecords.getState(), persistentRecords._level, persistentRecords.getEvenFlags(), persistentRecords._accessNumber, persistentRecords._numberOfLoadsFromInputStream, persistentRecords._numberOfStoresToOutputStream) {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::TestCellPacked(const bool& isInside, const State& state, const int& level, const std::bitset<DIMENSIONS>& evenFlags, const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const int& numberOfLoadsFromInputStream, const int& numberOfStoresToOutputStream):
_persistentRecords(isInside, state, level, evenFlags, accessNumber, numberOfLoadsFromInputStream, numberOfStoresToOutputStream) {
if ((DIMENSIONS+3 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((DIMENSIONS+3 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestCellPacked::~TestCellPacked() { }


 bool peano::grid::tests::records::TestCellPacked::getIsInside() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestCellPacked::setIsInside(const bool& isInside)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isInside ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestCell::State peano::grid::tests::records::TestCellPacked::getState() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (State) tmp;
}



 void peano::grid::tests::records::TestCellPacked::setState(const State& state)  {
assertion((state >= 0 && state <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(state) << (1));
}



 int peano::grid::tests::records::TestCellPacked::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestCellPacked::setLevel(const int& level)  {
_persistentRecords._level = level;
}



 std::bitset<DIMENSIONS> peano::grid::tests::records::TestCellPacked::getEvenFlags() const  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
tmp = static_cast<int>(tmp >> (3));
std::bitset<DIMENSIONS> result = tmp;
return result;
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(const std::bitset<DIMENSIONS>& evenFlags)  {
int mask = (int) (1 << (DIMENSIONS)) - 1 ;
mask = static_cast<int>(mask << (3));
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
_persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | evenFlags.to_ulong() << (3));
}



 bool peano::grid::tests::records::TestCellPacked::getEvenFlags(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
return (_persistentRecords._packedRecords0& mask);
}



 void peano::grid::tests::records::TestCellPacked::setEvenFlags(int elementIndex, const bool& evenFlags)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
assertion(!evenFlags || evenFlags==1);
int shift        = 3 + elementIndex; 
int mask         = 1     << (shift);
int shiftedValue = evenFlags << (shift);
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 & ~mask;
_persistentRecords._packedRecords0 = _persistentRecords._packedRecords0 |  shiftedValue;
}



 void peano::grid::tests::records::TestCellPacked::flipEvenFlags(int elementIndex)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
int mask = 1 << (3);
mask = mask << elementIndex;
_persistentRecords._packedRecords0^= mask;
}



 tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int> peano::grid::tests::records::TestCellPacked::getAccessNumber() const  {
return _persistentRecords._accessNumber;
}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber)  {
_persistentRecords._accessNumber = (accessNumber);
}



 short int peano::grid::tests::records::TestCellPacked::getAccessNumber(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
return _persistentRecords._accessNumber[elementIndex];

}



 void peano::grid::tests::records::TestCellPacked::setAccessNumber(int elementIndex, const short int& accessNumber)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS_TIMES_TWO);
_persistentRecords._accessNumber[elementIndex]= accessNumber;

}



 int peano::grid::tests::records::TestCellPacked::getNumberOfLoadsFromInputStream() const  {
return _persistentRecords._numberOfLoadsFromInputStream;
}



 void peano::grid::tests::records::TestCellPacked::setNumberOfLoadsFromInputStream(const int& numberOfLoadsFromInputStream)  {
_persistentRecords._numberOfLoadsFromInputStream = numberOfLoadsFromInputStream;
}



 int peano::grid::tests::records::TestCellPacked::getNumberOfStoresToOutputStream() const  {
return _persistentRecords._numberOfStoresToOutputStream;
}



 void peano::grid::tests::records::TestCellPacked::setNumberOfStoresToOutputStream(const int& numberOfStoresToOutputStream)  {
_persistentRecords._numberOfStoresToOutputStream = numberOfStoresToOutputStream;
}


std::string peano::grid::tests::records::TestCellPacked::toString(const State& param) {
return peano::grid::tests::records::TestCell::toString(param);
}

std::string peano::grid::tests::records::TestCellPacked::getStateMapping() {
return peano::grid::tests::records::TestCell::getStateMapping();
}



std::string peano::grid::tests::records::TestCellPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestCellPacked::toString (std::ostream& out) const {
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


peano::grid::tests::records::TestCellPacked::PersistentRecords peano::grid::tests::records::TestCellPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestCell peano::grid::tests::records::TestCellPacked::convert() const{
return TestCell(
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
tarch::logging::Log peano::grid::tests::records::TestCellPacked::_log( "peano::grid::tests::records::TestCellPacked" );

MPI_Datatype peano::grid::tests::records::TestCellPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestCellPacked::FullDatatype = 0;


void peano::grid::tests::records::TestCellPacked::initDatatype() {
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 3;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //level
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //level
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._level))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._level))), 		&disp[2] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::Datatype );
MPI_Type_commit( &TestCellPacked::Datatype );

}
{
TestCellPacked dummyTestCellPacked[2];

const int Attributes = 6;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //level
MPI_SHORT,		 //accessNumber
MPI_INT,		 //numberOfLoadsFromInputStream
MPI_INT,		 //numberOfStoresToOutputStream
MPI_INT,		 //_packedRecords0
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //level
DIMENSIONS_TIMES_TWO,		 //accessNumber
1,		 //numberOfLoadsFromInputStream
1,		 //numberOfStoresToOutputStream
1,		 //_packedRecords0
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._level))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._accessNumber[0]))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._numberOfLoadsFromInputStream))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._numberOfStoresToOutputStream))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestCellPacked[1]._persistentRecords._level))), 		&disp[5] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestCellPacked::FullDatatype );
MPI_Type_commit( &TestCellPacked::FullDatatype );

}

}


void peano::grid::tests::records::TestCellPacked::shutdownDatatype() {
MPI_Type_free( &TestCellPacked::Datatype );
MPI_Type_free( &TestCellPacked::FullDatatype );

}

void peano::grid::tests::records::TestCellPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "was not able to send message peano::grid::tests::records::TestCellPacked "
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
msg << "testing for finished send task for peano::grid::tests::records::TestCellPacked "
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
"peano::grid::tests::records::TestCellPacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



void peano::grid::tests::records::TestCellPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestCellPacked from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestCellPacked failed: "
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
"peano::grid::tests::records::TestCellPacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestCellPacked",
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



bool peano::grid::tests::records::TestCellPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestCellPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#endif


