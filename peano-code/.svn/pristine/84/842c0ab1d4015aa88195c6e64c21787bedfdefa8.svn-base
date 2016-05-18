#include "peano/grid/tests/records/TestVertex.h"

#if defined(Parallel) && defined(Asserts)
   peano::grid::tests::records::TestVertex::PersistentRecords::PersistentRecords() {
      
   }
   
   
   peano::grid::tests::records::TestVertex::PersistentRecords::PersistentRecords(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
   _isHangingNode(isHangingNode),
   _refinementControl(refinementControl),
   _adjacentCellsHeight(adjacentCellsHeight),
   _insideOutsideDomain(insideOutsideDomain),
   _x(x),
   _level(level),
   _adjacentRanks(adjacentRanks),
   _adjacentSubtreeForksIntoOtherRank(adjacentSubtreeForksIntoOtherRank) {
      
   }
   
   
    bool peano::grid::tests::records::TestVertex::PersistentRecords::getIsHangingNode() const  {
      return _isHangingNode;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::PersistentRecords::setIsHangingNode(const bool& isHangingNode)  {
      _isHangingNode = isHangingNode;
   }
   
   
   
    peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertex::PersistentRecords::getRefinementControl() const  {
      return _refinementControl;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::PersistentRecords::setRefinementControl(const RefinementControl& refinementControl)  {
      _refinementControl = refinementControl;
   }
   
   
   
    int peano::grid::tests::records::TestVertex::PersistentRecords::getAdjacentCellsHeight() const  {
      return _adjacentCellsHeight;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::PersistentRecords::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
      _adjacentCellsHeight = adjacentCellsHeight;
   }
   
   
   
    peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertex::PersistentRecords::getInsideOutsideDomain() const  {
      return _insideOutsideDomain;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::PersistentRecords::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
      _insideOutsideDomain = insideOutsideDomain;
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestVertex::PersistentRecords::getX() const  {
      return _x;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::PersistentRecords::setX(const tarch::la::Vector<DIMENSIONS,double>& x)  {
      _x = (x);
   }
   
   
   
    int peano::grid::tests::records::TestVertex::PersistentRecords::getLevel() const  {
      return _level;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::PersistentRecords::setLevel(const int& level)  {
      _level = level;
   }
   
   
   
    tarch::la::Vector<TWO_POWER_D,int> peano::grid::tests::records::TestVertex::PersistentRecords::getAdjacentRanks() const  {
      return _adjacentRanks;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::PersistentRecords::setAdjacentRanks(const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks)  {
      _adjacentRanks = (adjacentRanks);
   }
   
   
   
    bool peano::grid::tests::records::TestVertex::PersistentRecords::getAdjacentSubtreeForksIntoOtherRank() const  {
      return _adjacentSubtreeForksIntoOtherRank;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::PersistentRecords::setAdjacentSubtreeForksIntoOtherRank(const bool& adjacentSubtreeForksIntoOtherRank)  {
      _adjacentSubtreeForksIntoOtherRank = adjacentSubtreeForksIntoOtherRank;
   }
   
   
   peano::grid::tests::records::TestVertex::TestVertex() {
      
   }
   
   
   peano::grid::tests::records::TestVertex::TestVertex(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._isHangingNode, persistentRecords._refinementControl, persistentRecords._adjacentCellsHeight, persistentRecords._insideOutsideDomain, persistentRecords._x, persistentRecords._level, persistentRecords._adjacentRanks, persistentRecords._adjacentSubtreeForksIntoOtherRank) {
      
   }
   
   
   peano::grid::tests::records::TestVertex::TestVertex(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
   _persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, x, level, adjacentRanks, adjacentSubtreeForksIntoOtherRank) {
      
   }
   
   
   peano::grid::tests::records::TestVertex::TestVertex(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const int& adjacentCellsHeightOfPreviousIteration, const int& numberOfAdjacentRefinedCells, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
   _persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, x, level, adjacentRanks, adjacentSubtreeForksIntoOtherRank),_adjacentCellsHeightOfPreviousIteration(adjacentCellsHeightOfPreviousIteration),
   _numberOfAdjacentRefinedCells(numberOfAdjacentRefinedCells) {
      
   }
   
   peano::grid::tests::records::TestVertex::~TestVertex() { }
   
   
    bool peano::grid::tests::records::TestVertex::getIsHangingNode() const  {
      return _persistentRecords._isHangingNode;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setIsHangingNode(const bool& isHangingNode)  {
      _persistentRecords._isHangingNode = isHangingNode;
   }
   
   
   
    peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertex::getRefinementControl() const  {
      return _persistentRecords._refinementControl;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setRefinementControl(const RefinementControl& refinementControl)  {
      _persistentRecords._refinementControl = refinementControl;
   }
   
   
   
    int peano::grid::tests::records::TestVertex::getAdjacentCellsHeight() const  {
      return _persistentRecords._adjacentCellsHeight;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
      _persistentRecords._adjacentCellsHeight = adjacentCellsHeight;
   }
   
   
   
    int peano::grid::tests::records::TestVertex::getAdjacentCellsHeightOfPreviousIteration() const  {
      return _adjacentCellsHeightOfPreviousIteration;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setAdjacentCellsHeightOfPreviousIteration(const int& adjacentCellsHeightOfPreviousIteration)  {
      _adjacentCellsHeightOfPreviousIteration = adjacentCellsHeightOfPreviousIteration;
   }
   
   
   
    int peano::grid::tests::records::TestVertex::getNumberOfAdjacentRefinedCells() const  {
      return _numberOfAdjacentRefinedCells;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setNumberOfAdjacentRefinedCells(const int& numberOfAdjacentRefinedCells)  {
      _numberOfAdjacentRefinedCells = numberOfAdjacentRefinedCells;
   }
   
   
   
    peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertex::getInsideOutsideDomain() const  {
      return _persistentRecords._insideOutsideDomain;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
      _persistentRecords._insideOutsideDomain = insideOutsideDomain;
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestVertex::getX() const  {
      return _persistentRecords._x;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setX(const tarch::la::Vector<DIMENSIONS,double>& x)  {
      _persistentRecords._x = (x);
   }
   
   
   
    double peano::grid::tests::records::TestVertex::getX(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      return _persistentRecords._x[elementIndex];
      
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setX(int elementIndex, const double& x)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._x[elementIndex]= x;
      
   }
   
   
   
    int peano::grid::tests::records::TestVertex::getLevel() const  {
      return _persistentRecords._level;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setLevel(const int& level)  {
      _persistentRecords._level = level;
   }
   
   
   
    tarch::la::Vector<TWO_POWER_D,int> peano::grid::tests::records::TestVertex::getAdjacentRanks() const  {
      return _persistentRecords._adjacentRanks;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setAdjacentRanks(const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks)  {
      _persistentRecords._adjacentRanks = (adjacentRanks);
   }
   
   
   
    int peano::grid::tests::records::TestVertex::getAdjacentRanks(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<TWO_POWER_D);
      return _persistentRecords._adjacentRanks[elementIndex];
      
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setAdjacentRanks(int elementIndex, const int& adjacentRanks)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<TWO_POWER_D);
      _persistentRecords._adjacentRanks[elementIndex]= adjacentRanks;
      
   }
   
   
   
    bool peano::grid::tests::records::TestVertex::getAdjacentSubtreeForksIntoOtherRank() const  {
      return _persistentRecords._adjacentSubtreeForksIntoOtherRank;
   }
   
   
   
    void peano::grid::tests::records::TestVertex::setAdjacentSubtreeForksIntoOtherRank(const bool& adjacentSubtreeForksIntoOtherRank)  {
      _persistentRecords._adjacentSubtreeForksIntoOtherRank = adjacentSubtreeForksIntoOtherRank;
   }
   
   
   std::string peano::grid::tests::records::TestVertex::toString(const InsideOutsideDomain& param) {
      switch (param) {
         case Inside: return "Inside";
         case Boundary: return "Boundary";
         case Outside: return "Outside";
      }
      return "undefined";
   }
   
   std::string peano::grid::tests::records::TestVertex::getInsideOutsideDomainMapping() {
      return "InsideOutsideDomain(Inside=0,Boundary=1,Outside=2)";
   }
   std::string peano::grid::tests::records::TestVertex::toString(const RefinementControl& param) {
      switch (param) {
         case Unrefined: return "Unrefined";
         case Refined: return "Refined";
         case RefinementTriggered: return "RefinementTriggered";
         case Refining: return "Refining";
         case EraseTriggered: return "EraseTriggered";
         case Erasing: return "Erasing";
         case RefineDueToJoinThoughWorkerIsAlreadyErasing: return "RefineDueToJoinThoughWorkerIsAlreadyErasing";
         case EnforceRefinementTriggered: return "EnforceRefinementTriggered";
      }
      return "undefined";
   }
   
   std::string peano::grid::tests::records::TestVertex::getRefinementControlMapping() {
      return "RefinementControl(Unrefined=0,Refined=1,RefinementTriggered=2,Refining=3,EraseTriggered=4,Erasing=5,RefineDueToJoinThoughWorkerIsAlreadyErasing=6,EnforceRefinementTriggered=7)";
   }
   
   
   std::string peano::grid::tests::records::TestVertex::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void peano::grid::tests::records::TestVertex::toString (std::ostream& out) const {
      out << "("; 
      out << "isHangingNode:" << getIsHangingNode();
      out << ",";
      out << "refinementControl:" << toString(getRefinementControl());
      out << ",";
      out << "adjacentCellsHeight:" << getAdjacentCellsHeight();
      out << ",";
      out << "adjacentCellsHeightOfPreviousIteration:" << getAdjacentCellsHeightOfPreviousIteration();
      out << ",";
      out << "numberOfAdjacentRefinedCells:" << getNumberOfAdjacentRefinedCells();
      out << ",";
      out << "insideOutsideDomain:" << toString(getInsideOutsideDomain());
      out << ",";
      out << "x:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getX(i) << ",";
   }
   out << getX(DIMENSIONS-1) << "]";
      out << ",";
      out << "level:" << getLevel();
      out << ",";
      out << "adjacentRanks:[";
   for (int i = 0; i < TWO_POWER_D-1; i++) {
      out << getAdjacentRanks(i) << ",";
   }
   out << getAdjacentRanks(TWO_POWER_D-1) << "]";
      out << ",";
      out << "adjacentSubtreeForksIntoOtherRank:" << getAdjacentSubtreeForksIntoOtherRank();
      out <<  ")";
   }
   
   
   peano::grid::tests::records::TestVertex::PersistentRecords peano::grid::tests::records::TestVertex::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   peano::grid::tests::records::TestVertexPacked peano::grid::tests::records::TestVertex::convert() const{
      return TestVertexPacked(
         getIsHangingNode(),
         getRefinementControl(),
         getAdjacentCellsHeight(),
         getAdjacentCellsHeightOfPreviousIteration(),
         getNumberOfAdjacentRefinedCells(),
         getInsideOutsideDomain(),
         getX(),
         getLevel(),
         getAdjacentRanks(),
         getAdjacentSubtreeForksIntoOtherRank()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log peano::grid::tests::records::TestVertex::_log( "peano::grid::tests::records::TestVertex" );
      
      MPI_Datatype peano::grid::tests::records::TestVertex::Datatype = 0;
      MPI_Datatype peano::grid::tests::records::TestVertex::FullDatatype = 0;
      
      
      void peano::grid::tests::records::TestVertex::initDatatype() {
         {
            TestVertex dummyTestVertex[2];
            
            const int Attributes = 9;
            MPI_Datatype subtypes[Attributes] = {
               MPI_CHAR,		 //isHangingNode
               MPI_INT,		 //refinementControl
               MPI_INT,		 //insideOutsideDomain
               MPI_DOUBLE,		 //x
               MPI_INT,		 //level
               MPI_INT,		 //adjacentRanks
               MPI_CHAR,		 //adjacentSubtreeForksIntoOtherRank
               MPI_INT,		 //numberOfAdjacentRefinedCells
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //isHangingNode
               1,		 //refinementControl
               1,		 //insideOutsideDomain
               DIMENSIONS,		 //x
               1,		 //level
               TWO_POWER_D,		 //adjacentRanks
               1,		 //adjacentSubtreeForksIntoOtherRank
               1,		 //numberOfAdjacentRefinedCells
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._isHangingNode))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._refinementControl))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._insideOutsideDomain))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._x[0]))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._level))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentRanks[0]))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentSubtreeForksIntoOtherRank))), 		&disp[6] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._numberOfAdjacentRefinedCells))), 		&disp[7] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[1]._persistentRecords._isHangingNode))), 		&disp[8] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertex::Datatype );
            MPI_Type_commit( &TestVertex::Datatype );
            
         }
         {
            TestVertex dummyTestVertex[2];
            
            const int Attributes = 11;
            MPI_Datatype subtypes[Attributes] = {
               MPI_CHAR,		 //isHangingNode
               MPI_INT,		 //refinementControl
               MPI_INT,		 //adjacentCellsHeight
               MPI_INT,		 //insideOutsideDomain
               MPI_DOUBLE,		 //x
               MPI_INT,		 //level
               MPI_INT,		 //adjacentRanks
               MPI_CHAR,		 //adjacentSubtreeForksIntoOtherRank
               MPI_INT,		 //adjacentCellsHeightOfPreviousIteration
               MPI_INT,		 //numberOfAdjacentRefinedCells
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1,		 //isHangingNode
               1,		 //refinementControl
               1,		 //adjacentCellsHeight
               1,		 //insideOutsideDomain
               DIMENSIONS,		 //x
               1,		 //level
               TWO_POWER_D,		 //adjacentRanks
               1,		 //adjacentSubtreeForksIntoOtherRank
               1,		 //adjacentCellsHeightOfPreviousIteration
               1,		 //numberOfAdjacentRefinedCells
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._isHangingNode))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._refinementControl))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentCellsHeight))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._insideOutsideDomain))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._x[0]))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._level))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentRanks[0]))), 		&disp[6] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentSubtreeForksIntoOtherRank))), 		&disp[7] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._adjacentCellsHeightOfPreviousIteration))), 		&disp[8] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._numberOfAdjacentRefinedCells))), 		&disp[9] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[1]._persistentRecords._isHangingNode))), 		&disp[10] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertex::FullDatatype );
            MPI_Type_commit( &TestVertex::FullDatatype );
            
         }
         
      }
      
      
      void peano::grid::tests::records::TestVertex::shutdownDatatype() {
         MPI_Type_free( &TestVertex::Datatype );
         MPI_Type_free( &TestVertex::FullDatatype );
         
      }
      
      void peano::grid::tests::records::TestVertex::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         _senderDestinationRank = destination;
         
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message peano::grid::tests::records::TestVertex "
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
            msg << "was not able to send message peano::grid::tests::records::TestVertex "
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
               msg << "testing for finished send task for peano::grid::tests::records::TestVertex "
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
               "peano::grid::tests::records::TestVertex",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::grid::tests::records::TestVertex",
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
   
   
   
   void peano::grid::tests::records::TestVertex::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         _senderDestinationRank = status.MPI_SOURCE;
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive peano::grid::tests::records::TestVertex from node "
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
            msg << "failed to start to receive peano::grid::tests::records::TestVertex from node "
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
               msg << "testing for finished receive task for peano::grid::tests::records::TestVertex failed: "
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
               "peano::grid::tests::records::TestVertex",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "peano::grid::tests::records::TestVertex",
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
   
   
   
   bool peano::grid::tests::records::TestVertex::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
   
   int peano::grid::tests::records::TestVertex::getSenderRank() const {
      assertion( _senderDestinationRank!=-1 );
      return _senderDestinationRank;
      
   }
