#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch8

exp0o=/scratch/rfmw74/reluctantSphere-8THREADS
exp1o=/scratch/rfmw74/reluctantBf-8THREADS
exp2o=/scratch/rfmw74/reluctantPenalty-8THREADS
exp3o=/scratch/rfmw74/reluctantHtriangles-8THREADS
exp4o=/scratch/rfmw74/reluctantHbatches-8THREADS

exp0="amplxe-cl -r /scratch/rfmw74/proreluctantSphere-8THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 sphere 200"
exp1="amplxe-cl -r /scratch/rfmw74/proReluctantBf-8THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 bf 200"
exp2="amplxe-cl -r /scratch/rfmw74/proReluctantPenalty-8THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 penalty 200"
exp3="amplxe-cl -r /scratch/rfmw74/proReluctantHtriangles-8THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 hybrid-on-triangle-pairs 200"
exp4="amplxe-cl -r /scratch/rfmw74/proReluctantHbatches-8THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 hybrid-on-batches 200"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp2o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp2,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp3o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp3,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

sed "s,{OUTPUT_FILE},$exp4o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp4,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1
