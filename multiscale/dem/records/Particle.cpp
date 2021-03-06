#include "dem/records/Particle.h"

dem::records::Particle::PersistentRecords::PersistentRecords() {
   
}


dem::records::Particle::PersistentRecords::PersistentRecords(const tarch::la::Vector<6,double>& vertices, const tarch::la::Vector<6,double>& verticesA, const tarch::la::Vector<6,double>& verticesB, const tarch::la::Vector<6,double>& verticesC, const tarch::la::Vector<6,double>& verticesrefA, const tarch::la::Vector<6,double>& verticesrefB, const tarch::la::Vector<6,double>& verticesrefC, const tarch::la::Vector<9,double>& orientation, const tarch::la::Vector<9,double>& inertia, const tarch::la::Vector<9,double>& inverse, const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const tarch::la::Vector<DIMENSIONS,double>& referentialCentreOfMass, const tarch::la::Vector<DIMENSIONS,double>& velocity, const tarch::la::Vector<DIMENSIONS,double>& angular, const tarch::la::Vector<DIMENSIONS,double>& referentialAngular, const double& diameter, const double& haloDiameter, const double& epsilon, const double& mass, const double& hMin, const int& globalParticleId, const int& localParticleId, const int& numberOfTriangles, const int& material, const bool& isObstacle, const bool& friction):
_vertices(vertices),
_verticesA(verticesA),
_verticesB(verticesB),
_verticesC(verticesC),
_verticesrefA(verticesrefA),
_verticesrefB(verticesrefB),
_verticesrefC(verticesrefC),
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
_haloDiameter(haloDiameter),
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
_persistentRecords(persistentRecords._vertices, persistentRecords._verticesA, persistentRecords._verticesB, persistentRecords._verticesC, persistentRecords._verticesrefA, persistentRecords._verticesrefB, persistentRecords._verticesrefC, persistentRecords._orientation, persistentRecords._inertia, persistentRecords._inverse, persistentRecords._centre, persistentRecords._centreOfMass, persistentRecords._referentialCentreOfMass, persistentRecords._velocity, persistentRecords._angular, persistentRecords._referentialAngular, persistentRecords._diameter, persistentRecords._haloDiameter, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._globalParticleId, persistentRecords._localParticleId, persistentRecords._numberOfTriangles, persistentRecords._material, persistentRecords._isObstacle, persistentRecords._friction) {
   
}


