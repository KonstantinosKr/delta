#include "delta/contact/forces/forces.h"
#include "delta/dynamics/dynamics.h"

#include "dem/mappings/Collision.h"

#include <delta/contact/detection/bf.h>
#include <delta/contact/filter.h>
#include <delta/contact/detection/gjk.h>
#include <delta/contact/detection/hybrid.h>
#include <delta/contact/detection/penalty.h>
#include <delta/contact/detection/sphere.h>

#include "dem/mappings/MoveParticles.h"

#include "peano/utils/Loop.h"
#include "peano/datatraversal/TaskSet.h"

#include "tarch/multicore/Jobs.h"
#include "tarch/multicore/BooleanSemaphore.h"

#include <unordered_map>


peano::CommunicationSpecification   dem::mappings::Collision::communicationSpecification() const {
	return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

peano::MappingSpecification   dem::mappings::Collision::touchVertexFirstTimeSpecification(int level) const {
	return peano::MappingSpecification(
	  peano::MappingSpecification::WholeTree,
    dem::mappings::Collision::RunGridTraversalInParallel ?
	    peano::MappingSpecification::RunConcurrentlyOnFineGrid :
      peano::MappingSpecification::Serial,
	  true);
}

peano::MappingSpecification   dem::mappings::Collision::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification(
    peano::MappingSpecification::WholeTree,
    dem::mappings::Collision::RunGridTraversalInParallel ?
      peano::MappingSpecification::RunConcurrentlyOnFineGrid :
      peano::MappingSpecification::Serial,
    true
  );
}

peano::MappingSpecification   dem::mappings::Collision::enterCellSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::Collision::leaveCellSpecification(int level) const {
	return peano::MappingSpecification(
	  peano::MappingSpecification::WholeTree,
    dem::mappings::Collision::RunGridTraversalInParallel ?
      peano::MappingSpecification::RunConcurrentlyOnFineGrid:
      peano::MappingSpecification::Serial,
  	true
  );
}

peano::MappingSpecification   dem::mappings::Collision::ascendSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::Collision::descendSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                                                 dem::mappings::Collision::_log( "dem::mappings::Collision" );
std::map<int, std::vector<dem::mappings::Collision::Collisions> >   dem::mappings::Collision::_activeCollisions;
std::map<int, std::vector<dem::mappings::Collision::Collisions> >   dem::mappings::Collision::_collisionsOfNextTraversal;
dem::mappings::Collision::CollisionModel                            dem::mappings::Collision::_collisionModel;
bool																   dem::mappings::Collision::_enableOverlapCheck;
tarch::multicore::BooleanSemaphore                                  dem::mappings::Collision::_collisionSemaphore;
iREAL                                                              dem::mappings::Collision::gravity = 0.0;

bool dem::mappings::Collision::RunGridTraversalInParallel           = true;
bool dem::mappings::Collision::RunParticleLoopInParallel            = true;
bool dem::mappings::Collision::RunParticleComparisionsInBackground  = false;
dem::State dem::mappings::Collision::_backgroundTaskState;

void dem::mappings::Collision::addCollision(
    std::vector<delta::contact::contactpoint> & newContactPoints,
    const records::Particle&                    particleA,
    const records::Particle&                    particleB,
    bool sphere
) {
  assertion( !newContactPoints.empty() );

  //////////////START initial insertion of collision vectors into _collisionsOfNextTraversal<id, collision> map for next move update of particle A and B
  if( _collisionsOfNextTraversal.count(particleA.getGlobalParticleId())==0 ) {
    _collisionsOfNextTraversal.insert(std::pair<int,std::vector<Collisions>>(particleA.getGlobalParticleId(), std::vector<Collisions>()));
  }

  if( _collisionsOfNextTraversal.count(particleB.getGlobalParticleId())==0 ) {
    _collisionsOfNextTraversal.insert(std::pair<int,std::vector<Collisions>>(particleB.getGlobalParticleId(), std::vector<Collisions>()));
  }
  ///////////////END

  //START DATASET pointer
  Collisions* dataSetA = nullptr;
  Collisions* dataSetB = nullptr;
  //////////////END

  /////////////////START if already exist | find and assign reference collision list to dataA or dataB particle
  for (std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleA.getGlobalParticleId()].begin();
     p!=_collisionsOfNextTraversal[particleA.getGlobalParticleId()].end();  p++)
  {
    if(p->_copyOfPartnerParticle.getGlobalParticleId()==particleB.getGlobalParticleId())
    {
      dataSetA = &(*p);
    }
  }

  for(std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleB.getGlobalParticleId()].begin();
     p!=_collisionsOfNextTraversal[particleB.getGlobalParticleId()].end(); p++)
  {
    if(p->_copyOfPartnerParticle.getGlobalParticleId()==particleA.getGlobalParticleId())
    {
      dataSetB = &(*p);
    }
  }
  //////////////END

  if((dataSetA!=nullptr && dataSetB!=nullptr)) {
    newContactPoints.clear();
    return;
  }

  ///ASSERT we have data assigned to both data pointers
  assertion( (dataSetA==nullptr && dataSetB==nullptr) || (dataSetA!=nullptr && dataSetB!=nullptr) );
  //END

  //START if dataset A and B is empty
  if(dataSetA==nullptr)
  {
    //START push_back collisions object into corresponding both A and B particle index collision list
    _collisionsOfNextTraversal[particleA.getGlobalParticleId()].push_back( Collisions() );
    _collisionsOfNextTraversal[particleB.getGlobalParticleId()].push_back( Collisions() );
    //END push_back

    //START reference of vector to data A and B ready to used
    dataSetA = &(_collisionsOfNextTraversal[particleA.getGlobalParticleId()].back());
    dataSetB = &(_collisionsOfNextTraversal[particleB.getGlobalParticleId()].back());
    //END

    //START add copy of master and slave particles to sets (dual contact reference)
    dataSetA->_copyOfPartnerParticle = particleB;
    dataSetB->_copyOfPartnerParticle = particleA;
    //END
  }
  ////////END

  //delta::collision::filterNewContacts(newContactPoints);
  if(sphere)
  {
    //delta::collision::filterOldContacts(dataSetA->_contactPoints, newContactPoints);
    //delta::collision::filterOldContacts(dataSetB->_contactPoints, newContactPoints);
  } else {  //filter multiple contacts for same area of mesh
    //delta::collision::filterOldContacts(dataSetA->_contactPoints, newContactPoints, std::min(particleA.getHMin(), particleB.getHMin()));
    //delta::collision::filterOldContacts(dataSetB->_contactPoints, newContactPoints, std::min(particleA.getHMin(), particleB.getHMin()));
  }

  /*
   * Problem here was:
   * Although all normals were pointing to opposite direction for each particle due to how we loop particles
   * A. at each vertex or grid initial contact direction is interchangeable depending on what is A and B
   * B. we always want to have the normal for particles to the position update direction (inner-direction)
   * C. we always want to ensure that normal of particle point away from obstacle
   * D. we don't care about normal of obstacle.
   */
  dataSetA->_contactPoints.insert(dataSetA->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end());

  for (std::vector<delta::contact::contactpoint>::iterator p = newContactPoints.begin(); p != newContactPoints.end(); p++)
  {
    //invert normal for particle B
    p->normal[0] = -1.0 * p->normal[0];
    p->normal[1] = -1.0 * p->normal[1];
    p->normal[2] = -1.0 * p->normal[2];
  }

  dataSetB->_contactPoints.insert(dataSetB->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end());
}

