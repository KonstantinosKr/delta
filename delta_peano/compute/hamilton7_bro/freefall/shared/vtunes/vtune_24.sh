#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatch24

exp0o=/ddn/data/rfmw74/h7reluctantSphere-24THREADS
exp1o=/ddn/data/rfmw74/h7reluctantBf-24THREADS
exp2o=/ddn/data/rfmw74/h7reluctantPenalty-24THREADS
exp3o=/ddn/data/rfmw74/h7reluctantHtriangles-24THREADS
exp4o=/ddn/data/rfmw74/h7reluctantHbatches-24THREADS

exp0="amplxe-cl -r /ddn/data/rfmw74/h7proreluctantSphere-24THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.1 0.001 0.1 freefallWithGranulates 1000 reluctant-grid 0.0001 never 10 true sphere 200"
exp1="amplxe-cl -r /ddn/data/rfmw74/h7proReluctantBf-24THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.1 0.001 0.1 freefallWithGranulates 1000 reluctant-grid 0.0001 never 10 true bf 200"
exp2="amplxe-cl -r /ddn/data/rfmw74/h7proReluctantPenalty-24THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.1 0.001 0.1 freefallWithGranulates 1000 reluctant-grid 0.0001 never 10 true penalty 200"
exp3="amplxe-cl -r /ddn/data/rfmw74/h7proReluctantHtriangles-24THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.1 0.001 0.1 freefallWithGranulates 1000 reluctant-grid 0.0001 never 10 true hybrid-on-triangle-pairs 200"
exp4="amplxe-cl -r /ddn/data/rfmw74/h7proReluctantHbatches-24THREADS -collect general-exploration -no-auto-finalize -- ./dem-3d-release-omp-triangle-ampl 0.1 0.001 0.1 freefallWithGranulates 1000 reluctant-grid 0.0001 never 10 true hybrid-on-batches 200"

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
