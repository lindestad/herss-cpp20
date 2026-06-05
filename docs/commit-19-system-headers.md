# Node and system headers

This commit finishes shrinking the legacy umbrella header.

## What changed

- Added `herss_node.h` for the base `Node` declaration.
- Added `herss_reservoir.h`, `herss_powerstation.h`, and `herss_channel.h`
  for concrete node types.
- Added `herss_system.h` for `Riversystem` and `Herss`.
- Reduced `herss.h` to a compatibility umbrella that includes `herss_system.h`
  and `logger.h`.
- Pointed the riversystem module partition at `herss_system.h` instead of the
  full compatibility umbrella.

## Why

The remaining declarations in `herss.h` were strongly layered: nodes depend on
qmin and scenarios, concrete nodes depend on the base node, and the full system
depends on the concrete node types. Moving them into that shape makes textual
includes match the real dependency graph.

This gives module partitions and future C++ clients narrower entry points while
leaving cppyy and legacy downstream code free to keep including `herss.h`.
