#include "dem/records/Particle.h"

dem::records::Particle::PersistentRecords::PersistentRecords() {
   
}


dem::records::Particle::PersistentRecords::PersistentRecords(const tarch::la::Vector<6,double>& vertices, const tarch::la::Vector<9,double>& orientation, const tarch::la::Vector<9,double>& inertia, const tarch::la::Vector<9,double>& inverse, const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const tarch::la::Vector<DIMENSIONS,double>& referentialCentreOfMass, const tarch::la::Vector<DIMENSIONS,double>& velocity, const tarch::la::Vector<DIMENSIONS,double>& angular, const tarch::la::Vector<DIMENSIONS,double>& referentialAngular, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const int& globalParticleId, const int& localParticleId, const int& numberOfTriangles, const int& material, const bool& isObstacle, const bool& friction):
_vertices(vertices),
_orientation(orientation),
_inertia(inertia),
_inverse(inverse),
_centre(centre),
_centreOfMass(centreOfMass),
_referentialCentreOfMass(referentialCentreOfMass),
_velocity(velocity),
_angular(angular),
_referentialAngular(referentialAngular),
_diameter(diameter),
_influenceRadius(influenceRadius),
_epsilon(epsilon),
_mass(mass),
_hMin(hMin),
_globalParticleId(globalParticleId),
_localParticleId(localParticleId),
_numberOfTriangles(numberOfTriangles),
_material(material),
_isObstacle(isObstacle),
_friction(friction) {
   
}

dem::records::Particle::Particle() {
   
}


dem::records::Particle::Particle(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._vertices, persistentRecords._orientation, persistentRecords._inertia, persistentRecords._inverse, persistentRecords._centre, persistentRecords._centreOfMass, persistentRecords._referentialCentreOfMass, persistentRecords._velocity, persistentRecords._angular, persistentRecords._referentialAngular, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._globalParticleId, persistentRecords._localParticleId, persistentRecords._numberOfTriangles, persistentRecords._material, persistentRecords._isObstacle, persistentRecords._friction) {
   
}


dem::records::Particle::Particle(const tarch::la::Vector<6,double>& vertices, const tarch::la::Vector<9,double>& orientation, const tarch::la::Vector<9,double>& inertia, const tarch::la::Vector<9,double>& inverse, const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const tarch::la::Vector<DIMENSIONS,double>& referentialCentreOfMass, const tarch::la::Vector<DIMENSIONS,double>& velocity, const tarch::la::Vector<DIMENSIONS,double>& angular, const tarch::la::Vector<DIMENSIONS,double>& referentialAngular, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const int& globalParticleId, const int& localParticleId, const int& numberOfTriangles, const int& material, const bool& isObstacle, const bool& friction):
_persistentRecords(vertices, orientation, inertia, inverse, centre, centreOfMass, referentialCentreOfMass, velocity, angular, referentialAngular, diameter, influenceRadius, epsilon, mass, hMin, globalParticleId, localParticleId, numberOfTriangles, material, isObstacle, friction) {
   
}


dem::records::Particle::~Particle() { }



std::string dem::records::Particle::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void dem::records::Particle::toString (std::ostream& out) const {
   out << "("; 
   out << "vertices:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(6-1) << "]";
   out << ",";
   out << "orientation:[";
   for (int i = 0; i < 9-1; i++) {
      out << getOrientation(i) << ",";
   }
   out << getOrientation(9-1) << "]";
   out << ",";
   out << "inertia:[";
   for (int i = 0; i < 9-1; i++) {
      out << getInertia(i) << ",";
   }
   out << getInertia(9-1) << "]";
   out << ",";
   out << "inverse:[";
   for (int i = 0; i < 9-1; i++) {
      out << getInverse(i) << ",";
   }
   out << getInverse(9-1) << "]";
   out << ",";
   out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
   out << ",";
   out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
   out << ",";
   out << "referentialCentreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getReferentialCentreOfMass(i) << ",";
   }
   out << getReferentialCentreOfMass(DIMENSIONS-1) << "]";
   out << ",";
   out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
   out << ",";
   out << "angular:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getAngular(i) << ",";
   }
   out << getAngular(DIMENSIONS-1) << "]";
   out << ",";
   out << "referentialAngular:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getReferentialAngular(i) << ",";
   }
   out << getReferentialAngular(DIMENSIONS-1) << "]";
   out << ",";
   out << "diameter:" << getDiameter();
   out << ",";
   out << "influenceRadius:" << getInfluenceRadius();
   out << ",";
   out << "epsilon:" << getEpsilon();
   out << ",";
   out << "mass:" << getMass();
   out << ",";
   out << "hMin:" << getHMin();
   out << ",";
   out << "globalParticleId:" << getGlobalParticleId();
   out << ",";
   out << "localParticleId:" << getLocalParticleId();
   out << ",";
   out << "numberOfTriangles:" << getNumberOfTriangles();
   out << ",";
   out << "material:" << getMaterial();
   out << ",";
   out << "isObstacle:" << getIsObstacle();
   out << ",";
   out << "friction:" << getFriction();
   out <<  ")";
}


