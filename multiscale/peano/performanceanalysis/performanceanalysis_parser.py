#!/usr/bin/python
import sys
import re

'''
.. module:: performanceanalysis_parser.py
  :platform: Unix, Windows, Mac
  :synopsis: Contains routines for scanning Peano output files.
   
.. moduleauthor:: Tobias Weinzierl

:synopsis: Contains routines for scanning Peano output files.
'''

def getNumberOfRanks(filename):
  print "parse input file header ",
  try:
    inputFile = open( filename,  "r" )
    for line in inputFile:
      print ".",
      if "processes:" in line:
        numberOfRanks = int(line.split( "processes:" )[1].split( "," )[0])
        print str(numberOfRanks) + " MPI ranks used"
        return numberOfRanks

  except Exception as inst:
    print "failed to read " + filename
    print inst
  return 0


def getNumberOfThreads(filename):
  print "parse input file header ",
  try:
    inputFile = open( filename,  "r" )
    for line in inputFile:
      print ".",
      if "threads:" in line:
        numberOfThreads = int(line.split( "threads:" )[1].split( "(per process)" )[0])
        print str(numberOfThreads) + " threads per rank used"
        return numberOfThreads

  except Exception as inst:
    print "failed to read " + filename
    print inst
  return 0


def getDimensions(filename):
  print "parse input file header ",
  try:
    inputFile = open( filename,  "r" )
    for line in inputFile:
      print ".",
      if "peano::utils::UserInterface::writeHeader" in line and "dim=" in line:
        dim = int(line.split( "dim=" )[1].split( " " )[0])
        print str(dim) + " dimensions"
        return dim
  except Exception as inst:
    print "failed to read " + filename
    print inst
  return 0
  

def getBoundingBox(line):
  line    = line.replace( "]x[", "," )
  results = line.split( "node for subdomain [")[1].split( "] on level" )[0].split( "," )
  return results

  
def getLogicalTopology(numberOfRanks,dim,inputFileName,plotDirectoryName):
  parents  = [-1 for x in range(0,numberOfRanks)]
  nodes    = ["global master" for x in range(0,numberOfRanks)]
  levels   = [-1 for x in range(0,numberOfRanks)]
  if dim==2:
    volume = [(0.0,0.0) for x in range(0,numberOfRanks)]
    offset = [(0.0,0.0) for x in range(0,numberOfRanks)]
  else:
    volume = [(0.0,0.0,0.0) for x in range(0,numberOfRanks)]
    offset = [(0.0,0.0,0.0) for x in range(0,numberOfRanks)]
  level = [ 0 for x in range(0,numberOfRanks)]
  
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse topology",
    for line in inputFile:
      if "start node for subdomain" in line:
        print ".",
        child = int(line.split( "rank:" )[1].split( " " )[0])
        parent = int(line.split( "with master" )[1])
        node = line.split( ",rank:" )[0].split(" ")[-1]
        parents[child]=parent
        nodes[child]=node
        fragments = getBoundingBox(line) 
        if dim==2:
          offset[child] = ( float(fragments[0]),float(fragments[1]) )
          volume[child] = ( float(fragments[2]),float(fragments[3]) )
        else:
          offset[child] = ( float(fragments[0]),float(fragments[1]),float(fragments[2]) )
          volume[child] = ( float(fragments[3]),float(fragments[4]),float(fragments[5]) )
        levels[child] = int(line.split( "on level" )[1].split( " with" )[0])
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  return (parents,levels,offset,volume,nodes)


def getConcurrency(rank,inputFileName):
  timeStampPattern = "([0-9]+\.?[0-9]*)"
  floatPattern = "([0-9]\.?[0-9]*)"
  
  searchPatternConcurrencyPattern = timeStampPattern
  if rank>=0:
    searchPatternConcurrencyPattern = searchPatternConcurrencyPattern + ".*rank:" + str(rank)
  
  searchPatternConcurrencyPattern = searchPatternConcurrencyPattern + " .*peano::performanceanalysis::DefaultAnalyser::changeConcurrencyLevel" + \
                               ".*dt_real=" + floatPattern + \
                               ".*dt_cpu=" + floatPattern + \
                               ".*time-averaged-concurrency-level=" + floatPattern + \
                               ".*time-averaged-potential-concurrency-level=" + floatPattern + \
                               ".*max-concurrency-level=" + floatPattern + \
                               ".*max-potential-concurrency-level=" + floatPattern + \
                               ".*background-tasks=" + floatPattern

  timeStamps = []
  measuredConcurrencyLevels     = []  
  obtainedConcurrencyLevels     = []  
  maxConcurrencyLevels          = []
  maxPotentialConcurrencyLevels = []
  numberOfBackgroundTasks       = []  
  timeAveragedConcurrencyLevels = []
  timeAveragedPotentialConcurrencyLevels = []

  try:
    inputFile = open( inputFileName,  "r" )
    print "parse concurrency level of rank " + str(rank),
    for line in inputFile:
      m = re.search( searchPatternConcurrencyPattern, line )
      if (m):
        realTime                          = float( m.group(1) )
        dtRealTim                         = float( m.group(2) ) 
        dtCPUTime                         = float( m.group(3) ) 

        timeAveragedConcurrency           = float( m.group(4) )
        timeAveragedPotentialConcurrency  = float( m.group(5) )

        maxConcurrencyLevel               = float( m.group(6) )
        maxPotentialConcurrencyLevel      = float( m.group(7) )
        backgroundTasks                   = float( m.group(8) )
        
        timeStamps.append( realTime )
        measuredConcurrencyLevels.append( dtCPUTime/dtRealTim )

        maxConcurrencyLevels.append( maxConcurrencyLevel )
        maxPotentialConcurrencyLevels.append( maxPotentialConcurrencyLevel )
        numberOfBackgroundTasks.append( backgroundTasks )

        timeAveragedConcurrencyLevels.append(timeAveragedConcurrency/dtRealTim) 
        timeAveragedPotentialConcurrencyLevels.append(timeAveragedPotentialConcurrency/dtRealTim)

        print ".",
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst

  return (timeStamps,measuredConcurrencyLevels,obtainedConcurrencyLevels,maxConcurrencyLevels,
    maxPotentialConcurrencyLevels,numberOfBackgroundTasks,timeAveragedConcurrencyLevels,
    timeAveragedPotentialConcurrencyLevels)



