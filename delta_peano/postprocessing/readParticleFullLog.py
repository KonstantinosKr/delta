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

def parseParticleFullLine(readLine, index):
    segline = readLine.split(",")

    col1 = float(segline[0].split("=")[1])
    col2 = float(segline[1].split("=")[1])
    col3 = float(segline[2].split("=")[1])

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

iteration = []
reassigns = []
parCmp = []
triCmp = []
cnpt = []

particleXaxis = []
contactXaxis = []

def readLog(filename):
    contactXindex = 0
    particleXindex = 0
    ind = 0

    state = ""
    for line in open(filename, "r"):

        if "#####PARTICLE-INIT-PROPERTIES-DATA#####" in line:
            ind = 0
            state = "particleprop"
            continue

        if "particleprop" in state:
            ind += 1
            parseParticlePropertiesLine(line, ind)
            if ind == 2:
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

                listnoOfTriangles.append(0)
                listisObstacle.append(0)
                listmaterial.append(0)

                listiner.append(iner)
                listinve.append(inve)
                listorie.append(orie)

                listparticleId.append(0)
                listmass.append(0)
                listdiameter.append(0)
                listinfluenceRadius.append(0)
                listepsilon.append(0)
                listhMin.append(0)
                listnoOfTriangles.append(0)
                listisObstacle.append(0)
                listmaterial.append(0)
                listlinear.append(0)
                listangular.append(0)
                listrangular.append(0)
                listcentre.append(0)
                listcOfMas.append(0)
                listrcOfMa.append(0)
                listiner.append(0)
                listinve.append(0)
                listorie.append(0)


        if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
            iteration.append(line.split(",")[0].split("=")[1])
            reassigns.append(line.split(",")[1].split("=")[1])
            parCmp.append(line.split(",")[2].split("=")[1])
            triCmp.append(line.split(",")[3].split("=")[1])
            cnpt.append(line.split(",")[4].split("=")[1])
            #print("iteration " + line.split(",")[0].split("=")[1])
            continue

        ################################################################
        ################################################################

        if "#####PARTICLE-DATA#####" in line:
            ind = 0
            state = "particlemin"
            continue

        if "#####PARTICLE-DATAFULL#####" in line:
            ind = 0
            state = "particlefull"
            continue

        ################################################################
        ################################################################

        if "particlefull" in state:
            ind += 1
            parseParticleFullLine(line, ind)
            if ind == 15:
                state = ""
                particleXaxis.append(particleXindex)
                particleXindex += 1

    return  listparticleId, \
            listmass, \
            listdiameter, \
            listinfluenceRadius, \
            listepsilon, \
            listhMin, \
            listnoOfTriangles, \
            listisObstacle,\
            listmaterial, \
            listlinear, \
            listangular, \
            listrangular, \
            listcentre, \
            listcOfMas, \
            listrcOfMa, \
            listiner, \
            listinve, \
            listorie, \
            particleXaxis, \
            contactXaxis, \
            iteration, \
            reassigns, \
            parCmp, \
            triCmp, \
            cnpt
