#include "dem/records/Particle.h"

#if !defined(Dim2) && !defined(Dim3)
   dem::records::Particle::PersistentRecords::PersistentRecords() {
      
   }
   
   
   dem::records::Particle::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
   _centre(centre),
   _velocity(velocity),
   _diameter(diameter),
   _influenceRadius(influenceRadius),
   _epsilon(epsilon),
   _mass(mass),
   _hMin(hMin),
   _centreOfMass(centreOfMass),
   _globalParticleNumber(globalParticleNumber),
   _material(material),
   _numberOfTriangles(numberOfTriangles),
   _vertices(vertices) {
      
   }
   
   dem::records::Particle::Particle() {
      
   }
   
   
   dem::records::Particle::Particle(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._centre, persistentRecords._velocity, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._centreOfMass, persistentRecords._globalParticleNumber, persistentRecords._material, persistentRecords._numberOfTriangles, persistentRecords._vertices) {
      
   }
   
   
   dem::records::Particle::Particle(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
   _persistentRecords(centre, velocity, diameter, influenceRadius, epsilon, mass, hMin, centreOfMass, globalParticleNumber, material, numberOfTriangles, vertices) {
      
   }
   
   
   dem::records::Particle::~Particle() { }
   
   
   
   std::string dem::records::Particle::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::Particle::toString (std::ostream& out) const {
      out << "("; 
      out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
      out << ",";
      out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
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
      out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
      out << ",";
      out << "globalParticleNumber:" << getGlobalParticleNumber();
      out << ",";
      out << "material:" << getMaterial();
      out << ",";
      out << "numberOfTriangles:" << getNumberOfTriangles();
      out << ",";
      out << "vertices:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(DIMENSIONS-1) << "]";
      out <<  ")";
   }
   
   
   dem::records::Particle::PersistentRecords dem::records::Particle::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::ParticlePacked dem::records::Particle::convert() const{
      return ParticlePacked(
         getCentre(),
         getVelocity(),
         getDiameter(),
         getInfluenceRadius(),
         getEpsilon(),
         getMass(),
         getHMin(),
         getCentreOfMass(),
         getGlobalParticleNumber(),
         getMaterial(),
         getNumberOfTriangles(),
         getVertices()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::Particle::_log( "dem::records::Particle" );
      
      MPI_Datatype dem::records::Particle::Datatype = 0;
      MPI_Datatype dem::records::Particle::FullDatatype = 0;
      
      
      void dem::records::Particle::initDatatype() {
         {
            Particle dummyParticle[2];
            
            const int Attributes = 1;
            MPI_Datatype subtypes[Attributes] = {
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]))), &base);
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::Datatype );
            MPI_Type_commit( &Particle::Datatype );
            
         }
         {
            Particle dummyParticle[2];
            
            const int Attributes = 13;
            MPI_Datatype subtypes[Attributes] = {
               MPI_DOUBLE,		 //centre
               MPI_DOUBLE,		 //velocity
               MPI_DOUBLE,		 //diameter
               MPI_DOUBLE,		 //influenceRadius
               MPI_DOUBLE,		 //epsilon
               MPI_DOUBLE,		 //mass
               MPI_DOUBLE,		 //hMin
               MPI_DOUBLE,		 //centreOfMass
               MPI_INT,		 //globalParticleNumber
               MPI_INT,		 //material
               MPI_INT,		 //numberOfTriangles
               MPI_INT,		 //vertices
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               DIMENSIONS,		 //centre
               DIMENSIONS,		 //velocity
               1,		 //diameter
               1,		 //influenceRadius
               1,		 //epsilon
               1,		 //mass
               1,		 //hMin
               DIMENSIONS,		 //centreOfMass
               1,		 //globalParticleNumber
               1,		 //material
               1,		 //numberOfTriangles
               DIMENSIONS,		 //vertices
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centre[0]))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._velocity[0]))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._diameter))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._influenceRadius))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._epsilon))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._mass))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._hMin))), 		&disp[6] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centreOfMass[0]))), 		&disp[7] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._globalParticleNumber))), 		&disp[8] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._material))), 		&disp[9] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._numberOfTriangles))), 		&disp[10] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._vertices[0]))), 		&disp[11] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyParticle[1]._persistentRecords._centre[0])), 		&disp[12] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::FullDatatype );
            MPI_Type_commit( &Particle::FullDatatype );
            
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
   
   
   dem::records::ParticlePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
   _centre(centre),
   _velocity(velocity),
   _diameter(diameter),
   _influenceRadius(influenceRadius),
   _epsilon(epsilon),
   _mass(mass),
   _hMin(hMin),
   _centreOfMass(centreOfMass),
   _globalParticleNumber(globalParticleNumber),
   _material(material),
   _numberOfTriangles(numberOfTriangles),
   _vertices(vertices) {
      
   }
   
   dem::records::ParticlePacked::ParticlePacked() {
      
   }
   
   
   dem::records::ParticlePacked::ParticlePacked(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._centre, persistentRecords._velocity, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._centreOfMass, persistentRecords._globalParticleNumber, persistentRecords._material, persistentRecords._numberOfTriangles, persistentRecords._vertices) {
      
   }
   
   
   dem::records::ParticlePacked::ParticlePacked(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
   _persistentRecords(centre, velocity, diameter, influenceRadius, epsilon, mass, hMin, centreOfMass, globalParticleNumber, material, numberOfTriangles, vertices) {
      
   }
   
   
   dem::records::ParticlePacked::~ParticlePacked() { }
   
   
   
   std::string dem::records::ParticlePacked::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::ParticlePacked::toString (std::ostream& out) const {
      out << "("; 
      out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
      out << ",";
      out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
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
      out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
      out << ",";
      out << "globalParticleNumber:" << getGlobalParticleNumber();
      out << ",";
      out << "material:" << getMaterial();
      out << ",";
      out << "numberOfTriangles:" << getNumberOfTriangles();
      out << ",";
      out << "vertices:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(DIMENSIONS-1) << "]";
      out <<  ")";
   }
   
   
   dem::records::ParticlePacked::PersistentRecords dem::records::ParticlePacked::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::Particle dem::records::ParticlePacked::convert() const{
      return Particle(
         getCentre(),
         getVelocity(),
         getDiameter(),
         getInfluenceRadius(),
         getEpsilon(),
         getMass(),
         getHMin(),
         getCentreOfMass(),
         getGlobalParticleNumber(),
         getMaterial(),
         getNumberOfTriangles(),
         getVertices()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::ParticlePacked::_log( "dem::records::ParticlePacked" );
      
      MPI_Datatype dem::records::ParticlePacked::Datatype = 0;
      MPI_Datatype dem::records::ParticlePacked::FullDatatype = 0;
      
      
      void dem::records::ParticlePacked::initDatatype() {
         {
            ParticlePacked dummyParticlePacked[2];
            
            const int Attributes = 1;
            MPI_Datatype subtypes[Attributes] = {
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]))), &base);
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::Datatype );
            MPI_Type_commit( &ParticlePacked::Datatype );
            
         }
         {
            ParticlePacked dummyParticlePacked[2];
            
            const int Attributes = 13;
            MPI_Datatype subtypes[Attributes] = {
               MPI_DOUBLE,		 //centre
               MPI_DOUBLE,		 //velocity
               MPI_DOUBLE,		 //diameter
               MPI_DOUBLE,		 //influenceRadius
               MPI_DOUBLE,		 //epsilon
               MPI_DOUBLE,		 //mass
               MPI_DOUBLE,		 //hMin
               MPI_DOUBLE,		 //centreOfMass
               MPI_INT,		 //globalParticleNumber
               MPI_INT,		 //material
               MPI_INT,		 //numberOfTriangles
               MPI_INT,		 //vertices
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               DIMENSIONS,		 //centre
               DIMENSIONS,		 //velocity
               1,		 //diameter
               1,		 //influenceRadius
               1,		 //epsilon
               1,		 //mass
               1,		 //hMin
               DIMENSIONS,		 //centreOfMass
               1,		 //globalParticleNumber
               1,		 //material
               1,		 //numberOfTriangles
               DIMENSIONS,		 //vertices
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centre[0]))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._velocity[0]))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._diameter))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._influenceRadius))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._epsilon))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._mass))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._hMin))), 		&disp[6] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centreOfMass[0]))), 		&disp[7] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._globalParticleNumber))), 		&disp[8] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._material))), 		&disp[9] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._numberOfTriangles))), 		&disp[10] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._vertices[0]))), 		&disp[11] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyParticlePacked[1]._persistentRecords._centre[0])), 		&disp[12] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::FullDatatype );
            MPI_Type_commit( &ParticlePacked::FullDatatype );
            
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
   
   
   
