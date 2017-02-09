#REGULAR
#sphere-bf
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 1" -e "OMP_NUM_THREADS=1" > bf-regular-tbb-omp-PARTICLE-G0-1THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 2" -e "OMP_NUM_THREADS=2" > bf-regular-tbb-omp-PARTICLE-G0-2THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 3" -e "OMP_NUM_THREADS=3" > bf-regular-tbb-omp-PARTICLE-G0-3THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 4" -e "OMP_NUM_THREADS=4" > bf-regular-tbb-omp-PARTICLE-G0-4THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 8" -e "OMP_NUM_THREADS=8" > bf-regular-tbb-omp-PARTICLE-G0-8THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 12" -e "OMP_NUM_THREADS=12" > bf-regular-tbb-omp-PARTICLE-G0-12THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 16" -e "OMP_NUM_THREADS=16" > bf-regular-tbb-omp-PARTICLE-G0-16THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 18" -e "OMP_NUM_THREADS=18" > bf-regular-tbb-omp-PARTICLE-G0-18THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 24" -e "OMP_NUM_THREADS=24" > bf-regular-tbb-omp-PARTICLE-G0-24THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 60" -e "OMP_NUM_THREADS=60" > bf-regular-tbb-omp-PARTICLE-G0-60THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 72" -e "OMP_NUM_THREADS=72" > bf-regular-tbb-omp-PARTICLE-G0-72THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 120" -e "OMP_NUM_THREADS=120" > bf-regular-tbb-omp-PARTICLE-G0-120THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 180" -e "OMP_NUM_THREADS=180" > bf-regular-tbb-omp-PARTICLE-G0-180THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 bf 240" -e "OMP_NUM_THREADS=240" > bf-regular-tbb-omp-PARTICLE-G0-240THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 1" -e "OMP_NUM_THREADS=1" > penalty-regular-tbb-omp-PARTICLE-G0-1THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 2" -e "OMP_NUM_THREADS=2" > penalty-regular-tbb-omp-PARTICLE-G0-2THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 3" -e "OMP_NUM_THREADS=3" > penalty-regular-tbb-omp-PARTICLE-G0-3THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 4" -e "OMP_NUM_THREADS=4" > penalty-regular-tbb-omp-PARTICLE-G0-4THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 8" -e "OMP_NUM_THREADS=8" > penalty-regular-tbb-omp-PARTICLE-G0-8THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 12" -e "OMP_NUM_THREADS=12" > penalty-regular-tbb-omp-PARTICLE-G0-12THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 16" -e "OMP_NUM_THREADS=16" > penalty-regular-tbb-omp-PARTICLE-G0-16THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 18" -e "OMP_NUM_THREADS=18" > penalty-regular-tbb-omp-PARTICLE-G0-18THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 24" -e "OMP_NUM_THREADS=24" > penalty-regular-tbb-omp-PARTICLE-G0-24THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 60" -e "OMP_NUM_THREADS=60" > penalty-regular-tbb-omp-PARTICLE-G0-60THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 72" -e "OMP_NUM_THREADS=72" > penalty-regular-tbb-omp-PARTICLE-G0-72THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 120" -e "OMP_NUM_THREADS=120" > penalty-regular-tbb-omp-PARTICLE-G0-120THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 180" -e "OMP_NUM_THREADS=180" > penalty-regular-tbb-omp-PARTICLE-G0-180THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 penalty 240" -e "OMP_NUM_THREADS=240" > penalty-regular-tbb-omp-PARTICLE-G0-240THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 1" -e "OMP_NUM_THREADS=1" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-1THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 2" -e "OMP_NUM_THREADS=2" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-2THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 3" -e "OMP_NUM_THREADS=3" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-3THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 4" -e "OMP_NUM_THREADS=4" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-4THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 8" -e "OMP_NUM_THREADS=8" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-8THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 12" -e "OMP_NUM_THREADS=12" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-12THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 16" -e "OMP_NUM_THREADS=16" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-16THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 18" -e "OMP_NUM_THREADS=18" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-18THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 24" -e "OMP_NUM_THREADS=24" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-24THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 60" -e "OMP_NUM_THREADS=60" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-60THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 72" -e "OMP_NUM_THREADS=72" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-72THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 120" -e "OMP_NUM_THREADS=120" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-120THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 180" -e "OMP_NUM_THREADS=180" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-180THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-batches 240" -e "OMP_NUM_THREADS=240" > hybrid-on-batches-regular-tbb-omp-static-PARTICLE-G0-240THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 1" -e "OMP_NUM_THREADS=1" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-1THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 2" -e "OMP_NUM_THREADS=2" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-2THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 3" -e "OMP_NUM_THREADS=3" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-3THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 4" -e "OMP_NUM_THREADS=4" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-4THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 8" -e "OMP_NUM_THREADS=8" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-8THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 12" -e "OMP_NUM_THREADS=12" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-12THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 16" -e "OMP_NUM_THREADS=16" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-16THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 18" -e "OMP_NUM_THREADS=18" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-18THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 24" -e "OMP_NUM_THREADS=24" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-24THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 60" -e "OMP_NUM_THREADS=60" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-60THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 72" -e "OMP_NUM_THREADS=72" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-72THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 120" -e "OMP_NUM_THREADS=120" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-120THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 180" -e "OMP_NUM_THREADS=180" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-180THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-tbb-omp-particle-phi -a "0.5 0.5 0.5 random-velocities 500 regular-grid 0.001 never 1 10 hybrid-on-triangle-pairs 240" -e "OMP_NUM_THREADS=240" > hybrid-on-triangle-pairs-regular-tbb-omp-static-PARTICLE-G0-240THREADS &

