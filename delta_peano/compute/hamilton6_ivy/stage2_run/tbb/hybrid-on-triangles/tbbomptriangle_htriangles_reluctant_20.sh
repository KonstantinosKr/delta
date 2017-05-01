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

exp0o=/scratch/rfmw74/reluctantG4hybrid-on-trianglesTBBOMPTRIANGLE1_20
exp1o=/scratch/rfmw74/reluctantG4hybrid-on-trianglesTBBOMPTRIANGLE2_20
exp2o=/scratch/rfmw74/reluctantG4hybrid-on-trianglesTBBOMPTRIANGLE3_20
exp3o=/scratch/rfmw74/reluctantG4hybrid-on-trianglesTBBOMPTRIANGLE4_20
exp4o=/scratch/rfmw74/reluctantG4hybrid-on-trianglesTBBOMPTRIANGLE6_20
exp5o=/scratch/rfmw74/reluctantG4hybrid-on-trianglesTBBOMPTRIANGLE8_20
exp6o=/scratch/rfmw74/reluctantG4hybrid-on-trianglesTBBOMPTRIANGLE16_20

exp0="./dem-3d-release-tbb-omp-triangle 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-triangle-pairs 0.1 20"
exp1="./dem-3d-release-tbb-omp-triangle 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-triangle-pairs 0.1 20"
exp2="./dem-3d-release-tbb-omp-triangle 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-triangle-pairs 0.1 20"
exp3="./dem-3d-release-tbb-omp-triangle 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-triangle-pairs 0.1 20"
exp4="./dem-3d-release-tbb-omp-triangle 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-triangle-pairs 0.1 20"
exp5="./dem-3d-release-tbb-omp-triangle 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-triangle-pairs 0.1 20"
exp6="./dem-3d-release-tbb-omp-triangle 0.3 0.3 0.003 freefall 5000 reluctant-grid 0.0001 never 9.81 hybrid-on-triangle-pairs 0.1 20"

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
