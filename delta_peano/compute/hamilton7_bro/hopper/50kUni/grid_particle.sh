#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch24
JOB_SCRIPT1=runjobBatch24
JOB_SCRIPT2=runjobBatch24
JOB_SCRIPT3=runjobBatch24
JOB_SCRIPT4=runjobBatch24
JOB_SCRIPT5=runjobBatch24

exp0o=/ddn/data/rfmw74/h7X21hopperUni50k-regular-sphere-tbb24
exp1o=/ddn/data/rfmw74/h7X22hopperUni50k-adaptive-sphere-tbb24
exp2o=/ddn/data/rfmw74/h7X23hopperUni50k-reluctant-sphere-tbb24
exp3o=/ddn/data/rfmw74/h7X24hopperUni50k-regular-sphere-particle24
exp4o=/ddn/data/rfmw74/h7X25hopperUni50k-adaptive-sphere-particle24
exp5o=/ddn/data/rfmw74/h7X26hopperUni50k-reluctant-sphere-particle24

exp0="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform50k 10 regular-grid 0.000001 never 10 1 sphere 10 24"
exp1="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform50k 10 adaptive-grid 0.000001 never 10 1 sphere 10 24"
exp2="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniform50k 10 reluctant-adaptive-grid 0.000001 never 10 1 sphere 10 24"
exp3="./dem-3d-release-omp-particle 0.3 0.003 0.3 hopperUniform50k 10 regular-grid 0.000001 never 10 1 sphere 10"
exp4="./dem-3d-release-omp-particle 0.3 0.003 0.3 hopperUniform50k 10 adaptive-grid 0.000001 never 10 1 sphere 10"
exp5="./dem-3d-release-omp-particle 0.3 0.003 0.3 hopperUniform50k 10 reluctant-adaptive-grid 0.000001 never 10 1 sphere 10"

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