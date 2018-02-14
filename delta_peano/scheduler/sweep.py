#!/usr/bin/env python3
"""
.. module:: sweep
  :platform: Unix, Windows, Mac
  :synopsis: Generate benchmark suites for ExaHyPE.

.. moduleauthor:: Dominic Etienne Charrier <dominic.e.charrier@durham.ac.uk>,

:synopsis: Generate benchmark suites for ExaHyPE.
"""
def parseArgument(argv,i):
    if i<len(argv):
        return argv[i]
    else:
        return None

def haveToPrintHelpMessage(argv):
    """
    Check if we have to print a help message.
    """
    result = parseArgument(argv,2) not in subprograms or \
             parseArgument(argv,1)==None
    for arg in argv:
        result = result or ( arg=="-help" or arg=="-h" )
    return result

def dictProduct(dicts):
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
    return (dict(zip(dicts, x)) for x in itertools.product(*dicts.values()))

def hashDictionary(dictionary):
    """
    Hash a dictionary. Sort the dictionary according
    to the keys beforehand.
    """
    chain = ""
    for key,value in sorted(dictionary.items()):
        chain += key+","+value+";"

    return hashlib.md5(chain.encode()).hexdigest()

def clean(subFolder=""):
    """
    Clean the complete output folder or just a subfolder
    if specified.
    """
    folder = deltaRoot+"/"+outputPath+"/"+subFolder
    print("rm -r "+folder)
    subprocess.call("rm -r "+folder, shell=True)