#elif defined(Dim2) && defined(Dim3)
   dem::records::Particle::PersistentRecords::PersistentRecords() {
      
   }
   
   
   dem::records::Particle::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const double& angularVelocity, const tarch::la::Vector<DIMENSIONS,double>& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
   _centre(centre),
   _velocity(velocity),
   _diameter(diameter),
   _influenceRadius(influenceRadius),
   _epsilon(epsilon),
   _mass(mass),
   _hMin(hMin),
   _centreOfMass(centreOfMass),
   _globalParticleNumber(globalParticleNumber),
   _material(material),
   _angularVelocity(angularVelocity),
   _angularVelocity(angularVelocity),
   _numberOfTriangles(numberOfTriangles),
   _vertices(vertices) {
      
   }
   
   dem::records::Particle::Particle() {
      
   }
   
   
   dem::records::Particle::Particle(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._centre, persistentRecords._velocity, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._centreOfMass, persistentRecords._globalParticleNumber, persistentRecords._material, persistentRecords._angularVelocity, persistentRecords._angularVelocity, persistentRecords._numberOfTriangles, persistentRecords._vertices) {
      
   }
   
   
   dem::records::Particle::Particle(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const tarch::la::Vector<DIMENSIONS,double>& angularVelocity, const double& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
   _persistentRecords(centre, velocity, diameter, influenceRadius, epsilon, mass, hMin, centreOfMass, globalParticleNumber, material, angularVelocity, angularVelocity, numberOfTriangles, vertices) {
      
   }
   
   
   dem::records::Particle::~Particle() { }
   
   
   
   std::string dem::records::Particle::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::Particle::toString (std::ostream& out) const {
      out << "("; 
      out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
      out << ",";
      out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
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
      out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
      out << ",";
      out << "globalParticleNumber:" << getGlobalParticleNumber();
      out << ",";
      out << "material:" << getMaterial();
      out << ",";
      out << "angularVelocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getAngularVelocity(i) << ",";
   }
   out << getAngularVelocity(DIMENSIONS-1) << "]";
      out << ",";
      out << "angularVelocity:" << getAngularVelocity();
      out << ",";
      out << "numberOfTriangles:" << getNumberOfTriangles();
      out << ",";
      out << "vertices:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(DIMENSIONS-1) << "]";
      out <<  ")";
   }
   
   
   dem::records::Particle::PersistentRecords dem::records::Particle::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::ParticlePacked dem::records::Particle::convert() const{
      return ParticlePacked(
         getCentre(),
         getVelocity(),
         getDiameter(),
         getInfluenceRadius(),
         getEpsilon(),
         getMass(),
         getHMin(),
         getCentreOfMass(),
         getGlobalParticleNumber(),
         getMaterial(),
         getAngularVelocity(),
         getAngularVelocity(),
         getNumberOfTriangles(),
         getVertices()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::Particle::_log( "dem::records::Particle" );
      
      MPI_Datatype dem::records::Particle::Datatype = 0;
      MPI_Datatype dem::records::Particle::FullDatatype = 0;
      
      
      void dem::records::Particle::initDatatype() {
         {
            Particle dummyParticle[2];
            
            const int Attributes = 1;
            MPI_Datatype subtypes[Attributes] = {
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]))), &base);
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::Datatype );
            MPI_Type_commit( &Particle::Datatype );
            
         }
         {
            Particle dummyParticle[2];
            
            const int Attributes = 15;
            MPI_Datatype subtypes[Attributes] = {
               MPI_DOUBLE,		 //centre
               MPI_DOUBLE,		 //velocity
               MPI_DOUBLE,		 //diameter
               MPI_DOUBLE,		 //influenceRadius
               MPI_DOUBLE,		 //epsilon
               MPI_DOUBLE,		 //mass
               MPI_DOUBLE,		 //hMin
               MPI_DOUBLE,		 //centreOfMass
               MPI_INT,		 //globalParticleNumber
               MPI_INT,		 //material
               MPI_DOUBLE,		 //angularVelocity
               MPI_DOUBLE,		 //angularVelocity
               MPI_INT,		 //numberOfTriangles
               MPI_INT,		 //vertices
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               DIMENSIONS,		 //centre
               DIMENSIONS,		 //velocity
               1,		 //diameter
               1,		 //influenceRadius
               1,		 //epsilon
               1,		 //mass
               1,		 //hMin
               DIMENSIONS,		 //centreOfMass
               1,		 //globalParticleNumber
               1,		 //material
               1,		 //angularVelocity
               DIMENSIONS,		 //angularVelocity
               1,		 //numberOfTriangles
               DIMENSIONS,		 //vertices
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centre[0]))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._velocity[0]))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._diameter))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._influenceRadius))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._epsilon))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._mass))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._hMin))), 		&disp[6] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centreOfMass[0]))), 		&disp[7] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._globalParticleNumber))), 		&disp[8] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._material))), 		&disp[9] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._angularVelocity))), 		&disp[10] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._angularVelocity[0]))), 		&disp[11] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._numberOfTriangles))), 		&disp[12] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._vertices[0]))), 		&disp[13] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyParticle[1]._persistentRecords._centre[0])), 		&disp[14] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::FullDatatype );
            MPI_Type_commit( &Particle::FullDatatype );
            
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
   
   
   dem::records::ParticlePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const double& angularVelocity, const tarch::la::Vector<DIMENSIONS,double>& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
   _centre(centre),
   _velocity(velocity),
   _diameter(diameter),
   _influenceRadius(influenceRadius),
   _epsilon(epsilon),
   _mass(mass),
   _hMin(hMin),
   _centreOfMass(centreOfMass),
   _globalParticleNumber(globalParticleNumber),
   _material(material),
   _angularVelocity(angularVelocity),
   _angularVelocity(angularVelocity),
   _numberOfTriangles(numberOfTriangles),
   _vertices(vertices) {
      
   }
   
   dem::records::ParticlePacked::ParticlePacked() {
      
   }
   
   
   dem::records::ParticlePacked::ParticlePacked(const PersistentRecords& persistentRecords):
   _persistentRecords(persistentRecords._centre, persistentRecords._velocity, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._centreOfMass, persistentRecords._globalParticleNumber, persistentRecords._material, persistentRecords._angularVelocity, persistentRecords._angularVelocity, persistentRecords._numberOfTriangles, persistentRecords._vertices) {
      
   }
   
   
   dem::records::ParticlePacked::ParticlePacked(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const tarch::la::Vector<DIMENSIONS,double>& angularVelocity, const double& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
   _persistentRecords(centre, velocity, diameter, influenceRadius, epsilon, mass, hMin, centreOfMass, globalParticleNumber, material, angularVelocity, angularVelocity, numberOfTriangles, vertices) {
      
   }
   
   
   dem::records::ParticlePacked::~ParticlePacked() { }
   
   
   
   std::string dem::records::ParticlePacked::toString() const {
      std::ostringstream stringstr;
      toString(stringstr);
      return stringstr.str();
   }
   
   void dem::records::ParticlePacked::toString (std::ostream& out) const {
      out << "("; 
      out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
      out << ",";
      out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
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
      out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
      out << ",";
      out << "globalParticleNumber:" << getGlobalParticleNumber();
      out << ",";
      out << "material:" << getMaterial();
      out << ",";
      out << "angularVelocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getAngularVelocity(i) << ",";
   }
   out << getAngularVelocity(DIMENSIONS-1) << "]";
      out << ",";
      out << "angularVelocity:" << getAngularVelocity();
      out << ",";
      out << "numberOfTriangles:" << getNumberOfTriangles();
      out << ",";
      out << "vertices:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(DIMENSIONS-1) << "]";
      out <<  ")";
   }
   
   
   dem::records::ParticlePacked::PersistentRecords dem::records::ParticlePacked::getPersistentRecords() const {
      return _persistentRecords;
   }
   
   dem::records::Particle dem::records::ParticlePacked::convert() const{
      return Particle(
         getCentre(),
         getVelocity(),
         getDiameter(),
         getInfluenceRadius(),
         getEpsilon(),
         getMass(),
         getHMin(),
         getCentreOfMass(),
         getGlobalParticleNumber(),
         getMaterial(),
         getAngularVelocity(),
         getAngularVelocity(),
         getNumberOfTriangles(),
         getVertices()
      );
   }
   
   #ifdef Parallel
      tarch::logging::Log dem::records::ParticlePacked::_log( "dem::records::ParticlePacked" );
      
      MPI_Datatype dem::records::ParticlePacked::Datatype = 0;
      MPI_Datatype dem::records::ParticlePacked::FullDatatype = 0;
      
      
      void dem::records::ParticlePacked::initDatatype() {
         {
            ParticlePacked dummyParticlePacked[2];
            
            const int Attributes = 1;
            MPI_Datatype subtypes[Attributes] = {
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]))), &base);
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::Datatype );
            MPI_Type_commit( &ParticlePacked::Datatype );
            
         }
         {
            ParticlePacked dummyParticlePacked[2];
            
            const int Attributes = 15;
            MPI_Datatype subtypes[Attributes] = {
               MPI_DOUBLE,		 //centre
               MPI_DOUBLE,		 //velocity
               MPI_DOUBLE,		 //diameter
               MPI_DOUBLE,		 //influenceRadius
               MPI_DOUBLE,		 //epsilon
               MPI_DOUBLE,		 //mass
               MPI_DOUBLE,		 //hMin
               MPI_DOUBLE,		 //centreOfMass
               MPI_INT,		 //globalParticleNumber
               MPI_INT,		 //material
               MPI_DOUBLE,		 //angularVelocity
               MPI_DOUBLE,		 //angularVelocity
               MPI_INT,		 //numberOfTriangles
               MPI_INT,		 //vertices
               MPI_UB		 // end/displacement flag
            };
            
            int blocklen[Attributes] = {
               DIMENSIONS,		 //centre
               DIMENSIONS,		 //velocity
               1,		 //diameter
               1,		 //influenceRadius
               1,		 //epsilon
               1,		 //mass
               1,		 //hMin
               DIMENSIONS,		 //centreOfMass
               1,		 //globalParticleNumber
               1,		 //material
               1,		 //angularVelocity
               DIMENSIONS,		 //angularVelocity
               1,		 //numberOfTriangles
               DIMENSIONS,		 //vertices
               1		 // end/displacement flag
            };
            
            MPI_Aint     disp[Attributes];
            
            MPI_Aint base;
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]))), &base);
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centre[0]))), 		&disp[0] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._velocity[0]))), 		&disp[1] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._diameter))), 		&disp[2] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._influenceRadius))), 		&disp[3] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._epsilon))), 		&disp[4] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._mass))), 		&disp[5] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._hMin))), 		&disp[6] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centreOfMass[0]))), 		&disp[7] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._globalParticleNumber))), 		&disp[8] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._material))), 		&disp[9] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._angularVelocity))), 		&disp[10] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._angularVelocity[0]))), 		&disp[11] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._numberOfTriangles))), 		&disp[12] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._vertices[0]))), 		&disp[13] );
            MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyParticlePacked[1]._persistentRecords._centre[0])), 		&disp[14] );
            
            for (int i=1; i<Attributes; i++) {
               assertion1( disp[i] > disp[i-1], i );
            }
            for (int i=0; i<Attributes; i++) {
               disp[i] -= base;
            }
            MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::FullDatatype );
            MPI_Type_commit( &ParticlePacked::FullDatatype );
            
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
   
   
   

