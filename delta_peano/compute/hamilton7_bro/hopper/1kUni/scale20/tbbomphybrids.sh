#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=tbb2-omp12
JOB_SCRIPT1=tbb2-omp24
JOB_SCRIPT2=tbb6-omp4
JOB_SCRIPT3=tbb6-omp8
JOB_SCRIPT4=tbb12-omp2
JOB_SCRIPT5=tbb12-omp4
JOB_SCRIPT6=tbb2-omp12
JOB_SCRIPT7=tbb2-omp24
JOB_SCRIPT8=tbb6-omp4
JOB_SCRIPT9=tbb6-omp8
JOB_SCRIPT10=tbb12-omp2
JOB_SCRIPT11=tbb12-omp4

exp0o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_2_OMP_12_TRIANGLE1_20_1000hopper
exp1o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_2_OMP_24_TRIANGLE2_20_1000hopper
exp2o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_6_OMP_4_TRIANGLE3_20_1000hopper
exp3o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_6_OMP_8_RIANGLE4_20_1000hopper
exp4o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_12_OMP_2_RIANGLE6_20_1000hopper
exp5o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_12_OMP_4_RIANGLE8_20_1000hopper

exp6o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_2_OMP_12_TRIANGLE1_20_1000hopper
exp7o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_2_OMP_24_TRIANGLE2_20_1000hopper
exp8o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_6_OMP_4_TRIANGLE3_20_1000hopper
exp9o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_6_OMP_8_RIANGLE4_20_1000hopper
exp10o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_12_OMP_2_RIANGLE6_20_1000hopper
exp11o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_12_OMP_4_RIANGLE8_20_1000hopper

exp0="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20"
exp1="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20"
exp2="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20"
exp3="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20"
exp4="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20"
exp5="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20"

exp6="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20"
exp7="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20"
exp8="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20"
exp9="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20"
exp10="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20"
exp11="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20"


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

sed "s,{OUTPUT_FILE},$exp9o,g" ${JOB_SCRIPT9} > ${JOB_SCRIPT9}_tmp
sed "s,{RUN},$exp9,g" ${JOB_SCRIPT9}_tmp > ${JOB_SCRIPT9}_tmp1
sbatch ${JOB_SCRIPT9}_tmp1

rm ${JOB_SCRIPT9}_tmp
rm ${JOB_SCRIPT9}_tmp1

sed "s,{OUTPUT_FILE},$exp10o,g" ${JOB_SCRIPT10} > ${JOB_SCRIPT10}_tmp
sed "s,{RUN},$exp10,g" ${JOB_SCRIPT10}_tmp > ${JOB_SCRIPT10}_tmp1
sbatch ${JOB_SCRIPT10}_tmp1

rm ${JOB_SCRIPT10}_tmp
rm ${JOB_SCRIPT10}_tmp1

sed "s,{OUTPUT_FILE},$exp11o,g" ${JOB_SCRIPT11} > ${JOB_SCRIPT11}_tmp
sed "s,{RUN},$exp11,g" ${JOB_SCRIPT11}_tmp > ${JOB_SCRIPT11}_tmp1
sbatch ${JOB_SCRIPT11}_tmp1

rm ${JOB_SCRIPT11}_tmp
rm ${JOB_SCRIPT11}_tmp1
