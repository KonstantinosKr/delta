cmake_minimum_required(VERSION 3.20)
project(amrex_contact LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# AMReX 3D serial install. Look in this directory first (standalone checkout),
# then in the parent Delta tree (this directory vendored under delta/amrex_grids).
list(APPEND CMAKE_PREFIX_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/third_party/amrex-install"
     "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/amrex-install")
find_package(AMReX CONFIG QUIET)

if(NOT AMReX_FOUND)
  message(STATUS "AMReX config not found; importing third_party/amrex-install/lib/libamrex.a")
  add_library(amrex_contact_amrex STATIC IMPORTED)
  set_target_properties(amrex_contact_amrex PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/amrex-install/lib/libamrex.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/amrex-install/include"
    INTERFACE_COMPILE_DEFINITIONS "AMREX_SPACEDIM=3")
  add_library(AMReX::amrex ALIAS amrex_contact_amrex)
endif()

file(GLOB GEOM_SRC   CONFIGURE_DEPENDS src/geom/*.cpp)
file(GLOB GRID_SRC   CONFIGURE_DEPENDS src/grid/*.cpp)
file(GLOB PHYS_SRC   CONFIGURE_DEPENDS src/physics/*.cpp)
file(GLOB SCEN_SRC   CONFIGURE_DEPENDS src/scenario/*.cpp)
file(GLOB APP_SRC    CONFIGURE_DEPENDS src/app/*.cpp)

# ---- main driver -----------------------------------------------------------
add_executable(contact_dem ${APP_SRC} ${GEOM_SRC} ${GRID_SRC} ${PHYS_SRC} ${SCEN_SRC})
target_include_directories(contact_dem PRIVATE include)
target_link_libraries(contact_dem PRIVATE AMReX::amrex)
target_compile_options(contact_dem PRIVATE -Wall -Wextra -Wno-unused-parameter)

# ---- tests -----------------------------------------------------------------
enable_testing()

add_executable(test_narrowphase tests/test_narrowphase.cpp ${GEOM_SRC})
target_include_directories(test_narrowphase PRIVATE include)
add_test(NAME narrowphase COMMAND test_narrowphase)

add_executable(test_physics tests/test_physics.cpp ${PHYS_SRC})
target_include_directories(test_physics PRIVATE include)
add_test(NAME physics COMMAND test_physics)

add_executable(test_scenario tests/test_scenario.cpp ${SCEN_SRC})
target_include_directories(test_scenario PRIVATE include)
add_test(NAME scenario COMMAND test_scenario)

add_executable(test_grid tests/test_grid.cpp ${GRID_SRC})
target_include_directories(test_grid PRIVATE include)
target_link_libraries(test_grid PRIVATE AMReX::amrex)
add_test(NAME grid COMMAND test_grid)

# note: grid uses AMReX so the executable must be initialised before use
