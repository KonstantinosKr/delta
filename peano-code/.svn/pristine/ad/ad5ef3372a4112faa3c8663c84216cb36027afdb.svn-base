import sys
import re
import matplotlib
matplotlib.use('Agg') # Must be before importing matplotlib.pyplot or pylab!
import pylab
import networkx 




searchPatternNumberOfCells         = "peano::performanceanalysis::DefaultAnalyser::endIteration.*cells=\("
searchPatternNumberOfLocalVertices = "peano::performanceanalysis::DefaultAnalyser::endIteration.*local-vertices="
searchPatternTTotal                = "([0-9]\.?[0-9]*).*peano::performanceanalysis::DefaultAnalyser::endIteration.*t_total=\("
searchPatternTTraversal            = "peano::performanceanalysis::DefaultAnalyser::endIteration.*t_traversal=\("



numberOfInnerLeafCells  = {}
numberOfInnerCells      = {}
numberOfOuterLeafCells  = {}
numberOfOuterCells      = {}
numberOfLocalCells      = {}
numberOfLocalVertices   = {}
tTotal                  = {}
tTraversal              = {}

  
  
#
# Display constants. Might be reset later
#
AlphaValue = 1.0


GlobalSynchronisationOnRank0 = False



#
# Use these pairs also for sender and receiver
#
class Pair:
  def __init__(self, master, worker):
    self.master         = master
    self.worker         = worker
    self.count          = 0
    self.maxTime        = 0.0
    self.average        = 0
    self.maxCardinality = 0


def switchToLargePlot():
  DefaultSize = pylab.gcf().get_size_inches()
  pylab.gcf().set_size_inches( (DefaultSize[0]*10, DefaultSize[1]*10) )


def switchBackToStandardPlot():
  DefaultSize = pylab.gcf().get_size_inches()
  pylab.gcf().set_size_inches( (DefaultSize[0]/10, DefaultSize[1]/10) )


def drawTreeGraph(myGraph):
  try:
    pos=networkx.graphviz_layout(myGraph,prog='twopi',args='')
    #pos=networkx.graphviz_layout(myGraph,prog='dot',args='')
  except:
    pos=networkx.spring_layout(myGraph)
  networkx.draw(
    myGraph,
    pos,
    with_labels=True,
    node_color='#667766',
    node_size=10,
    alpha=0.2
  )


def parseInputFile():
  print "prepare data structures",
  
  myNumberOfRanks = numberOfRanks
  if myNumberOfRanks==0:
    myNumberOfRanks = 1
  for rank in range(0,myNumberOfRanks):
    numberOfInnerLeafCells[rank]  = []
    numberOfInnerCells[rank]      = []
    numberOfOuterLeafCells[rank]  = []
    numberOfOuterCells[rank]      = []
    numberOfLocalCells[rank]      = []
    numberOfLocalVertices[rank]   = []
    tTotal[rank]                  = []
    tTraversal[rank]              = []
    print ".",
  print " done"
   
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse ",
    for line in inputFile:
        if ("DefaultAnalyser" in line):
          m = re.search( searchPatternNumberOfCells, line )
          if (m):
            rank  = 0
            if numberOfRanks>0:
              rank  = int(line.split( "rank:" )[-1].split( " " )[0])
            token = line.replace("(",",").replace(")","").strip().split(",")
            numberOfInnerLeafCells[rank].append(float(token[-5]))
            numberOfOuterLeafCells[rank].append(float(token[-4]))
            numberOfInnerCells[rank].append(float(token[-3]))
            numberOfOuterCells[rank].append(float(token[-2]))
            numberOfLocalCells[rank].append(float(token[-1]))
            print ".",
          m = re.search( searchPatternNumberOfLocalVertices, line )
          if (m):
            rank  = 0
            if numberOfRanks>0:
              rank  = int(line.split( "rank:" )[-1].split( " " )[0])
            token = line.strip().split("=")
            numberOfLocalVertices[rank].append(float(token[-1]))
            print ".",
          m = re.search( searchPatternTTotal, line )
          if (m):
            rank  = 0
            if numberOfRanks>0:
              rank  = int(line.split( "rank:" )[-1].split( " " )[0])
            timeStamp = float( m.group(1) )
            tTotal[rank].append(timeStamp)
            print ".",
          m = re.search( searchPatternTTraversal, line )
          if (m):
            rank  = 0
            if numberOfRanks>0:
              rank  = int(line.split( "rank:" )[-1].split( " " )[0])
            token = line.strip().split("=(")[-1].split(",")[0]
            tTraversal[rank].append(float(token))
            print ".",
          
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst



def plotConcurrency(rank):
  pylab.clf()
  #pylab.title( "Concurrency levels" )

  ax = pylab.gca()
  #ax.set_aspect('equal','box')
  
  timeStampPattern = "([0-9]\.?[0-9]*)"
  floatPattern = "([0-9]\.?[0-9]*)"
  
  searchPattern              = timeStampPattern + ".*rank:" + str(rank) + " .*peano::performanceanalysis::DefaultAnalyser::changeConcurrencyLevel" + \
                               ".*dt=" + floatPattern + \
                               ".*cpu-time=" + floatPattern + \
                               ".*concurrent-time=" + floatPattern + \
                               ".*potential-concurrent-time=" + floatPattern + \
                               ".*max-concurrency-level=" + floatPattern + \
                               ".*max-potential-concurrency-level=" + floatPattern + \
                               ".*background-tasks=" + floatPattern

  lastTimeStamp = 0
  
  
  maxPotentialConcurrencyLevelLastPoint = 0
  potentialConcurrencyTimeLastPoint     = 0
  realConcurrencyLastPoint              = 1
  
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse concurrency level of rank " + str(rank)
    for line in inputFile:
      m = re.search( searchPattern, line )
      if (m):
        timeStamp                      = float( m.group(1) )
        dt                             = float( m.group(2) )
        cpuTime                        = float( m.group(3) ) 
        concurrentTime                 = float( m.group(4) )
        potentialConcurrentTime        = float( m.group(5) ) 
        maxConcurrencyLevel            = float( m.group(6) )
        maxPotentialConcurrencyLevel   = float( m.group(7) ) 
        backgroundTasks                = float( m.group(8) )
        
        
        #print "found dt=" + str(dt) + ", cpuTime=" + str(cpuTime)
         
  
        backgroundTasksBar         = pylab.Rectangle([lastTimeStamp,maxConcurrencyLevel],timeStamp-lastTimeStamp,backgroundTasks,facecolor="#0000ff",edgecolor="#0000ff")
        maxConcurrencyLevelBar     = pylab.Rectangle([lastTimeStamp,0],timeStamp-lastTimeStamp,maxConcurrencyLevel,              facecolor="#ff0000",edgecolor="#ff0000")
        concurrentTimeBar          = pylab.Rectangle([lastTimeStamp,0],timeStamp-lastTimeStamp,concurrentTime/dt,                facecolor="#aa0000",edgecolor="#bb3344")

        maxPotentialConcurrencyLevelSymbol = pylab.plot([lastTimeStamp,timeStamp],[maxPotentialConcurrencyLevelLastPoint, maxPotentialConcurrencyLevel], "-",  color="#00ff00" )
        potentialConcurrencyTimeSymbol     = pylab.plot([lastTimeStamp,timeStamp],[potentialConcurrencyTimeLastPoint,     potentialConcurrentTime/dt]  , ":",  color="#008800" )
        realConcurrencyLevelSymbol         = pylab.plot([lastTimeStamp,timeStamp],[realConcurrencyLastPoint,              cpuTime/dt                ]  , "-",  color="#000000")

        maxPotentialConcurrencyLevelLastPoint = maxPotentialConcurrencyLevel
        potentialConcurrencyTimeLastPoint     = potentialConcurrentTime/dt
        realConcurrencyLastPoint              = cpuTime/dt

        ax.add_patch(backgroundTasksBar)
        ax.add_patch(maxConcurrencyLevelBar)
        ax.add_patch(concurrentTimeBar)

        lastTimeStamp = timeStamp
        print ".",
    pylab.plot([0,timeStamp],[1,1], "--", color="#000000")
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  ax.autoscale_view()
  ax.set_yscale('symlog', basey=2)
  
  pylab.yticks( 
    [1,2,8,12,16,18,24,60,72,120,180,240,480], 
    ["1","2","8","12","16","18","24","60","72","120","180","240","480"] 
  )
  
  pylab.xlabel('t')
  pylab.ylabel('Concurrency level')
  pylab.grid(True)
  pylab.savefig( outputFileName + "-rank-" + str(rank) + ".concurrency.png" )
  pylab.savefig( outputFileName + "-rank-" + str(rank) + ".concurrency.pdf" )
  switchToLargePlot()
  pylab.savefig( outputFileName + "-rank-" + str(rank) + ".concurrency.large.png" )
  pylab.savefig( outputFileName + "-rank-" + str(rank) + ".concurrency.large.pdf" )
  switchBackToStandardPlot()  





