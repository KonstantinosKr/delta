#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP16

exp0o=reluctant0sphere-bfSIMDPARTICLEOMP-16THREADS
exp1o=reluctantG0sphere-bfSIMDPARTICLEOMP-16THREADS
exp2o=reluctant1sphere-bfSIMDPARTICLEOMP-16THREADS
exp3o=reluctantG1sphere-bfSIMDPARTICLEOMP-16THREADS
exp4o=reluctant2sphere-bfSIMDPARTICLEOMP-16THREADS
exp5o=reluctantG2sphere-bfSIMDPARTICLEOMP-16THREADS
exp6o=reluctant3sphere-bfSIMDPARTICLEOMP-16THREADS
exp7o=reluctantG3sphere-bfSIMDPARTICLEOMP-16THREADS

exp0="./delta-release-vecPARTICLE 0.5 0.5 0.5 random-velocities 500 reluctant-grid 0.001 never 0 bf"
exp1="./delta-release-vecPARTICLE 0.5 0.5 0.5 random-velocities 500 reluctant-grid 0.001 never 1 10 bf"
exp2="./delta-release-vecPARTICLE 0.05 0.05 random-velocities 500 reluctant-grid 0.001 never 0 bf"
exp3="./delta-release-vecPARTICLE 0.05 0.05 random-velocities 500 reluctant-grid 0.001 never 1 10 bf"
exp4="./delta-release-vecPARTICLE 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 0 bf"
exp5="./delta-release-vecPARTICLE 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 1 10 bf"
exp6="./delta-release-vecPARTICLE 0.05 0.001 random-velocities 500 reluctant-grid 0.001 never 0 bf"
exp7="./delta-release-vecPARTICLE 0.05 0.001 random-velocities 500 reluctant-grid 0.001 never 1 10 bf"

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










