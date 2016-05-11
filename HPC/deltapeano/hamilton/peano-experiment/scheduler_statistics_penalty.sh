#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch

b1_5o=/scratch/rfmw74/reluctant1penaltyStat
b1_6o=/scratch/rfmw74/reluctantG1penaltyStat

b1_5="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 penalty"
b1_6="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 penalty"

b2_5o=/scratch/rfmw74/reluctant2penaltyStat
b2_6o=/scratch/rfmw74/reluctantG2penaltyStat

b2_5="./dem-3d-release 0.1 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 penalty"
b2_6="./dem-3d-release 0.1 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 penalty"

#batch3
b3_5o=/scratch/rfmw74/reluctant3penaltyStat
b3_6o=/scratch/rfmw74/reluctantG3penaltyStat

b3_5="./dem-3d-release 0.1 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 penalty"
b3_6="./dem-3d-release 0.1 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 penalty"

sed "s,{OUTPUT_FILE},$b1_5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b1_5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b1_6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b1_6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b2_5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b2_6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b3_5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b3_6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1










