# Commit 02: CMake Test Foundation

## What Changed

- Added a root `CMakeLists.txt`.
- Builds now expose:
  - `herss_core`: reusable static library without `main`.
  - `herss_shared`: shared HERSS library.
  - `herss_cli`: command-line executable named `herss`.
  - `herss_tests`: GoogleTest-based test executable.
- Added `src_tests/test_paths.h` so tests can locate fixtures and the executable from CTest.
- GoogleTest is fetched by CMake instead of requiring `/usr/src/gtest`.
- The legacy `src/Makefile` `test` target now delegates to CMake/CTest.

## Why

The legacy `make test` path failed when system GoogleTest was unavailable. A CMake/CTest foundation gives the project a reproducible test entry point before deeper ownership and module changes.

## Interop Notes

This commit does not change HERSS runtime data structures. The existing include-based API and Makefile remain available.

The parser keeps legacy fixture compatibility for `OUTPUTFILE`, `TRAVELTIME`, and channel blocks that omit `ENDNODE`.

## Verification

Expected command:

```sh
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
cd src && make test
```
