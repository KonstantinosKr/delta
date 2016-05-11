#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch

exp0o=/scratch/rfmw74/reluctant0SphereSIMD
exp1o=/scratch/rfmw74/reluctantG0SphereSIMD
exp2o=/scratch/rfmw74/reluctant1SphereSIMD
exp3o=/scratch/rfmw74/reluctantG1SphereSIMD
exp4o=/scratch/rfmw74/reluctant2SphereSIMD
exp5o=/scratch/rfmw74/reluctantG2SphereSIMD
exp6o=/scratch/rfmw74/reluctant3SphereSIMD
exp7o=/scratch/rfmw74/reluctantG3SphereSIMD

exp0="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere"
exp1="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 sphere"
exp2="./dem-3d-release-vec 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere"
exp3="./dem-3d-release-vec 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 sphere"
exp4="./dem-3d-release-vec 0.1 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere"
exp5="./dem-3d-release-vec 0.1 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 sphere"
exp6="./dem-3d-release-vec 0.1 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere"
exp7="./dem-3d-release-vec 0.1 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 sphere"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp4o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp4,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp7o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp7,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1










