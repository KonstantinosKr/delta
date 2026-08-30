import argparse
from argparse import RawTextHelpFormatter


import performanceanalysis_parser
import performanceanalysis_output
import performanceanalysis_global_plotter
import performanceanalysis_analysis
import performanceanalysis_concurrency
import performanceanalysis_dd
import performanceanalysis_griddata
import performanceanalysis_mpicommunication


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
parser.add_argument('-domainoffset',nargs="+",required=True,help="Offset of bounding box. Ensure there are as many arguments as you have dimensions.")
parser.add_argument('-domainsize',nargs="+",required=True,help="Size of domain's bounding box. Ensure there are as many arguments as you have dimensions.")
args   = parser.parse_args();

scriptLocation = os.path.realpath(__file__)[:os.path.realpath(__file__).rfind("/")]

performanceanalysis_output.createOutputDirectory(args.file);

outFile         = performanceanalysis_output.getOutputFile(args.file)

numberOfRanks   = performanceanalysis_parser.getNumberOfRanks(args.file)
numberOfThreads = performanceanalysis_parser.getNumberOfThreads(args.file)
dim             = performanceanalysis_parser.getDimensions(args.file)

if len(args.domainoffset)!=dim:
  print "dimensions of offset does not match dimensions. If input file is for dimension 2, then we need two offset values separated by a space"
  exit(-1)

if len(args.domainsize)!=dim:
  print "dimensions of domain size does not match dimensions. If input file is for dimension 2, then we need two size values separated by a space"
  exit(-1)

performanceanalysis_output.writeHeader(outFile,args.file,numberOfRanks,numberOfThreads);

(parents,levels,offset,volume,nodes) = performanceanalysis_parser.getLogicalTopology(numberOfRanks,dim,args.file,".");
(volumes,overlaps,work)              = performanceanalysis_analysis.computeVolumesOverlapsWork(numberOfRanks,parents,offset,volume,dim,args.domainoffset,args.domainsize)

if numberOfRanks>1:
  performanceanalysis_global_plotter.plotLogicalTopology(numberOfRanks,performanceanalysis_output.getOutputDirectory(args.file)+"/topology",parents,levels,offset,volume);

  performanceanalysis_global_plotter.plotWorkloadAndResponsibilityDistributionPerRank(numberOfRanks,performanceanalysis_output.getOutputDirectory(args.file)+"/workload-per-rank",volumes,overlaps,work);
  performanceanalysis_global_plotter.plotWorkloadAndResponsibilityDistributionPerNode(numberOfRanks,performanceanalysis_output.getOutputDirectory(args.file)+"/workload-per-node",work,nodes);

  performanceanalysis_mpicommunication.plotLateWorkers(numberOfRanks,args.file,performanceanalysis_output.getOutputDirectory(args.file)+"/late-workers") 
  performanceanalysis_mpicommunication.plotLateMasters(numberOfRanks,args.file,performanceanalysis_output.getOutputDirectory(args.file)+"/late-masters") 
  performanceanalysis_mpicommunication.plotLateBoundaries(numberOfRanks,args.file,performanceanalysis_output.getOutputDirectory(args.file)+"/late-boundaries") 

  for l in range(1,max(levels)+1):
    if dim==2:
      performanceanalysis_dd.plot2dDomainDecompositionOnLevel(l,numberOfRanks,args.domainoffset,args.domainsize,offset,volume,levels,nodes,performanceanalysis_output.getOutputDirectory(args.file)+"/dd")
    if dim==3:
      performanceanalysis_dd.plot3dDomainDecompositionOnLevel(l,numberOfRanks,args.domainoffset,args.domainsize,offset,volume,levels,nodes,performanceanalysis_output.getOutputDirectory(args.file)+"/dd")

  if dim==2:
    performanceanalysis_dd.plot2dDomainDecomposition(numberOfRanks,args.domainoffset,args.domainsize,offset,volume,levels,nodes,performanceanalysis_output.getOutputDirectory(args.file)+"/dd")
  #if dim==3:
  #  performanceanalysis_dd.plot3dDomainDecomposition(numberOfRanks,args.domainoffset,args.domainsize,offset,volume,levels,nodes,performanceanalysis_output.getOutputDirectory(args.file)+"/dd")


  performanceanalysis_dd.printNodeTable(outFile,numberOfRanks,parents,nodes)

  outFile.write( "<h2>Fork history</h2>" )
  performanceanalysis_dd.extractForkHistory(outFile,args.file,numberOfRanks)


beginIterations = performanceanalysis_parser.getBeginIterations(args.file,numberOfRanks>1)
performanceanalysis_global_plotter.plotWalltimeOverview(performanceanalysis_output.getOutputDirectory(args.file)+"/walltime",beginIterations)


(
  numberOfInnerLeafCells,
  numberOfInnerCells,
  numberOfOuterLeafCells,
  numberOfOuterCells,
  numberOfLocalCells,
  tTotal
) = performanceanalysis_parser.getCellsPerRank(args.file,numberOfRanks)

performanceanalysis_griddata.plotGridEntities(
  performanceanalysis_output.getOutputDirectory(args.file)+"/inner-leaf-cells",
  numberOfRanks,
  numberOfInnerLeafCells,
  tTotal)

performanceanalysis_griddata.plotGridEntities(
  performanceanalysis_output.getOutputDirectory(args.file)+"/inner-cells",
  numberOfRanks,
  numberOfInnerCells,
  tTotal)

performanceanalysis_griddata.plotGridEntities(
  performanceanalysis_output.getOutputDirectory(args.file)+"/outer-leaf-cells",
  numberOfRanks,
  numberOfOuterLeafCells,
  tTotal)

performanceanalysis_griddata.plotGridEntities(
  performanceanalysis_output.getOutputDirectory(args.file)+"/outer-cells",
  numberOfRanks,
  numberOfOuterCells,
  tTotal)

performanceanalysis_griddata.plotGridEntities(
  performanceanalysis_output.getOutputDirectory(args.file)+"/local-cells",
  numberOfRanks,
  numberOfLocalCells,
  tTotal)

if numberOfRanks>1:
  performanceanalysis_mpicommunication.plotMPIPhases(numberOfRanks,args.file,performanceanalysis_output.getOutputDirectory(args.file)+"/mpi-trace",1) 
  performanceanalysis_mpicommunication.plotMPIPhases(numberOfRanks,args.file,performanceanalysis_output.getOutputDirectory(args.file)+"/mpi-trace-detailed",16) 

performanceanalysis_output.processTemplateFile(
 scriptLocation + "/performanceanalysis.template",outFile,
 {"_IMAGE_DIRECTORY_" : performanceanalysis_output.getOutputDirectory(args.file)}
)

if numberOfRanks<=1:
  (timeStamps,measuredConcurrencyLevels,obtainedConcurrencyLevels,maxConcurrencyLevels,
    maxPotentialConcurrencyLevels,numberOfBackgroundTasks,timeAveragedConcurrencyLevels,
    timeAveragedPotentialConcurrencyLevels) = performanceanalysis_parser.getConcurrency(-1,args.file)

  performanceanalysis_concurrency.plotConcurrency(
    numberOfThreads, performanceanalysis_output.getOutputDirectory(args.file)+"/concurrency",
    timeStamps,measuredConcurrencyLevels,obtainedConcurrencyLevels,maxConcurrencyLevels,
    maxPotentialConcurrencyLevels,numberOfBackgroundTasks,timeAveragedConcurrencyLevels,
    timeAveragedPotentialConcurrencyLevels,beginIterations)

  fileName = performanceanalysis_output.getOutputDirectory(args.file)+"/concurrency"

  outFile.write( "<a href=\"" + fileName + ".pdf\" >" )
  outFile.write( "<img src=\"" + fileName + ".png\" />" )
  outFile.write( "</a>" )

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
