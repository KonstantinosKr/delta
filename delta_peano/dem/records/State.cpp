#include "dem/records/State.h"

#if !defined(TrackGridStatistics) && !defined(Parallel)
   dem::records::State::PersistentRecords::PersistentRecords() {
      
   }
   
   
   dem::records::State::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
   _numberOfContactPoints(numberOfContactPoints),
   _numberOfParticleReassignments(numberOfParticleReassignments),
   _numberOfTriangleComparisons(numberOfTriangleComparisons),
   _numberOfParticleComparisons(numberOfParticleComparisons),
   _adaptiveStepSize(adaptiveStepSize),
   _timeStepSize(timeStepSize),
   _timeStep(timeStep),
   _currentTime(currentTime),
   _stepIncrement(stepIncrement),
   _twoParticlesAreClose(twoParticlesAreClose),
   _twoParticlesSeparate(twoParticlesSeparate),
   _numberOfParticles(numberOfParticles),
   _numberOfObstacles(numberOfObstacles),
   _prescribedMinimumMeshWidth(prescribedMinimumMeshWidth),
   _prescribedMaximumMeshWidth(prescribedMaximumMeshWidth),
   _maxVelocityApproach(maxVelocityApproach),
   _maxVelocityTravel(maxVelocityTravel),
   _hasRefined(hasRefined),
   _hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
   _hasErased(hasErased),
   _hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
   _hasChangedVertexOrCellState(hasChangedVertexOrCellState),
   _hasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration),
   _isTraversalInverted(isTraversalInverted) {
      
   }
   
   dem::records::State::State() {
      
   }
   
   
   dem::records::State::State(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._numberOfParticleComparisons, persistentRecords._adaptiveStepSize, persistentRecords._timeStepSize, persistentRecords._timeStep, persistentRecords._currentTime, persistentRecords._stepIncrement, persistentRecords._twoParticlesAreClose, persistentRecords._twoParticlesSeparate, persistentRecords._numberOfParticles, persistentRecords._numberOfObstacles, persistentRecords._prescribedMinimumMeshWidth, persistentRecords._prescribedMaximumMeshWidth, persistentRecords._maxVelocityApproach, persistentRecords._maxVelocityTravel, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._hasModifiedGridInPreviousIteration, persistentRecords._isTraversalInverted) {
      
   }
   
   
   dem::records::State::State(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
   _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, numberOfParticleComparisons, adaptiveStepSize, timeStepSize, timeStep, currentTime, stepIncrement, twoParticlesAreClose, twoParticlesSeparate, numberOfParticles, numberOfObstacles, prescribedMinimumMeshWidth, prescribedMaximumMeshWidth, maxVelocityApproach, maxVelocityTravel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted) {
      
   }
   
   
   dem::records::State::~State() { }
   
   
   
   std::string dem::records::State::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::State::toString (std::ostream& out) const {
      out << "("; 
      out << "numberOfContactPoints:" << getNumberOfContactPoints();
      out << ",";
      out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
      out << ",";
      out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
      out << ",";
      out << "numberOfParticleComparisons:" << getNumberOfParticleComparisons();
      out << ",";
      out << "adaptiveStepSize:" << getAdaptiveStepSize();
      out << ",";
      out << "timeStepSize:" << getTimeStepSize();
      out << ",";
      out << "timeStep:" << getTimeStep();
      out << ",";
      out << "currentTime:" << getCurrentTime();
      out << ",";
      out << "stepIncrement:" << getStepIncrement();
      out << ",";
      out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
      out << ",";
      out << "twoParticlesSeparate:" << getTwoParticlesSeparate();
      out << ",";
      out << "numberOfParticles:" << getNumberOfParticles();
      out << ",";
      out << "numberOfObstacles:" << getNumberOfObstacles();
      out << ",";
      out << "prescribedMinimumMeshWidth:" << getPrescribedMinimumMeshWidth();
      out << ",";
      out << "prescribedMaximumMeshWidth:" << getPrescribedMaximumMeshWidth();
      out << ",";
      out << "maxVelocityApproach:" << getMaxVelocityApproach();
      out << ",";
      out << "maxVelocityTravel:" << getMaxVelocityTravel();
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
      out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
      out << ",";
      out << "isTraversalInverted:" << getIsTraversalInverted();
      out <<  ")";
   }
   
   
   dem::records::State::PersistentRecords dem::records::State::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::StatePacked dem::records::State::convert() const{
      return StatePacked(
         getNumberOfContactPoints(),
         getNumberOfParticleReassignments(),
         getNumberOfTriangleComparisons(),
         getNumberOfParticleComparisons(),
         getAdaptiveStepSize(),
         getTimeStepSize(),
         getTimeStep(),
         getCurrentTime(),
         getStepIncrement(),
         getTwoParticlesAreClose(),
         getTwoParticlesSeparate(),
         getNumberOfParticles(),
         getNumberOfObstacles(),
         getPrescribedMinimumMeshWidth(),
         getPrescribedMaximumMeshWidth(),
         getMaxVelocityApproach(),
         getMaxVelocityTravel(),
         getHasRefined(),
         getHasTriggeredRefinementForNextIteration(),
         getHasErased(),
         getHasTriggeredEraseForNextIteration(),
         getHasChangedVertexOrCellState(),
         getHasModifiedGridInPreviousIteration(),
         getIsTraversalInverted()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::State::_log( "dem::records::State" );
      
      MPI_Datatype dem::records::State::Datatype = 0;
      MPI_Datatype dem::records::State::FullDatatype = 0;
      
      
      void dem::records::State::initDatatype() {
         {
            State dummyState[2];
            
            #ifdef MPI2
            const int Attributes = 24;
            #else
            const int Attributes = 25;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_DOUBLE		 //numberOfContactPoints
               , MPI_DOUBLE		 //numberOfParticleReassignments
               , MPI_DOUBLE		 //numberOfTriangleComparisons
               , MPI_DOUBLE		 //numberOfParticleComparisons
               , MPI_CXX_BOOL		 //adaptiveStepSize
               , MPI_DOUBLE		 //timeStepSize
               , MPI_INT		 //timeStep
               , MPI_DOUBLE		 //currentTime
               , MPI_DOUBLE		 //stepIncrement
               , MPI_DOUBLE		 //twoParticlesAreClose
               , MPI_CXX_BOOL		 //twoParticlesSeparate
               , MPI_INT		 //numberOfParticles
               , MPI_INT		 //numberOfObstacles
               , MPI_DOUBLE		 //prescribedMinimumMeshWidth
               , MPI_DOUBLE		 //prescribedMaximumMeshWidth
               , MPI_DOUBLE		 //maxVelocityApproach
               , MPI_DOUBLE		 //maxVelocityTravel
               , MPI_CXX_BOOL		 //hasRefined
               , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
               , MPI_CXX_BOOL		 //hasErased
               , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
               , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
               , MPI_CXX_BOOL		 //hasModifiedGridInPreviousIteration
               , MPI_CXX_BOOL		 //isTraversalInverted
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 1		 //numberOfContactPoints
               , 1		 //numberOfParticleReassignments
               , 1		 //numberOfTriangleComparisons
               , 1		 //numberOfParticleComparisons
               , 1		 //adaptiveStepSize
               , 1		 //timeStepSize
               , 1		 //timeStep
               , 1		 //currentTime
               , 1		 //stepIncrement
               , 1		 //twoParticlesAreClose
               , 1		 //twoParticlesSeparate
               , 1		 //numberOfParticles
               , 1		 //numberOfObstacles
               , 1		 //prescribedMinimumMeshWidth
               , 1		 //prescribedMaximumMeshWidth
               , 1		 //maxVelocityApproach
               , 1		 //maxVelocityTravel
               , 1		 //hasRefined
               , 1		 //hasTriggeredRefinementForNextIteration
               , 1		 //hasErased
               , 1		 //hasTriggeredEraseForNextIteration
               , 1		 //hasChangedVertexOrCellState
               , 1		 //hasModifiedGridInPreviousIteration
               , 1		 //isTraversalInverted
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[17] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[17] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[18] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[18] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[19] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[19] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[20] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[20] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[21] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[21] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[22] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[22] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[23] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[23] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(State)), i, disp[i], Attributes, sizeof(State));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]))), 		&disp[24] );
            disp[24] -= base;
            disp[24] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &State::Datatype );
            MPI_Type_commit( &State::Datatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::Datatype);
            MPI_Type_commit( &State::Datatype );
            #endif
            
         }
         {
            State dummyState[2];
            
            #ifdef MPI2
            const int Attributes = 24;
            #else
            const int Attributes = 25;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_DOUBLE		 //numberOfContactPoints
               , MPI_DOUBLE		 //numberOfParticleReassignments
               , MPI_DOUBLE		 //numberOfTriangleComparisons
               , MPI_DOUBLE		 //numberOfParticleComparisons
               , MPI_CXX_BOOL		 //adaptiveStepSize
               , MPI_DOUBLE		 //timeStepSize
               , MPI_INT		 //timeStep
               , MPI_DOUBLE		 //currentTime
               , MPI_DOUBLE		 //stepIncrement
               , MPI_DOUBLE		 //twoParticlesAreClose
               , MPI_CXX_BOOL		 //twoParticlesSeparate
               , MPI_INT		 //numberOfParticles
               , MPI_INT		 //numberOfObstacles
               , MPI_DOUBLE		 //prescribedMinimumMeshWidth
               , MPI_DOUBLE		 //prescribedMaximumMeshWidth
               , MPI_DOUBLE		 //maxVelocityApproach
               , MPI_DOUBLE		 //maxVelocityTravel
               , MPI_CXX_BOOL		 //hasRefined
               , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
               , MPI_CXX_BOOL		 //hasErased
               , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
               , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
               , MPI_CXX_BOOL		 //hasModifiedGridInPreviousIteration
               , MPI_CXX_BOOL		 //isTraversalInverted
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 1		 //numberOfContactPoints
               , 1		 //numberOfParticleReassignments
               , 1		 //numberOfTriangleComparisons
               , 1		 //numberOfParticleComparisons
               , 1		 //adaptiveStepSize
               , 1		 //timeStepSize
               , 1		 //timeStep
               , 1		 //currentTime
               , 1		 //stepIncrement
               , 1		 //twoParticlesAreClose
               , 1		 //twoParticlesSeparate
               , 1		 //numberOfParticles
               , 1		 //numberOfObstacles
               , 1		 //prescribedMinimumMeshWidth
               , 1		 //prescribedMaximumMeshWidth
               , 1		 //maxVelocityApproach
               , 1		 //maxVelocityTravel
               , 1		 //hasRefined
               , 1		 //hasTriggeredRefinementForNextIteration
               , 1		 //hasErased
               , 1		 //hasTriggeredEraseForNextIteration
               , 1		 //hasChangedVertexOrCellState
               , 1		 //hasModifiedGridInPreviousIteration
               , 1		 //isTraversalInverted
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[17] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[17] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[18] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[18] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[19] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[19] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[20] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[20] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[21] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[21] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[22] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[22] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[23] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[23] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(State)), i, disp[i], Attributes, sizeof(State));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]))), 		&disp[24] );
            disp[24] -= base;
            disp[24] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &State::FullDatatype );
            MPI_Type_commit( &State::FullDatatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::FullDatatype);
            MPI_Type_commit( &State::FullDatatype );
            #endif
            
         }
         
      }
      
      
      void dem::records::State::shutdownDatatype() {
         MPI_Type_free( &State::Datatype );
         MPI_Type_free( &State::FullDatatype );
         
      }
      
      void dem::records::State::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         _senderDestinationRank = destination;
         
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message dem::records::State "
               << toString()
               << " to node " << destination
               << ": " << tarch::parallel::MPIReturnValueToString(result);
               _log.error( "send(int)",msg.str() );
            }
            
         }
         else {
         
            MPI_Request* sendRequestHandle = new MPI_Request();
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
               msg << "was not able to send message dem::records::State "
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
                  msg << "testing for finished send task for dem::records::State "
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
                  "dem::records::State",
                  "send(int)", destination,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "dem::records::State",
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
      
      
      
      void dem::records::State::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            MPI_Status  status;
            const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
            _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
            if ( result != MPI_SUCCESS ) {
               std::ostringstream msg;
               msg << "failed to start to receive dem::records::State from node "
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
               msg << "failed to start to receive dem::records::State from node "
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
                  msg << "testing for finished receive task for dem::records::State failed: "
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
                  "dem::records::State",
                  "receive(int)", source,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "dem::records::State",
                  "receive(int)", source,tag,1
                  );
               }
               tarch::parallel::Node::getInstance().receiveDanglingMessages();
               usleep(communicateSleep);
               
            }
            
            delete sendRequestHandle;
            
            _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
            #ifdef Debug
            _log.debug("receive(int,int)", "received " + toString() ); 
            #endif
            
         }
         
      }
      
      
      
      bool dem::records::State::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
      
      int dem::records::State::getSenderRank() const {
         assertion( _senderDestinationRank!=-1 );
         return _senderDestinationRank;
         
      }
   #endif
   
   
   dem::records::StatePacked::PersistentRecords::PersistentRecords() {
      if ((6 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(short int))));
      
   }
   
   
   dem::records::StatePacked::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
   _numberOfContactPoints(numberOfContactPoints),
   _numberOfParticleReassignments(numberOfParticleReassignments),
   _numberOfTriangleComparisons(numberOfTriangleComparisons),
   _numberOfParticleComparisons(numberOfParticleComparisons),
   _adaptiveStepSize(adaptiveStepSize),
   _timeStepSize(timeStepSize),
   _timeStep(timeStep),
   _currentTime(currentTime),
   _stepIncrement(stepIncrement),
   _twoParticlesAreClose(twoParticlesAreClose),
   _twoParticlesSeparate(twoParticlesSeparate),
   _numberOfParticles(numberOfParticles),
   _numberOfObstacles(numberOfObstacles),
   _prescribedMinimumMeshWidth(prescribedMinimumMeshWidth),
   _prescribedMaximumMeshWidth(prescribedMaximumMeshWidth),
   _maxVelocityApproach(maxVelocityApproach),
   _maxVelocityTravel(maxVelocityTravel),
   _isTraversalInverted(isTraversalInverted) {
      setHasRefined(hasRefined);
      setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
      setHasErased(hasErased);
      setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
      setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
      setHasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration);
      if ((6 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(short int))));
      
   }
   
   dem::records::StatePacked::StatePacked() {
      if ((6 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(short int))));
      
   }
   
   
   dem::records::StatePacked::StatePacked(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._numberOfParticleComparisons, persistentRecords._adaptiveStepSize, persistentRecords._timeStepSize, persistentRecords._timeStep, persistentRecords._currentTime, persistentRecords._stepIncrement, persistentRecords._twoParticlesAreClose, persistentRecords._twoParticlesSeparate, persistentRecords._numberOfParticles, persistentRecords._numberOfObstacles, persistentRecords._prescribedMinimumMeshWidth, persistentRecords._prescribedMaximumMeshWidth, persistentRecords._maxVelocityApproach, persistentRecords._maxVelocityTravel, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords.getHasModifiedGridInPreviousIteration(), persistentRecords._isTraversalInverted) {
      if ((6 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(short int))));
      
   }
   
   
   dem::records::StatePacked::StatePacked(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
   _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, numberOfParticleComparisons, adaptiveStepSize, timeStepSize, timeStep, currentTime, stepIncrement, twoParticlesAreClose, twoParticlesSeparate, numberOfParticles, numberOfObstacles, prescribedMinimumMeshWidth, prescribedMaximumMeshWidth, maxVelocityApproach, maxVelocityTravel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted) {
      if ((6 >= (8 * sizeof(short int)))) {
         std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
         std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
         std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
      }
      assertion((6 < (8 * sizeof(short int))));
      
   }
   
   
   dem::records::StatePacked::~StatePacked() { }
   
   
   
   std::string dem::records::StatePacked::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::StatePacked::toString (std::ostream& out) const {
      out << "("; 
      out << "numberOfContactPoints:" << getNumberOfContactPoints();
      out << ",";
      out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
      out << ",";
      out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
      out << ",";
      out << "numberOfParticleComparisons:" << getNumberOfParticleComparisons();
      out << ",";
      out << "adaptiveStepSize:" << getAdaptiveStepSize();
      out << ",";
      out << "timeStepSize:" << getTimeStepSize();
      out << ",";
      out << "timeStep:" << getTimeStep();
      out << ",";
      out << "currentTime:" << getCurrentTime();
      out << ",";
      out << "stepIncrement:" << getStepIncrement();
      out << ",";
      out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
      out << ",";
      out << "twoParticlesSeparate:" << getTwoParticlesSeparate();
      out << ",";
      out << "numberOfParticles:" << getNumberOfParticles();
      out << ",";
      out << "numberOfObstacles:" << getNumberOfObstacles();
      out << ",";
      out << "prescribedMinimumMeshWidth:" << getPrescribedMinimumMeshWidth();
      out << ",";
      out << "prescribedMaximumMeshWidth:" << getPrescribedMaximumMeshWidth();
      out << ",";
      out << "maxVelocityApproach:" << getMaxVelocityApproach();
      out << ",";
      out << "maxVelocityTravel:" << getMaxVelocityTravel();
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
      out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
      out << ",";
      out << "isTraversalInverted:" << getIsTraversalInverted();
      out <<  ")";
   }
   
   
   dem::records::StatePacked::PersistentRecords dem::records::StatePacked::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::State dem::records::StatePacked::convert() const{
      return State(
         getNumberOfContactPoints(),
         getNumberOfParticleReassignments(),
         getNumberOfTriangleComparisons(),
         getNumberOfParticleComparisons(),
         getAdaptiveStepSize(),
         getTimeStepSize(),
         getTimeStep(),
         getCurrentTime(),
         getStepIncrement(),
         getTwoParticlesAreClose(),
         getTwoParticlesSeparate(),
         getNumberOfParticles(),
         getNumberOfObstacles(),
         getPrescribedMinimumMeshWidth(),
         getPrescribedMaximumMeshWidth(),
         getMaxVelocityApproach(),
         getMaxVelocityTravel(),
         getHasRefined(),
         getHasTriggeredRefinementForNextIteration(),
         getHasErased(),
         getHasTriggeredEraseForNextIteration(),
         getHasChangedVertexOrCellState(),
         getHasModifiedGridInPreviousIteration(),
         getIsTraversalInverted()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::StatePacked::_log( "dem::records::StatePacked" );
      
      MPI_Datatype dem::records::StatePacked::Datatype = 0;
      MPI_Datatype dem::records::StatePacked::FullDatatype = 0;
      
      
      void dem::records::StatePacked::initDatatype() {
         {
            StatePacked dummyStatePacked[2];
            
            #ifdef MPI2
            const int Attributes = 19;
            #else
            const int Attributes = 20;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_DOUBLE		 //numberOfContactPoints
               , MPI_DOUBLE		 //numberOfParticleReassignments
               , MPI_DOUBLE		 //numberOfTriangleComparisons
               , MPI_DOUBLE		 //numberOfParticleComparisons
               , MPI_CXX_BOOL		 //adaptiveStepSize
               , MPI_DOUBLE		 //timeStepSize
               , MPI_INT		 //timeStep
               , MPI_DOUBLE		 //currentTime
               , MPI_DOUBLE		 //stepIncrement
               , MPI_DOUBLE		 //twoParticlesAreClose
               , MPI_CXX_BOOL		 //twoParticlesSeparate
               , MPI_INT		 //numberOfParticles
               , MPI_INT		 //numberOfObstacles
               , MPI_DOUBLE		 //prescribedMinimumMeshWidth
               , MPI_DOUBLE		 //prescribedMaximumMeshWidth
               , MPI_DOUBLE		 //maxVelocityApproach
               , MPI_DOUBLE		 //maxVelocityTravel
               , MPI_CXX_BOOL		 //isTraversalInverted
               , MPI_SHORT		 //_packedRecords0
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 1		 //numberOfContactPoints
               , 1		 //numberOfParticleReassignments
               , 1		 //numberOfTriangleComparisons
               , 1		 //numberOfParticleComparisons
               , 1		 //adaptiveStepSize
               , 1		 //timeStepSize
               , 1		 //timeStep
               , 1		 //currentTime
               , 1		 //stepIncrement
               , 1		 //twoParticlesAreClose
               , 1		 //twoParticlesSeparate
               , 1		 //numberOfParticles
               , 1		 //numberOfObstacles
               , 1		 //prescribedMinimumMeshWidth
               , 1		 //prescribedMaximumMeshWidth
               , 1		 //maxVelocityApproach
               , 1		 //maxVelocityTravel
               , 1		 //isTraversalInverted
               , 1		 //_packedRecords0
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[17] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[17] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[18] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[18] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(StatePacked)), i, disp[i], Attributes, sizeof(StatePacked));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]))), 		&disp[19] );
            disp[19] -= base;
            disp[19] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &StatePacked::Datatype );
            MPI_Type_commit( &StatePacked::Datatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::Datatype);
            MPI_Type_commit( &StatePacked::Datatype );
            #endif
            
         }
         {
            StatePacked dummyStatePacked[2];
            
            #ifdef MPI2
            const int Attributes = 19;
            #else
            const int Attributes = 20;
            #endif
            MPI_Datatype subtypes[Attributes] = {
                 MPI_DOUBLE		 //numberOfContactPoints
               , MPI_DOUBLE		 //numberOfParticleReassignments
               , MPI_DOUBLE		 //numberOfTriangleComparisons
               , MPI_DOUBLE		 //numberOfParticleComparisons
               , MPI_CXX_BOOL		 //adaptiveStepSize
               , MPI_DOUBLE		 //timeStepSize
               , MPI_INT		 //timeStep
               , MPI_DOUBLE		 //currentTime
               , MPI_DOUBLE		 //stepIncrement
               , MPI_DOUBLE		 //twoParticlesAreClose
               , MPI_CXX_BOOL		 //twoParticlesSeparate
               , MPI_INT		 //numberOfParticles
               , MPI_INT		 //numberOfObstacles
               , MPI_DOUBLE		 //prescribedMinimumMeshWidth
               , MPI_DOUBLE		 //prescribedMaximumMeshWidth
               , MPI_DOUBLE		 //maxVelocityApproach
               , MPI_DOUBLE		 //maxVelocityTravel
               , MPI_CXX_BOOL		 //isTraversalInverted
               , MPI_SHORT		 //_packedRecords0
               #ifndef MPI2
               , MPI_UB
               #endif
               
            };
            
            int blocklen[Attributes] = {
                 1		 //numberOfContactPoints
               , 1		 //numberOfParticleReassignments
               , 1		 //numberOfTriangleComparisons
               , 1		 //numberOfParticleComparisons
               , 1		 //adaptiveStepSize
               , 1		 //timeStepSize
               , 1		 //timeStep
               , 1		 //currentTime
               , 1		 //stepIncrement
               , 1		 //twoParticlesAreClose
               , 1		 //twoParticlesSeparate
               , 1		 //numberOfParticles
               , 1		 //numberOfObstacles
               , 1		 //prescribedMinimumMeshWidth
               , 1		 //prescribedMaximumMeshWidth
               , 1		 //maxVelocityApproach
               , 1		 //maxVelocityTravel
               , 1		 //isTraversalInverted
               , 1		 //_packedRecords0
               #ifndef MPI2
               , 1
               #endif
               
            };
            
            MPI_Aint  disp[Attributes];
            MPI_Aint  base;
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[17] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[17] );
            #endif
            #ifdef MPI2
            MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[18] );
            #else
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[18] );
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
               assertion4(disp[i]<static_cast<int>(sizeof(StatePacked)), i, disp[i], Attributes, sizeof(StatePacked));
            }
            #ifndef MPI2
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]))), 		&disp[19] );
            disp[19] -= base;
            disp[19] += disp[0];
            #endif
            #ifdef MPI2
            MPI_Datatype tmpType; 
            MPI_Aint lowerBound, typeExtent; 
            MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
            MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
            MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &StatePacked::FullDatatype );
            MPI_Type_commit( &StatePacked::FullDatatype );
            #else
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::FullDatatype);
            MPI_Type_commit( &StatePacked::FullDatatype );
            #endif
            
         }
         
      }
      
      
      void dem::records::StatePacked::shutdownDatatype() {
         MPI_Type_free( &StatePacked::Datatype );
         MPI_Type_free( &StatePacked::FullDatatype );
         
      }
      
      void dem::records::StatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         _senderDestinationRank = destination;
         
         if (communicateSleep<0) {
         
            const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
            if  (result!=MPI_SUCCESS) {
               std::ostringstream msg;
               msg << "was not able to send message dem::records::StatePacked "
               << toString()
               << " to node " << destination
               << ": " << tarch::parallel::MPIReturnValueToString(result);
               _log.error( "send(int)",msg.str() );
            }
            
         }
         else {
         
            MPI_Request* sendRequestHandle = new MPI_Request();
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
               msg << "was not able to send message dem::records::StatePacked "
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
                  msg << "testing for finished send task for dem::records::StatePacked "
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
                  "dem::records::StatePacked",
                  "send(int)", destination,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "dem::records::StatePacked",
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
      
      
      
      void dem::records::StatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
         if (communicateSleep<0) {
         
            MPI_Status  status;
            const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
            _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
            if ( result != MPI_SUCCESS ) {
               std::ostringstream msg;
               msg << "failed to start to receive dem::records::StatePacked from node "
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
               msg << "failed to start to receive dem::records::StatePacked from node "
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
                  msg << "testing for finished receive task for dem::records::StatePacked failed: "
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
                  "dem::records::StatePacked",
                  "receive(int)", source,tag,1
                  );
                  triggeredTimeoutWarning = true;
               }
               if (
                  tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                  (clock()>timeOutShutdown)
               ) {
                  tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                  "dem::records::StatePacked",
                  "receive(int)", source,tag,1
                  );
               }
               tarch::parallel::Node::getInstance().receiveDanglingMessages();
               usleep(communicateSleep);
               
            }
            
            delete sendRequestHandle;
            
            _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
            #ifdef Debug
            _log.debug("receive(int,int)", "received " + toString() ); 
            #endif
            
         }
         
      }
      
      
      
      bool dem::records::StatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
      
      int dem::records::StatePacked::getSenderRank() const {
         assertion( _senderDestinationRank!=-1 );
         return _senderDestinationRank;
         
      }
   #endif
   
   
   #elif defined(TrackGridStatistics) && defined(Parallel)
      dem::records::State::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::State::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
      _numberOfContactPoints(numberOfContactPoints),
      _numberOfParticleReassignments(numberOfParticleReassignments),
      _numberOfTriangleComparisons(numberOfTriangleComparisons),
      _numberOfParticleComparisons(numberOfParticleComparisons),
      _adaptiveStepSize(adaptiveStepSize),
      _timeStepSize(timeStepSize),
      _timeStep(timeStep),
      _currentTime(currentTime),
      _stepIncrement(stepIncrement),
      _twoParticlesAreClose(twoParticlesAreClose),
      _twoParticlesSeparate(twoParticlesSeparate),
      _numberOfParticles(numberOfParticles),
      _numberOfObstacles(numberOfObstacles),
      _prescribedMinimumMeshWidth(prescribedMinimumMeshWidth),
      _prescribedMaximumMeshWidth(prescribedMaximumMeshWidth),
      _maxVelocityApproach(maxVelocityApproach),
      _maxVelocityTravel(maxVelocityTravel),
      _minMeshWidth(minMeshWidth),
      _maxMeshWidth(maxMeshWidth),
      _numberOfInnerVertices(numberOfInnerVertices),
      _numberOfBoundaryVertices(numberOfBoundaryVertices),
      _numberOfOuterVertices(numberOfOuterVertices),
      _numberOfInnerCells(numberOfInnerCells),
      _numberOfOuterCells(numberOfOuterCells),
      _numberOfInnerLeafVertices(numberOfInnerLeafVertices),
      _numberOfBoundaryLeafVertices(numberOfBoundaryLeafVertices),
      _numberOfOuterLeafVertices(numberOfOuterLeafVertices),
      _numberOfInnerLeafCells(numberOfInnerLeafCells),
      _numberOfOuterLeafCells(numberOfOuterLeafCells),
      _maxLevel(maxLevel),
      _hasRefined(hasRefined),
      _hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
      _hasErased(hasErased),
      _hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
      _hasChangedVertexOrCellState(hasChangedVertexOrCellState),
      _hasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration),
      _isTraversalInverted(isTraversalInverted),
      _reduceStateAndCell(reduceStateAndCell),
      _couldNotEraseDueToDecompositionFlag(couldNotEraseDueToDecompositionFlag),
      _subWorkerIsInvolvedInJoinOrFork(subWorkerIsInvolvedInJoinOrFork) {
         
      }
      
      dem::records::State::State() {
         
      }
      
      
      dem::records::State::State(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._numberOfParticleComparisons, persistentRecords._adaptiveStepSize, persistentRecords._timeStepSize, persistentRecords._timeStep, persistentRecords._currentTime, persistentRecords._stepIncrement, persistentRecords._twoParticlesAreClose, persistentRecords._twoParticlesSeparate, persistentRecords._numberOfParticles, persistentRecords._numberOfObstacles, persistentRecords._prescribedMinimumMeshWidth, persistentRecords._prescribedMaximumMeshWidth, persistentRecords._maxVelocityApproach, persistentRecords._maxVelocityTravel, persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._numberOfInnerLeafVertices, persistentRecords._numberOfBoundaryLeafVertices, persistentRecords._numberOfOuterLeafVertices, persistentRecords._numberOfInnerLeafCells, persistentRecords._numberOfOuterLeafCells, persistentRecords._maxLevel, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._hasModifiedGridInPreviousIteration, persistentRecords._isTraversalInverted, persistentRecords._reduceStateAndCell, persistentRecords._couldNotEraseDueToDecompositionFlag, persistentRecords._subWorkerIsInvolvedInJoinOrFork) {
         
      }
      
      
      dem::records::State::State(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
      _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, numberOfParticleComparisons, adaptiveStepSize, timeStepSize, timeStep, currentTime, stepIncrement, twoParticlesAreClose, twoParticlesSeparate, numberOfParticles, numberOfObstacles, prescribedMinimumMeshWidth, prescribedMaximumMeshWidth, maxVelocityApproach, maxVelocityTravel, minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, numberOfInnerLeafVertices, numberOfBoundaryLeafVertices, numberOfOuterLeafVertices, numberOfInnerLeafCells, numberOfOuterLeafCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted, reduceStateAndCell, couldNotEraseDueToDecompositionFlag, subWorkerIsInvolvedInJoinOrFork) {
         
      }
      
      
      dem::records::State::~State() { }
      
      
      
      std::string dem::records::State::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::State::toString (std::ostream& out) const {
         out << "("; 
         out << "numberOfContactPoints:" << getNumberOfContactPoints();
         out << ",";
         out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
         out << ",";
         out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
         out << ",";
         out << "numberOfParticleComparisons:" << getNumberOfParticleComparisons();
         out << ",";
         out << "adaptiveStepSize:" << getAdaptiveStepSize();
         out << ",";
         out << "timeStepSize:" << getTimeStepSize();
         out << ",";
         out << "timeStep:" << getTimeStep();
         out << ",";
         out << "currentTime:" << getCurrentTime();
         out << ",";
         out << "stepIncrement:" << getStepIncrement();
         out << ",";
         out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
         out << ",";
         out << "twoParticlesSeparate:" << getTwoParticlesSeparate();
         out << ",";
         out << "numberOfParticles:" << getNumberOfParticles();
         out << ",";
         out << "numberOfObstacles:" << getNumberOfObstacles();
         out << ",";
         out << "prescribedMinimumMeshWidth:" << getPrescribedMinimumMeshWidth();
         out << ",";
         out << "prescribedMaximumMeshWidth:" << getPrescribedMaximumMeshWidth();
         out << ",";
         out << "maxVelocityApproach:" << getMaxVelocityApproach();
         out << ",";
         out << "maxVelocityTravel:" << getMaxVelocityTravel();
         out << ",";
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
         out << "numberOfInnerLeafVertices:" << getNumberOfInnerLeafVertices();
         out << ",";
         out << "numberOfBoundaryLeafVertices:" << getNumberOfBoundaryLeafVertices();
         out << ",";
         out << "numberOfOuterLeafVertices:" << getNumberOfOuterLeafVertices();
         out << ",";
         out << "numberOfInnerLeafCells:" << getNumberOfInnerLeafCells();
         out << ",";
         out << "numberOfOuterLeafCells:" << getNumberOfOuterLeafCells();
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
         out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
         out << ",";
         out << "isTraversalInverted:" << getIsTraversalInverted();
         out << ",";
         out << "reduceStateAndCell:" << getReduceStateAndCell();
         out << ",";
         out << "couldNotEraseDueToDecompositionFlag:" << getCouldNotEraseDueToDecompositionFlag();
         out << ",";
         out << "subWorkerIsInvolvedInJoinOrFork:" << getSubWorkerIsInvolvedInJoinOrFork();
         out <<  ")";
      }
      
      
      dem::records::State::PersistentRecords dem::records::State::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::StatePacked dem::records::State::convert() const{
         return StatePacked(
            getNumberOfContactPoints(),
            getNumberOfParticleReassignments(),
            getNumberOfTriangleComparisons(),
            getNumberOfParticleComparisons(),
            getAdaptiveStepSize(),
            getTimeStepSize(),
            getTimeStep(),
            getCurrentTime(),
            getStepIncrement(),
            getTwoParticlesAreClose(),
            getTwoParticlesSeparate(),
            getNumberOfParticles(),
            getNumberOfObstacles(),
            getPrescribedMinimumMeshWidth(),
            getPrescribedMaximumMeshWidth(),
            getMaxVelocityApproach(),
            getMaxVelocityTravel(),
            getMinMeshWidth(),
            getMaxMeshWidth(),
            getNumberOfInnerVertices(),
            getNumberOfBoundaryVertices(),
            getNumberOfOuterVertices(),
            getNumberOfInnerCells(),
            getNumberOfOuterCells(),
            getNumberOfInnerLeafVertices(),
            getNumberOfBoundaryLeafVertices(),
            getNumberOfOuterLeafVertices(),
            getNumberOfInnerLeafCells(),
            getNumberOfOuterLeafCells(),
            getMaxLevel(),
            getHasRefined(),
            getHasTriggeredRefinementForNextIteration(),
            getHasErased(),
            getHasTriggeredEraseForNextIteration(),
            getHasChangedVertexOrCellState(),
            getHasModifiedGridInPreviousIteration(),
            getIsTraversalInverted(),
            getReduceStateAndCell(),
            getCouldNotEraseDueToDecompositionFlag(),
            getSubWorkerIsInvolvedInJoinOrFork()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::State::_log( "dem::records::State" );
         
         MPI_Datatype dem::records::State::Datatype = 0;
         MPI_Datatype dem::records::State::FullDatatype = 0;
         
         
         void dem::records::State::initDatatype() {
            {
               State dummyState[2];
               
               #ifdef MPI2
               const int Attributes = 40;
               #else
               const int Attributes = 41;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_DOUBLE		 //numberOfInnerLeafVertices
                  , MPI_DOUBLE		 //numberOfBoundaryLeafVertices
                  , MPI_DOUBLE		 //numberOfOuterLeafVertices
                  , MPI_DOUBLE		 //numberOfInnerLeafCells
                  , MPI_DOUBLE		 //numberOfOuterLeafCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //hasRefined
                  , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
                  , MPI_CXX_BOOL		 //hasErased
                  , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
                  , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
                  , MPI_CXX_BOOL		 //hasModifiedGridInPreviousIteration
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_CXX_BOOL		 //reduceStateAndCell
                  , MPI_CXX_BOOL		 //couldNotEraseDueToDecompositionFlag
                  , MPI_CXX_BOOL		 //subWorkerIsInvolvedInJoinOrFork
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //numberOfInnerLeafVertices
                  , 1		 //numberOfBoundaryLeafVertices
                  , 1		 //numberOfOuterLeafVertices
                  , 1		 //numberOfInnerLeafCells
                  , 1		 //numberOfOuterLeafCells
                  , 1		 //maxLevel
                  , 1		 //hasRefined
                  , 1		 //hasTriggeredRefinementForNextIteration
                  , 1		 //hasErased
                  , 1		 //hasTriggeredEraseForNextIteration
                  , 1		 //hasChangedVertexOrCellState
                  , 1		 //hasModifiedGridInPreviousIteration
                  , 1		 //isTraversalInverted
                  , 1		 //reduceStateAndCell
                  , 1		 //couldNotEraseDueToDecompositionFlag
                  , 1		 //subWorkerIsInvolvedInJoinOrFork
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[30] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[30] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[31] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[31] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[32] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[32] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[33] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[33] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[34] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[34] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[35] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[35] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[36] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[36] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[37] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[37] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[38] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[38] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[39] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[39] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(State)), i, disp[i], Attributes, sizeof(State));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]))), 		&disp[40] );
               disp[40] -= base;
               disp[40] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &State::Datatype );
               MPI_Type_commit( &State::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::Datatype);
               MPI_Type_commit( &State::Datatype );
               #endif
               
            }
            {
               State dummyState[2];
               
               #ifdef MPI2
               const int Attributes = 40;
               #else
               const int Attributes = 41;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_DOUBLE		 //numberOfInnerLeafVertices
                  , MPI_DOUBLE		 //numberOfBoundaryLeafVertices
                  , MPI_DOUBLE		 //numberOfOuterLeafVertices
                  , MPI_DOUBLE		 //numberOfInnerLeafCells
                  , MPI_DOUBLE		 //numberOfOuterLeafCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //hasRefined
                  , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
                  , MPI_CXX_BOOL		 //hasErased
                  , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
                  , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
                  , MPI_CXX_BOOL		 //hasModifiedGridInPreviousIteration
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_CXX_BOOL		 //reduceStateAndCell
                  , MPI_CXX_BOOL		 //couldNotEraseDueToDecompositionFlag
                  , MPI_CXX_BOOL		 //subWorkerIsInvolvedInJoinOrFork
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //numberOfInnerLeafVertices
                  , 1		 //numberOfBoundaryLeafVertices
                  , 1		 //numberOfOuterLeafVertices
                  , 1		 //numberOfInnerLeafCells
                  , 1		 //numberOfOuterLeafCells
                  , 1		 //maxLevel
                  , 1		 //hasRefined
                  , 1		 //hasTriggeredRefinementForNextIteration
                  , 1		 //hasErased
                  , 1		 //hasTriggeredEraseForNextIteration
                  , 1		 //hasChangedVertexOrCellState
                  , 1		 //hasModifiedGridInPreviousIteration
                  , 1		 //isTraversalInverted
                  , 1		 //reduceStateAndCell
                  , 1		 //couldNotEraseDueToDecompositionFlag
                  , 1		 //subWorkerIsInvolvedInJoinOrFork
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[30] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[30] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[31] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[31] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[32] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[32] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[33] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[33] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[34] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[34] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[35] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[35] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[36] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[36] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[37] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[37] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[38] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[38] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[39] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[39] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(State)), i, disp[i], Attributes, sizeof(State));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]))), 		&disp[40] );
               disp[40] -= base;
               disp[40] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &State::FullDatatype );
               MPI_Type_commit( &State::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::FullDatatype);
               MPI_Type_commit( &State::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::State::shutdownDatatype() {
            MPI_Type_free( &State::Datatype );
            MPI_Type_free( &State::FullDatatype );
            
         }
         
         void dem::records::State::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::State "
                  << toString()
                  << " to node " << destination
                  << ": " << tarch::parallel::MPIReturnValueToString(result);
                  _log.error( "send(int)",msg.str() );
               }
               
            }
            else {
            
               MPI_Request* sendRequestHandle = new MPI_Request();
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
                  msg << "was not able to send message dem::records::State "
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
                     msg << "testing for finished send task for dem::records::State "
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
                     "dem::records::State",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::State",
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
         
         
         
         void dem::records::State::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::State from node "
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
                  msg << "failed to start to receive dem::records::State from node "
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
                     msg << "testing for finished receive task for dem::records::State failed: "
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
                     "dem::records::State",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::State",
                     "receive(int)", source,tag,1
                     );
                  }
                  tarch::parallel::Node::getInstance().receiveDanglingMessages();
                  usleep(communicateSleep);
                  
               }
               
               delete sendRequestHandle;
               
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               #ifdef Debug
               _log.debug("receive(int,int)", "received " + toString() ); 
               #endif
               
            }
            
         }
         
         
         
         bool dem::records::State::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::State::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::StatePacked::PersistentRecords::PersistentRecords() {
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
      _numberOfContactPoints(numberOfContactPoints),
      _numberOfParticleReassignments(numberOfParticleReassignments),
      _numberOfTriangleComparisons(numberOfTriangleComparisons),
      _numberOfParticleComparisons(numberOfParticleComparisons),
      _adaptiveStepSize(adaptiveStepSize),
      _timeStepSize(timeStepSize),
      _timeStep(timeStep),
      _currentTime(currentTime),
      _stepIncrement(stepIncrement),
      _twoParticlesAreClose(twoParticlesAreClose),
      _twoParticlesSeparate(twoParticlesSeparate),
      _numberOfParticles(numberOfParticles),
      _numberOfObstacles(numberOfObstacles),
      _prescribedMinimumMeshWidth(prescribedMinimumMeshWidth),
      _prescribedMaximumMeshWidth(prescribedMaximumMeshWidth),
      _maxVelocityApproach(maxVelocityApproach),
      _maxVelocityTravel(maxVelocityTravel),
      _minMeshWidth(minMeshWidth),
      _maxMeshWidth(maxMeshWidth),
      _numberOfInnerVertices(numberOfInnerVertices),
      _numberOfBoundaryVertices(numberOfBoundaryVertices),
      _numberOfOuterVertices(numberOfOuterVertices),
      _numberOfInnerCells(numberOfInnerCells),
      _numberOfOuterCells(numberOfOuterCells),
      _numberOfInnerLeafVertices(numberOfInnerLeafVertices),
      _numberOfBoundaryLeafVertices(numberOfBoundaryLeafVertices),
      _numberOfOuterLeafVertices(numberOfOuterLeafVertices),
      _numberOfInnerLeafCells(numberOfInnerLeafCells),
      _numberOfOuterLeafCells(numberOfOuterLeafCells),
      _maxLevel(maxLevel),
      _isTraversalInverted(isTraversalInverted) {
         setHasRefined(hasRefined);
         setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
         setHasErased(hasErased);
         setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
         setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
         setHasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration);
         setReduceStateAndCell(reduceStateAndCell);
         setCouldNotEraseDueToDecompositionFlag(couldNotEraseDueToDecompositionFlag);
         setSubWorkerIsInvolvedInJoinOrFork(subWorkerIsInvolvedInJoinOrFork);
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      dem::records::StatePacked::StatePacked() {
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::StatePacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._numberOfParticleComparisons, persistentRecords._adaptiveStepSize, persistentRecords._timeStepSize, persistentRecords._timeStep, persistentRecords._currentTime, persistentRecords._stepIncrement, persistentRecords._twoParticlesAreClose, persistentRecords._twoParticlesSeparate, persistentRecords._numberOfParticles, persistentRecords._numberOfObstacles, persistentRecords._prescribedMinimumMeshWidth, persistentRecords._prescribedMaximumMeshWidth, persistentRecords._maxVelocityApproach, persistentRecords._maxVelocityTravel, persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._numberOfInnerLeafVertices, persistentRecords._numberOfBoundaryLeafVertices, persistentRecords._numberOfOuterLeafVertices, persistentRecords._numberOfInnerLeafCells, persistentRecords._numberOfOuterLeafCells, persistentRecords._maxLevel, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords.getHasModifiedGridInPreviousIteration(), persistentRecords._isTraversalInverted, persistentRecords.getReduceStateAndCell(), persistentRecords.getCouldNotEraseDueToDecompositionFlag(), persistentRecords.getSubWorkerIsInvolvedInJoinOrFork()) {
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::StatePacked(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
      _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, numberOfParticleComparisons, adaptiveStepSize, timeStepSize, timeStep, currentTime, stepIncrement, twoParticlesAreClose, twoParticlesSeparate, numberOfParticles, numberOfObstacles, prescribedMinimumMeshWidth, prescribedMaximumMeshWidth, maxVelocityApproach, maxVelocityTravel, minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, numberOfInnerLeafVertices, numberOfBoundaryLeafVertices, numberOfOuterLeafVertices, numberOfInnerLeafCells, numberOfOuterLeafCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted, reduceStateAndCell, couldNotEraseDueToDecompositionFlag, subWorkerIsInvolvedInJoinOrFork) {
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::~StatePacked() { }
      
      
      
      std::string dem::records::StatePacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::StatePacked::toString (std::ostream& out) const {
         out << "("; 
         out << "numberOfContactPoints:" << getNumberOfContactPoints();
         out << ",";
         out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
         out << ",";
         out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
         out << ",";
         out << "numberOfParticleComparisons:" << getNumberOfParticleComparisons();
         out << ",";
         out << "adaptiveStepSize:" << getAdaptiveStepSize();
         out << ",";
         out << "timeStepSize:" << getTimeStepSize();
         out << ",";
         out << "timeStep:" << getTimeStep();
         out << ",";
         out << "currentTime:" << getCurrentTime();
         out << ",";
         out << "stepIncrement:" << getStepIncrement();
         out << ",";
         out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
         out << ",";
         out << "twoParticlesSeparate:" << getTwoParticlesSeparate();
         out << ",";
         out << "numberOfParticles:" << getNumberOfParticles();
         out << ",";
         out << "numberOfObstacles:" << getNumberOfObstacles();
         out << ",";
         out << "prescribedMinimumMeshWidth:" << getPrescribedMinimumMeshWidth();
         out << ",";
         out << "prescribedMaximumMeshWidth:" << getPrescribedMaximumMeshWidth();
         out << ",";
         out << "maxVelocityApproach:" << getMaxVelocityApproach();
         out << ",";
         out << "maxVelocityTravel:" << getMaxVelocityTravel();
         out << ",";
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
         out << "numberOfInnerLeafVertices:" << getNumberOfInnerLeafVertices();
         out << ",";
         out << "numberOfBoundaryLeafVertices:" << getNumberOfBoundaryLeafVertices();
         out << ",";
         out << "numberOfOuterLeafVertices:" << getNumberOfOuterLeafVertices();
         out << ",";
         out << "numberOfInnerLeafCells:" << getNumberOfInnerLeafCells();
         out << ",";
         out << "numberOfOuterLeafCells:" << getNumberOfOuterLeafCells();
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
         out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
         out << ",";
         out << "isTraversalInverted:" << getIsTraversalInverted();
         out << ",";
         out << "reduceStateAndCell:" << getReduceStateAndCell();
         out << ",";
         out << "couldNotEraseDueToDecompositionFlag:" << getCouldNotEraseDueToDecompositionFlag();
         out << ",";
         out << "subWorkerIsInvolvedInJoinOrFork:" << getSubWorkerIsInvolvedInJoinOrFork();
         out <<  ")";
      }
      
      
      dem::records::StatePacked::PersistentRecords dem::records::StatePacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::State dem::records::StatePacked::convert() const{
         return State(
            getNumberOfContactPoints(),
            getNumberOfParticleReassignments(),
            getNumberOfTriangleComparisons(),
            getNumberOfParticleComparisons(),
            getAdaptiveStepSize(),
            getTimeStepSize(),
            getTimeStep(),
            getCurrentTime(),
            getStepIncrement(),
            getTwoParticlesAreClose(),
            getTwoParticlesSeparate(),
            getNumberOfParticles(),
            getNumberOfObstacles(),
            getPrescribedMinimumMeshWidth(),
            getPrescribedMaximumMeshWidth(),
            getMaxVelocityApproach(),
            getMaxVelocityTravel(),
            getMinMeshWidth(),
            getMaxMeshWidth(),
            getNumberOfInnerVertices(),
            getNumberOfBoundaryVertices(),
            getNumberOfOuterVertices(),
            getNumberOfInnerCells(),
            getNumberOfOuterCells(),
            getNumberOfInnerLeafVertices(),
            getNumberOfBoundaryLeafVertices(),
            getNumberOfOuterLeafVertices(),
            getNumberOfInnerLeafCells(),
            getNumberOfOuterLeafCells(),
            getMaxLevel(),
            getHasRefined(),
            getHasTriggeredRefinementForNextIteration(),
            getHasErased(),
            getHasTriggeredEraseForNextIteration(),
            getHasChangedVertexOrCellState(),
            getHasModifiedGridInPreviousIteration(),
            getIsTraversalInverted(),
            getReduceStateAndCell(),
            getCouldNotEraseDueToDecompositionFlag(),
            getSubWorkerIsInvolvedInJoinOrFork()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::StatePacked::_log( "dem::records::StatePacked" );
         
         MPI_Datatype dem::records::StatePacked::Datatype = 0;
         MPI_Datatype dem::records::StatePacked::FullDatatype = 0;
         
         
         void dem::records::StatePacked::initDatatype() {
            {
               StatePacked dummyStatePacked[2];
               
               #ifdef MPI2
               const int Attributes = 32;
               #else
               const int Attributes = 33;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_DOUBLE		 //numberOfInnerLeafVertices
                  , MPI_DOUBLE		 //numberOfBoundaryLeafVertices
                  , MPI_DOUBLE		 //numberOfOuterLeafVertices
                  , MPI_DOUBLE		 //numberOfInnerLeafCells
                  , MPI_DOUBLE		 //numberOfOuterLeafCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //numberOfInnerLeafVertices
                  , 1		 //numberOfBoundaryLeafVertices
                  , 1		 //numberOfOuterLeafVertices
                  , 1		 //numberOfInnerLeafCells
                  , 1		 //numberOfOuterLeafCells
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
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[30] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[30] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[31] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[31] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(StatePacked)), i, disp[i], Attributes, sizeof(StatePacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]))), 		&disp[32] );
               disp[32] -= base;
               disp[32] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &StatePacked::Datatype );
               MPI_Type_commit( &StatePacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::Datatype);
               MPI_Type_commit( &StatePacked::Datatype );
               #endif
               
            }
            {
               StatePacked dummyStatePacked[2];
               
               #ifdef MPI2
               const int Attributes = 32;
               #else
               const int Attributes = 33;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_DOUBLE		 //numberOfInnerLeafVertices
                  , MPI_DOUBLE		 //numberOfBoundaryLeafVertices
                  , MPI_DOUBLE		 //numberOfOuterLeafVertices
                  , MPI_DOUBLE		 //numberOfInnerLeafCells
                  , MPI_DOUBLE		 //numberOfOuterLeafCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //numberOfInnerLeafVertices
                  , 1		 //numberOfBoundaryLeafVertices
                  , 1		 //numberOfOuterLeafVertices
                  , 1		 //numberOfInnerLeafCells
                  , 1		 //numberOfOuterLeafCells
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
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[30] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[30] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[31] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[31] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(StatePacked)), i, disp[i], Attributes, sizeof(StatePacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]))), 		&disp[32] );
               disp[32] -= base;
               disp[32] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &StatePacked::FullDatatype );
               MPI_Type_commit( &StatePacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::FullDatatype);
               MPI_Type_commit( &StatePacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::StatePacked::shutdownDatatype() {
            MPI_Type_free( &StatePacked::Datatype );
            MPI_Type_free( &StatePacked::FullDatatype );
            
         }
         
         void dem::records::StatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::StatePacked "
                  << toString()
                  << " to node " << destination
                  << ": " << tarch::parallel::MPIReturnValueToString(result);
                  _log.error( "send(int)",msg.str() );
               }
               
            }
            else {
            
               MPI_Request* sendRequestHandle = new MPI_Request();
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
                  msg << "was not able to send message dem::records::StatePacked "
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
                     msg << "testing for finished send task for dem::records::StatePacked "
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
                     "dem::records::StatePacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::StatePacked",
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
         
         
         
         void dem::records::StatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::StatePacked from node "
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
                  msg << "failed to start to receive dem::records::StatePacked from node "
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
                     msg << "testing for finished receive task for dem::records::StatePacked failed: "
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
                     "dem::records::StatePacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::StatePacked",
                     "receive(int)", source,tag,1
                     );
                  }
                  tarch::parallel::Node::getInstance().receiveDanglingMessages();
                  usleep(communicateSleep);
                  
               }
               
               delete sendRequestHandle;
               
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               #ifdef Debug
               _log.debug("receive(int,int)", "received " + toString() ); 
               #endif
               
            }
            
         }
         
         
         
         bool dem::records::StatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::StatePacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif defined(TrackGridStatistics) && !defined(Parallel)
      dem::records::State::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::State::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
      _numberOfContactPoints(numberOfContactPoints),
      _numberOfParticleReassignments(numberOfParticleReassignments),
      _numberOfTriangleComparisons(numberOfTriangleComparisons),
      _numberOfParticleComparisons(numberOfParticleComparisons),
      _adaptiveStepSize(adaptiveStepSize),
      _timeStepSize(timeStepSize),
      _timeStep(timeStep),
      _currentTime(currentTime),
      _stepIncrement(stepIncrement),
      _twoParticlesAreClose(twoParticlesAreClose),
      _twoParticlesSeparate(twoParticlesSeparate),
      _numberOfParticles(numberOfParticles),
      _numberOfObstacles(numberOfObstacles),
      _prescribedMinimumMeshWidth(prescribedMinimumMeshWidth),
      _prescribedMaximumMeshWidth(prescribedMaximumMeshWidth),
      _maxVelocityApproach(maxVelocityApproach),
      _maxVelocityTravel(maxVelocityTravel),
      _minMeshWidth(minMeshWidth),
      _maxMeshWidth(maxMeshWidth),
      _numberOfInnerVertices(numberOfInnerVertices),
      _numberOfBoundaryVertices(numberOfBoundaryVertices),
      _numberOfOuterVertices(numberOfOuterVertices),
      _numberOfInnerCells(numberOfInnerCells),
      _numberOfOuterCells(numberOfOuterCells),
      _numberOfInnerLeafVertices(numberOfInnerLeafVertices),
      _numberOfBoundaryLeafVertices(numberOfBoundaryLeafVertices),
      _numberOfOuterLeafVertices(numberOfOuterLeafVertices),
      _numberOfInnerLeafCells(numberOfInnerLeafCells),
      _numberOfOuterLeafCells(numberOfOuterLeafCells),
      _maxLevel(maxLevel),
      _hasRefined(hasRefined),
      _hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
      _hasErased(hasErased),
      _hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
      _hasChangedVertexOrCellState(hasChangedVertexOrCellState),
      _hasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration),
      _isTraversalInverted(isTraversalInverted) {
         
      }
      
      dem::records::State::State() {
         
      }
      
      
      dem::records::State::State(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._numberOfParticleComparisons, persistentRecords._adaptiveStepSize, persistentRecords._timeStepSize, persistentRecords._timeStep, persistentRecords._currentTime, persistentRecords._stepIncrement, persistentRecords._twoParticlesAreClose, persistentRecords._twoParticlesSeparate, persistentRecords._numberOfParticles, persistentRecords._numberOfObstacles, persistentRecords._prescribedMinimumMeshWidth, persistentRecords._prescribedMaximumMeshWidth, persistentRecords._maxVelocityApproach, persistentRecords._maxVelocityTravel, persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._numberOfInnerLeafVertices, persistentRecords._numberOfBoundaryLeafVertices, persistentRecords._numberOfOuterLeafVertices, persistentRecords._numberOfInnerLeafCells, persistentRecords._numberOfOuterLeafCells, persistentRecords._maxLevel, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._hasModifiedGridInPreviousIteration, persistentRecords._isTraversalInverted) {
         
      }
      
      
      dem::records::State::State(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
      _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, numberOfParticleComparisons, adaptiveStepSize, timeStepSize, timeStep, currentTime, stepIncrement, twoParticlesAreClose, twoParticlesSeparate, numberOfParticles, numberOfObstacles, prescribedMinimumMeshWidth, prescribedMaximumMeshWidth, maxVelocityApproach, maxVelocityTravel, minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, numberOfInnerLeafVertices, numberOfBoundaryLeafVertices, numberOfOuterLeafVertices, numberOfInnerLeafCells, numberOfOuterLeafCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted) {
         
      }
      
      
      dem::records::State::~State() { }
      
      
      
      std::string dem::records::State::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::State::toString (std::ostream& out) const {
         out << "("; 
         out << "numberOfContactPoints:" << getNumberOfContactPoints();
         out << ",";
         out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
         out << ",";
         out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
         out << ",";
         out << "numberOfParticleComparisons:" << getNumberOfParticleComparisons();
         out << ",";
         out << "adaptiveStepSize:" << getAdaptiveStepSize();
         out << ",";
         out << "timeStepSize:" << getTimeStepSize();
         out << ",";
         out << "timeStep:" << getTimeStep();
         out << ",";
         out << "currentTime:" << getCurrentTime();
         out << ",";
         out << "stepIncrement:" << getStepIncrement();
         out << ",";
         out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
         out << ",";
         out << "twoParticlesSeparate:" << getTwoParticlesSeparate();
         out << ",";
         out << "numberOfParticles:" << getNumberOfParticles();
         out << ",";
         out << "numberOfObstacles:" << getNumberOfObstacles();
         out << ",";
         out << "prescribedMinimumMeshWidth:" << getPrescribedMinimumMeshWidth();
         out << ",";
         out << "prescribedMaximumMeshWidth:" << getPrescribedMaximumMeshWidth();
         out << ",";
         out << "maxVelocityApproach:" << getMaxVelocityApproach();
         out << ",";
         out << "maxVelocityTravel:" << getMaxVelocityTravel();
         out << ",";
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
         out << "numberOfInnerLeafVertices:" << getNumberOfInnerLeafVertices();
         out << ",";
         out << "numberOfBoundaryLeafVertices:" << getNumberOfBoundaryLeafVertices();
         out << ",";
         out << "numberOfOuterLeafVertices:" << getNumberOfOuterLeafVertices();
         out << ",";
         out << "numberOfInnerLeafCells:" << getNumberOfInnerLeafCells();
         out << ",";
         out << "numberOfOuterLeafCells:" << getNumberOfOuterLeafCells();
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
         out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
         out << ",";
         out << "isTraversalInverted:" << getIsTraversalInverted();
         out <<  ")";
      }
      
      
      dem::records::State::PersistentRecords dem::records::State::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::StatePacked dem::records::State::convert() const{
         return StatePacked(
            getNumberOfContactPoints(),
            getNumberOfParticleReassignments(),
            getNumberOfTriangleComparisons(),
            getNumberOfParticleComparisons(),
            getAdaptiveStepSize(),
            getTimeStepSize(),
            getTimeStep(),
            getCurrentTime(),
            getStepIncrement(),
            getTwoParticlesAreClose(),
            getTwoParticlesSeparate(),
            getNumberOfParticles(),
            getNumberOfObstacles(),
            getPrescribedMinimumMeshWidth(),
            getPrescribedMaximumMeshWidth(),
            getMaxVelocityApproach(),
            getMaxVelocityTravel(),
            getMinMeshWidth(),
            getMaxMeshWidth(),
            getNumberOfInnerVertices(),
            getNumberOfBoundaryVertices(),
            getNumberOfOuterVertices(),
            getNumberOfInnerCells(),
            getNumberOfOuterCells(),
            getNumberOfInnerLeafVertices(),
            getNumberOfBoundaryLeafVertices(),
            getNumberOfOuterLeafVertices(),
            getNumberOfInnerLeafCells(),
            getNumberOfOuterLeafCells(),
            getMaxLevel(),
            getHasRefined(),
            getHasTriggeredRefinementForNextIteration(),
            getHasErased(),
            getHasTriggeredEraseForNextIteration(),
            getHasChangedVertexOrCellState(),
            getHasModifiedGridInPreviousIteration(),
            getIsTraversalInverted()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::State::_log( "dem::records::State" );
         
         MPI_Datatype dem::records::State::Datatype = 0;
         MPI_Datatype dem::records::State::FullDatatype = 0;
         
         
         void dem::records::State::initDatatype() {
            {
               State dummyState[2];
               
               #ifdef MPI2
               const int Attributes = 37;
               #else
               const int Attributes = 38;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_DOUBLE		 //numberOfInnerLeafVertices
                  , MPI_DOUBLE		 //numberOfBoundaryLeafVertices
                  , MPI_DOUBLE		 //numberOfOuterLeafVertices
                  , MPI_DOUBLE		 //numberOfInnerLeafCells
                  , MPI_DOUBLE		 //numberOfOuterLeafCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //hasRefined
                  , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
                  , MPI_CXX_BOOL		 //hasErased
                  , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
                  , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
                  , MPI_CXX_BOOL		 //hasModifiedGridInPreviousIteration
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //numberOfInnerLeafVertices
                  , 1		 //numberOfBoundaryLeafVertices
                  , 1		 //numberOfOuterLeafVertices
                  , 1		 //numberOfInnerLeafCells
                  , 1		 //numberOfOuterLeafCells
                  , 1		 //maxLevel
                  , 1		 //hasRefined
                  , 1		 //hasTriggeredRefinementForNextIteration
                  , 1		 //hasErased
                  , 1		 //hasTriggeredEraseForNextIteration
                  , 1		 //hasChangedVertexOrCellState
                  , 1		 //hasModifiedGridInPreviousIteration
                  , 1		 //isTraversalInverted
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[30] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[30] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[31] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[31] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[32] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[32] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[33] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[33] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[34] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[34] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[35] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[35] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[36] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[36] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(State)), i, disp[i], Attributes, sizeof(State));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]))), 		&disp[37] );
               disp[37] -= base;
               disp[37] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &State::Datatype );
               MPI_Type_commit( &State::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::Datatype);
               MPI_Type_commit( &State::Datatype );
               #endif
               
            }
            {
               State dummyState[2];
               
               #ifdef MPI2
               const int Attributes = 37;
               #else
               const int Attributes = 38;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_DOUBLE		 //numberOfInnerLeafVertices
                  , MPI_DOUBLE		 //numberOfBoundaryLeafVertices
                  , MPI_DOUBLE		 //numberOfOuterLeafVertices
                  , MPI_DOUBLE		 //numberOfInnerLeafCells
                  , MPI_DOUBLE		 //numberOfOuterLeafCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //hasRefined
                  , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
                  , MPI_CXX_BOOL		 //hasErased
                  , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
                  , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
                  , MPI_CXX_BOOL		 //hasModifiedGridInPreviousIteration
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //numberOfInnerLeafVertices
                  , 1		 //numberOfBoundaryLeafVertices
                  , 1		 //numberOfOuterLeafVertices
                  , 1		 //numberOfInnerLeafCells
                  , 1		 //numberOfOuterLeafCells
                  , 1		 //maxLevel
                  , 1		 //hasRefined
                  , 1		 //hasTriggeredRefinementForNextIteration
                  , 1		 //hasErased
                  , 1		 //hasTriggeredEraseForNextIteration
                  , 1		 //hasChangedVertexOrCellState
                  , 1		 //hasModifiedGridInPreviousIteration
                  , 1		 //isTraversalInverted
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[30] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[30] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[31] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[31] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[32] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[32] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[33] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[33] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[34] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[34] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[35] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[35] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[36] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[36] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(State)), i, disp[i], Attributes, sizeof(State));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]))), 		&disp[37] );
               disp[37] -= base;
               disp[37] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &State::FullDatatype );
               MPI_Type_commit( &State::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::FullDatatype);
               MPI_Type_commit( &State::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::State::shutdownDatatype() {
            MPI_Type_free( &State::Datatype );
            MPI_Type_free( &State::FullDatatype );
            
         }
         
         void dem::records::State::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::State "
                  << toString()
                  << " to node " << destination
                  << ": " << tarch::parallel::MPIReturnValueToString(result);
                  _log.error( "send(int)",msg.str() );
               }
               
            }
            else {
            
               MPI_Request* sendRequestHandle = new MPI_Request();
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
                  msg << "was not able to send message dem::records::State "
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
                     msg << "testing for finished send task for dem::records::State "
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
                     "dem::records::State",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::State",
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
         
         
         
         void dem::records::State::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::State from node "
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
                  msg << "failed to start to receive dem::records::State from node "
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
                     msg << "testing for finished receive task for dem::records::State failed: "
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
                     "dem::records::State",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::State",
                     "receive(int)", source,tag,1
                     );
                  }
                  tarch::parallel::Node::getInstance().receiveDanglingMessages();
                  usleep(communicateSleep);
                  
               }
               
               delete sendRequestHandle;
               
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               #ifdef Debug
               _log.debug("receive(int,int)", "received " + toString() ); 
               #endif
               
            }
            
         }
         
         
         
         bool dem::records::State::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::State::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::StatePacked::PersistentRecords::PersistentRecords() {
         if ((6 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((6 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
      _numberOfContactPoints(numberOfContactPoints),
      _numberOfParticleReassignments(numberOfParticleReassignments),
      _numberOfTriangleComparisons(numberOfTriangleComparisons),
      _numberOfParticleComparisons(numberOfParticleComparisons),
      _adaptiveStepSize(adaptiveStepSize),
      _timeStepSize(timeStepSize),
      _timeStep(timeStep),
      _currentTime(currentTime),
      _stepIncrement(stepIncrement),
      _twoParticlesAreClose(twoParticlesAreClose),
      _twoParticlesSeparate(twoParticlesSeparate),
      _numberOfParticles(numberOfParticles),
      _numberOfObstacles(numberOfObstacles),
      _prescribedMinimumMeshWidth(prescribedMinimumMeshWidth),
      _prescribedMaximumMeshWidth(prescribedMaximumMeshWidth),
      _maxVelocityApproach(maxVelocityApproach),
      _maxVelocityTravel(maxVelocityTravel),
      _minMeshWidth(minMeshWidth),
      _maxMeshWidth(maxMeshWidth),
      _numberOfInnerVertices(numberOfInnerVertices),
      _numberOfBoundaryVertices(numberOfBoundaryVertices),
      _numberOfOuterVertices(numberOfOuterVertices),
      _numberOfInnerCells(numberOfInnerCells),
      _numberOfOuterCells(numberOfOuterCells),
      _numberOfInnerLeafVertices(numberOfInnerLeafVertices),
      _numberOfBoundaryLeafVertices(numberOfBoundaryLeafVertices),
      _numberOfOuterLeafVertices(numberOfOuterLeafVertices),
      _numberOfInnerLeafCells(numberOfInnerLeafCells),
      _numberOfOuterLeafCells(numberOfOuterLeafCells),
      _maxLevel(maxLevel),
      _isTraversalInverted(isTraversalInverted) {
         setHasRefined(hasRefined);
         setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
         setHasErased(hasErased);
         setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
         setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
         setHasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration);
         if ((6 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((6 < (8 * sizeof(short int))));
         
      }
      
      dem::records::StatePacked::StatePacked() {
         if ((6 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((6 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::StatePacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._numberOfParticleComparisons, persistentRecords._adaptiveStepSize, persistentRecords._timeStepSize, persistentRecords._timeStep, persistentRecords._currentTime, persistentRecords._stepIncrement, persistentRecords._twoParticlesAreClose, persistentRecords._twoParticlesSeparate, persistentRecords._numberOfParticles, persistentRecords._numberOfObstacles, persistentRecords._prescribedMinimumMeshWidth, persistentRecords._prescribedMaximumMeshWidth, persistentRecords._maxVelocityApproach, persistentRecords._maxVelocityTravel, persistentRecords._minMeshWidth, persistentRecords._maxMeshWidth, persistentRecords._numberOfInnerVertices, persistentRecords._numberOfBoundaryVertices, persistentRecords._numberOfOuterVertices, persistentRecords._numberOfInnerCells, persistentRecords._numberOfOuterCells, persistentRecords._numberOfInnerLeafVertices, persistentRecords._numberOfBoundaryLeafVertices, persistentRecords._numberOfOuterLeafVertices, persistentRecords._numberOfInnerLeafCells, persistentRecords._numberOfOuterLeafCells, persistentRecords._maxLevel, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords.getHasModifiedGridInPreviousIteration(), persistentRecords._isTraversalInverted) {
         if ((6 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((6 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::StatePacked(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const tarch::la::Vector<DIMENSIONS,double>& minMeshWidth, const tarch::la::Vector<DIMENSIONS,double>& maxMeshWidth, const double& numberOfInnerVertices, const double& numberOfBoundaryVertices, const double& numberOfOuterVertices, const double& numberOfInnerCells, const double& numberOfOuterCells, const double& numberOfInnerLeafVertices, const double& numberOfBoundaryLeafVertices, const double& numberOfOuterLeafVertices, const double& numberOfInnerLeafCells, const double& numberOfOuterLeafCells, const int& maxLevel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted):
      _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, numberOfParticleComparisons, adaptiveStepSize, timeStepSize, timeStep, currentTime, stepIncrement, twoParticlesAreClose, twoParticlesSeparate, numberOfParticles, numberOfObstacles, prescribedMinimumMeshWidth, prescribedMaximumMeshWidth, maxVelocityApproach, maxVelocityTravel, minMeshWidth, maxMeshWidth, numberOfInnerVertices, numberOfBoundaryVertices, numberOfOuterVertices, numberOfInnerCells, numberOfOuterCells, numberOfInnerLeafVertices, numberOfBoundaryLeafVertices, numberOfOuterLeafVertices, numberOfInnerLeafCells, numberOfOuterLeafCells, maxLevel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted) {
         if ((6 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((6 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::~StatePacked() { }
      
      
      
      std::string dem::records::StatePacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::StatePacked::toString (std::ostream& out) const {
         out << "("; 
         out << "numberOfContactPoints:" << getNumberOfContactPoints();
         out << ",";
         out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
         out << ",";
         out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
         out << ",";
         out << "numberOfParticleComparisons:" << getNumberOfParticleComparisons();
         out << ",";
         out << "adaptiveStepSize:" << getAdaptiveStepSize();
         out << ",";
         out << "timeStepSize:" << getTimeStepSize();
         out << ",";
         out << "timeStep:" << getTimeStep();
         out << ",";
         out << "currentTime:" << getCurrentTime();
         out << ",";
         out << "stepIncrement:" << getStepIncrement();
         out << ",";
         out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
         out << ",";
         out << "twoParticlesSeparate:" << getTwoParticlesSeparate();
         out << ",";
         out << "numberOfParticles:" << getNumberOfParticles();
         out << ",";
         out << "numberOfObstacles:" << getNumberOfObstacles();
         out << ",";
         out << "prescribedMinimumMeshWidth:" << getPrescribedMinimumMeshWidth();
         out << ",";
         out << "prescribedMaximumMeshWidth:" << getPrescribedMaximumMeshWidth();
         out << ",";
         out << "maxVelocityApproach:" << getMaxVelocityApproach();
         out << ",";
         out << "maxVelocityTravel:" << getMaxVelocityTravel();
         out << ",";
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
         out << "numberOfInnerLeafVertices:" << getNumberOfInnerLeafVertices();
         out << ",";
         out << "numberOfBoundaryLeafVertices:" << getNumberOfBoundaryLeafVertices();
         out << ",";
         out << "numberOfOuterLeafVertices:" << getNumberOfOuterLeafVertices();
         out << ",";
         out << "numberOfInnerLeafCells:" << getNumberOfInnerLeafCells();
         out << ",";
         out << "numberOfOuterLeafCells:" << getNumberOfOuterLeafCells();
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
         out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
         out << ",";
         out << "isTraversalInverted:" << getIsTraversalInverted();
         out <<  ")";
      }
      
      
      dem::records::StatePacked::PersistentRecords dem::records::StatePacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::State dem::records::StatePacked::convert() const{
         return State(
            getNumberOfContactPoints(),
            getNumberOfParticleReassignments(),
            getNumberOfTriangleComparisons(),
            getNumberOfParticleComparisons(),
            getAdaptiveStepSize(),
            getTimeStepSize(),
            getTimeStep(),
            getCurrentTime(),
            getStepIncrement(),
            getTwoParticlesAreClose(),
            getTwoParticlesSeparate(),
            getNumberOfParticles(),
            getNumberOfObstacles(),
            getPrescribedMinimumMeshWidth(),
            getPrescribedMaximumMeshWidth(),
            getMaxVelocityApproach(),
            getMaxVelocityTravel(),
            getMinMeshWidth(),
            getMaxMeshWidth(),
            getNumberOfInnerVertices(),
            getNumberOfBoundaryVertices(),
            getNumberOfOuterVertices(),
            getNumberOfInnerCells(),
            getNumberOfOuterCells(),
            getNumberOfInnerLeafVertices(),
            getNumberOfBoundaryLeafVertices(),
            getNumberOfOuterLeafVertices(),
            getNumberOfInnerLeafCells(),
            getNumberOfOuterLeafCells(),
            getMaxLevel(),
            getHasRefined(),
            getHasTriggeredRefinementForNextIteration(),
            getHasErased(),
            getHasTriggeredEraseForNextIteration(),
            getHasChangedVertexOrCellState(),
            getHasModifiedGridInPreviousIteration(),
            getIsTraversalInverted()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::StatePacked::_log( "dem::records::StatePacked" );
         
         MPI_Datatype dem::records::StatePacked::Datatype = 0;
         MPI_Datatype dem::records::StatePacked::FullDatatype = 0;
         
         
         void dem::records::StatePacked::initDatatype() {
            {
               StatePacked dummyStatePacked[2];
               
               #ifdef MPI2
               const int Attributes = 32;
               #else
               const int Attributes = 33;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_DOUBLE		 //numberOfInnerLeafVertices
                  , MPI_DOUBLE		 //numberOfBoundaryLeafVertices
                  , MPI_DOUBLE		 //numberOfOuterLeafVertices
                  , MPI_DOUBLE		 //numberOfInnerLeafCells
                  , MPI_DOUBLE		 //numberOfOuterLeafCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //numberOfInnerLeafVertices
                  , 1		 //numberOfBoundaryLeafVertices
                  , 1		 //numberOfOuterLeafVertices
                  , 1		 //numberOfInnerLeafCells
                  , 1		 //numberOfOuterLeafCells
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
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[30] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[30] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[31] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[31] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(StatePacked)), i, disp[i], Attributes, sizeof(StatePacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]))), 		&disp[32] );
               disp[32] -= base;
               disp[32] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &StatePacked::Datatype );
               MPI_Type_commit( &StatePacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::Datatype);
               MPI_Type_commit( &StatePacked::Datatype );
               #endif
               
            }
            {
               StatePacked dummyStatePacked[2];
               
               #ifdef MPI2
               const int Attributes = 32;
               #else
               const int Attributes = 33;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_DOUBLE		 //minMeshWidth
                  , MPI_DOUBLE		 //maxMeshWidth
                  , MPI_DOUBLE		 //numberOfInnerVertices
                  , MPI_DOUBLE		 //numberOfBoundaryVertices
                  , MPI_DOUBLE		 //numberOfOuterVertices
                  , MPI_DOUBLE		 //numberOfInnerCells
                  , MPI_DOUBLE		 //numberOfOuterCells
                  , MPI_DOUBLE		 //numberOfInnerLeafVertices
                  , MPI_DOUBLE		 //numberOfBoundaryLeafVertices
                  , MPI_DOUBLE		 //numberOfOuterLeafVertices
                  , MPI_DOUBLE		 //numberOfInnerLeafCells
                  , MPI_DOUBLE		 //numberOfOuterLeafCells
                  , MPI_INT		 //maxLevel
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , DIMENSIONS		 //minMeshWidth
                  , DIMENSIONS		 //maxMeshWidth
                  , 1		 //numberOfInnerVertices
                  , 1		 //numberOfBoundaryVertices
                  , 1		 //numberOfOuterVertices
                  , 1		 //numberOfInnerCells
                  , 1		 //numberOfOuterCells
                  , 1		 //numberOfInnerLeafVertices
                  , 1		 //numberOfBoundaryLeafVertices
                  , 1		 //numberOfOuterLeafVertices
                  , 1		 //numberOfInnerLeafCells
                  , 1		 //numberOfOuterLeafCells
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
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._minMeshWidth[0]))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxMeshWidth[0]))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerVertices))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryVertices))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterVertices))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerCells))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterCells))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafVertices))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfBoundaryLeafVertices))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafVertices))), 		&disp[26] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfInnerLeafCells))), 		&disp[27] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfOuterLeafCells))), 		&disp[28] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxLevel))), 		&disp[29] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[30] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[30] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[31] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[31] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(StatePacked)), i, disp[i], Attributes, sizeof(StatePacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]))), 		&disp[32] );
               disp[32] -= base;
               disp[32] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &StatePacked::FullDatatype );
               MPI_Type_commit( &StatePacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::FullDatatype);
               MPI_Type_commit( &StatePacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::StatePacked::shutdownDatatype() {
            MPI_Type_free( &StatePacked::Datatype );
            MPI_Type_free( &StatePacked::FullDatatype );
            
         }
         
         void dem::records::StatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::StatePacked "
                  << toString()
                  << " to node " << destination
                  << ": " << tarch::parallel::MPIReturnValueToString(result);
                  _log.error( "send(int)",msg.str() );
               }
               
            }
            else {
            
               MPI_Request* sendRequestHandle = new MPI_Request();
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
                  msg << "was not able to send message dem::records::StatePacked "
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
                     msg << "testing for finished send task for dem::records::StatePacked "
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
                     "dem::records::StatePacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::StatePacked",
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
         
         
         
         void dem::records::StatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::StatePacked from node "
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
                  msg << "failed to start to receive dem::records::StatePacked from node "
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
                     msg << "testing for finished receive task for dem::records::StatePacked failed: "
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
                     "dem::records::StatePacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::StatePacked",
                     "receive(int)", source,tag,1
                     );
                  }
                  tarch::parallel::Node::getInstance().receiveDanglingMessages();
                  usleep(communicateSleep);
                  
               }
               
               delete sendRequestHandle;
               
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               #ifdef Debug
               _log.debug("receive(int,int)", "received " + toString() ); 
               #endif
               
            }
            
         }
         
         
         
         bool dem::records::StatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::StatePacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   #elif !defined(TrackGridStatistics) && defined(Parallel)
      dem::records::State::PersistentRecords::PersistentRecords() {
         
      }
      
      
      dem::records::State::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
      _numberOfContactPoints(numberOfContactPoints),
      _numberOfParticleReassignments(numberOfParticleReassignments),
      _numberOfTriangleComparisons(numberOfTriangleComparisons),
      _numberOfParticleComparisons(numberOfParticleComparisons),
      _adaptiveStepSize(adaptiveStepSize),
      _timeStepSize(timeStepSize),
      _timeStep(timeStep),
      _currentTime(currentTime),
      _stepIncrement(stepIncrement),
      _twoParticlesAreClose(twoParticlesAreClose),
      _twoParticlesSeparate(twoParticlesSeparate),
      _numberOfParticles(numberOfParticles),
      _numberOfObstacles(numberOfObstacles),
      _prescribedMinimumMeshWidth(prescribedMinimumMeshWidth),
      _prescribedMaximumMeshWidth(prescribedMaximumMeshWidth),
      _maxVelocityApproach(maxVelocityApproach),
      _maxVelocityTravel(maxVelocityTravel),
      _hasRefined(hasRefined),
      _hasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration),
      _hasErased(hasErased),
      _hasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration),
      _hasChangedVertexOrCellState(hasChangedVertexOrCellState),
      _hasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration),
      _isTraversalInverted(isTraversalInverted),
      _reduceStateAndCell(reduceStateAndCell),
      _couldNotEraseDueToDecompositionFlag(couldNotEraseDueToDecompositionFlag),
      _subWorkerIsInvolvedInJoinOrFork(subWorkerIsInvolvedInJoinOrFork) {
         
      }
      
      dem::records::State::State() {
         
      }
      
      
      dem::records::State::State(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._numberOfParticleComparisons, persistentRecords._adaptiveStepSize, persistentRecords._timeStepSize, persistentRecords._timeStep, persistentRecords._currentTime, persistentRecords._stepIncrement, persistentRecords._twoParticlesAreClose, persistentRecords._twoParticlesSeparate, persistentRecords._numberOfParticles, persistentRecords._numberOfObstacles, persistentRecords._prescribedMinimumMeshWidth, persistentRecords._prescribedMaximumMeshWidth, persistentRecords._maxVelocityApproach, persistentRecords._maxVelocityTravel, persistentRecords._hasRefined, persistentRecords._hasTriggeredRefinementForNextIteration, persistentRecords._hasErased, persistentRecords._hasTriggeredEraseForNextIteration, persistentRecords._hasChangedVertexOrCellState, persistentRecords._hasModifiedGridInPreviousIteration, persistentRecords._isTraversalInverted, persistentRecords._reduceStateAndCell, persistentRecords._couldNotEraseDueToDecompositionFlag, persistentRecords._subWorkerIsInvolvedInJoinOrFork) {
         
      }
      
      
      dem::records::State::State(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
      _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, numberOfParticleComparisons, adaptiveStepSize, timeStepSize, timeStep, currentTime, stepIncrement, twoParticlesAreClose, twoParticlesSeparate, numberOfParticles, numberOfObstacles, prescribedMinimumMeshWidth, prescribedMaximumMeshWidth, maxVelocityApproach, maxVelocityTravel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted, reduceStateAndCell, couldNotEraseDueToDecompositionFlag, subWorkerIsInvolvedInJoinOrFork) {
         
      }
      
      
      dem::records::State::~State() { }
      
      
      
      std::string dem::records::State::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::State::toString (std::ostream& out) const {
         out << "("; 
         out << "numberOfContactPoints:" << getNumberOfContactPoints();
         out << ",";
         out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
         out << ",";
         out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
         out << ",";
         out << "numberOfParticleComparisons:" << getNumberOfParticleComparisons();
         out << ",";
         out << "adaptiveStepSize:" << getAdaptiveStepSize();
         out << ",";
         out << "timeStepSize:" << getTimeStepSize();
         out << ",";
         out << "timeStep:" << getTimeStep();
         out << ",";
         out << "currentTime:" << getCurrentTime();
         out << ",";
         out << "stepIncrement:" << getStepIncrement();
         out << ",";
         out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
         out << ",";
         out << "twoParticlesSeparate:" << getTwoParticlesSeparate();
         out << ",";
         out << "numberOfParticles:" << getNumberOfParticles();
         out << ",";
         out << "numberOfObstacles:" << getNumberOfObstacles();
         out << ",";
         out << "prescribedMinimumMeshWidth:" << getPrescribedMinimumMeshWidth();
         out << ",";
         out << "prescribedMaximumMeshWidth:" << getPrescribedMaximumMeshWidth();
         out << ",";
         out << "maxVelocityApproach:" << getMaxVelocityApproach();
         out << ",";
         out << "maxVelocityTravel:" << getMaxVelocityTravel();
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
         out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
         out << ",";
         out << "isTraversalInverted:" << getIsTraversalInverted();
         out << ",";
         out << "reduceStateAndCell:" << getReduceStateAndCell();
         out << ",";
         out << "couldNotEraseDueToDecompositionFlag:" << getCouldNotEraseDueToDecompositionFlag();
         out << ",";
         out << "subWorkerIsInvolvedInJoinOrFork:" << getSubWorkerIsInvolvedInJoinOrFork();
         out <<  ")";
      }
      
      
      dem::records::State::PersistentRecords dem::records::State::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::StatePacked dem::records::State::convert() const{
         return StatePacked(
            getNumberOfContactPoints(),
            getNumberOfParticleReassignments(),
            getNumberOfTriangleComparisons(),
            getNumberOfParticleComparisons(),
            getAdaptiveStepSize(),
            getTimeStepSize(),
            getTimeStep(),
            getCurrentTime(),
            getStepIncrement(),
            getTwoParticlesAreClose(),
            getTwoParticlesSeparate(),
            getNumberOfParticles(),
            getNumberOfObstacles(),
            getPrescribedMinimumMeshWidth(),
            getPrescribedMaximumMeshWidth(),
            getMaxVelocityApproach(),
            getMaxVelocityTravel(),
            getHasRefined(),
            getHasTriggeredRefinementForNextIteration(),
            getHasErased(),
            getHasTriggeredEraseForNextIteration(),
            getHasChangedVertexOrCellState(),
            getHasModifiedGridInPreviousIteration(),
            getIsTraversalInverted(),
            getReduceStateAndCell(),
            getCouldNotEraseDueToDecompositionFlag(),
            getSubWorkerIsInvolvedInJoinOrFork()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::State::_log( "dem::records::State" );
         
         MPI_Datatype dem::records::State::Datatype = 0;
         MPI_Datatype dem::records::State::FullDatatype = 0;
         
         
         void dem::records::State::initDatatype() {
            {
               State dummyState[2];
               
               #ifdef MPI2
               const int Attributes = 27;
               #else
               const int Attributes = 28;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_CXX_BOOL		 //hasRefined
                  , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
                  , MPI_CXX_BOOL		 //hasErased
                  , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
                  , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
                  , MPI_CXX_BOOL		 //hasModifiedGridInPreviousIteration
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_CXX_BOOL		 //reduceStateAndCell
                  , MPI_CXX_BOOL		 //couldNotEraseDueToDecompositionFlag
                  , MPI_CXX_BOOL		 //subWorkerIsInvolvedInJoinOrFork
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , 1		 //hasRefined
                  , 1		 //hasTriggeredRefinementForNextIteration
                  , 1		 //hasErased
                  , 1		 //hasTriggeredEraseForNextIteration
                  , 1		 //hasChangedVertexOrCellState
                  , 1		 //hasModifiedGridInPreviousIteration
                  , 1		 //isTraversalInverted
                  , 1		 //reduceStateAndCell
                  , 1		 //couldNotEraseDueToDecompositionFlag
                  , 1		 //subWorkerIsInvolvedInJoinOrFork
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[26] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(State)), i, disp[i], Attributes, sizeof(State));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]))), 		&disp[27] );
               disp[27] -= base;
               disp[27] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &State::Datatype );
               MPI_Type_commit( &State::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::Datatype);
               MPI_Type_commit( &State::Datatype );
               #endif
               
            }
            {
               State dummyState[2];
               
               #ifdef MPI2
               const int Attributes = 27;
               #else
               const int Attributes = 28;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_CXX_BOOL		 //hasRefined
                  , MPI_CXX_BOOL		 //hasTriggeredRefinementForNextIteration
                  , MPI_CXX_BOOL		 //hasErased
                  , MPI_CXX_BOOL		 //hasTriggeredEraseForNextIteration
                  , MPI_CXX_BOOL		 //hasChangedVertexOrCellState
                  , MPI_CXX_BOOL		 //hasModifiedGridInPreviousIteration
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_CXX_BOOL		 //reduceStateAndCell
                  , MPI_CXX_BOOL		 //couldNotEraseDueToDecompositionFlag
                  , MPI_CXX_BOOL		 //subWorkerIsInvolvedInJoinOrFork
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , 1		 //hasRefined
                  , 1		 //hasTriggeredRefinementForNextIteration
                  , 1		 //hasErased
                  , 1		 //hasTriggeredEraseForNextIteration
                  , 1		 //hasChangedVertexOrCellState
                  , 1		 //hasModifiedGridInPreviousIteration
                  , 1		 //isTraversalInverted
                  , 1		 //reduceStateAndCell
                  , 1		 //couldNotEraseDueToDecompositionFlag
                  , 1		 //subWorkerIsInvolvedInJoinOrFork
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasRefined))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredRefinementForNextIteration))), 		&disp[18] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[19] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasErased))), 		&disp[19] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[20] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasTriggeredEraseForNextIteration))), 		&disp[20] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[21] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasChangedVertexOrCellState))), 		&disp[21] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[22] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._hasModifiedGridInPreviousIteration))), 		&disp[22] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[23] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._isTraversalInverted))), 		&disp[23] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[24] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._reduceStateAndCell))), 		&disp[24] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[25] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._couldNotEraseDueToDecompositionFlag))), 		&disp[25] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[26] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[0]._persistentRecords._subWorkerIsInvolvedInJoinOrFork))), 		&disp[26] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(State)), i, disp[i], Attributes, sizeof(State));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyState[1]))), 		&disp[27] );
               disp[27] -= base;
               disp[27] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &State::FullDatatype );
               MPI_Type_commit( &State::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &State::FullDatatype);
               MPI_Type_commit( &State::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::State::shutdownDatatype() {
            MPI_Type_free( &State::Datatype );
            MPI_Type_free( &State::FullDatatype );
            
         }
         
         void dem::records::State::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::State "
                  << toString()
                  << " to node " << destination
                  << ": " << tarch::parallel::MPIReturnValueToString(result);
                  _log.error( "send(int)",msg.str() );
               }
               
            }
            else {
            
               MPI_Request* sendRequestHandle = new MPI_Request();
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
                  msg << "was not able to send message dem::records::State "
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
                     msg << "testing for finished send task for dem::records::State "
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
                     "dem::records::State",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::State",
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
         
         
         
         void dem::records::State::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::State from node "
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
                  msg << "failed to start to receive dem::records::State from node "
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
                     msg << "testing for finished receive task for dem::records::State failed: "
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
                     "dem::records::State",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::State",
                     "receive(int)", source,tag,1
                     );
                  }
                  tarch::parallel::Node::getInstance().receiveDanglingMessages();
                  usleep(communicateSleep);
                  
               }
               
               delete sendRequestHandle;
               
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               #ifdef Debug
               _log.debug("receive(int,int)", "received " + toString() ); 
               #endif
               
            }
            
         }
         
         
         
         bool dem::records::State::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::State::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      dem::records::StatePacked::PersistentRecords::PersistentRecords() {
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::PersistentRecords::PersistentRecords(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
      _numberOfContactPoints(numberOfContactPoints),
      _numberOfParticleReassignments(numberOfParticleReassignments),
      _numberOfTriangleComparisons(numberOfTriangleComparisons),
      _numberOfParticleComparisons(numberOfParticleComparisons),
      _adaptiveStepSize(adaptiveStepSize),
      _timeStepSize(timeStepSize),
      _timeStep(timeStep),
      _currentTime(currentTime),
      _stepIncrement(stepIncrement),
      _twoParticlesAreClose(twoParticlesAreClose),
      _twoParticlesSeparate(twoParticlesSeparate),
      _numberOfParticles(numberOfParticles),
      _numberOfObstacles(numberOfObstacles),
      _prescribedMinimumMeshWidth(prescribedMinimumMeshWidth),
      _prescribedMaximumMeshWidth(prescribedMaximumMeshWidth),
      _maxVelocityApproach(maxVelocityApproach),
      _maxVelocityTravel(maxVelocityTravel),
      _isTraversalInverted(isTraversalInverted) {
         setHasRefined(hasRefined);
         setHasTriggeredRefinementForNextIteration(hasTriggeredRefinementForNextIteration);
         setHasErased(hasErased);
         setHasTriggeredEraseForNextIteration(hasTriggeredEraseForNextIteration);
         setHasChangedVertexOrCellState(hasChangedVertexOrCellState);
         setHasModifiedGridInPreviousIteration(hasModifiedGridInPreviousIteration);
         setReduceStateAndCell(reduceStateAndCell);
         setCouldNotEraseDueToDecompositionFlag(couldNotEraseDueToDecompositionFlag);
         setSubWorkerIsInvolvedInJoinOrFork(subWorkerIsInvolvedInJoinOrFork);
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      dem::records::StatePacked::StatePacked() {
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::StatePacked(const PersistentRecords& persistentRecords):
      _persistentRecords(persistentRecords._numberOfContactPoints, persistentRecords._numberOfParticleReassignments, persistentRecords._numberOfTriangleComparisons, persistentRecords._numberOfParticleComparisons, persistentRecords._adaptiveStepSize, persistentRecords._timeStepSize, persistentRecords._timeStep, persistentRecords._currentTime, persistentRecords._stepIncrement, persistentRecords._twoParticlesAreClose, persistentRecords._twoParticlesSeparate, persistentRecords._numberOfParticles, persistentRecords._numberOfObstacles, persistentRecords._prescribedMinimumMeshWidth, persistentRecords._prescribedMaximumMeshWidth, persistentRecords._maxVelocityApproach, persistentRecords._maxVelocityTravel, persistentRecords.getHasRefined(), persistentRecords.getHasTriggeredRefinementForNextIteration(), persistentRecords.getHasErased(), persistentRecords.getHasTriggeredEraseForNextIteration(), persistentRecords.getHasChangedVertexOrCellState(), persistentRecords.getHasModifiedGridInPreviousIteration(), persistentRecords._isTraversalInverted, persistentRecords.getReduceStateAndCell(), persistentRecords.getCouldNotEraseDueToDecompositionFlag(), persistentRecords.getSubWorkerIsInvolvedInJoinOrFork()) {
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::StatePacked(const double& numberOfContactPoints, const double& numberOfParticleReassignments, const double& numberOfTriangleComparisons, const double& numberOfParticleComparisons, const bool& adaptiveStepSize, const double& timeStepSize, const int& timeStep, const double& currentTime, const double& stepIncrement, const double& twoParticlesAreClose, const bool& twoParticlesSeparate, const int& numberOfParticles, const int& numberOfObstacles, const double& prescribedMinimumMeshWidth, const double& prescribedMaximumMeshWidth, const double& maxVelocityApproach, const double& maxVelocityTravel, const bool& hasRefined, const bool& hasTriggeredRefinementForNextIteration, const bool& hasErased, const bool& hasTriggeredEraseForNextIteration, const bool& hasChangedVertexOrCellState, const bool& hasModifiedGridInPreviousIteration, const bool& isTraversalInverted, const bool& reduceStateAndCell, const bool& couldNotEraseDueToDecompositionFlag, const bool& subWorkerIsInvolvedInJoinOrFork):
      _persistentRecords(numberOfContactPoints, numberOfParticleReassignments, numberOfTriangleComparisons, numberOfParticleComparisons, adaptiveStepSize, timeStepSize, timeStep, currentTime, stepIncrement, twoParticlesAreClose, twoParticlesSeparate, numberOfParticles, numberOfObstacles, prescribedMinimumMeshWidth, prescribedMaximumMeshWidth, maxVelocityApproach, maxVelocityTravel, hasRefined, hasTriggeredRefinementForNextIteration, hasErased, hasTriggeredEraseForNextIteration, hasChangedVertexOrCellState, hasModifiedGridInPreviousIteration, isTraversalInverted, reduceStateAndCell, couldNotEraseDueToDecompositionFlag, subWorkerIsInvolvedInJoinOrFork) {
         if ((9 >= (8 * sizeof(short int)))) {
            std::cerr << "Packed-Type in " << __FILE__ << " too small. Either use bigger data type or append " << std::endl << std::endl;
            std::cerr << "  Packed-Type: short int hint-size no-of-bits;  " << std::endl << std::endl;
            std::cerr << "to your data type spec to guide DaStGen how many bits (no-of-bits) a data type has on your machine. DaStGen then can split up the bitfields into several attributes. " << std::endl; 
         }
         assertion((9 < (8 * sizeof(short int))));
         
      }
      
      
      dem::records::StatePacked::~StatePacked() { }
      
      
      
      std::string dem::records::StatePacked::toString() const {
         std::ostringstream stringstr;
         toString(stringstr);
         return stringstr.str();
      }
      
      void dem::records::StatePacked::toString (std::ostream& out) const {
         out << "("; 
         out << "numberOfContactPoints:" << getNumberOfContactPoints();
         out << ",";
         out << "numberOfParticleReassignments:" << getNumberOfParticleReassignments();
         out << ",";
         out << "numberOfTriangleComparisons:" << getNumberOfTriangleComparisons();
         out << ",";
         out << "numberOfParticleComparisons:" << getNumberOfParticleComparisons();
         out << ",";
         out << "adaptiveStepSize:" << getAdaptiveStepSize();
         out << ",";
         out << "timeStepSize:" << getTimeStepSize();
         out << ",";
         out << "timeStep:" << getTimeStep();
         out << ",";
         out << "currentTime:" << getCurrentTime();
         out << ",";
         out << "stepIncrement:" << getStepIncrement();
         out << ",";
         out << "twoParticlesAreClose:" << getTwoParticlesAreClose();
         out << ",";
         out << "twoParticlesSeparate:" << getTwoParticlesSeparate();
         out << ",";
         out << "numberOfParticles:" << getNumberOfParticles();
         out << ",";
         out << "numberOfObstacles:" << getNumberOfObstacles();
         out << ",";
         out << "prescribedMinimumMeshWidth:" << getPrescribedMinimumMeshWidth();
         out << ",";
         out << "prescribedMaximumMeshWidth:" << getPrescribedMaximumMeshWidth();
         out << ",";
         out << "maxVelocityApproach:" << getMaxVelocityApproach();
         out << ",";
         out << "maxVelocityTravel:" << getMaxVelocityTravel();
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
         out << "hasModifiedGridInPreviousIteration:" << getHasModifiedGridInPreviousIteration();
         out << ",";
         out << "isTraversalInverted:" << getIsTraversalInverted();
         out << ",";
         out << "reduceStateAndCell:" << getReduceStateAndCell();
         out << ",";
         out << "couldNotEraseDueToDecompositionFlag:" << getCouldNotEraseDueToDecompositionFlag();
         out << ",";
         out << "subWorkerIsInvolvedInJoinOrFork:" << getSubWorkerIsInvolvedInJoinOrFork();
         out <<  ")";
      }
      
      
      dem::records::StatePacked::PersistentRecords dem::records::StatePacked::getPersistentRecords() const {
         return _persistentRecords;
      }
      
      dem::records::State dem::records::StatePacked::convert() const{
         return State(
            getNumberOfContactPoints(),
            getNumberOfParticleReassignments(),
            getNumberOfTriangleComparisons(),
            getNumberOfParticleComparisons(),
            getAdaptiveStepSize(),
            getTimeStepSize(),
            getTimeStep(),
            getCurrentTime(),
            getStepIncrement(),
            getTwoParticlesAreClose(),
            getTwoParticlesSeparate(),
            getNumberOfParticles(),
            getNumberOfObstacles(),
            getPrescribedMinimumMeshWidth(),
            getPrescribedMaximumMeshWidth(),
            getMaxVelocityApproach(),
            getMaxVelocityTravel(),
            getHasRefined(),
            getHasTriggeredRefinementForNextIteration(),
            getHasErased(),
            getHasTriggeredEraseForNextIteration(),
            getHasChangedVertexOrCellState(),
            getHasModifiedGridInPreviousIteration(),
            getIsTraversalInverted(),
            getReduceStateAndCell(),
            getCouldNotEraseDueToDecompositionFlag(),
            getSubWorkerIsInvolvedInJoinOrFork()
         );
      }
      
      #ifdef Parallel
         tarch::logging::Log dem::records::StatePacked::_log( "dem::records::StatePacked" );
         
         MPI_Datatype dem::records::StatePacked::Datatype = 0;
         MPI_Datatype dem::records::StatePacked::FullDatatype = 0;
         
         
         void dem::records::StatePacked::initDatatype() {
            {
               StatePacked dummyStatePacked[2];
               
               #ifdef MPI2
               const int Attributes = 19;
               #else
               const int Attributes = 20;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , 1		 //isTraversalInverted
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[18] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(StatePacked)), i, disp[i], Attributes, sizeof(StatePacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]))), 		&disp[19] );
               disp[19] -= base;
               disp[19] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &StatePacked::Datatype );
               MPI_Type_commit( &StatePacked::Datatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::Datatype);
               MPI_Type_commit( &StatePacked::Datatype );
               #endif
               
            }
            {
               StatePacked dummyStatePacked[2];
               
               #ifdef MPI2
               const int Attributes = 19;
               #else
               const int Attributes = 20;
               #endif
               MPI_Datatype subtypes[Attributes] = {
                    MPI_DOUBLE		 //numberOfContactPoints
                  , MPI_DOUBLE		 //numberOfParticleReassignments
                  , MPI_DOUBLE		 //numberOfTriangleComparisons
                  , MPI_DOUBLE		 //numberOfParticleComparisons
                  , MPI_CXX_BOOL		 //adaptiveStepSize
                  , MPI_DOUBLE		 //timeStepSize
                  , MPI_INT		 //timeStep
                  , MPI_DOUBLE		 //currentTime
                  , MPI_DOUBLE		 //stepIncrement
                  , MPI_DOUBLE		 //twoParticlesAreClose
                  , MPI_CXX_BOOL		 //twoParticlesSeparate
                  , MPI_INT		 //numberOfParticles
                  , MPI_INT		 //numberOfObstacles
                  , MPI_DOUBLE		 //prescribedMinimumMeshWidth
                  , MPI_DOUBLE		 //prescribedMaximumMeshWidth
                  , MPI_DOUBLE		 //maxVelocityApproach
                  , MPI_DOUBLE		 //maxVelocityTravel
                  , MPI_CXX_BOOL		 //isTraversalInverted
                  , MPI_SHORT		 //_packedRecords0
                  #ifndef MPI2
                  , MPI_UB
                  #endif
                  
               };
               
               int blocklen[Attributes] = {
                    1		 //numberOfContactPoints
                  , 1		 //numberOfParticleReassignments
                  , 1		 //numberOfTriangleComparisons
                  , 1		 //numberOfParticleComparisons
                  , 1		 //adaptiveStepSize
                  , 1		 //timeStepSize
                  , 1		 //timeStep
                  , 1		 //currentTime
                  , 1		 //stepIncrement
                  , 1		 //twoParticlesAreClose
                  , 1		 //twoParticlesSeparate
                  , 1		 //numberOfParticles
                  , 1		 //numberOfObstacles
                  , 1		 //prescribedMinimumMeshWidth
                  , 1		 //prescribedMaximumMeshWidth
                  , 1		 //maxVelocityApproach
                  , 1		 //maxVelocityTravel
                  , 1		 //isTraversalInverted
                  , 1		 //_packedRecords0
                  #ifndef MPI2
                  , 1
                  #endif
                  
               };
               
               MPI_Aint  disp[Attributes];
               MPI_Aint  base;
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked))), &base);
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfContactPoints))), 		&disp[0] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleReassignments))), 		&disp[1] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfTriangleComparisons))), 		&disp[2] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticleComparisons))), 		&disp[3] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._adaptiveStepSize))), 		&disp[4] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStepSize))), 		&disp[5] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._timeStep))), 		&disp[6] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._currentTime))), 		&disp[7] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._stepIncrement))), 		&disp[8] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesAreClose))), 		&disp[9] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._twoParticlesSeparate))), 		&disp[10] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfParticles))), 		&disp[11] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._numberOfObstacles))), 		&disp[12] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMinimumMeshWidth))), 		&disp[13] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._prescribedMaximumMeshWidth))), 		&disp[14] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityApproach))), 		&disp[15] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._maxVelocityTravel))), 		&disp[16] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[17] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._isTraversalInverted))), 		&disp[17] );
               #endif
               #ifdef MPI2
               MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[18] );
               #else
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[0]._persistentRecords._packedRecords0))), 		&disp[18] );
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
                  assertion4(disp[i]<static_cast<int>(sizeof(StatePacked)), i, disp[i], Attributes, sizeof(StatePacked));
               }
               #ifndef MPI2
               MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyStatePacked[1]))), 		&disp[19] );
               disp[19] -= base;
               disp[19] += disp[0];
               #endif
               #ifdef MPI2
               MPI_Datatype tmpType; 
               MPI_Aint lowerBound, typeExtent; 
               MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
               MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
               MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &StatePacked::FullDatatype );
               MPI_Type_commit( &StatePacked::FullDatatype );
               #else
               MPI_Type_struct( Attributes, blocklen, disp, subtypes, &StatePacked::FullDatatype);
               MPI_Type_commit( &StatePacked::FullDatatype );
               #endif
               
            }
            
         }
         
         
         void dem::records::StatePacked::shutdownDatatype() {
            MPI_Type_free( &StatePacked::Datatype );
            MPI_Type_free( &StatePacked::FullDatatype );
            
         }
         
         void dem::records::StatePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            _senderDestinationRank = destination;
            
            if (communicateSleep<0) {
            
               const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
               if  (result!=MPI_SUCCESS) {
                  std::ostringstream msg;
                  msg << "was not able to send message dem::records::StatePacked "
                  << toString()
                  << " to node " << destination
                  << ": " << tarch::parallel::MPIReturnValueToString(result);
                  _log.error( "send(int)",msg.str() );
               }
               
            }
            else {
            
               MPI_Request* sendRequestHandle = new MPI_Request();
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
                  msg << "was not able to send message dem::records::StatePacked "
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
                     msg << "testing for finished send task for dem::records::StatePacked "
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
                     "dem::records::StatePacked",
                     "send(int)", destination,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::StatePacked",
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
         
         
         
         void dem::records::StatePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
            if (communicateSleep<0) {
            
               MPI_Status  status;
               const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE );
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               if ( result != MPI_SUCCESS ) {
                  std::ostringstream msg;
                  msg << "failed to start to receive dem::records::StatePacked from node "
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
                  msg << "failed to start to receive dem::records::StatePacked from node "
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
                     msg << "testing for finished receive task for dem::records::StatePacked failed: "
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
                     "dem::records::StatePacked",
                     "receive(int)", source,tag,1
                     );
                     triggeredTimeoutWarning = true;
                  }
                  if (
                     tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
                     (clock()>timeOutShutdown)
                  ) {
                     tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
                     "dem::records::StatePacked",
                     "receive(int)", source,tag,1
                     );
                  }
                  tarch::parallel::Node::getInstance().receiveDanglingMessages();
                  usleep(communicateSleep);
                  
               }
               
               delete sendRequestHandle;
               
               _senderDestinationRank = source==MPI_ANY_SOURCE ? status.MPI_SOURCE : source;
               #ifdef Debug
               _log.debug("receive(int,int)", "received " + toString() ); 
               #endif
               
            }
            
         }
         
         
         
         bool dem::records::StatePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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
         
         int dem::records::StatePacked::getSenderRank() const {
            assertion( _senderDestinationRank!=-1 );
            return _senderDestinationRank;
            
         }
      #endif
      
      
      
   
#endif


