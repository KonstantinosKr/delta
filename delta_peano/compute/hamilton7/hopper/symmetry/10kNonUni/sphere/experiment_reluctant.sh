#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT8=t24

exp8o=/ddn/data/rfmw74/experiment-hopperNonUni10k-regular-spheres

exp8="./dem-3d-release-tbb-omp-triangle 0.1 hopperNonUniform10k 500000 reluctant-grid 0.0000001 every-batch 10 true sphere 10 24"

sed "s,{OUTPUT_FILE},$exp8o,g" ${JOB_SCRIPT8} > ${JOB_SCRIPT8}_tmp
sed "s,{RUN},$exp8,g" ${JOB_SCRIPT8}_tmp > ${JOB_SCRIPT8}_tmp1
sbatch ${JOB_SCRIPT8}_tmp1

rm ${JOB_SCRIPT8}_tmp
rm ${JOB_SCRIPT8}_tmp1
