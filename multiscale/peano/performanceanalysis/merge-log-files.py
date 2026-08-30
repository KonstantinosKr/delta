import sys
import re


#
# main
#
if len(sys.argv)!=3:
  print "Usage: python merge-log-files.py logfilename ranks"
  print ""
  print "logfilename is the name of the log files without the rank-x- prefix."
  print "ranks is the number of ranks you have used for your simulation. If "
  print "four MPI ranks have been used and you pass in a log file name of "
  print "myfile.log, then the script searches for rank-0-myfile.log, "
  print "rank-1-myfile.log, rank-2-myfile.log and rank-3-myfile.log. "
  print ""
  print "(C) 2015 Tobias Weinzierl"
  quit()
  
  
  
filenameprefix = sys.argv[1]
ranks          = int( sys.argv[2] )

print "open fused output file merged-" + filenameprefix
outputFile = open( "merged-" + filenameprefix,  "w" )


inputFiles = []
for rank in range(0,ranks):
  filename = "rank-" + str(rank) + "-" + filenameprefix
  print "read " + filename
  with open(filename) as f:
    inputFiles.append( f.readlines() )

print "read in all " + str(len(inputFiles)) + " input files"

timeStamp = 0
while timeStamp<sys.float_info.max:
  timeStamp                 = sys.float_info.max
  rankWithSmallestTimeStamp = 0
  for rank in range(0,ranks): 
    searchPattern          = "([0-9]\.?[0-9]*).*"
    if len(inputFiles[rank])>0:
      firstLineInCurrentFile = inputFiles[rank][0]
      m = re.search( searchPattern, firstLineInCurrentFile )
      if (m):
        currentTimeStamp       = float(m.group(1))
        if currentTimeStamp < timeStamp:
          timeStamp                 = currentTimeStamp
          rankWithSmallestTimeStamp = rank
      else:
        print "ERROR: line in " + str(rank) + "th intput file does not hold time stamp. Line " + firstLineInCurrentFile
  if timeStamp<sys.float_info.max:
    print "- t=" + "%.4e"%(timeStamp) + ": take message from rank " + str( rankWithSmallestTimeStamp )
    outputFile.write( inputFiles[rankWithSmallestTimeStamp][0] )
    inputFiles[rankWithSmallestTimeStamp].pop(0)
  