#endif


peano::grid::tests::records::TestVertexPacked::PersistentRecords::PersistentRecords() {
   if ((6 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(int))));
   
}


peano::grid::tests::records::TestVertexPacked::PersistentRecords::PersistentRecords(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_adjacentCellsHeight(adjacentCellsHeight),
_x(x),
_level(level),
_adjacentRanks(adjacentRanks),
_adjacentSubtreeForksIntoOtherRank(adjacentSubtreeForksIntoOtherRank) {
   setIsHangingNode(isHangingNode);
   setRefinementControl(refinementControl);
   setInsideOutsideDomain(insideOutsideDomain);
   if ((6 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(int))));
   
}


 bool peano::grid::tests::records::TestVertexPacked::PersistentRecords::getIsHangingNode() const  {
   int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setIsHangingNode(const bool& isHangingNode)  {
   int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isHangingNode ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertexPacked::PersistentRecords::getRefinementControl() const  {
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 7));
   return (RefinementControl) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setRefinementControl(const RefinementControl& refinementControl)  {
   assertion((refinementControl >= 0 && refinementControl <= 7));
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(refinementControl) << (1));
}



 int peano::grid::tests::records::TestVertexPacked::PersistentRecords::getAdjacentCellsHeight() const  {
   return _adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
   _adjacentCellsHeight = adjacentCellsHeight;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertexPacked::PersistentRecords::getInsideOutsideDomain() const  {
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (4));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (InsideOutsideDomain) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
   assertion((insideOutsideDomain >= 0 && insideOutsideDomain <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(insideOutsideDomain) << (4));
}



 tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestVertexPacked::PersistentRecords::getX() const  {
   return _x;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setX(const tarch::la::Vector<DIMENSIONS,double>& x)  {
   _x = (x);
}



 int peano::grid::tests::records::TestVertexPacked::PersistentRecords::getLevel() const  {
   return _level;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setLevel(const int& level)  {
   _level = level;
}



 tarch::la::Vector<TWO_POWER_D,int> peano::grid::tests::records::TestVertexPacked::PersistentRecords::getAdjacentRanks() const  {
   return _adjacentRanks;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setAdjacentRanks(const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks)  {
   _adjacentRanks = (adjacentRanks);
}



 bool peano::grid::tests::records::TestVertexPacked::PersistentRecords::getAdjacentSubtreeForksIntoOtherRank() const  {
   return _adjacentSubtreeForksIntoOtherRank;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setAdjacentSubtreeForksIntoOtherRank(const bool& adjacentSubtreeForksIntoOtherRank)  {
   _adjacentSubtreeForksIntoOtherRank = adjacentSubtreeForksIntoOtherRank;
}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked() {
   if ((6 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(int))));
   
}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsHangingNode(), persistentRecords.getRefinementControl(), persistentRecords._adjacentCellsHeight, persistentRecords.getInsideOutsideDomain(), persistentRecords._x, persistentRecords._level, persistentRecords._adjacentRanks, persistentRecords._adjacentSubtreeForksIntoOtherRank) {
   if ((6 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(int))));
   
}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, x, level, adjacentRanks, adjacentSubtreeForksIntoOtherRank) {
   if ((6 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(int))));
   
}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const int& adjacentCellsHeightOfPreviousIteration, const int& numberOfAdjacentRefinedCells, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, x, level, adjacentRanks, adjacentSubtreeForksIntoOtherRank),_adjacentCellsHeightOfPreviousIteration(adjacentCellsHeightOfPreviousIteration),
_numberOfAdjacentRefinedCells(numberOfAdjacentRefinedCells) {
   if ((6 >= (8 * sizeof(int)))) {
      std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
      std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
      std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
   }
   assertion((6 < (8 * sizeof(int))));
   
}

