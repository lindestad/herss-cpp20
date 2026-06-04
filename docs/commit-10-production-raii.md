# Commit 10: Production RAII

This change removes the remaining manual ownership from production code.

`Channel` now owns its `CascadedReservoirs` through `std::unique_ptr` while keeping the existing public `casc_reservoirs` raw pointer as a non-owning compatibility view. Current callers can still inspect the same in-memory structure, but lifetime is now handled by the owning `Channel`.

The command-line entry point now uses automatic storage for `GlobalConfig`, `Dataset`, and `Herss`. Existing APIs still receive pointers where required, but cleanup no longer depends on matching `delete` calls in `main.cpp`.