dem::records::Particle::Particle(const tarch::la::Vector<6,double>& vertices, const tarch::la::Vector<6,double>& verticesA, const tarch::la::Vector<6,double>& verticesB, const tarch::la::Vector<6,double>& verticesC, const tarch::la::Vector<6,double>& verticesrefA, const tarch::la::Vector<6,double>& verticesrefB, const tarch::la::Vector<6,double>& verticesrefC, const tarch::la::Vector<9,double>& orientation, const tarch::la::Vector<9,double>& inertia, const tarch::la::Vector<9,double>& inverse, const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const tarch::la::Vector<DIMENSIONS,double>& referentialCentreOfMass, const tarch::la::Vector<DIMENSIONS,double>& velocity, const tarch::la::Vector<DIMENSIONS,double>& angular, const tarch::la::Vector<DIMENSIONS,double>& referentialAngular, const double& diameter, const double& haloDiameter, const double& epsilon, const double& mass, const double& hMin, const int& globalParticleId, const int& localParticleId, const int& numberOfTriangles, const int& material, const bool& isObstacle, const bool& friction):
_persistentRecords(vertices, verticesA, verticesB, verticesC, verticesrefA, verticesrefB, verticesrefC, orientation, inertia, inverse, centre, centreOfMass, referentialCentreOfMass, velocity, angular, referentialAngular, diameter, haloDiameter, epsilon, mass, hMin, globalParticleId, localParticleId, numberOfTriangles, material, isObstacle, friction) {
   
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
   out << "verticesA:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesA(i) << ",";
   }
   out << getVerticesA(6-1) << "]";
   out << ",";
   out << "verticesB:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesB(i) << ",";
   }
   out << getVerticesB(6-1) << "]";
   out << ",";
   out << "verticesC:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesC(i) << ",";
   }
   out << getVerticesC(6-1) << "]";
   out << ",";
   out << "verticesrefA:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesrefA(i) << ",";
   }
   out << getVerticesrefA(6-1) << "]";
   out << ",";
   out << "verticesrefB:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesrefB(i) << ",";
   }
   out << getVerticesrefB(6-1) << "]";
   out << ",";
   out << "verticesrefC:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesrefC(i) << ",";
   }
   out << getVerticesrefC(6-1) << "]";
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
   out << "haloDiameter:" << getHaloDiameter();
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
      getVerticesA(),
      getVerticesB(),
      getVerticesC(),
      getVerticesrefA(),
      getVerticesrefB(),
      getVerticesrefC(),
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
      getHaloDiameter(),
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
         const int Attributes = 27;
         #else
         const int Attributes = 28;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_DOUBLE		 //vertices
            , MPI_DOUBLE		 //verticesA
            , MPI_DOUBLE		 //verticesB
            , MPI_DOUBLE		 //verticesC
            , MPI_DOUBLE		 //verticesrefA
            , MPI_DOUBLE		 //verticesrefB
            , MPI_DOUBLE		 //verticesrefC
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
            , MPI_DOUBLE		 //haloDiameter
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
            , 6		 //verticesA
            , 6		 //verticesB
            , 6		 //verticesC
            , 6		 //verticesrefA
            , 6		 //verticesrefB
            , 6		 //verticesrefC
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
            , 1		 //haloDiameter
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
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesA[0]))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesA[0]))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesB[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesB[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesC[0]))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesC[0]))), 		&disp[3] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesrefA[0]))), 		&disp[4] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesrefA[0]))), 		&disp[4] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesrefB[0]))), 		&disp[5] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesrefB[0]))), 		&disp[5] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesrefC[0]))), 		&disp[6] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._verticesrefC[0]))), 		&disp[6] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._orientation[0]))), 		&disp[7] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._orientation[0]))), 		&disp[7] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._inertia[0]))), 		&disp[8] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._inertia[0]))), 		&disp[8] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._inverse[0]))), 		&disp[9] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._inverse[0]))), 		&disp[9] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centre[0]))), 		&disp[10] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centre[0]))), 		&disp[10] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centreOfMass[0]))), 		&disp[11] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._centreOfMass[0]))), 		&disp[11] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._referentialCentreOfMass[0]))), 		&disp[12] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._referentialCentreOfMass[0]))), 		&disp[12] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._velocity[0]))), 		&disp[13] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._velocity[0]))), 		&disp[13] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._angular[0]))), 		&disp[14] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._angular[0]))), 		&disp[14] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._referentialAngular[0]))), 		&disp[15] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._referentialAngular[0]))), 		&disp[15] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._diameter))), 		&disp[16] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._diameter))), 		&disp[16] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._haloDiameter))), 		&disp[17] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._haloDiameter))), 		&disp[17] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._epsilon))), 		&disp[18] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._epsilon))), 		&disp[18] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._mass))), 		&disp[19] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._mass))), 		&disp[19] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._hMin))), 		&disp[20] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._hMin))), 		&disp[20] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._globalParticleId))), 		&disp[21] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._globalParticleId))), 		&disp[21] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._localParticleId))), 		&disp[22] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._localParticleId))), 		&disp[22] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._numberOfTriangles))), 		&disp[23] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._numberOfTriangles))), 		&disp[23] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._material))), 		&disp[24] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._material))), 		&disp[24] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._isObstacle))), 		&disp[25] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._isObstacle))), 		&disp[25] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._friction))), 		&disp[26] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[0]._persistentRecords._friction))), 		&disp[26] );
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
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticle[1]))), 		&disp[27] );
         disp[27] -= base;
         disp[27] += disp[0];
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
   
   void dem::records::Particle::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
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
         msg << "was not able to send message dem::records::Particle "  
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
           msg << "testing for finished send task for dem::records::Particle " 
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
   
   
   
   void dem::records::Particle::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive dem::records::Particle from node " 
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
        msg << "failed to start to receive dem::records::Particle from node " 
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
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for dem::records::Particle failed: " 
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
        msg << "testing for finished receive task for dem::records::Particle failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
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
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for dem::records::Particle failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive dem::records::Particle from node " 
            << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
    }
    break; 
  } 
// =========================== 
// end injected snippet/aspect 
// =========================== 

      
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


