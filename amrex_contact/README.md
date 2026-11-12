# amrex_contact - thesis reference implementation

This is the second, independent implementation of the grid-hierarchy
constructions (thesis 6.3/6.4.2) and the contact law, kept because
`docs/RESULTS.md` was measured with it. The morphologies now ship in the delta
backend (`./delta --backend amrex --mode ...`), so nothing in the delta binaries
links this tree; it is opt-in with `-DDELTA_BUILD_AMREX_CONTACT=ON` and provides
two ctest suites: `physics` and `grid`.

What was removed once delta grew its own: the narrowphase (`brute`/`iterative`/
`hybrid`), scenario loading, the VTU writer and the `contact_dem` drivers. The
benchmark harness lives in `../tools/bench/`, the measurements in
`../docs/RESULTS.md`.

Contact law (2.3): `penalty` or `springdashpot`. Time stepping: fixed,
explicit-stability capped, or the chapter-7 dynamic controller.

## Compile

```
