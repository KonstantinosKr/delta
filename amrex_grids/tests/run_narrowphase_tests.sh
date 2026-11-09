#!/usr/bin/env bash
# Build and run the standalone narrowphase tests (no AMReX, no framework).
set -euo pipefail
cd "$(dirname "$0")/.."

CXX="${CXX:-g++}"
OUT="${OUT:-/tmp/test_narrowphase}"

"$CXX" -std=c++17 -O2 -Iinclude \
    src/geom/TriangleDistance.cpp \
    src/geom/Narrowphase.cpp \
    tests/test_narrowphase.cpp \
    -o "$OUT"

"$OUT"
