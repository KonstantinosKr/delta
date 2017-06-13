"""
Konstantinos Krestenitis 2017
Delta Project

post processing script for parsing meta log of delta particles.
"""

listparticleId = []
listmass = []
listdiameter = []
listinfluenceRadius = []
listepsilon = []
listhMin = []
listnoOfTriangles = []
listisObstacle = []
listmaterial = []
listlinear = []
listangular = []
listrangular =[]
listcentre = []
listcOfMas = []
listrcOfMa = []
listiner = []
listinve = []
listorie = []

iner = [float(0) for x in range(9)]
inve = [float(0) for x in range(9)]
orie = [float(0) for x in range(9)]

def splitParticlePROPLinePivot(readLine, index):

    segline = readLine.split(",")

    col1 = float(segline[0].split("=")[1])
    col2 = float(segline[1].split("=")[1])
    col3 = float(segline[2].split("=")[1])
    #print str(col1) + " " + str(col2) + " " + str(col3)

    if index == 1:
        particleId = col1
        mass = col2
        diameter = col3

        listparticleId.append(particleId)
        listmass.append(mass)
        listdiameter.append(diameter)
    elif index == 2:
        influenceRadius = col1
        epsilon = col2
        hMin = col3

        listinfluenceRadius.append(influenceRadius)
        listepsilon.append(epsilon)
        listhMin.append(hMin)
    elif index == 3:
        noOfTriangles = col1
        isObstacle = col2
        material = col3

        listnoOfTriangles.append(noOfTriangles)
        listisObstacle.append(isObstacle)
        listmaterial.append(material)


def splitParticleDATAFULLLinePivot(readLine, index):

    segline = readLine.split(",")

    col1 = float(segline[0].split("=")[1])
    col2 = float(segline[1].split("=")[1])
    col3 = float(segline[2].split("=")[1])

    #print str(col1) + " " + str(col2) + " " + str(col3)

    if index == 1:
        listlinear.append([col1, col2, col3])
    elif index == 2:
        listangular.append([col1, col2, col3])
    elif index == 3:
        listrangular.append([col1, col2, col3])
    elif index == 4:
        listcentre.append([col1, col2, col3])
    elif index == 5:
        listcOfMas.append([col1, col2, col3])
    elif index == 6:
        listrcOfMa.append([col1, col2, col3])
    elif index == 7:
        iner[0] = col1
        iner[1] = col2
        iner[2] = col3
    elif index == 8:
        iner[3] = col1
        iner[4] = col2
        iner[5] = col3
    elif index == 9:
        iner[6] = col1
        iner[7] = col2
        iner[8] = col3
    elif index == 10:
        inve[0] = col1
        inve[1] = col2
        inve[2] = col3
    elif index == 11:
        inve[3] = col1
        inve[4] = col2
        inve[5] = col3
    elif index == 12:
        inve[6] = col1
        inve[7] = col2
        inve[8] = col3
    elif index == 13:
        orie[0] = col1
        orie[1] = col2
        orie[2] = col3
    elif index == 14:
        orie[3] = col1
        orie[4] = col2
        orie[5] = col3
    elif index == 15:
        orie[6] = col1
        orie[7] = col2
        orie[8] = col3

        listiner.append(iner)
        listinve.append(inve)
        listorie.append(orie)


def splitParticleDATALinePivot(readLine, index):

    segline = readLine.split(",")

    col1 = float(segline[0].split("=")[1])
    col2 = float(segline[1].split("=")[1])
    col3 = float(segline[2].split("=")[1])

    #print str(col1) + " " + str(col2) + " " + str(col3)

    if index == 1:
        particleId = col1
        listparticleId.append(particleId)
    elif index == 2:
        listlinear.append([col1, col2, col3])
    elif index == 3:
        listangular.append([col1, col2, col3])
    elif index == 4:
        listcOfMas.append([col1, col2, col3])
        listrangular.append([0, 0, 0])
        listcentre.append([0, 0, 0])
        listrcOfMa.append([0, 0, 0])
        iner[0] = 0
        iner[1] = 0
        iner[2] = 0
        iner[3] = 0
        iner[4] = 0
        iner[5] = 0
        iner[6] = 0
        iner[7] = 0
        iner[8] = 0
        inve[0] = 0
        inve[1] = 0
        inve[2] = 0
        inve[3] = 0
        inve[4] = 0
        inve[5] = 0
        inve[6] = 0
        inve[7] = 0
        inve[8] = 0
        orie[0] = 0
        orie[1] = 0
        orie[2] = 0
        orie[3] = 0
        orie[4] = 0
        orie[5] = 0
        orie[6] = 9
        orie[7] = 0
        orie[8] = 0

        listiner.append(iner)
        listinve.append(inve)
        listorie.append(orie)


listcontactId = []
listmasterId = []
listSlaveId = []
listhasFriction = []
listdistance = []
listdepth = []
listepsilonTotal = []

listsubDamper = []
listsubSpring = []

listsubvij = []
listsubrelativeVelocity = []
listsubdepth = []
listsubspring_depth = []

listsubtotalForce = []
listsubdamp = []
listsubcontactmass = []

listxContactPosition = []
listnormalX = []
listfriction = []
listmassA = []
listmassB = []
listforce = []
listtorque = []

listp = []
listq = []

