cmake_minimum_required(VERSION 3.20)
project(delta LANGUAGES C CXX)

# --- Layout -----------------------------------------------------------------
#   src/                backend-agnostic core + backends/{serial,amrex}
#   apps/               entry points (one main.cpp per app)
#   tests/              one executable per suite
#   amrex_grids/        standalone contact/morphology experiment tree
#   third_party/        vendored AMReX install (git-ignored, tools/build_amrex.sh)

# Vendored AMReX, if it was built (amrex_grids/tools/build_amrex.sh).
list(APPEND CMAKE_PREFIX_PATH
     "${PROJECT_SOURCE_DIR}/third_party/amrex-install")

# Serial baseline. The AMReX backend is added behind find_package(AMReX) so this
# tree keeps building without it.
option(DELTA_USE_AMREX "Build the AMReX-backed backend" OFF)
option(DELTA_BUILD_AMREX_CONTACT "Build the amrex_grids experiment tree" ON)

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
# own target and delta_lib takes a recursive glob minus dem2d. The 3D IO does
# not depend on dem2d and dem2d only reaches core/io/MeshBuffer.h, which is
# header-only VTK-free, so excluding it is safe in both directions.
file(GLOB_RECURSE DEM2D_SOURCES CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/dim2/*.cpp")
file(GLOB_RECURSE DELTA_SOURCES CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/*.cpp")
if(NOT DELTA_USE_AMREX)
  # The AMReX backend is AMReX-only; keep it out of the serial build.
  list(FILTER DELTA_SOURCES EXCLUDE REGEX "/dim3/backends/amrex/")
endif()
list(REMOVE_ITEM DELTA_SOURCES ${DEM2D_SOURCES})

# VTK-free 2D solver: Vec2 + Dem2D.cpp, no writer, no VTK, no assimp.
add_library(delta_dem2d STATIC ${DEM2D_SOURCES})
target_compile_definitions(delta_dem2d PUBLIC iREAL=double)
target_include_directories(delta_dem2d PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_library(delta_lib STATIC ${DELTA_SOURCES})
target_compile_definitions(delta_lib PUBLIC iREAL=double byteAlignment=32)
#Scenario assets (hopper.mbfcp, *.STL) sit at the repo root; pin the path so the
#binaries run from any cwd instead of only from build/.
target_compile_definitions(delta_lib PUBLIC DELTA_INPUT_DIR="${PROJECT_SOURCE_DIR}/input/dim3")
if(NOT DELTA_USE_AMREX)
  # Nothing in src/ references Dim3 any more, and the macro mangles
  # amrex::Dim3 in the AMReX headers, so it is serial-build only.
  target_compile_definitions(delta_lib PUBLIC Dim3)
endif()
target_include_directories(delta_lib PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(delta_lib PUBLIC ${VTK_LIBRARIES})
if(assimp_FOUND)
  target_link_libraries(delta_lib PUBLIC assimp::assimp)
endif()

# Links both: the 2D writer lives in dem2d and pulls MeshBuffer out of delta_lib.
add_library(delta_dem2d_io STATIC "${PROJECT_SOURCE_DIR}/src/dim2/Dem2DWrite.cpp")
target_link_libraries(delta_dem2d_io PUBLIC delta_dem2d delta_lib)
vtk_module_autoinit(TARGETS delta_dem2d_io MODULES ${VTK_LIBRARIES})

add_executable(delta "${PROJECT_SOURCE_DIR}/apps/dem3d/main.cpp")
target_link_libraries(delta PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta MODULES ${VTK_LIBRARIES})

# 2D entry point. Needs the writer, so it links delta_dem2d_io rather than the
# VTK-free core alone.
add_executable(delta2d "${PROJECT_SOURCE_DIR}/apps/dem2d/main.cpp")
target_link_libraries(delta2d PRIVATE delta_dem2d_io)
vtk_module_autoinit(TARGETS delta2d MODULES ${VTK_LIBRARIES})

if(DELTA_USE_AMREX)
  find_package(AMReX REQUIRED CONFIG)
  # For MPIEXEC_EXECUTABLE, used by the parallel test below.
  find_package(MPI COMPONENTS CXX REQUIRED)
  target_link_libraries(delta_lib PUBLIC AMReX::amrex)
  target_compile_definitions(delta_lib PUBLIC DELTA_USE_AMREX)
endif()

# --- Tests ------------------------------------------------------------------
enable_testing()
add_executable(delta_viz_test "${PROJECT_SOURCE_DIR}/tests/viz_write_test.cpp")
target_link_libraries(delta_viz_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_viz_test MODULES ${VTK_LIBRARIES})
add_test(NAME viz_write COMMAND delta_viz_test)

add_executable(delta_dem_test "${PROJECT_SOURCE_DIR}/tests/dem_phase0_test.cpp")
target_link_libraries(delta_dem_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_dem_test MODULES ${VTK_LIBRARIES})
add_test(NAME dem_phase0 COMMAND delta_dem_test)

# 2D circle/triangle core. Dem2D.cpp is plain C++; Dem2DWrite.cpp pulls in
# VTK through delta_lib to emit the same .vtu/.pvd convention as the 3D path.
add_executable(delta_dem2d_test "${PROJECT_SOURCE_DIR}/tests/dem2d_test.cpp")
# Physics only: no writer, no VTK. This is the target that proves the 2D core
# stands alone.
target_link_libraries(delta_dem2d_test PRIVATE delta_dem2d)
add_test(NAME dem2d COMMAND delta_dem2d_test)

add_executable(delta_dem2d_demo "${PROJECT_SOURCE_DIR}/tests/dem2d_demo.cpp")
target_link_libraries(delta_dem2d_demo PRIVATE delta_dem2d_io)
vtk_module_autoinit(TARGETS delta_dem2d_demo MODULES ${VTK_LIBRARIES})

add_executable(delta_dem2d_viz_test "${PROJECT_SOURCE_DIR}/tests/dem2d_viz_test.cpp")
target_link_libraries(delta_dem2d_viz_test PRIVATE delta_dem2d_io)
vtk_module_autoinit(TARGETS delta_dem2d_viz_test MODULES ${VTK_LIBRARIES})
add_test(NAME dem2d_viz COMMAND delta_dem2d_viz_test)

# Latent-bug regression guards.
add_executable(delta_regression_test "${PROJECT_SOURCE_DIR}/tests/regression_test.cpp")
target_link_libraries(delta_regression_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_regression_test MODULES ${VTK_LIBRARIES})
add_test(NAME regression COMMAND delta_regression_test)

# Guard for the shared .vtu writer: MeshBuffer::validate() must reject the
# geometry that would otherwise be written silently wrong.
add_executable(delta_meshbuffer_test "${PROJECT_SOURCE_DIR}/tests/meshbuffer_test.cpp")
target_link_libraries(delta_meshbuffer_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_meshbuffer_test MODULES ${VTK_LIBRARIES})
add_test(NAME mesh_buffer COMMAND delta_meshbuffer_test)

if(DELTA_USE_AMREX)
  add_executable(delta_dem_amrex_test "${PROJECT_SOURCE_DIR}/tests/dem_phase1_amrex_test.cpp")
  target_link_libraries(delta_dem_amrex_test PRIVATE delta_lib)
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
endif()

# --- amrex_grids ------------------------------------------------------------
# Self-contained experiment tree (own CMakeLists, own targets). Only added when
# the vendored AMReX install exists; it is not part of the delta binaries.
if(DELTA_BUILD_AMREX_CONTACT)
  if(EXISTS "${PROJECT_SOURCE_DIR}/third_party/amrex-install/lib/libamrex_3d.a")
    add_subdirectory(amrex_grids)
  else()
    message(STATUS "amrex_grids skipped: third_party/amrex-install not built")
  endif()
endif()
