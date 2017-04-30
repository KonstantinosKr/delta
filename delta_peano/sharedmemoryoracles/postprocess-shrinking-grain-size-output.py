import sys
import re
import os
import traceback
from enum import Enum


Symbol = [ 
  "s", "o", ">", 
  "<", "^", "v" 
]
Colour = [
  "#ff0000", "#00ff00", "#0000ff",
  "#ffff00", "#ff00ff", "#00ffff"
]


class Analysis(Enum):
  Nop                = 0,
  CurrentGrainSizeSeemsNotToScale    = 2,
  MightScale         = 3,
  DoesNotScale       = 4,
  Scales             = 5,
  GrainSizePredictedManually = 6


AdapterNames = []

def readAdapterNumbers( filename ):
  if not filename.endswith( "specification" ):
    print "WARNING: Peano specification files usually end with \"specification\"" 

  inputFile = open(filename, "r" )

  runIntoAdapter = False
  for line in inputFile:  
    if re.search( "adapter:", line ):
      runIntoAdapter = True
    elif re.search( "name:", line ) and runIntoAdapter:
      adapterName = line.split( "name:") [-1]
      AdapterNames.append( adapterName )
    else:
      runIntoAdapter = False
      

  print "finished to read specification file " + filename
  
  

def processMeasurement(adapter):
  searchPattern = "adapter-number=" + str(adapter) 
  line          = inputFile.readline()
  while not line=="" and not re.search( searchPattern, line ):
    line = inputFile.readline()
    
  if line=="":
    return
  
  htmlOverview.write( "<table border=\"1\">" );
  htmlOverview.write( 
    "<tr><td><b>Method</b></td>\
    <td><b>Biggest problem size</b></td>\
    <td><b>Grain size</b></td>\
    <td><b>Search delta</b></td>\
    <td><b>Is accurate</b></td>\
    <td><b>Accuracy</b>\
    <td><b>Accum. serial measurements</b></td>\
    <td><b>Accum. parallel measurements</b></td>\
    <td><b>No of serial measurements</b></td>\
    <td><b>No of parallel measurements</b></td>\
    <td><b>Previous speedup</b></td>\
    <td><b>Max concurrency level</b></td>\
    <td><b>Predicted speedup</b></td>\
    <td><b>Comments</b></td>\
    </tr>" );


  line = inputFile.readline()
  while not re.search( "end OracleForOnePhaseWithShrinkingGrainSize", line):
    try:
      methodTrace                      = line.split( "=" )[0]
      biggestProblemSize               = line.split( "=" )[1].split( "," )[0]
      currentGrainSize                 = line.split( "=" )[1].split( "," )[1]
      searchDelta                      = line.split( "=" )[1].split( "," )[2]
      measurementsAreAccurate          = line.split( "=" )[1].split( "," )[3]
      accuracy                         = line.split( "=" )[1].split( "," )[4]
      accumulatedSerialMeasurement     = line.split( "=" )[1].split( "," )[5]
      accumulatedParallelMeasurement   = line.split( "=" )[1].split( "," )[6]
      numberOfSerialMeasurements       = line.split( "=" )[1].split( "," )[7]
      numberOfParallelMeasurements     = line.split( "=" )[1].split( "," )[8]
      previousSpeedup                  = line.split( "=" )[1].split( "," )[9]

      speedup = 0.0
      if float(numberOfSerialMeasurements)>0 and float(numberOfParallelMeasurements)>0:
        speedup = float(accumulatedSerialMeasurement) / float(numberOfSerialMeasurements) / float(accumulatedParallelMeasurement) * float(numberOfParallelMeasurements)
    
      maxConcurrencyLevel = -1
      if float(currentGrainSize)>0:
        maxConcurrencyLevel = float(biggestProblemSize) / float(currentGrainSize)
    
    
      analysis = Analysis.Nop
      if int(searchDelta)>0  and speedup<1.0 and speedup>0.0:
        analysis = Analysis.CurrentGrainSizeSeemsNotToScale
      elif int(searchDelta)>0  and speedup>1.0:
        analysis = Analysis.MightScale
      elif int(searchDelta)==0 and float(currentGrainSize)==float(biggestProblemSize):
        analysis = Analysis.DoesNotScale
      elif int(searchDelta)==0 and speedup>1.0:
        analysis = Analysis.Scales
      elif int(searchDelta)==0 and speedup==0.0 and float(currentGrainSize)>0:
        analysis = Analysis.GrainSizePredictedManually
      #elif maxConcurrencyLevel<0 and float(currentGrainSize)<float(biggestProblemSize):
      #  analysis = Analysis.Scales
      #elif maxConcurrencyLevel>0 and :
      #  analysis = Analysis.GrainSizePredictedManually
  

      colour = "Grey"
      if analysis==Analysis.CurrentGrainSizeSeemsNotToScale:
        colour = "Fuchsia"
      elif analysis==Analysis.MightScale:
        colour = "LightGreen"
      elif analysis==Analysis.DoesNotScale:
        colour = "Red"
      elif analysis==Analysis.Scales:
        colour = "Green"
      elif analysis==Analysis.GrainSizePredictedManually:
        colour = "Lime"
      else:
        colour = "White"

      htmlOverview.write( "<tr>" );
      htmlOverview.write( "<td>" + methodTrace + "</td>" );
      htmlOverview.write( "<td>" + biggestProblemSize + "</td>" );
      htmlOverview.write( "<td>" + currentGrainSize + "</td>" );
      htmlOverview.write( "<td>" + searchDelta + "</td>" );
      if re.search( "is-accurate", measurementsAreAccurate):
        htmlOverview.write( "<td bgcolor=\"Aqua\">" + measurementsAreAccurate + "</td>" );
        htmlOverview.write( "<td bgcolor=\"Aqua\">" + accuracy + "</td>" );
      else:
        htmlOverview.write( "<td>" + measurementsAreAccurate + "</td>" );
        htmlOverview.write( "<td>" + accuracy + "</td>" );
      htmlOverview.write( "<td>" + accumulatedSerialMeasurement + "</td>" );
      htmlOverview.write( "<td>" + accumulatedParallelMeasurement + "</td>" );
      htmlOverview.write( "<td>" + numberOfSerialMeasurements + "</td>" );
      htmlOverview.write( "<td>" + numberOfParallelMeasurements + "</td>" );
      htmlOverview.write( "<td>" + previousSpeedup + "</td>" );
      htmlOverview.write( "<td>" + str(maxConcurrencyLevel) + "</td>" );
      htmlOverview.write( "<td bgcolor=\"" + colour + "\">" + str(speedup) + "</td>" );
  
      htmlOverview.write( "<td>");
      if int(searchDelta)!=0:
        htmlOverview.write( "Still searching. " );
      htmlOverview.write( str(analysis) );
      htmlOverview.write( "</td>" );    
      htmlOverview.write( "</tr>" );
      
    except Exception as exc:
      print "ERROR: failed to parse input file for adapter " + str(adapter) + " in line: " + line
      print("       details:", sys.exc_info()[0])
      traceback.print_exc()
    
    line = inputFile.readline()  

  htmlOverview.write( "</table>" );
      
  return;

