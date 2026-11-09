#!/usr/bin/env bash
# Full verification: unit tests, cross-mode contact equivalence, benchmarks.
# Produces the numbers quoted in docs/RESULTS.md. Runtime ~1 min on 2 cores.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "${ROOT}"

BIN="${ROOT}/build/contact_dem"
OUT="${ROOT}/results"
CTEST="${CTEST:-ctest}"
mkdir -p "${OUT}"

echo "== 1. unit tests =="
"${CTEST}" --test-dir build --output-on-failure

echo "== 1b. narrowphase variants on one trajectory (brute / hybrid / hybrid-fast) =="
for np in brute hybrid hybrid-fast; do
    "${BIN}" --n 150 --size-min 0.03 --size-max 0.08 --gravity --steps 300 \
        --mode adaptive --narrow "${np}" --quiet | sed -n '/^mode/,$p'
done

echo "== 2. thesis 7.3 two-body, contacts checked against nogrid+brute every step =="
"${BIN}" --scenario two-body --mode all --steps 7000 --verify \
    --csv "${OUT}/twobody_stiff.csv" --quiet | tee "${OUT}/twobody_stiff.txt"

echo "== 3. same, step limited by the grid (chapter 7 regime) =="
"${BIN}" --scenario two-body --mode all --steps 7000 --dt-source grid \
    --csv "${OUT}/twobody_grid.csv" --quiet | tee "${OUT}/twobody_grid.txt"

echo "== 4. dense packing (150 bodies, gravity): contact-set equivalence =="
"${BIN}" --n 150 --size-min 0.03 --size-max 0.08 --gravity --steps 1500 \
    --mode all --csv "${OUT}/pack_dense.csv" --quiet | tee "${OUT}/pack_dense.txt"

echo "== 5. sparse packing (120 bodies): broadphase scaling =="
"${BIN}" --n 120 --size-min 0.03 --size-max 0.07 --velocity 0.05 --steps 200 \
    --mode all --csv "${OUT}/pack_sparse.csv" --quiet | tee "${OUT}/pack_sparse.txt"

echo "== 6. bench reports =="
for s in twobody_stiff twobody_grid pack_dense pack_sparse; do
    python3 tools/bench.py "${OUT}/${s}".*.csv --no-plots --out "${OUT}/${s}.md" > /dev/null
    echo "  ${OUT}/${s}.md"
done

echo "done -> ${OUT}"