bool dem::mappings::Collision::triggerParticleTooClose(
    const records::Particle& particleA,
    const records::Particle& particleB,
    State& state)
{
  //printf("Particle A:%d Particle B:%d\n", particleA.getGlobalParticleId(), particleB.getGlobalParticleId());
  iREAL increments = state.getStepIncrement();
  //printf("increment:%f\n", increments);
  iREAL dt = state.getTimeStepSize(); //current iteration/step
  iREAL pdt = dt * increments; //next iteration/step
  if(pdt >= state.getMaxDt()) pdt = state.getMaxDt(); //this maxDt is only valid for current iteration, so this is kind of wrong, we need to predict next iterations maxDt

  iREAL cA[3], cB[3];

  cA[0] = particleA.getCentre(0) + pdt*particleA.getVelocity(0);
  cA[1] = particleA.getCentre(1) + pdt*particleA.getVelocity(1);
  cA[2] = particleA.getCentre(2) + pdt*particleA.getVelocity(2);

  cB[0] = particleB.getCentre(0) + pdt*particleB.getVelocity(0);
  cB[1] = particleB.getCentre(1) + pdt*particleB.getVelocity(1);
  cB[2] = particleB.getCentre(2) + pdt*particleB.getVelocity(2);

  iREAL dAB[3], pdAB[3];

  dAB[0] = particleB.getCentre(0) - particleA.getCentre(0);
  dAB[1] = particleB.getCentre(1) - particleA.getCentre(1);
  dAB[2] = particleB.getCentre(2) - particleA.getCentre(2);

  pdAB[0] = cB[0] - cA[0];
  pdAB[1] = cB[1] - cA[1];
  pdAB[2] = cB[2] - cA[2];
/*
  printf("current position A: %f %f %f\n", particleA.getCentre(0), particleA.getCentre(1), particleA.getCentre(2));
  printf("current position B: %f %f %f\n", particleB.getCentre(0), particleB.getCentre(1), particleB.getCentre(2));

  printf("predicted position A: %f %f %f\n", cA[0], cA[1], cA[2]);
  printf("predicted position B: %f %f %f\n", cB[0], cB[1], cB[2]);*/

  iREAL vA[3], vB[3];
  vA[0] = particleA.getVelocity(0);
  vA[1] = particleA.getVelocity(1);
  vA[2] = particleA.getVelocity(2);

  vB[0] = particleB.getVelocity(0);
  vB[1] = particleB.getVelocity(1);
  vB[2] = particleB.getVelocity(2);

  iREAL d = sqrt((dAB[0] * dAB[0]) + (dAB[1] * dAB[1]) + (dAB[2] * dAB[2]));
  iREAL pd = sqrt((pdAB[0] * pdAB[0]) + (pdAB[1] * pdAB[1]) + (pdAB[2] * pdAB[2]));

  //velocity of B relative to A
  iREAL vBA = ((vB[0] * dAB[0]) + (vB[1] * dAB[1]) + (vB[2] * dAB[2])) -
              ((vA[0] * dAB[0]) + (vA[1] * dAB[1]) + (vA[2] * dAB[2]));

  //predicted velocity of B relative to A
  iREAL pvBA = ((vB[0] * pdAB[0]) + (vB[1] * pdAB[1]) + (vB[2] * pdAB[2])) -
               ((vA[0] * pdAB[0]) + (vA[1] * pdAB[1]) + (vA[2] * pdAB[2]));

  //if velocity of B relative to A is greater than zero and predicted relative velocity of B to A is greater than zero particles separate
  if (vBA > 0 && pvBA > 0) { //pvBA > 0 is redundant
    //printf("separation: %f\n", vBA);

    state.informStateThatTwoParticlesAreSeparate();
    return false;
  }

  //particles approach so save relative velocity B on A
  if(-vBA > state.getMaximumVelocityApproach())
  {
    state.setMaximumVelocityApproach(vBA);
  }

  iREAL rA = particleA.getHaloDiameter()/2.0;
  iREAL rB = particleB.getHaloDiameter()/2.0;

  iREAL currentMinHaloDistance = (d - rA - rB);
  iREAL predictedMinHaloDistance = (pd - rA - rB); //two steps after (current, after currect)

  //printf("halo distance : %f pdt: %f\n", predictedMinHaloDistance, pdt);

  iREAL distancePerStep = currentMinHaloDistance/dt;
  iREAL pdistancePerStep = predictedMinHaloDistance/pdt;

  if(-pvBA  >= pdistancePerStep)
  {
    //printf("entered TRIGER %f >= %f\n", -pvBA, pdistancePerStep);
    iREAL rrA = particleA.getDiameter()/2.0;
    iREAL rrB = particleB.getDiameter()/2.0;

    iREAL epsilonA = particleA.getEpsilon();
    iREAL epsilonB = particleB.getEpsilon();

    iREAL remainingDistance = (d -rrA -rrB -epsilonA -epsilonB);
    if(remainingDistance < 0.0)
    {
      remainingDistance = remainingDistance * -1.1;
    }
    iREAL localMaxdt = remainingDistance * 0.82;

    if(localMaxdt < 0.0) localMaxdt = -1*localMaxdt;

    //printf("localMax:%f d:%f pd:%f min distance:%f vBA%f pvBA%f \n", localMaxdt, d, pd, remainingDistance, -vBA, -pvBA);

    state.informStateThatTwoParticlesAreClose(localMaxdt);
  }

  /*  printf("pvBA: %f  pdt: %f | pmd: %f pdt: %f pd: %f\n",
         pvBA,    pdistancePerStep, distancePerStep, pdt, pd);
  printf(" vBA: %f  ddt: %f |  md: %f dt: %f  d: %f\n",
         vBA,     distancePerStep,  pdistancePerStep, dt, d);*/
  return true;
}

