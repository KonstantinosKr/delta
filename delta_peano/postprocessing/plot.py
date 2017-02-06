import readLog
import matplotlib.pyplot as plt
import numpy as np

#particle properties
#contact properties
#force properties

listparticleId, listmass, listdiameter, listinfluenceRadius, listepsilon, listhMin, \
listnoOfTriangles, listisObstacle, listmaterial, listlinear, listangular, listrangular, \
listcentre, listcOfMas, listrcOfMa, listiner, listinve, listorie, \
listcontactId, listmasterId, listSlaveId, listhasFriction, listdistance, listdepth, listepsilonTotal, \
listxContactPosition, listnormalX, listp, listq, \
listforceId, listforcemasterParticleNo, listforceslaveParticleNo, listmassA, listmassB, listforce, listfriction, \
listsubContactid, listsubDamper, listsubSpring, listsubrelativeVelocity, listsubdepth, listsubspring, listsubtotalForce, listsubdamp, listsubcontactmass, \
particleXaxis, contactXaxis, forceXaxis, subforceXaxis = readLog.readLog()

for i in range(0, len(listforce)):
    print (listforce[i][1])

fig, ax = plt.subplots()
ax.plot(particleXaxis, listparticleId, label='--', linestyle='-', marker='o', markersize=10, markevery=1)

print(listmassB)

plt.xlabel("time")
plt.ylabel("unit")
legend = plt.legend(loc='best', shadow=True)
plt.title('----')
plt.show()