peano::grid::tests::records::TestVertexPacked::~TestVertexPacked() { }


 bool peano::grid::tests::records::TestVertexPacked::getIsHangingNode() const  {
   int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestVertexPacked::setIsHangingNode(const bool& isHangingNode)  {
   int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isHangingNode ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertexPacked::getRefinementControl() const  {
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 7));
   return (RefinementControl) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::setRefinementControl(const RefinementControl& refinementControl)  {
   assertion((refinementControl >= 0 && refinementControl <= 7));
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(refinementControl) << (1));
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentCellsHeight() const  {
   return _persistentRecords._adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
   _persistentRecords._adjacentCellsHeight = adjacentCellsHeight;
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentCellsHeightOfPreviousIteration() const  {
   return _adjacentCellsHeightOfPreviousIteration;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentCellsHeightOfPreviousIteration(const int& adjacentCellsHeightOfPreviousIteration)  {
   _adjacentCellsHeightOfPreviousIteration = adjacentCellsHeightOfPreviousIteration;
}



 int peano::grid::tests::records::TestVertexPacked::getNumberOfAdjacentRefinedCells() const  {
   return _numberOfAdjacentRefinedCells;
}



 void peano::grid::tests::records::TestVertexPacked::setNumberOfAdjacentRefinedCells(const int& numberOfAdjacentRefinedCells)  {
   _numberOfAdjacentRefinedCells = numberOfAdjacentRefinedCells;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertexPacked::getInsideOutsideDomain() const  {
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (4));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (InsideOutsideDomain) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
   assertion((insideOutsideDomain >= 0 && insideOutsideDomain <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(insideOutsideDomain) << (4));
}



 tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestVertexPacked::getX() const  {
   return _persistentRecords._x;
}



 void peano::grid::tests::records::TestVertexPacked::setX(const tarch::la::Vector<DIMENSIONS,double>& x)  {
   _persistentRecords._x = (x);
}



 double peano::grid::tests::records::TestVertexPacked::getX(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   return _persistentRecords._x[elementIndex];
   
}



 void peano::grid::tests::records::TestVertexPacked::setX(int elementIndex, const double& x)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<DIMENSIONS);
   _persistentRecords._x[elementIndex]= x;
   
}



 int peano::grid::tests::records::TestVertexPacked::getLevel() const  {
   return _persistentRecords._level;
}



 void peano::grid::tests::records::TestVertexPacked::setLevel(const int& level)  {
   _persistentRecords._level = level;
}



 tarch::la::Vector<TWO_POWER_D,int> peano::grid::tests::records::TestVertexPacked::getAdjacentRanks() const  {
   return _persistentRecords._adjacentRanks;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentRanks(const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks)  {
   _persistentRecords._adjacentRanks = (adjacentRanks);
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentRanks(int elementIndex) const  {
   assertion(elementIndex>=0);
   assertion(elementIndex<TWO_POWER_D);
   return _persistentRecords._adjacentRanks[elementIndex];
   
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentRanks(int elementIndex, const int& adjacentRanks)  {
   assertion(elementIndex>=0);
   assertion(elementIndex<TWO_POWER_D);
   _persistentRecords._adjacentRanks[elementIndex]= adjacentRanks;
   
}



 bool peano::grid::tests::records::TestVertexPacked::getAdjacentSubtreeForksIntoOtherRank() const  {
   return _persistentRecords._adjacentSubtreeForksIntoOtherRank;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentSubtreeForksIntoOtherRank(const bool& adjacentSubtreeForksIntoOtherRank)  {
   _persistentRecords._adjacentSubtreeForksIntoOtherRank = adjacentSubtreeForksIntoOtherRank;
}


std::string peano::grid::tests::records::TestVertexPacked::toString(const InsideOutsideDomain& param) {
   return peano::grid::tests::records::TestVertex::toString(param);
}

std::string peano::grid::tests::records::TestVertexPacked::getInsideOutsideDomainMapping() {
   return peano::grid::tests::records::TestVertex::getInsideOutsideDomainMapping();
}

std::string peano::grid::tests::records::TestVertexPacked::toString(const RefinementControl& param) {
   return peano::grid::tests::records::TestVertex::toString(param);
}

std::string peano::grid::tests::records::TestVertexPacked::getRefinementControlMapping() {
   return peano::grid::tests::records::TestVertex::getRefinementControlMapping();
}



std::string peano::grid::tests::records::TestVertexPacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void peano::grid::tests::records::TestVertexPacked::toString (std::ostream& out) const {
   out << "("; 
   out << "isHangingNode:" << getIsHangingNode();
   out << ",";
   out << "refinementControl:" << toString(getRefinementControl());
   out << ",";
   out << "adjacentCellsHeight:" << getAdjacentCellsHeight();
   out << ",";
   out << "adjacentCellsHeightOfPreviousIteration:" << getAdjacentCellsHeightOfPreviousIteration();
   out << ",";
   out << "numberOfAdjacentRefinedCells:" << getNumberOfAdjacentRefinedCells();
   out << ",";
   out << "insideOutsideDomain:" << toString(getInsideOutsideDomain());
   out << ",";
   out << "x:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getX(i) << ",";
   }
   out << getX(DIMENSIONS-1) << "]";
   out << ",";
   out << "level:" << getLevel();
   out << ",";
   out << "adjacentRanks:[";
   for (int i = 0; i < TWO_POWER_D-1; i++) {
      out << getAdjacentRanks(i) << ",";
   }
   out << getAdjacentRanks(TWO_POWER_D-1) << "]";
   out << ",";
   out << "adjacentSubtreeForksIntoOtherRank:" << getAdjacentSubtreeForksIntoOtherRank();
   out <<  ")";
}


peano::grid::tests::records::TestVertexPacked::PersistentRecords peano::grid::tests::records::TestVertexPacked::getPersistentRecords() const {
   return _persistentRecords;
}

peano::grid::tests::records::TestVertex peano::grid::tests::records::TestVertexPacked::convert() const{
   return TestVertex(
      getIsHangingNode(),
      getRefinementControl(),
      getAdjacentCellsHeight(),
      getAdjacentCellsHeightOfPreviousIteration(),
      getNumberOfAdjacentRefinedCells(),
      getInsideOutsideDomain(),
      getX(),
      getLevel(),
      getAdjacentRanks(),
      getAdjacentSubtreeForksIntoOtherRank()
   );
}

#ifdef Parallel
   tarch::logging::Log peano::grid::tests::records::TestVertexPacked::_log( "peano::grid::tests::records::TestVertexPacked" );
   
   MPI_Datatype peano::grid::tests::records::TestVertexPacked::Datatype = 0;
   MPI_Datatype peano::grid::tests::records::TestVertexPacked::FullDatatype = 0;
   
   
   void peano::grid::tests::records::TestVertexPacked::initDatatype() {
      {
         TestVertexPacked dummyTestVertexPacked[2];
         
         const int Attributes = 7;
         MPI_Datatype subtypes[Attributes] = {
            MPI_DOUBLE,		 //x
            MPI_INT,		 //level
            MPI_INT,		 //adjacentRanks
            MPI_CHAR,		 //adjacentSubtreeForksIntoOtherRank
            MPI_INT,		 //_packedRecords0
            MPI_INT,		 //numberOfAdjacentRefinedCells
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            DIMENSIONS,		 //x
            1,		 //level
            TWO_POWER_D,		 //adjacentRanks
            1,		 //adjacentSubtreeForksIntoOtherRank
            1,		 //_packedRecords0
            1,		 //numberOfAdjacentRefinedCells
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._x[0]))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._level))), 		&disp[1] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentRanks[0]))), 		&disp[2] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentSubtreeForksIntoOtherRank))), 		&disp[3] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._packedRecords0))), 		&disp[4] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._numberOfAdjacentRefinedCells))), 		&disp[5] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyTestVertexPacked[1]._persistentRecords._x[0])), 		&disp[6] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertexPacked::Datatype );
         MPI_Type_commit( &TestVertexPacked::Datatype );
         
      }
      {
         TestVertexPacked dummyTestVertexPacked[2];
         
         const int Attributes = 9;
         MPI_Datatype subtypes[Attributes] = {
            MPI_INT,		 //adjacentCellsHeight
            MPI_DOUBLE,		 //x
            MPI_INT,		 //level
            MPI_INT,		 //adjacentRanks
            MPI_CHAR,		 //adjacentSubtreeForksIntoOtherRank
            MPI_INT,		 //_packedRecords0
            MPI_INT,		 //adjacentCellsHeightOfPreviousIteration
            MPI_INT,		 //numberOfAdjacentRefinedCells
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1,		 //adjacentCellsHeight
            DIMENSIONS,		 //x
            1,		 //level
            TWO_POWER_D,		 //adjacentRanks
            1,		 //adjacentSubtreeForksIntoOtherRank
            1,		 //_packedRecords0
            1,		 //adjacentCellsHeightOfPreviousIteration
            1,		 //numberOfAdjacentRefinedCells
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentCellsHeight))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._x[0]))), 		&disp[1] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._level))), 		&disp[2] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentRanks[0]))), 		&disp[3] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentSubtreeForksIntoOtherRank))), 		&disp[4] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._packedRecords0))), 		&disp[5] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._adjacentCellsHeightOfPreviousIteration))), 		&disp[6] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._numberOfAdjacentRefinedCells))), 		&disp[7] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[1]._persistentRecords._adjacentCellsHeight))), 		&disp[8] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertexPacked::FullDatatype );
         MPI_Type_commit( &TestVertexPacked::FullDatatype );
         
      }
      
   }
   
   
   void peano::grid::tests::records::TestVertexPacked::shutdownDatatype() {
      MPI_Type_free( &TestVertexPacked::Datatype );
      MPI_Type_free( &TestVertexPacked::FullDatatype );
      
   }
   
   void peano::grid::tests::records::TestVertexPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      _senderDestinationRank = destination;
      
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message peano::grid::tests::records::TestVertexPacked "
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
         msg << "was not able to send message peano::grid::tests::records::TestVertexPacked "
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
            msg << "testing for finished send task for peano::grid::tests::records::TestVertexPacked "
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
            "peano::grid::tests::records::TestVertexPacked",
            "send(int)", destination,tag,1
            );
            triggeredTimeoutWarning = true;
         }
         if (
            tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
            (clock()>timeOutShutdown)
         ) {
            tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
            "peano::grid::tests::records::TestVertexPacked",
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



void peano::grid::tests::records::TestVertexPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
   if (communicateSleep<0) {
   
      MPI_Status  status;
      const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
      _senderDestinationRank = status.MPI_SOURCE;
      if ( result != MPI_SUCCESS ) {
         std::ostringstream msg;
         msg << "failed to start to receive peano::grid::tests::records::TestVertexPacked from node "
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
         msg << "failed to start to receive peano::grid::tests::records::TestVertexPacked from node "
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
            msg << "testing for finished receive task for peano::grid::tests::records::TestVertexPacked failed: "
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
            "peano::grid::tests::records::TestVertexPacked",
            "receive(int)", source,tag,1
            );
            triggeredTimeoutWarning = true;
         }
         if (
            tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
            (clock()>timeOutShutdown)
         ) {
            tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
            "peano::grid::tests::records::TestVertexPacked",
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



bool peano::grid::tests::records::TestVertexPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestVertexPacked::getSenderRank() const {
   assertion( _senderDestinationRank!=-1 );
   return _senderDestinationRank;
   
}
#endif



#elif !defined(Parallel) && !defined(Asserts)
peano::grid::tests::records::TestVertex::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestVertex::PersistentRecords::PersistentRecords(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain):
_isHangingNode(isHangingNode),
_refinementControl(refinementControl),
_adjacentCellsHeight(adjacentCellsHeight),
_insideOutsideDomain(insideOutsideDomain) {

}


 bool peano::grid::tests::records::TestVertex::PersistentRecords::getIsHangingNode() const  {
return _isHangingNode;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setIsHangingNode(const bool& isHangingNode)  {
_isHangingNode = isHangingNode;
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertex::PersistentRecords::getRefinementControl() const  {
return _refinementControl;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setRefinementControl(const RefinementControl& refinementControl)  {
_refinementControl = refinementControl;
}



 int peano::grid::tests::records::TestVertex::PersistentRecords::getAdjacentCellsHeight() const  {
return _adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_adjacentCellsHeight = adjacentCellsHeight;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertex::PersistentRecords::getInsideOutsideDomain() const  {
return _insideOutsideDomain;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
_insideOutsideDomain = insideOutsideDomain;
}


peano::grid::tests::records::TestVertex::TestVertex() {

}


peano::grid::tests::records::TestVertex::TestVertex(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isHangingNode, persistentRecords._refinementControl, persistentRecords._adjacentCellsHeight, persistentRecords._insideOutsideDomain) {

}


peano::grid::tests::records::TestVertex::TestVertex(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain) {

}


peano::grid::tests::records::TestVertex::TestVertex(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const int& adjacentCellsHeightOfPreviousIteration, const int& numberOfAdjacentRefinedCells, const InsideOutsideDomain& insideOutsideDomain):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain),_adjacentCellsHeightOfPreviousIteration(adjacentCellsHeightOfPreviousIteration),
_numberOfAdjacentRefinedCells(numberOfAdjacentRefinedCells) {

}

peano::grid::tests::records::TestVertex::~TestVertex() { }


 bool peano::grid::tests::records::TestVertex::getIsHangingNode() const  {
return _persistentRecords._isHangingNode;
}



 void peano::grid::tests::records::TestVertex::setIsHangingNode(const bool& isHangingNode)  {
_persistentRecords._isHangingNode = isHangingNode;
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertex::getRefinementControl() const  {
return _persistentRecords._refinementControl;
}



 void peano::grid::tests::records::TestVertex::setRefinementControl(const RefinementControl& refinementControl)  {
_persistentRecords._refinementControl = refinementControl;
}



 int peano::grid::tests::records::TestVertex::getAdjacentCellsHeight() const  {
return _persistentRecords._adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertex::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_persistentRecords._adjacentCellsHeight = adjacentCellsHeight;
}



 int peano::grid::tests::records::TestVertex::getAdjacentCellsHeightOfPreviousIteration() const  {
return _adjacentCellsHeightOfPreviousIteration;
}



 void peano::grid::tests::records::TestVertex::setAdjacentCellsHeightOfPreviousIteration(const int& adjacentCellsHeightOfPreviousIteration)  {
_adjacentCellsHeightOfPreviousIteration = adjacentCellsHeightOfPreviousIteration;
}



 int peano::grid::tests::records::TestVertex::getNumberOfAdjacentRefinedCells() const  {
return _numberOfAdjacentRefinedCells;
}



 void peano::grid::tests::records::TestVertex::setNumberOfAdjacentRefinedCells(const int& numberOfAdjacentRefinedCells)  {
_numberOfAdjacentRefinedCells = numberOfAdjacentRefinedCells;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertex::getInsideOutsideDomain() const  {
return _persistentRecords._insideOutsideDomain;
}



 void peano::grid::tests::records::TestVertex::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
_persistentRecords._insideOutsideDomain = insideOutsideDomain;
}


std::string peano::grid::tests::records::TestVertex::toString(const InsideOutsideDomain& param) {
switch (param) {
   case Inside: return "Inside";
   case Boundary: return "Boundary";
   case Outside: return "Outside";
}
return "undefined";
}

std::string peano::grid::tests::records::TestVertex::getInsideOutsideDomainMapping() {
return "InsideOutsideDomain(Inside=0,Boundary=1,Outside=2)";
}
std::string peano::grid::tests::records::TestVertex::toString(const RefinementControl& param) {
switch (param) {
   case Unrefined: return "Unrefined";
   case Refined: return "Refined";
   case RefinementTriggered: return "RefinementTriggered";
   case Refining: return "Refining";
   case EraseTriggered: return "EraseTriggered";
   case Erasing: return "Erasing";
   case RefineDueToJoinThoughWorkerIsAlreadyErasing: return "RefineDueToJoinThoughWorkerIsAlreadyErasing";
   case EnforceRefinementTriggered: return "EnforceRefinementTriggered";
}
return "undefined";
}

std::string peano::grid::tests::records::TestVertex::getRefinementControlMapping() {
return "RefinementControl(Unrefined=0,Refined=1,RefinementTriggered=2,Refining=3,EraseTriggered=4,Erasing=5,RefineDueToJoinThoughWorkerIsAlreadyErasing=6,EnforceRefinementTriggered=7)";
}


std::string peano::grid::tests::records::TestVertex::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestVertex::toString (std::ostream& out) const {
out << "("; 
out << "isHangingNode:" << getIsHangingNode();
out << ",";
out << "refinementControl:" << toString(getRefinementControl());
out << ",";
out << "adjacentCellsHeight:" << getAdjacentCellsHeight();
out << ",";
out << "adjacentCellsHeightOfPreviousIteration:" << getAdjacentCellsHeightOfPreviousIteration();
out << ",";
out << "numberOfAdjacentRefinedCells:" << getNumberOfAdjacentRefinedCells();
out << ",";
out << "insideOutsideDomain:" << toString(getInsideOutsideDomain());
out <<  ")";
}


peano::grid::tests::records::TestVertex::PersistentRecords peano::grid::tests::records::TestVertex::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestVertexPacked peano::grid::tests::records::TestVertex::convert() const{
return TestVertexPacked(
   getIsHangingNode(),
   getRefinementControl(),
   getAdjacentCellsHeight(),
   getAdjacentCellsHeightOfPreviousIteration(),
   getNumberOfAdjacentRefinedCells(),
   getInsideOutsideDomain()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestVertex::_log( "peano::grid::tests::records::TestVertex" );

MPI_Datatype peano::grid::tests::records::TestVertex::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestVertex::FullDatatype = 0;


void peano::grid::tests::records::TestVertex::initDatatype() {
   {
      TestVertex dummyTestVertex[2];
      
      const int Attributes = 4;
      MPI_Datatype subtypes[Attributes] = {
         MPI_CHAR,		 //isHangingNode
         MPI_INT,		 //refinementControl
         MPI_INT,		 //numberOfAdjacentRefinedCells
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         1,		 //isHangingNode
         1,		 //refinementControl
         1,		 //numberOfAdjacentRefinedCells
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]))), &base);
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._isHangingNode))), 		&disp[0] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._refinementControl))), 		&disp[1] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._numberOfAdjacentRefinedCells))), 		&disp[2] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[1]._persistentRecords._isHangingNode))), 		&disp[3] );
      
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertex::Datatype );
      MPI_Type_commit( &TestVertex::Datatype );
      
   }
   {
      TestVertex dummyTestVertex[2];
      
      const int Attributes = 7;
      MPI_Datatype subtypes[Attributes] = {
         MPI_CHAR,		 //isHangingNode
         MPI_INT,		 //refinementControl
         MPI_INT,		 //adjacentCellsHeight
         MPI_INT,		 //insideOutsideDomain
         MPI_INT,		 //adjacentCellsHeightOfPreviousIteration
         MPI_INT,		 //numberOfAdjacentRefinedCells
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         1,		 //isHangingNode
         1,		 //refinementControl
         1,		 //adjacentCellsHeight
         1,		 //insideOutsideDomain
         1,		 //adjacentCellsHeightOfPreviousIteration
         1,		 //numberOfAdjacentRefinedCells
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]))), &base);
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._isHangingNode))), 		&disp[0] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._refinementControl))), 		&disp[1] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentCellsHeight))), 		&disp[2] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._insideOutsideDomain))), 		&disp[3] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._adjacentCellsHeightOfPreviousIteration))), 		&disp[4] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._numberOfAdjacentRefinedCells))), 		&disp[5] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[1]._persistentRecords._isHangingNode))), 		&disp[6] );
      
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertex::FullDatatype );
      MPI_Type_commit( &TestVertex::FullDatatype );
      
   }
   
}


