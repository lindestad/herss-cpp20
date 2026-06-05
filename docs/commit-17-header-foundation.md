# Header shrink foundation

This commit starts reducing `src/herss.h` without changing the public compatibility
entry point.

## What changed

- Added `herss_common.h` for shared constants, macros, smoothing helpers,
  `NodeType`, and forward declarations.
- Added `herss_config.h` for topology parsing, time helpers, line parsing, and
  `GlobalConfig`.
- Added `herss_dataset.h` for `Dataset`.
- Added `herss_scenario.h` for `Scenario`.
- Kept `herss.h` as the umbrella header used by cppyy and legacy C++ callers.
- Pointed the config, dataset, and scenario module partitions at the narrower
  headers instead of the full umbrella.

## Why

The project is moving toward C++20 modules, but the existing header is still the
interop surface for cppyy and downstream executable-based workflows. Splitting
stable declaration groups first lets module partitions depend on smaller textual
interfaces while preserving the old include path.

This keeps behavior unchanged and makes the next shrink steps easier to review:
remaining river-system declarations can be moved in smaller groups once the
umbrella pattern has test coverage.
