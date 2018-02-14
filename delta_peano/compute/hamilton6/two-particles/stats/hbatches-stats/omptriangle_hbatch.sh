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
JOB_SCRIPT6=runjobBatch24
JOB_SCRIPT7=runjobBatch24

exp0o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-batchesOMPTRIANGLE1_50_100hopper_batch25
exp1o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-batchesOMPTRIANGLE2_50_100hopper_batch20
exp2o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-batchesOMPTRIANGLE3_50_100hopper_batch15
exp3o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-batchesOMPTRIANGLE4_50_100hopper_batch10
exp4o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-batchesOMPTRIANGLE6_50_100hopper_batch5
exp5o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-batchesOMPTRIANGLE8_50_100hopper_batch3
exp6o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-batchesOMPTRIANGLE16_50_100hopper_batch1

exp0="./delta-release-tbb-omp-triangle25 0.1 hopperUniformMesh 100 regular-grid 0.00001 never 10 true hybrid-on-batches 50"
exp1="./delta-release-tbb-omp-triangle20 0.1 hopperUniformMesh 100 regular-grid 0.00001 never 10 true hybrid-on-batches 50"
exp2="./delta-release-tbb-omp-triangle15 0.1 hopperUniformMesh 100 regular-grid 0.00001 never 10 true hybrid-on-batches 50"
exp3="./delta-release-tbb-omp-triangle10 0.1 hopperUniformMesh 100 regular-grid 0.00001 never 10 true hybrid-on-batches 50"
exp4="./delta-release-tbb-omp-triangle5 0.1 hopperUniformMesh 100 regular-grid 0.00001 never 10 true hybrid-on-batches 50"
exp5="./delta-release-tbb-omp-triangle3 0.1 hopperUniformMesh 100 regular-grid 0.00001 never 10 true hybrid-on-batches 50"
exp6="./delta-release-tbb-omp-triangle1 0.1 hopperUniformMesh 100 regular-grid 0.00001 never 10 true hybrid-on-batches 50"

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
