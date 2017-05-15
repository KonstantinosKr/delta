#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=t2
JOB_SCRIPT1=t2
JOB_SCRIPT2=t2
JOB_SCRIPT3=t2
JOB_SCRIPT4=t2
JOB_SCRIPT5=t2

exp0o=/ddn/data/rfmw74/h7X4hopperUni100k-reluctant-bf-mesh10-TBB2
exp1o=/ddn/data/rfmw74/h7X4hopperUni100k-reluctant-penalty-mesh10-TBB2
exp2o=/ddn/data/rfmw74/h7X4hopperUni100k-reluctant-bf-mesh20-TBB2
exp3o=/ddn/data/rfmw74/h7X4hopperUni100k-reluctant-penalty-mesh20-TBB2
exp4o=/ddn/data/rfmw74/h7X4hopperUni100k-reluctant-bf-mesh40-TBB2
exp5o=/ddn/data/rfmw74/h7X4hopperUni100k-reluctant-penalty-mesh40-TBB2

exp0="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform100k 1000 reluctant-grid 0.00003 never 10 true bf 10 2"
exp1="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform100k 1000 reluctant-grid 0.00003 never 10 true penalty 10 2"
exp2="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform100k 1000 reluctant-grid 0.00003 never 10 true bf 20 2"
exp3="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform100k 1000 reluctant-grid 0.00003 never 10 true penalty 20 2"
exp4="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform100k 1000 reluctant-grid 0.00003 never 10 true bf 40 2"
exp5="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform100k 1000 reluctant-grid 0.00003 never 10 true penalty 40 2"

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