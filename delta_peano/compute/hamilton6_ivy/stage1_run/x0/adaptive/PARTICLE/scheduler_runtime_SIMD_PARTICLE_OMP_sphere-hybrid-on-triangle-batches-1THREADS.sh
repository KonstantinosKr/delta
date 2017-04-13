#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP1

exp5o=/scratch/rfmw74/adaptiveG0sphere-hybrid-on-batchesSIMDPARTICLEOMP-1THREADS

exp5="./dem-3d-release-particle 0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 sphere-hybrid-on-batches"

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1











