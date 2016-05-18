#include "peano/heap/records/MetaInformation.h"

#if defined(Asserts)
   peano::heap::records::MetaInformation::PersistentRecords::PersistentRecords() {
      
   }
   
   
   peano::heap::records::MetaInformation::PersistentRecords::PersistentRecords(const int& length, const tarch::la::Vector<DIMENSIONS,double>& position, const int& level):
   _length(length),
   _position(position),
   _level(level) {
      
   }
   
   
    int peano::heap::records::MetaInformation::PersistentRecords::getLength() const  {
      return _length;
   }
   
   
   
    void peano::heap::records::MetaInformation::PersistentRecords::setLength(const int& length)  {
      _length = length;
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::heap::records::MetaInformation::PersistentRecords::getPosition() const  {
      return _position;
   }
   
   
   
    void peano::heap::records::MetaInformation::PersistentRecords::setPosition(const tarch::la::Vector<DIMENSIONS,double>& position)  {
      _position = (position);
   }
   
   
   
    int peano::heap::records::MetaInformation::PersistentRecords::getLevel() const  {
      return _level;
   }
   
   
   
    void peano::heap::records::MetaInformation::PersistentRecords::setLevel(const int& level)  {
      _level = level;
   }
   
   
   peano::heap::records::MetaInformation::MetaInformation() {
      
   }
   
   
   peano::heap::records::MetaInformation::MetaInformation(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._length, persistentRecords._position, persistentRecords._level) {
      
   }
   
   
   peano::heap::records::MetaInformation::MetaInformation(const int& length, const tarch::la::Vector<DIMENSIONS,double>& position, const int& level):
   _persistentRecords(length, position, level) {
      
   }
   
   
   peano::heap::records::MetaInformation::~MetaInformation() { }
   
   
    int peano::heap::records::MetaInformation::getLength() const  {
      return _persistentRecords._length;
   }
   
   
   
    void peano::heap::records::MetaInformation::setLength(const int& length)  {
      _persistentRecords._length = length;
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::heap::records::MetaInformation::getPosition() const  {
      return _persistentRecords._position;
   }
   
   
   
    void peano::heap::records::MetaInformation::setPosition(const tarch::la::Vector<DIMENSIONS,double>& position)  {
      _persistentRecords._position = (position);
   }
   
   
   
    double peano::heap::records::MetaInformation::getPosition(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      return _persistentRecords._position[elementIndex];
      
   }
   
   
   
    void peano::heap::records::MetaInformation::setPosition(int elementIndex, const double& position)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._position[elementIndex]= position;
      
   }
   
   
   
    int peano::heap::records::MetaInformation::getLevel() const  {
      return _persistentRecords._level;
   }
   
   
   
    void peano::heap::records::MetaInformation::setLevel(const int& level)  {
      _persistentRecords._level = level;
   }
   
   
   
   
   std::string peano::heap::records::MetaInformation::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void peano::heap::records::MetaInformation::toString (std::ostream& out) const {
      out << "("; 
      out << "length:" << getLength();
      out << ",";
      out << "position:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getPosition(i) << ",";
   }
   out << getPosition(DIMENSIONS-1) << "]";
      out << ",";
      out << "level:" << getLevel();
      out <<  ")";
   }
   
   
   peano::heap::records::MetaInformation::PersistentRecords peano::heap::records::MetaInformation::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   peano::heap::records::MetaInformationPacked peano::heap::records::MetaInformation::convert() const{
      return MetaInformationPacked(
         getLength(),
         getPosition(),
         getLevel()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log peano::heap::records::MetaInformation::_log( "peano::heap::records::MetaInformation" );
      
      MPI_Datatype peano::heap::records::MetaInformation::Datatype = 0;
      MPI_Datatype peano::heap::records::MetaInformation::FullDatatype = 0;
      
      
      void peano::heap::records::MetaInformation::initDatatype() {
         {
            MetaInformation dummyMetaInformation[2];
            
            const int Attributes = 4;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //length
               MPI_DOUBLE,		 //position
               MPI_INT,		 //level
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //length
               DIMENSIONS,		 //position
               1,		 //level
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]._persistentRecords._length))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]._persistentRecords._position[0]))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]._persistentRecords._level))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[1]._persistentRecords._length))), 		&disp[3] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &MetaInformation::Datatype );
            MPI_Type_commit( &MetaInformation::Datatype );
            
         }
         {
            MetaInformation dummyMetaInformation[2];
            
            const int Attributes = 4;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //length
               MPI_DOUBLE,		 //position
               MPI_INT,		 //level
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //length
               DIMENSIONS,		 //position
               1,		 //level
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]._persistentRecords._length))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]._persistentRecords._position[0]))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]._persistentRecords._level))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[1]._persistentRecords._length))), 		&disp[3] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &MetaInformation::FullDatatype );
            MPI_Type_commit( &MetaInformation::FullDatatype );
            
         }
         
      }
      
      
      void peano::heap::records::MetaInformation::shutdownDatatype() {
         MPI_Type_free( &MetaInformation::Datatype );
         MPI_Type_free( &MetaInformation::FullDatatype );
         
      }
      
      void peano::heap::records::MetaInformation::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message peano::heap::records::MetaInformation "
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
               msg << "was not able to send message peano::heap::records::MetaInformation "
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
                  msg << "testing for finished send task for peano::heap::records::MetaInformation "
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
                  "peano::heap::records::MetaInformation",
                  "send(int)", destination,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "peano::heap::records::MetaInformation",
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
      
      
      
      void peano::heap::records::MetaInformation::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            MPI_Status  status;
            const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
            if ( result != MPI_SUCCESS ) {
               std::ostringstream msg;
               msg << "failed to start to receive peano::heap::records::MetaInformation from node "
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
               msg << "failed to start to receive peano::heap::records::MetaInformation from node "
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
                  msg << "testing for finished receive task for peano::heap::records::MetaInformation failed: "
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
                  "peano::heap::records::MetaInformation",
                  "receive(int)", source,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "peano::heap::records::MetaInformation",
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
      
      
      
      bool peano::heap::records::MetaInformation::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   
   peano::heap::records::MetaInformationPacked::PersistentRecords::PersistentRecords() {
      
   }
   
   
   peano::heap::records::MetaInformationPacked::PersistentRecords::PersistentRecords(const int& length, const tarch::la::Vector<DIMENSIONS,double>& position, const int& level):
   _length(length),
   _position(position),
   _level(level) {
      
   }
   
   
    int peano::heap::records::MetaInformationPacked::PersistentRecords::getLength() const  {
      return _length;
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::PersistentRecords::setLength(const int& length)  {
      _length = length;
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::heap::records::MetaInformationPacked::PersistentRecords::getPosition() const  {
      return _position;
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::PersistentRecords::setPosition(const tarch::la::Vector<DIMENSIONS,double>& position)  {
      _position = (position);
   }
   
   
   
    int peano::heap::records::MetaInformationPacked::PersistentRecords::getLevel() const  {
      return _level;
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::PersistentRecords::setLevel(const int& level)  {
      _level = level;
   }
   
   
   peano::heap::records::MetaInformationPacked::MetaInformationPacked() {
      
   }
   
   
   peano::heap::records::MetaInformationPacked::MetaInformationPacked(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._length, persistentRecords._position, persistentRecords._level) {
      
   }
   
   
   peano::heap::records::MetaInformationPacked::MetaInformationPacked(const int& length, const tarch::la::Vector<DIMENSIONS,double>& position, const int& level):
   _persistentRecords(length, position, level) {
      
   }
   
   
   peano::heap::records::MetaInformationPacked::~MetaInformationPacked() { }
   
   
    int peano::heap::records::MetaInformationPacked::getLength() const  {
      return _persistentRecords._length;
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::setLength(const int& length)  {
      _persistentRecords._length = length;
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::heap::records::MetaInformationPacked::getPosition() const  {
      return _persistentRecords._position;
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::setPosition(const tarch::la::Vector<DIMENSIONS,double>& position)  {
      _persistentRecords._position = (position);
   }
   
   
   
    double peano::heap::records::MetaInformationPacked::getPosition(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      return _persistentRecords._position[elementIndex];
      
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::setPosition(int elementIndex, const double& position)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._position[elementIndex]= position;
      
   }
   
   
   
    int peano::heap::records::MetaInformationPacked::getLevel() const  {
      return _persistentRecords._level;
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::setLevel(const int& level)  {
      _persistentRecords._level = level;
   }
   
   
   
   
   std::string peano::heap::records::MetaInformationPacked::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void peano::heap::records::MetaInformationPacked::toString (std::ostream& out) const {
      out << "("; 
      out << "length:" << getLength();
      out << ",";
      out << "position:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getPosition(i) << ",";
   }
   out << getPosition(DIMENSIONS-1) << "]";
      out << ",";
      out << "level:" << getLevel();
      out <<  ")";
   }
   
   
   peano::heap::records::MetaInformationPacked::PersistentRecords peano::heap::records::MetaInformationPacked::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   peano::heap::records::MetaInformation peano::heap::records::MetaInformationPacked::convert() const{
      return MetaInformation(
         getLength(),
         getPosition(),
         getLevel()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log peano::heap::records::MetaInformationPacked::_log( "peano::heap::records::MetaInformationPacked" );
      
      MPI_Datatype peano::heap::records::MetaInformationPacked::Datatype = 0;
      MPI_Datatype peano::heap::records::MetaInformationPacked::FullDatatype = 0;
      
      
      void peano::heap::records::MetaInformationPacked::initDatatype() {
         {
            MetaInformationPacked dummyMetaInformationPacked[2];
            
            const int Attributes = 4;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //length
               MPI_DOUBLE,		 //position
               MPI_INT,		 //level
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //length
               DIMENSIONS,		 //position
               1,		 //level
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]._persistentRecords._length))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]._persistentRecords._position[0]))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]._persistentRecords._level))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[1]._persistentRecords._length))), 		&disp[3] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &MetaInformationPacked::Datatype );
            MPI_Type_commit( &MetaInformationPacked::Datatype );
            
         }
         {
            MetaInformationPacked dummyMetaInformationPacked[2];
            
            const int Attributes = 4;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //length
               MPI_DOUBLE,		 //position
               MPI_INT,		 //level
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //length
               DIMENSIONS,		 //position
               1,		 //level
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]._persistentRecords._length))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]._persistentRecords._position[0]))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]._persistentRecords._level))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[1]._persistentRecords._length))), 		&disp[3] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &MetaInformationPacked::FullDatatype );
            MPI_Type_commit( &MetaInformationPacked::FullDatatype );
            
         }
         
      }
      
      
      void peano::heap::records::MetaInformationPacked::shutdownDatatype() {
         MPI_Type_free( &MetaInformationPacked::Datatype );
         MPI_Type_free( &MetaInformationPacked::FullDatatype );
         
      }
      
      void peano::heap::records::MetaInformationPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message peano::heap::records::MetaInformationPacked "
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
               msg << "was not able to send message peano::heap::records::MetaInformationPacked "
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
                  msg << "testing for finished send task for peano::heap::records::MetaInformationPacked "
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
                  "peano::heap::records::MetaInformationPacked",
                  "send(int)", destination,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "peano::heap::records::MetaInformationPacked",
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
      
      
      
      void peano::heap::records::MetaInformationPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            MPI_Status  status;
            const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
            if ( result != MPI_SUCCESS ) {
               std::ostringstream msg;
               msg << "failed to start to receive peano::heap::records::MetaInformationPacked from node "
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
               msg << "failed to start to receive peano::heap::records::MetaInformationPacked from node "
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
                  msg << "testing for finished receive task for peano::heap::records::MetaInformationPacked failed: "
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
                  "peano::heap::records::MetaInformationPacked",
                  "receive(int)", source,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "peano::heap::records::MetaInformationPacked",
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
      
      
      
      bool peano::heap::records::MetaInformationPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   
   
#elif !defined(Asserts)
   peano::heap::records::MetaInformation::PersistentRecords::PersistentRecords() {
      
   }
   
   
   peano::heap::records::MetaInformation::PersistentRecords::PersistentRecords(const int& length):
   _length(length) {
      
   }
   
   
    int peano::heap::records::MetaInformation::PersistentRecords::getLength() const  {
      return _length;
   }
   
   
   
    void peano::heap::records::MetaInformation::PersistentRecords::setLength(const int& length)  {
      _length = length;
   }
   
   
   peano::heap::records::MetaInformation::MetaInformation() {
      
   }
   
   
   peano::heap::records::MetaInformation::MetaInformation(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._length) {
      
   }
   
   
   peano::heap::records::MetaInformation::MetaInformation(const int& length):
   _persistentRecords(length) {
      
   }
   
   
   peano::heap::records::MetaInformation::~MetaInformation() { }
   
   
    int peano::heap::records::MetaInformation::getLength() const  {
      return _persistentRecords._length;
   }
   
   
   
    void peano::heap::records::MetaInformation::setLength(const int& length)  {
      _persistentRecords._length = length;
   }
   
   
   
   
   std::string peano::heap::records::MetaInformation::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void peano::heap::records::MetaInformation::toString (std::ostream& out) const {
      out << "("; 
      out << "length:" << getLength();
      out <<  ")";
   }
   
   
   peano::heap::records::MetaInformation::PersistentRecords peano::heap::records::MetaInformation::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   peano::heap::records::MetaInformationPacked peano::heap::records::MetaInformation::convert() const{
      return MetaInformationPacked(
         getLength()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log peano::heap::records::MetaInformation::_log( "peano::heap::records::MetaInformation" );
      
      MPI_Datatype peano::heap::records::MetaInformation::Datatype = 0;
      MPI_Datatype peano::heap::records::MetaInformation::FullDatatype = 0;
      
      
      void peano::heap::records::MetaInformation::initDatatype() {
         {
            MetaInformation dummyMetaInformation[2];
            
            const int Attributes = 2;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //length
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //length
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]._persistentRecords._length))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[1]._persistentRecords._length))), 		&disp[1] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &MetaInformation::Datatype );
            MPI_Type_commit( &MetaInformation::Datatype );
            
         }
         {
            MetaInformation dummyMetaInformation[2];
            
            const int Attributes = 2;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //length
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //length
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[0]._persistentRecords._length))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformation[1]._persistentRecords._length))), 		&disp[1] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &MetaInformation::FullDatatype );
            MPI_Type_commit( &MetaInformation::FullDatatype );
            
         }
         
      }
      
      
      void peano::heap::records::MetaInformation::shutdownDatatype() {
         MPI_Type_free( &MetaInformation::Datatype );
         MPI_Type_free( &MetaInformation::FullDatatype );
         
      }
      
      void peano::heap::records::MetaInformation::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message peano::heap::records::MetaInformation "
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
               msg << "was not able to send message peano::heap::records::MetaInformation "
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
                  msg << "testing for finished send task for peano::heap::records::MetaInformation "
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
                  "peano::heap::records::MetaInformation",
                  "send(int)", destination,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "peano::heap::records::MetaInformation",
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
      
      
      
      void peano::heap::records::MetaInformation::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            MPI_Status  status;
            const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
            if ( result != MPI_SUCCESS ) {
               std::ostringstream msg;
               msg << "failed to start to receive peano::heap::records::MetaInformation from node "
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
               msg << "failed to start to receive peano::heap::records::MetaInformation from node "
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
                  msg << "testing for finished receive task for peano::heap::records::MetaInformation failed: "
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
                  "peano::heap::records::MetaInformation",
                  "receive(int)", source,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "peano::heap::records::MetaInformation",
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
      
      
      
      bool peano::heap::records::MetaInformation::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   
   peano::heap::records::MetaInformationPacked::PersistentRecords::PersistentRecords() {
      
   }
   
   
   peano::heap::records::MetaInformationPacked::PersistentRecords::PersistentRecords(const int& length):
   _length(length) {
      
   }
   
   
    int peano::heap::records::MetaInformationPacked::PersistentRecords::getLength() const  {
      return _length;
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::PersistentRecords::setLength(const int& length)  {
      _length = length;
   }
   
   
   peano::heap::records::MetaInformationPacked::MetaInformationPacked() {
      
   }
   
   
   peano::heap::records::MetaInformationPacked::MetaInformationPacked(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._length) {
      
   }
   
   
   peano::heap::records::MetaInformationPacked::MetaInformationPacked(const int& length):
   _persistentRecords(length) {
      
   }
   
   
   peano::heap::records::MetaInformationPacked::~MetaInformationPacked() { }
   
   
    int peano::heap::records::MetaInformationPacked::getLength() const  {
      return _persistentRecords._length;
   }
   
   
   
    void peano::heap::records::MetaInformationPacked::setLength(const int& length)  {
      _persistentRecords._length = length;
   }
   
   
   
   
   std::string peano::heap::records::MetaInformationPacked::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void peano::heap::records::MetaInformationPacked::toString (std::ostream& out) const {
      out << "("; 
      out << "length:" << getLength();
      out <<  ")";
   }
   
   
   peano::heap::records::MetaInformationPacked::PersistentRecords peano::heap::records::MetaInformationPacked::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   peano::heap::records::MetaInformation peano::heap::records::MetaInformationPacked::convert() const{
      return MetaInformation(
         getLength()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log peano::heap::records::MetaInformationPacked::_log( "peano::heap::records::MetaInformationPacked" );
      
      MPI_Datatype peano::heap::records::MetaInformationPacked::Datatype = 0;
      MPI_Datatype peano::heap::records::MetaInformationPacked::FullDatatype = 0;
      
      
      void peano::heap::records::MetaInformationPacked::initDatatype() {
         {
            MetaInformationPacked dummyMetaInformationPacked[2];
            
            const int Attributes = 2;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //length
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //length
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]._persistentRecords._length))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[1]._persistentRecords._length))), 		&disp[1] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &MetaInformationPacked::Datatype );
            MPI_Type_commit( &MetaInformationPacked::Datatype );
            
         }
         {
            MetaInformationPacked dummyMetaInformationPacked[2];
            
            const int Attributes = 2;
            MPI_Datatype subtypes[Attributes] = {
               MPI_INT,		 //length
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //length
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[0]._persistentRecords._length))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyMetaInformationPacked[1]._persistentRecords._length))), 		&disp[1] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &MetaInformationPacked::FullDatatype );
            MPI_Type_commit( &MetaInformationPacked::FullDatatype );
            
         }
         
      }
      
      
      void peano::heap::records::MetaInformationPacked::shutdownDatatype() {
         MPI_Type_free( &MetaInformationPacked::Datatype );
         MPI_Type_free( &MetaInformationPacked::FullDatatype );
         
      }
      
      void peano::heap::records::MetaInformationPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message peano::heap::records::MetaInformationPacked "
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
               msg << "was not able to send message peano::heap::records::MetaInformationPacked "
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
                  msg << "testing for finished send task for peano::heap::records::MetaInformationPacked "
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
                  "peano::heap::records::MetaInformationPacked",
                  "send(int)", destination,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "peano::heap::records::MetaInformationPacked",
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
      
      
      
      void peano::heap::records::MetaInformationPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            MPI_Status  status;
            const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
            if ( result != MPI_SUCCESS ) {
               std::ostringstream msg;
               msg << "failed to start to receive peano::heap::records::MetaInformationPacked from node "
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
               msg << "failed to start to receive peano::heap::records::MetaInformationPacked from node "
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
                  msg << "testing for finished receive task for peano::heap::records::MetaInformationPacked failed: "
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
                  "peano::heap::records::MetaInformationPacked",
                  "receive(int)", source,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "peano::heap::records::MetaInformationPacked",
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
      
      
      
      bool peano::heap::records::MetaInformationPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   
   

#endif


