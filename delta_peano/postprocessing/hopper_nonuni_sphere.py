from store import *
import numpy as np
import symmetry as sym
import process as pr

filename = "../hopper/uniSphere100/nonuniformSphere.log"

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

print(getSimSteps())
getPlotSteps()
getParticleCount()
getListOfUniqueParticleIds()

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