void dem::mappings::Collision::collisionDetection(
  dem::records::Particle   particleA,
  int                      numberOfTrianglesA,
  const iREAL*            xCoordinatesA,
  const iREAL*            yCoordinatesA,
  const iREAL*            zCoordinatesA,
  dem::records::Particle   particleB,
  int                      numberOfTrianglesB,
  const iREAL*            xCoordinatesB,
  const iREAL*            yCoordinatesB,
  const iREAL*            zCoordinatesB,
  State* state,
  bool   protectStateAccess)
{
  if(_enableOverlapCheck)
  {
    bool overlap = delta::contact::detection::isSphereOverlayInContact(
      particleA.getCentre(0),
      particleA.getCentre(1),
      particleA.getCentre(2),
      particleA.getHaloDiameter(),

      particleB.getCentre(0),
      particleB.getCentre(1),
      particleB.getCentre(2),
      particleB.getHaloDiameter());

    if (!overlap) return;
  }

  tarch::multicore::Lock lock(_collisionSemaphore,false);

  if (protectStateAccess) lock.lock();
  bool approach = triggerParticleTooClose(particleA, particleB, *state);
  if (protectStateAccess) lock.free();

  //if(!approach) return;

  std::vector<delta::contact::contactpoint> newContactPoints;

  switch (_collisionModel) {
    case CollisionModel::Sphere:
      if(
        particleA.getIsObstacle() &&
        !particleB.getIsObstacle()
      ) {
        newContactPoints = delta::contact::detection::sphereWithBarrierBA(
							  particleB.getCentre(0),
							  particleB.getCentre(1),
							  particleB.getCentre(2),
							  particleB.getDiameter(),
							  particleB.getEpsilon(),
							  particleB.getFriction(),
							  particleB.getGlobalParticleId(),

							  xCoordinatesA,
							  yCoordinatesA,
							  zCoordinatesA,
							  numberOfTrianglesA,
							  particleA.getEpsilon(),
							  particleA.getFriction(),
							  particleA.getGlobalParticleId());
      } else if (
        !particleA.getIsObstacle() &&
         particleB.getIsObstacle()
      ) {
        newContactPoints = delta::contact::detection::sphereWithBarrierAB(
							  particleA.getCentre(0),
							  particleA.getCentre(1),
							  particleA.getCentre(2),
							  particleA.getDiameter(),
							  particleA.getEpsilon(),
							  particleA.getFriction(),
							  particleA.getGlobalParticleId(),

							  xCoordinatesB,
							  yCoordinatesB,
							  zCoordinatesB,
							  numberOfTrianglesB,
							  particleB.getEpsilon(),
							  particleB.getFriction(),
							  particleB.getGlobalParticleId());
      }
      else {
        newContactPoints = delta::contact::detection::sphere(
							  particleA.getCentre(0),
							  particleA.getCentre(1),
							  particleA.getCentre(2),
							  particleA.getDiameter(),
							  particleA.getEpsilon(),
							  particleA.getFriction(),
							  particleA.getGlobalParticleId(),

							  particleB.getCentre(0),
							  particleB.getCentre(1),
							  particleB.getCentre(2),
							  particleB.getDiameter(),
							  particleB.getEpsilon(),
							  particleB.getFriction(),
							  particleB.getGlobalParticleId());
      }
      break;
    case CollisionModel::BruteForce:

      newContactPoints = delta::contact::detection::bf(
							xCoordinatesA,
							yCoordinatesA,
							zCoordinatesA,
							numberOfTrianglesA,
							particleA.getEpsilon(),
							particleA.getFriction(),
							particleA.getGlobalParticleId(),

							xCoordinatesB,
							yCoordinatesB,
							zCoordinatesB,
							numberOfTrianglesB,
							particleB.getEpsilon(),
							particleB.getFriction(),
							particleB.getGlobalParticleId(),
							_collisionSemaphore);

      break;
    case CollisionModel::Penalty:

      newContactPoints = delta::contact::detection::penalty(
							xCoordinatesA,
							yCoordinatesA,
							zCoordinatesA,
							numberOfTrianglesA,
							particleA.getEpsilon(),
							particleA.getFriction(),
							particleA.getGlobalParticleId(),

							xCoordinatesB,
							yCoordinatesB,
							zCoordinatesB,
							numberOfTrianglesB,
							particleB.getEpsilon(),
							particleB.getFriction(),
							particleB.getGlobalParticleId(),
							_collisionSemaphore);

      break;
    case CollisionModel::PenaltyStat:

      newContactPoints = delta::contact::detection::penaltyStat(
							xCoordinatesA,
							yCoordinatesA,
							zCoordinatesA,
							numberOfTrianglesA,
							particleA.getEpsilon(),
							particleA.getFriction(),
							particleA.getGlobalParticleId(),

							xCoordinatesB,
							yCoordinatesB,
							zCoordinatesB,
							numberOfTrianglesB,
							particleB.getEpsilon(),
							particleB.getFriction(),
							particleB.getGlobalParticleId());

      break;
    case CollisionModel::PenaltyTune:

      delta::contact::detection::penaltyStat(
		  xCoordinatesA,
		  yCoordinatesA,
		  zCoordinatesA,
		  numberOfTrianglesB,
		  particleA.getEpsilon(),
		  particleA.getFriction(),
		  particleA.getGlobalParticleId(),

		  xCoordinatesB,
		  yCoordinatesB,
		  zCoordinatesB,
		  numberOfTrianglesB,
		  particleB.getEpsilon(),
		  particleB.getFriction(),
		  particleB.getGlobalParticleId());

      break;
    case CollisionModel::HybridOnBatches:

      newContactPoints = delta::contact::detection::hybridWithPerBatchFallBack(
							xCoordinatesA,
							yCoordinatesA,
							zCoordinatesA,
							numberOfTrianglesA,
							particleA.getEpsilon(),
							particleA.getFriction(),
							particleA.getGlobalParticleId(),

							xCoordinatesB,
							yCoordinatesB,
							zCoordinatesB,
							numberOfTrianglesB,
							particleB.getEpsilon(),
							particleB.getFriction(),
							particleB.getGlobalParticleId(),
							_collisionSemaphore);

      break;
    case CollisionModel::HybridOnTrianglePairs:

      newContactPoints = delta::contact::detection::hybridWithPerTriangleFallBack(
							xCoordinatesA,
							yCoordinatesA,
							zCoordinatesA,
							numberOfTrianglesA,
							particleA.getEpsilon(),
							particleA.getFriction(),
							particleA.getGlobalParticleId(),

							xCoordinatesB,
							yCoordinatesB,
							zCoordinatesB,
							numberOfTrianglesB,
							particleB.getEpsilon(),
							particleB.getFriction(),
							particleB.getGlobalParticleId(),
							_collisionSemaphore);

      break;
    case CollisionModel::HybridStat:

      newContactPoints = delta::contact::detection::hybridTriangleStat(
							xCoordinatesA,
							yCoordinatesA,
							zCoordinatesA,
							numberOfTrianglesA,
							particleA.getEpsilon(),
							particleA.getFriction(),
							particleA.getGlobalParticleId(),

							xCoordinatesB,
							yCoordinatesB,
							zCoordinatesB,
							numberOfTrianglesB,
							particleB.getEpsilon(),
							particleB.getFriction(),
							particleB.getGlobalParticleId());

      break;
    case CollisionModel::GJK:
      assertionMsg(false,"Konstantinos, bf has to use const iREAL* and not iREAL* as input" );
  /*		newContactPoints = delta::collision::gjk(
							numberOfTrianglesA,
							xCoordinatesA,
							yCoordinatesA,
							zCoordinatesA,
							particleA.getEpsilon(),
							particleA.getFriction(),
							particleA.getGlobalParticleId(),

							numberOfTrianglesB,
							xCoordinatesB,
							yCoordinatesB,
							zCoordinatesB,
							particleB.getEpsilon(),
							particleB.getFriction(),
							particleB.getGlobalParticleId());*/
      break;
    case CollisionModel::none:
      break;
  }

  if(!newContactPoints.empty()) {
    lock.lock();
    addCollision(newContactPoints, particleA, particleB, dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere);
    state->incNumberOfContactPoints(newContactPoints.size());
    lock.free();
  }

  if (protectStateAccess) lock.lock();
  state->incNumberOfTriangleComparisons(numberOfTrianglesA * numberOfTrianglesB);
  state->incNumberOfParticleComparisons(1);
  if (protectStateAccess) lock.free();
}

void dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
  dem::Vertex&  vertexA,
  dem::Vertex&  vertexB,
  State& state,
  State& backgroundstate
) {
  logDebug( "collideParticlesOfTwoDifferentVertices(...)", vertexA.toString() << ", " << vertexA.getNumberOfRealAndVirtualParticles() );
  logDebug( "collideParticlesOfTwoDifferentVertices(...)", vertexB.toString() << ", " << vertexB.getNumberOfRealAndVirtualParticles() );

  #ifdef SharedTBB
  // Take care: grain size has to be possitive even if loop degenerates
  const int grainSize = (RunParticleLoopInParallel || vertexA.getNumberOfParticles()==0) ? 1 : vertexA.getNumberOfParticles();
  tbb::parallel_for(
   tbb::blocked_range<int>(0,vertexA.getNumberOfParticles(),grainSize),
   [&](const tbb::blocked_range<int>& r) {
    for (int i=r.begin(); i!=r.end(); i++) {
  #else
  for(int i=0; i<vertexA.getNumberOfParticles(); i++) {
  #endif
    for(int j=0; j<vertexB.getNumberOfParticles(); j++)
    {
      if((vertexA.getParticle(i).getIsObstacle() && vertexB.getParticle(j).getIsObstacle()) ||
         (vertexA.getParticle(i).getGlobalParticleId() == vertexB.getParticle(j).getGlobalParticleId()))
        continue;

      if (RunParticleComparisionsInBackground) {
        auto p0 = vertexA.getParticle(i);
        auto p1 = vertexA.getNumberOfTriangles(i);
        auto p2 = vertexA.getXCoordinates(i);
        auto p3 = vertexA.getYCoordinates(i);
        auto p4 = vertexA.getZCoordinates(i);
        auto p5 = vertexB.getParticle(j);
        auto p6 = vertexB.getNumberOfTriangles(j);
        auto p7 = vertexB.getXCoordinates(j);
        auto p8 = vertexB.getYCoordinates(j);
        auto p9 = vertexB.getZCoordinates(j);

        peano::datatraversal::TaskSet backgroundTask(
         [=, &backgroundstate] ()->bool {
          dem::mappings::Collision::collisionDetection(
            p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,
            &backgroundstate,
            true
          );
          return false;
         },
         peano::datatraversal::TaskSet::TaskType::Background
        );
        logDebug( "collideParticlesOfTwoDifferentVertices(...)", "spawn background task" );
      }
      else {
        collisionDetection(
          vertexA.getParticle(i),
          vertexA.getNumberOfTriangles(i),
          vertexA.getXCoordinates(i),
          vertexA.getYCoordinates(i),
          vertexA.getZCoordinates(i),
          vertexB.getParticle(j),
          vertexB.getNumberOfTriangles(j),
          vertexB.getXCoordinates(j),
          vertexB.getYCoordinates(j),
          vertexB.getZCoordinates(j),
          &state,
          false
        );
      }
    }
  #ifdef SharedTBB
    }}
  );
  #else
  }
  #endif
}