dem::records::Particle::PersistentRecords dem::records::Particle::getPersistentRecords() const {
   return _persistentRecords;
}

dem::records::ParticlePacked dem::records::Particle::convert() const{
   return ParticlePacked(
      getVertices(),
      getOrientation(),
      getInertia(),
      getInverse(),
      getCentre(),
      getCentreOfMass(),
      getReferentialCentreOfMass(),
      getVelocity(),
      getAngular(),
      getReferentialAngular(),
      getDiameter(),
      getInfluenceRadius(),
      getEpsilon(),
      getMass(),
      getHMin(),
      getGlobalParticleId(),
      getLocalParticleId(),
      getNumberOfTriangles(),
      getMaterial(),
      getIsObstacle(),
      getFriction()
   );
}

#ifdef Parallel
   tarch::logging::Log dem::records::Particle::_log( "dem::records::Particle" );
   
   MPI_Datatype dem::records::Particle::Datatype = 0;
   MPI_Datatype dem::records::Particle::FullDatatype = 0;
   
   
   void dem::records::Particle::initDatatype() {
      {
         Particle dummyParticle[2];
         
         #ifdef MPI2
         const int Attributes = 0;
         #else
         const int Attributes = 1;
         #endif
         MPI_Datatype subtypes[Attributes] = {
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle))), &base);
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
            assertion4(disp[i]<static_cast<int>(sizeof(Particle)), i, disp[i], Attributes, sizeof(Particle));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[1]))), 		&disp[0] );
         disp[0] -= base;
         disp[0] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Particle::Datatype );
         MPI_Type_commit( &Particle::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::Datatype);
         MPI_Type_commit( &Particle::Datatype );
         #endif
         
      }
      {
         Particle dummyParticle[2];
         
         #ifdef MPI2
         const int Attributes = 21;
         #else
         const int Attributes = 22;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_DOUBLE		 //vertices
            , MPI_DOUBLE		 //orientation
            , MPI_DOUBLE		 //inertia
            , MPI_DOUBLE		 //inverse
            , MPI_DOUBLE		 //centre
            , MPI_DOUBLE		 //centreOfMass
            , MPI_DOUBLE		 //referentialCentreOfMass
            , MPI_DOUBLE		 //velocity
            , MPI_DOUBLE		 //angular
            , MPI_DOUBLE		 //referentialAngular
            , MPI_DOUBLE		 //diameter
            , MPI_DOUBLE		 //influenceRadius
            , MPI_DOUBLE		 //epsilon
            , MPI_DOUBLE		 //mass
            , MPI_DOUBLE		 //hMin
            , MPI_INT		 //globalParticleId
            , MPI_INT		 //localParticleId
            , MPI_INT		 //numberOfTriangles
            , MPI_INT		 //material
            , MPI_CXX_BOOL		 //isObstacle
            , MPI_CXX_BOOL		 //friction
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              6		 //vertices
            , 9		 //orientation
            , 9		 //inertia
            , 9		 //inverse
            , DIMENSIONS		 //centre
            , DIMENSIONS		 //centreOfMass
            , DIMENSIONS		 //referentialCentreOfMass
            , DIMENSIONS		 //velocity
            , DIMENSIONS		 //angular
            , DIMENSIONS		 //referentialAngular
            , 1		 //diameter
            , 1		 //influenceRadius
            , 1		 //epsilon
            , 1		 //mass
            , 1		 //hMin
            , 1		 //globalParticleId
            , 1		 //localParticleId
            , 1		 //numberOfTriangles
            , 1		 //material
            , 1		 //isObstacle
            , 1		 //friction
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._vertices[0]))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._vertices[0]))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._orientation[0]))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._orientation[0]))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._inertia[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._inertia[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._inverse[0]))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._inverse[0]))), 		&disp[3] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centre[0]))), 		&disp[4] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centre[0]))), 		&disp[4] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centreOfMass[0]))), 		&disp[5] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centreOfMass[0]))), 		&disp[5] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._referentialCentreOfMass[0]))), 		&disp[6] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._referentialCentreOfMass[0]))), 		&disp[6] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._velocity[0]))), 		&disp[7] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._velocity[0]))), 		&disp[7] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._angular[0]))), 		&disp[8] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._angular[0]))), 		&disp[8] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._referentialAngular[0]))), 		&disp[9] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._referentialAngular[0]))), 		&disp[9] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._diameter))), 		&disp[10] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._diameter))), 		&disp[10] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._influenceRadius))), 		&disp[11] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._influenceRadius))), 		&disp[11] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._epsilon))), 		&disp[12] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._epsilon))), 		&disp[12] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._mass))), 		&disp[13] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._mass))), 		&disp[13] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._hMin))), 		&disp[14] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._hMin))), 		&disp[14] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._globalParticleId))), 		&disp[15] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._globalParticleId))), 		&disp[15] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._localParticleId))), 		&disp[16] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._localParticleId))), 		&disp[16] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._numberOfTriangles))), 		&disp[17] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._numberOfTriangles))), 		&disp[17] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._material))), 		&disp[18] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._material))), 		&disp[18] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._isObstacle))), 		&disp[19] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._isObstacle))), 		&disp[19] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._friction))), 		&disp[20] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._friction))), 		&disp[20] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(Particle)), i, disp[i], Attributes, sizeof(Particle));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[1]))), 		&disp[21] );
         disp[21] -= base;
         disp[21] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &Particle::FullDatatype );
         MPI_Type_commit( &Particle::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::FullDatatype);
         MPI_Type_commit( &Particle::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void dem::records::Particle::shutdownDatatype() {
      MPI_Type_free( &Particle::Datatype );
      MPI_Type_free( &Particle::FullDatatype );
      
   }
   
   void dem::records::Particle::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message dem::records::Particle "
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
            msg << "was not able to send message dem::records::Particle "
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
               msg << "testing for finished send task for dem::records::Particle "
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
               "dem::records::Particle",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::Particle",
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
   
   
   
   void dem::records::Particle::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive dem::records::Particle from node "
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
            msg << "failed to start to receive dem::records::Particle from node "
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
               msg << "testing for finished receive task for dem::records::Particle failed: "
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
               "dem::records::Particle",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::Particle",
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
   
   
   
   bool dem::records::Particle::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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


dem::records::ParticlePacked::PersistentRecords::PersistentRecords() {
   
}


dem::records::ParticlePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<6,double>& vertices, const tarch::la::Vector<9,double>& orientation, const tarch::la::Vector<9,double>& inertia, const tarch::la::Vector<9,double>& inverse, const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const tarch::la::Vector<DIMENSIONS,double>& referentialCentreOfMass, const tarch::la::Vector<DIMENSIONS,double>& velocity, const tarch::la::Vector<DIMENSIONS,double>& angular, const tarch::la::Vector<DIMENSIONS,double>& referentialAngular, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const int& globalParticleId, const int& localParticleId, const int& numberOfTriangles, const int& material, const bool& isObstacle, const bool& friction):
_vertices(vertices),
_orientation(orientation),
_inertia(inertia),
_inverse(inverse),
_centre(centre),
_centreOfMass(centreOfMass),
_referentialCentreOfMass(referentialCentreOfMass),
_velocity(velocity),
_angular(angular),
_referentialAngular(referentialAngular),
_diameter(diameter),
_influenceRadius(influenceRadius),
_epsilon(epsilon),
_mass(mass),
_hMin(hMin),
_globalParticleId(globalParticleId),
_localParticleId(localParticleId),
_numberOfTriangles(numberOfTriangles),
_material(material),
_isObstacle(isObstacle),
_friction(friction) {
   
}

