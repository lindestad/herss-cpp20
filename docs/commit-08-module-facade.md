# Commit 08: C++20 Module Facade

This change adds a C++20 module target named `herss_module` and a smoke test that imports it.

The module is intentionally a facade over the existing public header. That keeps the current in-memory structure and header-based integrations working while giving new code a module import path:

```cpp
import herss;
```

The CMake build now enables module scanning and compiles `src/herss.cppm` as a module interface. The legacy library, shared library, command-line binary, and tests continue to build from the same implementation sources.

This is the first module step, not a full internal partitioning of the codebase. A facade is a safer boundary because it proves toolchain support and gives downstream code a migration target without forcing a large ABI or ownership change at the same time.
