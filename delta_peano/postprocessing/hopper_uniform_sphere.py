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
listRefCenterOfMass, listInertia, listInverse, listOrientation = getParticleData(0, getParticleCount(), 45000)
#400 - iteration 4800
'''
IEx: -0.000011
IEm: -0.000011
Mx: 0.082187
Mm: 0.000016
Varx: 0.001331
Varm: 0.000000
'''
#200 - iteration 4000
'''
IEx: -0.000581
IEm: -0.000581
Mx: 0.092234
Mm: 0.000593
Varx: 0.000860
Varm: 0.000000

'''
#100 - iteration 4000
'''
IEx: -0.000234
IEm: -0.000234
Mx: 0.059945
Mm: 0.000249
Varx: 0.000763
Varm: 0.000000
'''
#50 - iteration 4000
'''
IEx: 0.000027
IEm: 0.000027
Mx: 0.049429
Mm: 0.000052
Varx: 0.000485
Varm: 0.000000


#NONUNIFORM

#100 - iteration 45000
IEx: 0.001757
IEm: 0.000561
Mx: 0.096956
Mm: 0.000644
Varx: 0.001516
Varm: 0.000000


#UNIFORM Mesh 10

#50 - iteration 45000
IEx: -0.003801
IEm: -0.004195
Mx: 0.045487
Mm: 0.004280
Varx: 0.000552
Varm: 0.000018

'''

idx = []
for i, val in enumerate(listIsObstacle):
    if val == 1:
        idx.append(i)

for i in idx[::-1]:
    print(i)
    del listParticleId[i]
    del listMass[i]
    del listDiameter[i]
    del listInfluenceRadius[i]
    del listEpsilon[i]
    del listthMin[i]
    del listNoOfTriangles[i]
    del listMaterial[i]
    del listLinear[i]
    del listAngular[i]
    del listRefangular[i]
    del listCentre[i]
    del listCenterOfMass[i]
    del listRefCenterOfMass[i]
    del listInertia[i]
    del listInverse[i]
    del listOrientation[i]
    del listIsObstacle[i]

N = len(listParticleId)
print(N)

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



