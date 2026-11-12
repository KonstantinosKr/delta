#!/usr/bin/env bash
# Fetch and build the AMReX version this project is pinned to (3D, serial, CPU).
set -euo pipefail
AMREX_VERSION="${AMREX_VERSION:-25.11}"
# The vendored AMReX lives at the repo-root third_party/ so the delta backend
# and the amrex_contact reference tree find the same install.
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC="${ROOT}/third_party/amrex-${AMREX_VERSION}"
PREFIX="${ROOT}/third_party/amrex-install"

if [[ ! -d "${SRC}" ]]; then
  mkdir -p "${ROOT}/third_party"
  curl -sSL "https://github.com/AMReX-Codes/amrex/archive/refs/tags/${AMREX_VERSION}.tar.gz" \
    | tar xz -C "${ROOT}/third_party"
fi

cmake -S "${SRC}" -B "${ROOT}/third_party/amrex-build" \
  -DCMAKE_BUILD_TYPE=Release \
  -DAMReX_SPACEDIM=3 \
  -DAMReX_MPI=OFF \
  -DAMReX_PARTICLES=ON \
  -DAMReX_PIC=ON \
  -DAMReX_FORTRAN=OFF \
  -DAMReX_ASSERTIONS=OFF \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX="${PREFIX}"
cmake --build "${ROOT}/third_party/amrex-build" -j"$(nproc)" --target install
echo "AMReX ${AMREX_VERSION} installed in ${PREFIX}"
