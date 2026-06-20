# Mainline refresh

This commit ports the current upstream mainline fixes that are independent of
the spillway feature and keeps the C++20 rewrite aligned with the corrected
non-spillway behavior.

## What changed

- Updated HERSS version constants to `3.1.03` / `20260611`.
- Fixed `ArrayCurve::x2y` indexing at the upper boundary so exact maximum
  discharge values do not select an out-of-range lookup bucket.
- Added support for `UNIFORM_NORMALIZED_CURVE` generator efficiency curves.
- Guarded powerstation simulation against actions requesting more discharge
  than the inflow made available by the upstream reservoir.
- Split aggressive-action penalties from adjustment costs and use the shared
  `HERSS_AGGRESSIVE_ACTIONS_COST` constant instead of an inline penalty.
- Parsed optional channel downstream ids through a signed temporary value before
  setting the downstream flag.
- Cleared reservoir action slots for reservoirs that do not use hatches.
- Refreshed upstream reference outputs for the non-spillway golden cases.
- Updated README metadata while preserving the fork's experimental warning.

## Why

Upstream now has useful numerical and parser fixes mixed with spillway work and
generated-data churn. Porting the independent behavior keeps this C++20 branch
aligned with mainline output where we have reference data, while leaving the
spillway implementation for a dedicated follow-up.
