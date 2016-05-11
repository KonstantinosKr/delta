#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchVtunes

exp0o=/scratch/rfmw74/reluctantsphereV
exp1o=/scratch/rfmw74/reluctantbfV
exp2o=/scratch/rfmw74/reluctantpenaltyV
exp3o=/scratch/rfmw74/reluctanthybrid-on-triangle-pairsV
exp4o=/scratch/rfmw74/reluctanthybrid-on-batchesV

exp0="amplxe-cl -r /scratch/rfmw74/sphereProfile -collect hotspots -no-auto-finalize -- ./dem-3d-release-novec-ampl 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 sphere"
exp1="amplxe-cl -r /scratch/rfmw74/bfProfile -collect hotspots -no-auto-finalize -- ./dem-3d-release-novec-ampl 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 bf"
exp2="amplxe-cl -r /scratch/rfmw74/penaltyProfile -collect hotspots -no-auto-finalize -- ./dem-3d-release-novec-ampl 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 penalty"
exp3="amplxe-cl -r /scratch/rfmw74/hybrid_on_triangleProfile -collect hotspots -no-auto-finalize -- ./dem-3d-release-novec-ampl 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 hybrid-on-triangle-pairs"
exp4="amplxe-cl -r /scratch/rfmw74/hybrid_on_batchProfile -collect hotspots -no-auto-finalize -- ./dem-3d-release-novec-ampl 0.5 0.5 0.5 random-velocities 500 reluctant-adaptive-grid 0.001 never 0 hybrid-on-batches"

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










