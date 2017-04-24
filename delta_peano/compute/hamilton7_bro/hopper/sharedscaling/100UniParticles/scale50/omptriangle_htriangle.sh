#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch1
JOB_SCRIPT1=runjobBatch2
JOB_SCRIPT2=runjobBatch3
JOB_SCRIPT3=runjobBatch4
JOB_SCRIPT4=runjobBatch6
JOB_SCRIPT5=runjobBatch8
JOB_SCRIPT6=runjobBatch16
JOB_SCRIPT7=runjobBatch24

exp0o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE1_50_100hopper
exp1o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE2_50_100hopper
exp2o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE3_50_100hopper
exp3o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE4_50_100hopper
exp4o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE6_50_100hopper
exp5o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE8_50_100hopper
exp6o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE16_50_100hopper
exp7o=/ddn/data/rfmw74/h7Scaleregularhybrid-on-triangle-pairsOMPTRIANGLE24_50_100hopper

exp0="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 50"
exp1="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 50"
exp2="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 50"
exp3="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 50"
exp4="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 50"
exp5="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 50"
exp6="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 50"
exp7="./dem-3d-release-omp-triangle 0.3 0.003 0.3 hopperUniformMesh 100 regular-grid 0.00001 never 10 1 hybrid-on-triangle-pairs 50"

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