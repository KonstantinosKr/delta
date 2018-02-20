#!/bin/bash
#SBATCH --job-name="ExaHyPE-EulerFlow"
#SBATCH -o ExaHyPE-EulerFlow.%A.out
#SBATCH -e ExaHyPE-EulerFlow.%A.err
#SBATCH -t 01:00:00
#SBATCH --exclusive
#SBATCH -p test.q
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=dominic.e.charrier@durham.ac.uk
#SBATCH --mail-type=END
#SBATCH --array=1,12,24
##SBATCH --array=1-24,48 # ham7: 1..24 cores + 48 threads (HT)
##SBATCH --array=1-16,32 # ham6: 1..16 cores + 32 threads (HT) 

source /etc/profile.d/modules.sh
module purge
module load slurm
module load intel/xe_2017.2
module load intelmpi/intel/2017.2
module load gcc
module load likwid

export TBB_SHLIB="-L/ddn/apps/Cluster-Apps/intel/xe_2017.2/tbb/lib/intel64/gcc4.7 -ltbb"

export I_MPI_FABRICS="shm:dapl"

kernels=gen

nodes=1
tasks=1
tasksPerNode=1
coresPerTask=$SLURM_ARRAY_TASK_ID
let procsPerNode=tasksPerNode*coresPerTask

compiler=Intel
sharedMem=TBB
if (( coresPerTask==1 )); then
  sharedMem=None
fi

prefix=EulerFlow-no-output-p3-regular-0
out=$prefix-n$nodes-t$tasksPerNode-c$coresPerTask-$sharedMem-$compiler-$kernels.out

script=benchmarks/multicore/hamilton.slurm-script
spec=benchmarks/multicore/$prefix-t$tasksPerNode-c$coresPerTask.exahype

# pipe some information into output file
module list 2>$out
echo "" >> $out
cat $script >>$out
echo "" >> $out
cat $spec >> $out
#ExaHyPE-EulerFlow-p3-$sharedMem-$compiler --version >> $out

# execute the job
./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> $out

# excecute the job again wrapped in likwid-perfctr calls with different metrics
if (( $coresPerTask==1 )); then
  likwid-perfctr -f -C 0 -g MEM      ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> ${out}.likwid
  likwid-perfctr -f -C 0 -g FLOPS_DP ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> ${out}.likwid
  likwid-perfctr -f -C 0 -g L2CACHE  ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> ${out}.likwid
  likwid-perfctr -f -C 0 -g BRANCH   ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> ${out}.likwid
  # not available on ham7: likwid-perfctr -C 0 -g L1CACHE  ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> $out
else
  let maxCore=coresPerTask-1

  likwid-perfctr -f -C 0-$maxCore -g MEM      ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> ${out}.likwid
  likwid-perfctr -f -C 0-$maxCore -g FLOPS_DP ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> ${out}.likwid
  likwid-perfctr -f -C 0-$maxCore -g L2CACHE  ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> ${out}.likwid
  likwid-perfctr -f -C 0-$maxCore -g BRANCH   ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> ${out}.likwid
  # not available on ham7: likwid-perfctr -C 0-$maxCore -g L1CACHE  ./ExaHyPE-EulerFlow-p3-$sharedMem-$compiler $spec >> $out
fi
