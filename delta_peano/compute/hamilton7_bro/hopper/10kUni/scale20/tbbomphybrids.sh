#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file


JOB_SCRIPT0=tbb1-omp1
JOB_SCRIPT1=tbb1-omp24
JOB_SCRIPT2=tbb2-omp12
JOB_SCRIPT3=tbb2-omp24
JOB_SCRIPT4=tbb6-omp4
JOB_SCRIPT5=tbb6-omp8
JOB_SCRIPT6=tbb12-omp2
JOB_SCRIPT7=tbb12-omp4
JOB_SCRIPT8=tbb24-omp1

JOB_SCRIPT9=tbb1-omp1
JOB_SCRIPT10=tbb1-omp24
JOB_SCRIPT11=tbb2-omp12
JOB_SCRIPT12=tbb2-omp24
JOB_SCRIPT13=tbb6-omp4
JOB_SCRIPT14=tbb6-omp8
JOB_SCRIPT15=tbb12-omp2
JOB_SCRIPT16=tbb12-omp4
JOB_SCRIPT17=tbb24-omp1

exp0o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_1_OMP_1_TRIANGLE_20_10khopper
exp1o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_1_OMP_24_TRIANGLE_20_10khopper
exp2o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_2_OMP_12_TRIANGLE_20_10khopper
exp3o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_2_OMP_24_TRIANGLE_20_10khopper
exp4o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_6_OMP_4_TRIANGLE_20_10khopper
exp5o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_6_OMP_8_TRIANGLE_20_10khopper
exp6o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_12_OMP_2_TRIANGLE_20_10khopper
exp7o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_12_OMP_4_TRIANGLE_20_10khopper
exp8o=/ddn/data/rfmw74/h7regularhybrid-on-triangle-pairsTBB_24_OMP_1_TRIANGLE_20_10khopper

exp9o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_1_OMP_1_TRIANGLE_20_10khopper
exp10o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_1_OMP_24_TRIANGLE_20_10khopper
exp11o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_2_OMP_12_TRIANGLE_20_10khopper
exp12o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_2_OMP_24_TRIANGLE_20_10khopper
exp13o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_6_OMP_4_TRIANGLE_20_10khopper
exp14o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_6_OMP_8_TRIANGLE_20_10khopper
exp15o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_12_OMP_2_TRIANGLE_20_10khopper
exp16o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_12_OMP_4_TRIANGLE_20_10khopper
exp17o=/ddn/data/rfmw74/h7regularhybrid-on-batchesTBB_24_OMP_1_TRIANGLE_20_10khopper

exp0="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 1"
exp1="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 1"
exp2="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 2"
exp3="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 2"
exp4="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 6"
exp5="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 6"
exp6="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 12"
exp7="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 12"
exp8="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-triangle-pairs 20 24"

exp9="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 1"
exp10="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 1"
exp11="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 2"
exp12="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 2"
exp13="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 6"
exp14="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 6"
exp15="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 12"
exp16="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 12"
exp17="./dem-3d-release-tbb-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 regular-grid 0.00003 never 10 true hybrid-on-batches 20 24"

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


sed "s,{OUTPUT_FILE},$exp12o,g" ${JOB_SCRIPT12} > ${JOB_SCRIPT12}_tmp
sed "s,{RUN},$exp12,g" ${JOB_SCRIPT12}_tmp > ${JOB_SCRIPT12}_tmp1
sbatch ${JOB_SCRIPT12}_tmp1

rm ${JOB_SCRIPT12}_tmp
rm ${JOB_SCRIPT12}_tmp1

sed "s,{OUTPUT_FILE},$exp13o,g" ${JOB_SCRIPT13} > ${JOB_SCRIPT13}_tmp
sed "s,{RUN},$exp13,g" ${JOB_SCRIPT13}_tmp > ${JOB_SCRIPT13}_tmp1
sbatch ${JOB_SCRIPT13}_tmp1

rm ${JOB_SCRIPT13}_tmp
rm ${JOB_SCRIPT13}_tmp1

sed "s,{OUTPUT_FILE},$exp14o,g" ${JOB_SCRIPT14} > ${JOB_SCRIPT14}_tmp
sed "s,{RUN},$exp14,g" ${JOB_SCRIPT14}_tmp > ${JOB_SCRIPT14}_tmp1
sbatch ${JOB_SCRIPT14}_tmp1

rm ${JOB_SCRIPT14}_tmp
rm ${JOB_SCRIPT14}_tmp1

sed "s,{OUTPUT_FILE},$exp15o,g" ${JOB_SCRIPT15} > ${JOB_SCRIPT15}_tmp
sed "s,{RUN},$exp15,g" ${JOB_SCRIPT15}_tmp > ${JOB_SCRIPT15}_tmp1
sbatch ${JOB_SCRIPT15}_tmp1

rm ${JOB_SCRIPT15}_tmp
rm ${JOB_SCRIPT15}_tmp1

sed "s,{OUTPUT_FILE},$exp16o,g" ${JOB_SCRIPT16} > ${JOB_SCRIPT16}_tmp
sed "s,{RUN},$exp16,g" ${JOB_SCRIPT16}_tmp > ${JOB_SCRIPT16}_tmp1
sbatch ${JOB_SCRIPT16}_tmp1

rm ${JOB_SCRIPT16}_tmp
rm ${JOB_SCRIPT16}_tmp1

sed "s,{OUTPUT_FILE},$exp17o,g" ${JOB_SCRIPT17} > ${JOB_SCRIPT17}_tmp
sed "s,{RUN},$exp17,g" ${JOB_SCRIPT17}_tmp > ${JOB_SCRIPT17}_tmp1
sbatch ${JOB_SCRIPT17}_tmp1

rm ${JOB_SCRIPT17}_tmp
rm ${JOB_SCRIPT17}_tmp1
