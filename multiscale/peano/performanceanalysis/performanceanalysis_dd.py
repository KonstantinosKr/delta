import pylab
import re


from mpl_toolkits.mplot3d import Axes3D



def getNodeColor(c,nodes):
 NodeColors = [ 
  "#ff0000", "#00ff00", "#0000ff", 
  "#ffff00", "#ff00ff", "#00ffff", 
  "#ff6767", "#67ff67", "#6767ff", 
  "#ffff67", "#ff67ff", "#67ffff", 
  "#ababab", 
  "#ab55ab", "#abab55", 
  "#5555ab", "#55ab55", "#ab5555",
  "#cc9966", "#99cc66", "#66cc99",  
  "#cc6699", "#9966cc", "#6699cc"
 ]
 return NodeColors[nodes.index(nodes[c])%len(NodeColors)]
    

BaseFontSize=10
   
    
def plot2dDomainDecompositionOnLevel(l,numberOfRanks,domainoffset,domainsize,offset,volume,levels,nodes,outputFileName):
  print "plot domain decomposition on level " + str(l),
  pylab.clf()
  #pylab.figure(figsize=(float(domainsize[0]),float(domainsize[1])))
  for i in range(0,numberOfRanks):
    if levels[i]==l:
      print ".",
      
      pylab.gca().add_patch(pylab.Rectangle(offset[i]+(volume[i][0]*0.01,volume[i][0]*0.01), volume[i][0]*0.98, volume[i][1]*0.98, color=getNodeColor(i,nodes)))
      
      chosenFontSize = BaseFontSize/(l+1)
      
      pylab.text(
        offset[i][0] + volume[i][0]/2,
        offset[i][1] + volume[i][1]/2,
        str(i), fontsize=chosenFontSize
      )
  print ".",
  pylab.xlim( float(domainoffset[0]), float(domainoffset[0])+float(domainsize[0]) )
  pylab.ylim( float(domainoffset[1]), float(domainoffset[1])+float(domainsize[1]) )
  pylab.savefig( outputFileName + ".level" + str(l) + ".png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".level" + str(l) + ".pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  print "done"
  
     
def plot2dDomainDecomposition(numberOfRanks,domainoffset,domainsize,offset,volume,levels,nodes,outputFileName):
  print "plot domain decomposition ",
  pylab.clf()
  #pylab.figure(figsize=(float(domainsize[0]),float(domainsize[1])))
  for l in range(0,128):
   for i in range(0,numberOfRanks):
    if levels[i]==l:
      print ".",
      
      pylab.gca().add_patch(pylab.Rectangle(offset[i]+(volume[i][0]*0.01,volume[i][0]*0.01), volume[i][0]*0.98, volume[i][1]*0.98, color=getNodeColor(i,nodes)))
  print ".",
  pylab.xlim( float(domainoffset[0]), float(domainoffset[0])+float(domainsize[0]) )
  pylab.ylim( float(domainoffset[1]), float(domainoffset[1])+float(domainsize[1]) )
  pylab.savefig( outputFileName + ".png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  print "done"
    

def plot3dDomainDecompositionOnLevel(l,numberOfRanks,domainoffset,domainsize,offset,volume,levels,nodes,outputFileName):
  print "plot domain decomposition on level " + str(l),
  pylab.clf()
     
  #fig = pylab.figure()
  #ax = fig.add_subplot(111, projection='3d')
  
  NumberOfPartitionsPerPlot = 8
  NumberOfDifferentPlots    = int(round(numberOfRanks/NumberOfPartitionsPerPlot+0.5))
  numberOfPartitionsPlottedInThisSubfigure = 0
  currentSubPlot                           = 0

  DefaultSize = pylab.gcf().get_size_inches()
  pylab.figure(figsize=(float(domainsize[0]),float(domainsize[1])*NumberOfDifferentPlots))
  # The first entry is [1] on purpose!
  pylab.gcf().set_size_inches( DefaultSize[1], DefaultSize[1]*(NumberOfDifferentPlots+1) )
  
  numberOfPartitionsPlottedInThisSubfigure = NumberOfPartitionsPerPlot+1
  ax = pylab.subplot2grid((NumberOfDifferentPlots,1), (0,0), projection='3d')
  try:
   ax.set_xticks([])
   ax.set_yticks([])
   ax.set_zticks([])
  except:
   pass
  for i in range(0,numberOfRanks):
    if (numberOfPartitionsPlottedInThisSubfigure>NumberOfPartitionsPerPlot):
      numberOfPartitionsPlottedInThisSubfigure = 0
      try:
       ax = pylab.subplot2grid((NumberOfDifferentPlots,1), (currentSubPlot,0), projection='3d')
       ax.set_xlim3d( float(domainoffset[0]), float(domainoffset[0])+float(domainsize[0]) )
       ax.set_ylim3d( float(domainoffset[1]), float(domainoffset[1])+float(domainsize[1]) )
       ax.set_zlim3d( float(domainoffset[2]), float(domainoffset[2])+float(domainsize[2]) )
      except:
       pass
      currentSubPlot  = currentSubPlot + 1
   
    if levels[i]==l:
      print ".",
  
      myColor      = getNodeColor(i,nodes)
      
      NumberOfLines = 20
      for xCount in range(0,NumberOfLines):
        x = offset[i][0] + volume[i][0] * xCount / NumberOfLines 
        ax.plot3D([x,x],[offset[i][1]+volume[i][1], offset[i][1]], [offset[i][2]+volume[i][2]/2,offset[i][2]+volume[i][2]/2],color=myColor)
      ## bars along z
      ax.plot3D([offset[i][0],             offset[i][0]],             [offset[i][1], offset[i][1]], [offset[i][2],offset[i][2]+volume[i][2]],color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0]+volume[i][0],offset[i][0]+volume[i][0]],[offset[i][1], offset[i][1]], [offset[i][2],offset[i][2]+volume[i][2]],color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0],             offset[i][0]],             [offset[i][1]+volume[i][1], offset[i][1]+volume[i][1]], [offset[i][2],offset[i][2]+volume[i][2]],color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0]+volume[i][0],offset[i][0]+volume[i][0]],[offset[i][1]+volume[i][1], offset[i][1]+volume[i][1]], [offset[i][2],offset[i][2]+volume[i][2]],color=myColor, linewidth=2.0)
      ## bars along x
      ax.plot3D([offset[i][0],     offset[i][0]+volume[i][0]], [offset[i][1],              offset[i][1]],              [offset[i][2],              offset[i][2]],              color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0],     offset[i][0]+volume[i][0]], [offset[i][1],              offset[i][1]],              [offset[i][2]+volume[i][2], offset[i][2]+volume[i][2]], color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0],     offset[i][0]+volume[i][0]], [offset[i][1]+volume[i][1], offset[i][1]+volume[i][1]], [offset[i][2],              offset[i][2]],              color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0],     offset[i][0]+volume[i][0]], [offset[i][1]+volume[i][1], offset[i][1]+volume[i][1]], [offset[i][2]+volume[i][2], offset[i][2]+volume[i][2]], color=myColor, linewidth=2.0)
      ## bars along y
      ax.plot3D([offset[i][0],              offset[i][0]],              [offset[i][1],  offset[i][1]+volume[i][1]], [offset[i][2],              offset[i][2]],              color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0],              offset[i][0]],              [offset[i][1],  offset[i][1]+volume[i][1]], [offset[i][2]+volume[i][2], offset[i][2]+volume[i][2]], color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0]+volume[i][0], offset[i][0]+volume[i][0]], [offset[i][1],  offset[i][1]+volume[i][1]], [offset[i][2],              offset[i][2]],              color=myColor, linewidth=2.0)
      ax.plot3D([offset[i][0]+volume[i][0], offset[i][0]+volume[i][0]], [offset[i][1],  offset[i][1]+volume[i][1]], [offset[i][2]+volume[i][2], offset[i][2]+volume[i][2]], color=myColor, linewidth=2.0)

      chosenFontSize = BaseFontSize/(l+1)
      ax.text(offset[i][0]+volume[i][0]/2, offset[i][1]+volume[i][1]/2, offset[i][2]+volume[i][2]/2, str(i), "y", fontsize=chosenFontSize)      

      numberOfPartitionsPlottedInThisSubfigure = numberOfPartitionsPlottedInThisSubfigure+1      
  
  pylab.savefig( outputFileName + ".level" + str(l) + ".png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".level" + str(l) + ".pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  print "done"
    
  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
    
        

def printNodeTable(outputFile,numberOfRanks,parents,nodes):
 outputFile.write( "<table border=\"1\">" )

 outputFile.write( "<tr>" )
 outputFile.write( "<td><b>Rank (master)</b></td>" )
 outputFile.write( "<td><b>Node</b></td>" )
 outputFile.write( "<td><b>Children (worker)</b></td>" )
 outputFile.write( "</tr>" )

 for nodeNumber in range(0,numberOfRanks):
  outputFile.write( "<tr>" )
  outputFile.write( "<td bgcolor=\"" + getNodeColor(nodeNumber,nodes) + "\">" + str(nodeNumber) + "</td>" )
  outputFile.write( "<td bgcolor=\"" + getNodeColor(nodeNumber,nodes) + "\">" + nodes[nodeNumber] + "</td>" )
  for c in range(0,numberOfRanks):
   if parents[c]==nodeNumber:
     outputFile.write( "<td bgcolor=\"" + getNodeColor(c,nodes) + "\">->" + str(c) + "</td>" )
  outputFile.write( "</tr>" )
 outputFile.write( "</table>" )

 
def extractForkHistory(outFile,inputFileName,numberOfRanks):
  outFile.write( "<table border=\"1\">" )

  histogram        = []
  lastParentForked = 0
  
  outFile.write( "<tr><td><b>Step\\Rank</b></td>" )
  for i in range(0,numberOfRanks):
    outFile.write( "<td><i>" + str(i) + "</td>" )
  outFile.write( "</tr>" )

 
  histogramLevelForks = []
  histogramLevelJoins = []
  forksPerRank        = [0 for a in range(0,numberOfRanks)]
  joinsPerRank        = [0 for a in range(0,numberOfRanks)]
  
  outFile.write( "<tr>" )
  outFile.write( "<td><b>0</b></td>" )
  currentStep         = 1
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse forks/join history",
    for line in inputFile:
      searchPatternAddFork    = "peano::performanceanalysis::DefaultAnalyser::addWorker.*\d+->\d+\+\d+"
      searchPatternAddJoin    = "peano::performanceanalysis::DefaultAnalyser::removeWorker.*\d+\+\d+->d+"
      searchEndIteration      = "rank:0.*peano::performanceanalysis::DefaultAnalyser::endIteration"
      if ("DefaultAnalyser" in line):
        m = re.search( searchPatternAddFork, line )
        if (m):
          #
          # parse
          #
          parent = int(m.group(0).split("->")[0].split(" ")[-1]) 
          child  = int(m.group(0).split("+")[-1].split(" ")[-1])
          level  = line.split("level:")[1].split("]")[0]

          if parent<lastParentForked:
            outFile.write( "</tr>" )
            outFile.write( "<tr>" )
            lastParentForked = -1
          while lastParentForked<parent:
            outFile.write( "<td />" )
            lastParentForked = lastParentForked + 1
          lastParentForked = lastParentForked + 1
          outFile.write( "<td>" + str(parent) + "->" + str(parent) + "+" + str(child) + " (level=" + level + ")</td>" )        
          #while len(histogram)<=int(level):
          #  histogram.append( 0 )
          #histogram[int(level)] = histogram[int(level)] + 1   
          #lastParentForked = parent
          #
          # find right column in table
          #
          #if (lastColumnWrittenTo>parent):
          #  outFile.write( "</td></tr><tr>" )
          #  lastColumnWrittenTo=0
          #for i in range(lastColumnWrittenTo,parent):
          #  outFile.write( "</td>" )
          #  outFile.write( "<td>" )        
          #lastColumnWrittenTo = parent
          #
          # write entry
          #
          #outFile.write( str(parent) + "->" + str(parent) + "+" + str(child) + " (level=" + level + ")<br />" )
          #
          # update statistics
          #        
          while len(histogramLevelForks)<=int(level):
            histogramLevelForks.append( 0 )
            histogramLevelJoins.append( 0 )
          #while len(forksPerRank)<=int(parent):
          #  forksPerRank[parent] = forksPerRank[parent]+1
          histogramLevelForks[int(level)] = histogramLevelForks[int(level)] + 1
          forksPerRank[parent]            = forksPerRank[parent] + 1
        m = re.search( searchPatternAddJoin, line )
        if (m):
          print "not written yet"
          quit()
        m = re.search( searchEndIteration, line )
        if (m and lastParentForked>0):
          outFile.write( "</tr><tr>" )
          outFile.write( "<td><b>" + str(currentStep) + "</b></td>" )
          currentStep      = currentStep + 1
          lastParentForked = 0
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  outFile.write( "</tr>" )

  outFile.write( "<tr><td><b>no of forks:</b></td>" )
  for i in range(0,numberOfRanks):
    outFile.write( "<td><i>" + str(forksPerRank[i]) + "</i></td>" )
  outFile.write( "</tr>" )
  outFile.write( "<tr><td><b>no of joins:</b></td>" )
  for i in range(0,numberOfRanks):
    outFile.write( "<td><i>" + str(joinsPerRank[i]) + "</i></td>" )
  outFile.write( "</tr>" )
  outFile.write( "</table>" )
  
  
  outFile.write( "<h3>Histograms:</h3>" )
  outFile.write( "<table border=\"1\">" )
  outFile.write( "<tr><td><b>Level</b></td><td><b>Number of forks</b></td><td><b>Number of joins</b></td></tr>" )
  for i in range(1,len(histogramLevelForks)):
    outFile.write( "<tr><td>"  + str(i) + "</td>" )
    if histogramLevelForks[i]>0:
      outFile.write( "<td bgcolor=\"#aaaaFF\">"  + str(histogramLevelForks[i]) )
    else:
      outFile.write( "<td bgcolor=\"#FF0000\">"  + str(histogramLevelForks[i]) )
    if histogramLevelJoins[i]==0:
      outFile.write( "<td bgcolor=\"#aaaaFF\">"  + str(histogramLevelJoins[i]) )
    else:
      outFile.write( "<td bgcolor=\"#00FF00\">"  + str(histogramLevelJoins[i]) )
    outFile.write( "</td></tr>" )
  outFile.write( "</table>" )
 
