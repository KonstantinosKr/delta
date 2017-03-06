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

def splitParticleLinePivot(readLine, index):

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
    elif index == 4:
        listlinear.append([col1, col2, col3])
    elif index == 5:
        listangular.append([col1, col2, col3])
    elif index == 6:
        listrangular.append([col1, col2, col3])
    elif index == 7:
        listcentre.append([col1, col2, col3])
    elif index == 8:
        listcOfMas.append([col1, col2, col3])
    elif index == 9:
        listrcOfMa.append([col1, col2, col3])
    elif index == 10:
        iner[0] = col1
        iner[1] = col2
        iner[2] = col3
    elif index == 11:
        iner[3] = col1
        iner[4] = col2
        iner[5] = col3
    elif index == 12:
        iner[6] = col1
        iner[7] = col2
        iner[8] = col3
    elif index == 13:
        inve[0] = col1
        inve[1] = col2
        inve[2] = col3
    elif index == 14:
        inve[3] = col1
        inve[4] = col2
        inve[5] = col3
    elif index == 15:
        inve[6] = col1
        inve[7] = col2
        inve[8] = col3
    elif index == 16:
        orie[0] = col1
        orie[1] = col2
        orie[2] = col3
    elif index == 17:
        orie[3] = col1
        orie[4] = col2
        orie[5] = col3
    elif index == 18:
        orie[6] = col1
        orie[7] = col2
        orie[8] = col3

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
listxContactPosition = []
listnormalX = []
listp = []
listq = []

def splitContactLinePivot(readLine, index):

    segline = readLine.split(",")
    print(segline)
    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]

    #print str(col1) + " " + str(col2) + " " + str(col3)

    if index == 1:
        contactId = col1
        MasterId = col2
        SlaveId = col3

        listcontactId.append(contactId)
        listmasterId.append(MasterId)
        listdiameter.append(SlaveId)
    elif index == 2:
        hasFriction = col1
        nulldata1 = col2
        nulldata2 = col3

        listhasFriction.append(hasFriction)
    elif index == 3:
        distance = col1
        depth = col2
        epsilonTotal = col3

        listdistance.append(distance)
        listdepth.append(depth)
        listepsilonTotal.append(epsilonTotal)
    elif index == 4:
        listxContactPosition.append([col1, col2, col3])
    elif index == 5:
        listnormalX.append([col1, col2, col3])
    elif index == 6:
        listp.append([col1, col2, col3])
    elif index == 7:
        listq.append([col1, col2, col3])


#####subContact#####\n
listsubContactid = []
listsubDamper = []
listsubSpring = []

listsubrelativeVelocity = []
listsubdepth = []
listsubspring_depth = []

listsubtotalForce = []
listsubdamp = []
listsubcontactmass = []

listforceId = []
listforcemasterParticleNo = []
listforceslaveParticleNo = []
listmassA = []
listmassB = []
listforce = []
listfriction = []

def splitSubContactPivot(readLine, subindex):
    #print(readLine)
    segline = readLine.split(",")
    #print(segline)
    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]

    if subindex == 1:
        subcontactid = col1
        subdamper = col2
        subspring = col3

        listsubContactid.append(subcontactid)
        listsubDamper.append(subdamper)
        listsubSpring.append(subspring)
    elif subindex == 2:
        subrelativeV = col1
        subdepth = col2
        subspring_depth = col3

        listsubrelativeVelocity.append(subrelativeV)
        listsubdepth.append(subdepth)
        listsubspring_depth.append(subspring_depth)
    elif subindex == 3:
        subtotalforce = col1
        subdamp = col2
        subcontactmass = col3

        listsubtotalForce.append(subtotalforce)
        listsubdamp.append(subdamp)
        listsubcontactmass.append(subcontactmass)

def splitForceLinePivot(readLine, index):
    #print(readLine)
    segline = readLine.split(",")
    #print(segline)
    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]

    #print str(col1) + " " + str(col2) + " " + str(col3)

    if index == 1:
        forceId = col1
        masterParticleNo = col2
        slaveParticleNo = col3

        listforceId.append(forceId)
        listforcemasterParticleNo.append(masterParticleNo)
        listforceslaveParticleNo.append(slaveParticleNo)
    elif index == 2:
        massA = col1
        massB = col2
        nulldata3 = col3

        listmassA.append(massA)
        listmassB.append(massB)
    elif index == 3:
        listforce.append([col1, col2, col3])
    elif index == 4:
        listfriction.append([col1, col2, col3])

iteration = []
particleXaxis = []
contactXaxis = []
forceXaxis = []
subforceXaxis = []

def readLog():
    contactXindex = 0
    particleXindex = 0
    forceXindex = 0
    subforceXindex = 0
    it = 0
    ind = 0
    subind = 0

    state = ""
    for line in open("../log.log", "r"):
        if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
            #iteration data starts on next line
            it = line.split(",")[0].split("=")[1]
            iteration.append(it)
            #print("iteration " + it)
            continue

        if "#####PARTICLE-DATA#####" in line:
            ind = 0
            state = "particle"
            continue

        if "#####CONTACT-DATA#####" in line:
            ind = 0
            state = "contact"
            continue

        if "#####FORCE-DATA#####" in line:
            ind = 0
            subind = 0
            #state = "force"
            continue

        if "#####TOTAL-FDATA#####" in line:
            ind = 0
            state = "force"
            continue

        if "start-subContact" in line:
            ind = 0
            subind = 0
            state = "subcontact"
            continue

        if "end-subContact" in line:
            continue

        if "particle" in state:
            #print line
            ind += 1
            splitParticleLinePivot(line, ind)
            if ind == 18:
                state = ""
                particleXaxis.append(particleXindex)
                particleXindex += 1

        if "CONTACT-DATA" in state:
            #print line
            ind += 1
            splitContactLinePivot(line, ind)
            if ind == 7:
                state = ""
                contactXaxis.append(contactXindex)
                contactXindex += 1

        if "subcontact" in state:
            ind += 1
            splitSubContactPivot(line, ind)
            if ind == 3:
                state = ""
                subforceXaxis.append(subforceXindex)
                subforceXindex += 1

        if "force" in state:
            #print line
            ind += 1
            splitForceLinePivot(line, ind)
            if ind == 4:
                state = ""
                forceXaxis.append(forceXindex)
                forceXindex += 1

    return listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, listnoOfTriangles, \
           listisObstacle,listmaterial, listlinear, listangular, listrangular, listcentre, listcOfMas, \
           listrcOfMa, listiner, listinve, listorie, \
           listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal, \
           listxContactPosition, listnormalX, listp, listq, \
           listforceId, listforcemasterParticleNo, listforceslaveParticleNo, listmassA, listmassB, listforce, listfriction, \
            listsubContactid,listsubDamper, listsubSpring, listsubrelativeVelocity, listsubdepth, listsubspring_depth, listsubtotalForce, listsubdamp, listsubcontactmass, \
            particleXaxis, contactXaxis, forceXaxis, subforceXaxis, iteration
