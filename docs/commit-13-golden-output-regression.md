# Golden Output Regression

## Why

The only downstream compatibility surface for now is the compiled executable and
cppyy bindings. Before replacing more headers with modules, the executable needs
a direct regression guard that proves the same uTAHPS input fixture still writes
the same output files.

The Linux executable should also be named like a Linux executable. The `.exe`
suffix was kept from earlier project history but is misleading for the current
build.

## What Changed

- Renamed the CMake executable output from `herss.exe` to `herss`.
- Updated the legacy Makefile, CLI usage text, tests, and docs to use `herss`.
- Added `GoldenOutputTest.UtahpsExecutableOutputMatchesReferenceFiles`.
- Added checked-in uTAHPS reference output files under
  `src_tests/utahps_test/reference_output`.

The golden test runs the compiled executable in a temporary directory, copies the
uTAHPS input fixture, writes a temporary global config with isolated input and
output paths, and compares all generated output files against the reference
directory. File comparison is exact apart from CRLF normalization.

## Verification

```bash
cmake --build build --clean-first
ctest --test-dir build --output-on-failure
```
