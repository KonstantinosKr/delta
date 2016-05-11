#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch

#batch 0
b1_1o=/scratch/rfmw74/regular0cmp
b1_2o=/scratch/rfmw74/regularG0cmp
b1_3o=/scratch/rfmw74/adaptive0cmp
b1_4o=/scratch/rfmw74/adaptiveG0cmp
b1_5o=/scratch/rfmw74/reluctant0cmp
b1_6o=/scratch/rfmw74/reluctantG0cmp

b0_1="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 regular-grid 0.001 never 0 bf"
b0_2="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 regular-grid 0.001 never 9.81 bf"
b0_3="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 adaptive-grid 0.001 never 0 bf"
b0_4="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 adaptive-grid 0.001 never 9.81 bf"
b0_5="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 bf"
b0_6="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 bf"


#batch 1
b1_1o=/scratch/rfmw74/regular1cmp
b1_2o=/scratch/rfmw74/regularG1cmp
b1_3o=/scratch/rfmw74/adaptive1cmp
b1_4o=/scratch/rfmw74/adaptiveG1cmp
b1_5o=/scratch/rfmw74/reluctant1cmp
b1_6o=/scratch/rfmw74/reluctantG1cmp

b1_1="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 regular-grid 0.001 never 0 bf"
b1_2="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 regular-grid 0.001 never 9.81 bf"
b1_3="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 adaptive-grid 0.001 never 0 bf"
b1_4="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 adaptive-grid 0.001 never 9.81 bf"
b1_5="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 bf"
b1_6="./dem-3d-release 0.1 0.05 0.05 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 bf"

#batch 2
b2_1o=/scratch/rfmw74/regular2cmp
b2_2o=/scratch/rfmw74/regularG2cmp
b2_3o=/scratch/rfmw74/adaptive2cmp
b2_4o=/scratch/rfmw74/adaptiveG2cmp
b2_5o=/scratch/rfmw74/reluctant2cmp
b2_6o=/scratch/rfmw74/reluctantG2cmp

b2_1="./dem-3d-release 0.1 0.05 0.01 random-velocities 500 regular-grid 0.001 never 0 bf"
b2_2="./dem-3d-release 0.1 0.05 0.01 random-velocities 500 regular-grid 0.001 never 9.81 bf"
b2_3="./dem-3d-release 0.1 0.05 0.01 random-velocities 500 adaptive-grid 0.001 never 0 bf"		
b2_4="./dem-3d-release 0.1 0.05 0.01 random-velocities 500 adaptive-grid 0.001 never 9.81 bf"
b2_5="./dem-3d-release 0.1 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 bf"
b2_6="./dem-3d-release 0.1 0.05 0.01 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 bf"

#batch3
b3_1o=/scratch/rfmw74/regular3cmp
b3_2o=/scratch/rfmw74/regularG3cmp
b3_3o=/scratch/rfmw74/adaptive3cmp
b3_4o=/scratch/rfmw74/adaptiveG3cmp
b3_5o=/scratch/rfmw74/reluctant3cmp
b3_6o=/scratch/rfmw74/reluctantG3cmp

b3_1="./dem-3d-release 0.1 0.05 0.001 random-velocities 500 regular-grid 0.001 never 0 bf"
b3_2="./dem-3d-release 0.1 0.05 0.001 random-velocities 500 regular-grid 0.001 never 9.81 bf"
b3_3="./dem-3d-release 0.1 0.05 0.001 random-velocities 500 adaptive-grid 0.001 never 0 bf"
b3_4="./dem-3d-release 0.1 0.05 0.001 random-velocities 500 adaptive-grid 0.001 never 9.81 bf"
b3_5="./dem-3d-release 0.1 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 bf"
b3_6="./dem-3d-release 0.1 0.05 0.001 random-velocities 500 reluctant-adaptive-grid 0.001 never 9.81 bf"

#batch 0
sed "s,{OUTPUT_FILE},$b0_1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b0_1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b0_2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b0_2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b0_3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b0_3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b0_4o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b0_4,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b0_5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b0_5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b0_6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b0_6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

#batch 1
sed "s,{OUTPUT_FILE},$b1_1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b1_1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b1_2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b1_2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b1_3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b1_3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b1_4o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b1_4,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b1_5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b1_5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b1_6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b1_6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

#batch 2
sed "s,{OUTPUT_FILE},$b2_1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b2_2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b2_3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b2_4o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_4,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b2_5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b2_6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

#batch 3
sed "s,{OUTPUT_FILE},$b3_1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b3_2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b3_3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b3_4o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_4,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b3_5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$b3_6o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$b2_6,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1