def build(buildOnlyMissing=False):
    """
    Build the executables.

    Args:
    buildOnlyMissing(bool):
       Build only missing executables.
    """
    print("currently loaded modules:")
    subprocess.call("module list",shell=True)
    print("")
    print("delta build environment:")

    if not os.path.exists(buildFolderPath):
        print("create directory "+buildFolderPath)
        os.makedirs(buildFolderPath)

    firstIteration = True
    executables=0
    for environmentDict in dictProduct(environmentSpace):
        for key,value in environmentDict.items():

            if not firstIteration:
                command = "make clean"
                print(command)
                process = subprocess.Popen([command], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
                (output, err) = process.communicate()
                process.wait()

            oldExecutable = deltaRoot + "/" + value
            executable = buildFolderPath + "/" + value

            if not os.path.exists(executable) or not buildOnlyMissing:
                print("building executable for " + "environment="+str(environmentDict), file=sys.stderr)

                # clean application folder only
                command = "rm -r *.o cipofiles.mk cfiles.mk ffiles.mk"
                print(command)
                process = subprocess.Popen([command], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
                process.communicate()
                process.wait()

                if firstIteration:
                    command = "make" + " -C" + deltaRoot + " clean"
                    print(command)
                    process = subprocess.Popen([command], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
                    (output, err) = process.communicate()
                    process.wait()
                    firstIteration = False

                # call make
                make_threads=general["make_threads"]
                makeCommand="make " + "-C " + deltaRoot + " " + value + " -j"+make_threads
                print(makeCommand,end="",flush=True)
                process = subprocess.Popen([makeCommand], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
                (output, makeErr) = process.communicate()
                process.wait()
                if "build of delta successful" in str(output):
                    print(" [OK]")
                else:
                    print(" [FAILED]")
                    print("make errors/warnings=\n"+makeErr.decode('UTF-8'),file=sys.stderr)
                    sys.exit()

                moveCommand   = "mv "+oldExecutable+" "+executable
                print(moveCommand)
                subprocess.call(moveCommand,shell=True)
                print("--------------------------------------------------------------------------------")
                print("make errors/warnings=\n"+makeErr.decode('UTF-8'),file=sys.stderr)
                print("--------------------------------------------------------------------------------")
                executables+=1
            else:
                print("skipped building of '"+executable+"' as it already exists.")

    print("built executables: "+str(executables))

def verifyAllExecutablesExist(justWarn=False):
    """
    Verify that all executables exist.
    """

    messageType = "ERROR"
    if justWarn:
      messageType = "WARNING"

    if not justWarn and not os.path.exists(buildFolderPath):
        print("ERROR: build folder '"+buildFolderPath+"' doesn't exist! Please run subprogram 'build' beforehand.",file=sys.stderr)
        sys.exit()

    allExecutablesExist = True
    for environmentDict in dictProduct(environmentSpace):
        for key, value in environmentDict.items():

            executable = buildFolderPath #+ "/"+value
            print(buildFolderPath)
            if not os.path.exists(executable):
                allExecutablesExist = False
                print(messageType+ ": application for " + \
                      "environment="+str(environmentDict) + \
                      " does not exist! ('"+executable+"')",file=sys.stderr)

    if not justWarn and not allExecutablesExist:
        print("ERROR: subprogram failed as not all executables exist. Please adopt your options file according to the error messages.\n" + \
              "       Then rerun the 'build' subprogram.",file=sys.stderr)
        sys.exit()

def renderJobScript(templateBody,environmentDict,parameterDict,jobs,
                    jobName,jobFilePath,outputFileName,appName,
                    nodes,tasks,cores):
    """
    Render a job script.
    """
    renderedFile = templateBody

    context = {}
    # mandatory
    context["nodes"] = nodes
    context["tasks"] = tasks
    context["cores"] = cores
    context["job_name"] = jobName
    context["output_file"] = outputFileName

    context["environment"] = json.dumps(environmentDict).replace("\"","\\\"")
    context["parameters"]  = json.dumps(parameterDict).replace("\"","\\\"")

    context["job_file"]  = jobFilePath

    commandLineArguments = ""
    for key,value in parameterDict.items():
        #commandLineArguments += " -"+key+" "+value
        commandLineArguments += " " + value
    context["app"] = appName+commandLineArguments

    consistent = True
    # verify all mandatory(!) sweep options are defined in template
    keysInTemplate = [m.group(2) for m in re.finditer("(\{\{((\w|-)+)\}\})",templateBody)]
    for key in context:
        if key not in keysInTemplate:
            consistent = False
            print("ERROR: parameter '{{"+key+"}}' not found in job script template!",file=sys.stderr)

    # put optional sweep options in context
    context["mail"]  = jobs["mail"]
    context["time"]  = jobs["time"]
    context["ranks"] = str(int(nodes)*int(tasks))

    # now verify template parameters are defined in options file
    for key in keysInTemplate:
        if key not in context:
            consistent = False
            print("ERROR: job script template parameter '{{"+key+"}}' not defined in sweep options file!",file=sys.stderr)
    if not consistent:
        print("ERROR: subprogram aborted as job script template and sweep options file are inconsistent.",file=sys.stderr)
        sys.exit()

    for key,value in context.items():
        renderedFile = renderedFile.replace("{{"+key+"}}", value)

    return renderedFile

def verifySweepAgreesWithHistoricExperiments():
    """
    If there are any previous experiments ensure that the sweep
    parameter spaces contain the same axes.
    """
    if os.path.exists(historyFolderPath):
        previousSweeps = [f for f in os.listdir(historyFolderPath) if f.endswith(".ini")]
        print(previousSweeps)

        for f in previousSweeps:
            otherOptions = sweep_options.parseOptionsFile(historyFolderPath+"/"+f)

            otherEnvironmentSpace = otherOptions.environmentSpace
            otherParameterSpace   = otherOptions.parameterSpace

            environmentSpaceIntersection = set(environmentSpace.keys()).intersection(otherEnvironmentSpace.keys())
            parameterSpaceIntersection   = set(parameterSpace.keys()).intersection(otherParameterSpace.keys())
            if len(set(environmentSpace.keys()))!=len(environmentSpaceIntersection):
                print("ERROR: subprogram failed as environment variables differ from previous experiments found in the output folder.",file=sys.stderr)
                print("environment variables found for CURRENT experiment: " + ", ".join(sorted(environmentSpace.keys())))
                print("environment variables used in PREVIOUS experiment:  " + ", ".join(sorted(otherEnvironmentSpace.keys())))
                sys.exit()
            if len(set(parameterSpace.keys()))!=len(parameterSpaceIntersection):
                print("ERROR: subprogram failed as parameters differ from previous experiments found in the output folder.",file=sys.stderr)
                print("parameters found for CURRENT experiment: "+ ", ".join(sorted(parameterSpace.keys())))
                print("parameters used in PREVIOUS experiment:  "+ ", ".join(sorted(otherParameterSpace.keys())))
                sys.exit()

def generateScripts():
    """
    Generate spec files and job scripts.
    """
    cpus       = jobs["num_cpus"]

    jobScriptTemplatePath = general["job_template"]

    jobScriptTemplate = None
    try:
        with open(jobScriptTemplatePath, "r") as templateFile:
            jobScriptTemplate=templateFile.read()
    except IOError:
        print("ERROR: couldn\'t open template file: "+jobScriptTemplatePath,file=sys.stderr)
        sys.exit()

    if not os.path.exists(scriptsFolderPath):
        print("create directory "+scriptsFolderPath)
        os.makedirs(scriptsFolderPath)

    # check if required executables exist
    verifyAllExecutablesExist(True)

    # generate job scrips
    jobScripts = 0
    for nodes in nodeCounts:
        for tasks in taskCounts:
            for parsedCores in coreCounts:
                cores = parsedCores
                if parsedCores=="auto":
                    cores=str(int(int(cpus) / int(tasks)))
                for environmentDict in dictProduct(environmentSpace):
                    for key, value in environmentDict.items():
                        for parameterDict in dictProduct(parameterSpace):
                            parameterDictHash = hashDictionary(parameterDict)

                            executable   = "./" + value

                            jobName        = value + "-" + parameterDictHash + "-n" + nodes + "-t"+tasks+"-c"+cores
                            jobFilePrefix  = scriptsFolderPath + "/" + jobName
                            jobFilePath    = jobFilePrefix + ".job"
                            outputFileName = resultsFolderPath + "/" + jobName + ".out"

                            jobScriptBody = renderJobScript(jobScriptTemplate,environmentDict,parameterDict,jobs,
                                                            jobName,jobFilePath,outputFileName,executable,
                                                            nodes,tasks,cores)
                            with open(jobFilePath, "w") as jobFile:
                                jobFile.write(jobScriptBody)

                            jobScripts+=1

    print("generated job scripts: "+str(jobScripts))

def verifyAllJobScriptsExist():
    """
    Verify that all job scripts exist.
    """
    cpus       = jobs["num_cpus"]

    if not os.path.exists(scriptsFolderPath):
        print("ERROR: job script folder '"+scriptsFolderPath+"' doesn't exist! Please run subprogram 'scripts' beforehand.",file=sys.stderr)
        sys.exit()

    allJobScriptsExist = True
    for nodes in nodeCounts:
        for tasks in taskCounts:
            for parsedCores in coreCounts:
                cores = parsedCores
                if parsedCores=="auto":
                    cores=str(int(int(cpus) / int(tasks)))
                for environmentDict in dictProduct(environmentSpace):
                    for key, value in environmentDict.items():
                        for parameterDict in dictProduct(parameterSpace):
                            parameterDictHash = hashDictionary(parameterDict)

                            jobName        = value + "-" + parameterDictHash + \
                                             "-n" + nodes + "-t"+tasks+"-c"+cores+"-r"
                            jobFilePrefix  = scriptsFolderPath + "/" + jobName
                            jobFilePath    = jobFilePrefix + ".job"

                            if not os.path.exists(jobFilePath):
                                allJobScriptsExist = False
                                print("ERROR: job script for " + \
                                      "environment="+str(value)+ \
                                      ", parameters="+str(parameterDict) + \
                                      ", nodes="+nodes + \
                                      ", tasks="+tasks + \
                                      ", cores="+cores + \
                                      " does not exist! ('"+jobFilePath+"')",file=sys.stderr)
    if not allJobScriptsExist:
        print("ERROR: subprogram failed! Please adopt your sweep options file according to the error messages.\n" + \
              "       Then rerun the 'scripts' subprogram.")
        sys.exit()

def hashSweep():
    chain = ""
    for value in nodeCounts:
        chain += value+";"
    for value in taskCounts:
        chain += value+";"
    for value in coreCounts:
        chain += value+";"

    for environmentDict in dictProduct(environmentSpace):
        chain += hashDictionary(environmentDict)
    for parameterDict in dictProduct(parameterSpace):
        chain += hashDictionary(parameterDict)

    return hashlib.md5(chain.encode()).hexdigest()

def extractJobId(processOutput):
    jobId = "unknown"
    lines = processOutput.split("\n")
    for line in lines:
        # SLURM
        # hamilton: "Submitted batch job 67586"
        # coolmuc:  "Submitted batch job 67586 on cluster mpp3"
        if "Submitted batch job " in line:
            jobId = line.strip().split(" ")[3]
    return jobId

def submitJobs():
    """
    Submit all jobs spanned by the options.
    """
    jobSubmissionTool    = general["job_submission"]

    cpus = jobs["num_cpus"]

    # verify everything is fine
    verifyAllExecutablesExist()
    verifyAllJobScriptsExist()

    if not os.path.exists(resultsFolderPath):
        print("create directory "+resultsFolderPath)
        os.makedirs(resultsFolderPath)

    # loop over job scrips
    jobIds = []
    for nodes in nodeCounts:
        for tasks in taskCounts:
            for parsedCores in coreCounts:
                cores = parsedCores
                if parsedCores=="auto":
                    cores=str(int(int(cpus) / int(tasks)))
                for environmentDict in dictProduct(environmentSpace):
                    for key, value in environmentDict.items():
                        for parameterDict in dictProduct(parameterSpace):
                            parameterDictHash = hashDictionary(parameterDict)

                            jobName        = value + "-" + parameterDictHash + "-n" + nodes + "-t"+tasks+"-c"+cores+"-r"
                            jobFilePrefix  = scriptsFolderPath + "/" + jobName
                            jobFilePath    = jobFilePrefix + ".job"

                            command=jobSubmissionTool + " " + jobFilePath
                            print(command)
                            process = subprocess.Popen([command], stdout=subprocess.PIPE, shell=True)
                            (output, err) = process.communicate()
                            process.wait()
                            jobIds.append(extractJobId(output.decode("UTF_8")))

    if not os.path.exists(historyFolderPath):
        print("create directory "+historyFolderPath)
        os.makedirs(historyFolderPath)

    submittedJobsPath = historyFolderPath + "/" + hashSweep() + ".submitted"

    with open(submittedJobsPath, "w") as submittedJobsFile:
        submittedJobsFile.write(json.dumps(jobIds))

    print("submitted "+str(len(jobIds))+" jobs")
    print("job ids are memorised in: "+submittedJobsPath)
    command="cp "+optionsFile+" "+submittedJobsPath.replace(".submitted",".ini")
    print(command)
    process = subprocess.Popen([command], stdout=subprocess.PIPE, shell=True)
    (output, err) = process.communicate()
    process.wait()

def cancelJobs():
    """
    Cancel submitted jobs.
    """
    jobCancellationTool    = general["job_cancellation"]

    submittedJobsPath = historyFolderPath + "/" + hashSweep() + ".submitted"

    jobIds = None
    try:
        with open(submittedJobsPath, "r") as submittedJobsFile:
            jobIds = json.loads(submittedJobsFile.read())
    except IOError:
        print("ERROR: couldn't find any submitted jobs for current sweep ('"+submittedJobsPath+"').")
        sys.exit()

    for jobId in jobIds:
        command = jobCancellationTool + " " + jobId
        print(command)
        subprocess.call(command,shell=True)
    print("cancelled "+str(len(jobIds))+" jobs")

    command = "rm "+submittedJobsPath
    command = "rm "+submittedJobsPath.replace(".submitted",".ini")
    print(command)
    subprocess.call(command,shell=True)

if __name__ == "__main__":
    import sys,os
    import subprocess
    import itertools
    import hashlib
    import json
    import re

    import sweep_analysis
    import sweep_options

    subprograms = ["build","buildMissing","scripts","submit","cancel","parseAdapters","parseMetrics","cleanBuild", "cleanScripts","cleanResults","cleanAll"]

    if haveToPrintHelpMessage(sys.argv):
        info = \
"""sweep.py:

run:

./sweep.py myoptions.ini <subprogram>

available subprograms:

* build         - build all executables
* buildMissing  - build only missing executables
* scripts       - submit the generated jobs
* cancel        - cancel the submitted jobs
* parseAdapters - read the job output and parse adapter times
* parseMetrics  - read the job output and parse likwid metrics
* cleanAll      - remove the whole sweep benchmark suite
* cleanBuild    - remove the build subfolder
* cleanScripts  - remove the scripts subfolder
* cleanResults  - remove the results subfolder
* cleanHistory  - clean the submission history

typical workflow:

./sweep.py myoptions.ini build
./sweep.py myoptions.ini scripts
./sweep.py myoptions.ini submit

(after jobs have finished)

./sweep.py myoptions.ini parseAdapters

"""
        print(info) # correctly indented
        sys.exit()

    optionsFile = parseArgument(sys.argv,1)
    subprogram  = parseArgument(sys.argv,2)

    options = sweep_options.parseOptionsFile(optionsFile)

    general          = options.general
    jobs             = options.jobs
    environmentSpace = options.environmentSpace
    parameterSpace   = options.parameterSpace

    deltaRoot        = options.deltaRoot
    outputPath       = options.outputPath
    projectPath      = options.projectPath
    projectName      = options.projectName

    buildFolderPath   = options.buildFolderPath
    scriptsFolderPath = options.scriptsFolderPath
    resultsFolderPath = options.resultsFolderPath
    historyFolderPath = options.historyFolderPath

    nodeCounts = options.nodeCounts
    taskCounts = options.taskCounts
    coreCounts = options.coreCounts

    verifySweepAgreesWithHistoricExperiments()

    # select subprogram
    if subprogram == "cleanAll":
        clean()
    elif subprogram == "cleanBuild":
        clean("build")
    elif subprogram == "cleanScripts":
        clean("scripts")
    elif subprogram == "cleanResults":
        clean("results")
    elif subprogram == "cleanHistory":
        clean("history")
    elif subprogram == "build":
        build()
    elif subprogram == "buildMissing":
        build(True)
    elif subprogram == "scripts":
        generateScripts()
    elif subprogram == "submit":
        submitJobs()
    elif subprogram == "cancel":
        cancelJobs()
    elif subprogram == "parseAdapters":
        sweep_analysis.parseAdapterTimes(resultsFolderPath,projectName)
    elif subprogram == "parseMetrics":
        sweep_analysis.parseMetrics(resultsFolderPath,projectName)
