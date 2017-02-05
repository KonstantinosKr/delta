import readLog

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
    print listforce[i][1]
