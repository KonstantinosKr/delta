from plotdata import *
import matplotlib.pyplot as plt
import numpy as np
"""
getSimSteps():
getPlotSteps():
getParticleCount():
getListOfUniqueParticleIds():

getParticleData(particleIndex):
    return listParticleId, listMass, listDiameter, \
            listInfluenceRadius, listEpsilon, listthMin, listNoOfTriangles, \
            listIsObstacle, listMaterial, listLinear, listAngular, \
            listRefangular, listCentre, listCenterOfMass, \
            listRefCenterOfMass, listInertia, listInverse, listOrientation

getContactData(masterIndex, slaveIndex, interchangeable):
    return  listContactId, listMasterId, listSlaveId, listHasFriction, \
            listDistance, listDepth, listEpsilon, listContactPosition, \
            listNormalX, listP, listQ

getForce(masterIndex, slaveIndex, interchangeable):
    return listForceId, listForceMaster, listForceSlave, listMassA, \
           listMassB, listForce, listFriction

getContactDataOfInteraction(masterIndex, slaveIndex, interchangeable):
    return  listSubContactId, listSubDamper, listSubSpring, listSubRelativeVelocity, \
            listSubDepth, listSubSpring_Depth, listSubTotalForce, listSubDamp, listSubContactMass
"""

#ParticleData
listParticleId = []
listMass = []
listDiameter = []
listInfluenceRadius = []
listEpsilon = []
listthMin = []
listNoOfTriangles = []
listIsObstacle = []
listMaterial = []
listLinear = []
listAngular = []
listRefangular = []
listCentre = []
listCenterOfMass = []
listRefCenterOfMas = []
listInertia = []
listInverse = []
listOrientation = []

#ContactData
listContactId = []
listMasterId = []
listSlaveId = []
listHasFriction = []
listDistance = []
listDepth = []
listEpsilon = []
listContactPosition = []
listNormalX = []
listP = []
listQ = []

#ForceData
listForceId = []
listForceMaster = []
listForceSlave = []
listMassA = []
listMassB = []
listForce = []
listFriction = []

#SubContactData
listSubContactId = []
listSubDamper = []
listSubSpring = []
listSubRelativeVelocity = []
listSubDepth = []
listSubSpring_Depth = []
listSubTotalForce = []
listSubDamp = []
listSubContactMass = []

print(getParticleCount())


#fig, ax = plt.subplots()
#ax.plot(particleXaxis, listparticleId, label='--', linestyle='-', marker='o', markersize=10, markevery=1)

#print(listmassB)

#plt.xlabel("time")
#plt.ylabel("unit")
#legend = plt.legend(loc='best', shadow=True)
#plt.title('----')
#plt.show()