#elif defined(Dim2) && !defined(Dim3)
dem::records::Particle::PersistentRecords::PersistentRecords() {
   
}


dem::records::Particle::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const double& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
_centre(centre),
_velocity(velocity),
_diameter(diameter),
_influenceRadius(influenceRadius),
_epsilon(epsilon),
_mass(mass),
_hMin(hMin),
_centreOfMass(centreOfMass),
_globalParticleNumber(globalParticleNumber),
_material(material),
_angularVelocity(angularVelocity),
_numberOfTriangles(numberOfTriangles),
_vertices(vertices) {
   
}

dem::records::Particle::Particle() {
   
}


dem::records::Particle::Particle(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._centre, persistentRecords._velocity, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._centreOfMass, persistentRecords._globalParticleNumber, persistentRecords._material, persistentRecords._angularVelocity, persistentRecords._numberOfTriangles, persistentRecords._vertices) {
   
}


dem::records::Particle::Particle(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const double& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
_persistentRecords(centre, velocity, diameter, influenceRadius, epsilon, mass, hMin, centreOfMass, globalParticleNumber, material, angularVelocity, numberOfTriangles, vertices) {
   
}


dem::records::Particle::~Particle() { }



std::string dem::records::Particle::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void dem::records::Particle::toString (std::ostream& out) const {
   out << "("; 
   out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
   out << ",";
   out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
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
   out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
   out << ",";
   out << "globalParticleNumber:" << getGlobalParticleNumber();
   out << ",";
   out << "material:" << getMaterial();
   out << ",";
   out << "angularVelocity:" << getAngularVelocity();
   out << ",";
   out << "numberOfTriangles:" << getNumberOfTriangles();
   out << ",";
   out << "vertices:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(DIMENSIONS-1) << "]";
   out <<  ")";
}