void peano::grid::tests::records::TestVertex::shutdownDatatype() {
   MPI_Type_free( &TestVertex::Datatype );
   MPI_Type_free( &TestVertex::FullDatatype );
   
}

void peano::grid::tests::records::TestVertex::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
   _senderDestinationRank = destination;
   
   if (communicateSleep<0) {
   
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
      if  (result!=MPI_SUCCESS) {
         std::ostringstream msg;
         msg << "was not able to send message peano::grid::tests::records::TestVertex "
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
      msg << "was not able to send message peano::grid::tests::records::TestVertex "
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
         msg << "testing for finished send task for peano::grid::tests::records::TestVertex "
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
         "peano::grid::tests::records::TestVertex",
         "send(int)", destination,tag,1
         );
         triggeredTimeoutWarning = true;
      }
      if (
         tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
         (clock()>timeOutShutdown)
      ) {
         tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
         "peano::grid::tests::records::TestVertex",
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



void peano::grid::tests::records::TestVertex::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

   MPI_Status  status;
   const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
   _senderDestinationRank = status.MPI_SOURCE;
   if ( result != MPI_SUCCESS ) {
      std::ostringstream msg;
      msg << "failed to start to receive peano::grid::tests::records::TestVertex from node "
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
      msg << "failed to start to receive peano::grid::tests::records::TestVertex from node "
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
         msg << "testing for finished receive task for peano::grid::tests::records::TestVertex failed: "
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
         "peano::grid::tests::records::TestVertex",
         "receive(int)", source,tag,1
         );
         triggeredTimeoutWarning = true;
      }
      if (
         tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
         (clock()>timeOutShutdown)
      ) {
         tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
         "peano::grid::tests::records::TestVertex",
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



bool peano::grid::tests::records::TestVertex::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestVertex::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestVertexPacked::PersistentRecords::PersistentRecords() {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::PersistentRecords::PersistentRecords(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain):
_adjacentCellsHeight(adjacentCellsHeight) {
setIsHangingNode(isHangingNode);
setRefinementControl(refinementControl);
setInsideOutsideDomain(insideOutsideDomain);
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestVertexPacked::PersistentRecords::getIsHangingNode() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setIsHangingNode(const bool& isHangingNode)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isHangingNode ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertexPacked::PersistentRecords::getRefinementControl() const  {
int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 7));
   return (RefinementControl) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setRefinementControl(const RefinementControl& refinementControl)  {
assertion((refinementControl >= 0 && refinementControl <= 7));
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(refinementControl) << (1));
}



 int peano::grid::tests::records::TestVertexPacked::PersistentRecords::getAdjacentCellsHeight() const  {
return _adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_adjacentCellsHeight = adjacentCellsHeight;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertexPacked::PersistentRecords::getInsideOutsideDomain() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (4));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (InsideOutsideDomain) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
assertion((insideOutsideDomain >= 0 && insideOutsideDomain <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(insideOutsideDomain) << (4));
}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked() {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsHangingNode(), persistentRecords.getRefinementControl(), persistentRecords._adjacentCellsHeight, persistentRecords.getInsideOutsideDomain()) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const int& adjacentCellsHeightOfPreviousIteration, const int& numberOfAdjacentRefinedCells, const InsideOutsideDomain& insideOutsideDomain):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain),_adjacentCellsHeightOfPreviousIteration(adjacentCellsHeightOfPreviousIteration),
_numberOfAdjacentRefinedCells(numberOfAdjacentRefinedCells) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}

peano::grid::tests::records::TestVertexPacked::~TestVertexPacked() { }


 bool peano::grid::tests::records::TestVertexPacked::getIsHangingNode() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestVertexPacked::setIsHangingNode(const bool& isHangingNode)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isHangingNode ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertexPacked::getRefinementControl() const  {
int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 7));
   return (RefinementControl) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::setRefinementControl(const RefinementControl& refinementControl)  {
assertion((refinementControl >= 0 && refinementControl <= 7));
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(refinementControl) << (1));
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentCellsHeight() const  {
return _persistentRecords._adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_persistentRecords._adjacentCellsHeight = adjacentCellsHeight;
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentCellsHeightOfPreviousIteration() const  {
return _adjacentCellsHeightOfPreviousIteration;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentCellsHeightOfPreviousIteration(const int& adjacentCellsHeightOfPreviousIteration)  {
_adjacentCellsHeightOfPreviousIteration = adjacentCellsHeightOfPreviousIteration;
}



 int peano::grid::tests::records::TestVertexPacked::getNumberOfAdjacentRefinedCells() const  {
return _numberOfAdjacentRefinedCells;
}



 void peano::grid::tests::records::TestVertexPacked::setNumberOfAdjacentRefinedCells(const int& numberOfAdjacentRefinedCells)  {
_numberOfAdjacentRefinedCells = numberOfAdjacentRefinedCells;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertexPacked::getInsideOutsideDomain() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (4));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (InsideOutsideDomain) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
assertion((insideOutsideDomain >= 0 && insideOutsideDomain <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(insideOutsideDomain) << (4));
}


std::string peano::grid::tests::records::TestVertexPacked::toString(const InsideOutsideDomain& param) {
return peano::grid::tests::records::TestVertex::toString(param);
}

std::string peano::grid::tests::records::TestVertexPacked::getInsideOutsideDomainMapping() {
return peano::grid::tests::records::TestVertex::getInsideOutsideDomainMapping();
}

std::string peano::grid::tests::records::TestVertexPacked::toString(const RefinementControl& param) {
return peano::grid::tests::records::TestVertex::toString(param);
}

std::string peano::grid::tests::records::TestVertexPacked::getRefinementControlMapping() {
return peano::grid::tests::records::TestVertex::getRefinementControlMapping();
}



std::string peano::grid::tests::records::TestVertexPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestVertexPacked::toString (std::ostream& out) const {
out << "("; 
out << "isHangingNode:" << getIsHangingNode();
out << ",";
out << "refinementControl:" << toString(getRefinementControl());
out << ",";
out << "adjacentCellsHeight:" << getAdjacentCellsHeight();
out << ",";
out << "adjacentCellsHeightOfPreviousIteration:" << getAdjacentCellsHeightOfPreviousIteration();
out << ",";
out << "numberOfAdjacentRefinedCells:" << getNumberOfAdjacentRefinedCells();
out << ",";
out << "insideOutsideDomain:" << toString(getInsideOutsideDomain());
out <<  ")";
}


peano::grid::tests::records::TestVertexPacked::PersistentRecords peano::grid::tests::records::TestVertexPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestVertex peano::grid::tests::records::TestVertexPacked::convert() const{
return TestVertex(
getIsHangingNode(),
getRefinementControl(),
getAdjacentCellsHeight(),
getAdjacentCellsHeightOfPreviousIteration(),
getNumberOfAdjacentRefinedCells(),
getInsideOutsideDomain()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestVertexPacked::_log( "peano::grid::tests::records::TestVertexPacked" );

MPI_Datatype peano::grid::tests::records::TestVertexPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestVertexPacked::FullDatatype = 0;


void peano::grid::tests::records::TestVertexPacked::initDatatype() {
{
   TestVertexPacked dummyTestVertexPacked[2];
   
   const int Attributes = 3;
   MPI_Datatype subtypes[Attributes] = {
      MPI_INT,		 //_packedRecords0
      MPI_INT,		 //numberOfAdjacentRefinedCells
      MPI_UB		 // end/displacement flag
   };
   
   int blocklen[Attributes] = {
      1,		 //_packedRecords0
      1,		 //numberOfAdjacentRefinedCells
      1		 // end/displacement flag
   };
   
   MPI_Aint     disp[Attributes];
   
   MPI_Aint base;
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]))), &base);
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._packedRecords0))), 		&disp[0] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._numberOfAdjacentRefinedCells))), 		&disp[1] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[1]._persistentRecords._packedRecords0))), 		&disp[2] );
   
   for (int i=1; i<Attributes; i++) {
      assertion1( disp[i] > disp[i-1], i );
   }
   for (int i=0; i<Attributes; i++) {
      disp[i] -= base;
   }
   MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertexPacked::Datatype );
   MPI_Type_commit( &TestVertexPacked::Datatype );
   
}
{
   TestVertexPacked dummyTestVertexPacked[2];
   
   const int Attributes = 5;
   MPI_Datatype subtypes[Attributes] = {
      MPI_INT,		 //adjacentCellsHeight
      MPI_INT,		 //_packedRecords0
      MPI_INT,		 //adjacentCellsHeightOfPreviousIteration
      MPI_INT,		 //numberOfAdjacentRefinedCells
      MPI_UB		 // end/displacement flag
   };
   
   int blocklen[Attributes] = {
      1,		 //adjacentCellsHeight
      1,		 //_packedRecords0
      1,		 //adjacentCellsHeightOfPreviousIteration
      1,		 //numberOfAdjacentRefinedCells
      1		 // end/displacement flag
   };
   
   MPI_Aint     disp[Attributes];
   
   MPI_Aint base;
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]))), &base);
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentCellsHeight))), 		&disp[0] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._packedRecords0))), 		&disp[1] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._adjacentCellsHeightOfPreviousIteration))), 		&disp[2] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._numberOfAdjacentRefinedCells))), 		&disp[3] );
   MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[1]._persistentRecords._adjacentCellsHeight))), 		&disp[4] );
   
   for (int i=1; i<Attributes; i++) {
      assertion1( disp[i] > disp[i-1], i );
   }
   for (int i=0; i<Attributes; i++) {
      disp[i] -= base;
   }
   MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertexPacked::FullDatatype );
   MPI_Type_commit( &TestVertexPacked::FullDatatype );
   
}

}


