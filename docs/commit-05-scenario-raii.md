# Commit 05: Scenario RAII Storage

## What Changed

- Replaced `Scenario` manual array ownership with `std::vector` backing storage.
- Preserved the existing public pointer fields, including `action` as a `double**` view.
- Made the default constructor leave pointer views null instead of uninitialized.

## Why

`Scenario` owns the largest set of per-timestep arrays. Moving it to RAII removes a broad manual cleanup surface while preserving the current simulation and test access patterns.

## Interop Notes

Existing callers can continue indexing fields such as `S->price[t]`, `S->action[t][n]`, and `S->res_Mm3[t]` for the lifetime of the `Scenario`.

## Verification

```sh
cmake --build build
ctest --test-dir build --output-on-failure
```
