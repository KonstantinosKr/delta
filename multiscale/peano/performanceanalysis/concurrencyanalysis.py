import argparse
from argparse import RawTextHelpFormatter


import performanceanalysis_parser
import performanceanalysis_output
import performanceanalysis_global_plotter
import performanceanalysis_analysis
import performanceanalysis_concurrency


import gc
import re
import os


########################################################################
# START OF THE PROGRAM
########################################################################
# For the meaning of the command line arguments, see the documen\tation
# of the function 'plot_multithreading_adapter_scaling' above.
help = '''
Creates a brief overview of the domain decomposition
\n\n
Sample usages:\n
python domain-decomposition-analysis.py -file 112x16-0.results -dimension 2 -domainoffset 0.0 0.0 -domainsize 15.0 15.0\n
'''

parser = argparse.ArgumentParser(description=help,formatter_class=RawTextHelpFormatter)
parser.add_argument('-file',required=True,help="Input file")
parser.add_argument('-dimension',required=True,help="Dimension of problem. Either 2 or 3.")
parser.add_argument('-domainoffset',nargs="+",required=True,help="Offset of bounding box.")
parser.add_argument('-domainsize',nargs="+",required=True,help="Size of domain's bounding box.")
args   = parser.parse_args();

dim = int(args.dimension)

scriptLocation = os.path.realpath(__file__)[:os.path.realpath(__file__).rfind("/")]

performanceanalysis_output.createOutputDirectory(args.file);

outFile         = performanceanalysis_output.getOutputFile(args.file)

numberOfRanks   = performanceanalysis_parser.getNumberOfRanks(args.file)
numberOfThreads = performanceanalysis_parser.getNumberOfThreads(args.file)

performanceanalysis_output.writeHeader(outFile,args.file,numberOfRanks,numberOfThreads);

beginIterations = performanceanalysis_parser.getBeginIterations(args.file,numberOfThreads>1)
if numberOfRanks<=1:
  (timeStamps,measuredConcurrencyLevels,obtainedConcurrencyLevels,maxConcurrencyLevels,
    maxPotentialConcurrencyLevels,numberOfBackgroundTasks,timeAveragedConcurrencyLevels,
    timeAveragedPotentialConcurrencyLevels) = performanceanalysis_parser.getConcurrency(-1,args.file)

  performanceanalysis_concurrency.plotConcurrency(
    numberOfThreads, performanceanalysis_output.getOutputDirectory(args.file)+"/concurrency",
    timeStamps,measuredConcurrencyLevels,obtainedConcurrencyLevels,maxConcurrencyLevels,
    maxPotentialConcurrencyLevels,numberOfBackgroundTasks,timeAveragedConcurrencyLevels,
    timeAveragedPotentialConcurrencyLevels,beginIterations)
else:
  for i in range(0,numberOfRanks):
    outFile.write( "<h2>Rank " + str(i) + "</h2>" )
    
    (timeStamps,measuredConcurrencyLevels,obtainedConcurrencyLevels,maxConcurrencyLevels,
      maxPotentialConcurrencyLevels,numberOfBackgroundTasks,timeAveragedConcurrencyLevels,
      timeAveragedPotentialConcurrencyLevels) = performanceanalysis_parser.getConcurrency(i,args.file)

    fileName = performanceanalysis_output.getOutputDirectory(args.file)+"/concurrency-rank-" + str(i)
    performanceanalysis_concurrency.plotConcurrency(
      numberOfThreads, fileName,
      timeStamps,measuredConcurrencyLevels,obtainedConcurrencyLevels,maxConcurrencyLevels,
      maxPotentialConcurrencyLevels,numberOfBackgroundTasks,timeAveragedConcurrencyLevels,
      timeAveragedPotentialConcurrencyLevels,beginIterations)

    outFile.write( "<a href=\"" + fileName + ".pdf\" >" )
    outFile.write( "<img src=\"" + fileName + ".png\" />" )
    outFile.write( "</a>" )

performanceanalysis_output.writeTrailer(outFile)

