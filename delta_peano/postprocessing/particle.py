"""
Konstantinos Krestenitis 2017
Delta Project

post processing script for parsing meta log of delta particles.
"""

filepath = open("../log", "r")

index = 0

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

particleId = 0
mass = 0
diameter = 0
influenceRadius = 0
epsilon = 0
hMin = 0
noOfTriangles = 0
isObstacle = 0
material = 0

linear = [0 for x in range(3)]
angular = [0 for x in range(3)]
rangular = [0 for x in range(3)]
centre = [0 for x in range(3)]
cOfMas = [0 for x in range(3)]
rcOfMa = [0 for x in range(3)]
iner = [0 for x in range(9)]
inve = [0 for x in range(9)]
orie = [0 for x in range(9)]

trigger = "aux"

for line in open(filepath, "r"):

    print line
    if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
        #iteration data starts on next line
        index = 0
        continue
    index += 1

    segline = line.split(",")

    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]

    print
    str(col1) + " " + str(col2) + " " + str(col3)

    if index == 1:
        particleId = col1
        mass = col2
        diameter = col3
    elif index == 2:
        influenceRadius = col1
        epsilon = col2
        hMin = col3
    elif index == 3:
        noOfTriangles = col1
        isObstacle = col2
        material = col3
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

        listparticleId.append(particleId)
        listmass.append(mass)
        listdiameter.append(diameter)
        listinfluenceRadius.append(influenceRadius)
        listepsilon.append(epsilon)
        listhMin.append(hMin)
        listnoOfTriangles.append(noOfTriangles)
        listisObstacle.append(isObstacle)
        listmaterial.append(material)
        listlinear.append(linear)
        listangular.append(angular)
        listrangular.append(rangular)
        listcentre.append(centre)
        listcOfMas.append(cOfMas)
        listrcOfMa.append(rcOfMa)
        listiner.append(iner)
        listinve.append(inve)
        listorie.append(orie)

    if "#####PARTICLE-DATA#####" in line:
        #particle-data follows

    if "#####CONTACT-DATA#####" in line:
    # contact-Data follows
    if "#####FORCE-DATA#####" in line:
        #force data follows

    if "dem::mappings::Plot" in line:





