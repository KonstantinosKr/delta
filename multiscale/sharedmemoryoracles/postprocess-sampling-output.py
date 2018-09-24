import sys
import re
import pylab 
import os


Symbol = [ 
  "-s", "-o", "->", 
  "-<", "-^", "-v", 
  "-d", "-x", "-*",
  "--s", "--o", "-->", 
  "--<", "--^", "--v", 
  "--d", "--x", "--*",
]
Colour = [
  "#ff0000", "#00ff00", "#0000ff",
  "#ffff00", "#ff00ff", "#00ffff",
  "#ffaa33", "#ff33aa", "#aaff33",
  "#33ffaa", "#aa33ff", "#33aaff",
]
AlphaValue=0.5


def processMeasurement(adapter):
  currentColourAndSymbol = 0
    
  print  "process adapter " + str(adapter) 
  
  pylab.clf()

  searchPattern = "adapter-number=" + str(adapter) 
  line          = inputFile.readline()
  while not line=="" and not re.search( searchPattern, line ):
    line = inputFile.readline()
    #print "read " + line
   
  line            = inputFile.readline()
  numberOfSamples = int(line.split( "=" )[1])
  line            = inputFile.readline()
  line            = inputFile.readline()
       
  while not re.search( "end OracleForOnePhaseWithGrainSizeSampling", line):
    
    phaseName = line.split( "=" )[0]

    print "read data of phase " + phaseName
        
    xValues = []
    yValues = []

    entries = line.split( "bin" )
    for binEntry in range(1,len(entries)):
      grainSize = entries[binEntry].split( "," )[1].split( ")" )[0] 
      time      = entries[binEntry].split( "," )[1].split( "(" )[-1] 
      #print "process entry " + entries[binEntry] + " with grain size " + grainSize + " and timing " + time
      xValues.append( float(grainSize) )
      yValues.append( float(time)  )

    pylab.plot(xValues, yValues, Symbol[currentColourAndSymbol],  label=phaseName, markersize=10, color=Colour[currentColourAndSymbol] )
    currentColourAndSymbol = currentColourAndSymbol+1

    line = inputFile.readline()
  
  try:     
    outputFileName = sys.argv[1] + "-adapter+" + str(adapter)

    pylab.grid(True)
    pylab.ylabel('[t]=s')
    pylab.xlabel('grain size')
    pylab.legend(fontsize=8, framealpha=0.5, loc='best')
  
    pylab.xscale( 'linear' )
    pylab.yscale( 'linear' )
    thisName = outputFileName + "-linear-linear"
    pylab.savefig( thisName + ".png")
    pylab.savefig( thisName + ".pdf")
    htmlOverview.write( "<a href=\"" + thisName + ".pdf\"><img src=\"" + thisName + ".png\" /></a>" );
  
    pylab.xscale( 'linear' )
    pylab.yscale( 'log' )
    thisName = outputFileName + "-linear-log"
    pylab.savefig( thisName + ".png")
    pylab.savefig( thisName + ".pdf")
    htmlOverview.write( "<a href=\"" + thisName + ".pdf\"><img src=\"" + thisName + ".png\" /></a>" );
  
    pylab.xscale( 'log' )
    pylab.yscale( 'log' )
    thisName = outputFileName + "-log-log"
    pylab.savefig( thisName + ".png")
    pylab.savefig( thisName + ".pdf")
    htmlOverview.write( "<a href=\"" + thisName + ".pdf\"><img src=\"" + thisName + ".png\" /></a>" );

    pylab.xscale( 'symlog' )
    pylab.yscale( 'log' )
    thisName = outputFileName + "-symlog-log"
    pylab.savefig( thisName + ".png")
    pylab.savefig( thisName + ".pdf")
    htmlOverview.write( "<a href=\"" + thisName + ".pdf\"><img src=\"" + thisName + ".png\" /></a>" );
  except:
    print "Unexpected error:", sys.exc_info()[0]

  return





#
#
#   main
# ========
#
#    
if (len(sys.argv)!=2):
  print "usage: python ../postprocess-sampling-output.py outputfile"
  #print "  no-of-adapter  number of adapters you have in your project (see your specification file)"
  #print "  no-of-phases   number of code phases that are tuned via an oracle. 19 by default (cmp OracleForOnePhase)"
  quit()

htmlOverview = open( sys.argv[1] + ".html",  "w" )
htmlOverview.write( "<h1>" + sys.argv[1] + "</h1>" );

inputFile = open(sys.argv[1], "r" )

line                  = inputFile.readline()
totalNumberOfOracles  = int( line.split("=")[1] )
line                  = inputFile.readline()
oraclesForSteering    = int( line.split("=")[1] )

htmlOverview.write( "Number of oraces=" + str(totalNumberOfOracles) + " (incl. oracles required for repository/algorithm steering)" );
htmlOverview.write( "<br />" );
htmlOverview.write( "Oracles required for repository steering=" + str(oraclesForSteering) );

htmlOverview.write( 
  "<p>The first " + str(oraclesForSteering) + " oracles have to be ignored as they are used by the kernel to control the MPI ranks. The " + str(oraclesForSteering) + "th oracle thus corresponds to the first adapter in your specification file.</p>" 
);
htmlOverview.write( 
  "<p>The solid lines present timings for particular problem sizes plus grain sizes. The semitransparent symbols denote the standard deviation belonging to the measurements. Each pair of symbols plus symbols with lines studies one particular problem size over multiple grain sizes. The legend gives the problem size.</p>" 
);
htmlOverview.write( 
  "<p>By default, we create each plot with four different axis scales. Please klick on the respective image in your browser to swith to the PDF version. The PDF file is a vector file, i.e. you can zoom into it.</p>" 
);

htmlOverview.write( "<h3>Table of content:</h3>" );
htmlOverview.write( "<ul>" );
for adapter in range(oraclesForSteering,totalNumberOfOracles):
  htmlOverview.write( "<li><a href=\"#adapter-" + str(adapter) + "\">Adapter " + str(adapter) + "</a></li>" );
htmlOverview.write( "</ul>" );

for adapter in range(oraclesForSteering,totalNumberOfOracles):
  htmlOverview.write( "<h3 id=\"adapter-" + str(adapter) + "\">Adapter " + str(adapter) + "</h3>" );
  processMeasurement(adapter)
  