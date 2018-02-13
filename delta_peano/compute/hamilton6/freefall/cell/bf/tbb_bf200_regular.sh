#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch1
JOB_SCRIPT1=runjobBatch2
JOB_SCRIPT2=runjobBatch3
JOB_SCRIPT3=runjobBatch4
JOB_SCRIPT4=runjobBatch6
JOB_SCRIPT5=runjobBatch8
JOB_SCRIPT6=runjobBatch16
JOB_SCRIPT7=runjobBatch24

exp0o=/ddn/data/rfmw74/h7regularbfTBB1_200
exp1o=/ddn/data/rfmw74/h7regularbfTBB2_200
exp2o=/ddn/data/rfmw74/h7regularbfTBB3_200
exp3o=/ddn/data/rfmw74/h7regularbfTBB4_200
exp4o=/ddn/data/rfmw74/h7regularbfTBB6_200
exp5o=/ddn/data/rfmw74/h7regularbfTBB8_200
exp6o=/ddn/data/rfmw74/h7regularbfTBB16_200
exp7o=/ddn/data/rfmw74/h7regularbfTBB24_200

exp0="./dem-3d-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 1"
exp1="./dem-3d-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 2"
exp2="./dem-3d-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 3"
exp3="./dem-3d-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 4"
exp4="./dem-3d-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 6"
exp5="./dem-3d-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 8"
exp6="./dem-3d-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 16"
exp7="./dem-3d-release-tbb 0.1 freefallWithGranulates 1000 regular-grid 0.0001 never 10 true bf 200 24"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT0} > ${JOB_SCRIPT0}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT0}_tmp > ${JOB_SCRIPT0}_tmp1
sbatch ${JOB_SCRIPT0}_tmp1

rm ${JOB_SCRIPT0}_tmp
rm ${JOB_SCRIPT0}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT1} > ${JOB_SCRIPT1}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT1}_tmp > ${JOB_SCRIPT1}_tmp1
sbatch ${JOB_SCRIPT1}_tmp1

rm ${JOB_SCRIPT1}_tmp
rm ${JOB_SCRIPT1}_tmp1

sed "s,{OUTPUT_FILE},$exp2o,g" ${JOB_SCRIPT2} > ${JOB_SCRIPT2}_tmp
sed "s,{RUN},$exp2,g" ${JOB_SCRIPT2}_tmp > ${JOB_SCRIPT2}_tmp1
sbatch ${JOB_SCRIPT2}_tmp1

rm ${JOB_SCRIPT2}_tmp
rm ${JOB_SCRIPT2}_tmp1

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