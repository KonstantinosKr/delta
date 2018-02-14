#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=../t2
JOB_SCRIPT1=../t2
JOB_SCRIPT2=../t2
JOB_SCRIPT3=../t2
JOB_SCRIPT4=../t2
JOB_SCRIPT5=../t2
JOB_SCRIPT6=../t2
JOB_SCRIPT7=../t2

exp0o=/ddn/data/rfmw74/h7-hopperUniform1k-reluctant-hybrid-on-triangle-m10-TBB2
exp1o=/ddn/data/rfmw74/h7-hopperUniform1k-reluctant-hybrid-on-batches-m10-TBB2
exp2o=/ddn/data/rfmw74/h7-hopperUniform1k-reluctant-hybrid-on-triangle-m20-TBB2
exp3o=/ddn/data/rfmw74/h7-hopperUniform1k-reluctant-hybrid-on-batches-m20-TBB2
exp4o=/ddn/data/rfmw74/h7-hopperUniform1k-reluctant-hybrid-on-triangle-m40-TBB2
exp5o=/ddn/data/rfmw74/h7-hopperUniform1k-reluctant-hybrid-on-batches-m40-TBB2
exp6o=/ddn/data/rfmw74/h7-hopperUniform1k-reluctant-hybrid-on-triangle-m80-TBB2
exp7o=/ddn/data/rfmw74/h7-hopperUniform1k-reluctant-hybrid-on-batches-m80-TBB2

exp0="./delta-icc-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 2 true false off"
exp1="./delta-icc-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-batches 10 2 true false off"
exp2="./delta-icc-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 2 true false off"
exp3="./delta-icc-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-batches 20 2 true false off"
exp4="./delta-icc-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40 2 true false off"
exp5="./delta-icc-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-batches 40 2 true false off"
exp6="./delta-icc-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 80 2 true false off"
exp7="./delta-icc-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-batches 80 2 true false off"

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
