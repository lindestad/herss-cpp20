# Leaf implementation includes

This commit starts replacing implementation includes of `herss.h` with focused
headers.

## What changed

- `line.cpp`, `xtime.cpp`, `globalconfig.cpp`, and `topoparser.cpp` now include
  `herss_config.h`.
- `dataset.cpp` now includes `herss_dataset.h`.
- `scenario.cpp` now includes `herss_scenario.h`.
- `qmin.cpp` now includes `herss_qmin.h` and `herss_config.h`.
- `cascadedreservoirs.cpp` now includes `herss_routing.h`.
- Files that use logging macros include `logger.h` directly.

## Why

These implementations sit near the bottom of the dependency graph. Moving them
off the compatibility umbrella proves the split headers are usable for real
compilation units, not just for module partition declarations.

The public `herss.h` compatibility path is unchanged.
