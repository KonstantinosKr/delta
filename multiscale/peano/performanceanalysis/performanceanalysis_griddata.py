import pylab


def plotGridEntities(
  outputFileName,
  numberOfRanks,
  numberOfLocalCells,
  tTotal
):
  AlphaValue = 1.0/(numberOfRanks+1.0)
  
  pylab.clf()
  startRank = 1
  if (numberOfRanks==1):
    startRank = 0
  for rank in range(startRank,numberOfRanks):
    x = pylab.arange(0, len(numberOfLocalCells[rank]), 1.0)
    if len(tTotal[rank])==len(numberOfLocalCells[rank]):
      pylab.plot(tTotal[rank], numberOfLocalCells[rank], 'o',  color='#000000', alpha=AlphaValue, markersize=10)
  pylab.xlabel('t')
  pylab.savefig( outputFileName + ".png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( outputFileName + ".pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )
