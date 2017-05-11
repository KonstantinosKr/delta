#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch

exp1o=/scratch/rfmw74/regularGridcmp2000
exp2o=/scratch/rfmw74/adaptiveGridcmp2000
exp3o=/scratch/rfmw74/reluctantGridcmp2000

exp1="./dem-3d-release-vec 0.1 0.001 0.1 freefallWithGranulates 5000 regular-grid 0.0001 never 10 true bf 2000"
exp2="./dem-3d-release-vec 0.1 0.001 0.1 freefallWithGranulates 5000 adaptive-grid 0.0001 never 10 true bf 2000"
exp3="./dem-3d-release-vec 0.1 0.001 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true bf 2000"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1
