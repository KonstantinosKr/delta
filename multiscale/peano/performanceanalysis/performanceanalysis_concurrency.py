import pylab


def plotConcurrency(threads, file, 
  timeStamps,measuredConcurrencyLevels,obtainedConcurrencyLevels,maxConcurrencyLevels,
  maxPotentialConcurrencyLevels,numberOfBackgroundTasks,timeAveragedConcurrencyLevels,
  timeAveragedPotentialConcurrencyLevels,beginIterations):
  pylab.clf()
  #pylab.gcf().set_size_inches( 1*40, 10 )
  DefaultSize = pylab.gcf().get_size_inches()
  pylab.gcf().set_size_inches( DefaultSize[0]*4, DefaultSize[1] )
  pylab.title( "Concurrency levels" )

  ax = pylab.gca()
  
  maximumValue = max(maxPotentialConcurrencyLevels)
  
  for t in timeStamps:
    pylab.plot( [t,t], [0,maximumValue], "--", color="#787878" )

  if len(timeStamps)>0:
    pylab.plot( [0,timeStamps[-1]], [threads,threads], ":", color="#787878" )
    pylab.plot( [0,timeStamps[-1]], [1,1], ":", color="#787878" )

  pylab.plot(timeStamps,measuredConcurrencyLevels,     "-",  label="cpu time/real time",                 color="#000000" )
  pylab.plot(timeStamps,maxConcurrencyLevels,          "-",  label="max concurrency level",              color="#ff0000" )
  pylab.plot(timeStamps,maxPotentialConcurrencyLevels, "--", label="max pot. concurrency level",         color="#bb2323" )
  pylab.plot(timeStamps,numberOfBackgroundTasks,       "-",  label="no of pending background tasks",     color="#00ff00" )
  pylab.plot(timeStamps,timeAveragedConcurrencyLevels, "-",  label="time averaged concurrency levels",   color="#0000ff" )
  pylab.plot(timeStamps,timeAveragedPotentialConcurrencyLevels, "--", label="time averaged pot. concurrency levels", color="#2323ff" )
    
  ax.autoscale_view()
  ax.set_yscale('symlog', basey=2)

  try:
    pylab.legend(fontsize=9, framealpha=0.5)
  except:
    # old pylab version
    l = pylab.legend(prop={'size':9})
    l.get_frame().set_alpha(0.5)
  
  #pylab.yticks( 
  #  [1,2,8,12,16,18,24,60,72,120,180,240,480], 
  #  ["1","2","8","12","16","18","24","60","72","120","180","240","480"] 
  #)
  
  pylab.xlabel('t')
  pylab.ylabel('Concurrency level')
  pylab.grid(True)
  pylab.savefig( file + ".png", transparent = True, bbox_inches = 'tight', pad_inches = 0, dpi=80 )
  pylab.savefig( file + ".pdf", transparent = True, bbox_inches = 'tight', pad_inches = 0 )

  pylab.gcf().set_size_inches( DefaultSize[0], DefaultSize[1] )
  