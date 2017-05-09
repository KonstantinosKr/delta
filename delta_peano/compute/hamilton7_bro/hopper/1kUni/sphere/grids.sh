#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch24
JOB_SCRIPT1=runjobBatch24
JOB_SCRIPT2=runjobBatch24
JOB_SCRIPT3=runjobBatch24
JOB_SCRIPT4=runjobBatch24
JOB_SCRIPT5=runjobBatch24
JOB_SCRIPT6=runjobBatch24
JOB_SCRIPT7=runjobBatch24
JOB_SCRIPT8=runjobBatch24tbb12omp2
JOB_SCRIPT9=runjobBatch24tbb12omp2
JOB_SCRIPT10=runjobBatch24tbb12omp2
JOB_SCRIPT11=runjobBatch24tbb12omp2

exp0o=/ddn/data/rfmw74/h7X21hopperUni1k-nogrid-sphere-tbb24
exp1o=/ddn/data/rfmw74/h7X21hopperUni1k-regular-sphere-tbb24
exp2o=/ddn/data/rfmw74/h7X22hopperUni1k-adaptive-sphere-tbb24
exp3o=/ddn/data/rfmw74/h7X23hopperUni1k-reluctant-sphere-tbb24

exp4o=/ddn/data/rfmw74/h7X24hopperUni1k-regular-sphere-particle24
exp5o=/ddn/data/rfmw74/h7X24hopperUni1k-regular-sphere-particle24
exp6o=/ddn/data/rfmw74/h7X25hopperUni1k-adaptive-sphere-particle24
exp7o=/ddn/data/rfmw74/h7X26hopperUni1k-reluctant-sphere-particle24

exp8o=/ddn/data/rfmw74/h7X21hopperUni1k-nogrid-sphere-tbb12omp2
exp9o=/ddn/data/rfmw74/h7X21hopperUni1k-regular-sphere-tbb12omp2
exp10o=/ddn/data/rfmw74/h7X22hopperUni1k-adaptive-sphere-tbb12omp2
exp11o=/ddn/data/rfmw74/h7X23hopperUni1k-reluctant-sphere-tbb12omp2

exp0="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform1k 1000 no-grid 0.00003 never 10 true sphere 10 24"
exp1="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true sphere 10 24"
exp2="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform1k 1000 adaptive-grid 0.00003 never 10 true sphere 10 24"
exp3="./dem-3d-release-tbb 0.1 0.001 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true sphere 10 24"

exp4="./dem-3d-release-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 no-grid 0.00003 never 10 true sphere 10"
exp5="./dem-3d-release-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true sphere 10"
exp6="./dem-3d-release-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 adaptive-grid 0.00003 never 10 true sphere 10"
exp7="./dem-3d-release-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true sphere 10"

exp8="./dem-3d-release-tbb-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 no-grid 0.00003 never 10 true sphere 10 12"
exp9="./dem-3d-release-tbb-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 regular-grid 0.00003 never 10 true sphere 10 12"
exp10="./dem-3d-release-tbb-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 adaptive-grid 0.00003 never 10 true sphere 10 12"
exp11="./dem-3d-release-tbb-omp-particle 0.1 0.001 0.1 hopperUniform1k 1000 reluctant-grid 0.00003 never 10 true sphere 10 12"

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