void peano::grid::tests::records::TestVertexPacked::shutdownDatatype() {
MPI_Type_free( &TestVertexPacked::Datatype );
MPI_Type_free( &TestVertexPacked::FullDatatype );

}

void peano::grid::tests::records::TestVertexPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

   const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
   if  (result!=MPI_SUCCESS) {
      std::ostringstream msg;
      msg << "was not able to send message peano::grid::tests::records::TestVertexPacked "
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
   msg << "was not able to send message peano::grid::tests::records::TestVertexPacked "
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
      msg << "testing for finished send task for peano::grid::tests::records::TestVertexPacked "
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
      "peano::grid::tests::records::TestVertexPacked",
      "send(int)", destination,tag,1
      );
      triggeredTimeoutWarning = true;
   }
   if (
      tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
      (clock()>timeOutShutdown)
   ) {
      tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
      "peano::grid::tests::records::TestVertexPacked",
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



void peano::grid::tests::records::TestVertexPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
   std::ostringstream msg;
   msg << "failed to start to receive peano::grid::tests::records::TestVertexPacked from node "
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
   msg << "failed to start to receive peano::grid::tests::records::TestVertexPacked from node "
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
      msg << "testing for finished receive task for peano::grid::tests::records::TestVertexPacked failed: "
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
      "peano::grid::tests::records::TestVertexPacked",
      "receive(int)", source,tag,1
      );
      triggeredTimeoutWarning = true;
   }
   if (
      tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
      (clock()>timeOutShutdown)
   ) {
      tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
      "peano::grid::tests::records::TestVertexPacked",
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



bool peano::grid::tests::records::TestVertexPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestVertexPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif !defined(Parallel) && defined(Asserts)
peano::grid::tests::records::TestVertex::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestVertex::PersistentRecords::PersistentRecords(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level):
_isHangingNode(isHangingNode),
_refinementControl(refinementControl),
_adjacentCellsHeight(adjacentCellsHeight),
_insideOutsideDomain(insideOutsideDomain),
_x(x),
_level(level) {

}


 bool peano::grid::tests::records::TestVertex::PersistentRecords::getIsHangingNode() const  {
return _isHangingNode;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setIsHangingNode(const bool& isHangingNode)  {
_isHangingNode = isHangingNode;
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertex::PersistentRecords::getRefinementControl() const  {
return _refinementControl;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setRefinementControl(const RefinementControl& refinementControl)  {
_refinementControl = refinementControl;
}



 int peano::grid::tests::records::TestVertex::PersistentRecords::getAdjacentCellsHeight() const  {
return _adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_adjacentCellsHeight = adjacentCellsHeight;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertex::PersistentRecords::getInsideOutsideDomain() const  {
return _insideOutsideDomain;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
_insideOutsideDomain = insideOutsideDomain;
}



 tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestVertex::PersistentRecords::getX() const  {
return _x;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setX(const tarch::la::Vector<DIMENSIONS,double>& x)  {
_x = (x);
}



 int peano::grid::tests::records::TestVertex::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setLevel(const int& level)  {
_level = level;
}


peano::grid::tests::records::TestVertex::TestVertex() {

}


peano::grid::tests::records::TestVertex::TestVertex(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isHangingNode, persistentRecords._refinementControl, persistentRecords._adjacentCellsHeight, persistentRecords._insideOutsideDomain, persistentRecords._x, persistentRecords._level) {

}


peano::grid::tests::records::TestVertex::TestVertex(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, x, level) {

}


peano::grid::tests::records::TestVertex::TestVertex(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const int& adjacentCellsHeightOfPreviousIteration, const int& numberOfAdjacentRefinedCells, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, x, level),_adjacentCellsHeightOfPreviousIteration(adjacentCellsHeightOfPreviousIteration),
_numberOfAdjacentRefinedCells(numberOfAdjacentRefinedCells) {

}

peano::grid::tests::records::TestVertex::~TestVertex() { }


 bool peano::grid::tests::records::TestVertex::getIsHangingNode() const  {
return _persistentRecords._isHangingNode;
}



 void peano::grid::tests::records::TestVertex::setIsHangingNode(const bool& isHangingNode)  {
_persistentRecords._isHangingNode = isHangingNode;
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertex::getRefinementControl() const  {
return _persistentRecords._refinementControl;
}



 void peano::grid::tests::records::TestVertex::setRefinementControl(const RefinementControl& refinementControl)  {
_persistentRecords._refinementControl = refinementControl;
}



 int peano::grid::tests::records::TestVertex::getAdjacentCellsHeight() const  {
return _persistentRecords._adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertex::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_persistentRecords._adjacentCellsHeight = adjacentCellsHeight;
}



 int peano::grid::tests::records::TestVertex::getAdjacentCellsHeightOfPreviousIteration() const  {
return _adjacentCellsHeightOfPreviousIteration;
}



 void peano::grid::tests::records::TestVertex::setAdjacentCellsHeightOfPreviousIteration(const int& adjacentCellsHeightOfPreviousIteration)  {
_adjacentCellsHeightOfPreviousIteration = adjacentCellsHeightOfPreviousIteration;
}



 int peano::grid::tests::records::TestVertex::getNumberOfAdjacentRefinedCells() const  {
return _numberOfAdjacentRefinedCells;
}



 void peano::grid::tests::records::TestVertex::setNumberOfAdjacentRefinedCells(const int& numberOfAdjacentRefinedCells)  {
_numberOfAdjacentRefinedCells = numberOfAdjacentRefinedCells;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertex::getInsideOutsideDomain() const  {
return _persistentRecords._insideOutsideDomain;
}



 void peano::grid::tests::records::TestVertex::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
_persistentRecords._insideOutsideDomain = insideOutsideDomain;
}



 tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestVertex::getX() const  {
return _persistentRecords._x;
}



 void peano::grid::tests::records::TestVertex::setX(const tarch::la::Vector<DIMENSIONS,double>& x)  {
_persistentRecords._x = (x);
}



 double peano::grid::tests::records::TestVertex::getX(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._x[elementIndex];

}



 void peano::grid::tests::records::TestVertex::setX(int elementIndex, const double& x)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._x[elementIndex]= x;

}



 int peano::grid::tests::records::TestVertex::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestVertex::setLevel(const int& level)  {
_persistentRecords._level = level;
}


std::string peano::grid::tests::records::TestVertex::toString(const InsideOutsideDomain& param) {
switch (param) {
case Inside: return "Inside";
case Boundary: return "Boundary";
case Outside: return "Outside";
}
return "undefined";
}

std::string peano::grid::tests::records::TestVertex::getInsideOutsideDomainMapping() {
return "InsideOutsideDomain(Inside=0,Boundary=1,Outside=2)";
}
std::string peano::grid::tests::records::TestVertex::toString(const RefinementControl& param) {
switch (param) {
case Unrefined: return "Unrefined";
case Refined: return "Refined";
case RefinementTriggered: return "RefinementTriggered";
case Refining: return "Refining";
case EraseTriggered: return "EraseTriggered";
case Erasing: return "Erasing";
case RefineDueToJoinThoughWorkerIsAlreadyErasing: return "RefineDueToJoinThoughWorkerIsAlreadyErasing";
case EnforceRefinementTriggered: return "EnforceRefinementTriggered";
}
return "undefined";
}

std::string peano::grid::tests::records::TestVertex::getRefinementControlMapping() {
return "RefinementControl(Unrefined=0,Refined=1,RefinementTriggered=2,Refining=3,EraseTriggered=4,Erasing=5,RefineDueToJoinThoughWorkerIsAlreadyErasing=6,EnforceRefinementTriggered=7)";
}


std::string peano::grid::tests::records::TestVertex::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestVertex::toString (std::ostream& out) const {
out << "("; 
out << "isHangingNode:" << getIsHangingNode();
out << ",";
out << "refinementControl:" << toString(getRefinementControl());
out << ",";
out << "adjacentCellsHeight:" << getAdjacentCellsHeight();
out << ",";
out << "adjacentCellsHeightOfPreviousIteration:" << getAdjacentCellsHeightOfPreviousIteration();
out << ",";
out << "numberOfAdjacentRefinedCells:" << getNumberOfAdjacentRefinedCells();
out << ",";
out << "insideOutsideDomain:" << toString(getInsideOutsideDomain());
out << ",";
out << "x:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getX(i) << ",";
   }
   out << getX(DIMENSIONS-1) << "]";
out << ",";
out << "level:" << getLevel();
out <<  ")";
}


peano::grid::tests::records::TestVertex::PersistentRecords peano::grid::tests::records::TestVertex::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestVertexPacked peano::grid::tests::records::TestVertex::convert() const{
return TestVertexPacked(
getIsHangingNode(),
getRefinementControl(),
getAdjacentCellsHeight(),
getAdjacentCellsHeightOfPreviousIteration(),
getNumberOfAdjacentRefinedCells(),
getInsideOutsideDomain(),
getX(),
getLevel()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestVertex::_log( "peano::grid::tests::records::TestVertex" );

MPI_Datatype peano::grid::tests::records::TestVertex::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestVertex::FullDatatype = 0;


void peano::grid::tests::records::TestVertex::initDatatype() {
{
TestVertex dummyTestVertex[2];

const int Attributes = 7;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isHangingNode
MPI_INT,		 //refinementControl
MPI_INT,		 //insideOutsideDomain
MPI_DOUBLE,		 //x
MPI_INT,		 //level
MPI_INT,		 //numberOfAdjacentRefinedCells
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isHangingNode
1,		 //refinementControl
1,		 //insideOutsideDomain
DIMENSIONS,		 //x
1,		 //level
1,		 //numberOfAdjacentRefinedCells
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._isHangingNode))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._refinementControl))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._insideOutsideDomain))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._x[0]))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._level))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._numberOfAdjacentRefinedCells))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[1]._persistentRecords._isHangingNode))), 		&disp[6] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertex::Datatype );
MPI_Type_commit( &TestVertex::Datatype );

}
{
TestVertex dummyTestVertex[2];

const int Attributes = 9;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isHangingNode
MPI_INT,		 //refinementControl
MPI_INT,		 //adjacentCellsHeight
MPI_INT,		 //insideOutsideDomain
MPI_DOUBLE,		 //x
MPI_INT,		 //level
MPI_INT,		 //adjacentCellsHeightOfPreviousIteration
MPI_INT,		 //numberOfAdjacentRefinedCells
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isHangingNode
1,		 //refinementControl
1,		 //adjacentCellsHeight
1,		 //insideOutsideDomain
DIMENSIONS,		 //x
1,		 //level
1,		 //adjacentCellsHeightOfPreviousIteration
1,		 //numberOfAdjacentRefinedCells
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._isHangingNode))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._refinementControl))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentCellsHeight))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._insideOutsideDomain))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._x[0]))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._level))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._adjacentCellsHeightOfPreviousIteration))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._numberOfAdjacentRefinedCells))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[1]._persistentRecords._isHangingNode))), 		&disp[8] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertex::FullDatatype );
MPI_Type_commit( &TestVertex::FullDatatype );

}

}


void peano::grid::tests::records::TestVertex::shutdownDatatype() {
MPI_Type_free( &TestVertex::Datatype );
MPI_Type_free( &TestVertex::FullDatatype );

}

void peano::grid::tests::records::TestVertex::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestVertex "
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
msg << "was not able to send message peano::grid::tests::records::TestVertex "
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
msg << "testing for finished send task for peano::grid::tests::records::TestVertex "
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
"peano::grid::tests::records::TestVertex",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestVertex",
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



void peano::grid::tests::records::TestVertex::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestVertex from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestVertex from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestVertex failed: "
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
"peano::grid::tests::records::TestVertex",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestVertex",
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



