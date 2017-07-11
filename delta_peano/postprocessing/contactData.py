import readParticleLog

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

def getContactData(masterIndex, slaveIndex, interchangeable, filename):

    listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, listnoOfTriangles, \
    listisObstacle, listmaterial, listlinear, listangular, listrangular, listcentre, listcOfMas, \
    listrcOfMa, listiner, listinve, listorie, \
    listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal, \
    listsubDamper, listsubSpring, listsubvij, listsubrelativeVelocity, listsubdepth, listsubspring_depth, listsubtotalForce, listsubdamp, listsubcontactmass, \
    listxContactPosition, listnormalX, listfriction, listmassA, listmassB, listforce, listtorque, listp, listq, \
    particleXaxis, contactXaxis, iterations, reassigns, parCmp, triCmp, cnpt, gridV, t, dt = readParticleLog.readLog(filename)

    listFilteredContactId = []
    listFilteredMasterId = []
    listFilteredSlaveId = []
    listFilteredHasfriction = []
    listFilteredDistance = []
    listFilteredDepth = []
    listFilteredEpsilon = []
    listFilteredSubDamper = []
    listFilteredSubString = []
    listFilteredSubVij = []
    listFilteredSubRelativeVelocity = []
    listFilteredSubDepth = []
    listFilteredSubSpring_Depth = []
    listFilteredSubTotalForce = []
    listFilteredSubDamp = []
    listFilteredSubContactMass = []
    listFilteredContactPosition = []
    listFilteredNormalX = []
    listFilteredfriction = []
    listFilteredMassA = []
    listFilteredMassB = []
    listFilteredForce = []
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
                listFilteredSubDamper.append(listsubDamper[i])
                listFilteredSubString.append(listsubSpring[i])
                listFilteredSubVij.append(listsubvij[i])
                listFilteredSubRelativeVelocity.append(listsubrelativeVelocity[i])
                listFilteredSubDepth.append(listsubdepth[i])
                listFilteredSubSpring_Depth.append(listsubspring_depth[i])
                listFilteredSubTotalForce.append(listsubtotalForce[i])
                listFilteredSubDamp.append(listsubdamp[i])
                listFilteredSubContactMass.append(listsubcontactmass[i])
                listFilteredContactPosition.append(listxContactPosition[i])
                listFilteredNormalX.append(listnormalX[i])
                listFilteredfriction.append(listfriction[i])
                listFilteredMassA.append(listmassA[i])
                listFilteredMassB.append(listmassB[i])
                listFilteredForce.append(listforce[i])
                listFilteredP.append(listp[i])
                listFilteredQ.append(listq[i])
        else:
            if (masterIndex == listmasterId[i] and slaveIndex == listSlaveId[i]) or (slaveIndex == listmasterId[i] and masterIndex == listSlaveId[i]):
                listFilteredContactId.append(listcontactId[i])
                listFilteredMasterId.append(listmasterId[i])
                listFilteredSlaveId.append(listSlaveId[i])
                listFilteredHasfriction.append(listhasFriction[i])
                listFilteredDistance.append(listdistance[i])
                listFilteredDepth.append(listdepth[i])
                listFilteredEpsilon.append(listepsilonTotal[i])
                listFilteredSubDamper.append(listsubDamper[i])
                listFilteredSubString.append(listsubSpring[i])
                listFilteredSubVij.append(listsubvij[i])
                listFilteredSubRelativeVelocity.append(listsubrelativeVelocity[i])
                listFilteredSubDepth.append(listsubdepth[i])
                listFilteredSubSpring_Depth.append(listsubspring_depth[i])
                listFilteredSubTotalForce.append(listsubtotalForce[i])
                listFilteredSubDamp.append(listsubdamp[i])
                listFilteredSubContactMass.append(listsubcontactmass[i])
                listFilteredContactPosition.append(listxContactPosition[i])
                listFilteredNormalX.append(listnormalX[i])
                listFilteredfriction.append(listfriction[i])
                listFilteredMassA.append(listmassA[i])
                listFilteredMassB.append(listmassB[i])
                listFilteredForce.append(listforce[i])
                listFilteredP.append(listp[i])
                listFilteredQ.append(listq[i])

    return  listFilteredContactId, listFilteredMasterId, listFilteredSlaveId, listFilteredHasfriction, \
            listFilteredDistance, listFilteredDepth, listFilteredEpsilon, listFilteredSubDamper.append(listsubDamper[i]), \
            listFilteredSubString, listFilteredSubVij, listFilteredSubRelativeVelocity, listFilteredSubDepth, \
            listFilteredSubSpring_Depth, listFilteredSubTotalForce, listFilteredSubDamp, \
            listFilteredSubContactMass, listFilteredContactPosition, \
            listFilteredNormalX, listFilteredP, listFilteredQ


