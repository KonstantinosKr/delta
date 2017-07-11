"""
Konstantinos Krestenitis 2017
Delta Project

post processing script for parsing meta log of delta particles.
"""

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

def splitContactLine(readLine, index):
    segline = readLine.split(",")
    col1 = segline[0].split("=")[1]
    col2 = segline[1].split("=")[1]
    col3 = segline[2].split("=")[1]
    #print(str(col1) + " " + str(col2) + " " + str(col3))

    if index == 1:
        listcontactId.append(col1)
        listmasterId.append(col2)
        listSlaveId.append(col3)
    elif index == 2:
        listhasFriction.append(col1)
        listmassA.append(col2)
        listmassB.append(col3)
    elif index == 3:
        listdistance.append(col1)
        listdepth.append(col2)
        listepsilonTotal.append(col3)
    elif index == 4:
        listsubDamper.append(col2)
        listsubSpring.append(col3)
    elif index == 5:
        listsubvij.append([col1, col2, col3])
    elif index == 6:
        listsubrelativeVelocity.append(col1)
        listsubdepth.append(col2)
        listsubspring_depth.append(col3)
    elif index == 7:
        listsubtotalForce.append(col1)
        listsubdamp.append(col2)
        listsubcontactmass.append(col3)
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

particleXaxis = []
contactXaxis = []

def readLog(filename):
    contactXindex = 0
    particleXindex = 0
    ind = 0

    state = ""
    for line in open(filename, "r"):
        if "dem::runners::Runner::runAsMaster(...)                  i=" in line:
            iteration.append(line.split(",")[0].split("=")[1])
            reassigns.append(line.split(",")[1].split("=")[1])
            parCmp.append(line.split(",")[2].split("=")[1])
            triCmp.append(line.split(",")[3].split("=")[1])
            cnpt.append(line.split(",")[4].split("=")[1])
            #print("iteration " + line.split(",")[0].split("=")[1])
            continue

        if "#####CONTACT-DATA#####" in line:
            ind = 0
            state = "contactplot"
            continue

        if "contactplot" in state:
            ind += 1
            splitContactLine(line, ind)
            if ind == 14:
                state = ""
                contactXaxis.append(contactXindex)
                contactXindex += 1

    return  listcontactId, \
            listmasterId, \
            listSlaveId, \
            listhasFriction, \
            listdistance, \
            listdepth, \
            listepsilonTotal, \
            listsubDamper, \
            listsubSpring, \
            listsubvij, \
            listsubrelativeVelocity, \
            listsubdepth, \
            listsubspring_depth, \
            listsubtotalForce, \
            listsubdamp, \
            listsubcontactmass, \
            listxContactPosition, \
            listnormalX, \
            listfriction, \
            listmassA, \
            listmassB, \
            listforce, \
            listtorque, \
            listp, \
            listq, \
            particleXaxis, \
            contactXaxis, \
            iteration, \
            reassigns, \
            parCmp, \
            triCmp, \
            cnpt