dem::records::Particle::PersistentRecords dem::records::Particle::getPersistentRecords() const {
   return _persistentRecords;
}

dem::records::ParticlePacked dem::records::Particle::convert() const{
   return ParticlePacked(
      getCentre(),
      getVelocity(),
      getDiameter(),
      getInfluenceRadius(),
      getEpsilon(),
      getMass(),
      getHMin(),
      getCentreOfMass(),
      getGlobalParticleNumber(),
      getMaterial(),
      getAngularVelocity(),
      getNumberOfTriangles(),
      getVertices()
   );
}

#ifdef Parallel
   tarch::logging::Log dem::records::Particle::_log( "dem::records::Particle" );
   
   MPI_Datatype dem::records::Particle::Datatype = 0;
   MPI_Datatype dem::records::Particle::FullDatatype = 0;
   
   
   void dem::records::Particle::initDatatype() {
      {
         Particle dummyParticle[2];
         
         const int Attributes = 1;
         MPI_Datatype subtypes[Attributes] = {
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]))), &base);
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::Datatype );
         MPI_Type_commit( &Particle::Datatype );
         
      }
      {
         Particle dummyParticle[2];
         
         const int Attributes = 14;
         MPI_Datatype subtypes[Attributes] = {
            MPI_DOUBLE,		 //centre
            MPI_DOUBLE,		 //velocity
            MPI_DOUBLE,		 //diameter
            MPI_DOUBLE,		 //influenceRadius
            MPI_DOUBLE,		 //epsilon
            MPI_DOUBLE,		 //mass
            MPI_DOUBLE,		 //hMin
            MPI_DOUBLE,		 //centreOfMass
            MPI_INT,		 //globalParticleNumber
            MPI_INT,		 //material
            MPI_DOUBLE,		 //angularVelocity
            MPI_INT,		 //numberOfTriangles
            MPI_INT,		 //vertices
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            DIMENSIONS,		 //centre
            DIMENSIONS,		 //velocity
            1,		 //diameter
            1,		 //influenceRadius
            1,		 //epsilon
            1,		 //mass
            1,		 //hMin
            DIMENSIONS,		 //centreOfMass
            1,		 //globalParticleNumber
            1,		 //material
            1,		 //angularVelocity
            1,		 //numberOfTriangles
            DIMENSIONS,		 //vertices
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centre[0]))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._velocity[0]))), 		&disp[1] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._diameter))), 		&disp[2] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._influenceRadius))), 		&disp[3] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._epsilon))), 		&disp[4] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._mass))), 		&disp[5] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._hMin))), 		&disp[6] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centreOfMass[0]))), 		&disp[7] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._globalParticleNumber))), 		&disp[8] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._material))), 		&disp[9] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._angularVelocity))), 		&disp[10] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._numberOfTriangles))), 		&disp[11] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._vertices[0]))), 		&disp[12] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyParticle[1]._persistentRecords._centre[0])), 		&disp[13] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::FullDatatype );
         MPI_Type_commit( &Particle::FullDatatype );
         
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


