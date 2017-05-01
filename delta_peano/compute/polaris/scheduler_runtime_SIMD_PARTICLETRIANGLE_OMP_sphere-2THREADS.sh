#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP2

exp0o=reluctant0SphereSIMDPARTICLETRIANGLEOMP-2THREADS
exp1o=reluctantG0SphereSIMDPARTICLETRIANGLEOMP-2THREADS
exp2o=reluctant1SphereSIMDPARTICLETRIANGLEOMP-2THREADS
exp3o=reluctantG1SphereSIMDPARTICLETRIANGLEOMP-2THREADS
exp4o=reluctant2SphereSIMDPARTICLETRIANGLEOMP-2THREADS
exp5o=reluctantG2SphereSIMDPARTICLETRIANGLEOMP-2THREADS
exp6o=reluctant3SphereSIMDPARTICLETRIANGLEOMP-2THREADS
exp7o=reluctantG3SphereSIMDPARTICLETRIANGLEOMP-2THREADS

exp0="./dem-3d-release-vecPARTICLETRIANGLE 0.5 0.5 0.5 random-velocities 500 reluctant-grid 0.001 never 0 sphere"
exp1="./dem-3d-release-vecPARTICLETRIANGLE 0.5 0.5 0.5 random-velocities 500 reluctant-grid 0.001 never 1 10 sphere"
exp2="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.05 random-velocities 500 reluctant-grid 0.001 never 0 sphere"
exp3="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.05 random-velocities 500 reluctant-grid 0.001 never 1 10 sphere"
exp4="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 0 sphere"
exp5="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 1 10 sphere"
exp6="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.001 random-velocities 500 reluctant-grid 0.001 never 0 sphere"
exp7="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.001 random-velocities 500 reluctant-grid 0.001 never 1 10 sphere"

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










