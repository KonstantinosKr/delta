cmake_minimum_required(VERSION 3.20)
project(delta_dem LANGUAGES CXX)

# --- Build scaffolding ------------------------------------------------------
# Serial baseline target. AMReX (Phase 2+) is added behind find_package(AMReX)
# so this tree keeps building without it.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DELTA_USE_AMREX "Build the AMReX-backed target (Phase 2+)" OFF)

find_package(VTK REQUIRED COMPONENTS
  CommonCore CommonDataModel IOLegacy IOXML IOImport IOExport)
find_package(assimp QUIET)

file(GLOB_RECURSE DELTA_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(FILTER DELTA_SOURCES EXCLUDE REGEX "/main\\.cpp$")

add_library(delta_lib STATIC ${DELTA_SOURCES})
target_compile_definitions(delta_lib PUBLIC Dim3 iREAL=double byteAlignment=32)
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
  target_link_libraries(delta_lib PUBLIC AMReX::amrex)
  target_compile_definitions(delta_lib PUBLIC DELTA_USE_AMREX)
endif()

# --- Tests ------------------------------------------------------------------
enable_testing()
add_executable(delta_viz_test "${CMAKE_CURRENT_SOURCE_DIR}/../tests/viz_write_test.cpp")
target_link_libraries(delta_viz_test PRIVATE delta_lib)
vtk_module_autoinit(TARGETS delta_viz_test MODULES ${VTK_LIBRARIES})
add_test(NAME viz_write COMMAND delta_viz_test)
