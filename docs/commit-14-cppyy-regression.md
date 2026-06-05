# cppyy Regression

## Why

The executable is now guarded by a golden-output test, but cppyy is the other
downstream interoperability surface. Header and module work must not break the
ability to load HERSS through cppyy, construct the legacy object graph, and run
the uTAHPS fixture.

## What Changed

- Added `HerssCppyyInterop`, a Python CTest driven by cppyy.
- The test includes `herss.h`, loads the built `libherss.so`, runs uTAHPS
  through the C++ object API, and checks the reference value function.
- The test also verifies that the legacy node-to-scenario pointer relationship
  and dataset raw pointer views remain available from cppyy.
- CMake now fails early when cppyy testing is enabled but cppyy cannot be
  imported by the configured Python interpreter.

This environment needs Python 3.12 with cppyy installed and Cling pinned to the
GCC 15 standard library headers. CMake auto-detects `g++-15` and derives the
required `EXTRA_CLING_ARGS`; callers can override `HERSS_CPPYY_PYTHON` or
`HERSS_CPPYY_EXTRA_CLING_ARGS` when needed.

## Verification

```bash
cmake --build build --clean-first
ctest --test-dir build --output-on-failure
```
