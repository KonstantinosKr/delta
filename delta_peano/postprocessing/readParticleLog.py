"""
Konstantinos Krestenitis 2017
Delta Project

post processing script for parsing meta log of delta particles.
"""

def readParticlePropertiesLog(filename):
    listparticleId = []
    listmass = []
    listdiameter = []
    listinfluenceRadius = []
    listepsilon = []
    listhMin = []

    ind = 0
    state = ""
    for line in open(filename, "r"):
        if "#####PARTICLE-INIT-PROPERTIES-DATA#####" in line:
            ind = 0
            state = "particleprop"
            continue

        if "particleprop" in state:

            ind += 1
            segline = line.split(",")

            col1 = float(segline[0].split("=")[1])
            col2 = float(segline[1].split("=")[1])
            col3 = float(segline[2].split("=")[1])

            if ind == 1:
                listparticleId.append(col1)
                listmass.append(col2)
                listdiameter.append(col3)
            elif ind == 2:
                listinfluenceRadius.append(col1)
                listepsilon.append(col2)
                listhMin.append(col3)
                state = ""

        if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
            if int(line.split(",")[0].split("=")[1]) >= 1:
                break
            continue

    return  listparticleId, \
            listmass, \
            listdiameter, \
            listinfluenceRadius, \
            listepsilon, \
            listhMin

def readMinParticleLog(filename):
    listparticleId = []
    listlinear = []
    listangular = []
    listcOfMas = []

    ind = 0

    state = ""
    for line in open(filename, "r"):

        if "#####PARTICLE-DATA#####" in line:
            ind = 0
            state = "particlemin"
            continue

        if "particlemin" in state:
            ind += 1

            segline = line.split(",")
            if ind == 1:
                col1 = float(segline[0].split("=")[1])
                listparticleId.append(col1)
            elif ind == 2:
                col1 = float(segline[0].split("=")[1])
                col2 = float(segline[1].split("=")[1])
                col3 = float(segline[2].split("=")[1])

                listlinear.append([col1, col2, col3])
            elif ind == 3:
                col1 = float(segline[0].split("=")[1])
                col2 = float(segline[1].split("=")[1])
                col3 = float(segline[2].split("=")[1])

                listangular.append([col1, col2, col3])
            elif ind == 4:
                col1 = float(segline[0].split("=")[1])
                col2 = float(segline[1].split("=")[1])
                col3 = float(segline[2].split("=")[1])

                listcOfMas.append([col1, col2, col3])
            if ind == 4:
                state = ""

    return  listparticleId, \
            listlinear, \
            listangular, \
            listcOfMas


