from os import listdir
from os.path import isfile, join

searchString = ""
projectName = "peano_knl_two-particles-crash"
time = "00:30:00"

#purepath = '/Users/konstantinos/Downloads/'
purepath = '/work/knl-users/durkk/'
#purepath = '/work/e573/e573/durkk/'

scriptsPath = "/" + purepath + "/" + projectName + "/scripts/"
outputDirectory = purepath + "/"+ projectName + "/"+ projectName +".job"

job = "\
#!/bin/bash\n"+"\
#PBS -N "+projectName+"\n" + "\
#PBS -l select=1:aoe=quad_100\n" + "\
#PBS -l walltime="+time+"\n" + "\
#PBS -A e573-durkk\n" + "\
#PBS -m ae\n" + "\
#PBS -M konstantinos.krestenitis@durham.ac.uk\n" + "\
#PBS -V\n" + "\
export OMP_NUM_THREADS=1\n" + "\
export PBS_O_WORKDIR=$(readlink -f $PBS_O_WORKDIR)\n" + "\
cd $PBS_O_WORKDIR\n" + "\
"

file = open(outputDirectory, 'w+')
print(outputDirectory)
file.write(job)

selectedFiles = [join(scriptsPath, f) for f in listdir(scriptsPath) if isfile(join(scriptsPath, f)) and searchString in f]

print(selectedFiles)

commands = []
for i in selectedFiles:
    with open(i) as f:
        for line in f:
            if "aprun" in line and "#aprun" not in line:
                commands.append(line)
                file.write(line + "\n")
                file.write("echo finished run\n")

print(commands)
print(file)

f.close()
