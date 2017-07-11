import math
import readParticleLog
import readSimulationLog


def getPropertyData(particleStart, particleEnd, filename):
    listparticleId, \
    listmass, \
    listdiameter, \
    listinfluenceRadius, \
    listepsilon, \
    listhMin = readParticleLog.readParticlePropertiesLog(filename)

    listFilteredParticleId = []
    listFilteredmass = []
    listFiltereddiameter = []
    listFilteredinfluenceRadius = []
    listFilteredepsilon = []
    listFilteredhMin = []

    for i in range(0, len(listparticleId)):
        if particleStart <= int(listparticleId[i]) <= particleEnd:
            listFilteredParticleId.append(listparticleId[i])
            listFilteredmass.append(listmass[i])
            listFiltereddiameter.append(listdiameter[i])
            listFilteredinfluenceRadius.append(listdiameter[i])
            listFilteredepsilon.append(listepsilon[i])
            listFilteredhMin.append(listhMin[i])

    return  listFilteredParticleId, \
            listFilteredmass, \
            listFiltereddiameter, \
            listFilteredinfluenceRadius, \
            listFilteredepsilon, \
            listFilteredhMin


def getMinData(particleStart, particleEnd, timestepRequested, filename):

    listparticleId, \
    listlinear, \
    listangular, \
    listcOfMas = readParticleLog.readMinParticleLog(filename)

    listFilteredParticleId = []
    listFilteredlinear = []
    listFilteredAngular = []
    listFilteredcOfMas = []

    it = math.floor(timestepRequested/50)

    print("Step Requested: " + str(timestepRequested) + " Step closest tracked: " + str(it))
    particleCount, obstacleCount, triangleCount = readSimulationLog.readSimCreatedParticlesLog(filename)
    print("Number of Particles: " + str(particleCount))

    fromI = int(it*particleCount)
    toI = int((it+1)*particleCount)

    for i in range(fromI, toI):
        if particleStart <= int(listparticleId[i]) <= particleEnd:
            listFilteredParticleId.append(listparticleId[i])
            listFilteredlinear.append(listlinear[i])
            listFilteredAngular.append(listangular[i])
            listFilteredcOfMas.append(listcOfMas[i])

    return  listFilteredParticleId, \
            listFilteredlinear, \
            listFilteredAngular, \
            listFilteredcOfMas, \

