#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT3=runjobBatchTBB3OMP1
JOB_SCRIPT4=runjobBatchTBB3OMP3
JOB_SCRIPT5=runjobBatchTBB3OMP5
JOB_SCRIPT6=runjobBatchTBB3OMP13
JOB_SCRIPT7=runjobBatchTBB3OMP21

exp3o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB3_200_plus_1triangle
exp4o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB3_200_plus_3triangle
exp5o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB3_200_plus_5triangle
exp6o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB3_200_plus_13triangle
exp7o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB3_200_plus_21triangle

exp3="./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 1000 regular-grid 0.0001 never 10 true hybrid-on-triangle-pairs 200 3"
exp4="./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 1000 regular-grid 0.0001 never 10 true hybrid-on-triangle-pairs 200 3"
exp5="./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 1000 regular-grid 0.0001 never 10 true hybrid-on-triangle-pairs 200 3"
exp6="./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 1000 regular-grid 0.0001 never 10 true hybrid-on-triangle-pairs 200 3"
exp7="./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 1000 regular-grid 0.0001 never 10 true hybrid-on-triangle-pairs 200 3"


sed "s,{OUTPUT_FILE},$exp3o,g" ${JOB_SCRIPT3} > ${JOB_SCRIPT3}_tmp
sed "s,{RUN},$exp3,g" ${JOB_SCRIPT3}_tmp > ${JOB_SCRIPT3}_tmp1
sbatch ${JOB_SCRIPT3}_tmp1

rm ${JOB_SCRIPT3}_tmp
rm ${JOB_SCRIPT3}_tmp1

sed "s,{OUTPUT_FILE},$exp4o,g" ${JOB_SCRIPT4} > ${JOB_SCRIPT4}_tmp
sed "s,{RUN},$exp4,g" ${JOB_SCRIPT4}_tmp > ${JOB_SCRIPT4}_tmp1
sbatch ${JOB_SCRIPT4}_tmp1

rm ${JOB_SCRIPT4}_tmp
rm ${JOB_SCRIPT4}_tmp1

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT5} > ${JOB_SCRIPT5}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT5}_tmp > ${JOB_SCRIPT5}_tmp1
sbatch ${JOB_SCRIPT5}_tmp1

rm ${JOB_SCRIPT5}_tmp
rm ${JOB_SCRIPT5}_tmp1

sed "s,{OUTPUT_FILE},$exp6o,g" ${JOB_SCRIPT6} > ${JOB_SCRIPT6}_tmp
sed "s,{RUN},$exp6,g" ${JOB_SCRIPT6}_tmp > ${JOB_SCRIPT6}_tmp1
sbatch ${JOB_SCRIPT6}_tmp1

rm ${JOB_SCRIPT6}_tmp
rm ${JOB_SCRIPT6}_tmp1

sed "s,{OUTPUT_FILE},$exp7o,g" ${JOB_SCRIPT7} > ${JOB_SCRIPT7}_tmp
sed "s,{RUN},$exp7,g" ${JOB_SCRIPT7}_tmp > ${JOB_SCRIPT7}_tmp1
sbatch ${JOB_SCRIPT7}_tmp1

rm ${JOB_SCRIPT7}_tmp
rm ${JOB_SCRIPT7}_tmp1
