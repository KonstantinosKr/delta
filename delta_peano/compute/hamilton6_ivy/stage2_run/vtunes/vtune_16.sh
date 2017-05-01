#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch16

exp0o=/scratch/rfmw74/reluctantSphere-16THREADS
exp1o=/scratch/rfmw74/reluctantBf-16THREADS
exp2o=/scratch/rfmw74/reluctantPenalty-16THREADS
exp3o=/scratch/rfmw74/reluctantHtriangles-16THREADS
exp4o=/scratch/rfmw74/reluctantHbatches-16THREADS

exp0="amplxe-cl -r /scratch/rfmw74/proreluctantSphere-16THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 sphere 0.1 200"
exp1="amplxe-cl -r /scratch/rfmw74/proReluctantBf-16THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 bf 0.1 200"
exp2="amplxe-cl -r /scratch/rfmw74/proReluctantPenalty-16THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 penalty 0.1 200"
exp3="amplxe-cl -r /scratch/rfmw74/proReluctantHtriangles-16THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-triangle-pairs 0.1 200"
exp4="amplxe-cl -r /scratch/rfmw74/proReluctantHbatches-16THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-batches 0.1 200"

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
