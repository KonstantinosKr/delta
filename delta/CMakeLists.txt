cmake_minimum_required(VERSION 3.20)
project(delta_dem LANGUAGES C CXX)

# --- Build scaffolding ------------------------------------------------------
# Serial baseline target. AMReX (Phase 2+) is added behind find_package(AMReX)
# so this tree keeps building without it.
option(DELTA_USE_AMREX "Build the AMReX-backed target (Phase 2+)" OFF)

if(DELTA_USE_AMREX)
  # AMReX headers need C++17 and its particle machinery needs C++20.
  set(CMAKE_CXX_STANDARD 20)
else()
  set(CMAKE_CXX_STANDARD 14)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(VTK REQUIRED COMPONENTS
  CommonCore CommonDataModel IOLegacy IOXML IOImport IOExport)
find_package(assimp QUIET)

file(GLOB_RECURSE DELTA_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(FILTER DELTA_SOURCES EXCLUDE REGEX "/main\\.cpp$")
if(NOT DELTA_USE_AMREX)
  # The parallel engine is AMReX-only; keep it out of the serial build.
  list(FILTER DELTA_SOURCES EXCLUDE REGEX "/parallel/")
endif()

add_library(delta_lib STATIC ${DELTA_SOURCES})
target_compile_definitions(delta_lib PUBLIC iREAL=double byteAlignment=32)
if(NOT DELTA_USE_AMREX)
  # Nothing in delta/ references Dim3 any more, and the macro mangles
  # amrex::Dim3 in the AMReX headers, so it is serial-build only.
  target_compile_definitions(delta_lib PUBLIC Dim3)
endif()
target_include_directories(delta_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(delta_lib PUBLIC ${VTK_LIBRARIES})
if(assimp_FOUND)
  target_link_libraries(delta_lib PUBLIC assimp::assimp)
endif()

add_executable(delta main.cpp)
target_link_libraries(delta PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta MODULES ${VTK_LIBRARIES})

if(DELTA_USE_AMREX)
  find_package(AMReX REQUIRED CONFIG)
  # For MPIEXEC_EXECUTABLE, used by the parallel test below.
  find_package(MPI COMPONENTS CXX REQUIRED)
  target_link_libraries(delta_lib PUBLIC AMReX::amrex)
  target_compile_definitions(delta_lib PUBLIC DELTA_USE_AMREX)
endif()

# --- Tests ------------------------------------------------------------------
enable_testing()
add_executable(delta_viz_test "${CMAKE_CURRENT_SOURCE_DIR}/../tests/viz_write_test.cpp")
target_link_libraries(delta_viz_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_viz_test MODULES ${VTK_LIBRARIES})
add_test(NAME viz_write COMMAND delta_viz_test)

add_executable(delta_dem_test "${CMAKE_CURRENT_SOURCE_DIR}/../tests/dem_phase0_test.cpp")
target_link_libraries(delta_dem_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_dem_test MODULES ${VTK_LIBRARIES})
add_test(NAME dem_phase0 COMMAND delta_dem_test)

# 2D circle/triangle core. Dem2D.cpp is plain C++; Dem2DWrite.cpp pulls in
# VTK through delta_lib to emit the same .vtu/.pvd convention as the 3D path.
add_executable(delta_dem2d_test "${CMAKE_CURRENT_SOURCE_DIR}/../tests/dem2d_test.cpp")
target_link_libraries(delta_dem2d_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_dem2d_test MODULES ${VTK_LIBRARIES})
add_test(NAME dem2d COMMAND delta_dem2d_test)

add_executable(delta_dem2d_demo "${CMAKE_CURRENT_SOURCE_DIR}/../tests/dem2d_demo.cpp")
target_link_libraries(delta_dem2d_demo PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_dem2d_demo MODULES ${VTK_LIBRARIES})

add_executable(delta_dem2d_viz_test "${CMAKE_CURRENT_SOURCE_DIR}/../tests/dem2d_viz_test.cpp")
target_link_libraries(delta_dem2d_viz_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_dem2d_viz_test MODULES ${VTK_LIBRARIES})
add_test(NAME dem2d_viz COMMAND delta_dem2d_viz_test)

if(DELTA_USE_AMREX)
  add_executable(delta_dem_amrex_test "${CMAKE_CURRENT_SOURCE_DIR}/../tests/dem_phase1_amrex_test.cpp")
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
