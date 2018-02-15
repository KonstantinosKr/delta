#!/usr/bin/env python3
def parseArgument(argv,i):
    if i<len(argv):
        return argv[i]
    else:
        return None

def haveToPrintHelpMessage(argv):
    """
    Check if we have to print a help message.
    """
    result = parseArgument(argv,1)==None
    for arg in argv:
        result = result or ( arg=="-help" or arg=="-h" )
    return result

def parseList(string):
    """
    Decomposes strings like '"val1,val2",val3,"val4,val5"'
    into a list of strings:
    [ 'val1,val2' ,'val3', 'val4,val5' ]
    """
    for line in csv.reader([string],delimiter=","):
      values = line
      return values

def parseParameterSpace(config,section):
    """
    Parse the environment section.
    """
    parameterSpace = collections.OrderedDict()
    if section in config and len(config[section].keys()):
        for key, value in config[section].items():
            parameterSpace[key] = parseList(value)
    else:
        print("ERROR: Section '"+section+"' must not be empty!",file=sys.stderr)
        sys.exit()
    
    return parameterSpace

def dictProduct(dictionary):
    """
    Computes the Cartesian product of a dictionary of lists as 
    a list of dictionaries.
    
    Gladly copied this code from:
    https://stackoverflow.com/questions/5228158/cartesian-product-of-a-dictionary-of-lists
    
    Example input:
    options = {"number": [1,2,3], "color": ["orange","blue"] }
    
    Example output:
    [ {"number": 1, "color": "orange"},
      {"number": 1, "color": "blue"},
      {"number": 2, "color": "orange"},
      {"number": 2, "color": "blue"},
      {"number": 3, "color": "orange"},
      {"number": 3, "color": "blue"}
    ]
    """
    return (collections.OrderedDict(zip(dictionary, x)) for x in itertools.product(*dictionary.values()))

def column(matrix, i):
    return [row[i] for row in matrix]