def splitContactLinePivot(readLine, index):

    segline = readLine.split(",")
    #print(segline)
    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]

    #print(str(col1) + " " + str(col2) + " " + str(col3))

    if index == 1:
        contactId = col1
        MasterId = col2
        SlaveId = col3

        listcontactId.append(contactId)
        listmasterId.append(MasterId)
        listSlaveId.append(SlaveId)
    elif index == 2:
        hasFriction = col1
        massA = col2
        massB = col3

        listhasFriction.append(hasFriction)
        listmassA.append(massA)
        listmassB.append(massB)
    elif index == 3:
        distance = col1
        depth = col2
        epsilonTotal = col3

        listdistance.append(distance)
        listdepth.append(depth)
        listepsilonTotal.append(epsilonTotal)
    elif index == 4:
        subcontactid = col1
        subdamper = col2
        subspring = col3

        listsubDamper.append(subdamper)
        listsubSpring.append(subspring)
    elif index == 5:
        listsubvij.append([col1, col2, col3])
    elif index == 6:
        subrelativeV = col1
        subdepth = col2
        subspring_depth = col3

        listsubrelativeVelocity.append(subrelativeV)
        listsubdepth.append(subdepth)
        listsubspring_depth.append(subspring_depth)
    elif index == 7:
        subtotalforce = col1
        subdamp = col2
        subcontactmass = col3

        listsubtotalForce.append(subtotalforce)
        listsubdamp.append(subdamp)
        listsubcontactmass.append(subcontactmass)
    elif index == 8:
        listxContactPosition.append([col1, col2, col3])
    elif index == 9:
        listnormalX.append([col1, col2, col3])
    elif index == 10:
        listfriction.append([col1, col2, col3])
    elif index == 11:
        listforce.append([col1, col2, col3])
    elif index == 12:
        listforce.append([col1, col2, col3])
    elif index == 13:
        listp.append([col1, col2, col3])
    elif index == 14:
        listq.append([col1, col2, col3])


iteration = []
reassigns = []
parCmp = []
triCmp = []
cnpt = []
gridV = []
t = []
dt =[]

particleXaxis = []
contactXaxis = []

def readLog(filename):
    contactXindex = 0
    particleXindex = 0
    subforceXindex = 0
    ind = 0

    state = ""
    for line in open(filename, "r"):

        if "#####PARTICLE-INIT-PROPERTIES-DATA#####" in line:
            ind = 0
            state = "particleprop"
            continue

        if "particleprop" in state:
            ind += 1
            splitParticlePROPLinePivot(line, ind)
            if ind == 3:
                state = ""
                listlinear.append([0, 0, 0])
                listangular.append([0, 0, 0])
                listrangular.append([0, 0, 0])
                listcentre.append([0, 0, 0])
                listcOfMas.append([0, 0, 0])
                listrcOfMa.append([0, 0, 0])
                iner[0] = 0
                iner[1] = 0
                iner[2] = 0
                iner[3] = 0
                iner[4] = 0
                iner[5] = 0
                iner[6] = 0
                iner[7] = 0
                iner[8] = 0
                inve[0] = 0
                inve[1] = 0
                inve[2] = 0
                inve[3] = 0
                inve[4] = 0
                inve[5] = 0
                inve[6] = 0
                inve[7] = 0
                inve[8] = 0
                orie[0] = 0
                orie[1] = 0
                orie[2] = 0
                orie[3] = 0
                orie[4] = 0
                orie[5] = 0
                orie[6] = 9
                orie[7] = 0
                orie[8] = 0

                listiner.append(iner)
                listinve.append(inve)
                listorie.append(orie)


        if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
            #iteration data starts on next line
            #print(line.split(","))
            it = line.split(",")[0].split("=")[1]
            re = line.split(",")[1].split("=")[1]
            pa = line.split(",")[2].split("=")[1]
            ti = line.split(",")[3].split("=")[1]
            cn = line.split(",")[4].split("=")[1]
            iteration.append(it)
            reassigns.append(re)
            parCmp.append(pa)
            triCmp.append(ti)
            cnpt.append(cn)
            #print("iteration " + it)
            continue

        if "#####PARTICLE-DATA#####" in line:
            ind = 0
            state = "particlemin"
            continue

        if "#####PARTICLE-DATAFULL#####" in line:
            ind = 0
            state = "particlefull"
            continue

        if "#####CONTACT-DATA#####" in line:
            ind = 0
            state = "contactplot"
            continue

        if "particlemin" in state:
            #print line
            ind += 1
            splitParticleDATALinePivot(line, ind)
            if ind == 4:
                state = ""
                particleXaxis.append(particleXindex)
                particleXindex += 1

        if "particlefull" in state:
            #print line
            ind += 1
            splitParticleDATAFULLLinePivot(line, ind)
            if ind == 15:
                state = ""
                particleXaxis.append(particleXindex)
                particleXindex += 1

        if "contactplot" in state:
            #print(line)
            ind += 1
            splitContactLinePivot(line, ind)
            if ind == 14:
                state = ""
                contactXaxis.append(contactXindex)
                contactXindex += 1

    return  listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, listnoOfTriangles, \
            listisObstacle,listmaterial, listlinear, listangular, listrangular, listcentre, listcOfMas, \
            listrcOfMa, listiner, listinve, listorie, \
            listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal, \
            listsubDamper, listsubSpring, listsubvij, listsubrelativeVelocity, listsubdepth, listsubspring_depth, listsubtotalForce, listsubdamp, listsubcontactmass, \
            listxContactPosition, listnormalX, listfriction, listmassA, listmassB, listforce, listtorque, listp, listq, \
            particleXaxis, contactXaxis, iteration, reassigns, parCmp, triCmp, cnpt, gridV, t, dt

