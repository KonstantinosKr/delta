#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch

exp0o=/ddn/data/rfmw74/h7-hopperUniform1k-regular-penalty-m40-novec-BRANCH
exp1o=/ddn/data/rfmw74/h7-hopperUniform1k-regular-penalty-m40-novec-FLOPS
exp2o=/ddn/data/rfmw74/h7-hopperUniform1k-regular-penalty-m40-novec-L2CACHE
exp3o=/ddn/data/rfmw74/h7-hopperUniform1k-regular-penalty-m40-novec-MEM

exp0="likwid-perfctr -f -C 1 -g BRANCH ./delta-release-novec 0.1 hopperUniform1k 25 regular-grid 0.0001 never 10 true penalty 40"
exp1="likwid-perfctr -f -C 1 -g FLOPS_DP ./delta-release-novec 0.1 hopperUniform1k 25 regular-grid 0.0001 never 10 true penalty 40"
exp2="likwid-perfctr -f -C 1 -g L2CACHE ./delta-release-novec 0.1 hopperUniform1k 25 regular-grid 0.0001 never 10 true penalty 40"
exp3="likwid-perfctr -f -C 1 -g MEM ./delta-release-novec 0.1 hopperUniform1k 25 regular-grid 0.0001 never 10 true penalty 40"

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
