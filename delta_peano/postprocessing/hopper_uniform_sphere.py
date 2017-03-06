from plotDataMethods import *
import matplotlib.pyplot as plt
import numpy as np



"""
getSimSteps()
getPlotSteps()
getParticleCount()
getListOfUniqueParticleIds()

listParticleId, listMass, listDiameter, \
listInfluenceRadius, listEpsilon, listthMin, listNoOfTriangles, \
listIsObstacle, listMaterial, listLinear, listAngular, \
listRefangular, listCentre, listCenterOfMass, \
listRefCenterOfMass, listInertia, listInverse, listOrientation = getParticleData(particleIndex)

listParticleId, listMass, listDiameter, \
listInfluenceRadius, listEpsilon, listthMin, listNoOfTriangles, \
listIsObstacle, listMaterial, listLinear, listAngular, \
listRefangular, listCentre, listCenterOfMass, \
listRefCenterOfMass, listInertia, listInverse, listOrientation = getParticleData(particleIndexStart, particleIndexEnd, timestep)

listContactId, listMasterId, listSlaveId, listHasFriction, \
listDistance, listDepth, listEpsilon, listContactPosition, \
listNormalX, listP, listQ = getContactData(masterIndex, slaveIndex, interchangeable)

listForceId, listForceMaster, listForceSlave, listMassA, \
listMassB, listForce, listFriction = getForce(masterIndex, slaveIndex, interchangeable)

listSubContactId, listSubDamper, listSubSpring, listSubRelativeVelocity, \
listSubDepth, listSubSpring_Depth, listSubTotalForce, listSubDamp, listSubContactMass = getContactDataOfInteraction(masterIndex, slaveIndex, interchangeable)

"""


getSimSteps()
getPlotSteps()
getParticleCount()
getListOfUniqueParticleIds()


listParticleId, listMass, listDiameter, \
listInfluenceRadius, listEpsilon, listthMin, listNoOfTriangles, \
listIsObstacle, listMaterial, listLinear, listAngular, \
listRefangular, listCentre, listCenterOfMass, \
listRefCenterOfMass, listInertia, listInverse, listOrientation = getParticleData(0, getParticleCount(), 10500)

print(listParticleId)

counter = 0
for i in listIsObstacle:
    if i == 1:
        del listParticleId[counter]
        del listMass[counter]
        del listDiameter[counter]
        del listInfluenceRadius[counter]
        del listEpsilon[counter]
        del listthMin[counter]
        del listNoOfTriangles[counter]
        del listIsObstacle[counter]
        del listMaterial[counter]
        del listLinear[counter]
        del listAngular[counter]
        del listRefangular[counter]
        del listCentre[counter]
        del listCenterOfMass[counter]
        del listRefCenterOfMass[counter]
        del listInertia[counter]
        del listInverse[counter]
        del listOrientation[counter]
    counter += 1

N = len(listParticleId)
print(len(listDiameter))
print(N)
x = np.array([np.array(i[0]) for i in listCentre])
y = np.array([np.array(i[1]) for i in listCentre])

minx = min(x)
maxx = max(x)
width = minx-max
miny = min(y)
maxy = max(y)
height = miny-maxy

midpointX = maxx-width/2
midpointY = maxy-height/2

counter = 0
for i in x:
    if i < midpointX:
        left = listParticleId[counter]
    else:
        right = listParticleId[counter]
    counter += 1

distanceLeft = []
for i in left:
    distanceLeft.append(midpointX-x[i])

distanceRight = []
for i in right:
    distanceRight.append(midpointX-x[i])


colors = np.random.rand(N)
area = np.array([np.array(i*100000) for i in listDiameter])  # 0 to 15 point radii

print(area)

plt.scatter(x, y, s=area, c=colors, alpha=0.5)
plt.show()