dem::records::ParticlePacked::PersistentRecords::PersistentRecords(const tarch::la::Vector<6,double>& vertices, const tarch::la::Vector<6,double>& verticesA, const tarch::la::Vector<6,double>& verticesB, const tarch::la::Vector<6,double>& verticesC, const tarch::la::Vector<6,double>& verticesrefA, const tarch::la::Vector<6,double>& verticesrefB, const tarch::la::Vector<6,double>& verticesrefC, const tarch::la::Vector<9,double>& orientation, const tarch::la::Vector<9,double>& inertia, const tarch::la::Vector<9,double>& inverse, const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const tarch::la::Vector<DIMENSIONS,double>& referentialCentreOfMass, const tarch::la::Vector<DIMENSIONS,double>& velocity, const tarch::la::Vector<DIMENSIONS,double>& angular, const tarch::la::Vector<DIMENSIONS,double>& referentialAngular, const double& diameter, const double& haloDiameter, const double& epsilon, const double& mass, const double& hMin, const int& globalParticleId, const int& localParticleId, const int& numberOfTriangles, const int& material, const bool& isObstacle, const bool& friction):
_vertices(vertices),
_verticesA(verticesA),
_verticesB(verticesB),
_verticesC(verticesC),
_verticesrefA(verticesrefA),
_verticesrefB(verticesrefB),
_verticesrefC(verticesrefC),
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
_haloDiameter(haloDiameter),
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
_persistentRecords(persistentRecords._vertices, persistentRecords._verticesA, persistentRecords._verticesB, persistentRecords._verticesC, persistentRecords._verticesrefA, persistentRecords._verticesrefB, persistentRecords._verticesrefC, persistentRecords._orientation, persistentRecords._inertia, persistentRecords._inverse, persistentRecords._centre, persistentRecords._centreOfMass, persistentRecords._referentialCentreOfMass, persistentRecords._velocity, persistentRecords._angular, persistentRecords._referentialAngular, persistentRecords._diameter, persistentRecords._haloDiameter, persistentRecords._epsilon, persistentRecords._mass, persistentRecords._hMin, persistentRecords._globalParticleId, persistentRecords._localParticleId, persistentRecords._numberOfTriangles, persistentRecords._material, persistentRecords._isObstacle, persistentRecords._friction) {
   
}


dem::records::ParticlePacked::ParticlePacked(const tarch::la::Vector<6,double>& vertices, const tarch::la::Vector<6,double>& verticesA, const tarch::la::Vector<6,double>& verticesB, const tarch::la::Vector<6,double>& verticesC, const tarch::la::Vector<6,double>& verticesrefA, const tarch::la::Vector<6,double>& verticesrefB, const tarch::la::Vector<6,double>& verticesrefC, const tarch::la::Vector<9,double>& orientation, const tarch::la::Vector<9,double>& inertia, const tarch::la::Vector<9,double>& inverse, const tarch::la::Vector<DIMENSIONS,double>& centre, const tarch::la::Vector<DIMENSIONS,double>& centreOfMass, const tarch::la::Vector<DIMENSIONS,double>& referentialCentreOfMass, const tarch::la::Vector<DIMENSIONS,double>& velocity, const tarch::la::Vector<DIMENSIONS,double>& angular, const tarch::la::Vector<DIMENSIONS,double>& referentialAngular, const double& diameter, const double& haloDiameter, const double& epsilon, const double& mass, const double& hMin, const int& globalParticleId, const int& localParticleId, const int& numberOfTriangles, const int& material, const bool& isObstacle, const bool& friction):
_persistentRecords(vertices, verticesA, verticesB, verticesC, verticesrefA, verticesrefB, verticesrefC, orientation, inertia, inverse, centre, centreOfMass, referentialCentreOfMass, velocity, angular, referentialAngular, diameter, haloDiameter, epsilon, mass, hMin, globalParticleId, localParticleId, numberOfTriangles, material, isObstacle, friction) {
   
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
   out << "verticesA:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesA(i) << ",";
   }
   out << getVerticesA(6-1) << "]";
   out << ",";
   out << "verticesB:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesB(i) << ",";
   }
   out << getVerticesB(6-1) << "]";
   out << ",";
   out << "verticesC:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesC(i) << ",";
   }
   out << getVerticesC(6-1) << "]";
   out << ",";
   out << "verticesrefA:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesrefA(i) << ",";
   }
   out << getVerticesrefA(6-1) << "]";
   out << ",";
   out << "verticesrefB:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesrefB(i) << ",";
   }
   out << getVerticesrefB(6-1) << "]";
   out << ",";
   out << "verticesrefC:[";
   for (int i = 0; i < 6-1; i++) {
      out << getVerticesrefC(i) << ",";
   }
   out << getVerticesrefC(6-1) << "]";
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
   out << "haloDiameter:" << getHaloDiameter();
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
      getVerticesA(),
      getVerticesB(),
      getVerticesC(),
      getVerticesrefA(),
      getVerticesrefB(),
      getVerticesrefC(),
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
      getHaloDiameter(),
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
         const int Attributes = 27;
         #else
         const int Attributes = 28;
         #endif
         MPI_Datatype subtypes[Attributes] = {
              MPI_DOUBLE		 //vertices
            , MPI_DOUBLE		 //verticesA
            , MPI_DOUBLE		 //verticesB
            , MPI_DOUBLE		 //verticesC
            , MPI_DOUBLE		 //verticesrefA
            , MPI_DOUBLE		 //verticesrefB
            , MPI_DOUBLE		 //verticesrefC
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
            , MPI_DOUBLE		 //haloDiameter
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
            , 6		 //verticesA
            , 6		 //verticesB
            , 6		 //verticesC
            , 6		 //verticesrefA
            , 6		 //verticesrefB
            , 6		 //verticesrefC
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
            , 1		 //haloDiameter
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
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesA[0]))), 		&disp[1] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesA[0]))), 		&disp[1] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesB[0]))), 		&disp[2] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesB[0]))), 		&disp[2] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesC[0]))), 		&disp[3] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesC[0]))), 		&disp[3] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesrefA[0]))), 		&disp[4] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesrefA[0]))), 		&disp[4] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesrefB[0]))), 		&disp[5] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesrefB[0]))), 		&disp[5] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesrefC[0]))), 		&disp[6] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._verticesrefC[0]))), 		&disp[6] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._orientation[0]))), 		&disp[7] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._orientation[0]))), 		&disp[7] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._inertia[0]))), 		&disp[8] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._inertia[0]))), 		&disp[8] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._inverse[0]))), 		&disp[9] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._inverse[0]))), 		&disp[9] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centre[0]))), 		&disp[10] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centre[0]))), 		&disp[10] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centreOfMass[0]))), 		&disp[11] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._centreOfMass[0]))), 		&disp[11] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._referentialCentreOfMass[0]))), 		&disp[12] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._referentialCentreOfMass[0]))), 		&disp[12] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._velocity[0]))), 		&disp[13] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._velocity[0]))), 		&disp[13] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._angular[0]))), 		&disp[14] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._angular[0]))), 		&disp[14] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._referentialAngular[0]))), 		&disp[15] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._referentialAngular[0]))), 		&disp[15] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._diameter))), 		&disp[16] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._diameter))), 		&disp[16] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._haloDiameter))), 		&disp[17] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._haloDiameter))), 		&disp[17] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._epsilon))), 		&disp[18] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._epsilon))), 		&disp[18] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._mass))), 		&disp[19] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._mass))), 		&disp[19] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._hMin))), 		&disp[20] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._hMin))), 		&disp[20] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._globalParticleId))), 		&disp[21] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._globalParticleId))), 		&disp[21] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._localParticleId))), 		&disp[22] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._localParticleId))), 		&disp[22] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._numberOfTriangles))), 		&disp[23] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._numberOfTriangles))), 		&disp[23] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._material))), 		&disp[24] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._material))), 		&disp[24] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._isObstacle))), 		&disp[25] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._isObstacle))), 		&disp[25] );
         #endif
         #ifdef MPI2
         MPI_Get_address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._friction))), 		&disp[26] );
         #else
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[0]._persistentRecords._friction))), 		&disp[26] );
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
         MPI_Address( const_cast<void*>(static_cast<const void*>(&(dummyParticlePacked[1]))), 		&disp[27] );
         disp[27] -= base;
         disp[27] += disp[0];
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
   
   void dem::records::ParticlePacked::send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
      // ============================= 