def getBeginIterations(inputFileName,isParallelCode):
  timeStampPattern = "([0-9]+\.?[0-9]*)"
  floatPattern = "([0-9]\.?[0-9]*)"
  
  searchPatternBeginIteration = timeStampPattern
  if isParallelCode:
    searchPatternBeginIteration = searchPatternBeginIteration + ".*rank:0"
  searchPatternBeginIteration = searchPatternBeginIteration + " .*peano::performanceanalysis::DefaultAnalyser::beginIteration";

  beginIterations               = []

  try:
    inputFile = open( inputFileName,  "r" )
    print "parse begin iteration statements ",
    for line in inputFile:
      m = re.search( searchPatternBeginIteration, line )
      if (m):
        realTime                          = float( m.group(1) )
        beginIterations.append(realTime)
        print ".",
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst

  return beginIterations
    
    

def getCellsPerRank(inputFileName,numberOfRanks):
  print "parse cells per rank ",

  searchPatternNumberOfCells         = "peano::performanceanalysis::DefaultAnalyser::endIteration.*cells=\("
  searchPatternTTotal                = "([0-9]\.?[0-9]*).*peano::performanceanalysis::DefaultAnalyser::endIteration.*t_total=\("

  numberOfInnerLeafCells  = {}
  numberOfInnerCells      = {}
  numberOfOuterLeafCells  = {}
  numberOfOuterCells      = {}
  numberOfLocalCells      = {}
  tTotal                  = {}
  
  myNumberOfRanks = numberOfRanks
  if myNumberOfRanks==0:
    myNumberOfRanks = 1
  for rank in range(0,myNumberOfRanks):
    numberOfInnerLeafCells[rank]  = []
    numberOfInnerCells[rank]      = []
    numberOfOuterLeafCells[rank]  = []
    numberOfOuterCells[rank]      = []
    numberOfLocalCells[rank]      = []
    tTotal[rank]                  = []
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
          m = re.search( searchPatternTTotal, line )
          if (m):
            rank  = 0
            if numberOfRanks>0:
              rank  = int(line.split( "rank:" )[-1].split( " " )[0])
            timeStamp = float( m.group(1) )
            tTotal[rank].append(timeStamp)
            print ".",
          
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst

  return (
    numberOfInnerLeafCells,
    numberOfInnerCells,
    numberOfOuterLeafCells,
    numberOfOuterCells,
    numberOfLocalCells,
    tTotal )
    
    
    
def getMemoryUsagePerRank(inputFileName,numberOfRanks):
  '''
  Per rank, this function scans the file for entries of the form
  
  " 4.08824      [cn6001.hpc.dur.ac.uk],rank:1 info         exahype::runners::Runner::runAsWorker(...)                     memoryUsage    =839 MB"
  
  Args:
     inputFileName(str):
        Path to the input file.
     numberOfRanks (int):
        The number of ranks used for the simulation.
  
  Returns:
     A list per rank storing the rank's memory usage in each time step
  '''
  print "parse memory usage per rank ",

  memoryUsage  = [ ([],[]) for r in range(0,numberOfRanks) ]
  firstWorker  = -1
  
  try:
    inputFile = open( inputFileName,  "r" )
    print "parse ",
    for line in inputFile:
      if ("memoryUsage" in line):
        rank  = 0
        if numberOfRanks>0:
          rank  = int(line.split( "rank:" )[-1].split( " " )[0])
        print ".",
        
        # Parse time
        # Example: " 4.08824      [cn6001.hpc.dur.ac.uk]"
        time = line.split("      ")[0].strip()
        memoryUsage[rank][0].append(float(time))
        
        # Parse memory usage
        # Example: "memoryUsage    =839 MB"
        memory = line.split("=")[-1].split("MB")[0];
        memoryUsage[rank][1].append(int(memory))
    print " done"
  except Exception as inst:
    print "failed to read " + inputFileName
    print inst
  
  return memoryUsage
