# Commit 07: Herss RAII Ownership

## What Changed

- Replaced manual `new`/`delete` ownership in `Herss` with:
  - `std::unique_ptr<Riversystem>`
  - `std::vector<std::unique_ptr<Scenario>>`
- Preserved public raw views:
  - `rs`
  - `scen`

## Why

`Herss` owns the top-level simulation graph. Moving ownership into RAII containers makes teardown safe and keeps constructor failure paths simpler.

## Interop Notes

Existing callers can still use `herss.rs` and `herss.scen[n]` while the `Herss` object is alive.

## Verification

```sh
cmake --build build
ctest --test-dir build --output-on-failure
```
