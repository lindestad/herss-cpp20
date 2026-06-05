# Mainline parity

This commit incorporates the current upstream mainline fixes that fit the C++20
branch and adds output parity coverage for upstream reference cases.

## What changed

- Updated HERSS version constants to `3.1.02` / `20260605`.
- Moved reservoir curve/geometry selection from `GlobalConfig` to each
  `Reservoir`, allowing mixed topology files where one reservoir uses geometry
  and another uses curves.
- Mapped reservoir hatch actions by action-file column name, matching generator
  action mapping.
- Added hatch and generator action range checks before simulation uses actions.
- Avoided recomputing reservoir start volumes through curve interpolation during
  water-balance checks, which is invalid for geometry-only reservoirs.
- Imported upstream mainline data/reference-output fixtures for
  `mini_utahps_new_inputformat`, `res_casc_C`, and `res_casc_D`.
- Added a golden-output regression that runs the compiled executable against the
  upstream reference-output cases.
- Skipped the upstream `mini_utahps_spillway` input-only case because upstream
  does not include reference output and the `SPILLWAY` behavior is still
  incomplete.

## Why

The C++20 branch has diverged structurally from mainline, so a direct merge would
remove the modern build, module, RAII, and interop work. Porting the behavior
fixes manually keeps the branch modern while preserving executable output parity
with mainline data.
