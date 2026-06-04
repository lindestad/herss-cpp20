# HERSS C++20 Modernization Plan

This plan keeps the current in-memory data structure interoperable while moving the codebase toward a C++20, RAII-backed, testable shape.

## Baseline

- The current build path is `src/Makefile`.
- `make all` builds `herss.exe` and `herss.so`.
- `make test` currently depends on `/usr/src/gtest` and fails when that system package is missing.
- The main public surface is the aggregate header `src/herss.h`.
- The interop-sensitive structure is the current `Riversystem`/`Node` graph:
  - `Riversystem::nodes` remains index-addressable as `Node**`.
  - `Riversystem::reservoirs`, `Riversystem::pstations`, and `Riversystem::channels` remain contiguous arrays.
  - Existing callers can continue to read node pointers and concrete component arrays.

## Commit Sequence

1. Document the migration plan.
   - Add this plan and define the compatibility boundary.

2. Add a modern build and test foundation.
   - Add root CMake support using C++20.
   - Build a reusable HERSS core target, CLI target, and CTest test target.
   - Fetch GoogleTest through CMake so tests do not require `/usr/src/gtest`.

3. Make tests path-stable and expand coverage.
   - Replace source-directory-relative fixture paths with test helpers.
   - Add focused tests around ownership, topology parsing, and simulation invariants.

4. Introduce RAII backing storage without breaking interop.
   - Keep public raw pointer views where current callers require them.
   - Back arrays with standard-library owners such as `std::vector` or `std::unique_ptr`.
   - Remove manual `new[]`/`delete[]` paths in focused, verified steps.

5. Add C++20 module entry points.
   - Keep `src/herss.h` available for existing include-based users.
   - Add an importable `herss` module facade first.
   - Move internal declarations behind module partitions only after tests protect behavior.

6. Fix spelling and documentation issues.
   - Correct comments, docs, and build target spelling without changing input file formats.
   - Preserve user-facing legacy keywords such as topology node names.

7. Final verification.
   - Run the CMake build, CTest suite, and legacy Makefile build.
   - Keep each commit small enough to review independently.

## Verification Standard

Each code commit should include:

- A matching short markdown note in `docs/`.
- A build or test command that proves the changed surface.
- No intentional breakage of existing in-memory graph access.
