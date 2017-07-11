import particleData
import readSimulationLog
import symmetry as sym

filename = "../hopper/nonunimesh100/nonuniformMesh.log"

step = 0

particleCount, obstacleCount, triangleCount = readSimulationLog.readSimCreatedParticlesLog(filename)

listParticlePropertyId, \
listMass, \
listDiameter, \
listInfluenceRadius, \
listEpsilon, \
listhMin = particleData.getPropertyData(obstacleCount, particleCount, filename)

listParticleMinId, \
listLinear, \
listAngular, \
listCenterOfMass = particleData.getMinData(obstacleCount, particleCount, step, filename)

centrePoint = 0.5
sym.symmetryPlot(listMass, listDiameter, listCenterOfMass, centrePoint)
