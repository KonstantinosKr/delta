#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP1

exp3o=/scratch/rfmw74/adaptiveGAdaptivitySphereSIMDPARTICLEOMPTBB-1THREADS

exp3="./dem-3d-release-tbb-omp-particle 0.4 0.1 0.01 random-velocities 500 adaptive-grid 0.0001 never 9.81 sphere 1"

sed "s,{OUTPUT_FILE},$exp3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1









