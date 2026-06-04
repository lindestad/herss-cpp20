# Commit 03: Interop Tests

## What Changed

- Added tests that pin the legacy raw-pointer views exposed by `Dataset`.
- Added tests that pin `Herss` preparation of `Riversystem::nodes` and `Node::S` scenario pointers.

## Why

The modernization work will replace manual ownership with RAII backing storage. These tests make the compatibility boundary executable: current callers can still use raw pointers while internals become safer.

## Interop Notes

The tests intentionally exercise:

- `Dataset::price`, `Dataset::inflow`, `Dataset::action`, and date arrays.
- `Riversystem::nodes` pointing at concrete reservoir, powerstation, and channel storage.
- `Node::S` pointing at the scenarios owned by `Herss`.

## Verification

```sh
cmake --build build
ctest --test-dir build --output-on-failure
```
