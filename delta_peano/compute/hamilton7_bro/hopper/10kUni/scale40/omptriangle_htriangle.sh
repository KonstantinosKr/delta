#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=t1
JOB_SCRIPT1=t2
JOB_SCRIPT2=t3
JOB_SCRIPT3=t4
JOB_SCRIPT4=t6
JOB_SCRIPT5=t8
JOB_SCRIPT6=t12
JOB_SCRIPT7=t16
JOB_SCRIPT8=t24

exp0o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE1_40_10khopper
exp1o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE2_40_10khopper
exp2o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE3_40_10khopper
exp3o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE4_40_10khopper
exp4o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE6_40_10khopper
exp5o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE8_40_10khopper
exp6o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE12_40_10khopper
exp7o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE16_40_10khopper
exp8o=/ddn/data/rfmw74/h7reluctanthybrid-on-triangle-pairsOMPTRIANGLE24_40_10khopper

exp0="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"
exp1="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"
exp2="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"
exp3="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"
exp4="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"
exp5="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"
exp6="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"
exp7="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"
exp8="./dem-3d-release-omp-triangle 0.1 0.001 0.1 hopperUniform10k 1000 reluctant-grid 0.00003 never 10 true hybrid-on-triangle-pairs 40"

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