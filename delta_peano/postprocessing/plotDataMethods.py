import readLogMethods
import math

#PARTICLE PROPERTIES - SNAPPED PER PLOT - axis - particleXaxis
#listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin,
#listnoOfTriangles, listisObstacle, listmaterial, listlinear, listangular, listrangular,
#listcentre, listcOfMas, listrcOfMa, listiner, listinve, listorie

#CONTACT PROPERTIES - SNAPPED PER PLOT AND PER CONTACT EVENT - axis - contactXaxis
#listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal,
#listxContactPosition, listnormalX, listp, listq

#FORCE PROPERTIES between two PARTICLES - axis - forceXaxis
#listforceId, listforcemasterParticleNo, listforceslaveParticleNo, listmassA, listmassB, listforce, listfriction,

#CONTACTs PROPERTIES of TOTAL FORCE between two PARTICLES - axis - subforceXaxis
#listsubContactid, listsubDamper, listsubSpring, listsubrelativeVelocity, listsubdepth, listsubspring, listsubtotalForce, listsubdamp, listsubcontactmass

listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, \
listnoOfTriangles, listisObstacle, listmaterial, listlinear, listangular, listrangular, \
listcentre, listcOfMas, listrcOfMa, listiner, listinve, listorie, \
listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal, \
listxContactPosition, listnormalX, listp, listq, \
listforceId, listforcemasterParticleNo, listforceslaveParticleNo, listmassA, listmassB, listforce, listfriction, \
listsubContactid, listsubDamper, listsubSpring, listsubrelativeVelocity, listsubdepth, listsubspring_depth, listsubtotalForce, listsubdamp, listsubcontactmass, \
particleXaxis, contactXaxis, forceXaxis, subforceXaxis, iterations = readLogMethods.readLog()

def getSimSteps():
    return len(iterations)

def getPlotSteps():
    return getSimSteps()/50

def getParticleCount():
    return len(particleXaxis)/getPlotSteps()

def getListOfUniqueParticleIds():
    list = []
    for i in range(0, len(particleXaxis)):
        found = False
        for j in range(0, len(list)):
            if listparticleId[i] == list[j]:
                found = True
        if found == False:
            list.append(listparticleId[i])
    return list

def getParticleData(particleIndex):

    listFilteredParticleId = []
    listFilteredMass = []
    listFilteredDiameter = []
    listFilteredInfluenceRadius = []
    listFilteredEpsilon = []
    listFilteredthMin = []
    listFilterednoOfTriangles = []
    listFilteredisObstacle = []
    listFilteredmaterial = []
    listFilteredlinear = []
    listFilteredAngular = []
    listFilteredrangular = []
    listFilteredcentre = []
    listFilteredcOfMas = []
    listFilteredrcOfMa = []
    listFilterediner = []
    listFilteredinve = []
    listFilteredorie = []

    for i in range(0, getPlotSteps()*getParticleCount()):
        if particleIndex == listparticleId[i]:
            listFilteredParticleId.append(listparticleId[i])
            listFilteredMass.append(listmass[i])
            listFilteredDiameter.append(listdiameter[i])
            listFilteredInfluenceRadius.append(listinfluenceRadius[i])
            listFilteredEpsilon.append(listepsilon[i])
            listFilteredthMin.append(listhMin[i])
            listFilterednoOfTriangles.append(listnoOfTriangles[i])
            listFilteredisObstacle.append(listisObstacle[i])
            listFilteredmaterial.append(listmaterial[i])
            listFilteredlinear.append(listlinear[i])
            listFilteredAngular.append(listangular[i])
            listFilteredrangular.append(listrangular[i])
            listFilteredcentre.append(listcentre[i])
            listFilteredcOfMas.append(listcOfMas[i])
            listFilteredrcOfMa.append(listrcOfMa[i])
            listFilterediner.append(listiner[i])
            listFilteredinve.append(listinve[i])
            listFilteredorie.append(listorie[i])

    return listFilteredParticleId, listFilteredMass, listFilteredDiameter, \
            listFilteredInfluenceRadius, listFilteredEpsilon, listFilteredthMin, listFilterednoOfTriangles, \
            listFilteredisObstacle, listFilteredmaterial, listFilteredlinear, listFilteredAngular, \
            listFilteredrangular, listFilteredcentre, listFilteredcOfMas, \
            listFilteredrcOfMa, listFilterediner, listFilteredinve, listFilteredorie

