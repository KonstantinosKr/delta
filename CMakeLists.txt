cmake_minimum_required(VERSION 3.20)
project(delta LANGUAGES C CXX)

# --- Layout -----------------------------------------------------------------
#   src/                backend-agnostic core + backends/{serial,amrex}
#   apps/               entry points (one main.cpp per app)
#   tests/              one executable per suite
#   amrex_contact/      standalone contact/morphology experiment tree
#   third_party/        vendored AMReX install (git-ignored, tools/build_amrex.sh)

# Vendored AMReX, if it was built (amrex_contact/tools/build_amrex.sh).
list(APPEND CMAKE_PREFIX_PATH
     "${PROJECT_SOURCE_DIR}/third_party/amrex-install")
find_package(AMReX CONFIG QUIET)

# AMReX is the parallel backend, not an optional extra: default on whenever an
# install is found, so a machine without one builds serial-only instead of
# failing. -DDELTA_USE_AMREX=OFF still forces the serial build.
option(DELTA_USE_AMREX "Build the AMReX-backed backend" ${AMReX_FOUND})
option(DELTA_BUILD_AMREX_CONTACT "Build the amrex_contact experiment tree" ON)

if(DELTA_USE_AMREX)
  # AMReX headers need C++17 and its particle machinery needs C++20.
  set(CMAKE_CXX_STANDARD 20)
else()
  set(CMAKE_CXX_STANDARD 14)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only what the code uses. The old list also pulled IOImport/IOExport, which
# transitively require RenderingCore/FiltersCore - whole subsystems nothing here
# instantiates. Keep this narrow or a from-source VTK build needs OpenGL too.
find_package(VTK REQUIRED COMPONENTS
  CommonCore CommonDataModel IOLegacy IOXML)
find_package(assimp QUIET)

