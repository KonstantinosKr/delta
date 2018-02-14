#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch

exp0o=reluctant0penaltysimd
exp1o=reluctantG0penaltysimd
exp2o=reluctant1penaltysimd
exp3o=reluctantG1penaltysimd
exp4o=reluctant2penaltysimd
exp5o=reluctantG2penaltysimd
exp6o=reluctant3penaltysimd
exp7o=reluctantG3penaltysimd

exp0="./delta-release-vec 0.5 0.5 0.5 random-velocities 500 reluctant-grid 0.001 never 0 penalty"
exp1="./delta-release-vec 0.5 0.5 0.5 random-velocities 500 reluctant-grid 0.001 never 1 10 penalty"
exp2="./delta-release-vec 0.05 0.05 random-velocities 500 reluctant-grid 0.001 never 0 penalty"
exp3="./delta-release-vec 0.05 0.05 random-velocities 500 reluctant-grid 0.001 never 1 10 penalty"
exp4="./delta-release-vec 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 0 penalty"
exp5="./delta-release-vec 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 1 10 penalty"
exp6="./delta-release-vec 0.05 0.001 random-velocities 500 reluctant-grid 0.001 never 0 penalty"
exp7="./delta-release-vec 0.05 0.001 random-velocities 500 reluctant-grid 0.001 never 1 10 penalty"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
qsub ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
qsub ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
qsub ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
qsub ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp4o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp4,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
qsub ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
qsub ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
qsub ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp7o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp7,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
qsub ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1