// start injected snippet/aspect 
// ============================= 
switch (mode) { 
  case ExchangeMode::Blocking: 
    {
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
         msg << "was not able to send message dem::records::ParticlePacked "  
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
           msg << "testing for finished send task for dem::records::ParticlePacked " 
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
   
   
   
   void dem::records::ParticlePacked::receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise, ExchangeMode mode) {
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
        msg << "failed to start to receive dem::records::ParticlePacked from node " 
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
        msg << "failed to start to receive dem::records::ParticlePacked from node " 
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
        result = MPI_Test( sendRequestHandle, &flag, source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
        if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for dem::records::ParticlePacked failed: " 
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
        msg << "testing for finished receive task for dem::records::ParticlePacked failed: " 
            << tarch::parallel::MPIReturnValueToString(result); 
        _log.error("receive(int)", msg.str() ); 
      } 
      while (!flag) { 
        if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp(); 
        if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp(); 
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
        result = MPI_Iprobe(source, tag, tarch::parallel::Node::getInstance().getCommunicator(), &flag, MPI_STATUS_IGNORE ); 
         if (result!=MPI_SUCCESS) { 
          std::ostringstream msg; 
          msg << "testing for finished receive task for dem::records::ParticlePacked failed: " 
              << tarch::parallel::MPIReturnValueToString(result); 
          _log.error("receive(int)", msg.str() ); 
        } 
      } 
      result = MPI_Recv(this, 1, exchangeOnlyAttributesMarkedWithParallelise ? Datatype : FullDatatype, source, tag, tarch::parallel::Node::getInstance().getCommunicator(), source==MPI_ANY_SOURCE ? &status : MPI_STATUS_IGNORE ); 
      if ( result != MPI_SUCCESS ) { 
        std::ostringstream msg; 
        msg << "failed to start to receive dem::records::ParticlePacked from node " 
            << source << ": " << tarch::parallel::MPIReturnValueToString(result); 
        _log.error( "receive(int)", msg.str() ); 
      } 
    }
    break; 
  } 
// =========================== 
// end injected snippet/aspect 
// =========================== 

      
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



