#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch1
JOB_SCRIPT1=runjobBatch1
JOB_SCRIPT2=runjobBatch1

exp0o=/scratch/rfmw74/regularG4Sphere
exp1o=/scratch/rfmw74/adaptiveG4Sphere
exp2o=/scratch/rfmw74/reluctantG4Sphere

exp0="./dem-3d-release-omp-particle 0.3 0.3 0.003 freefall 5000 regular-grid 0.0001 never 9.81 sphere 0.1"
exp1="./dem-3d-release-omp-particle 0.3 0.3 0.003 freefall 5000 adaptive-grid 0.0001 never 9.81 sphere 0.1"
exp2="./dem-3d-release-omp-particle 0.3 0.3 0.003 freefall 5000 reluctant-adaptive-grid 0.0001 never 9.81 sphere 0.1"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT0} > ${JOB_SCRIPT0}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT0}_tmp > ${JOB_SCRIPT0}_tmp1
sbatch ${JOB_SCRIPT0}_tmp1

rm ${JOB_SCRIPT0}_tmp
rm ${JOB_SCRIPT0}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT1} > ${JOB_SCRIPT1}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT1}_tmp > ${JOB_SCRIPT1}_tmp1
sbatch ${JOB_SCRIPT1}_tmp1

rm ${JOB_SCRIPT1}_tmp
rm ${JOB_SCRIPT1}_tmp1

sed "s,{OUTPUT_FILE},$exp2o,g" ${JOB_SCRIPT2} > ${JOB_SCRIPT2}_tmp
sed "s,{RUN},$exp2,g" ${JOB_SCRIPT2}_tmp > ${JOB_SCRIPT2}_tmp1
sbatch ${JOB_SCRIPT2}_tmp1

rm ${JOB_SCRIPT2}_tmp
rm ${JOB_SCRIPT2}_tmp1
