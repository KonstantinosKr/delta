#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=tbb1-omp1
JOB_SCRIPT1=tbb1-omp1

exp0o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_1_OMP_1_TRIANGLE_20_1000hopper
exp1o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_1_OMP_1_TRIANGLE_20_1000hopper

exp0="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 1"
exp1="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 1"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT0} > ${JOB_SCRIPT0}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT0}_tmp > ${JOB_SCRIPT0}_tmp1
sbatch ${JOB_SCRIPT0}_tmp1

rm ${JOB_SCRIPT0}_tmp
rm ${JOB_SCRIPT0}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT1} > ${JOB_SCRIPT1}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT1}_tmp > ${JOB_SCRIPT1}_tmp1
sbatch ${JOB_SCRIPT1}_tmp1

rm ${JOB_SCRIPT1}_tmp
rm ${JOB_SCRIPT1}_tmp1