dem::records::ParticlePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const double& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
_centre(centre),
_velocity(velocity),
_diameter(diameter),
_influenceRadius(influenceRadius),
_epsilon(epsilon),
_mass(mass),
_hMin(hMin),
_centreOfMass(centreOfMass),
_globalParticleNumber(globalParticleNumber),
_material(material),
_angularVelocity(angularVelocity),
_numberOfTriangles(numberOfTriangles),
_vertices(vertices) {
   
}

dem::records::ParticlePacked::ParticlePacked() {
   
}


dem::records::ParticlePacked::ParticlePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._centre, persistentRecords._velocity, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._centreOfMass, persistentRecords._globalParticleNumber, persistentRecords._material, persistentRecords._angularVelocity, persistentRecords._numberOfTriangles, persistentRecords._vertices) {
   
}


dem::records::ParticlePacked::ParticlePacked(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const double& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
_persistentRecords(centre, velocity, diameter, influenceRadius, epsilon, mass, hMin, centreOfMass, globalParticleNumber, material, angularVelocity, numberOfTriangles, vertices) {
   
}


dem::records::ParticlePacked::~ParticlePacked() { }



std::string dem::records::ParticlePacked::toString() const {
   std::ostringstream stringstr;
   toString(stringstr);
   return stringstr.str();
}

void dem::records::ParticlePacked::toString (std::ostream& out) const {
   out << "("; 
   out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
   out << ",";
   out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
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
   out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
   out << ",";
   out << "globalParticleNumber:" << getGlobalParticleNumber();
   out << ",";
   out << "material:" << getMaterial();
   out << ",";
   out << "angularVelocity:" << getAngularVelocity();
   out << ",";
   out << "numberOfTriangles:" << getNumberOfTriangles();
   out << ",";
   out << "vertices:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(DIMENSIONS-1) << "]";
   out <<  ")";
}


