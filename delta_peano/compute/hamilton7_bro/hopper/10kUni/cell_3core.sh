#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch3
JOB_SCRIPT1=runjobBatch3
JOB_SCRIPT2=runjobBatch3
JOB_SCRIPT3=runjobBatch3
JOB_SCRIPT4=runjobBatch3
JOB_SCRIPT5=runjobBatch3
JOB_SCRIPT6=runjobBatch3
JOB_SCRIPT7=runjobBatch3

exp0o=/ddn/data/rfmw74/h7X4hopperUni10k-adaptive-hybrid-on-triangle-mesh10-TBB3
exp1o=/ddn/data/rfmw74/h7X4hopperUni10k-adaptive-hybrid-on-batches-mesh10-TBB3
exp2o=/ddn/data/rfmw74/h7X4hopperUni10k-adaptive-hybrid-on-triangle-mesh20-TBB3
exp3o=/ddn/data/rfmw74/h7X4hopperUni10k-adaptive-hybrid-on-batches-mesh20-TBB3
exp4o=/ddn/data/rfmw74/h7X4hopperUni10k-adaptive-hybrid-on-triangle-mesh40-TBB3
exp5o=/ddn/data/rfmw74/h7X4hopperUni10k-adaptive-hybrid-on-batches-mesh40-TBB3
exp6o=/ddn/data/rfmw74/h7X4hopperUni10k-adaptive-hybrid-on-triangle-mesh80-TBB3
exp7o=/ddn/data/rfmw74/h7X4hopperUni10k-adaptive-hybrid-on-batches-mesh80-TBB3

exp0="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform10k 500 adaptive-grid 0.000008 never 10 true hybrid-on-triangle-pairs 10 3"
exp1="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform10k 500 adaptive-grid 0.000008 never 10 true hybrid-on-batches 10 3"
exp2="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform10k 500 adaptive-grid 0.000008 never 10 true hybrid-on-triangle-pairs 20 3"
exp3="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform10k 500 adaptive-grid 0.000008 never 10 true hybrid-on-batches 20 3"
exp4="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform10k 500 adaptive-grid 0.000008 never 10 true hybrid-on-triangle-pairs 40 3"
exp5="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform10k 500 adaptive-grid 0.000008 never 10 true hybrid-on-batches 40 3"
exp6="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform10k 500 adaptive-grid 0.000008 never 10 true hybrid-on-triangle-pairs 80 3"
exp7="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform10k 500 adaptive-grid 0.000008 never 10 true hybrid-on-batches 80 3"

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
