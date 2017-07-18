def removeMinObstacles(obstacles,
                   listParticleMinId,
                   listLinear,
                   listAngular,
                   listCenterOfMass,
                   listParticlePropertyId,
                   listMass,
                   listDiameter,
                   listInfluenceRadius,
                   listEpsilon,
                   listhMin):

    for i, val in enumerate(listParticleMinId):
        if val < obstacles:
            del listLinear[i]
            del listAngular[i]
            del listCenterOfMass[i]

    for i, val in enumerate(listParticlePropertyId):
        if val < obstacles:
            del listMass[i]
            del listDiameter[i]
            del listInfluenceRadius[i]
            del listEpsilon[i]
            del listhMin[i]

    return listParticleMinId, \
           listLinear, \
           listAngular, \
           listCenterOfMass, \
           listParticlePropertyId, \
           listMass, \
           listDiameter, \
           listInfluenceRadius, \
           listEpsilon, \
           listhMin


def removeObstacles(listIsObstacle,
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
                    listOrientation):
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