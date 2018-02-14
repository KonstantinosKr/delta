#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=../tbb
JOB_SCRIPT1=../tbb
JOB_SCRIPT2=../tbb
JOB_SCRIPT3=../tbb
JOB_SCRIPT4=../tbb
JOB_SCRIPT5=../tbb
JOB_SCRIPT6=../tbb
JOB_SCRIPT7=../tbb
JOB_SCRIPT8=../tbb

exp0o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB1
exp1o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB2
exp2o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB3
exp3o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB4
exp4o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB6
exp5o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB8
exp6o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB12
exp7o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB16
exp8o=/ddn/data/rfmw74/h7X4hopperUni-adaptive-hybrid-on-triangle-mesh60-TBB24

exp0="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 1 true false off"
exp1="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 2 true false off"
exp2="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 3 true false off"
exp3="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 4 true false off"
exp4="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 6 true false off"
exp5="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 8 true false off"
exp6="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 12 true false off"
exp7="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 16 true false off"
exp8="./delta-release-tbb-omp-triangle 0.1 hopperUniform 1000 adaptive-grid 0.00003 never 10 true hybrid-on-triangle-pairs 60 24 true false off"

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

sed "s,{OUTPUT_FILE},$exp8o,g" ${JOB_SCRIPT8} > ${JOB_SCRIPT8}_tmp
sed "s,{RUN},$exp8,g" ${JOB_SCRIPT8}_tmp > ${JOB_SCRIPT8}_tmp1
sbatch ${JOB_SCRIPT8}_tmp1

rm ${JOB_SCRIPT8}_tmp
rm ${JOB_SCRIPT8}_tmp1
