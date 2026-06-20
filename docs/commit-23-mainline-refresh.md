# Mainline refresh

This commit ports the current upstream mainline refresh and keeps the C++20
rewrite aligned with the corrected behavior where mainline has clear edge-case
bugs.

## What changed

- Updated HERSS version constants to `3.1.03` / `20260611`.
- Fixed `ArrayCurve::x2y` indexing at the upper boundary so exact maximum
  discharge values do not select an out-of-range lookup bucket.
- Added support for `UNIFORM_NORMALIZED_CURVE` generator efficiency curves.
- Added `SPILLWAY` reservoir overflow support, including the small upstream
  spillway fixture as a golden-output case.
- Guarded powerstation simulation against actions requesting more discharge
  than the inflow made available by the upstream reservoir. The C++20 port caps
  to available water instead of zeroing all requested flow.
- Split aggressive-action penalties from adjustment costs and use the shared
  `HERSS_AGGRESSIVE_ACTIONS_COST` constant instead of an inline penalty.
- Parsed optional channel downstream ids through a signed temporary value before
  setting the downstream flag.
- Cleared reservoir action slots for reservoirs that do not use hatches.
- Refreshed upstream reference outputs for the golden cases.
- Updated README metadata while preserving the fork's experimental warning.

## Why

Upstream now has useful numerical and parser fixes mixed with generated-data
churn. This brings over the behavior needed by the rewritten ports while
avoiding two bugs noticed during the port: exact full-discharge uniform curves
must not read past the final point, and aggressive reservoir actions should
produce the available active water rather than throwing away the entire request.
