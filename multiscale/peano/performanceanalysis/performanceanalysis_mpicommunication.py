import pylab
import re
import networkx 


import performanceanalysis_global_plotter


#
# Creates the one big trace picture where we see what different ranks do at 
# different times
#
def plotMPIPhases(numberOfRanks,inputFileName,fileName,upscaling):
  ColorInsideTree                  = "#00ff00"
  ColorReceiveDataFromWorker       = "#ff0000"
  ColorReceiveDataFromMaster       = "#660000"
  ColorReleaseSynchronousHeapData  = "#0066ff"
  ColorReleaseAsynchronousHeapData = "#000066"
  ColorReleaseJoinData             = "#ffff00"
  ColorReleaseBoundaryData         = "#666600"
  
  pylab.clf()
  DefaultSize = pylab.gcf().get_size_inches()
  pylab.gcf().set_size_inches( DefaultSize[0]*4*upscaling, DefaultSize[1]*upscaling )
  pylab.title( "MPI trace of activities" )
  ax = pylab.gca()
  
  timeStampPattern = "([0-9]+\.?[0-9]*)"
  floatPattern = "([0-9]\.?[0-9]*)"
  
  beginIterationPattern              = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::beginIteration"
  leaveCentralElementPattern         = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::leaveCentralElementOfEnclosingSpacetree.*t_central-tree-traversal=\(" + floatPattern
  receiveDataFromWorkerPattern       = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endToReceiveDataFromWorker.* for " + floatPattern
  receiveDataFromMasterPattern       = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endToReceiveDataFromMaster.* for " + floatPattern
  releaseSynchronousHeapDataPattern  = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endToReleaseSynchronousHeapData.*time=" + floatPattern
  releaseAsynchronousHeapDataPattern = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endToPrepareAsynchronousHeapDataExchange.*time=" + floatPattern
  releaseJoinDataPattern             = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endReleaseOfJoinData.*time=" + floatPattern
  releaseBoundaryDataPattern         = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endReleaseOfBoundaryData.*time=" + floatPattern

  def plotMPIPhasesBar( rank, start, end, color):
    if end>start and (end-start)*upscaling>=1:
      rect = pylab.Rectangle([start*upscaling,(rank-0.5)*upscaling],(end-start)*upscaling,upscaling,facecolor=color,edgecolor=color,alpha=Alpha)
      ax.add_patch(rect)
  
  Alpha = 0.5
  
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse mpi phases",
    for line in inputFile:
      m = re.search( beginIterationPattern, line )
      if (m):
        rank = int( m.group(2) )
        timeStamp = float( m.group(1) )
        print ".",
        if (rank==0):
          pylab.plot((timeStamp*upscaling, timeStamp*upscaling), (-0.5, (numberOfRanks+1)*upscaling), '--', color="#000000", alpha=Alpha)
        pylab.plot((timeStamp*upscaling, timeStamp*upscaling), ( (rank-0.5)*upscaling, (rank+0.5)*upscaling), '-', color="#000000" )
      m = re.search( leaveCentralElementPattern, line )
      if (m):
        timeStamp = float( m.group(1) )
        rank      = int( m.group(2) )
        duration  = float( m.group(3) )
        plotMPIPhasesBar(rank,timeStamp-duration,timeStamp,ColorInsideTree)
      m = re.search( receiveDataFromWorkerPattern, line )
      if (m):
        timeStamp = float( m.group(1) )
        rank      = int( m.group(2) )
        duration  = float( m.group(3) )
        plotMPIPhasesBar(rank,timeStamp-duration,timeStamp,ColorReceiveDataFromWorker)
      m = re.search( receiveDataFromMasterPattern, line )
      if (m):
        timeStamp = float( m.group(1) )
        rank      = int( m.group(2) )
        duration  = float( m.group(3) )
        plotMPIPhasesBar(rank,timeStamp-duration,timeStamp,ColorReceiveDataFromMaster)
      m = re.search( releaseSynchronousHeapDataPattern, line )
      if (m):
        timeStamp = float( m.group(1) )
        rank      = int( m.group(2) )
        duration  = float( m.group(3) )
        plotMPIPhasesBar(rank,timeStamp-duration,timeStamp,ColorReleaseSynchronousHeapData)
      m = re.search( releaseAsynchronousHeapDataPattern, line )
      if (m):
        timeStamp = float( m.group(1) )
        rank      = int( m.group(2) )
        duration  = float( m.group(3) )
        plotMPIPhasesBar(rank,timeStamp-duration,timeStamp,ColorReleaseAsynchronousHeapData)
      m = re.search( releaseJoinDataPattern, line )
      if (m):
        timeStamp = float( m.group(1) )
        rank      = int( m.group(2) )
        duration  = float( m.group(3) )
        plotMPIPhasesBar(rank,timeStamp-duration,timeStamp,ColorReleaseJoinData)
      m = re.search( releaseBoundaryDataPattern, line )
      if (m):
        timeStamp = float( m.group(1) )
        rank      = int( m.group(2) )
        duration  = float( m.group(3) )
        plotMPIPhasesBar(rank,timeStamp-duration,timeStamp,ColorReleaseBoundaryData)
        
        

    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  ax.invert_yaxis()
  ax.autoscale_view()
  pylab.xlabel('t')
  pylab.grid(False)
  try:
    pylab.savefig( fileName + ".png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( fileName + ".pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
    pylab.gcf().set_size_inches( DefaultSize[0]*4*10, DefaultSize[1]*10 )
    if numberOfRanks<=16:
      pylab.yticks([i for i in range(0,numberOfRanks)]) 
    else:
      pylab.yticks([i*16 for i in range(0,numberOfRanks/16)]) 
    pylab.savefig( fileName + ".large.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( fileName + ".large.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  except:
    print "ERROR: failed to generated large-scale plot"

  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )



#
# Use these pairs also for sender and receiver
#
class Pair:
  def __init__(self, master, worker):
    self.master         = master
    self.worker         = worker
    self.count          = 0
    self.maxTime        = 0.0
    self.averageTime        = 0

  

def plotLateBoundaries(numberOfRanks,inputFileName,outputFileName):
  pairs = dict()
  
  floatPattern = "([0-9]\.?[0-9]*)"
  
  totalMaxTime     = 0
  totalCount       = 0
  totalAverageTime = 0
  vertexBoundaryWaitPattern = "rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::dataWasNotReceivedInBackground.*rank had to wait for.* record(s) from (\d+)"

  print "search for late boundary data exchange ...",
  try:
    inputFile = open( inputFileName,  "r" )
    for line in inputFile:
      m = re.search( vertexBoundaryWaitPattern, line )
      if (m):
        rank      = int(m.group(1))
        neighbour = int(m.group(2))
        key = (master, worker)
        
        if not pairs.has_key(key):
          pairs[key] = Pair(master, worker)
        pair = pairs[key]
        
        cardinality = int(m.group(1))
        pair.count += 1
        totalCount += 1 

    print "late boundary data analysis found " + str( len(pairs) ) + " entries of interest in trace file" 
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  graph              = networkx.DiGraph()
  for master in range(0,numberOfRanks):
    for worker in range(0,numberOfRanks):
      key = (master, worker)
      if pairs.has_key(key):
        pair = pairs[key]
        if pair.count>2:
          graph.add_edge(str(worker),str(master))
        
  DefaultSize = pylab.gcf().get_size_inches()
              
  pylab.clf()
  pylab.title( "Late boundaries" )
  performanceanalysis_global_plotter.drawTreeGraph(graph)
  pylab.savefig( outputFileName + ".png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  try:
    pylab.gcf().set_size_inches( DefaultSize[0]*10, DefaultSize[1]*10 )
    pylab.savefig( outputFileName + ".large.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( outputFileName + ".large.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  except:
    print "ERROR: failed to generated large-scale plot"
  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
  
   
     
def plotLateMasters(numberOfRanks,inputFileName,outputFileName):
  pairs = dict()
  
  floatPattern = "([0-9]\.?[0-9]*)"
  
  totalMaxTime     = 0
  totalCount       = 0
  totalAverageTime = 0
  waitPattern      = "rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endToReceiveDataFromMaster.*rank had to wait for master (\d+) for " + floatPattern

  print "search for late workers ...",
  try:
    inputFile = open( inputFileName,  "r" )
    for line in inputFile:
      m = re.search( waitPattern, line )
      if (m):
        worker   = int(m.group(1))
        master   = int(m.group(2))
        duration = float(m.group(3))
        key = (master, worker)
        
        if not pairs.has_key(key):
          pairs[key] = Pair(master, worker)
        pair = pairs[key]
        
        cardinality = int(m.group(1))
        pair.count += 1
        totalCount += 1 
     
        pair.maxTime = max(pair.maxTime, duration)
        totalMaxTime = max(totalMaxTime, duration)

        pair.averageTime += duration
        totalAverageTime += duration

    print "late master analysis found " + str( len(pairs) ) + " entries of interest in trace file" 
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  graph              = networkx.DiGraph()
  sparseAverageGraph = networkx.DiGraph()
  sparseMaxGraph     = networkx.DiGraph()
  for master in range(0,numberOfRanks):
    for worker in range(0,numberOfRanks):
      key = (master, worker)
      if pairs.has_key(key):
        pair = pairs[key]
        if pair.count>2:
          graph.add_edge(str(worker),str(master))
          #edge = pydot.Edge(str(worker),str(master), label="(" + str(pair.count) + "," + str(pair.maxTime) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
        
        if totalCount > 0 and pair.count>2 and (float(pair.averageTime) / float(pair.count) > float(totalAverageTime) / float(totalCount)):
          sparseAverageGraph.add_edge(str(worker),str(master))
          if master==0:
            print "detected very strong MPI synchronisation"
          #edge = pydot.Edge(str(worker),str(master), label="(" + str(pair.count) + "," + str(pair.maxTime) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
 
        if pair.count>2 and ( float(pair.maxTime) > 0.9 * float(totalMaxTime) ):
          sparseMaxGraph.add_edge(str(worker),str(master))
          if master==0:
            print "detected very strong MPI synchronisation"
        
        
  DefaultSize = pylab.gcf().get_size_inches()
              
  pylab.clf()
  pylab.title( "Late masters" )
  performanceanalysis_global_plotter.drawTreeGraph(graph)
  pylab.savefig( outputFileName + ".plain.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".plain.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  try:
    pylab.gcf().set_size_inches( DefaultSize[0]*10, DefaultSize[1]*10 )
    pylab.savefig( outputFileName + ".plain.large.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( outputFileName + ".plain.large.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  except:
    print "ERROR: failed to generated large-scale plot"
  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
  
  pylab.clf()
  pylab.title( "Late masters (only more than average weight)" )
  performanceanalysis_global_plotter.drawTreeGraph(sparseAverageGraph)
  pylab.savefig( outputFileName + ".average.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".average.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  try:
    pylab.gcf().set_size_inches( DefaultSize[0]*10, DefaultSize[1]*10 )
    pylab.savefig( outputFileName + ".average.large.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( outputFileName + ".average.large.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  except:
    print "ERROR: failed to generated large-scale plot"
  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
  
  pylab.clf()
  pylab.title( "Late masters (max 10%)" )
  performanceanalysis_global_plotter.drawTreeGraph(sparseMaxGraph)
  pylab.savefig( outputFileName + ".max.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".max.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  try:
    pylab.gcf().set_size_inches( DefaultSize[0]*10, DefaultSize[1]*10 )
    pylab.savefig( outputFileName + ".max.large.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( outputFileName + ".max.large.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  except:
    print "ERROR: failed to generated large-scale plot"
  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
  

  
  
def plotLateWorkers(numberOfRanks,inputFileName,outputFileName):
  pairs = dict()
  
  floatPattern = "([0-9]\.?[0-9]*)"
  
  totalMaxTime     = 0
  totalCount       = 0
  totalAverageTime = 0
  waitPattern      = "rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endToReceiveDataFromWorker.*rank had to wait for worker (\d+) for " + floatPattern

  print "search for late workers ...",
  try:
    inputFile = open( inputFileName,  "r" )
    for line in inputFile:
      m = re.search( waitPattern, line )
      if (m):
        master   = int(m.group(1))
        worker   = int(m.group(2))
        duration = float(m.group(3))
        key = (master, worker)
        
        if not pairs.has_key(key):
          pairs[key] = Pair(master, worker)
        pair = pairs[key]
        
        cardinality = int(m.group(1))
        pair.count += 1
        totalCount += 1 
     
        pair.maxTime = max(pair.maxTime, duration)
        totalMaxTime = max(totalMaxTime, duration)

        pair.averageTime += duration
        totalAverageTime += duration

    print "late worker analysis found " + str( len(pairs) ) + " entries of interest in trace file" 
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  graph              = networkx.DiGraph()
  sparseAverageGraph = networkx.DiGraph()
  sparseMaxGraph     = networkx.DiGraph()
  for master in range(0,numberOfRanks):
    for worker in range(0,numberOfRanks):
      key = (master, worker)
      if pairs.has_key(key):
        pair = pairs[key]
        if pair.count>2:
          graph.add_edge(str(worker),str(master))
          #edge = pydot.Edge(str(worker),str(master), label="(" + str(pair.count) + "," + str(pair.maxTime) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
        
        if totalCount > 0 and pair.count>2 and (float(pair.averageTime) / float(pair.count) > float(totalAverageTime) / float(totalCount)):
          sparseAverageGraph.add_edge(str(worker),str(master))
          if master==0:
            print "detected very strong MPI synchronisation"
          #edge = pydot.Edge(str(worker),str(master), label="(" + str(pair.count) + "," + str(pair.maxTime) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
 
        if pair.count>2 and ( float(pair.maxTime) > 0.9 * float(totalMaxTime) ):
          sparseMaxGraph.add_edge(str(worker),str(master))
          if master==0:
            print "detected very strong MPI synchronisation"
        
        
  DefaultSize = pylab.gcf().get_size_inches()
              
  pylab.clf()
  pylab.title( "Late workers" )
  performanceanalysis_global_plotter.drawTreeGraph(graph)
  pylab.savefig( outputFileName + ".plain.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".plain.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  try:
    pylab.gcf().set_size_inches( DefaultSize[0]*10, DefaultSize[1]*10 )
    pylab.savefig( outputFileName + ".plain.large.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( outputFileName + ".plain.large.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  except:
    print "ERROR: failed to generated large-scale plot"
  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
  
  pylab.clf()
  pylab.title( "Late workers (only more than average weight)" )
  performanceanalysis_global_plotter.drawTreeGraph(sparseAverageGraph)
  pylab.savefig( outputFileName + ".average.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".average.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  try:
    pylab.gcf().set_size_inches( DefaultSize[0]*10, DefaultSize[1]*10 )
    pylab.savefig( outputFileName + ".average.large.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( outputFileName + ".average.large.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  except:
    print "ERROR: failed to generated large-scale plot"
  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
  
  pylab.clf()
  pylab.title( "Late workers (max 10%)" )
  performanceanalysis_global_plotter.drawTreeGraph(sparseMaxGraph)
  pylab.savefig( outputFileName + ".max.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".max.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  try:
    pylab.gcf().set_size_inches( DefaultSize[0]*10, DefaultSize[1]*10 )
    pylab.savefig( outputFileName + ".max.large.png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
    pylab.savefig( outputFileName + ".max.large.pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
  except:
    print "ERROR: failed to generated large-scale plot"
  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
  
  
  
  
  
  