dem::Vertex dem::mappings::reduceVirtuals(
    dem::Vertex &v0,
    dem::Vertex &v1,
    dem::Vertex &v2,
    dem::Vertex &v3,
    dem::Vertex &v4,
    dem::Vertex &v5,
    dem::Vertex &v6,
    dem::Vertex &v7)
{
  std::unordered_map<int, int> virtualGlobalIDCount;

  dem::Vertex vcentre;
  vcentre.init();

  //if all vertices have same virtual particle, then remove from all.
  //*already check in intra-vertex stage

  for(int i=0; i<v0.getNumberOfVirtualParticles(); i++)
  {
    int virtualGlobalParticleID = v0.getParticle(i).getGlobalParticleId();

    if(virtualGlobalIDCount[virtualGlobalParticleID] != -1)
    {
      virtualGlobalIDCount[virtualGlobalParticleID]++;
    }

    if(virtualGlobalIDCount[virtualGlobalParticleID] > 1)
    {
      records::Particle&  particle = v0.getParticle(i);
      vcentre.appendParticle(particle);
      virtualGlobalIDCount[virtualGlobalParticleID] = -1;
    }
  }
  
  for(int i=0; i<v1.getNumberOfVirtualParticles(); i++)
  {
    int virtualGlobalParticleID = v1.getParticle(i).getGlobalParticleId();
    if(virtualGlobalIDCount[virtualGlobalParticleID] != -1)
    {
      virtualGlobalIDCount[virtualGlobalParticleID]++;
    }

    if(virtualGlobalIDCount[virtualGlobalParticleID] > 1)
    {
      records::Particle&  particle = v1.getParticle(i);
      vcentre.appendParticle(particle);
      virtualGlobalIDCount[virtualGlobalParticleID] = -1;
    }
  }

  for(int i=0; i<v2.getNumberOfVirtualParticles(); i++)
  {
    int virtualGlobalParticleID = v2.getParticle(i).getGlobalParticleId();
    if(virtualGlobalIDCount[virtualGlobalParticleID] != -1)
    {
      virtualGlobalIDCount[virtualGlobalParticleID]++;
    }

    if(virtualGlobalIDCount[virtualGlobalParticleID] > 1)
    {
      records::Particle&  particle = v2.getParticle(i);
      vcentre.appendParticle(particle);
      virtualGlobalIDCount[virtualGlobalParticleID] = -1;
    }
  }

  for(int i=0; i<v3.getNumberOfVirtualParticles(); i++)
  {
    int virtualGlobalParticleID = v3.getParticle(i).getGlobalParticleId();
    if(virtualGlobalIDCount[virtualGlobalParticleID] != -1)
    {
      virtualGlobalIDCount[virtualGlobalParticleID]++;
    }

    if(virtualGlobalIDCount[virtualGlobalParticleID] > 1)
    {
      records::Particle&  particle = v3.getParticle(i);
      vcentre.appendParticle(particle);
      virtualGlobalIDCount[virtualGlobalParticleID] = -1;
    }
  }

  for(int i=0; i<v4.getNumberOfVirtualParticles(); i++)
  {
    int virtualGlobalParticleID = v4.getParticle(i).getGlobalParticleId();
    if(virtualGlobalIDCount[virtualGlobalParticleID] != -1)
    {
      virtualGlobalIDCount[virtualGlobalParticleID]++;
    }

    if(virtualGlobalIDCount[virtualGlobalParticleID] > 1)
    {
      records::Particle&  particle = v4.getParticle(i);
      vcentre.appendParticle(particle);
      virtualGlobalIDCount[virtualGlobalParticleID] = -1;
    }
  }

  for(int i=0; i<v5.getNumberOfVirtualParticles(); i++)
  {
    int virtualGlobalParticleID = v5.getParticle(i).getGlobalParticleId();
    if(virtualGlobalIDCount[virtualGlobalParticleID] != -1)
    {
      virtualGlobalIDCount[virtualGlobalParticleID]++;
    }

    if(virtualGlobalIDCount[virtualGlobalParticleID] > 1)
    {
      records::Particle&  particle = v5.getParticle(i);
      vcentre.appendParticle(particle);
      virtualGlobalIDCount[virtualGlobalParticleID] = -1;
    }
  }

  for(int i=0; i<v6.getNumberOfVirtualParticles(); i++)
  {
    int virtualGlobalParticleID = v6.getParticle(i).getGlobalParticleId();
    if(virtualGlobalIDCount[virtualGlobalParticleID] != -1)
    {
      virtualGlobalIDCount[virtualGlobalParticleID]++;
    }

    if(virtualGlobalIDCount[virtualGlobalParticleID] > 1)
    {
      records::Particle&  particle = v6.getParticle(i);
      vcentre.appendParticle(particle);
      virtualGlobalIDCount[virtualGlobalParticleID] = -1;
    }
  }

  for(int i=0; i<v7.getNumberOfVirtualParticles(); i++)
  {
    int virtualGlobalParticleID = v7.getParticle(i).getGlobalParticleId();
    if(virtualGlobalIDCount[virtualGlobalParticleID] != -1)
    {
      virtualGlobalIDCount[virtualGlobalParticleID]++;
    }

    if(virtualGlobalIDCount[virtualGlobalParticleID] > 1)
    {
      records::Particle&  particle = v7.getParticle(i);
      vcentre.appendParticle(particle);
      virtualGlobalIDCount[virtualGlobalParticleID] = -1;
    }
  }

  //assumption that per vertex there is only one inherited coarse particle
  /*for(auto it = virtualGlobalIDCount.begin(); it != virtualGlobalIDCount.end(); ++it )
  {
    printf("counter[%i]: %i\n", it->first, it->second);
  }*/

  return vcentre;
}

