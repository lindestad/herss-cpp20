# Commit 12: Warning Cleanup

This change removes build warnings from the CMake targets.

The fixes are intentionally behavioral no-ops: unused parameters in placeholder or compatibility functions are unnamed or explicitly marked, and impossible lower-bound checks on unsigned values are removed.

Keeping the build warning-clean makes the new CMake test path easier to trust. Future warnings should now stand out as real review items instead of being hidden in existing noise.