dem::records::ParticlePacked::ParticlePacked() {
   
}


dem::records::ParticlePacked::ParticlePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._vertices, persistentRecords._orientation, persistentRecords._inertia, persistentRecords._inverse, persistentRecords._centre, persistentRecords._centreOfMass, persistentRecords._referentialCentreOfMass, persistentRecords._velocity, persistentRecords._angular, persistentRecords._referentialAngular, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._globalParticleId, persistentRecords._localParticleId, persistentRecords._numberOfTriangles, persistentRecords._material, persistentRecords._isObstacle, persistentRecords._friction) {
   
}


dem::records::ParticlePacked::ParticlePacked(const tarch::la::Vector<6,double>& vertices, const tarch::la::Vector<9,double>& orientation, const tarch::la::Vector<9,double>& inertia, const tarch::la::Vector<9,double>& inverse, const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const tarch::la::Vector<DIMENSIONS,double>& referentialCentreOfMass, const tarch::la::Vector<DIMENSIONS,double>& velocity, const tarch::la::Vector<DIMENSIONS,double>& angular, const tarch::la::Vector<DIMENSIONS,double>& referentialAngular, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const int& globalParticleId, const int& localParticleId, const int& numberOfTriangles, const int& material, const bool& isObstacle, const bool& friction):
_persistentRecords(vertices, orientation, inertia, inverse, centre, centreOfMass, referentialCentreOfMass, velocity, angular, referentialAngular, diameter, influenceRadius, epsilon, mass, hMin, globalParticleId, localParticleId, numberOfTriangles, material, isObstacle, friction) {
   
}


