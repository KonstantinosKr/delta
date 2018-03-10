nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 regular-grid 0.0001 never 10 true bf 20 8 | tee reg20Grid.log &
nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 adaptive-grid 0.0001 never 10 true bf 20 8 | tee ada20Grid.log &
nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true bf 20 8 | tee rel20Grid.log &

nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 regular-grid 0.0001 never 10 true bf 200 8 | tee reg200Grid.log &
nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 adaptive-grid 0.0001 never 10 true bf 200 8 | tee ada200Grid.log &
nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true bf 200 8 | tee rel200Grid.log &

nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 regular-grid 0.0001 never 10 true bf 2000 8 | tee reg2000Grid.log &
nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 adaptive-grid 0.0001 never 10 true bf 2000 8 | tee ada2000Grid.log &
nohup ./delta-icc-release-tbb 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true bf 2000 8 | tee rel2000Grid.log & 