dem::records::ParticlePacked::PersistentRecords dem::records::ParticlePacked::getPersistentRecords() const {
   return _persistentRecords;
}

dem::records::Particle dem::records::ParticlePacked::convert() const{
   return Particle(
      getCentre(),
      getVelocity(),
      getDiameter(),
      getInfluenceRadius(),
      getEpsilon(),
      getMass(),
      getHMin(),
      getCentreOfMass(),
      getGlobalParticleNumber(),
      getMaterial(),
      getAngularVelocity(),
      getNumberOfTriangles(),
      getVertices()
   );
}

#ifdef Parallel
   tarch::logging::Log dem::records::ParticlePacked::_log( "dem::records::ParticlePacked" );
   
   MPI_Datatype dem::records::ParticlePacked::Datatype = 0;
   MPI_Datatype dem::records::ParticlePacked::FullDatatype = 0;
   
   
   void dem::records::ParticlePacked::initDatatype() {
      {
         ParticlePacked dummyParticlePacked[2];
         
         const int Attributes = 1;
         MPI_Datatype subtypes[Attributes] = {
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]))), &base);
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::Datatype );
         MPI_Type_commit( &ParticlePacked::Datatype );
         
      }
      {
         ParticlePacked dummyParticlePacked[2];
         
         const int Attributes = 14;
         MPI_Datatype subtypes[Attributes] = {
            MPI_DOUBLE,		 //centre
            MPI_DOUBLE,		 //velocity
            MPI_DOUBLE,		 //diameter
            MPI_DOUBLE,		 //influenceRadius
            MPI_DOUBLE,		 //epsilon
            MPI_DOUBLE,		 //mass
            MPI_DOUBLE,		 //hMin
            MPI_DOUBLE,		 //centreOfMass
            MPI_INT,		 //globalParticleNumber
            MPI_INT,		 //material
            MPI_DOUBLE,		 //angularVelocity
            MPI_INT,		 //numberOfTriangles
            MPI_INT,		 //vertices
            MPI_UB		 // end/displacement flag
         };
         
         int blocklen[Attributes] = {
            DIMENSIONS,		 //centre
            DIMENSIONS,		 //velocity
            1,		 //diameter
            1,		 //influenceRadius
            1,		 //epsilon
            1,		 //mass
            1,		 //hMin
            DIMENSIONS,		 //centreOfMass
            1,		 //globalParticleNumber
            1,		 //material
            1,		 //angularVelocity
            1,		 //numberOfTriangles
            DIMENSIONS,		 //vertices
            1		 // end/displacement flag
         };
         
         MPI_Aint     disp[Attributes];
         
         MPI_Aint base;
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]))), &base);
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centre[0]))), 		&disp[0] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._velocity[0]))), 		&disp[1] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._diameter))), 		&disp[2] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._influenceRadius))), 		&disp[3] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._epsilon))), 		&disp[4] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._mass))), 		&disp[5] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._hMin))), 		&disp[6] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centreOfMass[0]))), 		&disp[7] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._globalParticleNumber))), 		&disp[8] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._material))), 		&disp[9] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._angularVelocity))), 		&disp[10] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._numberOfTriangles))), 		&disp[11] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._vertices[0]))), 		&disp[12] );
         MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyParticlePacked[1]._persistentRecords._centre[0])), 		&disp[13] );
         
         for (int i=1; i<Attributes; i++) {
            assertion1( disp[i] > disp[i-1], i );
         }
         for (int i=0; i<Attributes; i++) {
            disp[i] -= base;
         }
         MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::FullDatatype );
         MPI_Type_commit( &ParticlePacked::FullDatatype );
         
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




#elif !defined(Dim2) && defined(Dim3)
dem::records::Particle::PersistentRecords::PersistentRecords() {

}


dem::records::Particle::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const tarch::la::Vector<DIMENSIONS,double>& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
_centre(centre),
_velocity(velocity),
_diameter(diameter),
_influenceRadius(influenceRadius),
_epsilon(epsilon),
_mass(mass),
_hMin(hMin),
_centreOfMass(centreOfMass),
_globalParticleNumber(globalParticleNumber),
_material(material),
_angularVelocity(angularVelocity),
_numberOfTriangles(numberOfTriangles),
_vertices(vertices) {

}

dem::records::Particle::Particle() {

}


dem::records::Particle::Particle(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._centre, persistentRecords._velocity, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._centreOfMass, persistentRecords._globalParticleNumber, persistentRecords._material, persistentRecords._angularVelocity, persistentRecords._numberOfTriangles, persistentRecords._vertices) {

}


