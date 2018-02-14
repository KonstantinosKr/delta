#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT5=runjobBatchTBB6OMP2
JOB_SCRIPT6=runjobBatchTBB12OMP4
JOB_SCRIPT7=runjobBatchTBB20OMP4

exp5o=/ddn/data/rfmw74/h7regularbfTBB6_200_plus_2triangle-8
exp6o=/ddn/data/rfmw74/h7regularbfTBB12_200_plus_4triangle-16
exp7o=/ddn/data/rfmw74/h7regularbfTBB20_200_plus_4triangle-24

exp5="./delta-icc-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 6"
exp6="./delta-icc-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 12"
exp7="./delta-icc-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 20"

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
