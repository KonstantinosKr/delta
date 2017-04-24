nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 regular-grid 0.0001 never 10 1 bf 20 8 | tee reg20Grid.log &
nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 adaptive-grid 0.0001 never 10 1 bf 20 8 | tee ada20Grid.log &
nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 bf 20 8 | tee rel20Grid.log &

nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 regular-grid 0.0001 never 10 1 bf 200 8 | tee reg200Grid.log &
nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 adaptive-grid 0.0001 never 10 1 bf 200 8 | tee ada200Grid.log &
nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 bf 200 8 | tee rel200Grid.log &

nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 regular-grid 0.0001 never 10 1 bf 2000 8 | tee reg2000Grid.log &
nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 adaptive-grid 0.0001 never 10 1 bf 2000 8 | tee ada2000Grid.log &
nohup ./dem-3d-release-tbb-omp-triangle 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 bf 2000 8 | tee rel2000Grid.log & 