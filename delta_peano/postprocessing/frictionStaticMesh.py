from storeDataMethods import *
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

listContactId, listMasterId, listSlaveId, listHasFriction, \
listDistance, listDepth, listEpsilon, listContactPosition, \
listNormalX, listP, listQ = getContactData(particleIndex)
"""

getSimSteps()
checkpoints = getPlotSteps()
iterations = getSteps()
getParticleCount()
print(getListOfUniqueParticleIds())
particleIndex=1

listParticleId, listMass, listDiameter, \
listInfluenceRadius, listEpsilon, listthMin, listNoOfTriangles, \
listIsObstacle, listMaterial, listLinear, listAngular, \
listRefangular, listCentre, listCenterOfMass, \
listRefCenterOfMass, listInertia, listInverse, listOrientation = getParticleData(particleIndex)

reassigns = getReassigns()
parCmp = getParComparisons()
triCmp = getTriComparisons()
cnpt = getContactPoints()


#kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);
#rotational = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);

print(checkpoints)
x = np.array([np.array(i) for i in range(0, int(checkpoints))])

#linear energy
y = np.array([np.array((velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2])) for velocity in listLinear])
for counter, mass in enumerate(listMass):
    y[counter] = mass*y[counter]

#rotational energy
#y = np.array([np.array(0.0) for i in range(0, len(listAngular))])
#for counter, angular in enumerate(listAngular):
#    print(counter)
#    y[counter] = 0.5 * listInertia[counter][0]*(angular[0]*angular[0])+0.5*listInertia[counter][4]*(angular[1]*angular[1])+0.5*listInertia[counter][4]*(angular[2]*angular[2])

plt.scatter(x, y)
plt.show()



