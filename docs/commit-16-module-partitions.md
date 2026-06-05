# Module Partitions

## Why

The first C++20 module step exported everything from one facade. That proved the
toolchain path, but it did not create useful API boundaries. The next safe step
is to split the public module surface into named partitions while keeping the
header compatibility surface for cppyy.

## What Changed

- `src/herss.cppm` is now the primary module interface.
- Added exported partitions:
  - `herss:config`
  - `herss:dataset`
  - `herss:scenario`
  - `herss:riversystem`
- The primary `herss` module re-exports those partitions so existing module
  clients can continue to write `import herss;`.
- CMake now lists all module interface units in the `CXX_MODULES` file set.

Each partition still uses `herss.h` internally for declarations. That preserves
cppyy/header interop while moving native C++ users toward partitioned imports.

## Verification

```bash
cmake --build build --clean-first
ctest --test-dir build --output-on-failure
```
