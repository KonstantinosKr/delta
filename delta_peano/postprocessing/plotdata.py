import readLog
import matplotlib.pyplot as plt
import numpy as np

#PARTICLE PROPERTIES - SNAPPED PER PLOT - axis - particleXaxis
#listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin,
#listnoOfTriangles, listisObstacle, listmaterial, listlinear, listangular, listrangular,
#listcentre, listcOfMas, listrcOfMa, listiner, listinve, listorie

#CONTACT PROPERTIES - SNAPPED PER PLOT AND PER CONTACT EVENT - axis - contactXaxis
#listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal,
#listxContactPosition, listnormalX, listp, listq

#FORCE PROPERTIES between two PARTICLES - axis - forceXaxis
#listforceId, listforcemasterParticleNo, listforceslaveParticleNo, listmassA, listmassB, listforce, listfriction,

#CONTACTs PROPERTIES of TOTAL FORCE between two PARTICLES - axis - subforceXaxis
#listsubContactid, listsubDamper, listsubSpring, listsubrelativeVelocity, listsubdepth, listsubspring, listsubtotalForce, listsubdamp, listsubcontactmass

listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, \
listnoOfTriangles, listisObstacle, listmaterial, listlinear, listangular, listrangular, \
listcentre, listcOfMas, listrcOfMa, listiner, listinve, listorie, \
listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal, \
listxContactPosition, listnormalX, listp, listq, \
listforceId, listforcemasterParticleNo, listforceslaveParticleNo, listmassA, listmassB, listforce, listfriction, \
listsubContactid, listsubDamper, listsubSpring, listsubrelativeVelocity, listsubdepth, listsubspring, listsubtotalForce, listsubdamp, listsubcontactmass, \
particleXaxis, contactXaxis, forceXaxis, subforceXaxis = readLog.readLog()

def getSimSteps():
    return


def getPlotSteps():
    count = 0
    for i in len(particleXaxis):
        count += 1
    return count

def getParticleCount():
    return particleXaxis/getPlotSteps()


print(getParticleCount())

def getParticleData(particleIndex):
    listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, \
    listnoOfTriangles, listisObstacle, listmaterial, listlinear, listangular, listrangular, \
    listcentre, listcOfMas, listrcOfMa, listiner, listinve, listorie,
    #for i in range(0, getPlotSteps()):
    #    for j in range(0, getParticleCount()):
            #if particleIndex == listparticleId:

    return 0

"""
def getContactData(masterIndex, slaveIndex, interchangeable):

    return


def getForce(masterIndex, slaveIndex):

    return


def getContactDataOfInteraction(masterIndex, slaveIndex, interchangeable):

    return
"""