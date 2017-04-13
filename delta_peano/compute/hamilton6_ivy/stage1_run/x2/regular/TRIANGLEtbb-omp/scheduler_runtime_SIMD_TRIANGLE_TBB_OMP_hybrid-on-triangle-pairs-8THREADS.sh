#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP8

exp5o=/scratch/rfmw74/regularG2sphere-hybrid-on-triangle-pairsSIMDTRIANGLEOMPTBB-8THREADS

exp5="./dem-3d-release-tbb-omp-triangle 0.1 0.05 0.01 random-velocities 500 regular-grid 0.001 never 9.81 sphere-hybrid-on-triangle-pairs 8"

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1









