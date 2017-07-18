#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch

exp0o=/ddn/data/rfmw74/h7regularpenaltynovecFLOPSHopper1k

exp0="likwid-perfctr -f -C 1 -g FLOPS_DP ./dem-3d-release-novec 0.1 0.001 0.1 hopperUniform1k 25 regular-grid 0.0001 never 10 true penalty 40"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT0} > ${JOB_SCRIPT0}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT0}_tmp > ${JOB_SCRIPT0}_tmp1
sbatch ${JOB_SCRIPT0}_tmp1

rm ${JOB_SCRIPT0}_tmp
rm ${JOB_SCRIPT0}_tmp1