def plotMPIPhases():
  inTraversalColor        = "#00ab00"
  beforeInTraversalColor  = "#ab0000"
  afterInTraversalColor   = "#560000"
  afterBoundaryExchange   = "#0000ab"

  pylab.clf()
  pylab.title( "MPI phases overview" )
  ax = pylab.gca()
  ax.set_aspect('equal','box')
  
  timeStampPattern = "([0-9]\.?[0-9]*)"
  floatPattern = "([0-9]\.?[0-9]*)"
  
  beginIterationPattern      = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::beginIteration"
  enterCentralElementPattern = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::enterCentralElementOfEnclosingSpacetree"
  leaveCentralElementPattern = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::leaveCentralElementOfEnclosingSpacetree.*t_central-tree-traversal=\(" + floatPattern
  endIterationPattern        = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endIteration.*t_traversal=\(" + floatPattern
  endDataExchange            = timeStampPattern + ".*rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endReleaseOfBoundaryData"

  lastTimeStamp  = [0] * numberOfRanks
  
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse mpi phases",
    for line in inputFile:
      m = re.search( beginIterationPattern, line )
      if (m):
        rank = int( m.group(2) )
        timeStamp = float( m.group(1) )
        lastTimeStamp[rank] = timeStamp
        print ".",
        if (rank==0):
          pylab.plot((timeStamp, timeStamp), (-0.5, numberOfRanks+1), 'k-')
        
      m = re.search( enterCentralElementPattern, line )
      if (m):
        rank = int( m.group(2) )
        timeStamp = float( m.group(1) )
        if (lastTimeStamp[rank]==0):
          lastTimeStamp[rank] = timeStamp
        rectLength = timeStamp-lastTimeStamp[rank]
        rect = pylab.Rectangle([lastTimeStamp[rank],rank-0.5],rectLength,1,facecolor=beforeInTraversalColor,edgecolor=beforeInTraversalColor)
        ax.add_patch(rect)
        lastTimeStamp[rank] = lastTimeStamp[rank] + rectLength
      m = re.search( leaveCentralElementPattern, line )
      if (m):
        rank = int( m.group(2) )
        timeStamp = float( m.group(1) )
        if (lastTimeStamp[rank]==0):
          lastTimeStamp[rank] = timeStamp
        #rectLength = float( m.group(3) )
        rectLength = timeStamp-lastTimeStamp[rank]
        rect = pylab.Rectangle([lastTimeStamp[rank],rank-0.5],rectLength,1,facecolor=inTraversalColor,edgecolor=inTraversalColor)
        ax.add_patch(rect)
        lastTimeStamp[rank] = lastTimeStamp[rank] + rectLength
      m = re.search( endIterationPattern, line )
      if (m):
        rank = int( m.group(2) )
        timeStamp = float( m.group(1) )
        if (lastTimeStamp[rank]==0):
          lastTimeStamp[rank] = timeStamp
        #rectLength = float( m.group(3) )
        rectLength = timeStamp-lastTimeStamp[rank]
        rect = pylab.Rectangle([lastTimeStamp[rank],rank-0.5],rectLength,1,facecolor=afterInTraversalColor,edgecolor=afterInTraversalColor)
        ax.add_patch(rect)
        lastTimeStamp[rank] = lastTimeStamp[rank] + rectLength
      m = re.search( endDataExchange, line )
      if (m):
        rank = int( m.group(2) )
        timeStamp = float( m.group(1) )
        if (lastTimeStamp[rank]==0):
          lastTimeStamp[rank] = timeStamp
        rectLength = timeStamp-lastTimeStamp[rank]
        rect = pylab.Rectangle([lastTimeStamp[rank],rank-0.5],rectLength,1,facecolor=afterBoundaryExchange,edgecolor=afterBoundaryExchange)
        ax.add_patch(rect)
        lastTimeStamp[rank] = timeStamp

    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  ax.invert_yaxis()
  ax.autoscale_view()
  pylab.xlabel('t')
  pylab.grid(True)
  pylab.savefig( outputFileName + ".mpi-phases.png" )
  pylab.savefig( outputFileName + ".mpi-phases.pdf" )
  switchToLargePlot()
  pylab.yticks([i for i in range(0,numberOfRanks)]) 
  pylab.savefig( outputFileName + ".mpi-phases.large.png" )
  pylab.savefig( outputFileName + ".mpi-phases.large.pdf" )
  switchBackToStandardPlot()  



def plotForkJoinStatistics():
  numberOfWorkingNodes = []
  numberOfIdleNodes    = []
  numberOfForks        = []
  numberOfJoins        = []
  timelineOfWorkingNodes = []
  timelineOfIdleNodes    = []
  timelineOfForks        = []
  timelineOfJoins        = []
  
  numberOfWorkingNodes.append(1)
  numberOfIdleNodes.append(numberOfRanks-1)
  numberOfForks.append(0)
  numberOfJoins.append(0)
  timelineOfWorkingNodes.append(0.0)
  timelineOfIdleNodes.append(0.0)
  timelineOfForks.append(0.0)
  timelineOfJoins.append(0.0)
  
  timeStampPattern  = "([0-9]\.?[0-9]*)"
  
  searchPatternFork    = timeStampPattern + ".*peano::performanceanalysis::DefaultAnalyser::addWorker"
  searchPatternJoin    = timeStampPattern + ".*peano::performanceanalysis::DefaultAnalyser::removeWorker"
  
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse fork/join statistics",
    for line in inputFile:
      m = re.search( searchPatternFork, line )
      if (m):
        timeStamp = float( m.group(1) )
        numberOfWorkingNodes.append(numberOfWorkingNodes[-1]+1)
        numberOfIdleNodes.append(numberOfIdleNodes[-1]-1)
        numberOfForks.append(numberOfForks[-1]+1)

        timelineOfWorkingNodes.append(timeStamp)
        timelineOfIdleNodes.append(timeStamp)
        timelineOfForks.append(timeStamp)
      m = re.search( searchPatternJoin, line )
      if (m):
        timeStamp = float( m.group(1) )
        
        numberOfWorkingNodes.append(numberOfWorkingNodes[-1]-1)
        numberOfIdleNodes.append(numberOfIdleNodes[-1]+1)
        numberOfJoins.append(numberOfJoins[-1]+1)

        timelineOfWorkingNodes.append(timeStamp)
        timelineOfIdleNodes.append(timeStamp)
        timelineOfJoins.append(timeStamp)
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  
  pylab.clf()
  pylab.title( "Fork and join statistics" )

  pylab.plot(timelineOfWorkingNodes, numberOfWorkingNodes, 'o-',  markersize=10, color='#ffaa00', label='working nodes' )
  pylab.plot(timelineOfIdleNodes, numberOfIdleNodes,    '+-',  markersize=10, color='#00ffaa', label='idle nodes' )
  pylab.plot(timelineOfForks, numberOfForks,        '.-',  markersize=10, color='#aa00ff', label='total forks' )
  pylab.plot(timelineOfJoins, numberOfJoins,        'x-',  markersize=10, color='#ff00aa', label='total joins' )
  
  setGeneralPlotSettings()
  
  pylab.savefig( outputFileName + ".fork-join-statistics.png" )
  pylab.savefig( outputFileName + ".fork-join-statistics.pdf" )


def  extractForkHistory():
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


  

def plotBoundaryLateSends():
  pairs = dict()
  totalMaxCardinality     = 0
  totalCount              = 0
  totalAverage            = 0
  waitingForNeighborLine  = "rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::dataWasNotReceivedInBackground.*rank had to wait for (\d+) record\(s\)" \
                          + " from (\d+) on tag (\d) with page size (\d+)"

  print "search for late boundary answers ...",
  try:
    inputFile = open( inputFileName,  "r" )
    for line in inputFile:
      m = re.search( waitingForNeighborLine, line )
      if (m):
        master = int(m.group(1))
        worker = int(m.group(3))
        key = (master, worker)
        
        if not pairs.has_key(key):
          pairs[key] = Pair(master, worker)
        pair = pairs[key]
        
        cardinality = int(m.group(1))
        pair.count += 1
        totalCount += 1 
     
        pair.maxCardinality = max(pair.maxCardinality, cardinality)
        totalMaxCardinality = max(totalMaxCardinality, cardinality)

        pair.average += cardinality
        totalAverage += cardinality

    print "boundary analysis found " + str( len(pairs) ) + " entries of interest in trace file" 
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  
  graph              = networkx.DiGraph()
  sparseAverageGraph = networkx.DiGraph()
  sparseMaxGraph     = networkx.DiGraph()
  for rank in range(0,numberOfRanks):
    graph.add_node(str(rank))
    sparseAverageGraph.add_node(str(rank))
    sparseMaxGraph.add_node(str(rank))
  for master in range(0,numberOfRanks):
    for worker in range(0,numberOfRanks):
      key = (master, worker)
      if pairs.has_key(key):
        pair = pairs[key]
        if pair.count>2:
          graph.add_edge(str(worker),str(master))
          #edge = pydot.Edge(str(sender),str(receiver), label="(" + str(pair.count) + "," + str(pair.maxCardinality) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
        
        if totalCount>0 and pair.count>2 and (float(pair.average) / float(pair.count) > float(totalAverage) / float(totalCount)):
          #edge = pydot.Edge(str(sender),str(receiver), label="(" + str(pair.count) + "," + str(pair.maxCardinality) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
          sparseAverageGraph.add_edge(str(worker),str(master))
 
        if pair.count>2 and ( float(pair.maxCardinality) > 0.9 * float(totalMaxCardinality) ):
          #edge = pydot.Edge(str(sender),str(receiver), label="(" + str(pair.count) + "," + str(pair.maxCardinality) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
          sparseMaxGraph.add_edge(str(worker),str(master))
  pylab.clf()
  pylab.title( "Late sends from neighbours" )
  drawTreeGraph(graph)
  pylab.savefig( outputFileName + ".boundary-data-exchange.png" )
  pylab.savefig( outputFileName + ".boundary-data-exchange.pdf" )
  switchToLargePlot()
  pylab.savefig( outputFileName + ".boundary-data-exchange.large.png" )
  pylab.savefig( outputFileName + ".boundary-data-exchange.large.pdf" )
  switchBackToStandardPlot()

  pylab.clf()
  pylab.title( "Late sends from neighbours (only edges with weight more than average)" )
  drawTreeGraph(sparseAverageGraph)
  pylab.savefig( outputFileName + ".boundary-data-exchange.sparse-average.png" )
  pylab.savefig( outputFileName + ".boundary-data-exchange.sparse-average.pdf" )
  switchToLargePlot()
  pylab.savefig( outputFileName + ".boundary-data-exchange.sparse-average.large.png" )
  pylab.savefig( outputFileName + ".boundary-data-exchange.sparse-average.large.pdf" )
  switchBackToStandardPlot()

  pylab.clf()
  pylab.title( "Late sends from neighbours (only 10% heaviest edges)" )
  drawTreeGraph(sparseMaxGraph)
  pylab.savefig( outputFileName + ".boundary-data-exchange.sparse-max.png" )
  pylab.savefig( outputFileName + ".boundary-data-exchange.sparse-max.pdf" )
  switchToLargePlot()
  pylab.savefig( outputFileName + ".boundary-data-exchange.sparse-max.large.png" )
  pylab.savefig( outputFileName + ".boundary-data-exchange.sparse-max.large.pdf" )
  switchBackToStandardPlot()


def plotMasterWorkerLateSends():
  global GlobalSynchronisationOnRank0
  
  #  <a href=\"" + outputFileName + ".master-worker-data-exchange.large.png\" /><img src=\"" + outputFileName + ".master-worker-data-exchange.png\" />\
  pairs = dict()
  totalMaxTime        = 0.0
  totalAverage        = 0
  totalCount          = 0
  floatPattern = "[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?"
  waitingForWorkerLine = "rank:(\d+)*.*peano::performanceanalysis::DefaultAnalyser::endToReceiveDataFromWorker.+rank had to wait for worker (\d+) for ("+ floatPattern + ")s"

  print "search for late worker answers ",
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse fork/join statistics",
    for line in inputFile:
      m = re.search( waitingForWorkerLine, line )
      if (m):
        master = int(m.group(1))
        worker = int(m.group(2))
        key = (master, worker)
        
        if not pairs.has_key(key):
          pairs[key] = Pair(master, worker)
        pair = pairs[key]
        
        time = float(m.group(3))
     
        pair.maxTime = max(pair.maxTime, time)
        totalMaxTime = max(totalMaxTime, time)
        pair.average += time
        totalAverage += time
        
        pair.count += 1
        totalCount += 1 
    print " found " + str( len(pairs) ) + " entries of interest in trace file" 
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  
  graph              = networkx.DiGraph()
  sparseAverageGraph = networkx.DiGraph()
  sparseMaxGraph     = networkx.DiGraph()
  for rank in range(0,numberOfRanks):
    graph.add_node(str(rank))
    sparseAverageGraph.add_node(str(rank))
    sparseMaxGraph.add_node(str(rank))
  for master in range(0,numberOfRanks):
    for worker in range(0,numberOfRanks):
      key = (master, worker)
      if pairs.has_key(key):
        pair = pairs[key]
        if pair.count>2:
          graph.add_edge(str(worker),str(master))
          #edge = pydot.Edge(str(worker),str(master), label="(" + str(pair.count) + "," + str(pair.maxTime) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
        
        if totalCount > 0 and pair.count>2 and (float(pair.average) / float(pair.count) > float(totalAverage) / float(totalCount)):
          sparseAverageGraph.add_edge(str(worker),str(master))
          if master==0:
            GlobalSynchronisationOnRank0 = True
            print "detected very strong MPI synchronisation"
          #edge = pydot.Edge(str(worker),str(master), label="(" + str(pair.count) + "," + str(pair.maxTime) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
 
        if pair.count>2 and ( float(pair.maxTime) > 0.9 * float(totalMaxTime) ):
          sparseMaxGraph.add_edge(str(worker),str(master))
          if master==0:
            print "detected very strong MPI synchronisation"
            GlobalSynchronisationOnRank0 = True
          #edge = pydot.Edge(str(worker),str(master), label="(" + str(pair.count) + "," + str(pair.maxTime) + "," + str(float(pair.average) / float(pair.count)) + ")", fontsize=str(myfontsize), labelfontcolor="blue" )
          #sparseMaxGraph.add_edge(edge)      
  pylab.clf()
  pylab.title( "Late workers" )
  drawTreeGraph(graph)
  pylab.savefig( outputFileName + ".master-worker-data-exchange.png" )
  pylab.savefig( outputFileName + ".master-worker-data-exchange.pdf" )
  switchToLargePlot()
  pylab.savefig( outputFileName + ".master-worker-data-exchange.large.png" )
  pylab.savefig( outputFileName + ".master-worker-data-exchange.large.pdf" )
  switchBackToStandardPlot()

  pylab.clf()
  pylab.title( "Late workers (only edges with weight more than average)" )
  drawTreeGraph(sparseAverageGraph)
  pylab.savefig( outputFileName + ".master-worker-data-exchange.sparse-average.png" )
  pylab.savefig( outputFileName + ".master-worker-data-exchange.sparse-average.pdf" )
  switchToLargePlot()
  pylab.savefig( outputFileName + ".master-worker-data-exchange.sparse-average.large.png" )
  pylab.savefig( outputFileName + ".master-worker-data-exchange.sparse-average.large.pdf" )
  switchBackToStandardPlot()

  pylab.clf()
  pylab.title( "Late workers (only 10% heaviest edges)" )
  drawTreeGraph(sparseMaxGraph)
  pylab.savefig( outputFileName + ".master-worker-data-exchange.sparse-max.png" )
  pylab.savefig( outputFileName + ".master-worker-data-exchange.sparse-max.pdf" )
  switchToLargePlot()
  pylab.savefig( outputFileName + ".master-worker-data-exchange.sparse-max.large.png" )
  pylab.savefig( outputFileName + ".master-worker-data-exchange.sparse-max.large.pdf" )
  switchBackToStandardPlot()



def plotLogicalTopology():
  topologyGraph = networkx.MultiDiGraph()
  for rank in range(0,numberOfRanks):
    topologyGraph.add_node(str(rank))
    #topologyGraph.node[rank]['data']  = True
    #topologyGraph.node[rank]['label']  = str(rank)
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse topology",
    for line in inputFile:
      searchPatternAddFork    = "peano::performanceanalysis::DefaultAnalyser::addWorker.*\d+->\d+\+\d+"
      searchPatternAddJoin    = "peano::performanceanalysis::DefaultAnalyser::removeWorker.*\d+\+\d+->d+"
      if ("DefaultAnalyser" in line):
          m = re.search( searchPatternAddFork, line )
          if (m):
            parent = m.group(0).split("->")[0].split(" ")[-1] 
            child  = m.group(0).split("+")[-1].split(" ")[-1]
            topologyGraph.add_edge(child,parent)
            print ".",
          m = re.search( searchPatternAddJoin, line )
          if (m):
            child  = m.group(0).split("+")[1].split("->")[0] 
            parent = m.group(0).split("->")[-1] 
            topologyGraph.add_edge(child,parent)
            print ".",
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  pylab.clf()
  pylab.title( "Logical topology" )
  drawTreeGraph(topologyGraph)
  pylab.savefig( outputFileName + ".topology.png" )
  pylab.savefig( outputFileName + ".topology.pdf" )
  switchToLargePlot()
  pylab.savefig( outputFileName + ".topology.large.png" )
  pylab.savefig( outputFileName + ".topology.large.pdf" )
  switchBackToStandardPlot()


def setGeneralPlotSettings():
  pylab.xlabel('t')
  pylab.grid(True)
  # pylab.style.use('greyscale')
  # bmh              Hintergrund ist grau. Passt nicht so richtig hier. Im Gegensatz zu ggplot sind Gitterlinien gestrichelt
  # fivethirtyeight  Hintergrundfarbe ist homogen grau, kein Gitter. Es gibt einen grossen, dicken Rand. Passt zu Business-Grafiken 
  # ggplot           Hintergrund ist grau. Passt nicht so richtig hier 
  # greyscale        Geht bei mir nicht
  try:
    pylab.legend(fontsize=9, framealpha=0.5)
  except:
    # old pylab version
    l = pylab.legend(prop={'size':9})
    l.get_frame().set_alpha(0.5)



def plotGlobalGridOverview():
  pylab.clf()
  pylab.title( "Cells on global master" )

  pylab.plot(tTotal[0], numberOfInnerLeafCells[0], 'o-',  markersize=10, color='#ff0000', label='#inner leaf cells' )
  pylab.plot(tTotal[0], numberOfOuterLeafCells[0], '+-',  markersize=10, color='#00ff00', label='#outer leaf cells' )
  pylab.plot(tTotal[0], numberOfInnerCells[0],     '.-',  markersize=10, color='#aa0066', label='#inner cells' )
  pylab.plot(tTotal[0], numberOfOuterCells[0],     'x-',  markersize=10, color='#00aa66', label='#outer cells' )
  setGeneralPlotSettings()
  pylab.legend(fontsize=9, loc='upper left', framealpha=0.5)
  pylab.savefig( outputFileName + ".grid-overview-global-master.png" )
  pylab.savefig( outputFileName + ".grid-overview-global-master.pdf" )

  pylab.clf()
  pylab.title( "Local cells" )
  startRank = 1
  if (numberOfRanks==1):
    startRank = 0
  for rank in range(startRank,numberOfRanks):
    x = pylab.arange(0, len(numberOfLocalCells[rank]), 1.0)
    pylab.plot(tTotal[rank], numberOfLocalCells[rank], 'o',  color='#000000', alpha=AlphaValue, markersize=10)
  pylab.xlabel('t')
  pylab.savefig( outputFileName + ".local-cells.png" )
  pylab.savefig( outputFileName + ".local-cells.pdf" )

  globalNumberOfInnerLeafCells = [a for a in numberOfInnerLeafCells[0]]
  globalNumberOfOuterLeafCells = [a for a in numberOfOuterLeafCells[0]]
  globalNumberOfInnerCells     = [a for a in numberOfInnerCells[0]]
  globalNumberOfOuterCells     = [a for a in numberOfOuterCells[0]]
  
  for rank in range(1,numberOfRanks):
    currentEntry = len(globalNumberOfInnerLeafCells)-1
    for i in reversed(numberOfInnerLeafCells[rank]):
      globalNumberOfInnerLeafCells[currentEntry] = globalNumberOfInnerLeafCells[currentEntry] + i
      currentEntry = currentEntry - 1
    currentEntry = len(globalNumberOfOuterLeafCells)-1
    for i in reversed(numberOfOuterLeafCells[rank]):
      globalNumberOfOuterLeafCells[currentEntry] = globalNumberOfOuterLeafCells[currentEntry] + i
      currentEntry = currentEntry - 1
    currentEntry = len(globalNumberOfInnerCells)-1
    for i in reversed(numberOfInnerCells[rank]):
      globalNumberOfInnerCells[currentEntry] = globalNumberOfInnerCells[currentEntry] + i
      currentEntry = currentEntry - 1
    currentEntry = len(globalNumberOfOuterCells)-1
    for i in reversed(numberOfOuterCells[rank]):
      globalNumberOfOuterCells[currentEntry] = globalNumberOfOuterCells[currentEntry] + i
      currentEntry = currentEntry - 1

  pylab.clf()
  pylab.title( "Global cells" )
  x = pylab.arange(0, len(numberOfInnerLeafCells[0]), 1.0)
  pylab.plot(tTotal[0], globalNumberOfInnerLeafCells, 'o-',  markersize=10, color='#ff0000', label='#inner leaf cells' )
  pylab.plot(tTotal[0], globalNumberOfOuterLeafCells, '+-',  markersize=10, color='#00ff00', label='#outer leaf cells' )
  pylab.plot(tTotal[0], globalNumberOfInnerCells,     '.-',  markersize=10, color='#aa0066', label='#inner cells' )
  pylab.plot(tTotal[0], globalNumberOfOuterCells,     'x-',  markersize=10, color='#00aa66', label='#outer cells' )
  pylab.legend(fontsize=9, loc='upper left', framealpha=0.5)
  setGeneralPlotSettings()
  pylab.savefig( outputFileName + ".grid-overview.png" )
  pylab.savefig( outputFileName + ".grid-overview.pdf" )

  pylab.clf()
  pylab.title( "Local vertices" )
  startRank = 1
  if (numberOfRanks==1):
    startRank = 0
  for rank in range(startRank,numberOfRanks):
    x = pylab.arange(0, len(numberOfLocalVertices[rank]), 1.0)
    pylab.plot(tTotal[rank], numberOfLocalVertices[rank], 'o',  color='#000000', alpha=AlphaValue, markersize=10)
  pylab.xlabel('t')
  pylab.savefig( outputFileName + ".local-vertices.png" )
  pylab.savefig( outputFileName + ".local-vertices.pdf" )


def plotWalltimeOverview():
  pylab.clf()
  pylab.title( "Walltime" )
  pylab.ylabel( "time per grid sweep [t]=s" )
  x = pylab.arange(0, len(tTotal[0]), 1.0)
  pylab.plot(tTotal[0], tTraversal[0], '-',  markersize=10, color='#000066', label='time per traversal on global master' )
  startRank = 1
  if (numberOfRanks==1):
    startRank = 0
  for rank in range(startRank,numberOfRanks):
    x = pylab.arange(0, len(tTraversal[rank]), 1.0)
    pylab.plot(tTotal[rank], tTraversal[rank], 'o',  color='r', alpha=AlphaValue, markersize=10)
  setGeneralPlotSettings()
  pylab.savefig( outputFileName + ".walltime.png" )
  pylab.savefig( outputFileName + ".walltime.pdf" )


def createRankDetails(rank):
  #searchPattern = "(\d+),rank:" + str(rank) + ".*::repositories::.*::restart(...).*start node for subdomain"
  #searchPattern = str(rank) + ".*repositories.*restart.*start node for subdomain"
  searchPattern = "rank:" + str(rank) + ".*repositories.*restart.*start node for subdomain"

  outFile.write( "<h4>Rank details:</h4>" );
  outFile.write( "<ol>" );
 
  wroteDetails = False
  #try:
  inputFile = open( inputFileName,  "r" )
  for line in inputFile:
    m = re.search( searchPattern, line )
    if (m):
      wroteDetails = True
      outFile.write( "<li>" );
      outFile.write( line );
      outFile.write( "</li>" );

  #except:
  #  pass
    
  if not wroteDetails:
    outFile.write( "<li>Rank details are available if and only if info messages from the repositories subcomponent are switched on</li>" );
  
  outFile.write( "</ol>" );



def plotStatisticsForRank(currentRank):
  pylab.clf()
  pylab.title( "Walltime" )
  pylab.plot(tTotal[0], tTraversal[0], '-',  markersize=10, color='#000066', label='time per traversal (global master)' )
  pylab.plot(tTotal[currentRank], tTraversal[currentRank], '-',  markersize=10, color='#550000', label='time per traversal (only inside local domain)' )
  startRank = 1
  if (numberOfRanks==1):
    startRank = 0
  for rank in range(startRank,numberOfRanks):
    pylab.plot(tTotal[rank], tTraversal[rank], 'o',  color='r', alpha=AlphaValue, markersize=10)
  setGeneralPlotSettings()
  pylab.savefig( outputFileName + ".walltime-rank-" + str(currentRank) + ".png" )
  pylab.savefig( outputFileName + ".walltime-rank-" + str(currentRank) + ".pdf" )

  pylab.clf()
  pylab.title( "Cells" )
  pylab.plot(tTotal[currentRank], numberOfLocalCells[currentRank], '-',  markersize=10, color='b', label='local cells' )
  startRank = 1
  if (numberOfRanks==1):
    startRank = 0
  for rank in range(startRank,numberOfRanks):
    pylab.plot(tTotal[rank], numberOfLocalCells[rank], 'o',  color='r', alpha=AlphaValue, markersize=10)
  setGeneralPlotSettings()
  pylab.savefig( outputFileName + ".local-cells-rank-" + str(currentRank) + ".png" )
  pylab.savefig( outputFileName + ".local-cells-rank-" + str(currentRank) + ".pdf" )
  
  totalTimeCalendar            = []
  joinTimeCalendar             = []
  boundaryTimeCalendar         = []
  synchronousHeapDataCalendar  = []
  asynchronousHeapDataCalendar = []
  centralElementCalendar       = []
  workerWaitTimeCalendar       = []

  totalTimeCPU                 = []
  joinTimeCPU                  = []
  boundaryTimeCPU              = []
  synchronousHeapDataCPU       = []
  asynchronousHeapDataCPU      = []
  centralElementCPU            = []

  ttotalTimeCalendar            = []
  tjoinTimeCalendar             = []
  tboundaryTimeCalendar         = []
  tsynchronousHeapDataCalendar  = []
  tasynchronousHeapDataCalendar = []
  tcentralElementCalendar       = []
  tworkerWaitTimeCalendar       = []

  ttotalTimeCPU                 = []
  tjoinTimeCPU                  = []
  tboundaryTimeCPU              = []
  tsynchronousHeapDataCPU       = []
  tasynchronousHeapDataCPU      = []
  tcentralElementCPU            = []

  try:
    inputFile = open( inputFileName,  "r" )
    for line in inputFile:
      if (re.search( "DefaultAnalyser::endIteration", line ) and re.search( "rank:" + str(currentRank) + " ", line ) and re.search( "t_total", line )):
        totalTimeCalendar.append( float( line.split( "(" )[-1].split(",")[0] ))
        totalTimeCPU.append(      float( line.split( "," )[-1].split(")")[0] ))
        
        ttotalTimeCalendar.append( float( line.strip().split( " " )[0] ))
        ttotalTimeCPU.append(      float( line.strip().split( " " )[0] ))
      if (re.search( "DefaultAnalyser::endReleaseOfJoinData", line ) and re.search( "rank:" + str(currentRank) + " ", line )):
        joinTimeCalendar.append( float( line.split( "=" )[1].split(",")[0] ))
        joinTimeCPU.append(      float( line.split( "=" )[2].strip() ))

        tjoinTimeCalendar.append( float( line.strip().split( " " )[0] ))
        tjoinTimeCPU.append(      float( line.strip().split( " " )[0] ))
      if (re.search( "DefaultAnalyser::endReleaseOfBoundaryData", line ) and re.search( "rank:" + str(currentRank) + " ", line )):
        boundaryTimeCalendar.append( float( line.split( "=" )[-2].split(",")[0] ))
        boundaryTimeCPU.append(      float( line.split( "=" )[-1].strip() ))

        tboundaryTimeCalendar.append( float( line.strip().split( " " )[0] ))
        tboundaryTimeCPU.append(      float( line.strip().split( " " )[0] ))
      if (re.search( "DefaultAnalyser::endToReleaseSynchronousHeapData", line ) and re.search( "rank:" + str(currentRank) + " ", line )):
        synchronousHeapDataCalendar.append( float( line.split( "=" )[-2].split(",")[0] ))      
        synchronousHeapDataCPU.append(      float( line.split( "=" )[-1].strip() ))

        tsynchronousHeapDataCalendar.append( float( line.strip().split( " " )[0] )    )  
        tsynchronousHeapDataCPU.append(      float( line.strip().split( " " )[0] ))
      if (re.search( "DefaultAnalyser::endToPrepareAsynchronousHeapDataExchange", line ) and re.search( "rank:" + str(currentRank) + " ", line )):
        asynchronousHeapDataCalendar.append( float( line.split( "=" )[-2].split(",")[0] ))      
        asynchronousHeapDataCPU.append(      float( line.split( "=" )[-1].strip() ))

        tasynchronousHeapDataCalendar.append( float( line.strip().split( " " )[0] ))     
        tasynchronousHeapDataCPU.append(      float( line.strip().split( " " )[0] ))
      if (re.search( "DefaultAnalyser::leaveCentralElementOfEnclosingSpacetree", line ) and re.search( "rank:" + str(currentRank) + " ", line )):
        centralElementCalendar.append( float( line.split( "(" )[-1].split(",")[0] ))
        centralElementCPU.append(      float( line.split( "," )[-1].split(")")[0] ))

        tcentralElementCalendar.append( float( line.strip().split( " " )[0] ))
        tcentralElementCPU.append(      float( line.strip().split( " " )[0] ))
      if (re.search( "DefaultAnalyser::dataWasNotReceivedFromWorker", line ) and re.search( "rank:" + str(currentRank) + " ", line )):
        workerWaitTimeCalendar.append( float( line.split( "for " )[2].split("s")[0] ))      
  
        tworkerWaitTimeCalendar.append( float( line.strip().split( " " )[0] ) )     
        
        
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst

  pylab.clf()
  pylab.title( "Runtime profile (calendar time)" )

  pylab.plot(ttotalTimeCalendar, totalTimeCalendar, 'x-', label='total', color='#FF0000', markersize=10)

  pylab.plot(tjoinTimeCalendar, joinTimeCalendar, 'o-', label='join', color='#00FF00', markersize=10)

  pylab.plot(tboundaryTimeCalendar, boundaryTimeCalendar, '^-', label='boundary exchange', color='#0000FF', markersize=10)

  pylab.plot(tsynchronousHeapDataCalendar, synchronousHeapDataCalendar, '+-', label='synchronous heap', color='#aaaa00', markersize=10)

  pylab.plot(tasynchronousHeapDataCalendar, asynchronousHeapDataCalendar, '1-', label='asynchronous heap', color='#aa00aa', markersize=10)

  #
  # If multiple ranks plot to one terminal, lines can be mixed up and parsing can fail. 
  #
  if (len(tcentralElementCalendar)==len(centralElementCalendar)):
    pylab.plot(tcentralElementCalendar, centralElementCalendar, 'v--', label='local elements traversal', color='#00aaaa', markersize=10)
  else:
    print "- error in centralElementCalendar"

  pylab.plot(tworkerWaitTimeCalendar, workerWaitTimeCalendar, '3-', label='wait for worker', color='#667766', markersize=10)

  setGeneralPlotSettings()

  pylab.savefig( outputFileName + ".runtime-profile-calendar-rank-" + str(currentRank) + ".png" )
  pylab.savefig( outputFileName + ".runtime-profile-calendar-rank-" + str(currentRank) + ".pdf" )



  pylab.clf()
  pylab.title( "Runtime profile (cpu time)" )

  pylab.plot(ttotalTimeCPU, totalTimeCPU, 'x-', label='total', color='#FF0000', markersize=10)

  pylab.plot(tjoinTimeCPU, joinTimeCPU, 'o-', label='join', color='#00FF00', markersize=10)

  pylab.plot(tboundaryTimeCPU, boundaryTimeCPU, '^-', label='boundary exchange', color='#0000FF', markersize=10)

  pylab.plot(tsynchronousHeapDataCPU, synchronousHeapDataCPU, '+-', label='synchronous heap', color='#aaaa00', markersize=10)

  pylab.plot(tasynchronousHeapDataCPU, asynchronousHeapDataCPU, '1-', label='asynchronous heap', color='#aa00aa', markersize=10)

  if (len(tcentralElementCPU)==len(centralElementCPU)):
    pylab.plot(tcentralElementCPU, centralElementCPU, 'v--', label='local elements traversal', color='#00aaaa', markersize=10)
  else:
    print "- error in centralElementCPU"

  setGeneralPlotSettings()
  pylab.savefig( outputFileName + ".runtime-profile-cpu-rank-" + str(currentRank) + ".png" )
  pylab.savefig( outputFileName + ".runtime-profile-cpu-rank-" + str(currentRank) + ".pdf" )




#
# main
#
if ( len(sys.argv)!=2 and len(sys.argv)!=4):
  print "Usage: python performanceanalysis.py inputfile [ranks threads]"
  print ""
  print "ranks and threads are optional integers. Skip them or set them to"
  print "0 if you use no MPI or multithreading, respectively."
  print ""
  print "(C) 2015 Tobias Weinzierl"
  quit()
if ( len(sys.argv)==2 ):
  print "input data stems from single core run" 
  numberOfRanks   = 0
  numberOfThreads = 0
  AlphaValue     = 0.9
else:
  numberOfRanks   = int(sys.argv[2])
  numberOfThreads = int(sys.argv[3])
  if numberOfRanks>0:
    AlphaValue     = 1.0/numberOfRanks
    if (AlphaValue<0.1):
      AlphaValue   = 0.1
  else:
    AlphaValue     = 0.9

inputFileName   = sys.argv[1]
outputFileName  = sys.argv[1] + ".html"

outFile        = open( outputFileName, "w" )
  
    


#
# Now all the images are created and the data is analysed.
# I use this data to generate some problem-specific hints, 
# so it has to be done first
#  
parseInputFile()

print "plot walltime overview"
plotWalltimeOverview()

print "plot global grid overview"
plotGlobalGridOverview()

if (numberOfThreads>0):      
  print "plot concurrency levels"
  if (numberOfRanks>0):
    for rank in range(0,numberOfRanks):
      print "plot concurrency levels for rank " + str(rank)
      plotConcurrency(rank)
  else:
    print "plot concurrency levels"
    plotConcurrency(0)
  
if (numberOfRanks>0):      
  print "plot logical grid topology"
  plotLogicalTopology()

  print "extract fork and join statistics"
  plotForkJoinStatistics()

  print "plot mpi phases"
  plotMPIPhases()

  print "master-worker data exchange"
  plotMasterWorkerLateSends()

  print "boundary data exchange"
  plotBoundaryLateSends()

  for rank in range(0,numberOfRanks):
    print "plot statistics for rank " + str(rank)
    plotStatisticsForRank(rank)
  




#
# Header of report
#
outFile.write( 
  "<html>\
     <body>\
     <h1>Peano Performance Report</h1>\
     <p>Peano's default performance analysis</p>\
     <p>Author: Tobias Weinzierl</p>\
     <p>Data file: " + inputFileName + "</p>\
  ")
     
if (numberOfRanks==0):      
  outFile.write( "\
     <p>Ranks: no MPI used</p>\
  ")
else:      
  outFile.write( "\
     <p>Ranks: " + str(numberOfRanks) + "</p>\
  ")
     
if (numberOfThreads==0):      
  outFile.write( "\
     <p>Threads: no multithreading used</p>\
  ")
else:      
  outFile.write( "\
     <p>Threads: " + str(numberOfThreads) + "</p>\
  ")
     
outFile.write( "\
     <center><img src=\"http://www.peano-framework.org/logo.png\" />\
     <br /><a href=\"http://www.peano-framework.org\" >www.peano-framework.org</a></center>\
  ")

outFile.write( "\
   <h2 id=\"table-of-contents\">Table of contents</h2>\
   <ul>\
   ")

outFile.write( "\
     <li><a href=\"#walltime-overview\">Walltime overview</a></li>\
     <li><a href=\"#global-grid-overview\">Global grid overview</a></li>\
    ")


if (numberOfThreads>0):      
  outFile.write( "<li><a href=\"#concurrency\">Multithreading concurrency</a></li>")
  
  
if (numberOfRanks>0):      
  outFile.write( "\
     <li><a href=\"#logical-topology\">Logical topology</a></li>\
     <li><a href=\"#fork-join-statistics\">Fork and join statistics</a></li>\
     <li><a href=\"#fork-history\">Fork history</a></li>\
     <li><a href=\"#mpi-phases\">MPI phases</a></li>\
     <li><a href=\"#master-worker-data-exchange\">Master-worker data exchange</a></li>\
     <li><a href=\"#boundary-data-exchange\">Boundary data exchange</a></li>\
     <li><a href=\"#individual-ranks\">Individual ranks</a></li>\
    ")

outFile.write( "\
   </ul>\
   ")






#
# Walltime information
#
outFile.write( "\
    <h2 id=\"walltime-overview\">Walltime overview</h2>\
     <img src=\"" + outputFileName + ".walltime.png\" />\
    <br /><br />\
    <p>\
    The fuzzy dots summarise the local runtimes of the individual ranks, \
    i.e. the time spent within the local traversals. \
    Cf. shift discussion in the next section. \
    For non-reducing algorithms that do not synchronise their boundaries, \
    local compute times can exceed the global time per traversal. \
    </p>\
    <p>\
    The solid line is the time per traversal on the global master rank. \
    If you run without MPI, this equals your application's runtime. \
    </p>\
    <p>\
    If your code does not synchronise the ranks with the global master, the solid line does not give a good \
    overview of your runtime. If the code synchronises only everh kth grid sweep, k sweeps are always summarised \
    by one linear graph. \
    </p>\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")

#
# Grid information
#
outFile.write( "\
    <h2 id=\"global-grid-overview\">Global grid overview</h2>\
    <img src=\"" + outputFileName + ".grid-overview.png\" />\
    <img src=\"" + outputFileName + ".grid-overview-global-master.png\" />\
    <br /><br />\
    ")
    
    
if (numberOfRanks>0):      
  outFile.write( "\
    <img src=\"" + outputFileName + ".local-cells.png\" />\
    <img src=\"" + outputFileName + ".local-vertices.png\" />\
    <br /><br />\
    ")

outFile.write( "\
    <p>\
    <b>Remarks on the global cell/vertex and the master plots:</b> \
    If you are implementing a code that allows ranks to send up their state to the \
    master before they've completed the whole traversal (see communicator object in \
    the mappings) or even to skip reduction, you will see oscillations and non-smooth \
    behaviour for some data. This is not a bug, but has to do with the communication scheme. \
    </p>\
    <p>\
    <b>Remarks on the local cell/vertex plots:</b> The darker the plots the more ranks exhibit a certain characteristics. \
    If the points spread out, this is an indicator that your load balancing is \
    inadequate. \
    If you run Peano on multiple ranks, rank 0 typically degenerates to a pure \
    administrative rank, i.e. it is o.k. if there is close to 0 vertices and cells \
    for one particular rank. \
    Please note that the outputs of the individual ranks are not synchronised \
    with each other. \
    Ranks that start to work later yield shorter histograms that are all aligned \
    to the left. \
    </p>\
    <p>\
    <i>Performance hint:</i></p><p>\
    In Peano's spacetree world, the coarse grid ranks are the most critical ranks. \
    If your global master holds many cells, e.g., almost for sure you run into poor-scaling \
    code. \
    <p>\
    <i>Performance hint:</i></p><p>\
    If you identify ranks whose local load decreases incrementally, these are ranks \
    that step by step fork more of their work to other ranks. In this case, it might \
    make sense to switch from an aggressive refinement into an iterative grid refinement \
    strategy (one refinement level per step, e.g.) to allow the rank to deploy work \
    throughout the grid construction and thus build up the grid in parallel and avoid \
    the transfer of whole grid blocks due to rebalancing. \
    </p>\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")



#
# Concurrency analysis
#
if (numberOfThreads>0):      
  outFile.write( "<h2 id=\"concurrency\">Multithreading concurrency</h2>" )
  if numberOfRanks==0:
    outFile.write( "<img src=\"" + outputFileName + "-rank-0.concurrency.png\" />" )
    outFile.write( "<br /><a href=\"" + outputFileName + "-rank-0.concurrency.large.png\">Big version</a>" )

  for rank in range(0,numberOfRanks):
    outFile.write( "<h3>Rank " + str(rank) + "</h2>" )
    outFile.write( "<img src=\"" + outputFileName + "-rank-" + str(rank) + ".concurrency.png\" />" )
    outFile.write( "<br /><a href=\"" + outputFileName + "-rank-" + str(rank) + ".concurrency.large.png\">Big version</a>" )
    
  outFile.write("\
    <br /><br />\
    <p>\
    Legend:\
    <ul>\
     <li>Dotted black line: Concurrency level is one</li> \
     <li>Solid black line: Real concurrency level of current code.</li> \
     <li>Solid green line: Maximum concurrency that could be used in the code if all grain sizes were set to one. Ignores background tasks.</li> \
     <li>Dotted dark green line: Maximum algorithmic concurrency level introduced by the code. Takes not into account that background tasks (blue) might be handled in the background, i.e. real concurrency level could be higher.</li> \
     <li>Light red bar: Average-case algorithmic concurrency level that could be obtained if the code selected grain size one everywhere. Ignores background tasks.</li> \
     <li>Dark red bar: Average-case algorithmic concurrency observed for selected grain sizes. Takes not into account that background tasks (blue) might be handled in the background, i.e. real concurrency level could be higher.</li> \
     <li>Blue bar: Additional tasks that are spawned into the background. They might remain there quite long (if the system is busy), but the diagram only tracks when they are first spawned.</li> \
    </ul>\
    If your events internally are multithreaded, this multithreading is not tracked by the performance analysis. Use a real performance analysis tool to get statements on your actual core usage. \
    </p>\
    <i>Performance hint: </i>\
    <p>\
      If your real concurrency falls below one, there has to be some IO or OS swapping that should be removed. \
      If your real concurrency level falls below the number of cores available, your multicore usage is poor. Try to improve concurrency by reducing grain sizes in the oracle, e.g. \
    </p>\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")
  if (numberOfRanks>0):      
    pass
    
    

if (numberOfRanks>0):      
  #
  # Logical topology  
  #
  outFile.write( "\
    <h2 id=\"logical-topology\">Logical topology</h2>\
    <img src=\"" + outputFileName + ".topology.png\" />\
    <br /><br />\
    <p>\
    A bigger version of the file can be found \
    <a href=\"" + outputFileName + ".topology.large.png\">here</a>. \
    </p>\
    <i>Performance hint: </i>\
    <p>\
    If the maximal number of working ranks is significantly smaller than the total number of ranks, your application might \
    have ran into a weak scaling issue. The problem then was too small. If the problem is sufficiently big, and the number \
    if idle ranks still remains high, it might had happened that Peano was not able to fork on a coarse enough level. \
    See the remarks on MPI tuning in Peano's quick start guide discussing multiscale concurrency. In such a case, it often helps \
    to make the computational grid more regular to some degree, i.e. to prescribe a finer maximum mesh size. \
    </p>\
    <p>\
    <i>Performance hint: </i></p><p>\
    If you find this graph to become a multigraph in many places, lots of joins and forks are conducted. \
    Joins and forks are expensive operations in terms of walltime. Evaluating the load balancing \
    information also is not for free. Hence, try to reduce the number of joins and forks as you \
    switch on rebalancing only from time to time, and reduce the load balancing overhead. See \
    the section on 'Disable load balancing' in Peano's quick start guide. \
    </p>\
    <p>\
    <i>Performance hint:</i></p><p>\
    Peano scales best if the coarsest grids are decomposed. In turn, it performs good if the logical topology tree is not too \
    wide, i.e. if it is deep and its breadth is bounded by 3^d you typically have a good performance. If your tree is shallow, \
    few ranks grab all the workers which is often not a good sign. Often, such an unfair behaviour goes hand in hand with a \
    monotonic decrease of cells on one worker. See discussion in the Section 'Global grid overview'. \
    Also consult the notes on multiscale concurrency in Peano's quick start guide. The latter also discusses how to manually enforce a higher coarse grid \
    regularity and thus allow the load balancing to fork. \
    </p>\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")


  #
  # Fork and join statistics
  #
  outFile.write( "\
    <h2 id=\"fork-join-statistics\">Fork and join statistics</h2>\
    <img src=\"" + outputFileName + ".fork-join-statistics.png\" />\
    <br /><br />\
    <p>\
    The statistics use the node's timers. If the timers of nodes ran with MPI \
    are slightly non-synchronised or many forks and joins happen at the same time, \
    you might see vertical zig-zag patterns. They are not errors but measurement inaccuracies. \
    </p>\
    <i>Performance hint: </i>\
    <p>\
    Joins and forks are expensive operations in terms of walltime. Evaluating the load balancing \
    information also is not for free. Hence, try to reduce the number of joins and forks as you \
    switch on rebalancing only from time to time, and reduce the load balancing overhead. See \
    the section on load balancing disabling in Peano's quick start guide. \
    </p>\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")


  #
  # Fork history
  #
  outFile.write( "<h2 id=\"fork-history\">Fork/join history/histograms</h2>" )
  extractForkHistory()
  outFile.write( " \
    <p>\
    The table above is an overview over all forks and joins. It allows you to explicitly keep track of these repartitionings and to interpret other performance data. \
    </p>\
    <i>Performance hint: </i>\
    <p>\
    Most Peano applications suffer if the forks are not conducted on the coarsest grid levels. If you run into a red (empty) level above, the code does not fork on a \
    particular level while finer levels then continue to fork. This will, due to the dfs nature of the grid traversal, induce a serialisation of the code. Try to fork \
    more aggressively on coarser levels by using a more regular grid for the coarse resolutions, e.g. \
    </p>\
    <i>Performance hint: </i>\
    <p>\
    Master-worker communication is very critical for the runtime as this information is exchanged synchronously. If some columns in the first table above holds only \
    one entry, you should have a very shallow topology (cf. topology graph) and thus one master for many ranks. This is typically not a good idea as this master then \
    easily becomes a bottleneck. Try to distribute the master job (by forking on coarser levels, e.g.). \
    </p>\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")


  #
  # MPI Phases
  # 
  outFile.write( "\
    <h2 id=\"mpi-phases\">MPI Phases</h2>\
    <a href=\"" + outputFileName + ".mpi-phases.large.png\"> <img src=\"" + outputFileName + ".mpi-phases.png\" /> </a> \
    <br /><br />\
    <h3>Legend:</h3>\
    <ul>\
    <li>x-axis: simulation time</li>\
    <li>y-axis: ranks</li>\
    <li>vertical black bars: end of iteration on master rank</li>\
    <li>bright red: time spent outside of the domain prior to enter the actual local tree; comprises the time the node has to wait for data from the master if master-worker data exchange is not switched off.</li>\
    <li>green: time spent within local tree</li>\
    <li>dark red: time spent outside of the domain after local tree has been processed; comprises the time the node has to wait until data is successfully sent to the master if worker-master data exchange is not switched off. Usually this effect however is negligible as MPI buffers away the blocking messages</li>\
    <li>dark red on master: time spent to wait for first worker</li>\
    <li>blue: time required to exchange heap and boundary data; the heap data usually is for free as it overlaps into the subsequent traversal</li>\
    <li>white: idle time and time used to exchange boundary and heap data</li>\
    </ul>\
    <i>Visualisation remarks/performance hints: </i>\
    <ul>\
    <li>The sampling accuracy is low, i.e. if your code has a very low \
    runtime per traversal ratio, the measurements become inaccurate. </li>\
    <li>If you switch off reduction (workers do not send data back to their master), the end iteration bar is inserted though workers still might be working on the traversal.</li>\
    <li>If long green blocks assigned to nodes without any further workers introduce a critical path, your problem is ill-balanced, i.e. some ranks have signficiantly more work to do than others. You might want to use more ranks to give the load balancing more degrees of freedom to balance.</li>\
    <li>If all ranks suffer from long light red blocks, your setup suffers from latency. Ranks wait quite a while for their masters to give them a go. If you run a sequence of multiple sweeps in a row, you might be able map them onto one iterate. Skipping master-worker data exchange also might help.</li>\
    <li>If large dark redish blocks introduce your critical path, your problem either is decomposed into too small chunks. Reduce number of forks/ranks. Or you have to search for ways to skip worker-master data exchange.</li>\
    <li>If all ranks have large blue blocks that all end at the same time, then you have a very tight coupling of the ranks through boundary data. Change your bounding box, e.g. (see remarks in guide book on boundary data skips).</li>\
    <li>If the green bars are significantly smaller than the other bars, your problem is too small for this number of ranks. All your time is spent on communication.</li>\
    </ul>\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")


  #
  # Master-worker data exchange. In particular late senders
  #
  outFile.write( "\
    <h2 id=\"master-worker-data-exchange\">Master-worker data exchange</h2>\
    <p>If an edge points from a to b, it means that master b had to wait for its worker a. The labels are wait times in seconds. </p>\
    <a href=\"" + outputFileName + ".master-worker-data-exchange.large.png\" /><img src=\"" + outputFileName + ".master-worker-data-exchange.png\" /></a>\
    <p>The following graph holds only edges whose average is beyond the average of averages.</p>\
    <a href=\"" + outputFileName + ".master-worker-data-exchange.sparse-average.large.png\" /><img src=\"" + outputFileName + ".master-worker-data-exchange.sparse-average.png\" /></a>\
    <p>The following graph holds only edges whose maximum weight is with 10% of the total maximum weight.</p>\
    <a href=\"" + outputFileName + ".master-worker-data-exchange.sparse-max.large.png\" /><img src=\"" + outputFileName + ".master-worker-data-exchange.sparse-max.png\" /></a>\
    ")

  # This is one of the reasons why we have to generate the plots first    
  if  GlobalSynchronisationOnRank0:
    outFile.write( "<table bgcolor=\"#ff0000\"><tr><td>Attention: Code might suffer from strong synchronisation through rank 0.</td></tr></table>" ) 
  
  outFile.write( "\
    <br /><br />\
    <p>\
    If you obtain a warning no strong synchronisation with the master above but switched off all/most synchronisation with the master \
    please ignore the warning. In this case, it is natural that the global master waits for the workers to finish at some points. It \
    then is not an inefficiency pattern but an efficiency validation. \
    </p>\
    <i>Performance hints: </i>\
    <p>\
    If you use purely administrative ranks, i.e. ranks that hold only one cell of the spacetree and deploy all children to other ranks, such ranks always should have incoming \
    edges from all their children in the plot. \
    </p>\
    <p>\
    The edges here illustrate the critical communication path for one traversal, i.e. long graphs running from a node to rank 0 indicate \
    that along these graphs the Peano traversal is serialised. Eliminate these edges by reducing the \
    workload of the involved nodes. See remark in Peano's quick start guide on the optimisation of the worker-master communication or \
    reduction avoiding algorithms. \
    </p>\
    <p>\
    If you have late workers that you cannot explain, also try to scale up the problem. Late \
    workers often are an indicator for classic strong scaling issues, i.e. too small problem sizes. \
    </p>\
    <p>\
    If rank 0 synchronises all other ranks (see attention), try to reduce the synchronisation according to guide book. \
    </p>\
    <p>\
    If rank 0 continues to synchronises all other ranks and your MPI phases exhibit lots of dark red areas, your rank 0 might participate in too many boundary exchanges. Consult guide book how to adopt the domain boundaries accordingly. \
    </p>\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")



  #
  # Boundary data exchange. In particular late senders
  #
  outFile.write( "\
    <h2 id=\"boundary-data-exchange\">Boundary data exchange</h2>\
    <p>\
    The follow diagrams display an edge whenever a node A had to wait for a neighbouring node B. \
    The first entry of the label counters how often it had to work, the second the maximum data \
    cardinality, i.e. vertices, and the last one the average wait cardinality. If A points to B, \
    B hat to wait for data coming from A. The directed edges illustrate the data flow. As mentioned, \
    all figures enlist data cardinalities. They are not wait times but number of records missing. \
    </p>\
    <p>\
    In all diagrams, singular events, i.e. events waits only once or twice, are omitted.\
    </p>\
    <a href=\"" + outputFileName + ".boundary-data-exchange.large.png\" /><img src=\"" + outputFileName + ".boundary-data-exchange.png\" /></a>\
    <p>The following graph holds only edges whose average is beyond the average of averages.</p>\
    <a href=\"" + outputFileName + ".boundary-data-exchange.sparse-average.large.png\" /><img src=\"" + outputFileName + ".boundary-data-exchange.sparse-average.png\" /></a>\
    <p>The following graph holds only edges whose maximum weight is with 10% of the total maximum weight.</p>\
    <a href=\"" + outputFileName + ".boundary-data-exchange.sparse-max.large.png\" /><img src=\"" + outputFileName + ".boundary-data-exchange.sparse-max.png\" /></a>\
    ")

  outFile.write( "\
    <br /><br />\
    <i>Performance hint: </i>\
    <p>\
    Peano is designed to exchange all boundary data in the background of the computation. \
    In the ideal case, the above graph hence should be empty or very sparse. \
    If  it is pretty dense, your algorithm is bandwidth-bound. This is not a problem \
    per se, if the bandwidth-bound nodes do not slow down their masters, i.e. \
    the diagrams above have to be read in context with the worker-master graphs. \
    </p>\
    <p>\
    The global rank 0 should not have adjacent edges. If it has, ensure you've followed the avoid communications \
    with rank 0 recipes from Peano's quick start guide. \
    </p>\
    <p>\
    If this graph is a clique, your mpi buffer sizes might be too small or too big. Cf. \
    SendReceiveBufferPool's setBufferSize(). Its default values might be ill-suited for your \
    application's memory footprint. \
    </p>\
    <p>\
    If single nodes are the hot-spots making the others wait (many outgoing edges), those nodes might either have \
    a significantly higher load than others (cf. balancing remark above) or they might have a significant higher surface to \
    other nodes that has to be exchanged. Adopt load balancing and check the load balancing \
    as discussed in Peano's quick start guide. \
    </p>\
    <p>\
    If edges point from workers to their direct parents (cf. logical topology), the local master-worker balancing might be ill-balanced. Such \
    edges are in the first place not that important - any node might have a severe workload where it can only deploy \
    smaller parts of the work. The involved worker then has, by definition, to be smaller than its master (it got only \
    a fragment of the total work) and then waits for the master's boundary data for the subsequent iteration. A \
    critical situation (from a performance point of view) would occur, if the master had to wait for the worker's \
    finished message. Cf. Master-worker statistics above. \
    </p>\
    <p>\
    If nodes delay their masters but have no significant heavy edges in the boundary graph, study their individual runtime \
    profile carefully. If these profiles also indicate that the data exchange is not signficiant, your worker-master \
    data exchange suffers from worker-master latency. See Peano's quick start guide on the optimisation of worker-master communication. \
    </p><br /><br />\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")


  #
  # Boundary data exchange. In particular late senders
  #
  outFile.write( "\
    <h2 id=\"individual-ranks\">Individual ranks</h2>\
    <p>\
      Each Peano traversal consists of the following phases: \
      <ul> \
      <li> \
        <i>Busy time</i>: This is the time the node is actually traversing the tree, i.e. \
        the time between a start message received from its master and the time until the finished \
        message is sent back to the master. Busy times also comprise message exchanges, but these \
        message exchanges (boundary, load balancing, and so forth) run parallel to or are merged \
        into the grid traversal. If the workload is homogeneous (i.e. roughly the same number of \
        operations per spacetree node or at least leaf), the busy time usually directly correlates \
        to the number of cells, i.e. to the workload distribution. \
        The only fragments of the busy time that are illustrated separately is the time consumed by \
        the heap data exchange as well as the accumulated time a rank has to wait for the . \
      </li> \
      <li> \
        Definition <i>Work within domain</i>: Part of the busy time where the inner domain's bounding \
        box is traversed and not the embedding cells. Peano embeds all domains into a greater box to \
        simplify the boundary handling. This means, a traversal has to run through the actual domain \
        (work within domain time) and to run through boundary/shadow layers. Busy time comprises the \
        whole run while work within domain is only a subset. \
      </li> \
      <li> \
        Definition <i>Communication time</i>: As soon as a node has sent the information back to the master \
        that it has finished its traversal, it starts to clean up all the pending sends of boundary \
        data to the neighbours, receives the boundary data from all adjacent ranks, switches local \
        data structures, and so forth. It prepares all the data for the subsequent iteration. The \
        communication time comprises the \
        <ul> \
          <li> exchange of <i>join</i> data and the </li>  \
          <li> exchange of <i>boundary</i> vertices. </li> \
        </ul> \
      </li> \
      <li> \
        Definition <i>Idle time</i>: As soon as all data is communicated, the rank \
        waits for the next startup message from its master. This waiting \
        time is real idle time where nothing is done. \
      </li> \
    </p><br /><br />\
    <a href=\"#table-of-contents\">To table of contents</a>\
    " )

  for rank in range(0,numberOfRanks):
    outFile.write( "<a href=\"#runtime-rank-" + str(rank) + "\">Profile of rank " + str(rank) + "</a> - " )


  for rank in range(0,numberOfRanks):
    outFile.write( "<h3 id=\"runtime-rank-" + str(rank) + "\">Profile of rank " + str(rank) + "</h2>")
    createRankDetails(rank)  
    outFile.write( "\
      <img src=\"" + outputFileName + ".walltime-rank-" + str(rank) + ".png\" />\
      <img src=\"" + outputFileName + ".local-cells-rank-" + str(rank) + ".png\" />\
      <img src=\"" + outputFileName + ".runtime-profile-calendar-rank-" + str(rank) + ".png\" />\
      <img src=\"" + outputFileName + ".runtime-profile-cpu-rank-" + str(rank) + ".png\" />\
    <br /><br />\
    <a href=\"#individual-ranks\">To rank overview</a>\
    <br /><br />\
    <a href=\"#table-of-contents\">To table of contents</a>\
    ")


#
# Trailor of report
#
outFile.write( "</body>" )
outFile.write( "</html>" )
outFile.close()
print "html file written"