dem::records::Particle::Particle(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const tarch::la::Vector<DIMENSIONS,double>& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
_persistentRecords(centre, velocity, diameter, influenceRadius, epsilon, mass, hMin, centreOfMass, globalParticleNumber, material, angularVelocity, numberOfTriangles, vertices) {

}


dem::records::Particle::~Particle() { }



std::string dem::records::Particle::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void dem::records::Particle::toString (std::ostream& out) const {
out << "("; 
out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
out << ",";
out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
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
out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
out << ",";
out << "globalParticleNumber:" << getGlobalParticleNumber();
out << ",";
out << "material:" << getMaterial();
out << ",";
out << "angularVelocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getAngularVelocity(i) << ",";
   }
   out << getAngularVelocity(DIMENSIONS-1) << "]";
out << ",";
out << "numberOfTriangles:" << getNumberOfTriangles();
out << ",";
out << "vertices:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(DIMENSIONS-1) << "]";
out <<  ")";
}


dem::records::Particle::PersistentRecords dem::records::Particle::getPersistentRecords() const {
return _persistentRecords;
}

dem::records::ParticlePacked dem::records::Particle::convert() const{
return ParticlePacked(
   getCentre(),
   getVelocity(),
   getDiameter(),
   getInfluenceRadius(),
   getEpsilon(),
   getMass(),
   getHMin(),
   getCentreOfMass(),
   getGlobalParticleNumber(),
   getMaterial(),
   getAngularVelocity(),
   getNumberOfTriangles(),
   getVertices()
);
}

#ifdef Parallel
tarch::logging::Log dem::records::Particle::_log( "dem::records::Particle" );

MPI_Datatype dem::records::Particle::Datatype = 0;
MPI_Datatype dem::records::Particle::FullDatatype = 0;


void dem::records::Particle::initDatatype() {
   {
      Particle dummyParticle[2];
      
      const int Attributes = 1;
      MPI_Datatype subtypes[Attributes] = {
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]))), &base);
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::Datatype );
      MPI_Type_commit( &Particle::Datatype );
      
   }
   {
      Particle dummyParticle[2];
      
      const int Attributes = 14;
      MPI_Datatype subtypes[Attributes] = {
         MPI_DOUBLE,		 //centre
         MPI_DOUBLE,		 //velocity
         MPI_DOUBLE,		 //diameter
         MPI_DOUBLE,		 //influenceRadius
         MPI_DOUBLE,		 //epsilon
         MPI_DOUBLE,		 //mass
         MPI_DOUBLE,		 //hMin
         MPI_DOUBLE,		 //centreOfMass
         MPI_INT,		 //globalParticleNumber
         MPI_INT,		 //material
         MPI_DOUBLE,		 //angularVelocity
         MPI_INT,		 //numberOfTriangles
         MPI_INT,		 //vertices
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         DIMENSIONS,		 //centre
         DIMENSIONS,		 //velocity
         1,		 //diameter
         1,		 //influenceRadius
         1,		 //epsilon
         1,		 //mass
         1,		 //hMin
         DIMENSIONS,		 //centreOfMass
         1,		 //globalParticleNumber
         1,		 //material
         DIMENSIONS,		 //angularVelocity
         1,		 //numberOfTriangles
         DIMENSIONS,		 //vertices
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]))), &base);
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centre[0]))), 		&disp[0] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._velocity[0]))), 		&disp[1] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._diameter))), 		&disp[2] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._influenceRadius))), 		&disp[3] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._epsilon))), 		&disp[4] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._mass))), 		&disp[5] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._hMin))), 		&disp[6] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centreOfMass[0]))), 		&disp[7] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._globalParticleNumber))), 		&disp[8] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._material))), 		&disp[9] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._angularVelocity[0]))), 		&disp[10] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._numberOfTriangles))), 		&disp[11] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._vertices[0]))), 		&disp[12] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyParticle[1]._persistentRecords._centre[0])), 		&disp[13] );
      
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &Particle::FullDatatype );
      MPI_Type_commit( &Particle::FullDatatype );
      
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


dem::records::ParticlePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const tarch::la::Vector<DIMENSIONS,double>& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
_centre(centre),
_velocity(velocity),
_diameter(diameter),
_influenceRadius(influenceRadius),
_epsilon(epsilon),
_mass(mass),
_hMin(hMin),
_centreOfMass(centreOfMass),
_globalParticleNumber(globalParticleNumber),
_material(material),
_angularVelocity(angularVelocity),
_numberOfTriangles(numberOfTriangles),
_vertices(vertices) {

}

dem::records::ParticlePacked::ParticlePacked() {

}


dem::records::ParticlePacked::ParticlePacked(const PersistentRecords& persistentRecords):
_persistentRecords(persistentRecords._centre, persistentRecords._velocity, persistentRecords._diameter, persistentRecords._influenceRadius, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._centreOfMass, persistentRecords._globalParticleNumber, persistentRecords._material, persistentRecords._angularVelocity, persistentRecords._numberOfTriangles, persistentRecords._vertices) {

}


dem::records::ParticlePacked::ParticlePacked(const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& velocity, const double& diameter, const double& influenceRadius, const double& epsilon, const double& mass, const double& hMin, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const int& globalParticleNumber, const int& material, const tarch::la::Vector<DIMENSIONS,double>& angularVelocity, const int& numberOfTriangles, const tarch::la::Vector<DIMENSIONS,int>& vertices):
_persistentRecords(centre, velocity, diameter, influenceRadius, epsilon, mass, hMin, centreOfMass, globalParticleNumber, material, angularVelocity, numberOfTriangles, vertices) {

}