def getParticleData(particleStart, particleEnd, timestep):

    listFilteredParticleId = []
    listFilteredMass = []
    listFilteredDiameter = []
    listFilteredInfluenceRadius = []
    listFilteredEpsilon = []
    listFilteredthMin = []
    listFilterednoOfTriangles = []
    listFilteredisObstacle = []
    listFilteredmaterial = []
    listFilteredlinear = []
    listFilteredAngular = []
    listFilteredrangular = []
    listFilteredcentre = []
    listFilteredcOfMas = []
    listFilteredrcOfMa = []
    listFilterediner = []
    listFilteredinve = []
    listFilteredorie = []

    #print(getPlotSteps())
    #print(timestep)
    #print(getParticleCount())
    #print(int(getParticleCount()*getPlotSteps()))
    #print(int(timestep*getPlotSteps()))
    #print(int((getParticleCount()*getPlotSteps())+timestep))
    it = math.floor(timestep/50)
    for i in range(int((it)*getParticleCount()), int(((it+1)*getParticleCount()))):
        if int(listparticleId[i]) >= particleStart and int(listparticleId[i]) <= particleEnd:
            listFilteredParticleId.append(listparticleId[i])
            listFilteredMass.append(listmass[i])
            listFilteredDiameter.append(listdiameter[i])
            listFilteredInfluenceRadius.append(listinfluenceRadius[i])
            listFilteredEpsilon.append(listepsilon[i])
            listFilteredthMin.append(listhMin[i])
            listFilterednoOfTriangles.append(listnoOfTriangles[i])
            listFilteredisObstacle.append(listisObstacle[i])
            listFilteredmaterial.append(listmaterial[i])
            listFilteredlinear.append(listlinear[i])
            listFilteredAngular.append(listangular[i])
            listFilteredrangular.append(listrangular[i])
            listFilteredcentre.append(listcentre[i])
            listFilteredcOfMas.append(listcOfMas[i])
            listFilteredrcOfMa.append(listrcOfMa[i])
            listFilterediner.append(listiner[i])
            listFilteredinve.append(listinve[i])
            listFilteredorie.append(listorie[i])

    return listFilteredParticleId, listFilteredMass, listFilteredDiameter, \
            listFilteredInfluenceRadius, listFilteredEpsilon, listFilteredthMin, listFilterednoOfTriangles, \
            listFilteredisObstacle, listFilteredmaterial, listFilteredlinear, listFilteredAngular, \
            listFilteredrangular, listFilteredcentre, listFilteredcOfMas, \
            listFilteredrcOfMa, listFilterediner, listFilteredinve, listFilteredorie


def getContactData(masterIndex, slaveIndex, interchangeable):

    listFilteredContactId = []
    listFilteredMasterId = []
    listFilteredSlaveId = []
    listFilteredHasfriction = []
    listFilteredDistance = []
    listFilteredDepth = []
    listFilteredEpsilon = []
    listFilteredContactPosition = []
    listFilteredNormalX = []
    listFilteredP = []
    listFilteredQ = []

    for i in range(0, len(listcontactId)):
        if not interchangeable:
            if masterIndex == listmasterId[i] and slaveIndex == listSlaveId[i]:
                listFilteredContactId.append(listcontactId[i])
                listFilteredMasterId.append(listmasterId[i])
                listFilteredSlaveId.append(listSlaveId[i])
                listFilteredHasfriction.append(listhasFriction[i])
                listFilteredDistance.append(listdistance[i])
                listFilteredDepth.append(listdepth[i])
                listFilteredEpsilon.append(listepsilonTotal[i])
                listFilteredContactPosition.append(listxContactPosition[i])
                listFilteredNormalX.append(listnormalX[i])
                listFilteredP.append(listp[i])
                listFilteredQ.append(listq[i])
        else:
            if (masterIndex == listmasterId[i] and slaveIndex == listSlaveId[i]) or (slaveIndex == listmasterId[i] and masterindex == listSlaveId[i]):
                listFilteredContactId.append(listcontactId[i])
                listFilteredMasterId.append(listmasterId[i])
                listFilteredSlaveId.append(listSlaveId[i])
                listFilteredHasfriction.append(listhasFriction[i])
                listFilteredDistance.append(listdistance[i])
                listFilteredDepth.append(listdepth[i])
                listFilteredEpsilon.append(listepsilonTotal[i])
                listFilteredContactPosition.append(listxContactPosition[i])
                listFilteredNormalX.append(listnormalX[i])
                listFilteredP.append(listp[i])
                listFilteredQ.append(listq[i])

    return  listFilteredContactId, listFilteredMasterId, listFilteredSlaveId, listFilteredHasfriction, \
            listFilteredDistance, listFilteredDepth, listFilteredEpsilon, listFilteredContactPosition, \
            listFilteredNormalX, listFilteredP, listFilteredQ


