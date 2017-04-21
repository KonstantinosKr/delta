#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT5=runjobBatch8

exp5o=/ddn/data/rfmw74/h7reluctantbfTBB8_20

exp5="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniformMesh 50000 reluctant-adaptive-grid 0.00001 never 10 1 penaltyStat 50 8"


sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT5} > ${JOB_SCRIPT5}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT5}_tmp > ${JOB_SCRIPT5}_tmp1
sbatch ${JOB_SCRIPT5}_tmp1

rm ${JOB_SCRIPT5}_tmp
rm ${JOB_SCRIPT5}_tmp1
