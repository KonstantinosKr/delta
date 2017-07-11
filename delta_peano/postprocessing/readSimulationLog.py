"""
Konstantinos Krestenitis 2017
Delta Project

post processing script for parsing meta log of delta particles.
"""

def readSimCreatedParticlesLog(filename):
    for line in open(filename, "r"):
        if "dem::mappings::CreateGrid::endIteration(State)          created" in line:
            particles = int(line.split("created ")[1].split("particles")[0])
            obstacles = int(line.split("created ")[1].split("with")[1].split("obstacles")[0])
            triangles = int(line.split("created ")[1].split("obstacles")[1].split("triangles")[0])
            return particles, obstacles, triangles
    return 0, 0, 0

def readSimulationLog(filename):
    iteration = []
    reassigns = []
    parCmp = []
    triCmp = []
    cnpt = []

    for line in open(filename, "r"):
        if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
            iteration.append(line.split(",")[0].split("=")[1])
            reassigns.append(line.split(",")[1].split("=")[1])
            parCmp.append(line.split(",")[2].split("=")[1])
            triCmp.append(line.split(",")[3].split("=")[1])
            cnpt.append(line.split(",")[4].split("=")[1])
            #print("iteration " + line.split(",")[0].split("=")[1])
            continue

    return  iteration, \
            reassigns, \
            parCmp, \
            triCmp, \
            cnpt

def getSimulationData(startStep, endStep, filename):
    iteration = []
    reassigns = []
    parCmp = []
    triCmp = []
    cnpt = []

    for line in open(filename, "r"):
        if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
            if startStep <= line.split(",")[0].split("=")[1] <= endStep:
                iteration.append(line.split(",")[0].split("=")[1])
                reassigns.append(line.split(",")[1].split("=")[1])
                parCmp.append(line.split(",")[2].split("=")[1])
                triCmp.append(line.split(",")[3].split("=")[1])
                cnpt.append(line.split(",")[4].split("=")[1])
                #print("iteration " + line.split(",")[0].split("=")[1])
            continue

    return  iteration, \
            reassigns, \
            parCmp, \
            triCmp, \
            cnpt