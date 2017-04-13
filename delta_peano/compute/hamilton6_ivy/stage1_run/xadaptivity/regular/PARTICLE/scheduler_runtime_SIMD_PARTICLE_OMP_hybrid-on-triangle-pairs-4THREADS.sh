#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP4

exp5o=/scratch/rfmw74/regularGAdaptivitysphere-hybrid-on-triangle-pairsSIMDPARTICLEOMP-4THREADS

exp5="./dem-3d-release-particle 0.4 0.1 0.01 random-velocities 500 regular-grid 0.0001 never 9.81 sphere-hybrid-on-triangle-pairs"

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1









