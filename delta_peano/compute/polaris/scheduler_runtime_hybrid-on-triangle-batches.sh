#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch

exp0o=reluctant0hybrid-on-batches
exp1o=reluctantG0hybrid-on-batches
exp2o=reluctant1hybrid-on-batches
exp3o=reluctantG1hybrid-on-batches
exp4o=reluctant2hybrid-on-batches
exp5o=reluctantG2hybrid-on-batches
exp6o=reluctant3hybrid-on-batches
exp7o=reluctantG3hybrid-on-batches

exp0="./delta-release-novec 0.5 0.5 0.5 random-velocities 500 reluctant-grid 0.001 never 0 hybrid-on-batches"
exp1="./delta-release-novec 0.5 0.5 0.5 random-velocities 500 reluctant-grid 0.001 never 1 10 hybrid-on-batches"
exp2="./delta-release-novec 0.05 0.05 random-velocities 500 reluctant-grid 0.001 never 0 hybrid-on-batches"
exp3="./delta-release-novec 0.05 0.05 random-velocities 500 reluctant-grid 0.001 never 1 10 hybrid-on-batches"
exp4="./delta-release-novec 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 0 hybrid-on-batches"
exp5="./delta-release-novec 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 1 10 hybrid-on-batches"
exp6="./delta-release-novec 0.05 0.001 random-velocities 500 reluctant-grid 0.001 never 0 hybrid-on-batches"
exp7="./delta-release-novec 0.05 0.001 random-velocities 500 reluctant-grid 0.001 never 1 10 hybrid-on-batches"

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