def getContactData(particleIndex):

    listFilteredContactId = []
    listFilteredMasterId = []
    listFilteredSlaveId = []
    listFilteredHasfriction = []
    listFilteredDistance = []
    listFilteredDepth = []
    listFilteredEpsilon = []
    listFilteredContactPosition = []
    listFilteredNormalX = []
    listFilteredP = []
    listFilteredQ = []

    for i in range(0, len(listcontactId)):
        if particleIndex == listmasterId[i] or particleIndex == listSlaveId[i]:
            listFilteredContactId.append(listcontactId[i])
            listFilteredMasterId.append(listmasterId[i])
            listFilteredSlaveId.append(listSlaveId[i])
            listFilteredHasfriction.append(listhasFriction[i])
            listFilteredDistance.append(listdistance[i])
            listFilteredDepth.append(listdepth[i])
            listFilteredEpsilon.append(listepsilonTotal[i])
            listFilteredContactPosition.append(listxContactPosition[i])
            listFilteredNormalX.append(listnormalX[i])
            listFilteredP.append(listp[i])
            listFilteredQ.append(listq[i])

    return  listFilteredContactId, listFilteredMasterId, listFilteredSlaveId, listFilteredHasfriction, \
            listFilteredDistance, listFilteredDepth, listFilteredEpsilon, listFilteredContactPosition, \
            listFilteredNormalX, listFilteredP, listFilteredQ


def getForce(masterIndex, slaveIndex, interchangeable):

    listFilteredForceId = []
    listFilteredForceMasterParticleNo = []
    listFilteredForceSlaveParticleNo = []
    listFilteredMassA = []
    listFilteredMassB = []
    listFilteredForce = []
    listFilteredfriction = []

    for i in range(0, len(forceXaxis)):
        if interchangeable:
            if masterIndex == listforcemasterParticleNo[i] and slaveIndex == listforceslaveParticleNo[i]:
                listFilteredForceId.append(listforceId[i])
                listFilteredForceMasterParticleNo.append(listFilteredForceMasterParticleNo[i])
                listFilteredForceSlaveParticleNo.append(listFilteredForceSlaveParticleNo[i])
                listFilteredMassA.append(listmassA[i])
                listFilteredMassB.append(listmassB[i])
                listFilteredForce.append(listforce[i])
                listFilteredfriction.append(listfriction[i])
        else:
            if (masterIndex == listforcemasterParticleNo[i] and slaveIndex == listforceslaveParticleNo[i]) or (slaveIndex == listforcemasterParticleNo[i] and masterIndex == listforceslaveParticleNo[i]):
                listFilteredForceId.append(listforceId[i])
                listFilteredForceMasterParticleNo.append(listFilteredForceMasterParticleNo[i])
                listFilteredForceSlaveParticleNo.append(listFilteredForceSlaveParticleNo[i])
                listFilteredMassA.append(listmassA[i])
                listFilteredMassB.append(listmassB[i])
                listFilteredForce.append(listforce[i])
                listFilteredfriction.append(listfriction[i])

    return listFilteredForceId, listFilteredForceMasterParticleNo, listFilteredForceSlaveParticleNo, listFilteredMassA, \
           listFilteredMassB, listFilteredForce, listFilteredfriction


