#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch2
JOB_SCRIPT1=runjobBatch2
JOB_SCRIPT2=runjobBatch2
JOB_SCRIPT3=runjobBatch2
JOB_SCRIPT4=runjobBatch2
JOB_SCRIPT5=runjobBatch2

exp0o=/ddn/data/rfmw74/h7X21hopperUni1k-regular-sphere-mesh10-tbb
exp1o=/ddn/data/rfmw74/h7X22hopperUni1k-adaptive-sphere-mesh10-tbb
exp2o=/ddn/data/rfmw74/h7X23hopperUni1k-reluctant-sphere-mesh10-tbb
exp3o=/ddn/data/rfmw74/h7X24hopperUni1k-regular-sphere-mesh10-particle
exp4o=/ddn/data/rfmw74/h7X25hopperUni1k-adaptive-sphere-mesh10-particle
exp5o=/ddn/data/rfmw74/h7X26hopperUni1k-reluctant-sphere-mesh10-particle

exp0="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true sphere 10 2"
exp1="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform1k 1000 adaptive-grid 0.00003 never 10 true sphere 10 2"
exp2="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform1k 1000 reluctant-adaptive-grid 0.00003 never 10 true sphere 10 2"
exp3="./dem-3d-release-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true sphere 10"
exp4="./dem-3d-release-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 adaptive-grid 0.00003 never 10 true sphere 10"
exp5="./dem-3d-release-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 reluctant-adaptive-grid 0.00003 never 10 true sphere 10"

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
