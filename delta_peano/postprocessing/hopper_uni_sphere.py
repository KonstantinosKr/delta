from store import *
import numpy as np
import symmetry as sym
import process as pr

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

"""


getSimSteps()
getPlotSteps()
getParticleCount()
getListOfUniqueParticleIds()

filename = "../hopper/uniSphere100/uniformSphere.log"

listParticleId, \
listMass, \
listDiameter, \
listInfluenceRadius, \
listEpsilon, \
listthMin, \
listNoOfTriangles, \
listIsObstacle, \
listMaterial, \
listLinear, \
listAngular, \
listRefangular, \
listCentre, \
listCenterOfMass, \
listRefCenterOfMass, \
listInertia, \
listInverse, \
listOrientation = getParticleData(0, 2, 0, filename)

pr.removeObstacles(listIsObstacle,
                    listParticleId,
                    listMass,
                    listDiameter,
                    listInfluenceRadius,
                    listEpsilon,
                    listthMin,
                    listNoOfTriangles,
                    listMaterial,
                    listLinear,
                    listAngular,
                    listRefangular,
                    listCentre,
                    listCenterOfMass,
                    listRefCenterOfMass,
                    listInertia,
                    listInverse,
                    listOrientation)

centrePoint = 0.5
sym.symmetryPlot(listMass, listDiameter, listCentre)

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