dem::records::ParticlePacked::~ParticlePacked() { }



std::string dem::records::ParticlePacked::toString() const {
std::ostringstream stringstr;
toString(stringstr);
return stringstr.str();
}

void dem::records::ParticlePacked::toString (std::ostream& out) const {
out << "("; 
out << "centre:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentre(i) << ",";
   }
   out << getCentre(DIMENSIONS-1) << "]";
out << ",";
out << "velocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVelocity(i) << ",";
   }
   out << getVelocity(DIMENSIONS-1) << "]";
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
out << "centreOfMass:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getCentreOfMass(i) << ",";
   }
   out << getCentreOfMass(DIMENSIONS-1) << "]";
out << ",";
out << "globalParticleNumber:" << getGlobalParticleNumber();
out << ",";
out << "material:" << getMaterial();
out << ",";
out << "angularVelocity:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getAngularVelocity(i) << ",";
   }
   out << getAngularVelocity(DIMENSIONS-1) << "]";
out << ",";
out << "numberOfTriangles:" << getNumberOfTriangles();
out << ",";
out << "vertices:[";
   for (int i = 0; i < DIMENSIONS-1; i++) {
      out << getVertices(i) << ",";
   }
   out << getVertices(DIMENSIONS-1) << "]";
out <<  ")";
}


dem::records::ParticlePacked::PersistentRecords dem::records::ParticlePacked::getPersistentRecords() const {
return _persistentRecords;
}

dem::records::Particle dem::records::ParticlePacked::convert() const{
return Particle(
   getCentre(),
   getVelocity(),
   getDiameter(),
   getInfluenceRadius(),
   getEpsilon(),
   getMass(),
   getHMin(),
   getCentreOfMass(),
   getGlobalParticleNumber(),
   getMaterial(),
   getAngularVelocity(),
   getNumberOfTriangles(),
   getVertices()
);
}

#ifdef Parallel
tarch::logging::Log dem::records::ParticlePacked::_log( "dem::records::ParticlePacked" );

MPI_Datatype dem::records::ParticlePacked::Datatype = 0;
MPI_Datatype dem::records::ParticlePacked::FullDatatype = 0;


void dem::records::ParticlePacked::initDatatype() {
   {
      ParticlePacked dummyParticlePacked[2];
      
      const int Attributes = 1;
      MPI_Datatype subtypes[Attributes] = {
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]))), &base);
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::Datatype );
      MPI_Type_commit( &ParticlePacked::Datatype );
      
   }
   {
      ParticlePacked dummyParticlePacked[2];
      
      const int Attributes = 14;
      MPI_Datatype subtypes[Attributes] = {
         MPI_DOUBLE,		 //centre
         MPI_DOUBLE,		 //velocity
         MPI_DOUBLE,		 //diameter
         MPI_DOUBLE,		 //influenceRadius
         MPI_DOUBLE,		 //epsilon
         MPI_DOUBLE,		 //mass
         MPI_DOUBLE,		 //hMin
         MPI_DOUBLE,		 //centreOfMass
         MPI_INT,		 //globalParticleNumber
         MPI_INT,		 //material
         MPI_DOUBLE,		 //angularVelocity
         MPI_INT,		 //numberOfTriangles
         MPI_INT,		 //vertices
         MPI_UB		 // end/displacement flag
      };
      
      int blocklen[Attributes] = {
         DIMENSIONS,		 //centre
         DIMENSIONS,		 //velocity
         1,		 //diameter
         1,		 //influenceRadius
         1,		 //epsilon
         1,		 //mass
         1,		 //hMin
         DIMENSIONS,		 //centreOfMass
         1,		 //globalParticleNumber
         1,		 //material
         DIMENSIONS,		 //angularVelocity
         1,		 //numberOfTriangles
         DIMENSIONS,		 //vertices
         1		 // end/displacement flag
      };
      
      MPI_Aint     disp[Attributes];
      
      MPI_Aint base;
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]))), &base);
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centre[0]))), 		&disp[0] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._velocity[0]))), 		&disp[1] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._diameter))), 		&disp[2] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._influenceRadius))), 		&disp[3] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._epsilon))), 		&disp[4] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._mass))), 		&disp[5] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._hMin))), 		&disp[6] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centreOfMass[0]))), 		&disp[7] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._globalParticleNumber))), 		&disp[8] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._material))), 		&disp[9] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._angularVelocity[0]))), 		&disp[10] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._numberOfTriangles))), 		&disp[11] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._vertices[0]))), 		&disp[12] );
      MPI_Address( const_cast<void*>(static_cast<const void*>(&dummyParticlePacked[1]._persistentRecords._centre[0])), 		&disp[13] );
      
      for (int i=1; i<Attributes; i++) {
         assertion1( disp[i] > disp[i-1], i );
      }
      for (int i=0; i<Attributes; i++) {
         disp[i] -= base;
      }
      MPI_Type_struct( Attributes, blocklen, disp, subtypes, &ParticlePacked::FullDatatype );
      MPI_Type_commit( &ParticlePacked::FullDatatype );
      
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




#endif


