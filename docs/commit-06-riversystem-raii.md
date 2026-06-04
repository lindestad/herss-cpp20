# Commit 06: Riversystem RAII Storage

## What Changed

- Replaced manual array ownership in `Riversystem` with vector-backed storage.
- Preserved public raw views:
  - `nodes`
  - `reservoirs`
  - `pstations`
  - `channels`

## Why

`Riversystem` is the central interop-sensitive graph. This change removes manual ownership while keeping the current contiguous component arrays and node pointer table intact for callers.

## Interop Notes

The raw views point into `Riversystem`-owned vectors and remain valid for the lifetime of the `Riversystem` object. The concrete component arrays remain contiguous.

## Verification

```sh
cmake --build build
ctest --test-dir build --output-on-failure
```
