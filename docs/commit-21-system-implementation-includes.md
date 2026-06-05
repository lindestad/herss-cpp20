# System implementation includes

This commit moves the remaining production implementation files off the legacy
`herss.h` umbrella.

## What changed

- `node.cpp` now includes `herss_node.h`.
- `reservoir.cpp`, `powerstation.cpp`, and `channel.cpp` now include their
  concrete node headers plus `herss_system.h` for `Herss::getDeltaT`.
- `riversystem.cpp`, `herss.cpp`, and `main.cpp` now include `herss_system.h`.
- `arraycurve.cpp` now includes `herss_common.h` and `logger.h` instead of the
  full system umbrella.
- Files that use logging macros include `logger.h` directly.

## Why

The production code no longer depends on the compatibility umbrella for normal
compilation. That makes `herss.h` a true external interop surface and keeps
internal implementation dependencies visible at the top of each file.

Tests and downstream cppyy users can still include `herss.h`.