bool peano::grid::tests::records::TestVertex::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestVertex::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestVertexPacked::PersistentRecords::PersistentRecords() {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::PersistentRecords::PersistentRecords(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level):
_adjacentCellsHeight(adjacentCellsHeight),
_x(x),
_level(level) {
setIsHangingNode(isHangingNode);
setRefinementControl(refinementControl);
setInsideOutsideDomain(insideOutsideDomain);
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestVertexPacked::PersistentRecords::getIsHangingNode() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setIsHangingNode(const bool& isHangingNode)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isHangingNode ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertexPacked::PersistentRecords::getRefinementControl() const  {
int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 7));
   return (RefinementControl) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setRefinementControl(const RefinementControl& refinementControl)  {
assertion((refinementControl >= 0 && refinementControl <= 7));
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(refinementControl) << (1));
}



 int peano::grid::tests::records::TestVertexPacked::PersistentRecords::getAdjacentCellsHeight() const  {
return _adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_adjacentCellsHeight = adjacentCellsHeight;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertexPacked::PersistentRecords::getInsideOutsideDomain() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (4));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (InsideOutsideDomain) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
assertion((insideOutsideDomain >= 0 && insideOutsideDomain <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(insideOutsideDomain) << (4));
}



 tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestVertexPacked::PersistentRecords::getX() const  {
return _x;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setX(const tarch::la::Vector<DIMENSIONS,double>& x)  {
_x = (x);
}



 int peano::grid::tests::records::TestVertexPacked::PersistentRecords::getLevel() const  {
return _level;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setLevel(const int& level)  {
_level = level;
}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked() {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsHangingNode(), persistentRecords.getRefinementControl(), persistentRecords._adjacentCellsHeight, persistentRecords.getInsideOutsideDomain(), persistentRecords._x, persistentRecords._level) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, x, level) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const int& adjacentCellsHeightOfPreviousIteration, const int& numberOfAdjacentRefinedCells, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<DIMENSIONS,double>& x, const int& level):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, x, level),_adjacentCellsHeightOfPreviousIteration(adjacentCellsHeightOfPreviousIteration),
_numberOfAdjacentRefinedCells(numberOfAdjacentRefinedCells) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}

peano::grid::tests::records::TestVertexPacked::~TestVertexPacked() { }


 bool peano::grid::tests::records::TestVertexPacked::getIsHangingNode() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestVertexPacked::setIsHangingNode(const bool& isHangingNode)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isHangingNode ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertexPacked::getRefinementControl() const  {
int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 7));
   return (RefinementControl) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::setRefinementControl(const RefinementControl& refinementControl)  {
assertion((refinementControl >= 0 && refinementControl <= 7));
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(refinementControl) << (1));
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentCellsHeight() const  {
return _persistentRecords._adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_persistentRecords._adjacentCellsHeight = adjacentCellsHeight;
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentCellsHeightOfPreviousIteration() const  {
return _adjacentCellsHeightOfPreviousIteration;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentCellsHeightOfPreviousIteration(const int& adjacentCellsHeightOfPreviousIteration)  {
_adjacentCellsHeightOfPreviousIteration = adjacentCellsHeightOfPreviousIteration;
}



 int peano::grid::tests::records::TestVertexPacked::getNumberOfAdjacentRefinedCells() const  {
return _numberOfAdjacentRefinedCells;
}



 void peano::grid::tests::records::TestVertexPacked::setNumberOfAdjacentRefinedCells(const int& numberOfAdjacentRefinedCells)  {
_numberOfAdjacentRefinedCells = numberOfAdjacentRefinedCells;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertexPacked::getInsideOutsideDomain() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (4));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (InsideOutsideDomain) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
assertion((insideOutsideDomain >= 0 && insideOutsideDomain <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(insideOutsideDomain) << (4));
}



 tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestVertexPacked::getX() const  {
return _persistentRecords._x;
}



 void peano::grid::tests::records::TestVertexPacked::setX(const tarch::la::Vector<DIMENSIONS,double>& x)  {
_persistentRecords._x = (x);
}



 double peano::grid::tests::records::TestVertexPacked::getX(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
return _persistentRecords._x[elementIndex];

}



 void peano::grid::tests::records::TestVertexPacked::setX(int elementIndex, const double& x)  {
assertion(elementIndex>=0);
assertion(elementIndex<DIMENSIONS);
_persistentRecords._x[elementIndex]= x;

}



 int peano::grid::tests::records::TestVertexPacked::getLevel() const  {
return _persistentRecords._level;
}



 void peano::grid::tests::records::TestVertexPacked::setLevel(const int& level)  {
_persistentRecords._level = level;
}


std::string peano::grid::tests::records::TestVertexPacked::toString(const InsideOutsideDomain& param) {
return peano::grid::tests::records::TestVertex::toString(param);
}

std::string peano::grid::tests::records::TestVertexPacked::getInsideOutsideDomainMapping() {
return peano::grid::tests::records::TestVertex::getInsideOutsideDomainMapping();
}

std::string peano::grid::tests::records::TestVertexPacked::toString(const RefinementControl& param) {
return peano::grid::tests::records::TestVertex::toString(param);
}

std::string peano::grid::tests::records::TestVertexPacked::getRefinementControlMapping() {
return peano::grid::tests::records::TestVertex::getRefinementControlMapping();
}



std::string peano::grid::tests::records::TestVertexPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestVertexPacked::toString (std::ostream& out) const {
out << "("; 
out << "isHangingNode:" << getIsHangingNode();
out << ",";
out << "refinementControl:" << toString(getRefinementControl());
out << ",";
out << "adjacentCellsHeight:" << getAdjacentCellsHeight();
out << ",";
out << "adjacentCellsHeightOfPreviousIteration:" << getAdjacentCellsHeightOfPreviousIteration();
out << ",";
out << "numberOfAdjacentRefinedCells:" << getNumberOfAdjacentRefinedCells();
out << ",";
out << "insideOutsideDomain:" << toString(getInsideOutsideDomain());
out << ",";
out << "x:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getX(i) << ",";
   }
   out << getX(DIMENSIONS-1) << "]";
out << ",";
out << "level:" << getLevel();
out <<  ")";
}


peano::grid::tests::records::TestVertexPacked::PersistentRecords peano::grid::tests::records::TestVertexPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestVertex peano::grid::tests::records::TestVertexPacked::convert() const{
return TestVertex(
getIsHangingNode(),
getRefinementControl(),
getAdjacentCellsHeight(),
getAdjacentCellsHeightOfPreviousIteration(),
getNumberOfAdjacentRefinedCells(),
getInsideOutsideDomain(),
getX(),
getLevel()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestVertexPacked::_log( "peano::grid::tests::records::TestVertexPacked" );

MPI_Datatype peano::grid::tests::records::TestVertexPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestVertexPacked::FullDatatype = 0;


void peano::grid::tests::records::TestVertexPacked::initDatatype() {
{
TestVertexPacked dummyTestVertexPacked[2];

const int Attributes = 5;
MPI_Datatype subtypes[Attributes] = {
MPI_DOUBLE,		 //x
MPI_INT,		 //level
MPI_INT,		 //_packedRecords0
MPI_INT,		 //numberOfAdjacentRefinedCells
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
DIMENSIONS,		 //x
1,		 //level
1,		 //_packedRecords0
1,		 //numberOfAdjacentRefinedCells
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._x[0]))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._level))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._numberOfAdjacentRefinedCells))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyTestVertexPacked[1]._persistentRecords._x[0])), 		&disp[4] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertexPacked::Datatype );
MPI_Type_commit( &TestVertexPacked::Datatype );

}
{
TestVertexPacked dummyTestVertexPacked[2];

const int Attributes = 7;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //adjacentCellsHeight
MPI_DOUBLE,		 //x
MPI_INT,		 //level
MPI_INT,		 //_packedRecords0
MPI_INT,		 //adjacentCellsHeightOfPreviousIteration
MPI_INT,		 //numberOfAdjacentRefinedCells
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //adjacentCellsHeight
DIMENSIONS,		 //x
1,		 //level
1,		 //_packedRecords0
1,		 //adjacentCellsHeightOfPreviousIteration
1,		 //numberOfAdjacentRefinedCells
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentCellsHeight))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._x[0]))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._level))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._adjacentCellsHeightOfPreviousIteration))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._numberOfAdjacentRefinedCells))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[1]._persistentRecords._adjacentCellsHeight))), 		&disp[6] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertexPacked::FullDatatype );
MPI_Type_commit( &TestVertexPacked::FullDatatype );

}

}


void peano::grid::tests::records::TestVertexPacked::shutdownDatatype() {
MPI_Type_free( &TestVertexPacked::Datatype );
MPI_Type_free( &TestVertexPacked::FullDatatype );

}

void peano::grid::tests::records::TestVertexPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestVertexPacked "
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
msg << "was not able to send message peano::grid::tests::records::TestVertexPacked "
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
msg << "testing for finished send task for peano::grid::tests::records::TestVertexPacked "
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
"peano::grid::tests::records::TestVertexPacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestVertexPacked",
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



void peano::grid::tests::records::TestVertexPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestVertexPacked from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestVertexPacked from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestVertexPacked failed: "
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
"peano::grid::tests::records::TestVertexPacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestVertexPacked",
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



bool peano::grid::tests::records::TestVertexPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestVertexPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#elif defined(Parallel) && !defined(Asserts)
peano::grid::tests::records::TestVertex::PersistentRecords::PersistentRecords() {

}