def createPlots():
    """
    Create a plot per plotDict. 
    Per plot, plot all rows found for the elements of the perPlotSpace.
    """
    TINY_SIZE   = 7
    SMALL_SIZE  = 11
    MEDIUM_SIZE = 11
    BIGGER_SIZE = 11
    
    pyplot.rc('font', size=SMALL_SIZE)          # controls default text sizes
    pyplot.rc('axes', titlesize=SMALL_SIZE)     # fontsize of the axes title
    pyplot.rc('axes', labelsize=MEDIUM_SIZE)    # fontsize of the x and y labels
    pyplot.rc('xtick', labelsize=SMALL_SIZE)    # fontsize of the tick labels
    pyplot.rc('ytick', labelsize=SMALL_SIZE)    # fontsize of the tick labels
    pyplot.rc('legend', fontsize=SMALL_SIZE)    # legend fontsize
    
    pyplot.rc('text', usetex=True)
    pyplot.rc('font', family='serif')
    
    for plotDict in dictProduct(plotsSpace):
        for yScale in yScales:
            # create new plot
            figure = pyplot.figure()
            axes   = figure.add_subplot(111)
            
            positions  = []
            labels     = []
            dataPoints = []
            for perPlotDict in dictProduct(perPlotSpace):
                def tableFilter(row):
                    hit = True
                    for key,index in indexMappingPlots.items():
                        hit = hit and row[index]==plotDict[key]
                    for key,index in indexMappingPerPlot.items():
                        hit = hit and row[index]==perPlotDict[key]
                    return hit
                
                filtered = list(filter(tableFilter,tableData))
                filterKeyAsStringPart1 = ",".join("%s=%s" %  pair for pair in plotDict.items())
                filterkeyAsStringPart2 = ",".join("%s=%s" %  pair for pair in perPlotDict.items())
                filterKeyAsString      = filterKeyAsStringPart1 + "," + filterkeyAsStringPart2
                if len(filtered)>1:
                    print("ERROR: Parameter combinations are not unique!",file=sys.stderr)
                    print("ERROR: Found multiple rows for filter key=("+filterKeyAsString+").",file=sys.stderr)
                    print("ERROR: Differing column values:",file=sys.stderr)
                    for index,name in enumerate(columnNames):
                        values = set(column(filtered,index))
                        if len(values)>1:
                            print("ERROR: "+name+"={"+",".join(values)+"}",file=sys.stderr)
                    sys.exit()
                elif len(filtered)==1:
                    positions.append(len(positions))
                    labels.append(""+"-".join(perPlotDict.values())+"")
                    dataPoints.append(float(filtered[0][dataColumnIndex]))
                elif len(filtered)==0:
                    print("WARNING: Found no rows for key=("+filterKeyAsString+")!",file=sys.stderr)
            
            if len(positions):
                # plot
                container = axes.bar(positions,dataPoints,width=0.8,color="0.8",edgecolor="0.8",align='center',log=(yScale=="log"),label=labels)
                axes.grid(True, which="both")
                
                # x axis
                axes.get_xaxis().set_visible(False)
                xMargin = 0.6
                xLimits = [-xMargin,len(positions)-1+xMargin]
                axes.set_xlim(xLimits)
                
                # y axis and annotate the bar chart
                dataMin = min(dataPoints)
                dataMax = max(dataPoints)
                iMin    = dataPoints.index(dataMin)
                iMax    = dataPoints.index(dataMax)
                
                colours = ["0.8","0.8"]
                if   best=="min":
                  colours = ["g", "r"]
                elif best=="max":
                  colours = ["r", "g"]
                container.patches[iMin].set_color(colours[0])
                container.patches[iMax].set_color(colours[1])
                
                axes.set_ylabel(yLabel)
                if (yScale=="linear"):
                    axes.ticklabel_format(axis="y", style="sci", scilimits=(-2,2))
                    axes.set_ylim([0,dataMax*1.05])
                else:
                    yMin = 10**math.floor(math.log10(dataMin))
                    yMax = max(dataMax*1.05,2.2*yMin)
                    axes.set_ylim([yMin,yMax])
                    
                for i,x in enumerate(positions):
                    xTrans = ( xMargin + float(x) ) / (xLimits[1]-xLimits[0])
                    label  = labels[i]
                    axes.text(xTrans,0.05,"%s" % label,ha='center', va='bottom',fontweight="bold",fontsize=TINY_SIZE,rotation=90,transform=axes.transAxes)
                
                if not os.path.exists(plotFolderPath):
                    print("create directory "+plotFolderPath)
                    os.makedirs(plotFolderPath)
                
                # write files
                figure.set_size_inches(4.90,4.90)
                #figure.set_size_inches(2.40,2.20) # width: 0.470 * SIAM SISC \textwidth (=5.125in)
                
                filename = plotFolderPath + "/" + plotPrefix + "-" + "-".join(plotDict.values())
                figure.savefig('%s-%s.pdf' % (filename,yScale), bbox_inches="tight")
                pyplot.close(figure)
                print("created plot: %s-%s.pdf" % (filename,yScale))

def renderPDF():
    """
    Render a LaTeX document.
    """
    latexFigureTemplate = \
r"""
\begin{figure}
\centering
\includegraphics[scale=1.0]{{{file}}}
\caption{{{caption}}}
\end{figure}
"""

    latexFileTemplate = \
