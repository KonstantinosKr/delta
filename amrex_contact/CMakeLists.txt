cmake_minimum_required(VERSION 3.20)
project(amrex_contact LANGUAGES C CXX)

# Thesis reference implementation: the grid-hierarchy constructions and the
# contact law as they were measured in docs/RESULTS.md. The morphologies now
# live in src/dim3/backends/amrex/; what is left here is the second
# implementation plus the assertion suites that measured it.
#
# Opt-in because nothing in the delta binaries links it: -DDELTA_BUILD_AMREX_CONTACT=ON.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# AMReX: the vendored install at the repo root, or a standalone checkout.
list(APPEND CMAKE_PREFIX_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/third_party/amrex-install"
     "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/amrex-install")
find_package(AMReX CONFIG QUIET)

if(NOT AMReX_FOUND)
  add_library(amrex_contact_amrex STATIC IMPORTED)
  set_target_properties(amrex_contact_amrex PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/amrex-install/lib/libamrex.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/amrex-install/include"
    INTERFACE_COMPILE_DEFINITIONS "AMREX_SPACEDIM=3")
  add_library(AMReX::amrex ALIAS amrex_contact_amrex)
endif()

file(GLOB GRID_SRC CONFIGURE_DEPENDS src/grid/*.cpp)
file(GLOB PHYS_SRC CONFIGURE_DEPENDS src/physics/*.cpp)

add_library(contact_core STATIC ${GRID_SRC} ${PHYS_SRC})
target_include_directories(contact_core PUBLIC include)
target_link_libraries(contact_core PUBLIC AMReX::amrex)
target_compile_options(contact_core PRIVATE -Wall -Wextra -Wno-unused-parameter)

# ---- tests -----------------------------------------------------------------
enable_testing()

add_executable(test_physics tests/test_physics.cpp ${PHYS_SRC})
target_include_directories(test_physics PRIVATE include)
add_test(NAME physics COMMAND test_physics)

add_executable(test_grid tests/test_grid.cpp ${GRID_SRC})
target_include_directories(test_grid PRIVATE include)
target_link_libraries(test_grid PRIVATE AMReX::amrex)
add_test(NAME grid COMMAND test_grid)

# note: grid uses AMReX so the executable must be initialised before use