peano::grid::tests::records::TestVertex::PersistentRecords::PersistentRecords(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_isHangingNode(isHangingNode),
_refinementControl(refinementControl),
_adjacentCellsHeight(adjacentCellsHeight),
_insideOutsideDomain(insideOutsideDomain),
_adjacentRanks(adjacentRanks),
_adjacentSubtreeForksIntoOtherRank(adjacentSubtreeForksIntoOtherRank) {

}


 bool peano::grid::tests::records::TestVertex::PersistentRecords::getIsHangingNode() const  {
return _isHangingNode;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setIsHangingNode(const bool& isHangingNode)  {
_isHangingNode = isHangingNode;
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertex::PersistentRecords::getRefinementControl() const  {
return _refinementControl;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setRefinementControl(const RefinementControl& refinementControl)  {
_refinementControl = refinementControl;
}



 int peano::grid::tests::records::TestVertex::PersistentRecords::getAdjacentCellsHeight() const  {
return _adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_adjacentCellsHeight = adjacentCellsHeight;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertex::PersistentRecords::getInsideOutsideDomain() const  {
return _insideOutsideDomain;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
_insideOutsideDomain = insideOutsideDomain;
}



 tarch::la::Vector<TWO_POWER_D,int> peano::grid::tests::records::TestVertex::PersistentRecords::getAdjacentRanks() const  {
return _adjacentRanks;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setAdjacentRanks(const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks)  {
_adjacentRanks = (adjacentRanks);
}



 bool peano::grid::tests::records::TestVertex::PersistentRecords::getAdjacentSubtreeForksIntoOtherRank() const  {
return _adjacentSubtreeForksIntoOtherRank;
}



 void peano::grid::tests::records::TestVertex::PersistentRecords::setAdjacentSubtreeForksIntoOtherRank(const bool& adjacentSubtreeForksIntoOtherRank)  {
_adjacentSubtreeForksIntoOtherRank = adjacentSubtreeForksIntoOtherRank;
}


peano::grid::tests::records::TestVertex::TestVertex() {

}


peano::grid::tests::records::TestVertex::TestVertex(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._isHangingNode, persistentRecords._refinementControl, persistentRecords._adjacentCellsHeight, persistentRecords._insideOutsideDomain, persistentRecords._adjacentRanks, persistentRecords._adjacentSubtreeForksIntoOtherRank) {

}


peano::grid::tests::records::TestVertex::TestVertex(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, adjacentRanks, adjacentSubtreeForksIntoOtherRank) {

}


peano::grid::tests::records::TestVertex::TestVertex(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const int& adjacentCellsHeightOfPreviousIteration, const int& numberOfAdjacentRefinedCells, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, adjacentRanks, adjacentSubtreeForksIntoOtherRank),_adjacentCellsHeightOfPreviousIteration(adjacentCellsHeightOfPreviousIteration),
_numberOfAdjacentRefinedCells(numberOfAdjacentRefinedCells) {

}

peano::grid::tests::records::TestVertex::~TestVertex() { }


 bool peano::grid::tests::records::TestVertex::getIsHangingNode() const  {
return _persistentRecords._isHangingNode;
}



 void peano::grid::tests::records::TestVertex::setIsHangingNode(const bool& isHangingNode)  {
_persistentRecords._isHangingNode = isHangingNode;
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertex::getRefinementControl() const  {
return _persistentRecords._refinementControl;
}



 void peano::grid::tests::records::TestVertex::setRefinementControl(const RefinementControl& refinementControl)  {
_persistentRecords._refinementControl = refinementControl;
}



 int peano::grid::tests::records::TestVertex::getAdjacentCellsHeight() const  {
return _persistentRecords._adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertex::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_persistentRecords._adjacentCellsHeight = adjacentCellsHeight;
}



 int peano::grid::tests::records::TestVertex::getAdjacentCellsHeightOfPreviousIteration() const  {
return _adjacentCellsHeightOfPreviousIteration;
}



 void peano::grid::tests::records::TestVertex::setAdjacentCellsHeightOfPreviousIteration(const int& adjacentCellsHeightOfPreviousIteration)  {
_adjacentCellsHeightOfPreviousIteration = adjacentCellsHeightOfPreviousIteration;
}



 int peano::grid::tests::records::TestVertex::getNumberOfAdjacentRefinedCells() const  {
return _numberOfAdjacentRefinedCells;
}



 void peano::grid::tests::records::TestVertex::setNumberOfAdjacentRefinedCells(const int& numberOfAdjacentRefinedCells)  {
_numberOfAdjacentRefinedCells = numberOfAdjacentRefinedCells;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertex::getInsideOutsideDomain() const  {
return _persistentRecords._insideOutsideDomain;
}



 void peano::grid::tests::records::TestVertex::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
_persistentRecords._insideOutsideDomain = insideOutsideDomain;
}



 tarch::la::Vector<TWO_POWER_D,int> peano::grid::tests::records::TestVertex::getAdjacentRanks() const  {
return _persistentRecords._adjacentRanks;
}



 void peano::grid::tests::records::TestVertex::setAdjacentRanks(const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks)  {
_persistentRecords._adjacentRanks = (adjacentRanks);
}



 int peano::grid::tests::records::TestVertex::getAdjacentRanks(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<TWO_POWER_D);
return _persistentRecords._adjacentRanks[elementIndex];

}



 void peano::grid::tests::records::TestVertex::setAdjacentRanks(int elementIndex, const int& adjacentRanks)  {
assertion(elementIndex>=0);
assertion(elementIndex<TWO_POWER_D);
_persistentRecords._adjacentRanks[elementIndex]= adjacentRanks;

}



 bool peano::grid::tests::records::TestVertex::getAdjacentSubtreeForksIntoOtherRank() const  {
return _persistentRecords._adjacentSubtreeForksIntoOtherRank;
}



 void peano::grid::tests::records::TestVertex::setAdjacentSubtreeForksIntoOtherRank(const bool& adjacentSubtreeForksIntoOtherRank)  {
_persistentRecords._adjacentSubtreeForksIntoOtherRank = adjacentSubtreeForksIntoOtherRank;
}


std::string peano::grid::tests::records::TestVertex::toString(const InsideOutsideDomain& param) {
switch (param) {
case Inside: return "Inside";
case Boundary: return "Boundary";
case Outside: return "Outside";
}
return "undefined";
}

std::string peano::grid::tests::records::TestVertex::getInsideOutsideDomainMapping() {
return "InsideOutsideDomain(Inside=0,Boundary=1,Outside=2)";
}
std::string peano::grid::tests::records::TestVertex::toString(const RefinementControl& param) {
switch (param) {
case Unrefined: return "Unrefined";
case Refined: return "Refined";
case RefinementTriggered: return "RefinementTriggered";
case Refining: return "Refining";
case EraseTriggered: return "EraseTriggered";
case Erasing: return "Erasing";
case RefineDueToJoinThoughWorkerIsAlreadyErasing: return "RefineDueToJoinThoughWorkerIsAlreadyErasing";
case EnforceRefinementTriggered: return "EnforceRefinementTriggered";
}
return "undefined";
}

std::string peano::grid::tests::records::TestVertex::getRefinementControlMapping() {
return "RefinementControl(Unrefined=0,Refined=1,RefinementTriggered=2,Refining=3,EraseTriggered=4,Erasing=5,RefineDueToJoinThoughWorkerIsAlreadyErasing=6,EnforceRefinementTriggered=7)";
}


std::string peano::grid::tests::records::TestVertex::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestVertex::toString (std::ostream& out) const {
out << "("; 
out << "isHangingNode:" << getIsHangingNode();
out << ",";
out << "refinementControl:" << toString(getRefinementControl());
out << ",";
out << "adjacentCellsHeight:" << getAdjacentCellsHeight();
out << ",";
out << "adjacentCellsHeightOfPreviousIteration:" << getAdjacentCellsHeightOfPreviousIteration();
out << ",";
out << "numberOfAdjacentRefinedCells:" << getNumberOfAdjacentRefinedCells();
out << ",";
out << "insideOutsideDomain:" << toString(getInsideOutsideDomain());
out << ",";
out << "adjacentRanks:[";
   for (int i = 0; i < TWO_POWER_D-1; i++) {
      out << getAdjacentRanks(i) << ",";
   }
   out << getAdjacentRanks(TWO_POWER_D-1) << "]";
out << ",";
out << "adjacentSubtreeForksIntoOtherRank:" << getAdjacentSubtreeForksIntoOtherRank();
out <<  ")";
}


peano::grid::tests::records::TestVertex::PersistentRecords peano::grid::tests::records::TestVertex::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestVertexPacked peano::grid::tests::records::TestVertex::convert() const{
return TestVertexPacked(
getIsHangingNode(),
getRefinementControl(),
getAdjacentCellsHeight(),
getAdjacentCellsHeightOfPreviousIteration(),
getNumberOfAdjacentRefinedCells(),
getInsideOutsideDomain(),
getAdjacentRanks(),
getAdjacentSubtreeForksIntoOtherRank()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestVertex::_log( "peano::grid::tests::records::TestVertex" );

MPI_Datatype peano::grid::tests::records::TestVertex::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestVertex::FullDatatype = 0;


void peano::grid::tests::records::TestVertex::initDatatype() {
{
TestVertex dummyTestVertex[2];

const int Attributes = 6;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isHangingNode
MPI_INT,		 //refinementControl
MPI_INT,		 //adjacentRanks
MPI_CHAR,		 //adjacentSubtreeForksIntoOtherRank
MPI_INT,		 //numberOfAdjacentRefinedCells
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isHangingNode
1,		 //refinementControl
TWO_POWER_D,		 //adjacentRanks
1,		 //adjacentSubtreeForksIntoOtherRank
1,		 //numberOfAdjacentRefinedCells
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._isHangingNode))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._refinementControl))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentRanks[0]))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentSubtreeForksIntoOtherRank))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._numberOfAdjacentRefinedCells))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[1]._persistentRecords._isHangingNode))), 		&disp[5] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertex::Datatype );
MPI_Type_commit( &TestVertex::Datatype );

}
{
TestVertex dummyTestVertex[2];

const int Attributes = 9;
MPI_Datatype subtypes[Attributes] = {
MPI_CHAR,		 //isHangingNode
MPI_INT,		 //refinementControl
MPI_INT,		 //adjacentCellsHeight
MPI_INT,		 //insideOutsideDomain
MPI_INT,		 //adjacentRanks
MPI_CHAR,		 //adjacentSubtreeForksIntoOtherRank
MPI_INT,		 //adjacentCellsHeightOfPreviousIteration
MPI_INT,		 //numberOfAdjacentRefinedCells
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //isHangingNode
1,		 //refinementControl
1,		 //adjacentCellsHeight
1,		 //insideOutsideDomain
TWO_POWER_D,		 //adjacentRanks
1,		 //adjacentSubtreeForksIntoOtherRank
1,		 //adjacentCellsHeightOfPreviousIteration
1,		 //numberOfAdjacentRefinedCells
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._isHangingNode))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._refinementControl))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentCellsHeight))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._insideOutsideDomain))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentRanks[0]))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._persistentRecords._adjacentSubtreeForksIntoOtherRank))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._adjacentCellsHeightOfPreviousIteration))), 		&disp[6] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[0]._numberOfAdjacentRefinedCells))), 		&disp[7] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertex[1]._persistentRecords._isHangingNode))), 		&disp[8] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertex::FullDatatype );
MPI_Type_commit( &TestVertex::FullDatatype );

}

}


void peano::grid::tests::records::TestVertex::shutdownDatatype() {
MPI_Type_free( &TestVertex::Datatype );
MPI_Type_free( &TestVertex::FullDatatype );

}

void peano::grid::tests::records::TestVertex::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestVertex "
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
msg << "was not able to send message peano::grid::tests::records::TestVertex "
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
msg << "testing for finished send task for peano::grid::tests::records::TestVertex "
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
"peano::grid::tests::records::TestVertex",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestVertex",
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



void peano::grid::tests::records::TestVertex::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestVertex from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestVertex from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestVertex failed: "
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
"peano::grid::tests::records::TestVertex",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestVertex",
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



bool peano::grid::tests::records::TestVertex::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestVertex::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif


peano::grid::tests::records::TestVertexPacked::PersistentRecords::PersistentRecords() {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::PersistentRecords::PersistentRecords(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_adjacentCellsHeight(adjacentCellsHeight),
_adjacentRanks(adjacentRanks),
_adjacentSubtreeForksIntoOtherRank(adjacentSubtreeForksIntoOtherRank) {
setIsHangingNode(isHangingNode);
setRefinementControl(refinementControl);
setInsideOutsideDomain(insideOutsideDomain);
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


 bool peano::grid::tests::records::TestVertexPacked::PersistentRecords::getIsHangingNode() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setIsHangingNode(const bool& isHangingNode)  {
int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( isHangingNode ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertexPacked::PersistentRecords::getRefinementControl() const  {
int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 7));
   return (RefinementControl) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setRefinementControl(const RefinementControl& refinementControl)  {
assertion((refinementControl >= 0 && refinementControl <= 7));
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(refinementControl) << (1));
}



 int peano::grid::tests::records::TestVertexPacked::PersistentRecords::getAdjacentCellsHeight() const  {
return _adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_adjacentCellsHeight = adjacentCellsHeight;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertexPacked::PersistentRecords::getInsideOutsideDomain() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   int tmp = static_cast<int>(_packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (4));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (InsideOutsideDomain) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
assertion((insideOutsideDomain >= 0 && insideOutsideDomain <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   _packedRecords0 = static_cast<int>(_packedRecords0 & ~mask);
   _packedRecords0 = static_cast<int>(_packedRecords0 | static_cast<int>(insideOutsideDomain) << (4));
}



 tarch::la::Vector<TWO_POWER_D,int> peano::grid::tests::records::TestVertexPacked::PersistentRecords::getAdjacentRanks() const  {
return _adjacentRanks;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setAdjacentRanks(const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks)  {
_adjacentRanks = (adjacentRanks);
}



 bool peano::grid::tests::records::TestVertexPacked::PersistentRecords::getAdjacentSubtreeForksIntoOtherRank() const  {
return _adjacentSubtreeForksIntoOtherRank;
}



 void peano::grid::tests::records::TestVertexPacked::PersistentRecords::setAdjacentSubtreeForksIntoOtherRank(const bool& adjacentSubtreeForksIntoOtherRank)  {
_adjacentSubtreeForksIntoOtherRank = adjacentSubtreeForksIntoOtherRank;
}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked() {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords.getIsHangingNode(), persistentRecords.getRefinementControl(), persistentRecords._adjacentCellsHeight, persistentRecords.getInsideOutsideDomain(), persistentRecords._adjacentRanks, persistentRecords._adjacentSubtreeForksIntoOtherRank) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, adjacentRanks, adjacentSubtreeForksIntoOtherRank) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}


peano::grid::tests::records::TestVertexPacked::TestVertexPacked(const bool& isHangingNode, const RefinementControl& refinementControl, const int& adjacentCellsHeight, const int& adjacentCellsHeightOfPreviousIteration, const int& numberOfAdjacentRefinedCells, const InsideOutsideDomain& insideOutsideDomain, const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks, const bool& adjacentSubtreeForksIntoOtherRank):
_persistentRecords(isHangingNode, refinementControl, adjacentCellsHeight, insideOutsideDomain, adjacentRanks, adjacentSubtreeForksIntoOtherRank),_adjacentCellsHeightOfPreviousIteration(adjacentCellsHeightOfPreviousIteration),
_numberOfAdjacentRefinedCells(numberOfAdjacentRefinedCells) {
if ((6 >= (8 * sizeof(int)))) {
std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
}
assertion((6 < (8 * sizeof(int))));

}

peano::grid::tests::records::TestVertexPacked::~TestVertexPacked() { }


 bool peano::grid::tests::records::TestVertexPacked::getIsHangingNode() const  {
int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
}



 void peano::grid::tests::records::TestVertexPacked::setIsHangingNode(const bool& isHangingNode)  {
int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( isHangingNode ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
}



 peano::grid::tests::records::TestVertex::RefinementControl peano::grid::tests::records::TestVertexPacked::getRefinementControl() const  {
int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (1));
   assertion(( tmp >= 0 &&  tmp <= 7));
   return (RefinementControl) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::setRefinementControl(const RefinementControl& refinementControl)  {
assertion((refinementControl >= 0 && refinementControl <= 7));
   int mask =  (1 << (3)) - 1;
   mask = static_cast<int>(mask << (1));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(refinementControl) << (1));
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentCellsHeight() const  {
return _persistentRecords._adjacentCellsHeight;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentCellsHeight(const int& adjacentCellsHeight)  {
_persistentRecords._adjacentCellsHeight = adjacentCellsHeight;
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentCellsHeightOfPreviousIteration() const  {
return _adjacentCellsHeightOfPreviousIteration;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentCellsHeightOfPreviousIteration(const int& adjacentCellsHeightOfPreviousIteration)  {
_adjacentCellsHeightOfPreviousIteration = adjacentCellsHeightOfPreviousIteration;
}



 int peano::grid::tests::records::TestVertexPacked::getNumberOfAdjacentRefinedCells() const  {
return _numberOfAdjacentRefinedCells;
}



 void peano::grid::tests::records::TestVertexPacked::setNumberOfAdjacentRefinedCells(const int& numberOfAdjacentRefinedCells)  {
_numberOfAdjacentRefinedCells = numberOfAdjacentRefinedCells;
}



 peano::grid::tests::records::TestVertex::InsideOutsideDomain peano::grid::tests::records::TestVertexPacked::getInsideOutsideDomain() const  {
int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   tmp = static_cast<int>(tmp >> (4));
   assertion(( tmp >= 0 &&  tmp <= 2));
   return (InsideOutsideDomain) tmp;
}



 void peano::grid::tests::records::TestVertexPacked::setInsideOutsideDomain(const InsideOutsideDomain& insideOutsideDomain)  {
assertion((insideOutsideDomain >= 0 && insideOutsideDomain <= 2));
   int mask =  (1 << (2)) - 1;
   mask = static_cast<int>(mask << (4));
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 & ~mask);
   _persistentRecords._packedRecords0 = static_cast<int>(_persistentRecords._packedRecords0 | static_cast<int>(insideOutsideDomain) << (4));
}



 tarch::la::Vector<TWO_POWER_D,int> peano::grid::tests::records::TestVertexPacked::getAdjacentRanks() const  {
return _persistentRecords._adjacentRanks;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentRanks(const tarch::la::Vector<TWO_POWER_D,int>& adjacentRanks)  {
_persistentRecords._adjacentRanks = (adjacentRanks);
}



 int peano::grid::tests::records::TestVertexPacked::getAdjacentRanks(int elementIndex) const  {
assertion(elementIndex>=0);
assertion(elementIndex<TWO_POWER_D);
return _persistentRecords._adjacentRanks[elementIndex];

}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentRanks(int elementIndex, const int& adjacentRanks)  {
assertion(elementIndex>=0);
assertion(elementIndex<TWO_POWER_D);
_persistentRecords._adjacentRanks[elementIndex]= adjacentRanks;

}



 bool peano::grid::tests::records::TestVertexPacked::getAdjacentSubtreeForksIntoOtherRank() const  {
return _persistentRecords._adjacentSubtreeForksIntoOtherRank;
}



 void peano::grid::tests::records::TestVertexPacked::setAdjacentSubtreeForksIntoOtherRank(const bool& adjacentSubtreeForksIntoOtherRank)  {
_persistentRecords._adjacentSubtreeForksIntoOtherRank = adjacentSubtreeForksIntoOtherRank;
}


std::string peano::grid::tests::records::TestVertexPacked::toString(const InsideOutsideDomain& param) {
return peano::grid::tests::records::TestVertex::toString(param);
}

std::string peano::grid::tests::records::TestVertexPacked::getInsideOutsideDomainMapping() {
return peano::grid::tests::records::TestVertex::getInsideOutsideDomainMapping();
}

std::string peano::grid::tests::records::TestVertexPacked::toString(const RefinementControl& param) {
return peano::grid::tests::records::TestVertex::toString(param);
}

std::string peano::grid::tests::records::TestVertexPacked::getRefinementControlMapping() {
return peano::grid::tests::records::TestVertex::getRefinementControlMapping();
}



std::string peano::grid::tests::records::TestVertexPacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void peano::grid::tests::records::TestVertexPacked::toString (std::ostream& out) const {
out << "("; 
out << "isHangingNode:" << getIsHangingNode();
out << ",";
out << "refinementControl:" << toString(getRefinementControl());
out << ",";
out << "adjacentCellsHeight:" << getAdjacentCellsHeight();
out << ",";
out << "adjacentCellsHeightOfPreviousIteration:" << getAdjacentCellsHeightOfPreviousIteration();
out << ",";
out << "numberOfAdjacentRefinedCells:" << getNumberOfAdjacentRefinedCells();
out << ",";
out << "insideOutsideDomain:" << toString(getInsideOutsideDomain());
out << ",";
out << "adjacentRanks:[";
   for (int i = 0; i < TWO_POWER_D-1; i++) {
      out << getAdjacentRanks(i) << ",";
   }
   out << getAdjacentRanks(TWO_POWER_D-1) << "]";
out << ",";
out << "adjacentSubtreeForksIntoOtherRank:" << getAdjacentSubtreeForksIntoOtherRank();
out <<  ")";
}


peano::grid::tests::records::TestVertexPacked::PersistentRecords peano::grid::tests::records::TestVertexPacked::getPersistentRecords() const {
return _persistentRecords;
}

peano::grid::tests::records::TestVertex peano::grid::tests::records::TestVertexPacked::convert() const{
return TestVertex(
getIsHangingNode(),
getRefinementControl(),
getAdjacentCellsHeight(),
getAdjacentCellsHeightOfPreviousIteration(),
getNumberOfAdjacentRefinedCells(),
getInsideOutsideDomain(),
getAdjacentRanks(),
getAdjacentSubtreeForksIntoOtherRank()
);
}

#ifdef Parallel
tarch::logging::Log peano::grid::tests::records::TestVertexPacked::_log( "peano::grid::tests::records::TestVertexPacked" );

MPI_Datatype peano::grid::tests::records::TestVertexPacked::Datatype = 0;
MPI_Datatype peano::grid::tests::records::TestVertexPacked::FullDatatype = 0;


void peano::grid::tests::records::TestVertexPacked::initDatatype() {
{
TestVertexPacked dummyTestVertexPacked[2];

const int Attributes = 5;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //adjacentRanks
MPI_CHAR,		 //adjacentSubtreeForksIntoOtherRank
MPI_INT,		 //_packedRecords0
MPI_INT,		 //numberOfAdjacentRefinedCells
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
TWO_POWER_D,		 //adjacentRanks
1,		 //adjacentSubtreeForksIntoOtherRank
1,		 //_packedRecords0
1,		 //numberOfAdjacentRefinedCells
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentRanks[0]))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentSubtreeForksIntoOtherRank))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._packedRecords0))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._numberOfAdjacentRefinedCells))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyTestVertexPacked[1]._persistentRecords._adjacentRanks[0])), 		&disp[4] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertexPacked::Datatype );
MPI_Type_commit( &TestVertexPacked::Datatype );

}
{
TestVertexPacked dummyTestVertexPacked[2];

const int Attributes = 7;
MPI_Datatype subtypes[Attributes] = {
MPI_INT,		 //adjacentCellsHeight
MPI_INT,		 //adjacentRanks
MPI_CHAR,		 //adjacentSubtreeForksIntoOtherRank
MPI_INT,		 //_packedRecords0
MPI_INT,		 //adjacentCellsHeightOfPreviousIteration
MPI_INT,		 //numberOfAdjacentRefinedCells
MPI_UB		 // end/displacement flag
};

int blocklen[Attributes] = {
1,		 //adjacentCellsHeight
TWO_POWER_D,		 //adjacentRanks
1,		 //adjacentSubtreeForksIntoOtherRank
1,		 //_packedRecords0
1,		 //adjacentCellsHeightOfPreviousIteration
1,		 //numberOfAdjacentRefinedCells
1		 // end/displacement flag
};

MPI_Aint     disp[Attributes];

MPI_Aint base;
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]))), &base);
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentCellsHeight))), 		&disp[0] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentRanks[0]))), 		&disp[1] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._adjacentSubtreeForksIntoOtherRank))), 		&disp[2] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._persistentRecords._packedRecords0))), 		&disp[3] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._adjacentCellsHeightOfPreviousIteration))), 		&disp[4] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[0]._numberOfAdjacentRefinedCells))), 		&disp[5] );
MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestVertexPacked[1]._persistentRecords._adjacentCellsHeight))), 		&disp[6] );

for (int i=1; i<Attributes; i++) {
assertion1( disp[i] > disp[i-1], i );
}
for (int i=0; i<Attributes; i++) {
disp[i] -= base;
}
MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestVertexPacked::FullDatatype );
MPI_Type_commit( &TestVertexPacked::FullDatatype );

}

}


void peano::grid::tests::records::TestVertexPacked::shutdownDatatype() {
MPI_Type_free( &TestVertexPacked::Datatype );
MPI_Type_free( &TestVertexPacked::FullDatatype );

}

void peano::grid::tests::records::TestVertexPacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
_senderDestinationRank = destination;

if (communicateSleep<0) {

const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
if  (result!=MPI_SUCCESS) {
std::ostringstream msg;
msg << "was not able to send message peano::grid::tests::records::TestVertexPacked "
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
msg << "was not able to send message peano::grid::tests::records::TestVertexPacked "
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
msg << "testing for finished send task for peano::grid::tests::records::TestVertexPacked "
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
"peano::grid::tests::records::TestVertexPacked",
"send(int)", destination,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestVertexPacked",
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



void peano::grid::tests::records::TestVertexPacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
if (communicateSleep<0) {

MPI_Status  status;
const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
_senderDestinationRank = status.MPI_SOURCE;
if ( result != MPI_SUCCESS ) {
std::ostringstream msg;
msg << "failed to start to receive peano::grid::tests::records::TestVertexPacked from node "
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
msg << "failed to start to receive peano::grid::tests::records::TestVertexPacked from node "
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
msg << "testing for finished receive task for peano::grid::tests::records::TestVertexPacked failed: "
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
"peano::grid::tests::records::TestVertexPacked",
"receive(int)", source,tag,1
);
triggeredTimeoutWarning = true;
}
if (
tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
(clock()>timeOutShutdown)
) {
tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
"peano::grid::tests::records::TestVertexPacked",
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



bool peano::grid::tests::records::TestVertexPacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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

int peano::grid::tests::records::TestVertexPacked::getSenderRank() const {
assertion( _senderDestinationRank!=-1 );
return _senderDestinationRank;

}
#endif




#endif