void dem::mappings::Collision::all_to_all(
    dem::Vertex * const                       fineGridVertices,
    const peano::grid::VertexEnumerator&      fineGridVerticesEnumerator,
    State& state,
    State& backgroundstate)
{
  Vertex &v0 = fineGridVertices[fineGridVerticesEnumerator(0)];
  Vertex &v1 = fineGridVertices[fineGridVerticesEnumerator(1)];
  Vertex &v2 = fineGridVertices[fineGridVerticesEnumerator(2)];
  Vertex &v3 = fineGridVertices[fineGridVerticesEnumerator(3)];
  Vertex &v4 = fineGridVertices[fineGridVerticesEnumerator(4)];
  Vertex &v5 = fineGridVertices[fineGridVerticesEnumerator(5)];
  Vertex &v6 = fineGridVertices[fineGridVerticesEnumerator(6)];
  Vertex &v7 = fineGridVertices[fineGridVerticesEnumerator(7)];

  if(
    v0.getNumberOfParticles() == 0 &&
    v1.getNumberOfParticles() == 0 &&
    v2.getNumberOfParticles() == 0 &&
    v3.getNumberOfParticles() == 0 &&
    v4.getNumberOfParticles() == 0 &&
    v5.getNumberOfParticles() == 0 &&
    v6.getNumberOfParticles() == 0 &&
    v7.getNumberOfParticles() == 0) return;

  dem::Vertex vcentre = reduceVirtuals(v0, v1, v2, v3, v4, v5, v6, v7);

  for(int i=0; i<8; i++)
  {
    for(int j=i+1; j<8; j++)
    {
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
          fineGridVertices[fineGridVerticesEnumerator(i)],
          fineGridVertices[fineGridVerticesEnumerator(j)],
          state,
          backgroundstate);
    }
  }

  //full coarse inheritance check
  dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(vcentre, v0, state, backgroundstate);
  dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(vcentre, v1, state, backgroundstate);
  dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(vcentre, v2, state, backgroundstate);
  dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(vcentre, v3, state, backgroundstate);
  dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(vcentre, v4, state, backgroundstate);
  dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(vcentre, v5, state, backgroundstate);
  dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(vcentre, v6, state, backgroundstate);
  dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(vcentre, v7, state, backgroundstate);
  ///
}

dem::mappings::Collision::Collision() {
  logTraceIn( "Collision()" );

  logTraceOut( "Collision()" );
}

dem::mappings::Collision::~Collision() {
  logTraceIn( "~Collision()" );
  logTraceOut( "~Collision()" );
}

#if defined(SharedMemoryParallelisation)
dem::mappings::Collision::Collision(const Collision&  masterThread): _state(masterThread._state) {
  _state.clearAccumulatedData();
}

void dem::mappings::Collision::mergeWithWorkerThread(const Collision& workerThread) {
  _state.merge( workerThread._state );
}
#endif

void dem::mappings::Collision::beginIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "beginIteration(State)", solverState );

	_state = solverState;
	_backgroundTaskState = solverState;
	//_state.clearAccumulatedData();//redundant
	//_backgroundTaskState.clearAccumulatedData();

	assertion( _collisionsOfNextTraversal.empty() );

	if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::PenaltyStat)
	delta::contact::detection::cleanPenaltyStatistics();

	if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::HybridStat)
	delta::contact::detection::cleanHybridStatistics();

	logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::Collision::endIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "endIteration(State)", solverState );

	_state.merge(_backgroundTaskState);
	solverState.merge(_state);

	_activeCollisions.clear();

	assertion( _activeCollisions.empty() );
	assertion( _state.getNumberOfContactPoints()==0 || !_collisionsOfNextTraversal.empty() );

	_activeCollisions.insert(_collisionsOfNextTraversal.begin(), _collisionsOfNextTraversal.end());

	assertion( _state.getNumberOfContactPoints()==0 || !_activeCollisions.empty() );
	_collisionsOfNextTraversal.clear();

	if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::PenaltyStat)
	{
		std::vector<int> penaltyStatistics = delta::contact::detection::getPenaltyStatistics();
		for (int i=0; i<static_cast<int>(penaltyStatistics.size()); i++)
		{
			logInfo( "endIteration(State)", i << " Newton iterations: " << penaltyStatistics[i] );
		}
	}

	if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::HybridStat)
	{
	logInfo( "endIteration(State)", std::endl
								 << "Penalty Fails: " << delta::contact::detection::getPenaltyFails() << " PenaltyFail avg: " << (iREAL)delta::contact::detection::getPenaltyFails()/(iREAL)delta::contact::detection::getBatchSize() << std::endl
								 << "Batch Size: " << delta::contact::detection::getBatchSize() << std::endl
								 << "Batch Fails: " << delta::contact::detection::getBatchFails() << " BatchFail avg: " << (iREAL)delta::contact::detection::getBatchFails()/(iREAL)delta::contact::detection::getBatchSize() << std::endl
								 << "BatchError avg: " << (iREAL)delta::contact::detection::getBatchError()/(iREAL)delta::contact::detection::getBatchSize());
	}

	while (tarch::multicore::jobs::getNumberOfWaitingBackgroundJobs()>0) {
		tarch::multicore::jobs::processBackgroundJobs();
	}

	logTraceOutWith1Argument( "endIteration(State)", solverState);
}