def getForce(particleIndex):

    listFilteredForceId = []
    listFilteredForceMasterParticleNo = []
    listFilteredForceSlaveParticleNo = []
    listFilteredMassA = []
    listFilteredMassB = []
    listFilteredForce = []
    listFilteredfriction = []

    for i in range(0, len(forceXaxis)):
        if particleIndex == listforcemasterParticleNo[i] or particleIndex == listforceslaveParticleNo[i]:
            listFilteredForceId.append(listforceId[i])
            listFilteredForceMasterParticleNo.append(listFilteredForceMasterParticleNo[i])
            listFilteredForceSlaveParticleNo.append(listFilteredForceSlaveParticleNo[i])
            listFilteredMassA.append(listmassA[i])
            listFilteredMassB.append(listmassB[i])
            listFilteredForce.append(listforce[i])
            listFilteredfriction.append(listfriction[i])

    return listFilteredForceId, listFilteredForceMasterParticleNo, listFilteredForceSlaveParticleNo, listFilteredMassA, \
           listFilteredMassB, listFilteredForce, listFilteredfriction


def getContactDataOfInteraction(masterIndex, slaveIndex, interchangeable):

    listFilteredSubContactId = []
    listFilteredSubDamper = []
    listFilteredSubString = []
    listFilteredSubRelativeVelocity = []
    listFilteredSubDepth = []
    listFilteredSubSpring_Depth = []
    listFilteredSubTotalForce = []
    listFilteredSubDamp = []
    listFilteredSubContactMass = []

    for i in range(0, len(forceXaxis)):
        if interchangeable:
            if masterIndex == listforcemasterParticleNo[i] and slaveIndex == listforceslaveParticleNo[i]:
                listFilteredSubContactId.append(listsubContactid[i])
                listFilteredSubDamper.append(listsubDamper[i])
                listFilteredSubString.append(listsubSpring[i])
                listFilteredSubRelativeVelocity.append(listsubrelativeVelocity[i])
                listFilteredSubDepth.append(listsubdepth[i])
                listFilteredSubSpring_Depth.append(listsubspring_depth[i])
                listFilteredSubTotalForce.append(listsubtotalForce[i])
                listFilteredSubDamp.append(listsubdamp[i])
                listFilteredSubContactMass.append(listsubcontactmass[i])
        else:
            if (masterIndex == listforcemasterParticleNo[i] and slaveIndex == listforceslaveParticleNo[i]) or (slaveIndex == listforcemasterParticleNo[i] and masterIndex == listforceslaveParticleNo[i]):
                listFilteredSubContactId.append(listsubContactid[i])
                listFilteredSubDamper.append(listsubDamper[i])
                listFilteredSubString.append(listsubSpring[i])
                listFilteredSubRelativeVelocity.append(listsubrelativeVelocity[i])
                listFilteredSubDepth.append(listsubdepth[i])
                listFilteredSubSpring_Depth.append(listsubspring_depth[i])
                listFilteredSubTotalForce.append(listsubtotalForce[i])
                listFilteredSubDamp.append(listsubdamp[i])
                listFilteredSubContactMass.append(listsubcontactmass[i])

    return  listFilteredSubContactId, listFilteredSubDamper, listFilteredSubString, listFilteredSubRelativeVelocity, \
            listFilteredSubDepth, listFilteredSubSpring_Depth, listFilteredSubTotalForce, listFilteredSubDamp, listFilteredSubContactMass


def getContactDataOfInteraction(particleIndex):

    listFilteredSubContactId = []
    listFilteredSubDamper = []
    listFilteredSubString = []
    listFilteredSubRelativeVelocity = []
    listFilteredSubDepth = []
    listFilteredSubSpring = []
    listFilteredSubTotalForce = []
    listFilteredSubDamp = []
    listFilteredSubContactMass = []

    for i in range(0, len(forceXaxis)):
        if particleIndex == listforcemasterParticleNo[i] or particleIndex == listforceslaveParticleNo[i]:
            listFilteredSubContactId.append(listsubContactid[i])
            listFilteredSubDamper.append(listsubDamper[i])
            listFilteredSubString.append(listsubSpring[i])
            listFilteredSubRelativeVelocity.append(listsubrelativeVelocity[i])
            listFilteredSubDepth.append(listsubdepth[i])
            listFilteredSubSpring.append(listsubSpring[i])
            listFilteredSubTotalForce.append(listsubtotalForce[i])
            listFilteredSubDamp.append(listsubdamp[i])
            listFilteredSubContactMass.append(listsubcontactmass[i])

    return  listFilteredSubContactId, listFilteredSubDamper, listFilteredSubString, listFilteredSubRelativeVelocity, \
            listFilteredSubDepth, listFilteredSubSpring, listFilteredSubTotalForce, listFilteredSubDamp, listFilteredSubContactMass