# The 2D solver is plain C++ and must stay buildable without VTK, so it is its
# own target and delta3d_core takes a recursive glob minus dim2. The 3D IO does
# not depend on dim2 and dim2 only reaches core/io/MeshBuffer.h, which is
# header-only VTK-free, so excluding it is safe in both directions.
file(GLOB_RECURSE DELTA2D_SOURCES CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/dim2/*.cpp")
file(GLOB_RECURSE DELTA3D_SOURCES CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/*.cpp")
if(NOT DELTA_USE_AMREX)
  # The AMReX backend is AMReX-only; keep it out of the serial build.
  list(FILTER DELTA3D_SOURCES EXCLUDE REGEX "/dim3/backends/amrex/")
endif()
list(REMOVE_ITEM DELTA3D_SOURCES ${DELTA2D_SOURCES})
# The writer is VTK-dependent and is compiled into delta2d_io below; the
# glob above would otherwise drag it into the "VTK-free" 2D solver.
list(FILTER DELTA2D_SOURCES EXCLUDE REGEX "/dim2/Dem2DWrite\\.cpp$")

# VTK-free 2D solver: Vec2 + Dem2D.cpp, no writer, no VTK, no assimp.
add_library(delta2d_core STATIC ${DELTA2D_SOURCES})
target_compile_definitions(delta2d_core PUBLIC iREAL=double)
target_include_directories(delta2d_core PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_library(delta3d_core STATIC ${DELTA3D_SOURCES})
target_compile_definitions(delta3d_core PUBLIC iREAL=double byteAlignment=32)
#Scenario assets (hopper.mbfcp, *.STL) sit at the repo root; pin the path so the
#binaries run from any cwd instead of only from build/.
target_compile_definitions(delta3d_core PUBLIC DELTA_INPUT_DIR="${PROJECT_SOURCE_DIR}/input/dim3")
target_include_directories(delta3d_core PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(delta3d_core PUBLIC ${VTK_LIBRARIES})
if(assimp_FOUND)
  target_link_libraries(delta3d_core PUBLIC assimp::assimp)
endif()

# Links both: the 2D writer lives in dem2d and pulls MeshBuffer out of delta_lib.
add_library(delta2d_io STATIC "${PROJECT_SOURCE_DIR}/src/dim2/Dem2DWrite.cpp")
target_link_libraries(delta2d_io PUBLIC delta2d_core delta3d_core)
vtk_module_autoinit(TARGETS delta2d_io MODULES ${VTK_LIBRARIES})

add_executable(delta "${PROJECT_SOURCE_DIR}/apps/dem3d/main.cpp")
target_link_libraries(delta PRIVATE delta3d_core)
vtk_module_autoinit(TARGETS delta MODULES ${VTK_LIBRARIES})

# 2D entry point. Needs the writer, so it links delta2d_io rather than the
# VTK-free core alone.
add_executable(delta2d "${PROJECT_SOURCE_DIR}/apps/dem2d/main.cpp")
target_link_libraries(delta2d PRIVATE delta2d_io)
vtk_module_autoinit(TARGETS delta2d MODULES ${VTK_LIBRARIES})

if(DELTA_USE_AMREX)
  find_package(AMReX REQUIRED CONFIG)
  target_link_libraries(delta3d_core PUBLIC AMReX::amrex)
  target_compile_definitions(delta3d_core PUBLIC DELTA_USE_AMREX)
endif()

# --- Tests ------------------------------------------------------------------
enable_testing()
add_executable(delta_viz_test "${PROJECT_SOURCE_DIR}/tests/viz_write_test.cpp")
target_link_libraries(delta_viz_test PRIVATE delta3d_core)
vtk_module_autoinit(TARGETS delta_viz_test MODULES ${VTK_LIBRARIES})
add_test(NAME viz_write COMMAND delta_viz_test)

add_executable(delta_dem_test "${PROJECT_SOURCE_DIR}/tests/dem_phase0_test.cpp")
target_link_libraries(delta_dem_test PRIVATE delta3d_core)
vtk_module_autoinit(TARGETS delta_dem_test MODULES ${VTK_LIBRARIES})
add_test(NAME dem_phase0 COMMAND delta_dem_test)

# 2D circle/triangle core. Dem2D.cpp is plain C++; Dem2DWrite.cpp pulls in
# VTK through delta3d_core to emit the same .vtu/.pvd convention as the 3D path.
add_executable(delta_dem2d_test "${PROJECT_SOURCE_DIR}/tests/dem2d_test.cpp")
# Physics only: no writer, no VTK. This is the target that proves the 2D core
# stands alone.
target_link_libraries(delta_dem2d_test PRIVATE delta2d_core)
add_test(NAME dem2d COMMAND delta_dem2d_test)

add_executable(delta_dem2d_demo "${PROJECT_SOURCE_DIR}/tests/dem2d_demo.cpp")
target_link_libraries(delta_dem2d_demo PRIVATE delta2d_io)
vtk_module_autoinit(TARGETS delta_dem2d_demo MODULES ${VTK_LIBRARIES})

add_executable(delta_dem2d_viz_test "${PROJECT_SOURCE_DIR}/tests/dem2d_viz_test.cpp")
target_link_libraries(delta_dem2d_viz_test PRIVATE delta2d_io)
vtk_module_autoinit(TARGETS delta_dem2d_viz_test MODULES ${VTK_LIBRARIES})
add_test(NAME dem2d_viz COMMAND delta_dem2d_viz_test)

# Latent-bug regression guards.
add_executable(delta_regression_test "${PROJECT_SOURCE_DIR}/tests/regression_test.cpp")
target_link_libraries(delta_regression_test PRIVATE delta3d_core)
vtk_module_autoinit(TARGETS delta_regression_test MODULES ${VTK_LIBRARIES})
add_test(NAME regression COMMAND delta_regression_test)

# Guard for the shared .vtu writer: MeshBuffer::validate() must reject the
# geometry that would otherwise be written silently wrong.
add_executable(delta_meshbuffer_test "${PROJECT_SOURCE_DIR}/tests/meshbuffer_test.cpp")
target_link_libraries(delta_meshbuffer_test PRIVATE delta3d_core)
vtk_module_autoinit(TARGETS delta_meshbuffer_test MODULES ${VTK_LIBRARIES})
add_test(NAME mesh_buffer COMMAND delta_meshbuffer_test)

# Broadphase grid modes: the grid may change which pairs are tested, never which
# pairs are in contact. Single process, so it needs no MPI.
if(DELTA_USE_AMREX)
  add_executable(delta_grid_modes_test "${PROJECT_SOURCE_DIR}/tests/grid_modes_test.cpp")
  target_link_libraries(delta_grid_modes_test PRIVATE delta3d_core)
  vtk_module_autoinit(TARGETS delta_grid_modes_test MODULES ${VTK_LIBRARIES})
  add_test(NAME grid_modes COMMAND delta_grid_modes_test)
  set_tests_properties(grid_modes PROPERTIES ENVIRONMENT "AMREX_VERBOSE=0")
endif()

# The AMReX test reads amrex::ParallelDescriptor's communicator, so it needs an
# MPI-enabled AMReX: with AMReX_MPI=OFF, amrex::mpidatatypes::MPI_Comm is an int
# and does not convert to OpenMPI's pointer type.
if(DELTA_USE_AMREX AND AMReX_MPI)
  # For MPIEXEC_EXECUTABLE, used by the parallel tests below.
  find_package(MPI COMPONENTS CXX REQUIRED)
  add_executable(delta_dem_amrex_test "${PROJECT_SOURCE_DIR}/tests/dem_phase1_amrex_test.cpp")
  target_link_libraries(delta_dem_amrex_test PRIVATE delta3d_core)
  vtk_module_autoinit(TARGETS delta_dem_amrex_test MODULES ${VTK_LIBRARIES})
  # Run both at one rank and at two: the parallel path's whole point is that the
  # result does not depend on the rank count. --allow-run-as-root because CI
  # here runs as root.
  add_test(NAME dem_phase1_amrex_np1
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 1 --allow-run-as-root
            $<TARGET_FILE:delta_dem_amrex_test>)
  add_test(NAME dem_phase1_amrex_np2
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2 --allow-run-as-root
            $<TARGET_FILE:delta_dem_amrex_test>)
elseif(DELTA_USE_AMREX)
  message(STATUS "delta_dem_amrex_test skipped: AMReX was built without MPI")
endif()

# --- amrex_contact ------------------------------------------------------------
# Self-contained experiment tree (own CMakeLists, own targets). Only added when
# the vendored AMReX install exists; it is not part of the delta binaries.
if(DELTA_BUILD_AMREX_CONTACT)
  if(EXISTS "${PROJECT_SOURCE_DIR}/third_party/amrex-install/lib/libamrex_3d.a")
    add_subdirectory(amrex_contact)
  else()
    message(STATUS "amrex_contact skipped: third_party/amrex-install not built")
  endif()
endif()
