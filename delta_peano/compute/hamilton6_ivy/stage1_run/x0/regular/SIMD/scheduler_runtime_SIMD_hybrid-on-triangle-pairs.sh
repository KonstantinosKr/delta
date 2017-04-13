#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobSIMD

exp5o=/scratch/rfmw74/regularG0hybrid-on-triangle-pairsSIMD

exp5="./dem-3d-release-vec 0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 9.81 hybrid-on-triangle-pairs"

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1









