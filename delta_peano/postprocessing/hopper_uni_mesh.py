from store import *
import numpy as np
import symmetry as sym
import process as pr

getSimSteps()
getPlotSteps()
getParticleCount()
getListOfUniqueParticleIds()

filename = "../hopper/unimesh100/uniformMesh.log"

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