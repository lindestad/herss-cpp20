# Qmin and routing headers

This commit moves the lowest-level remaining declarations out of `herss.h`.

## What changed

- Added `herss_qmin.h` for `QminPeriod` and `Qmin`.
- Added `herss_routing.h` for `ReservoirState`, `ReservoirStepResult`, and
  `CascadedReservoirs`.
- Kept `herss.h` as the compatibility umbrella by including the new headers.

## Why

`Qmin` and cascaded routing are leaf-level concepts used by node and channel
classes. Extracting them first gives higher-level declarations a clean include
target and avoids starting the remaining split from the middle of the dependency
graph.

This is a declaration-only move. The executable output and cppyy include path
remain protected by the existing regression tests.
