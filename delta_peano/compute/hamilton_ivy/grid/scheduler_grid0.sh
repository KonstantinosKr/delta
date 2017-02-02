#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch

exp0o=/scratch/rfmw74/regular0cmp
exp1o=/scratch/rfmw74/regularG0cmp
exp2o=/scratch/rfmw74/adaptive0cmp
exp3o=/scratch/rfmw74/adaptiveG0cmp
exp4o=/scratch/rfmw74/reluctant0cmp
exp5o=/scratch/rfmw74/reluctantG0cmp

exp0="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 10000 regular-grid 0.0001 never 0 sphere 0.1"
exp1="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 10000 regular-grid 0.0001 never 9.81 sphere 0.1"
exp2="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 10000 adaptive-grid 0.0001 never 0 sphere 0.1"
exp3="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 10000 adaptive-grid 0.0001 never 9.81 sphere 0.1"
exp4="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 10000 reluctant-adaptive-grid 0.0001 0 sphere never 0.1"
exp5="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 10000 reluctant-adaptive-grid 0.0001 9.81 sphere never 0.1"

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
