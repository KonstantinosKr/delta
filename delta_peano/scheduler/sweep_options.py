"""
.. module:: sweep_analysis
  :platform: Unix, Windows, Mac
  :synopsis: Submodule containing modules for analysing 
   
.. moduleauthor:: Dominic Etienne Charrier <dominic.e.charrier@durham.ac.uk>, 

:synopsis: Generate benchmark suites for ExaHyPE.
"""
import sys
import configparser
import csv
import collections

def parseList(string):
    """
    Decomposes strings like '"val1,val2",val3,"val4,val5"'
    into a list of strings:
    [ 'val1,val2' ,'val3', 'val4,val5' ]
    """
    for line in csv.reader([string],delimiter=","):
      values = line
      #for i,value in enumerate(values):
      #    values[i] = value.replace(" ","")
      return values

def parseEnvironment(config):
    """
    Parse the environment section.
    """
    environmentSpace = {}
    if "environment" in config and len(config["environment"].keys()):
        for key, value in config["environment"].items():
            environmentSpace[key] = parseList(value)

    return environmentSpace

def parseParameters(config):
    """
    Parse the parameters section.
    """
    parameterSpace = {}
    if "parameters" in config and len(config["parameters"].keys()):
        for key, value in config["parameters"].items():
            parameterSpace[key] = parseList(value)
            
        if "time-steps" not in parameterSpace:
            print("ERROR: 'order' missing in section 'parameters'.",file=sys.stderr)
            sys.exit()
    else:
        print("ERROR: Section 'parameters' is missing or empty! (Must contain at least 'dimension' and 'order'.)",file=sys.stderr)
        sys.exit()
    
    return parameterSpace

def parseOptionsFile(optionsFile,ignoreMetadata=False):
    configParser = configparser.ConfigParser()
    configParser.optionxform=str
    configParser.read(optionsFile)
    
    general          = dict(configParser["general"])
    deltaRoot        = general["delta_root"]
    outputPath       = general["output_path"]
    projectPath      = general["project_path"]
    projectName      = general["project_name"]

    buildFolderPath   = deltaRoot+"/"+outputPath+"/"
    scriptsFolderPath = deltaRoot+"/"+outputPath+"/" + "scripts"
    resultsFolderPath = "results"
    historyFolderPath = "history"
    
    jobs             = dict(configParser["jobs"])
    environmentSpace = parseEnvironment(configParser)
    parameterSpace   = parseParameters(configParser)

    nodeCounts = [x.strip() for x in jobs["nodes"].split(",")]
    taskCounts = [x.strip() for x in jobs["tasks"].split(",")]
    coreCounts = [x.strip() for x in jobs["cores"].split(",")]
    
    Options = collections.namedtuple("options", \
           ("general jobs environmentSpace parameterSpace "
            "deltaRoot outputPath projectPath projectName "
            "buildFolderPath scriptsFolderPath "
            "resultsFolderPath historyFolderPath "
            "nodeCounts taskCounts coreCounts"))
    
    options = Options(
      general          = general,
      jobs             = jobs,\
      environmentSpace = environmentSpace,\
      parameterSpace   = parameterSpace,\
      \
      deltaRoot      = deltaRoot,\
      outputPath       = outputPath,\
      projectPath      = projectPath,\
      projectName      = projectName,\
      \
      buildFolderPath   = buildFolderPath,\
      scriptsFolderPath = scriptsFolderPath,\
      resultsFolderPath = resultsFolderPath,\
      historyFolderPath = historyFolderPath,\
      \
      nodeCounts = nodeCounts,\
      taskCounts = taskCounts,\
      coreCounts = coreCounts\
    )
    
    return options
