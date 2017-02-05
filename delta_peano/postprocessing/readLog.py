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

linear = [0 for x in range(3)]
angular = [0 for x in range(3)]
rangular = [0 for x in range(3)]
centre = [0 for x in range(3)]
cOfMas = [0 for x in range(3)]
rcOfMa = [0 for x in range(3)]
iner = [0 for x in range(9)]
inve = [0 for x in range(9)]
orie = [0 for x in range(9)]

def splitParticleLinePivot(readLine, index):

    segline = readLine.split(",")

    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]

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
        linear[0] = col1
        linear[1] = col2
        linear[2] = col3
    elif index == 5:
        angular[0] = col1
        angular[1] = col2
        angular[2] = col3
    elif index == 6:
        rangular[0] = col1
        rangular[1] = col2
        rangular[2] = col3
    elif index == 7:
        centre[0] = col1
        centre[1] = col2
        centre[2] = col3
    elif index == 8:
        cOfMas[0] = col1
        cOfMas[1] = col2
        cOfMas[2] = col3
    elif index == 9:
        rcOfMa[0] = col1
        rcOfMa[1] = col2
        rcOfMa[2] = col3
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

        # add particle properties to lists
        listlinear.append(linear)
        listangular.append(angular)
        listrangular.append(rangular)
        listcentre.append(centre)
        listcOfMas.append(cOfMas)
        listrcOfMa.append(rcOfMa)
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

xContactPosition = [0 for x in range(3)]
normalX = [0 for x in range(3)]
force = [0 for x in range(3)]
friction = [0 for x in range(3)]
p = [0 for x in range(3)]
q = [0 for x in range(3)]

def splitContactLinePivot(readLine, index):

    segline = readLine.split(",")
    #print segline
    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]

    print str(col1) + " " + str(col2) + " " + str(col3)

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
        xContactPosition[0] = col1
        xContactPosition[1] = col2
        xContactPosition[2] = col3

        listxContactPosition.append(xContactPosition)
    elif index == 5:
        normalX[0] = col1
        normalX[1] = col2
        normalX[2] = col3

        listnormalX.append(normalX)
    elif index == 6:
        p[0] = col1
        p[1] = col2
        p[2] = col3

        listp.append(p)
    elif index == 7:
        q[0] = col1
        q[1] = col2
        q[2] = col3

        listq.append(q)


#####subContact#####\n
listsubContactid = []
listsubDamper = []
listsubSpring = []

listsubrelativeVelocity = []
listsubdepth = []
listsubspring = []

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

f = [0 for x in range(3)]
t = [0 for x in range(3)]

def splitForceLinePivot(readLine, index, subindex):

    segline = readLine.split(",")
    print segline
    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]

    #print str(col1) + " " + str(col2) + " " + str(col3)

    if index == 1:
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
            subspring = col3

            listsubrelativeVelocity.append(subrelativeV)
            listsubdepth.append(subdepth)
            listsubspring.append(subspring)
        elif subindex == 3:
            subtotalforce = col1
            subdamp = col2
            subcontactmass = col3

            listsubtotalForce.append(subtotalforce)
            listsubdamp.append(subdamp)
            listsubcontactmass.append(subcontactmass)

    if index == 2:
        forceId = col1
        masterParticleNo = col2
        slaveParticleNo = col3

        listforceId.append(forceId)
        listforcemasterParticleNo.append(masterParticleNo)
        listforceslaveParticleNo.append(slaveParticleNo)
    elif index == 3:
        massA = col1
        massB = col2
        nulldata3 = col3

        listmassA.append(massA)
        listmassB.append(massB)
    elif index == 4:
        f[0] = col1
        f[1] = col2
        f[2] = col3

        listforce.append(f)
    elif index == 5:
        t[0] = col1
        t[1] = col2
        t[2] = col3
        listfriction.append(t)

iteration = []
particleXaxis = []
contactXaxis = []
forceXaxis = []
subforceXaxis = []

def readLog():
    contactXindex = 0
    particleXindex = 0
    forceXindex = 0
    it = 0
    ind = 0
    subind = 0

    state = ""
    for line in open("../log.log", "r"):
        if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
            #iteration data starts on next line
            it = line.split(",")[0].split("=")[1]
            iteration.append(it)
            print "iteration " + it
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
            state = "force"
            continue

        if "start-subContact" in line:
            ind = 1
            state = "subcontact"

        if "end-subContact" in line:
            ind = 1
            state = "force"
            continue

        if "particle" in state:
            #print line
            ind += 1
            splitParticleLinePivot(line, ind)
            if ind == 18:
                state = ""
                particleXaxis.append(particleXindex)
                particleXindex += 1

        if "contact" in state:
            #print line
            ind += 1
            splitContactLinePivot(line, ind)
            if ind == 7:
                state = ""
                contactXaxis.append(contactXindex)
                contactXindex += 1

        if "force" in state or "subcontact" in state:
            #print line
            if "subcontact" in state:
                subind += 1
            splitForceLinePivot(line, ind, subind)
            if subind == 3:
                state = "force"
                ind += 1
                subind = 0
                subforceXaxis.append(forceXindex)
            if ind == 5:
                state = ""
                forceXaxis.append(forceXindex)
                forceXindex += 1

    return listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, listnoOfTriangles, \
           listisObstacle,listmaterial, listlinear, listangular, listrangular, listcentre, listcOfMas, \
           listrcOfMa, listiner, listinve, listorie, \
           listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal, \
           listxContactPosition, listnormalX, listp, listq, \
           listforceId, listforcemasterParticleNo, listforceslaveParticleNo, listmassA, listmassB, listforce, listfriction, \
            listsubContactid,listsubDamper, listsubSpring, listsubrelativeVelocity, listsubdepth, listsubspring, listsubtotalForce, listsubdamp, listsubcontactmass, \
            particleXaxis, contactXaxis, forceXaxis, subforceXaxis
