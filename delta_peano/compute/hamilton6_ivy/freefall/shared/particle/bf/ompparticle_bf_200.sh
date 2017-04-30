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

exp0o=/scratch/rfmw74/reluctantbfOMPPARTICLE1_200
exp1o=/scratch/rfmw74/reluctantbfOMPPARTICLE2_200
exp2o=/scratch/rfmw74/reluctantbfOMPPARTICLE3_200
exp3o=/scratch/rfmw74/reluctantbfOMPPARTICLE4_200
exp4o=/scratch/rfmw74/reluctantbfOMPPARTICLE6_200
exp5o=/scratch/rfmw74/reluctantbfOMPPARTICLE8_200
exp6o=/scratch/rfmw74/reluctantbfOMPPARTICLE16_200

exp0="./dem-3d-release-omp-particle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 true bf 200 1"
exp1="./dem-3d-release-omp-particle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 true bf 200 2"
exp2="./dem-3d-release-omp-particle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 true bf 200 3"
exp3="./dem-3d-release-omp-particle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 true bf 200 4"
exp4="./dem-3d-release-omp-particle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 true bf 200 6"
exp5="./dem-3d-release-omp-particle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 true bf 200 8"
exp6="./dem-3d-release-omp-particle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 true bf 200 16"

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
