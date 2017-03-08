./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 regular-grid 0.0001 never 10 1 bf 20 | tee reg20Grid.log
./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 adaptive-grid 0.0001 never 10 1 bf 20 | tee ada20Grid.log
./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 bf 20 | tee rel20Grid.log

./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 regular-grid 0.0001 never 10 1 bf 200 | tee reg200Grid.log
./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 adaptive-grid 0.0001 never 10 1 bf 200 | tee ada200Grid.log
./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 bf 200 | tee rel200Grid.log

./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 regular-grid 0.0001 never 10 1 bf 2000 | tee reg2000Grid.log
./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 adaptive-grid 0.0001 never 10 1 bf 2000 | tee ada2000Grid.log
./dem-3d-release-novec 0.3 0.003 0.3 freefall 5000 reluctant-adaptive-grid 0.0001 never 10 1 bf 2000 | tee rel2000Grid.log
