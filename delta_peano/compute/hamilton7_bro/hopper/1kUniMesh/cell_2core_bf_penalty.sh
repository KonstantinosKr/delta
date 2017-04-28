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

exp0o=/ddn/data/rfmw74/h7X4hopperUniMesh1k-adaptive-bf-mesh10-TBB2
exp1o=/ddn/data/rfmw74/h7X4hopperUniMesh1k-adaptive-penalty-mesh10-TBB2
exp2o=/ddn/data/rfmw74/h7X4hopperUniMesh1k-adaptive-bf-mesh20-TBB2
exp3o=/ddn/data/rfmw74/h7X4hopperUniMesh1k-adaptive-penalty-mesh20-TBB2

exp0="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniformMesh1k 1000 adaptive-grid 0.00003 never 10 1 bf 10 2"
exp1="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniformMesh1k 1000 adaptive-grid 0.00003 never 10 1 penalty 10 2"
exp2="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniformMesh1k 1000 adaptive-grid 0.00003 never 10 1 bf 20 2"
exp3="./dem-3d-release-tbb 0.3 0.003 0.3 hopperUniformMesh1k 1000 adaptive-grid 0.00003 never 10 1 penalty 20 2"

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