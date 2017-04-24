#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch12tmp
JOB_SCRIPT1=runjobBatch12tmp
JOB_SCRIPT2=runjobBatch12tmp
JOB_SCRIPT3=runjobBatch12tmp

exp0o=/ddn/data/rfmw74/h7ScaleregularbfOMPTRIANGLE12_200_100hopper
exp1o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE12_200_100hopper
exp2o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-batchesOMPTRIANGLE12_200_100hopper
exp3o=/ddn/data/rfmw74/h7ScaleregularpenaltyOMPTRIANGLE12_200_100hopper

exp0="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 bf 200"
exp1="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 200"
exp2="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-batches 200"
exp3="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 penalty 200"

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