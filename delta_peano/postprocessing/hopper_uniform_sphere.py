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
listRefCenterOfMass, listInertia, listInverse, listOrientation = getParticleData(0, getParticleCount(), 10000)

#print(listParticleId)

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
#print(len(listDiameter))
#print(N)
x = np.array([np.array(i[0]-0.5) for i in listCentre])
y = np.array([np.array(i[1]) for i in listCentre])

minx = min(x)
maxx = max(x)
width = abs(minx-maxx)
miny = min(y)
maxy = max(y)
height = abs(miny-maxy)

midpointX = maxx-width/2
midpointY = maxy-height/2

counter = 0
distanceLeft = []
distanceRight = []
for i in x:
    if i < midpointX:
        distanceLeft.append(midpointX - i)
    else:
        distanceRight.append(midpointX - i)
    counter += 1

colors = np.random.rand(N)
area = np.array([np.array(i*100000) for i in listDiameter])  # 0 to 15 point radii

#print(area)

####EXPECTED value for particle center
IEx = sum(x)/N
print("IEx: %f" % IEx)

IEarray = [a*b for a,b in zip(x,listMass)]
IEmass = sum(IEarray)/sum(listMass)
print("IEm: %f" % IEmass)

####WIDTH value of pile
Mx = abs(max([abs(IEx-i) for i in x]))
print("Mx: %f" % Mx)

xm = [a*b for a,b in zip(listMass, x)]
Mm = max([abs(IEmass-i) for i in xm])
print("Mm: %f" % Mm)

#####Variance value of pile
Varx = sum([pow(i-IEx,2) for i in x])/N
print("Varx: %f" % Varx)

xm = [a*b for a,b in zip(listMass, x)]
Varmass = sum([pow(i-IEmass, 2) for i in xm])/N
print("Varm: %f" % Varmass)

#####PLOT PILE
plt.scatter(x, y, s=area, c=colors, alpha=0.5)
plt.show()



