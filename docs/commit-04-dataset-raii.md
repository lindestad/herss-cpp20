# Commit 04: Dataset RAII Storage

## What Changed

- Replaced manual `new[]`/`delete[]` ownership in `Dataset` with `std::vector` backing storage.
- Kept the existing raw pointer fields as non-owning views:
  - `price`
  - `inflow`
  - `action`
  - `year`, `month`, `day`, `hour`

## Why

`Dataset` is a contained ownership boundary and a good first RAII migration target. The public data layout is still usable by existing code while allocation and cleanup are now exception-safe.

## Interop Notes

The raw pointer views remain valid for the lifetime of the `Dataset` object. Callers must still not retain those pointers after `Dataset` is destroyed.

## Verification

```sh
cmake --build build
ctest --test-dir build --output-on-failure
```
