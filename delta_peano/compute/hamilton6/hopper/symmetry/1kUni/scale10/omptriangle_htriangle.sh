#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=../t1
JOB_SCRIPT1=../t2
JOB_SCRIPT2=../t3
JOB_SCRIPT3=../t4
JOB_SCRIPT4=../t6
JOB_SCRIPT5=../t8
JOB_SCRIPT6=../t12
JOB_SCRIPT7=../t16
JOB_SCRIPT8=../t24

exp0o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE1_10_1000hopper
exp1o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE2_10_1000hopper
exp2o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE3_10_1000hopper
exp3o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE4_10_1000hopper
exp4o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE6_10_1000hopper
exp5o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE8_10_1000hopper
exp6o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE12_10_1000hopper
exp7o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE16_10_1000hopper
exp8o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE24_10_1000hopper

exp0="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"
exp1="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"
exp2="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"
exp3="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"
exp4="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"
exp5="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"
exp6="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"
exp7="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"
exp8="./dem-3d-release-tbb-omp-triangle 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 10 1 false false off"

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