dem::records::ParticlePacked::~ParticlePacked() { }



std::string dem::records::ParticlePacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void dem::records::ParticlePacked::toString (std::ostream& out) const {
   out << "("; 
   out << "vertices:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(6-1) << "]";
   out << ",";
   out << "orientation:[";
   for (int i = 0; i < 9-1; i++) {
      out << getOrientation(i) << ",";
   }
   out << getOrientation(9-1) << "]";
   out << ",";
   out << "inertia:[";
   for (int i = 0; i < 9-1; i++) {
      out << getInertia(i) << ",";
   }
   out << getInertia(9-1) << "]";
   out << ",";
   out << "inverse:[";
   for (int i = 0; i < 9-1; i++) {
      out << getInverse(i) << ",";
   }
   out << getInverse(9-1) << "]";
   out << ",";
   out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
   out << ",";
   out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
   out << ",";
   out << "referentialCentreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getReferentialCentreOfMass(i) << ",";
   }
   out << getReferentialCentreOfMass(DIMENSIONS-1) << "]";
   out << ",";
   out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
   out << ",";
   out << "angular:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getAngular(i) << ",";
   }
   out << getAngular(DIMENSIONS-1) << "]";
   out << ",";
   out << "referentialAngular:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getReferentialAngular(i) << ",";
   }
   out << getReferentialAngular(DIMENSIONS-1) << "]";
   out << ",";
   out << "diameter:" << getDiameter();
   out << ",";
   out << "influenceRadius:" << getInfluenceRadius();
   out << ",";
   out << "epsilon:" << getEpsilon();
   out << ",";
   out << "mass:" << getMass();
   out << ",";
   out << "hMin:" << getHMin();
   out << ",";
   out << "globalParticleId:" << getGlobalParticleId();
   out << ",";
   out << "localParticleId:" << getLocalParticleId();
   out << ",";
   out << "numberOfTriangles:" << getNumberOfTriangles();
   out << ",";
   out << "material:" << getMaterial();
   out << ",";
   out << "isObstacle:" << getIsObstacle();
   out << ",";
   out << "friction:" << getFriction();
   out <<  ")";
}


