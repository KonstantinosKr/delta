#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch24
JOB_SCRIPT4=runjobBatch24
JOB_SCRIPT8=runjobBatch24tbb12omp2

exp0o=/ddn/data/rfmw74/h7X21hopperUni1k-nogrid-sphere-tbb24
exp4o=/ddn/data/rfmw74/h7X24hopperUni1k-nogrid-sphere-particle24
exp8o=/ddn/data/rfmw74/h7X21hopperUni1k-nogrid-sphere-tbb12omp2

exp0="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform1k 1000 no-grid 0.00003 never 10 true sphere 10 24"
exp4="./dem-3d-release-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 no-grid 0.00003 never 10 true sphere 10"
exp8="./dem-3d-release-tbb-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 no-grid 0.00003 never 10 true sphere 10 12"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT0} > ${JOB_SCRIPT0}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT0}_tmp > ${JOB_SCRIPT0}_tmp1
sbatch ${JOB_SCRIPT0}_tmp1

rm ${JOB_SCRIPT0}_tmp
rm ${JOB_SCRIPT0}_tmp1

sed "s,{OUTPUT_FILE},$exp4o,g" ${JOB_SCRIPT4} > ${JOB_SCRIPT4}_tmp
sed "s,{RUN},$exp4,g" ${JOB_SCRIPT4}_tmp > ${JOB_SCRIPT4}_tmp1
sbatch ${JOB_SCRIPT4}_tmp1

rm ${JOB_SCRIPT4}_tmp
rm ${JOB_SCRIPT4}_tmp1

sed "s,{OUTPUT_FILE},$exp8o,g" ${JOB_SCRIPT8} > ${JOB_SCRIPT8}_tmp
sed "s,{RUN},$exp8,g" ${JOB_SCRIPT8}_tmp > ${JOB_SCRIPT8}_tmp1
sbatch ${JOB_SCRIPT8}_tmp1

rm ${JOB_SCRIPT8}_tmp
rm ${JOB_SCRIPT8}_tmp1