void dem::mappings::Collision::touchVertexFirstTime(
		dem::Vertex&                                 fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridH,
		dem::Vertex * const                          coarseGridVertices,
		const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
		dem::Cell&                                   coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

	iREAL timeStepSize = _state.getTimeStepSize();

	for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
	{
		records::Particle& currentParticle = fineGridVertex.getParticle(i);

		//if value doesn't exist in map - no collision - skip particle
		if(_activeCollisions.count(currentParticle.getGlobalParticleId())==0) {continue;}

		//iREAL force[3]  = {0.0,gravity*currentParticle._persistentRecords.getMass()*(-10),0.0};
		iREAL force[3]  = {0.0,0.0,0.0};
		iREAL torque[3] = {0.0,0.0,0.0};

		//collisions with partner particles
		for(std::vector<Collisions>::iterator p = _activeCollisions[currentParticle.getGlobalParticleId()].begin();
		                                      p != _activeCollisions[currentParticle.getGlobalParticleId()].end();
		                                      p++)
		{
			iREAL rforce[3]  = {0.0,0.0,0.0};
			iREAL rtorque[3] = {0.0,0.0,0.0};

			delta::contact::forces::getContactsForces(p->_contactPoints,
                                       &(currentParticle._persistentRecords._centreOfMass(0)),
                                       &(currentParticle._persistentRecords._referentialCentreOfMass(0)),
                                       &(currentParticle._persistentRecords._angular(0)),
                                       &(currentParticle._persistentRecords._referentialAngular(0)),
                                       &(currentParticle._persistentRecords._velocity(0)),
                                       currentParticle.getMass(),
                                       &(currentParticle._persistentRecords._inverse(0)),
                                       &(currentParticle._persistentRecords._orientation(0)),
                                       currentParticle.getMaterial(),
                                       &(p->_copyOfPartnerParticle._persistentRecords._centreOfMass(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._referentialCentreOfMass(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._angular(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._referentialAngular(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._velocity(0)),
                                       p->_copyOfPartnerParticle.getMass(),
                                       &(p->_copyOfPartnerParticle._persistentRecords._inverse(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._orientation(0)),
                                       p->_copyOfPartnerParticle.getMaterial(),
                                       rforce, rtorque,
                                       (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere));

			force[0] += rforce[0];
			force[1] += rforce[1];
			force[2] += rforce[2];

			torque[0] += rtorque[0];
			torque[1] += rtorque[1];
			torque[2] += rtorque[2];
		}

		if(!currentParticle.getIsObstacle())
		{
			currentParticle._persistentRecords._velocity(0) += timeStepSize * (force[0] / currentParticle.getMass());
			currentParticle._persistentRecords._velocity(1) += timeStepSize * (force[1] / currentParticle.getMass());
			currentParticle._persistentRecords._velocity(2) += timeStepSize * (force[2] / currentParticle.getMass());

			delta::dynamics::updateAngular(&currentParticle._persistentRecords._referentialAngular(0),
                                      &currentParticle._persistentRecords._orientation(0),
                                      &currentParticle._persistentRecords._inertia(0),
                                      &currentParticle._persistentRecords._inverse(0),
                                      currentParticle.getMass(), //why mass is passed here, remove if not used
                                      torque, timeStepSize);
		}
	}

	fineGridVertex.clearInheritedCoarseGridParticles();// clear adaptivity/multilevel data

	dfor2(k)
	fineGridVertex.inheritCoarseGridParticles(coarseGridVertices[coarseGridVerticesEnumerator(k)], fineGridX, fineGridH(0));
	enddforx

	logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}

void dem::mappings::Collision::enterCell(
		dem::Cell&                                fineGridCell,
		dem::Vertex * const                       fineGridVertices,
		const peano::grid::VertexEnumerator&      fineGridVerticesEnumerator,
		dem::Vertex * const                       coarseGridVertices,
		const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
		dem::Cell&                                coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

	logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}

void dem::mappings::Collision::leaveCell(
    dem::Cell&           fineGridCell,
    dem::Vertex * const  fineGridVertices,
    const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
    dem::Vertex * const  coarseGridVertices,
    const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
    dem::Cell&           coarseGridCell,
    const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {

  all_to_all(fineGridVertices, fineGridVerticesEnumerator, _state, _backgroundTaskState);

}

void dem::mappings::Collision::touchVertexLastTime(
    dem::Vertex&         fineGridVertex,
    const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
    const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
    dem::Vertex * const  coarseGridVertices,
    const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
    dem::Cell&           coarseGridCell,
    const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  if (fineGridVertex.getNumberOfParticles()==0) return;

  #ifdef SharedTBB
  const int grainSize = (RunParticleLoopInParallel||fineGridVertex.getNumberOfParticles()==0) ? 1 : fineGridVertex.getNumberOfParticles();
  tbb::parallel_for(
   tbb::blocked_range<int>(0,fineGridVertex.getNumberOfParticles(),grainSize),
   [&](const tbb::blocked_range<int>& r) {
    for (int i=r.begin(); i!=r.end(); i++) {
  #else
  for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++) {
  #endif
    for(int j=i+1; j<fineGridVertex.getNumberOfParticles(); j++)
    {
    if((fineGridVertex.getParticle(i).getGlobalParticleId() == fineGridVertex.getParticle(j).getGlobalParticleId()) ||
       (fineGridVertex.getParticle(i).getIsObstacle() && fineGridVertex.getParticle(j).getIsObstacle()))
      continue;

    if (RunParticleComparisionsInBackground) {
      auto p0 = fineGridVertex.getParticle(i);
      auto p1 = fineGridVertex.getNumberOfTriangles(i);
      auto p2 = fineGridVertex.getXCoordinates(i);
      auto p3 = fineGridVertex.getYCoordinates(i);
      auto p4 = fineGridVertex.getZCoordinates(i);
      auto p5 = fineGridVertex.getParticle(j);
      auto p6 = fineGridVertex.getNumberOfTriangles(j);
      auto p7 = fineGridVertex.getXCoordinates(j);
      auto p8 = fineGridVertex.getYCoordinates(j);
      auto p9 = fineGridVertex.getZCoordinates(j);

      peano::datatraversal::TaskSet backgroundTask(
       [=] ()->bool {
        dem::mappings::Collision::collisionDetection(
          p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,
          &_backgroundTaskState,
          true
        );
        return false;
       },
       peano::datatraversal::TaskSet::TaskType::Background
      );

      logDebug( "collideParticlesOfTwoDifferentVertices(...)", "spawn background task" );
    }
    else {
      collisionDetection(
        fineGridVertex.getParticle(i),
        fineGridVertex.getNumberOfTriangles(i),
        fineGridVertex.getXCoordinates(i),
        fineGridVertex.getYCoordinates(i),
        fineGridVertex.getZCoordinates(i),
        fineGridVertex.getParticle(j),
        fineGridVertex.getNumberOfTriangles(j),
        fineGridVertex.getXCoordinates(j),
        fineGridVertex.getYCoordinates(j),
        fineGridVertex.getZCoordinates(j),
        &_state,
        false
      );
    }
   }
  #ifdef SharedTBB
  }});
  #else
  }
  #endif
}

void dem::mappings::Collision::createHangingVertex(
		dem::Vertex&     fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                fineGridH,
		dem::Vertex * const   coarseGridVertices,
		const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
		dem::Cell&       coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::destroyHangingVertex(
		const dem::Vertex&   fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::createInnerVertex(
		dem::Vertex&               fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridH,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::createBoundaryVertex(
		dem::Vertex&               fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridH,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::destroyVertex(
		const dem::Vertex&   fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::createCell(
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
	// @todo Insert your code here
	logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}

void dem::mappings::Collision::destroyCell(
		const dem::Cell&           fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "destroyCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyCell(...)", fineGridCell );
}

#ifdef Parallel
void dem::mappings::Collision::mergeWithNeighbour(
		dem::Vertex&  vertex,
		const dem::Vertex&  neighbour,
		int                                           fromRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridH,
		int                                           level
) {
	logTraceInWith6Arguments( "mergeWithNeighbour(...)", vertex, neighbour, fromRank, fineGridX, fineGridH, level );
	// @todo Insert your code here
	logTraceOut( "mergeWithNeighbour(...)" );
}

void dem::mappings::Collision::prepareSendToNeighbour(
		dem::Vertex&  vertex,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
		int                                           level
) {
	logTraceInWith3Arguments( "prepareSendToNeighbour(...)", vertex, toRank, level );
	// @todo Insert your code here
	logTraceOut( "prepareSendToNeighbour(...)" );
}

void dem::mappings::Collision::prepareCopyToRemoteNode(
		dem::Vertex&  localVertex,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
		int                                           level
) {
	logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localVertex, toRank, x, h, level );
	// @todo Insert your code here
	logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::Collision::prepareCopyToRemoteNode(
		dem::Cell&  localCell,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   cellCentre,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   cellSize,
		int                                           level
) {
	logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localCell, toRank, cellCentre, cellSize, level );
	// @todo Insert your code here
	logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::Collision::mergeWithRemoteDataDueToForkOrJoin(
		dem::Vertex&  localVertex,
		const dem::Vertex&  masterOrWorkerVertex,
		int                                       fromRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  x,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  h,
		int                                       level
) {
	logTraceInWith6Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localVertex, masterOrWorkerVertex, fromRank, x, h, level );
	// @todo Insert your code here
	logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

void dem::mappings::Collision::mergeWithRemoteDataDueToForkOrJoin(
		dem::Cell&  localCell,
		const dem::Cell&  masterOrWorkerCell,
		int                                       fromRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  cellCentre,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  cellSize,
		int                                       level
) {
	logTraceInWith3Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localCell, masterOrWorkerCell, fromRank );
	// @todo Insert your code here
	logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

bool dem::mappings::Collision::prepareSendToWorker(
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
		int                                                                  worker
) {
	logTraceIn( "prepareSendToWorker(...)" );
	// @todo Insert your code here
	logTraceOutWith1Argument( "prepareSendToWorker(...)", true );
	return true;
}

void dem::mappings::Collision::prepareSendToMaster(
		dem::Cell&                       localCell,
		dem::Vertex *                    vertices,
		const peano::grid::VertexEnumerator&       verticesEnumerator,
		const dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
		const dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
	logTraceInWith2Arguments( "prepareSendToMaster(...)", localCell, verticesEnumerator.toString() );
	// @todo Insert your code here
	logTraceOut( "prepareSendToMaster(...)" );
}

void dem::mappings::Collision::mergeWithMaster(
		const dem::Cell&           workerGridCell,
		dem::Vertex * const        workerGridVertices,
		const peano::grid::VertexEnumerator& workerEnumerator,
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
		int                                                                  worker,
		const dem::State&          workerState,
		dem::State&                masterState
) {
	logTraceIn( "mergeWithMaster(...)" );
	// @todo Insert your code here
	logTraceOut( "mergeWithMaster(...)" );
}

void dem::mappings::Collision::receiveDataFromMaster(
		dem::Cell&                        receivedCell,
		dem::Vertex *                     receivedVertices,
		const peano::grid::VertexEnumerator&        receivedVerticesEnumerator,
		dem::Vertex * const               receivedCoarseGridVertices,
		const peano::grid::VertexEnumerator&        receivedCoarseGridVerticesEnumerator,
		dem::Cell&                        receivedCoarseGridCell,
		dem::Vertex * const               workersCoarseGridVertices,
		const peano::grid::VertexEnumerator&        workersCoarseGridVerticesEnumerator,
		dem::Cell&                        workersCoarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&    fineGridPositionOfCell
) {
	logTraceIn( "receiveDataFromMaster(...)" );
	// @todo Insert your code here
	logTraceOut( "receiveDataFromMaster(...)" );
}

void dem::mappings::Collision::mergeWithWorker(
		dem::Cell&           localCell,
		const dem::Cell&     receivedMasterCell,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  cellCentre,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  cellSize,
		int                                          level
) {
	logTraceInWith2Arguments( "mergeWithWorker(...)", localCell.toString(), receivedMasterCell.toString() );
	// @todo Insert your code here
	logTraceOutWith1Argument( "mergeWithWorker(...)", localCell.toString() );
}

void dem::mappings::Collision::mergeWithWorker(
		dem::Vertex&        localVertex,
		const dem::Vertex&  receivedMasterVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
		int                                           level
) {
	logTraceInWith2Arguments( "mergeWithWorker(...)", localVertex.toString(), receivedMasterVertex.toString() );
	// @todo Insert your code here
	logTraceOutWith1Argument( "mergeWithWorker(...)", localVertex.toString() );
}
#endif

void dem::mappings::Collision::descend(
		dem::Cell * const          fineGridCells,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell
) {
	logTraceInWith2Arguments( "descend(...)", coarseGridCell.toString(), coarseGridVerticesEnumerator.toString() );
	// @todo Insert your code here
	logTraceOut( "descend(...)" );
}

void dem::mappings::Collision::ascend(
		dem::Cell * const    fineGridCells,
		dem::Vertex * const  fineGridVertices,
		const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell
) {
	logTraceInWith2Arguments( "ascend(...)", coarseGridCell.toString(), coarseGridVerticesEnumerator.toString() );
	// @todo Insert your code here
	logTraceOut( "ascend(...)" );
}
