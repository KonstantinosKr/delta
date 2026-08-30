#include "peano/grid/tests/records/TestState.h"

#if defined(Parallel)
   peano::grid::tests::records::TestState::PersistentRecords::PersistentRecords() {
      
   }
   
   
   peano::grid::tests::records::TestState::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted, const LoadBalancingState& loadRebalancingState, const bool& reduceStateAndCell):
   _minMeshWidth(minMeshWidth),
   _maxMeshWidth(maxMeshWidth),
   _numberOfInnerVertices(numberOfInnerVertices),
   _numberOfBoundaryVertices(numberOfBoundaryVertices),
   _numberOfOuterVertices(numberOfOuterVertices),
   _numberOfInnerCells(numberOfInnerCells),
   _numberOfOuterCells(numberOfOuterCells),
   _maxLevel(maxLevel),
   _hasRefined(hasRefined),
   _hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
   _hasErased(hasErased),
   _hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
   _hasChangedVertexOrCellState(hasChangedVertexOrCellState),
   _isTraversalInverted(isTraversalInverted),
   _loadRebalancingState(loadRebalancingState),
   _reduceStateAndCell(reduceStateAndCell) {
      
   }
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestState::PersistentRecords::getMinMeshWidth() const  {
      return _minMeshWidth;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setMinMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth)  {
      _minMeshWidth = (minMeshWidth);
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestState::PersistentRecords::getMaxMeshWidth() const  {
      return _maxMeshWidth;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setMaxMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth)  {
      _maxMeshWidth = (maxMeshWidth);
   }
   
   
   
    double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfInnerVertices() const  {
      return _numberOfInnerVertices;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfInnerVertices(const double& numberOfInnerVertices)  {
      _numberOfInnerVertices = numberOfInnerVertices;
   }
   
   
   
    double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfBoundaryVertices() const  {
      return _numberOfBoundaryVertices;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfBoundaryVertices(const double& numberOfBoundaryVertices)  {
      _numberOfBoundaryVertices = numberOfBoundaryVertices;
   }
   
   
   
    double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfOuterVertices() const  {
      return _numberOfOuterVertices;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfOuterVertices(const double& numberOfOuterVertices)  {
      _numberOfOuterVertices = numberOfOuterVertices;
   }
   
   
   
    double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfInnerCells() const  {
      return _numberOfInnerCells;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfInnerCells(const double& numberOfInnerCells)  {
      _numberOfInnerCells = numberOfInnerCells;
   }
   
   
   
    double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfOuterCells() const  {
      return _numberOfOuterCells;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfOuterCells(const double& numberOfOuterCells)  {
      _numberOfOuterCells = numberOfOuterCells;
   }
   
   
   
    int peano::grid::tests::records::TestState::PersistentRecords::getMaxLevel() const  {
      return _maxLevel;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setMaxLevel(const int& maxLevel)  {
      _maxLevel = maxLevel;
   }
   
   
   
    bool peano::grid::tests::records::TestState::PersistentRecords::getHasRefined() const  {
      return _hasRefined;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setHasRefined(const bool& hasRefined)  {
      _hasRefined = hasRefined;
   }
   
   
   
    bool peano::grid::tests::records::TestState::PersistentRecords::getHasTriggeredRefinementForNextIteration() const  {
      return _hasTriggeredRefinementForNextIteration;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setHasTriggeredRefinementForNextIteration(const bool& hasTriggeredRefinementForNextIteration)  {
      _hasTriggeredRefinementForNextIteration = hasTriggeredRefinementForNextIteration;
   }
   
   
   
    bool peano::grid::tests::records::TestState::PersistentRecords::getHasErased() const  {
      return _hasErased;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setHasErased(const bool& hasErased)  {
      _hasErased = hasErased;
   }
   
   
   
    bool peano::grid::tests::records::TestState::PersistentRecords::getHasTriggeredEraseForNextIteration() const  {
      return _hasTriggeredEraseForNextIteration;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setHasTriggeredEraseForNextIteration(const bool& hasTriggeredEraseForNextIteration)  {
      _hasTriggeredEraseForNextIteration = hasTriggeredEraseForNextIteration;
   }
   
   
   
    bool peano::grid::tests::records::TestState::PersistentRecords::getHasChangedVertexOrCellState() const  {
      return _hasChangedVertexOrCellState;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setHasChangedVertexOrCellState(const bool& hasChangedVertexOrCellState)  {
      _hasChangedVertexOrCellState = hasChangedVertexOrCellState;
   }
   
   
   
    bool peano::grid::tests::records::TestState::PersistentRecords::getIsTraversalInverted() const  {
      return _isTraversalInverted;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setIsTraversalInverted(const bool& isTraversalInverted)  {
      _isTraversalInverted = isTraversalInverted;
   }
   
   
   
    peano::grid::tests::records::TestState::LoadBalancingState peano::grid::tests::records::TestState::PersistentRecords::getLoadRebalancingState() const  {
      return _loadRebalancingState;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setLoadRebalancingState(const LoadBalancingState& loadRebalancingState)  {
      _loadRebalancingState = loadRebalancingState;
   }
   
   
   
    bool peano::grid::tests::records::TestState::PersistentRecords::getReduceStateAndCell() const  {
      return _reduceStateAndCell;
   }
   
   
   
    void peano::grid::tests::records::TestState::PersistentRecords::setReduceStateAndCell(const bool& reduceStateAndCell)  {
      _reduceStateAndCell = reduceStateAndCell;
   }
   
   
   peano::grid::tests::records::TestState::TestState() {
      
   }
   
   
   peano::grid::tests::records::TestState::TestState(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._maxLevel, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._isTraversalInverted, persistentRecords._loadRebalancingState, persistentRecords._reduceStateAndCell) {
      
   }
   
   
   peano::grid::tests::records::TestState::TestState(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted, const LoadBalancingState& loadRebalancingState, const bool& reduceStateAndCell):
   _persistentRecords(minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, isTraversalInverted, loadRebalancingState, reduceStateAndCell) {
      
   }
   
   
   peano::grid::tests::records::TestState::TestState(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted, const LoadBalancingState& loadRebalancingState, const bool& reduceStateAndCell, const bool& hasWorkerWithWorker):
   _persistentRecords(minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, isTraversalInverted, loadRebalancingState, reduceStateAndCell),_hasWorkerWithWorker(hasWorkerWithWorker) {
      
   }
   
   peano::grid::tests::records::TestState::~TestState() { }
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestState::getMinMeshWidth() const  {
      return _persistentRecords._minMeshWidth;
   }
   
   
   
    void peano::grid::tests::records::TestState::setMinMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth)  {
      _persistentRecords._minMeshWidth = (minMeshWidth);
   }
   
   
   
    double peano::grid::tests::records::TestState::getMinMeshWidth(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      return _persistentRecords._minMeshWidth[elementIndex];
      
   }
   
   
   
    void peano::grid::tests::records::TestState::setMinMeshWidth(int elementIndex, const double& minMeshWidth)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._minMeshWidth[elementIndex]= minMeshWidth;
      
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestState::getMaxMeshWidth() const  {
      return _persistentRecords._maxMeshWidth;
   }
   
   
   
    void peano::grid::tests::records::TestState::setMaxMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth)  {
      _persistentRecords._maxMeshWidth = (maxMeshWidth);
   }
   
   
   
    double peano::grid::tests::records::TestState::getMaxMeshWidth(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      return _persistentRecords._maxMeshWidth[elementIndex];
      
   }
   
   
   
    void peano::grid::tests::records::TestState::setMaxMeshWidth(int elementIndex, const double& maxMeshWidth)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._maxMeshWidth[elementIndex]= maxMeshWidth;
      
   }
   
   
   
    double peano::grid::tests::records::TestState::getNumberOfInnerVertices() const  {
      return _persistentRecords._numberOfInnerVertices;
   }
   
   
   
    void peano::grid::tests::records::TestState::setNumberOfInnerVertices(const double& numberOfInnerVertices)  {
      _persistentRecords._numberOfInnerVertices = numberOfInnerVertices;
   }
   
   
   
    double peano::grid::tests::records::TestState::getNumberOfBoundaryVertices() const  {
      return _persistentRecords._numberOfBoundaryVertices;
   }
   
   
   
    void peano::grid::tests::records::TestState::setNumberOfBoundaryVertices(const double& numberOfBoundaryVertices)  {
      _persistentRecords._numberOfBoundaryVertices = numberOfBoundaryVertices;
   }
   
   
   
    double peano::grid::tests::records::TestState::getNumberOfOuterVertices() const  {
      return _persistentRecords._numberOfOuterVertices;
   }
   
   
   
    void peano::grid::tests::records::TestState::setNumberOfOuterVertices(const double& numberOfOuterVertices)  {
      _persistentRecords._numberOfOuterVertices = numberOfOuterVertices;
   }
   
   
   
    double peano::grid::tests::records::TestState::getNumberOfInnerCells() const  {
      return _persistentRecords._numberOfInnerCells;
   }
   
   
   
    void peano::grid::tests::records::TestState::setNumberOfInnerCells(const double& numberOfInnerCells)  {
      _persistentRecords._numberOfInnerCells = numberOfInnerCells;
   }
   
   
   
    double peano::grid::tests::records::TestState::getNumberOfOuterCells() const  {
      return _persistentRecords._numberOfOuterCells;
   }
   
   
   
    void peano::grid::tests::records::TestState::setNumberOfOuterCells(const double& numberOfOuterCells)  {
      _persistentRecords._numberOfOuterCells = numberOfOuterCells;
   }
   
   
   
    int peano::grid::tests::records::TestState::getMaxLevel() const  {
      return _persistentRecords._maxLevel;
   }
   
   
   
    void peano::grid::tests::records::TestState::setMaxLevel(const int& maxLevel)  {
      _persistentRecords._maxLevel = maxLevel;
   }
   
   
   
    bool peano::grid::tests::records::TestState::getHasRefined() const  {
      return _persistentRecords._hasRefined;
   }
   
   
   
    void peano::grid::tests::records::TestState::setHasRefined(const bool& hasRefined)  {
      _persistentRecords._hasRefined = hasRefined;
   }
   
   
   
    bool peano::grid::tests::records::TestState::getHasTriggeredRefinementForNextIteration() const  {
      return _persistentRecords._hasTriggeredRefinementForNextIteration;
   }
   
   
   
    void peano::grid::tests::records::TestState::setHasTriggeredRefinementForNextIteration(const bool& hasTriggeredRefinementForNextIteration)  {
      _persistentRecords._hasTriggeredRefinementForNextIteration = hasTriggeredRefinementForNextIteration;
   }
   
   
   
    bool peano::grid::tests::records::TestState::getHasErased() const  {
      return _persistentRecords._hasErased;
   }
   
   
   
    void peano::grid::tests::records::TestState::setHasErased(const bool& hasErased)  {
      _persistentRecords._hasErased = hasErased;
   }
   
   
   
    bool peano::grid::tests::records::TestState::getHasTriggeredEraseForNextIteration() const  {
      return _persistentRecords._hasTriggeredEraseForNextIteration;
   }
   
   
   
    void peano::grid::tests::records::TestState::setHasTriggeredEraseForNextIteration(const bool& hasTriggeredEraseForNextIteration)  {
      _persistentRecords._hasTriggeredEraseForNextIteration = hasTriggeredEraseForNextIteration;
   }
   
   
   
    bool peano::grid::tests::records::TestState::getHasChangedVertexOrCellState() const  {
      return _persistentRecords._hasChangedVertexOrCellState;
   }
   
   
   
    void peano::grid::tests::records::TestState::setHasChangedVertexOrCellState(const bool& hasChangedVertexOrCellState)  {
      _persistentRecords._hasChangedVertexOrCellState = hasChangedVertexOrCellState;
   }
   
   
   
    bool peano::grid::tests::records::TestState::getIsTraversalInverted() const  {
      return _persistentRecords._isTraversalInverted;
   }
   
   
   
    void peano::grid::tests::records::TestState::setIsTraversalInverted(const bool& isTraversalInverted)  {
      _persistentRecords._isTraversalInverted = isTraversalInverted;
   }
   
   
   
    peano::grid::tests::records::TestState::LoadBalancingState peano::grid::tests::records::TestState::getLoadRebalancingState() const  {
      return _persistentRecords._loadRebalancingState;
   }
   
   
   
    void peano::grid::tests::records::TestState::setLoadRebalancingState(const LoadBalancingState& loadRebalancingState)  {
      _persistentRecords._loadRebalancingState = loadRebalancingState;
   }
   
   
   
    bool peano::grid::tests::records::TestState::getReduceStateAndCell() const  {
      return _persistentRecords._reduceStateAndCell;
   }
   
   
   
    void peano::grid::tests::records::TestState::setReduceStateAndCell(const bool& reduceStateAndCell)  {
      _persistentRecords._reduceStateAndCell = reduceStateAndCell;
   }
   
   
   
    bool peano::grid::tests::records::TestState::getHasWorkerWithWorker() const  {
      return _hasWorkerWithWorker;
   }
   
   
   
    void peano::grid::tests::records::TestState::setHasWorkerWithWorker(const bool& hasWorkerWithWorker)  {
      _hasWorkerWithWorker = hasWorkerWithWorker;
   }
   
   
   std::string peano::grid::tests::records::TestState::toString(const LoadBalancingState& param) {
      switch (param) {
         case NoRebalancing: return "NoRebalancing";
         case ForkTriggered: return "ForkTriggered";
         case Forking: return "Forking";
         case JoinTriggered: return "JoinTriggered";
         case Joining: return "Joining";
         case JoinWithMasterTriggered: return "JoinWithMasterTriggered";
         case JoiningWithMaster: return "JoiningWithMaster";
         case HasJoinedWithMaster: return "HasJoinedWithMaster";
         case IsNewWorkerDueToForkOfExistingDomain: return "IsNewWorkerDueToForkOfExistingDomain";
      }
      return "undefined";
   }
   
   std::string peano::grid::tests::records::TestState::getLoadBalancingStateMapping() {
      return "LoadBalancingState(NoRebalancing=0,ForkTriggered=1,Forking=2,JoinTriggered=3,Joining=4,JoinWithMasterTriggered=5,JoiningWithMaster=6,HasJoinedWithMaster=7,IsNewWorkerDueToForkOfExistingDomain=8)";
   }
   
   
   std::string peano::grid::tests::records::TestState::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void peano::grid::tests::records::TestState::toString (std::ostream& out) const {
      out << "("; 
      out << "minMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMinMeshWidth(i) << ",";
   }
   out << getMinMeshWidth(DIMENSIONS-1) << "]";
      out << ",";
      out << "maxMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMaxMeshWidth(i) << ",";
   }
   out << getMaxMeshWidth(DIMENSIONS-1) << "]";
      out << ",";
      out << "numberOfInnerVertices:" << getNumberOfInnerVertices();
      out << ",";
      out << "numberOfBoundaryVertices:" << getNumberOfBoundaryVertices();
      out << ",";
      out << "numberOfOuterVertices:" << getNumberOfOuterVertices();
      out << ",";
      out << "numberOfInnerCells:" << getNumberOfInnerCells();
      out << ",";
      out << "numberOfOuterCells:" << getNumberOfOuterCells();
      out << ",";
      out << "maxLevel:" << getMaxLevel();
      out << ",";
      out << "hasRefined:" << getHasRefined();
      out << ",";
      out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
      out << ",";
      out << "hasErased:" << getHasErased();
      out << ",";
      out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
      out << ",";
      out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
      out << ",";
      out << "isTraversalInverted:" << getIsTraversalInverted();
      out << ",";
      out << "loadRebalancingState:" << toString(getLoadRebalancingState());
      out << ",";
      out << "reduceStateAndCell:" << getReduceStateAndCell();
      out << ",";
      out << "hasWorkerWithWorker:" << getHasWorkerWithWorker();
      out <<  ")";
   }
   
   
   peano::grid::tests::records::TestState::PersistentRecords peano::grid::tests::records::TestState::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   peano::grid::tests::records::TestStatePacked peano::grid::tests::records::TestState::convert() const{
      return TestStatePacked(
         getMinMeshWidth(),
         getMaxMeshWidth(),
         getNumberOfInnerVertices(),
         getNumberOfBoundaryVertices(),
         getNumberOfOuterVertices(),
         getNumberOfInnerCells(),
         getNumberOfOuterCells(),
         getMaxLevel(),
         getHasRefined(),
         getHasTriggeredRefinementForNextIteration(),
         getHasErased(),
         getHasTriggeredEraseForNextIteration(),
         getHasChangedVertexOrCellState(),
         getIsTraversalInverted(),
         getLoadRebalancingState(),
         getReduceStateAndCell(),
         getHasWorkerWithWorker()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log peano::grid::tests::records::TestState::_log( "peano::grid::tests::records::TestState" );
      
      MPI_Datatype peano::grid::tests::records::TestState::Datatype = 0;
      MPI_Datatype peano::grid::tests::records::TestState::FullDatatype = 0;
      
      
      void peano::grid::tests::records::TestState::initDatatype() {
         {
            TestState dummyTestState[2];
            
            #ifdef MPI2
            const int Attributes = 15;
            #else
            const int Attributes = 16;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_DOUBLE		 //minMeshWidth
               , MPI_DOUBLE		 //maxMeshWidth
               , MPI_DOUBLE		 //numberOfInnerVertices
               , MPI_DOUBLE		 //numberOfBoundaryVertices
               , MPI_DOUBLE		 //numberOfOuterVertices
               , MPI_DOUBLE		 //numberOfInnerCells
               , MPI_DOUBLE		 //numberOfOuterCells
               , MPI_INT		 //maxLevel
               , MPI_CXX_BOOL		 //hasRefined
               , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
               , MPI_CXX_BOOL		 //hasErased
               , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
               , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
               , MPI_CXX_BOOL		 //isTraversalInverted
               , MPI_CXX_BOOL		 //hasWorkerWithWorker
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 DIMENSIONS		 //minMeshWidth
               , DIMENSIONS		 //maxMeshWidth
               , 1		 //numberOfInnerVertices
               , 1		 //numberOfBoundaryVertices
               , 1		 //numberOfOuterVertices
               , 1		 //numberOfInnerCells
               , 1		 //numberOfOuterCells
               , 1		 //maxLevel
               , 1		 //hasRefined
               , 1		 //hasTriggeredRefinementForNextIteration
               , 1		 //hasErased
               , 1		 //hasTriggeredEraseForNextIteration
               , 1		 //hasChangedVertexOrCellState
               , 1		 //isTraversalInverted
               , 1		 //hasWorkerWithWorker
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxLevel))), 		&disp[7] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxLevel))), 		&disp[7] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasRefined))), 		&disp[8] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasRefined))), 		&disp[8] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[9] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[9] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasErased))), 		&disp[10] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasErased))), 		&disp[10] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[11] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[11] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[12] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[12] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._isTraversalInverted))), 		&disp[13] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._isTraversalInverted))), 		&disp[13] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._hasWorkerWithWorker))), 		&disp[14] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._hasWorkerWithWorker))), 		&disp[14] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(TestState)), i, disp[i], Attributes, sizeof(TestState));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[1]))), 		&disp[15] );
            disp[15] -= base;
            disp[15] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &TestState::Datatype );
            MPI_Type_commit( &TestState::Datatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestState::Datatype);
            MPI_Type_commit( &TestState::Datatype );
            #endif
            
         }
         {
            TestState dummyTestState[2];
            
            #ifdef MPI2
            const int Attributes = 17;
            #else
            const int Attributes = 18;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_DOUBLE		 //minMeshWidth
               , MPI_DOUBLE		 //maxMeshWidth
               , MPI_DOUBLE		 //numberOfInnerVertices
               , MPI_DOUBLE		 //numberOfBoundaryVertices
               , MPI_DOUBLE		 //numberOfOuterVertices
               , MPI_DOUBLE		 //numberOfInnerCells
               , MPI_DOUBLE		 //numberOfOuterCells
               , MPI_INT		 //maxLevel
               , MPI_CXX_BOOL		 //hasRefined
               , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
               , MPI_CXX_BOOL		 //hasErased
               , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
               , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
               , MPI_CXX_BOOL		 //isTraversalInverted
               , MPI_INT		 //loadRebalancingState
               , MPI_CXX_BOOL		 //reduceStateAndCell
               , MPI_CXX_BOOL		 //hasWorkerWithWorker
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 DIMENSIONS		 //minMeshWidth
               , DIMENSIONS		 //maxMeshWidth
               , 1		 //numberOfInnerVertices
               , 1		 //numberOfBoundaryVertices
               , 1		 //numberOfOuterVertices
               , 1		 //numberOfInnerCells
               , 1		 //numberOfOuterCells
               , 1		 //maxLevel
               , 1		 //hasRefined
               , 1		 //hasTriggeredRefinementForNextIteration
               , 1		 //hasErased
               , 1		 //hasTriggeredEraseForNextIteration
               , 1		 //hasChangedVertexOrCellState
               , 1		 //isTraversalInverted
               , 1		 //loadRebalancingState
               , 1		 //reduceStateAndCell
               , 1		 //hasWorkerWithWorker
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxLevel))), 		&disp[7] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxLevel))), 		&disp[7] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasRefined))), 		&disp[8] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasRefined))), 		&disp[8] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[9] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[9] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasErased))), 		&disp[10] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasErased))), 		&disp[10] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[11] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[11] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[12] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[12] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._isTraversalInverted))), 		&disp[13] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._isTraversalInverted))), 		&disp[13] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._loadRebalancingState))), 		&disp[14] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._loadRebalancingState))), 		&disp[14] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._reduceStateAndCell))), 		&disp[15] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._reduceStateAndCell))), 		&disp[15] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._hasWorkerWithWorker))), 		&disp[16] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._hasWorkerWithWorker))), 		&disp[16] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(TestState)), i, disp[i], Attributes, sizeof(TestState));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[1]))), 		&disp[17] );
            disp[17] -= base;
            disp[17] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &TestState::FullDatatype );
            MPI_Type_commit( &TestState::FullDatatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestState::FullDatatype);
            MPI_Type_commit( &TestState::FullDatatype );
            #endif
            
         }
         
      }
      
      
      void peano::grid::tests::records::TestState::shutdownDatatype() {
         MPI_Type_free( &TestState::Datatype );
         MPI_Type_free( &TestState::FullDatatype );
         
      }
      
      void peano::grid::tests::records::TestState::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
         // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message peano::grid::tests::records::TestState " 
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
         msg << "was not able to send message peano::grid::tests::records::TestState "  
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
           msg << "testing for finished send task for peano::grid::tests::records::TestState " 
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
             "peano::grid::tests::records::TestState", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "peano::grid::tests::records::TestState", 
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
      
      
      
      void peano::grid::tests::records::TestState::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive peano::grid::tests::records::TestState from node " 
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
        msg << "failed to start to receive peano::grid::tests::records::TestState from node " 
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
            "peano::grid::tests::records::TestState", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::grid::tests::records::TestState", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::grid::tests::records::TestState failed: " 
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
        msg << "testing for finished receive task for peano::grid::tests::records::TestState failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::grid::tests::records::TestState failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::grid::tests::records::TestState", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::grid::tests::records::TestState", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive peano::grid::tests::records::TestState from node " 
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
      
      
      
      bool peano::grid::tests::records::TestState::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
      
      int peano::grid::tests::records::TestState::getSenderRank() const {
         assertion( _senderDestinationRank!=-1 );
         return _senderDestinationRank;
         
      }
   #endif
   
   
   peano::grid::tests::records::TestStatePacked::PersistentRecords::PersistentRecords() {
      if ((6 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(int))));
      
   }
   
   
   peano::grid::tests::records::TestStatePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted, const LoadBalancingState& loadRebalancingState, const bool& reduceStateAndCell):
   _minMeshWidth(minMeshWidth),
   _maxMeshWidth(maxMeshWidth),
   _numberOfInnerVertices(numberOfInnerVertices),
   _numberOfBoundaryVertices(numberOfBoundaryVertices),
   _numberOfOuterVertices(numberOfOuterVertices),
   _numberOfInnerCells(numberOfInnerCells),
   _numberOfOuterCells(numberOfOuterCells),
   _maxLevel(maxLevel),
   _isTraversalInverted(isTraversalInverted),
   _loadRebalancingState(loadRebalancingState) {
      setHasRefined(hasRefined);
      setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
      setHasErased(hasErased);
      setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
      setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
      setReduceStateAndCell(reduceStateAndCell);
      if ((6 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(int))));
      
   }
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestStatePacked::PersistentRecords::getMinMeshWidth() const  {
      return _minMeshWidth;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setMinMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth)  {
      _minMeshWidth = (minMeshWidth);
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestStatePacked::PersistentRecords::getMaxMeshWidth() const  {
      return _maxMeshWidth;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setMaxMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth)  {
      _maxMeshWidth = (maxMeshWidth);
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfInnerVertices() const  {
      return _numberOfInnerVertices;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfInnerVertices(const double& numberOfInnerVertices)  {
      _numberOfInnerVertices = numberOfInnerVertices;
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfBoundaryVertices() const  {
      return _numberOfBoundaryVertices;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfBoundaryVertices(const double& numberOfBoundaryVertices)  {
      _numberOfBoundaryVertices = numberOfBoundaryVertices;
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfOuterVertices() const  {
      return _numberOfOuterVertices;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfOuterVertices(const double& numberOfOuterVertices)  {
      _numberOfOuterVertices = numberOfOuterVertices;
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfInnerCells() const  {
      return _numberOfInnerCells;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfInnerCells(const double& numberOfInnerCells)  {
      _numberOfInnerCells = numberOfInnerCells;
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfOuterCells() const  {
      return _numberOfOuterCells;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfOuterCells(const double& numberOfOuterCells)  {
      _numberOfOuterCells = numberOfOuterCells;
   }
   
   
   
    int peano::grid::tests::records::TestStatePacked::PersistentRecords::getMaxLevel() const  {
      return _maxLevel;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setMaxLevel(const int& maxLevel)  {
      _maxLevel = maxLevel;
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasRefined() const  {
      int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasRefined(const bool& hasRefined)  {
      int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( hasRefined ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasTriggeredRefinementForNextIteration() const  {
      int mask = 1 << (1);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasTriggeredRefinementForNextIteration(const bool& hasTriggeredRefinementForNextIteration)  {
      int mask = 1 << (1);
   _packedRecords0 = static_cast<int>( hasTriggeredRefinementForNextIteration ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasErased() const  {
      int mask = 1 << (2);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasErased(const bool& hasErased)  {
      int mask = 1 << (2);
   _packedRecords0 = static_cast<int>( hasErased ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasTriggeredEraseForNextIteration() const  {
      int mask = 1 << (3);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasTriggeredEraseForNextIteration(const bool& hasTriggeredEraseForNextIteration)  {
      int mask = 1 << (3);
   _packedRecords0 = static_cast<int>( hasTriggeredEraseForNextIteration ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasChangedVertexOrCellState() const  {
      int mask = 1 << (4);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasChangedVertexOrCellState(const bool& hasChangedVertexOrCellState)  {
      int mask = 1 << (4);
   _packedRecords0 = static_cast<int>( hasChangedVertexOrCellState ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getIsTraversalInverted() const  {
      return _isTraversalInverted;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setIsTraversalInverted(const bool& isTraversalInverted)  {
      _isTraversalInverted = isTraversalInverted;
   }
   
   
   
    peano::grid::tests::records::TestState::LoadBalancingState peano::grid::tests::records::TestStatePacked::PersistentRecords::getLoadRebalancingState() const  {
      return _loadRebalancingState;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setLoadRebalancingState(const LoadBalancingState& loadRebalancingState)  {
      _loadRebalancingState = loadRebalancingState;
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getReduceStateAndCell() const  {
      int mask = 1 << (5);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::PersistentRecords::setReduceStateAndCell(const bool& reduceStateAndCell)  {
      int mask = 1 << (5);
   _packedRecords0 = static_cast<int>( reduceStateAndCell ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
   }
   
   
   peano::grid::tests::records::TestStatePacked::TestStatePacked() {
      if ((1 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((1 < (8 * sizeof(int))));
      if ((6 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(int))));
      
   }
   
   
   peano::grid::tests::records::TestStatePacked::TestStatePacked(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._maxLevel, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords._isTraversalInverted, persistentRecords._loadRebalancingState, persistentRecords.getReduceStateAndCell()) {
      if ((1 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((1 < (8 * sizeof(int))));
      if ((6 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(int))));
      
   }
   
   
   peano::grid::tests::records::TestStatePacked::TestStatePacked(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted, const LoadBalancingState& loadRebalancingState, const bool& reduceStateAndCell):
   _persistentRecords(minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, isTraversalInverted, loadRebalancingState, reduceStateAndCell) {
      if ((1 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((1 < (8 * sizeof(int))));
      if ((6 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(int))));
      
   }
   
   
   peano::grid::tests::records::TestStatePacked::TestStatePacked(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted, const LoadBalancingState& loadRebalancingState, const bool& reduceStateAndCell, const bool& hasWorkerWithWorker):
   _persistentRecords(minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, isTraversalInverted, loadRebalancingState, reduceStateAndCell) {
      setHasWorkerWithWorker(hasWorkerWithWorker);
      if ((1 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((1 < (8 * sizeof(int))));
      if ((6 >= (8 * sizeof(int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(int))));
      
   }
   
   peano::grid::tests::records::TestStatePacked::~TestStatePacked() { }
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestStatePacked::getMinMeshWidth() const  {
      return _persistentRecords._minMeshWidth;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setMinMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth)  {
      _persistentRecords._minMeshWidth = (minMeshWidth);
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::getMinMeshWidth(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      return _persistentRecords._minMeshWidth[elementIndex];
      
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setMinMeshWidth(int elementIndex, const double& minMeshWidth)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._minMeshWidth[elementIndex]= minMeshWidth;
      
   }
   
   
   
    tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestStatePacked::getMaxMeshWidth() const  {
      return _persistentRecords._maxMeshWidth;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setMaxMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth)  {
      _persistentRecords._maxMeshWidth = (maxMeshWidth);
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::getMaxMeshWidth(int elementIndex) const  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      return _persistentRecords._maxMeshWidth[elementIndex];
      
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setMaxMeshWidth(int elementIndex, const double& maxMeshWidth)  {
      assertion(elementIndex>=0);
      assertion(elementIndex<DIMENSIONS);
      _persistentRecords._maxMeshWidth[elementIndex]= maxMeshWidth;
      
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::getNumberOfInnerVertices() const  {
      return _persistentRecords._numberOfInnerVertices;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setNumberOfInnerVertices(const double& numberOfInnerVertices)  {
      _persistentRecords._numberOfInnerVertices = numberOfInnerVertices;
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::getNumberOfBoundaryVertices() const  {
      return _persistentRecords._numberOfBoundaryVertices;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setNumberOfBoundaryVertices(const double& numberOfBoundaryVertices)  {
      _persistentRecords._numberOfBoundaryVertices = numberOfBoundaryVertices;
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::getNumberOfOuterVertices() const  {
      return _persistentRecords._numberOfOuterVertices;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setNumberOfOuterVertices(const double& numberOfOuterVertices)  {
      _persistentRecords._numberOfOuterVertices = numberOfOuterVertices;
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::getNumberOfInnerCells() const  {
      return _persistentRecords._numberOfInnerCells;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setNumberOfInnerCells(const double& numberOfInnerCells)  {
      _persistentRecords._numberOfInnerCells = numberOfInnerCells;
   }
   
   
   
    double peano::grid::tests::records::TestStatePacked::getNumberOfOuterCells() const  {
      return _persistentRecords._numberOfOuterCells;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setNumberOfOuterCells(const double& numberOfOuterCells)  {
      _persistentRecords._numberOfOuterCells = numberOfOuterCells;
   }
   
   
   
    int peano::grid::tests::records::TestStatePacked::getMaxLevel() const  {
      return _persistentRecords._maxLevel;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setMaxLevel(const int& maxLevel)  {
      _persistentRecords._maxLevel = maxLevel;
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::getHasRefined() const  {
      int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setHasRefined(const bool& hasRefined)  {
      int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( hasRefined ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::getHasTriggeredRefinementForNextIteration() const  {
      int mask = 1 << (1);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setHasTriggeredRefinementForNextIteration(const bool& hasTriggeredRefinementForNextIteration)  {
      int mask = 1 << (1);
   _persistentRecords._packedRecords0 = static_cast<int>( hasTriggeredRefinementForNextIteration ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::getHasErased() const  {
      int mask = 1 << (2);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setHasErased(const bool& hasErased)  {
      int mask = 1 << (2);
   _persistentRecords._packedRecords0 = static_cast<int>( hasErased ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::getHasTriggeredEraseForNextIteration() const  {
      int mask = 1 << (3);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setHasTriggeredEraseForNextIteration(const bool& hasTriggeredEraseForNextIteration)  {
      int mask = 1 << (3);
   _persistentRecords._packedRecords0 = static_cast<int>( hasTriggeredEraseForNextIteration ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::getHasChangedVertexOrCellState() const  {
      int mask = 1 << (4);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setHasChangedVertexOrCellState(const bool& hasChangedVertexOrCellState)  {
      int mask = 1 << (4);
   _persistentRecords._packedRecords0 = static_cast<int>( hasChangedVertexOrCellState ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::getIsTraversalInverted() const  {
      return _persistentRecords._isTraversalInverted;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setIsTraversalInverted(const bool& isTraversalInverted)  {
      _persistentRecords._isTraversalInverted = isTraversalInverted;
   }
   
   
   
    peano::grid::tests::records::TestState::LoadBalancingState peano::grid::tests::records::TestStatePacked::getLoadRebalancingState() const  {
      return _persistentRecords._loadRebalancingState;
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setLoadRebalancingState(const LoadBalancingState& loadRebalancingState)  {
      _persistentRecords._loadRebalancingState = loadRebalancingState;
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::getReduceStateAndCell() const  {
      int mask = 1 << (5);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setReduceStateAndCell(const bool& reduceStateAndCell)  {
      int mask = 1 << (5);
   _persistentRecords._packedRecords0 = static_cast<int>( reduceStateAndCell ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
   }
   
   
   
    bool peano::grid::tests::records::TestStatePacked::getHasWorkerWithWorker() const  {
      int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
   }
   
   
   
    void peano::grid::tests::records::TestStatePacked::setHasWorkerWithWorker(const bool& hasWorkerWithWorker)  {
      int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( hasWorkerWithWorker ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
   }
   
   
   std::string peano::grid::tests::records::TestStatePacked::toString(const LoadBalancingState& param) {
      return peano::grid::tests::records::TestState::toString(param);
   }
   
   std::string peano::grid::tests::records::TestStatePacked::getLoadBalancingStateMapping() {
      return peano::grid::tests::records::TestState::getLoadBalancingStateMapping();
   }
   
   
   
   std::string peano::grid::tests::records::TestStatePacked::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void peano::grid::tests::records::TestStatePacked::toString (std::ostream& out) const {
      out << "("; 
      out << "minMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMinMeshWidth(i) << ",";
   }
   out << getMinMeshWidth(DIMENSIONS-1) << "]";
      out << ",";
      out << "maxMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMaxMeshWidth(i) << ",";
   }
   out << getMaxMeshWidth(DIMENSIONS-1) << "]";
      out << ",";
      out << "numberOfInnerVertices:" << getNumberOfInnerVertices();
      out << ",";
      out << "numberOfBoundaryVertices:" << getNumberOfBoundaryVertices();
      out << ",";
      out << "numberOfOuterVertices:" << getNumberOfOuterVertices();
      out << ",";
      out << "numberOfInnerCells:" << getNumberOfInnerCells();
      out << ",";
      out << "numberOfOuterCells:" << getNumberOfOuterCells();
      out << ",";
      out << "maxLevel:" << getMaxLevel();
      out << ",";
      out << "hasRefined:" << getHasRefined();
      out << ",";
      out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
      out << ",";
      out << "hasErased:" << getHasErased();
      out << ",";
      out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
      out << ",";
      out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
      out << ",";
      out << "isTraversalInverted:" << getIsTraversalInverted();
      out << ",";
      out << "loadRebalancingState:" << toString(getLoadRebalancingState());
      out << ",";
      out << "reduceStateAndCell:" << getReduceStateAndCell();
      out << ",";
      out << "hasWorkerWithWorker:" << getHasWorkerWithWorker();
      out <<  ")";
   }
   
   
   peano::grid::tests::records::TestStatePacked::PersistentRecords peano::grid::tests::records::TestStatePacked::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   peano::grid::tests::records::TestState peano::grid::tests::records::TestStatePacked::convert() const{
      return TestState(
         getMinMeshWidth(),
         getMaxMeshWidth(),
         getNumberOfInnerVertices(),
         getNumberOfBoundaryVertices(),
         getNumberOfOuterVertices(),
         getNumberOfInnerCells(),
         getNumberOfOuterCells(),
         getMaxLevel(),
         getHasRefined(),
         getHasTriggeredRefinementForNextIteration(),
         getHasErased(),
         getHasTriggeredEraseForNextIteration(),
         getHasChangedVertexOrCellState(),
         getIsTraversalInverted(),
         getLoadRebalancingState(),
         getReduceStateAndCell(),
         getHasWorkerWithWorker()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log peano::grid::tests::records::TestStatePacked::_log( "peano::grid::tests::records::TestStatePacked" );
      
      MPI_Datatype peano::grid::tests::records::TestStatePacked::Datatype = 0;
      MPI_Datatype peano::grid::tests::records::TestStatePacked::FullDatatype = 0;
      
      
      void peano::grid::tests::records::TestStatePacked::initDatatype() {
         {
            TestStatePacked dummyTestStatePacked[2];
            
            #ifdef MPI2
            const int Attributes = 11;
            #else
            const int Attributes = 12;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_DOUBLE		 //minMeshWidth
               , MPI_DOUBLE		 //maxMeshWidth
               , MPI_DOUBLE		 //numberOfInnerVertices
               , MPI_DOUBLE		 //numberOfBoundaryVertices
               , MPI_DOUBLE		 //numberOfOuterVertices
               , MPI_DOUBLE		 //numberOfInnerCells
               , MPI_DOUBLE		 //numberOfOuterCells
               , MPI_INT		 //maxLevel
               , MPI_CXX_BOOL		 //isTraversalInverted
               , MPI_INT		 //_packedRecords0
               , MPI_INT		 //_packedRecords0
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 DIMENSIONS		 //minMeshWidth
               , DIMENSIONS		 //maxMeshWidth
               , 1		 //numberOfInnerVertices
               , 1		 //numberOfBoundaryVertices
               , 1		 //numberOfOuterVertices
               , 1		 //numberOfInnerCells
               , 1		 //numberOfOuterCells
               , 1		 //maxLevel
               , 1		 //isTraversalInverted
               , 1		 //_packedRecords0
               , 1		 //_packedRecords0
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxLevel))), 		&disp[7] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxLevel))), 		&disp[7] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[8] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[8] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[9] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[9] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._packedRecords0))), 		&disp[10] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._packedRecords0))), 		&disp[10] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(TestStatePacked)), i, disp[i], Attributes, sizeof(TestStatePacked));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[1]))), 		&disp[11] );
            disp[11] -= base;
            disp[11] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &TestStatePacked::Datatype );
            MPI_Type_commit( &TestStatePacked::Datatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestStatePacked::Datatype);
            MPI_Type_commit( &TestStatePacked::Datatype );
            #endif
            
         }
         {
            TestStatePacked dummyTestStatePacked[2];
            
            #ifdef MPI2
            const int Attributes = 12;
            #else
            const int Attributes = 13;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_DOUBLE		 //minMeshWidth
               , MPI_DOUBLE		 //maxMeshWidth
               , MPI_DOUBLE		 //numberOfInnerVertices
               , MPI_DOUBLE		 //numberOfBoundaryVertices
               , MPI_DOUBLE		 //numberOfOuterVertices
               , MPI_DOUBLE		 //numberOfInnerCells
               , MPI_DOUBLE		 //numberOfOuterCells
               , MPI_INT		 //maxLevel
               , MPI_CXX_BOOL		 //isTraversalInverted
               , MPI_INT		 //loadRebalancingState
               , MPI_INT		 //_packedRecords0
               , MPI_INT		 //_packedRecords0
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 DIMENSIONS		 //minMeshWidth
               , DIMENSIONS		 //maxMeshWidth
               , 1		 //numberOfInnerVertices
               , 1		 //numberOfBoundaryVertices
               , 1		 //numberOfOuterVertices
               , 1		 //numberOfInnerCells
               , 1		 //numberOfOuterCells
               , 1		 //maxLevel
               , 1		 //isTraversalInverted
               , 1		 //loadRebalancingState
               , 1		 //_packedRecords0
               , 1		 //_packedRecords0
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxLevel))), 		&disp[7] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxLevel))), 		&disp[7] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[8] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[8] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._loadRebalancingState))), 		&disp[9] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._loadRebalancingState))), 		&disp[9] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[10] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[10] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._packedRecords0))), 		&disp[11] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._packedRecords0))), 		&disp[11] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(TestStatePacked)), i, disp[i], Attributes, sizeof(TestStatePacked));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[1]))), 		&disp[12] );
            disp[12] -= base;
            disp[12] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &TestStatePacked::FullDatatype );
            MPI_Type_commit( &TestStatePacked::FullDatatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestStatePacked::FullDatatype);
            MPI_Type_commit( &TestStatePacked::FullDatatype );
            #endif
            
         }
         
      }
      
      
      void peano::grid::tests::records::TestStatePacked::shutdownDatatype() {
         MPI_Type_free( &TestStatePacked::Datatype );
         MPI_Type_free( &TestStatePacked::FullDatatype );
         
      }
      
      void peano::grid::tests::records::TestStatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
         // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message peano::grid::tests::records::TestStatePacked " 
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
         msg << "was not able to send message peano::grid::tests::records::TestStatePacked "  
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
           msg << "testing for finished send task for peano::grid::tests::records::TestStatePacked " 
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
             "peano::grid::tests::records::TestStatePacked", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "peano::grid::tests::records::TestStatePacked", 
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
      
      
      
      void peano::grid::tests::records::TestStatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive peano::grid::tests::records::TestStatePacked from node " 
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
        msg << "failed to start to receive peano::grid::tests::records::TestStatePacked from node " 
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
            "peano::grid::tests::records::TestStatePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::grid::tests::records::TestStatePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::grid::tests::records::TestStatePacked failed: " 
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
        msg << "testing for finished receive task for peano::grid::tests::records::TestStatePacked failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::grid::tests::records::TestStatePacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::grid::tests::records::TestStatePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::grid::tests::records::TestStatePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive peano::grid::tests::records::TestStatePacked from node " 
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
      
      
      
      bool peano::grid::tests::records::TestStatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
      
      int peano::grid::tests::records::TestStatePacked::getSenderRank() const {
         assertion( _senderDestinationRank!=-1 );
         return _senderDestinationRank;
         
      }
   #endif
   
   
   #elif !defined(Parallel)
      peano::grid::tests::records::TestState::PersistentRecords::PersistentRecords() {
         
      }
      
      
      peano::grid::tests::records::TestState::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted):
      _minMeshWidth(minMeshWidth),
      _maxMeshWidth(maxMeshWidth),
      _numberOfInnerVertices(numberOfInnerVertices),
      _numberOfBoundaryVertices(numberOfBoundaryVertices),
      _numberOfOuterVertices(numberOfOuterVertices),
      _numberOfInnerCells(numberOfInnerCells),
      _numberOfOuterCells(numberOfOuterCells),
      _maxLevel(maxLevel),
      _hasRefined(hasRefined),
      _hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
      _hasErased(hasErased),
      _hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
      _hasChangedVertexOrCellState(hasChangedVertexOrCellState),
      _isTraversalInverted(isTraversalInverted) {
         
      }
      
      
       tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestState::PersistentRecords::getMinMeshWidth() const  {
         return _minMeshWidth;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setMinMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth)  {
         _minMeshWidth = (minMeshWidth);
      }
      
      
      
       tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestState::PersistentRecords::getMaxMeshWidth() const  {
         return _maxMeshWidth;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setMaxMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth)  {
         _maxMeshWidth = (maxMeshWidth);
      }
      
      
      
       double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfInnerVertices() const  {
         return _numberOfInnerVertices;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfInnerVertices(const double& numberOfInnerVertices)  {
         _numberOfInnerVertices = numberOfInnerVertices;
      }
      
      
      
       double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfBoundaryVertices() const  {
         return _numberOfBoundaryVertices;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfBoundaryVertices(const double& numberOfBoundaryVertices)  {
         _numberOfBoundaryVertices = numberOfBoundaryVertices;
      }
      
      
      
       double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfOuterVertices() const  {
         return _numberOfOuterVertices;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfOuterVertices(const double& numberOfOuterVertices)  {
         _numberOfOuterVertices = numberOfOuterVertices;
      }
      
      
      
       double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfInnerCells() const  {
         return _numberOfInnerCells;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfInnerCells(const double& numberOfInnerCells)  {
         _numberOfInnerCells = numberOfInnerCells;
      }
      
      
      
       double peano::grid::tests::records::TestState::PersistentRecords::getNumberOfOuterCells() const  {
         return _numberOfOuterCells;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setNumberOfOuterCells(const double& numberOfOuterCells)  {
         _numberOfOuterCells = numberOfOuterCells;
      }
      
      
      
       int peano::grid::tests::records::TestState::PersistentRecords::getMaxLevel() const  {
         return _maxLevel;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setMaxLevel(const int& maxLevel)  {
         _maxLevel = maxLevel;
      }
      
      
      
       bool peano::grid::tests::records::TestState::PersistentRecords::getHasRefined() const  {
         return _hasRefined;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setHasRefined(const bool& hasRefined)  {
         _hasRefined = hasRefined;
      }
      
      
      
       bool peano::grid::tests::records::TestState::PersistentRecords::getHasTriggeredRefinementForNextIteration() const  {
         return _hasTriggeredRefinementForNextIteration;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setHasTriggeredRefinementForNextIteration(const bool& hasTriggeredRefinementForNextIteration)  {
         _hasTriggeredRefinementForNextIteration = hasTriggeredRefinementForNextIteration;
      }
      
      
      
       bool peano::grid::tests::records::TestState::PersistentRecords::getHasErased() const  {
         return _hasErased;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setHasErased(const bool& hasErased)  {
         _hasErased = hasErased;
      }
      
      
      
       bool peano::grid::tests::records::TestState::PersistentRecords::getHasTriggeredEraseForNextIteration() const  {
         return _hasTriggeredEraseForNextIteration;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setHasTriggeredEraseForNextIteration(const bool& hasTriggeredEraseForNextIteration)  {
         _hasTriggeredEraseForNextIteration = hasTriggeredEraseForNextIteration;
      }
      
      
      
       bool peano::grid::tests::records::TestState::PersistentRecords::getHasChangedVertexOrCellState() const  {
         return _hasChangedVertexOrCellState;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setHasChangedVertexOrCellState(const bool& hasChangedVertexOrCellState)  {
         _hasChangedVertexOrCellState = hasChangedVertexOrCellState;
      }
      
      
      
       bool peano::grid::tests::records::TestState::PersistentRecords::getIsTraversalInverted() const  {
         return _isTraversalInverted;
      }
      
      
      
       void peano::grid::tests::records::TestState::PersistentRecords::setIsTraversalInverted(const bool& isTraversalInverted)  {
         _isTraversalInverted = isTraversalInverted;
      }
      
      
      peano::grid::tests::records::TestState::TestState() {
         
      }
      
      
      peano::grid::tests::records::TestState::TestState(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._maxLevel, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._isTraversalInverted) {
         
      }
      
      
      peano::grid::tests::records::TestState::TestState(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted):
      _persistentRecords(minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, isTraversalInverted) {
         
      }
      
      
      peano::grid::tests::records::TestState::~TestState() { }
      
      
       tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestState::getMinMeshWidth() const  {
         return _persistentRecords._minMeshWidth;
      }
      
      
      
       void peano::grid::tests::records::TestState::setMinMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth)  {
         _persistentRecords._minMeshWidth = (minMeshWidth);
      }
      
      
      
       double peano::grid::tests::records::TestState::getMinMeshWidth(int elementIndex) const  {
         assertion(elementIndex>=0);
         assertion(elementIndex<DIMENSIONS);
         return _persistentRecords._minMeshWidth[elementIndex];
         
      }
      
      
      
       void peano::grid::tests::records::TestState::setMinMeshWidth(int elementIndex, const double& minMeshWidth)  {
         assertion(elementIndex>=0);
         assertion(elementIndex<DIMENSIONS);
         _persistentRecords._minMeshWidth[elementIndex]= minMeshWidth;
         
      }
      
      
      
       tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestState::getMaxMeshWidth() const  {
         return _persistentRecords._maxMeshWidth;
      }
      
      
      
       void peano::grid::tests::records::TestState::setMaxMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth)  {
         _persistentRecords._maxMeshWidth = (maxMeshWidth);
      }
      
      
      
       double peano::grid::tests::records::TestState::getMaxMeshWidth(int elementIndex) const  {
         assertion(elementIndex>=0);
         assertion(elementIndex<DIMENSIONS);
         return _persistentRecords._maxMeshWidth[elementIndex];
         
      }
      
      
      
       void peano::grid::tests::records::TestState::setMaxMeshWidth(int elementIndex, const double& maxMeshWidth)  {
         assertion(elementIndex>=0);
         assertion(elementIndex<DIMENSIONS);
         _persistentRecords._maxMeshWidth[elementIndex]= maxMeshWidth;
         
      }
      
      
      
       double peano::grid::tests::records::TestState::getNumberOfInnerVertices() const  {
         return _persistentRecords._numberOfInnerVertices;
      }
      
      
      
       void peano::grid::tests::records::TestState::setNumberOfInnerVertices(const double& numberOfInnerVertices)  {
         _persistentRecords._numberOfInnerVertices = numberOfInnerVertices;
      }
      
      
      
       double peano::grid::tests::records::TestState::getNumberOfBoundaryVertices() const  {
         return _persistentRecords._numberOfBoundaryVertices;
      }
      
      
      
       void peano::grid::tests::records::TestState::setNumberOfBoundaryVertices(const double& numberOfBoundaryVertices)  {
         _persistentRecords._numberOfBoundaryVertices = numberOfBoundaryVertices;
      }
      
      
      
       double peano::grid::tests::records::TestState::getNumberOfOuterVertices() const  {
         return _persistentRecords._numberOfOuterVertices;
      }
      
      
      
       void peano::grid::tests::records::TestState::setNumberOfOuterVertices(const double& numberOfOuterVertices)  {
         _persistentRecords._numberOfOuterVertices = numberOfOuterVertices;
      }
      
      
      
       double peano::grid::tests::records::TestState::getNumberOfInnerCells() const  {
         return _persistentRecords._numberOfInnerCells;
      }
      
      
      
       void peano::grid::tests::records::TestState::setNumberOfInnerCells(const double& numberOfInnerCells)  {
         _persistentRecords._numberOfInnerCells = numberOfInnerCells;
      }
      
      
      
       double peano::grid::tests::records::TestState::getNumberOfOuterCells() const  {
         return _persistentRecords._numberOfOuterCells;
      }
      
      
      
       void peano::grid::tests::records::TestState::setNumberOfOuterCells(const double& numberOfOuterCells)  {
         _persistentRecords._numberOfOuterCells = numberOfOuterCells;
      }
      
      
      
       int peano::grid::tests::records::TestState::getMaxLevel() const  {
         return _persistentRecords._maxLevel;
      }
      
      
      
       void peano::grid::tests::records::TestState::setMaxLevel(const int& maxLevel)  {
         _persistentRecords._maxLevel = maxLevel;
      }
      
      
      
       bool peano::grid::tests::records::TestState::getHasRefined() const  {
         return _persistentRecords._hasRefined;
      }
      
      
      
       void peano::grid::tests::records::TestState::setHasRefined(const bool& hasRefined)  {
         _persistentRecords._hasRefined = hasRefined;
      }
      
      
      
       bool peano::grid::tests::records::TestState::getHasTriggeredRefinementForNextIteration() const  {
         return _persistentRecords._hasTriggeredRefinementForNextIteration;
      }
      
      
      
       void peano::grid::tests::records::TestState::setHasTriggeredRefinementForNextIteration(const bool& hasTriggeredRefinementForNextIteration)  {
         _persistentRecords._hasTriggeredRefinementForNextIteration = hasTriggeredRefinementForNextIteration;
      }
      
      
      
       bool peano::grid::tests::records::TestState::getHasErased() const  {
         return _persistentRecords._hasErased;
      }
      
      
      
       void peano::grid::tests::records::TestState::setHasErased(const bool& hasErased)  {
         _persistentRecords._hasErased = hasErased;
      }
      
      
      
       bool peano::grid::tests::records::TestState::getHasTriggeredEraseForNextIteration() const  {
         return _persistentRecords._hasTriggeredEraseForNextIteration;
      }
      
      
      
       void peano::grid::tests::records::TestState::setHasTriggeredEraseForNextIteration(const bool& hasTriggeredEraseForNextIteration)  {
         _persistentRecords._hasTriggeredEraseForNextIteration = hasTriggeredEraseForNextIteration;
      }
      
      
      
       bool peano::grid::tests::records::TestState::getHasChangedVertexOrCellState() const  {
         return _persistentRecords._hasChangedVertexOrCellState;
      }
      
      
      
       void peano::grid::tests::records::TestState::setHasChangedVertexOrCellState(const bool& hasChangedVertexOrCellState)  {
         _persistentRecords._hasChangedVertexOrCellState = hasChangedVertexOrCellState;
      }
      
      
      
       bool peano::grid::tests::records::TestState::getIsTraversalInverted() const  {
         return _persistentRecords._isTraversalInverted;
      }
      
      
      
       void peano::grid::tests::records::TestState::setIsTraversalInverted(const bool& isTraversalInverted)  {
         _persistentRecords._isTraversalInverted = isTraversalInverted;
      }
      
      
      
      
      std::string peano::grid::tests::records::TestState::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void peano::grid::tests::records::TestState::toString (std::ostream& out) const {
         out << "("; 
         out << "minMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMinMeshWidth(i) << ",";
   }
   out << getMinMeshWidth(DIMENSIONS-1) << "]";
         out << ",";
         out << "maxMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMaxMeshWidth(i) << ",";
   }
   out << getMaxMeshWidth(DIMENSIONS-1) << "]";
         out << ",";
         out << "numberOfInnerVertices:" << getNumberOfInnerVertices();
         out << ",";
         out << "numberOfBoundaryVertices:" << getNumberOfBoundaryVertices();
         out << ",";
         out << "numberOfOuterVertices:" << getNumberOfOuterVertices();
         out << ",";
         out << "numberOfInnerCells:" << getNumberOfInnerCells();
         out << ",";
         out << "numberOfOuterCells:" << getNumberOfOuterCells();
         out << ",";
         out << "maxLevel:" << getMaxLevel();
         out << ",";
         out << "hasRefined:" << getHasRefined();
         out << ",";
         out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
         out << ",";
         out << "hasErased:" << getHasErased();
         out << ",";
         out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
         out << ",";
         out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
         out << ",";
         out << "isTraversalInverted:" << getIsTraversalInverted();
         out <<  ")";
      }
      
      
      peano::grid::tests::records::TestState::PersistentRecords peano::grid::tests::records::TestState::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      peano::grid::tests::records::TestStatePacked peano::grid::tests::records::TestState::convert() const{
         return TestStatePacked(
            getMinMeshWidth(),
            getMaxMeshWidth(),
            getNumberOfInnerVertices(),
            getNumberOfBoundaryVertices(),
            getNumberOfOuterVertices(),
            getNumberOfInnerCells(),
            getNumberOfOuterCells(),
            getMaxLevel(),
            getHasRefined(),
            getHasTriggeredRefinementForNextIteration(),
            getHasErased(),
            getHasTriggeredEraseForNextIteration(),
            getHasChangedVertexOrCellState(),
            getIsTraversalInverted()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log peano::grid::tests::records::TestState::_log( "peano::grid::tests::records::TestState" );
         
         MPI_Datatype peano::grid::tests::records::TestState::Datatype = 0;
         MPI_Datatype peano::grid::tests::records::TestState::FullDatatype = 0;
         
         
         void peano::grid::tests::records::TestState::initDatatype() {
            {
               TestState dummyTestState[2];
               
               #ifdef MPI2
               const int Attributes = 14;
               #else
               const int Attributes = 15;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //hasRefined
                  , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
                  , MPI_CXX_BOOL		 //hasErased
                  , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
                  , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //maxLevel
                  , 1		 //hasRefined
                  , 1		 //hasTriggeredRefinementForNextIteration
                  , 1		 //hasErased
                  , 1		 //hasTriggeredEraseForNextIteration
                  , 1		 //hasChangedVertexOrCellState
                  , 1		 //isTraversalInverted
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxLevel))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxLevel))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasRefined))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasRefined))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasErased))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasErased))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._isTraversalInverted))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._isTraversalInverted))), 		&disp[13] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(TestState)), i, disp[i], Attributes, sizeof(TestState));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[1]))), 		&disp[14] );
               disp[14] -= base;
               disp[14] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &TestState::Datatype );
               MPI_Type_commit( &TestState::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestState::Datatype);
               MPI_Type_commit( &TestState::Datatype );
               #endif
               
            }
            {
               TestState dummyTestState[2];
               
               #ifdef MPI2
               const int Attributes = 14;
               #else
               const int Attributes = 15;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //hasRefined
                  , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
                  , MPI_CXX_BOOL		 //hasErased
                  , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
                  , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //maxLevel
                  , 1		 //hasRefined
                  , 1		 //hasTriggeredRefinementForNextIteration
                  , 1		 //hasErased
                  , 1		 //hasTriggeredEraseForNextIteration
                  , 1		 //hasChangedVertexOrCellState
                  , 1		 //isTraversalInverted
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxLevel))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._maxLevel))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasRefined))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasRefined))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasErased))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasErased))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._isTraversalInverted))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[0]._persistentRecords._isTraversalInverted))), 		&disp[13] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(TestState)), i, disp[i], Attributes, sizeof(TestState));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestState[1]))), 		&disp[14] );
               disp[14] -= base;
               disp[14] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &TestState::FullDatatype );
               MPI_Type_commit( &TestState::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestState::FullDatatype);
               MPI_Type_commit( &TestState::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void peano::grid::tests::records::TestState::shutdownDatatype() {
            MPI_Type_free( &TestState::Datatype );
            MPI_Type_free( &TestState::FullDatatype );
            
         }
         
         void peano::grid::tests::records::TestState::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
            // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message peano::grid::tests::records::TestState " 
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
         msg << "was not able to send message peano::grid::tests::records::TestState "  
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
           msg << "testing for finished send task for peano::grid::tests::records::TestState " 
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
             "peano::grid::tests::records::TestState", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "peano::grid::tests::records::TestState", 
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
         
         
         
         void peano::grid::tests::records::TestState::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive peano::grid::tests::records::TestState from node " 
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
        msg << "failed to start to receive peano::grid::tests::records::TestState from node " 
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
            "peano::grid::tests::records::TestState", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::grid::tests::records::TestState", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::grid::tests::records::TestState failed: " 
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
        msg << "testing for finished receive task for peano::grid::tests::records::TestState failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::grid::tests::records::TestState failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::grid::tests::records::TestState", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::grid::tests::records::TestState", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive peano::grid::tests::records::TestState from node " 
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
         
         
         
         bool peano::grid::tests::records::TestState::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int peano::grid::tests::records::TestState::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      peano::grid::tests::records::TestStatePacked::PersistentRecords::PersistentRecords() {
         if ((5 >= (8 * sizeof(int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((5 < (8 * sizeof(int))));
         
      }
      
      
      peano::grid::tests::records::TestStatePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted):
      _minMeshWidth(minMeshWidth),
      _maxMeshWidth(maxMeshWidth),
      _numberOfInnerVertices(numberOfInnerVertices),
      _numberOfBoundaryVertices(numberOfBoundaryVertices),
      _numberOfOuterVertices(numberOfOuterVertices),
      _numberOfInnerCells(numberOfInnerCells),
      _numberOfOuterCells(numberOfOuterCells),
      _maxLevel(maxLevel),
      _isTraversalInverted(isTraversalInverted) {
         setHasRefined(hasRefined);
         setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
         setHasErased(hasErased);
         setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
         setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
         if ((5 >= (8 * sizeof(int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((5 < (8 * sizeof(int))));
         
      }
      
      
       tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestStatePacked::PersistentRecords::getMinMeshWidth() const  {
         return _minMeshWidth;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setMinMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth)  {
         _minMeshWidth = (minMeshWidth);
      }
      
      
      
       tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestStatePacked::PersistentRecords::getMaxMeshWidth() const  {
         return _maxMeshWidth;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setMaxMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth)  {
         _maxMeshWidth = (maxMeshWidth);
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfInnerVertices() const  {
         return _numberOfInnerVertices;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfInnerVertices(const double& numberOfInnerVertices)  {
         _numberOfInnerVertices = numberOfInnerVertices;
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfBoundaryVertices() const  {
         return _numberOfBoundaryVertices;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfBoundaryVertices(const double& numberOfBoundaryVertices)  {
         _numberOfBoundaryVertices = numberOfBoundaryVertices;
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfOuterVertices() const  {
         return _numberOfOuterVertices;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfOuterVertices(const double& numberOfOuterVertices)  {
         _numberOfOuterVertices = numberOfOuterVertices;
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfInnerCells() const  {
         return _numberOfInnerCells;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfInnerCells(const double& numberOfInnerCells)  {
         _numberOfInnerCells = numberOfInnerCells;
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::PersistentRecords::getNumberOfOuterCells() const  {
         return _numberOfOuterCells;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setNumberOfOuterCells(const double& numberOfOuterCells)  {
         _numberOfOuterCells = numberOfOuterCells;
      }
      
      
      
       int peano::grid::tests::records::TestStatePacked::PersistentRecords::getMaxLevel() const  {
         return _maxLevel;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setMaxLevel(const int& maxLevel)  {
         _maxLevel = maxLevel;
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasRefined() const  {
         int mask = 1 << (0);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasRefined(const bool& hasRefined)  {
         int mask = 1 << (0);
   _packedRecords0 = static_cast<int>( hasRefined ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasTriggeredRefinementForNextIteration() const  {
         int mask = 1 << (1);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasTriggeredRefinementForNextIteration(const bool& hasTriggeredRefinementForNextIteration)  {
         int mask = 1 << (1);
   _packedRecords0 = static_cast<int>( hasTriggeredRefinementForNextIteration ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasErased() const  {
         int mask = 1 << (2);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasErased(const bool& hasErased)  {
         int mask = 1 << (2);
   _packedRecords0 = static_cast<int>( hasErased ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasTriggeredEraseForNextIteration() const  {
         int mask = 1 << (3);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasTriggeredEraseForNextIteration(const bool& hasTriggeredEraseForNextIteration)  {
         int mask = 1 << (3);
   _packedRecords0 = static_cast<int>( hasTriggeredEraseForNextIteration ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getHasChangedVertexOrCellState() const  {
         int mask = 1 << (4);
   int tmp = static_cast<int>(_packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setHasChangedVertexOrCellState(const bool& hasChangedVertexOrCellState)  {
         int mask = 1 << (4);
   _packedRecords0 = static_cast<int>( hasChangedVertexOrCellState ? (_packedRecords0 | mask) : (_packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::PersistentRecords::getIsTraversalInverted() const  {
         return _isTraversalInverted;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::PersistentRecords::setIsTraversalInverted(const bool& isTraversalInverted)  {
         _isTraversalInverted = isTraversalInverted;
      }
      
      
      peano::grid::tests::records::TestStatePacked::TestStatePacked() {
         if ((5 >= (8 * sizeof(int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((5 < (8 * sizeof(int))));
         
      }
      
      
      peano::grid::tests::records::TestStatePacked::TestStatePacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._maxLevel, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords._isTraversalInverted) {
         if ((5 >= (8 * sizeof(int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((5 < (8 * sizeof(int))));
         
      }
      
      
      peano::grid::tests::records::TestStatePacked::TestStatePacked(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& isTraversalInverted):
      _persistentRecords(minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, isTraversalInverted) {
         if ((5 >= (8 * sizeof(int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((5 < (8 * sizeof(int))));
         
      }
      
      
      peano::grid::tests::records::TestStatePacked::~TestStatePacked() { }
      
      
       tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestStatePacked::getMinMeshWidth() const  {
         return _persistentRecords._minMeshWidth;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setMinMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth)  {
         _persistentRecords._minMeshWidth = (minMeshWidth);
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::getMinMeshWidth(int elementIndex) const  {
         assertion(elementIndex>=0);
         assertion(elementIndex<DIMENSIONS);
         return _persistentRecords._minMeshWidth[elementIndex];
         
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setMinMeshWidth(int elementIndex, const double& minMeshWidth)  {
         assertion(elementIndex>=0);
         assertion(elementIndex<DIMENSIONS);
         _persistentRecords._minMeshWidth[elementIndex]= minMeshWidth;
         
      }
      
      
      
       tarch::la::Vector<DIMENSIONS,double> peano::grid::tests::records::TestStatePacked::getMaxMeshWidth() const  {
         return _persistentRecords._maxMeshWidth;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setMaxMeshWidth(const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth)  {
         _persistentRecords._maxMeshWidth = (maxMeshWidth);
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::getMaxMeshWidth(int elementIndex) const  {
         assertion(elementIndex>=0);
         assertion(elementIndex<DIMENSIONS);
         return _persistentRecords._maxMeshWidth[elementIndex];
         
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setMaxMeshWidth(int elementIndex, const double& maxMeshWidth)  {
         assertion(elementIndex>=0);
         assertion(elementIndex<DIMENSIONS);
         _persistentRecords._maxMeshWidth[elementIndex]= maxMeshWidth;
         
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::getNumberOfInnerVertices() const  {
         return _persistentRecords._numberOfInnerVertices;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setNumberOfInnerVertices(const double& numberOfInnerVertices)  {
         _persistentRecords._numberOfInnerVertices = numberOfInnerVertices;
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::getNumberOfBoundaryVertices() const  {
         return _persistentRecords._numberOfBoundaryVertices;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setNumberOfBoundaryVertices(const double& numberOfBoundaryVertices)  {
         _persistentRecords._numberOfBoundaryVertices = numberOfBoundaryVertices;
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::getNumberOfOuterVertices() const  {
         return _persistentRecords._numberOfOuterVertices;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setNumberOfOuterVertices(const double& numberOfOuterVertices)  {
         _persistentRecords._numberOfOuterVertices = numberOfOuterVertices;
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::getNumberOfInnerCells() const  {
         return _persistentRecords._numberOfInnerCells;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setNumberOfInnerCells(const double& numberOfInnerCells)  {
         _persistentRecords._numberOfInnerCells = numberOfInnerCells;
      }
      
      
      
       double peano::grid::tests::records::TestStatePacked::getNumberOfOuterCells() const  {
         return _persistentRecords._numberOfOuterCells;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setNumberOfOuterCells(const double& numberOfOuterCells)  {
         _persistentRecords._numberOfOuterCells = numberOfOuterCells;
      }
      
      
      
       int peano::grid::tests::records::TestStatePacked::getMaxLevel() const  {
         return _persistentRecords._maxLevel;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setMaxLevel(const int& maxLevel)  {
         _persistentRecords._maxLevel = maxLevel;
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::getHasRefined() const  {
         int mask = 1 << (0);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setHasRefined(const bool& hasRefined)  {
         int mask = 1 << (0);
   _persistentRecords._packedRecords0 = static_cast<int>( hasRefined ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::getHasTriggeredRefinementForNextIteration() const  {
         int mask = 1 << (1);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setHasTriggeredRefinementForNextIteration(const bool& hasTriggeredRefinementForNextIteration)  {
         int mask = 1 << (1);
   _persistentRecords._packedRecords0 = static_cast<int>( hasTriggeredRefinementForNextIteration ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::getHasErased() const  {
         int mask = 1 << (2);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setHasErased(const bool& hasErased)  {
         int mask = 1 << (2);
   _persistentRecords._packedRecords0 = static_cast<int>( hasErased ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::getHasTriggeredEraseForNextIteration() const  {
         int mask = 1 << (3);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setHasTriggeredEraseForNextIteration(const bool& hasTriggeredEraseForNextIteration)  {
         int mask = 1 << (3);
   _persistentRecords._packedRecords0 = static_cast<int>( hasTriggeredEraseForNextIteration ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::getHasChangedVertexOrCellState() const  {
         int mask = 1 << (4);
   int tmp = static_cast<int>(_persistentRecords._packedRecords0 & mask);
   return (tmp != 0);
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setHasChangedVertexOrCellState(const bool& hasChangedVertexOrCellState)  {
         int mask = 1 << (4);
   _persistentRecords._packedRecords0 = static_cast<int>( hasChangedVertexOrCellState ? (_persistentRecords._packedRecords0 | mask) : (_persistentRecords._packedRecords0 & ~mask));
      }
      
      
      
       bool peano::grid::tests::records::TestStatePacked::getIsTraversalInverted() const  {
         return _persistentRecords._isTraversalInverted;
      }
      
      
      
       void peano::grid::tests::records::TestStatePacked::setIsTraversalInverted(const bool& isTraversalInverted)  {
         _persistentRecords._isTraversalInverted = isTraversalInverted;
      }
      
      
      
      
      std::string peano::grid::tests::records::TestStatePacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void peano::grid::tests::records::TestStatePacked::toString (std::ostream& out) const {
         out << "("; 
         out << "minMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMinMeshWidth(i) << ",";
   }
   out << getMinMeshWidth(DIMENSIONS-1) << "]";
         out << ",";
         out << "maxMeshWidth:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getMaxMeshWidth(i) << ",";
   }
   out << getMaxMeshWidth(DIMENSIONS-1) << "]";
         out << ",";
         out << "numberOfInnerVertices:" << getNumberOfInnerVertices();
         out << ",";
         out << "numberOfBoundaryVertices:" << getNumberOfBoundaryVertices();
         out << ",";
         out << "numberOfOuterVertices:" << getNumberOfOuterVertices();
         out << ",";
         out << "numberOfInnerCells:" << getNumberOfInnerCells();
         out << ",";
         out << "numberOfOuterCells:" << getNumberOfOuterCells();
         out << ",";
         out << "maxLevel:" << getMaxLevel();
         out << ",";
         out << "hasRefined:" << getHasRefined();
         out << ",";
         out << "hasTriggeredRefinementForNextIteration:" << getHasTriggeredRefinementForNextIteration();
         out << ",";
         out << "hasErased:" << getHasErased();
         out << ",";
         out << "hasTriggeredEraseForNextIteration:" << getHasTriggeredEraseForNextIteration();
         out << ",";
         out << "hasChangedVertexOrCellState:" << getHasChangedVertexOrCellState();
         out << ",";
         out << "isTraversalInverted:" << getIsTraversalInverted();
         out <<  ")";
      }
      
      
      peano::grid::tests::records::TestStatePacked::PersistentRecords peano::grid::tests::records::TestStatePacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      peano::grid::tests::records::TestState peano::grid::tests::records::TestStatePacked::convert() const{
         return TestState(
            getMinMeshWidth(),
            getMaxMeshWidth(),
            getNumberOfInnerVertices(),
            getNumberOfBoundaryVertices(),
            getNumberOfOuterVertices(),
            getNumberOfInnerCells(),
            getNumberOfOuterCells(),
            getMaxLevel(),
            getHasRefined(),
            getHasTriggeredRefinementForNextIteration(),
            getHasErased(),
            getHasTriggeredEraseForNextIteration(),
            getHasChangedVertexOrCellState(),
            getIsTraversalInverted()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log peano::grid::tests::records::TestStatePacked::_log( "peano::grid::tests::records::TestStatePacked" );
         
         MPI_Datatype peano::grid::tests::records::TestStatePacked::Datatype = 0;
         MPI_Datatype peano::grid::tests::records::TestStatePacked::FullDatatype = 0;
         
         
         void peano::grid::tests::records::TestStatePacked::initDatatype() {
            {
               TestStatePacked dummyTestStatePacked[2];
               
               #ifdef MPI2
               const int Attributes = 10;
               #else
               const int Attributes = 11;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_INT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //maxLevel
                  , 1		 //isTraversalInverted
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxLevel))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxLevel))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[9] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(TestStatePacked)), i, disp[i], Attributes, sizeof(TestStatePacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[1]))), 		&disp[10] );
               disp[10] -= base;
               disp[10] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &TestStatePacked::Datatype );
               MPI_Type_commit( &TestStatePacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestStatePacked::Datatype);
               MPI_Type_commit( &TestStatePacked::Datatype );
               #endif
               
            }
            {
               TestStatePacked dummyTestStatePacked[2];
               
               #ifdef MPI2
               const int Attributes = 10;
               #else
               const int Attributes = 11;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_INT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //maxLevel
                  , 1		 //isTraversalInverted
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxLevel))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._maxLevel))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[9] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(TestStatePacked)), i, disp[i], Attributes, sizeof(TestStatePacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyTestStatePacked[1]))), 		&disp[10] );
               disp[10] -= base;
               disp[10] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &TestStatePacked::FullDatatype );
               MPI_Type_commit( &TestStatePacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &TestStatePacked::FullDatatype);
               MPI_Type_commit( &TestStatePacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void peano::grid::tests::records::TestStatePacked::shutdownDatatype() {
            MPI_Type_free( &TestStatePacked::Datatype );
            MPI_Type_free( &TestStatePacked::FullDatatype );
            
         }
         
         void peano::grid::tests::records::TestStatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
            // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
      const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator()); 
       if  (result!=MPI_SUCCESS) { 
         std::ostringstream msg; 
         msg << "was not able to send message peano::grid::tests::records::TestStatePacked " 
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
         msg << "was not able to send message peano::grid::tests::records::TestStatePacked "  
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
           msg << "testing for finished send task for peano::grid::tests::records::TestStatePacked " 
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
             "peano::grid::tests::records::TestStatePacked", 
             "send(int)", destination,tag,1 
           ); 
           triggeredTimeoutWarning = true; 
         } 
         if ( 
           tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
           (clock()>timeOutShutdown) 
         ) { 
           tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
             "peano::grid::tests::records::TestStatePacked", 
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
         
         
         
         void peano::grid::tests::records::TestStatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive peano::grid::tests::records::TestStatePacked from node " 
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
        msg << "failed to start to receive peano::grid::tests::records::TestStatePacked from node " 
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
            "peano::grid::tests::records::TestStatePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::grid::tests::records::TestStatePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::grid::tests::records::TestStatePacked failed: " 
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
        msg << "testing for finished receive task for peano::grid::tests::records::TestStatePacked failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for peano::grid::tests::records::TestStatePacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() && 
          (clock()>timeOutWarning) && 
          (!triggeredTimeoutWarning) 
        ) { 
          tarch::parallel::Node::getInstance().writeTimeOutWarning( 
            "peano::grid::tests::records::TestStatePacked", 
            "receive(int)", source,tag,1 
          ); 
          triggeredTimeoutWarning = true; 
        } 
        if ( 
          tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() && 
          (clock()>timeOutShutdown) 
        ) { 
          tarch::parallel::Node::getInstance().triggerDeadlockTimeOut( 
            "peano::grid::tests::records::TestStatePacked", 
            "receive(int)", source,tag,1 
          ); 
        } 
        tarch::parallel::Node::getInstance().receiveDanglingMessages(); 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive peano::grid::tests::records::TestStatePacked from node " 
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
         
         
         
         bool peano::grid::tests::records::TestStatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int peano::grid::tests::records::TestStatePacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   
#endif


