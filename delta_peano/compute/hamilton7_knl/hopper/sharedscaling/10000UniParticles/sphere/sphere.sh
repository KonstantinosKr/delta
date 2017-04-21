#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch1

exp0o=/ddn/data/rfmw74/h7hopperScaleregularSphere1_10000hopper

exp0="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 reluctant-adaptive-grid 0.00001 never 10 1 sphere 100"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT0} > ${JOB_SCRIPT0}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT0}_tmp > ${JOB_SCRIPT0}_tmp1
sbatch ${JOB_SCRIPT0}_tmp1

rm ${JOB_SCRIPT0}_tmp
rm ${JOB_SCRIPT0}_tmp1