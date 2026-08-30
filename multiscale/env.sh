#!/usr/bin/env bash
# Build environment for the Delta @ Peano 3 application (branch: peano_version)
# Source this file before running make, e.g.:
#   source env.sh
#   make delta-gcc-asserts -j$(nproc)
#
# The Makefile adds the -I/-L/-l flags itself; these variables are bare paths.

# Assimp (v5 headers + lib in /usr/local)
export ASSIMP_INC="/usr/local/include"
export ASSIMP_LIB="/usr/local/lib -lassimp"

# Delta library include path (the delta source tree is symlinked into multiscale/)
export DELTA_INC="../delta"

# TBB (oneAPI TBB installed system-wide)
export TBB_INC="/usr/include"
export TBB_SHLIB="/usr/lib -ltbb"

# VTK/Paraview 5.9 (headers only needed; the VTK code in delta/core/io is commented out)
export VTK_INC="/usr/local/include/paraview-5.9"
export VTK_LIB="/usr/local/lib"

# MPI (system OpenMPI)
export MPI_INC=""
export MPI_SHLIB="-lmpi"

# Make sure the runtime can find the libs
export LD_LIBRARY_PATH="/usr/local/lib:/usr/lib:$LD_LIBRARY_PATH"
