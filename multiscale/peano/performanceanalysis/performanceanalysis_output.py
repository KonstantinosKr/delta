import sys
import re
import errno
import datetime
import os


def processTemplateFile(inputFileName,outFile,replacementMap):
  template = open( inputFileName, "r" )
  print "process template " + inputFileName,
  for line in template:
    for i in replacementMap:
      line = line.replace( i, replacementMap[i] )
    outFile.write(line+"\n")
    print ".",
  print "done"


def getOutputFile(inputFileName):
  return open( inputFileName+".html", "w" )


def writeTrailer(outFile):
  outFile.write( "</body></html>" )
  
  
def writeHeader(outFile,inputFileName,numberOfRanks,numberOfThreads):
 outFile.write( 
  "<html>\
     <body>\
     <h1>Peano Performance Analysis - Report</h1>\
     <p>Report time stamp: " + str(datetime.datetime.utcnow()) + "</p>\
     <p>Author: Tobias Weinzierl</p>\
     <p>Input file: " + inputFileName + "</p>\
 ")

 if (numberOfRanks==1):      
    outFile.write( "\
     <p>Ranks: no MPI used</p>\
    ")
 else:      
    outFile.write( "\
     <p>Ranks: " + str(numberOfRanks) + "</p>\
    ")
     
 if (numberOfThreads==1):      
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


def getOutputDirectory(inputFileName):
  return "_" + inputFileName


def createOutputDirectory(inputFileName):
  try:
    os.makedirs(getOutputDirectory(inputFileName))
  except OSError as e:
    if e.errno != errno.EEXIST:
      print "ERROR: failed to create output directory " + getOutputDirectory(inputFileName)