#
#
#   main
# ========
#
#    
if (len(sys.argv)<3):
  print "usage: python ../postprocess-shrinking-grain-size-output.py Peano-specification-file output-files\n"
  print "\n"
  print "       You can hand over multiple output files in one rush\n"
  quit()
  
readAdapterNumbers( sys.argv[1] )

for i in range(2,len(sys.argv)):
  print "process data file " + sys.argv[i]
  htmlOverview = open( sys.argv[i] + ".html",  "w" )
  htmlOverview.write( "<h1>" + sys.argv[i] + "</h1>" );

  inputFile = open(sys.argv[i], "r" )

  line                  = inputFile.readline()
  totalNumberOfOracles  = int( line.split("=")[1] )
  line                  = inputFile.readline()
  oraclesForSteering    = int( line.split("=")[1] )

  htmlOverview.write( "Number of oracles=" + str(totalNumberOfOracles) + " (incl. oracles required for repository/algorithm steering)" );
  htmlOverview.write( "<br />" );
  htmlOverview.write( "Oracles required for repository steering=" + str(oraclesForSteering) );

  htmlOverview.write( "<h3>Table of content:</h3>" );
  htmlOverview.write( "<ul>" );
  for adapter in range(oraclesForSteering,totalNumberOfOracles):
    htmlOverview.write( "<li><a href=\"#adapter-" + str(adapter) + "\">Adapter " + str(adapter)  + ": " + AdapterNames[adapter-oraclesForSteering] + "</a></li>" );
  htmlOverview.write( "</ul>" );

  htmlOverview.write( "<p>Empty adapter sections imply that this adapter is not used by the code. The adapter order requals the adapter order in the specification file. Only those program phases that are actually used are also displayed.</p>" );
  htmlOverview.write( "<p>All individual timings are normalised by the number of entries handled, i.e. they do specify time per grid entity. We thus may not compare them directly to the total runtime.</p>" );

  for adapter in range(oraclesForSteering,totalNumberOfOracles):
    if adapter-oraclesForSteering>=len(AdapterNames):
      print "ERROR: No name available for adapter no " + str(adapter-oraclesForSteering)
      print "ERROR: Available adapters are " + str(AdapterNames)
      htmlOverview.write( "<h3 id=\"adapter-" + str(adapter) + "\">Adapter " + str(adapter) + "</h3>" );
    else:
      htmlOverview.write( "<h3 id=\"adapter-" + str(adapter) + "\">Adapter " + str(adapter) + ": " + AdapterNames[adapter-oraclesForSteering] + "</h3>" );
    processMeasurement(adapter)
    htmlOverview.write( "<p>The serial runtime is not reliable: It tracks only timinigs if the code fragment is not parallelised and the search for a grain size still is switched on. The longer your code runs, the more `invalid' this figure becomes. However, large entries indicate that you have heavy-weight serial part in your code.</p>" );
  
