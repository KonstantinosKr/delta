Grid
sh grid/grid20.sh && sh grid/grid200.sh && sh grid/grid2000.sh

Novec
sh novec/bfSerial2000.sh && sh novec/hbatchesSerial2000.sh && sh novec/htrianglesSerial2000.sh && sh novec/penaltySerial2000.sh && sh novec/sphereSerial2000.sh

Vec
sh novec/bfVec2000.sh && sh novec/hbatchesVec2000.sh && sh novec/htrianglesVec2000.sh && sh novec/penaltyVec2000.sh

Shared/cell/BF
sh cell/shared/bf/tbb_bf20_regular.sh && sh cell/shared/bf/tbb_bf200_regular.sh && sh cell/shared/bf/tbb_bf2000_regular.sh
sh cell/shared/bf/tbb_bf20_reluctant.sh && sh cell/shared/bf/tbb_bf200_reluctant.sh && sh cell/shared/bf/tbb_bf200_reluctant.sh

Shared/cell/Sphere
sh shared/cell/bf/tbb_sphere_regular.sh && sh shared/cell/bf/tbb_sphere_reluctant.sh

Shared/mesh/BF
sh shared/mesh/bf/omptriangle_bf_reluctant_20.sh && sh shared/mesh/bf/omptriangle_bf_reluctant_200.sh &&  sh shared/mesh/bf/omptriangle_bf_reluctant_2000.sh

Shared/mesh/HTriangle
sh shared/mesh/hybrid-on-triangles/omptriangle_htriangles_20.sh && sh shared/mesh/hybrid-on-triangles/omptriangle_htriangles_200.sh &&  sh shared/mesh/hybrid-on-triangles/omptriangle_htriangles_2000.sh

Shared/mesh/HBatches
sh shared/mesh/hybrid-on-batches/omptriangle_hbatches_20.sh && sh shared/mesh/hybrid-on-batches/omptriangle_hbatches_200.sh &&  sh shared/mesh/hybrid-on-batches/omptriangle_hbatches_2000.sh

Shared/particle/BF
sh shared/particle/ompparticle_bf_20.sh && sh shared/particle/ompparticle_bf_200.sh && sh shared/particle/ompparticle_bf_20.sh

Shared/particle/Sphere
sh shared/particle/ompparticle_sphere.sh