dem::records::ParticlePacked::PersistentRecords dem::records::ParticlePacked::getPersistentRecords() const {
   return _persistentRecords;
}

dem::records::Particle dem::records::ParticlePacked::convert() const{
   return Particle(
      getVertices(),
      getOrientation(),
      getInertia(),
      getInverse(),
      getCentre(),
      getCentreOfMass(),
      getReferentialCentreOfMass(),
      getVelocity(),
      getAngular(),
      getReferentialAngular(),
      getDiameter(),
      getInfluenceRadius(),
      getEpsilon(),
      getMass(),
      getHMin(),
      getGlobalParticleId(),
      getLocalParticleId(),
      getNumberOfTriangles(),
      getMaterial(),
      getIsObstacle(),
      getFriction()
   );
}

#ifdef Parallel
   tarch::logging::Log dem::records::ParticlePacked::_log( "dem::records::ParticlePacked" );
   
   MPI_Datatype dem::records::ParticlePacked::Datatype = 0;
   MPI_Datatype dem::records::ParticlePacked::FullDatatype = 0;
   
   
   void dem::records::ParticlePacked::initDatatype() {
      {
         ParticlePacked dummyParticlePacked[2];
         
         #ifdef MPI2
         const int Attributes = 0;
         #else
         const int Attributes = 1;
         #endif
         MPI_Datatype subtypes[Attributes] = {
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked))), &base);
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
            assertion4(disp[i]<static_cast<int>(sizeof(ParticlePacked)), i, disp[i], Attributes, sizeof(ParticlePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[1]))), 		&disp[0] );
         disp[0] -= base;
         disp[0] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &ParticlePacked::Datatype );
         MPI_Type_commit( &ParticlePacked::Datatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::Datatype);
         MPI_Type_commit( &ParticlePacked::Datatype );
         #endif
         
      }
      {
         ParticlePacked dummyParticlePacked[2];
         
         #ifdef MPI2
         const int Attributes = 21;
         #else
         const int Attributes = 22;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_DOUBLE		 //vertices
            , MPI_DOUBLE		 //orientation
            , MPI_DOUBLE		 //inertia
            , MPI_DOUBLE		 //inverse
            , MPI_DOUBLE		 //centre
            , MPI_DOUBLE		 //centreOfMass
            , MPI_DOUBLE		 //referentialCentreOfMass
            , MPI_DOUBLE		 //velocity
            , MPI_DOUBLE		 //angular
            , MPI_DOUBLE		 //referentialAngular
            , MPI_DOUBLE		 //diameter
            , MPI_DOUBLE		 //influenceRadius
            , MPI_DOUBLE		 //epsilon
            , MPI_DOUBLE		 //mass
            , MPI_DOUBLE		 //hMin
            , MPI_INT		 //globalParticleId
            , MPI_INT		 //localParticleId
            , MPI_INT		 //numberOfTriangles
            , MPI_INT		 //material
            , MPI_CXX_BOOL		 //isObstacle
            , MPI_CXX_BOOL		 //friction
            #ifndef MPI2
            , MPI_UB
            #endif
            
         };
         
         int blocklen[Attributes] = {
              6		 //vertices
            , 9		 //orientation
            , 9		 //inertia
            , 9		 //inverse
            , DIMENSIONS		 //centre
            , DIMENSIONS		 //centreOfMass
            , DIMENSIONS		 //referentialCentreOfMass
            , DIMENSIONS		 //velocity
            , DIMENSIONS		 //angular
            , DIMENSIONS		 //referentialAngular
            , 1		 //diameter
            , 1		 //influenceRadius
            , 1		 //epsilon
            , 1		 //mass
            , 1		 //hMin
            , 1		 //globalParticleId
            , 1		 //localParticleId
            , 1		 //numberOfTriangles
            , 1		 //material
            , 1		 //isObstacle
            , 1		 //friction
            #ifndef MPI2
            , 1
            #endif
            
         };
         
         MPI_Aint  disp[Attributes];
         MPI_Aint  base;
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked))), &base);
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked))), &base);
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._vertices[0]))), 		&disp[0] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._vertices[0]))), 		&disp[0] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._orientation[0]))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._orientation[0]))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._inertia[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._inertia[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._inverse[0]))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._inverse[0]))), 		&disp[3] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centre[0]))), 		&disp[4] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centre[0]))), 		&disp[4] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centreOfMass[0]))), 		&disp[5] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centreOfMass[0]))), 		&disp[5] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._referentialCentreOfMass[0]))), 		&disp[6] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._referentialCentreOfMass[0]))), 		&disp[6] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._velocity[0]))), 		&disp[7] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._velocity[0]))), 		&disp[7] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._angular[0]))), 		&disp[8] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._angular[0]))), 		&disp[8] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._referentialAngular[0]))), 		&disp[9] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._referentialAngular[0]))), 		&disp[9] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._diameter))), 		&disp[10] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._diameter))), 		&disp[10] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._influenceRadius))), 		&disp[11] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._influenceRadius))), 		&disp[11] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._epsilon))), 		&disp[12] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._epsilon))), 		&disp[12] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._mass))), 		&disp[13] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._mass))), 		&disp[13] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._hMin))), 		&disp[14] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._hMin))), 		&disp[14] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._globalParticleId))), 		&disp[15] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._globalParticleId))), 		&disp[15] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._localParticleId))), 		&disp[16] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._localParticleId))), 		&disp[16] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._numberOfTriangles))), 		&disp[17] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._numberOfTriangles))), 		&disp[17] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._material))), 		&disp[18] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._material))), 		&disp[18] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._isObstacle))), 		&disp[19] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._isObstacle))), 		&disp[19] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._friction))), 		&disp[20] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._friction))), 		&disp[20] );
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
            assertion4(disp[i]<static_cast<int>(sizeof(ParticlePacked)), i, disp[i], Attributes, sizeof(ParticlePacked));
         }
         #ifndef MPI2
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[1]))), 		&disp[21] );
         disp[21] -= base;
         disp[21] += disp[0];
         #endif
         #ifdef MPI2
         MPI_Datatype tmpType; 
         MPI_Aint lowerBound, typeExtent; 
         MPI_Type_create_struct( Attributes, blocklen, disp, subtypes, &tmpType );
         MPI_Type_get_extent( tmpType, &lowerBound, &typeExtent );
         MPI_Type_create_resized( tmpType, lowerBound, typeExtent, &ParticlePacked::FullDatatype );
         MPI_Type_commit( &ParticlePacked::FullDatatype );
         #else
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::FullDatatype);
         MPI_Type_commit( &ParticlePacked::FullDatatype );
         #endif
         
      }
      
   }
   
   
   void dem::records::ParticlePacked::shutdownDatatype() {
      MPI_Type_free( &ParticlePacked::Datatype );
      MPI_Type_free( &ParticlePacked::FullDatatype );
      
   }
   
   void dem::records::ParticlePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         const int result = MPI_Send(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, destination, tag, tarch::parallel::Node::getInstance().getCommunicator());
         if  (result!=MPI_SUCCESS) {
            std::ostringstream msg;
            msg << "was not able to send message dem::records::ParticlePacked "
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
            msg << "was not able to send message dem::records::ParticlePacked "
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
               msg << "testing for finished send task for dem::records::ParticlePacked "
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
               "dem::records::ParticlePacked",
               "send(int)", destination,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::ParticlePacked",
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
   
   
   
   void dem::records::ParticlePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, int communicateSleep) {
      if (communicateSleep<0) {
      
         MPI_Status  status;
         const int   result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &status);
         if ( result != MPI_SUCCESS ) {
            std::ostringstream msg;
            msg << "failed to start to receive dem::records::ParticlePacked from node "
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
            msg << "failed to start to receive dem::records::ParticlePacked from node "
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
               msg << "testing for finished receive task for dem::records::ParticlePacked failed: "
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
               "dem::records::ParticlePacked",
               "receive(int)", source,tag,1
               );
               triggeredTimeoutWarning = true;
            }
            if (
               tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
               (clock()>timeOutShutdown)
            ) {
               tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
               "dem::records::ParticlePacked",
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
   
   
   
   bool dem::records::ParticlePacked::isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise) {
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



