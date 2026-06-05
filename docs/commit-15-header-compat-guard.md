# Header Compatibility Guard

## Why

cppyy still needs a parseable C++ header. Native C++ users can move toward
`import herss`, but the header cannot be removed or made module-only before a
future tagged transition point.

## What Changed

- Marked `src/herss.h` as the compatibility header for cppyy and legacy C++
  clients.
- Added `HerssHeaderCompatSmoke`, a standalone CTest executable that includes
  `herss.h` directly and links against `herss_core`.

This test keeps the compatibility surface visible. If later module work makes
the header unparseable or removes required legacy symbols, the test fails before
that change can be treated as safe.

## Verification

```bash
cmake --build build --clean-first
ctest --test-dir build --output-on-failure
```
