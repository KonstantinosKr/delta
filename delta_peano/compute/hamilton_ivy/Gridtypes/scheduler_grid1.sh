#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch

exp0o=/scratch/rfmw74/regular1cmp
exp1o=/scratch/rfmw74/regularG1cmp
exp2o=/scratch/rfmw74/adaptive1cmp
exp3o=/scratch/rfmw74/adaptiveG1cmp
exp4o=/scratch/rfmw74/reluctant1cmp
exp5o=/scratch/rfmw74/reluctantG1cmp

exp0="./dem-3d-release-vec 0.05 0.05 random-velocities 500 regular-grid 0.0001 never 0 sphere 0.1"
exp1="./dem-3d-release-vec 0.05 0.05 random-velocities 500 regular-grid 0.0001 never 1 10 sphere 0.1"
exp2="./dem-3d-release-vec 0.05 0.05 random-velocities 500 adaptive-grid 0.0001 never 0 sphere 0.1"
exp3="./dem-3d-release-vec 0.05 0.05 random-velocities 500 adaptive-grid 0.0001 never 1 10 sphere 0.1"
exp4="./dem-3d-release-vec 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.0001 never 0 sphere 0.1"
exp5="./dem-3d-release-vec 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.0001 never 1 10 sphere 0.1"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp4o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp4,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1
