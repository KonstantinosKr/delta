#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0000=runjobBatch1
JOB_SCRIPT000=runjobBatch1
JOB_SCRIPT00=runjobBatch1
JOB_SCRIPT0=runjobBatch1
JOB_SCRIPT1=runjobBatch2
JOB_SCRIPT2=runjobBatch3
JOB_SCRIPT3=runjobBatch4
JOB_SCRIPT4=runjobBatch6
JOB_SCRIPT5=runjobBatch8
JOB_SCRIPT6=runjobBatch16

exp00o=/ddn/data/rfmw74/streamSerialLikwidMEM
exp00o=/ddn/data/rfmw74/streamSerialLikwidFlop
exp00o=/ddn/data/rfmw74/streamSerial
exp0o=/ddn/data/rfmw74/streamOMP1
exp1o=/ddn/data/rfmw74/streamOMP2
exp2o=/ddn/data/rfmw74/streamOMP3
exp3o=/ddn/data/rfmw74/streamOMP4
exp4o=/ddn/data/rfmw74/streamOMP6
exp5o=/ddn/data/rfmw74/streamOMP8
exp6o=/ddn/data/rfmw74/streamOMP16

exp0000="likwid-perfctr -f -C 1 -g MEM ./streamSerial"
exp000="likwid-perfctr -f -C 1 -g FLOPS_DP ./streamSerial"
exp00="./streamSerial"
exp0="./streamOMP"
exp1="./streamOMP"
exp2="./streamOMP"
exp3="./streamOMP"
exp4="./streamOMP"
exp5="./streamOMP"
exp6="./streamOMP"

sed "s,{OUTPUT_FILE},$exp0000o,g" ${JOB_SCRIPT000} > ${JOB_SCRIPT0000}_tmp
sed "s,{RUN},$exp0000,g" ${JOB_SCRIPT0000}_tmp > ${JOB_SCRIPT0000}_tmp1
sbatch ${JOB_SCRIPT0000}_tmp1

rm ${JOB_SCRIPT0000}_tmp
rm ${JOB_SCRIPT0000}_tmp1

sed "s,{OUTPUT_FILE},$exp000o,g" ${JOB_SCRIPT00} > ${JOB_SCRIPT000}_tmp
sed "s,{RUN},$exp000,g" ${JOB_SCRIPT000}_tmp > ${JOB_SCRIPT000}_tmp1
sbatch ${JOB_SCRIPT000}_tmp1

rm ${JOB_SCRIPT000}_tmp
rm ${JOB_SCRIPT000}_tmp1

sed "s,{OUTPUT_FILE},$exp00o,g" ${JOB_SCRIPT00} > ${JOB_SCRIPT00}_tmp
sed "s,{RUN},$exp00,g" ${JOB_SCRIPT00}_tmp > ${JOB_SCRIPT00}_tmp1
sbatch ${JOB_SCRIPT00}_tmp1

rm ${JOB_SCRIPT00}_tmp
rm ${JOB_SCRIPT00}_tmp1

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

sed "s,{OUTPUT_FILE},$exp6o,g" ${JOB_SCRIPT5} > ${JOB_SCRIPT5}_tmp
sed "s,{RUN},$exp6,g" ${JOB_SCRIPT5}_tmp > ${JOB_SCRIPT5}_tmp1
sbatch ${JOB_SCRIPT5}_tmp1

rm ${JOB_SCRIPT5}_tmp
rm ${JOB_SCRIPT5}_tmp1
