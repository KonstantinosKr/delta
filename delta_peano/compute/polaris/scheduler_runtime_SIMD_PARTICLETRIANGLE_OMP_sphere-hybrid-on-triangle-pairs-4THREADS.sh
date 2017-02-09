#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP4

exp0o=reluctant0sphere-hybrid-on-triangle-pairsSIMDPARTICLETRIANGLEOMP-4THREADS
exp1o=reluctantG0sphere-hybrid-on-triangle-pairsSIMDPARTICLETRIANGLEOMP-4THREADS
exp2o=reluctant1sphere-hybrid-on-triangle-pairsSIMDPARTICLETRIANGLEOMP-4THREADS
exp3o=reluctantG1sphere-hybrid-on-triangle-pairsSIMDPARTICLETRIANGLEOMP-4THREADS
exp4o=reluctant2sphere-hybrid-on-triangle-pairsSIMDPARTICLETRIANGLEOMP-4THREADS
exp5o=reluctantG2sphere-hybrid-on-triangle-pairsSIMDPARTICLETRIANGLEOMP-4THREADS
exp6o=reluctant3sphere-hybrid-on-triangle-pairsSIMDPARTICLETRIANGLEOMP-4THREADS
exp7o=reluctantG3sphere-hybrid-on-triangle-pairsSIMDPARTICLETRIANGLEOMP-4THREADS

exp0="./dem-3d-release-vecPARTICLETRIANGLE 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere-hybrid-on-triangle-pairs"
exp1="./dem-3d-release-vecPARTICLETRIANGLE 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 1 10 sphere-hybrid-on-triangle-pairs"
exp2="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere-hybrid-on-triangle-pairs"
exp3="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 1 10 sphere-hybrid-on-triangle-pairs"
exp4="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere-hybrid-on-triangle-pairs"
exp5="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 1 10 sphere-hybrid-on-triangle-pairs"
exp6="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere-hybrid-on-triangle-pairs"
exp7="./dem-3d-release-vecPARTICLETRIANGLE 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 1 10 sphere-hybrid-on-triangle-pairs"

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










