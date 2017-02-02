#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP3

exp0o=reluctant0sphere-hybrid-on-batchesSIMDPARTICLEOMP-3THREADS
exp1o=reluctantG0sphere-hybrid-on-batchesSIMDPARTICLEOMP-3THREADS
exp2o=reluctant1sphere-hybrid-on-batchesSIMDPARTICLEOMP-3THREADS
exp3o=reluctantG1sphere-hybrid-on-batchesSIMDPARTICLEOMP-3THREADS
exp4o=reluctant2sphere-hybrid-on-batchesSIMDPARTICLEOMP-3THREADS
exp5o=reluctantG2sphere-hybrid-on-batchesSIMDPARTICLEOMP-3THREADS
exp6o=reluctant3sphere-hybrid-on-batchesSIMDPARTICLEOMP-3THREADS
exp7o=reluctantG3sphere-hybrid-on-batchesSIMDPARTICLEOMP-3THREADS

exp0="./dem-3d-release-vecPARTICLE 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere-hybrid-on-on-batches"
exp1="./dem-3d-release-vecPARTICLE 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 sphere-hybrid-on-batches"
exp2="./dem-3d-release-vecPARTICLE 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere-hybrid-on-batches"
exp3="./dem-3d-release-vecPARTICLE 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 sphere-hybrid-on-batches"
exp4="./dem-3d-release-vecPARTICLE 0.1 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere-hybrid-on-batches"
exp5="./dem-3d-release-vecPARTICLE 0.1 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 sphere-hybrid-on-batches"
exp6="./dem-3d-release-vecPARTICLE 0.1 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere-hybrid-on-batches"
exp7="./dem-3d-release-vecPARTICLE 0.1 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 sphere-hybrid-on-batches"

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