def getContactData(particleIndex, filename):

    listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, listnoOfTriangles, \
    listisObstacle, listmaterial, listlinear, listangular, listrangular, listcentre, listcOfMas, \
    listrcOfMa, listiner, listinve, listorie, \
    listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal, \
    listsubDamper, listsubSpring, listsubvij, listsubrelativeVelocity, listsubdepth, listsubspring_depth, listsubtotalForce, listsubdamp, listsubcontactmass, \
    listxContactPosition, listnormalX, listfriction, listmassA, listmassB, listforce, listtorque, listp, listq, \
    particleXaxis, contactXaxis, iterations, reassigns, parCmp, triCmp, cnpt, gridV, t, dt = readParticleLog.readLog(filename)

    listFilteredContactId = []
    listFilteredMasterId = []
    listFilteredSlaveId = []
    listFilteredHasfriction = []
    listFilteredDistance = []
    listFilteredDepth = []
    listFilteredEpsilon = []
    listFilteredSubDamper = []
    listFilteredSubString = []
    listFilteredSubVij = []
    listFilteredSubRelativeVelocity = []
    listFilteredSubDepth = []
    listFilteredSubSpring_Depth = []
    listFilteredSubTotalForce = []
    listFilteredSubDamp = []
    listFilteredSubContactMass = []
    listFilteredContactPosition = []
    listFilteredNormalX = []
    listFilteredfriction = []
    listFilteredMassA = []
    listFilteredMassB = []
    listFilteredForce = []

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
            listFilteredSubDamper.append(listsubDamper[i])
            listFilteredSubString.append(listsubSpring[i])
            listFilteredSubVij.append(listsubvij[i])
            listFilteredSubRelativeVelocity.append(listsubrelativeVelocity[i])
            listFilteredSubDepth.append(listsubdepth[i])
            listFilteredSubSpring_Depth.append(listsubspring_depth[i])
            listFilteredSubTotalForce.append(listsubtotalForce[i])
            listFilteredSubDamp.append(listsubdamp[i])
            listFilteredSubContactMass.append(listsubcontactmass[i])
            listFilteredContactPosition.append(listxContactPosition[i])
            listFilteredNormalX.append(listnormalX[i])
            listFilteredfriction.append(listfriction[i])
            listFilteredMassA.append(listmassA[i])
            listFilteredMassB.append(listmassB[i])
            listFilteredForce.append(listforce[i])
            listFilteredP.append(listp[i])
            listFilteredQ.append(listq[i])

    return  listFilteredContactId, listFilteredMasterId, listFilteredSlaveId, listFilteredHasfriction, \
            listFilteredDistance, listFilteredDepth, listFilteredEpsilon, listFilteredSubDamper.append(listsubDamper[i]), \
            listFilteredSubString, listFilteredSubVij, listFilteredSubRelativeVelocity, listFilteredSubDepth, \
            listFilteredSubSpring_Depth, listFilteredSubTotalForce, listFilteredSubDamp, \
            listFilteredSubContactMass, listFilteredContactPosition, \
            listFilteredNormalX, listFilteredP, listFilteredQ