r"""
\title{{{title}}}
\author{{{author}}}
\date{\today}

\documentclass[11pt]{article}

\usepackage{amssymb}
\usepackage{graphicx}
\usepackage[justification=justified,singlelinecheck=false]{caption}
%\usepackage{layouts}

\begin{document}
\maketitle

%\printinunitsof{in}\prntlen{\textwidth}

{{body}}

\end{document}
"""
    
    latexFileContent = latexFileTemplate;
    latexFileContent = latexFileContent.replace("{{title}}",plotPrefix)
    latexFileContent = latexFileContent.replace("{{author}}",os.environ["USER"])
    
    body = ""
    counter = 0
    for plotDict in dictProduct(plotsSpace):
        plotFileName = plotFolder + "/" + plotPrefix + "-" + "-".join(plotDict.values())
        
        if os.path.exists(outputPath + "/" + plotFileName+"-linear.pdf"):
            for yScale in yScales:
                renderedFigure = latexFigureTemplate;
                
                caption  = "\\textbf{"+", ".join("%s: %s" %  pair for pair in plotDict.items())
                caption += " (y-scale: "+yScale+"):} "
                caption += "The bars show measurements for different values of the tuples ("
                caption += ",".join("%s" % item for item in perPlotSpace.keys()) 
                caption += ")."
                renderedFigure = renderedFigure.replace("{{caption}}",caption)
                
                ending = "-"+yScale+".pdf"
                body += renderedFigure.replace("{{file}}",plotFileName+ending) + "\n\n"
                if counter % 10 == 0:
                    body += r"\clearpage"
                counter += 1
    latexFileContent = latexFileContent.replace("{{body}}",body)
    
    latexFileName = plotPrefix + ".tex"
    with open(outputPath + "/" + latexFileName, 'w') as latexFile:
         latexFile.write(latexFileContent)
    print("created tex file: "+outputPath + "/" + latexFileName)
    command = "( cd "+outputPath+" && pdflatex "+latexFileName + ")"
    print(command)
    process = subprocess.Popen([command], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    (output, err) = process.communicate()
    process.wait()
    print("created PDF document: "+outputPath+"/"+latexFileName.replace(".tex",".pdf"))

def getDataColumnIndex():
    """
    Returns the parameter to index mappings for the keys of the given 
    parameterSpace dictionary.
    Performs an one-sided on-the-fly check if all keys are column names.
    """
    columnIndex = 0;
    if dataColumnName in columnNames:
        columnIndex = columnNames.index(dataColumnName)
    else:
      print("ERROR: program aborted since data column to plot "+dataColumnName+" is not a column name of the table.",file=sys.stderr)
      print("ERROR: found table column names: "+",".join(columnNames),file=sys.stderr)
      sys.exit()
    
    return columnIndex

def createParameterKeysToColumnIndexMapping(parameterSpace):
    """
    Returns the parameter to index mappings for the keys of the given 
    parameterSpace dictionary.
    Performs an one-sided on-the-fly check if all keys are column names.
    """
    indexMapping   = {}
    
    success = True
    columnNamesSet = set(columnNames)
    for key in parameterSpace:
        if key in columnNamesSet:
            indexMapping[key] = columnNames.index(key)
        else:
            print("ERROR: parameter key '"+key+"' is not a column name of the table!",file=sys.stderr)
            success = False
    if not success:
        print("ERROR: program aborted since not all parameter keys are a column name of the table.",file=sys.stderr)
        print("ERROR: found table column names: "+",".join(columnNames),file=sys.stderr)
        sys.exit()
    
    return indexMapping

if __name__ == "__main__":
    import sys,os
    import csv
    import configparser
    import collections
    import itertools
    import subprocess
    
    import matplotlib
    import matplotlib.pyplot as pyplot
    import matplotlib.ticker as ticker
    
    import math
    
    if haveToPrintHelpMessage(sys.argv):
        info = \
"""tableplotter.py:

run:

./tableplotter.py myoptions.ini mytable.csv

NOTE: The order of the parameters in the section 'per_plot' is preserved. 
      You thus have some control over the position of bars in the resulting diagrams.
"""
        print(info) # correctly indented
        sys.exit()
    
    # read options
    optionsFilePath = parseArgument(sys.argv,1)
    tablePath       = parseArgument(sys.argv,2)
    
    configParser = configparser.ConfigParser()
    configParser.optionxform=str
    configParser.read(optionsFilePath)
    
    plotPrefix      = configParser["output"]["prefix"].replace("\"","")
    outputPath      = configParser["output"]["path"].replace("\"","")
    plotFolder      = "plots"
    plotFolderPath  = outputPath + "/" + plotFolder
    
    dataColumnName = configParser["to_plot"]["data"].replace("\"","")
    yLabel         = configParser["to_plot"]["label"].replace("\"","")
    yScales        = parseList(configParser["to_plot"]["scale"])
    best           = configParser["to_plot"]["best"].replace("\"","")
    
    plotsSpace     = parseParameterSpace(configParser,"plots")
    perPlotSpace   = parseParameterSpace(configParser,"per_plot")
    
    # read table
    tableFile   = open(tablePath, 'r')
    columnNames = next(tableFile)
    columnNames = columnNames.strip()
    columnNames = columnNames.split(";")
    tableData   = list(csv.reader(tableFile,delimiter=";"))
    tableFile.close()
    
    dataColumnIndex     = getDataColumnIndex()
    indexMappingPlots   = createParameterKeysToColumnIndexMapping(plotsSpace)
    indexMappingPerPlot = createParameterKeysToColumnIndexMapping(perPlotSpace)
    
    # plot
    createPlots()
    renderPDF()
