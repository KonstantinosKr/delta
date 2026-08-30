def computeVolumesOverlapsWork(numberOfRanks,parents,offset,volume,dim,domainoffset,domainsize):
  print "compute volumes, overlaps and work ",
  volumes = [0.0 for x in offset]
  for i in range(0,numberOfRanks):
    print ".",
    if dim==2:
      volumes[i] = volume[i][0]*volume[i][1]
    else:
      volumes[i] = volume[i][0]*volume[i][1]*volume[i][2]

  overlaps = [0.0 for x in offset]
  for i in range(0,numberOfRanks):
    print ".",
    overlaps[i] = 1.0
    for d in range(0,dim):
      left  = 0
      right = 0
      try:
       left  = max(offset[i][d],float(domainoffset[d]))
       right = min(offset[i][d]+volume[i][d],float(domainoffset[d])+float(domainsize[d]))
      except:
       pass
      delta = right-left
      if delta<0:
        print "ERROR for rank " + str(i) +  \
              ": region=" + str(offset[i]) + "x" + str(volume[i]) + \
              ", bonding box=" + str(domainoffset) + "x" + str(domainsize) + \
              ", delta= " + str(delta) 
      overlaps[i] = overlaps[i] * delta
    if overlaps[i]>volume[i]:
      print "ERROR for rank " + str(i) +  \
            ": region=" + str(offset[i]) + "x" + str(volume[i]) + \
            ", bounding box=" + str(domainoffset) + "x" + str(domainsize) + \
            ", volume= " + str(volumes[i]) + ", overlaps=" + str(overlaps[i]) 
  work = [x for x in overlaps]
  for i in range(1,numberOfRanks):
    if work[ parents[i] ]<overlaps[i] and i==1:
      print "INFO: global root seems to delegate all work to another rank and to focus on load balancing. Decrement rank count when plotting performance graphs accordingly." 
    elif work[ parents[i] ]<overlaps[i] and parents[i]>0:
      print "\nWARNING: work of rank " + str(parents[i]) + " will become negative as overlap of rank " + str(i) + " equals " + str(overlaps[i]) 
    work[ parents[i] ] = work[ parents[i] ] - overlaps[i]
  #work[0] = 0.0     # can become negative
  for i in range(0,numberOfRanks):
    if work[i] <0:
      work[i]=0

  if len(volumes)>1:
    volumes[0]=0
    volumes[1]=0
      
  print " done "
  return (volumes,overlaps,work)
  
  
  
