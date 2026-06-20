# Defer spillway fixture

This commit keeps the mainline refresh scoped to behavior this fork currently
supports and tests.

## What changed

- Removed the imported `mini_utahps_spillway` fixture from tracked data.
- Removed that fixture from the golden-output regression list.
- Corrected the mainline refresh note to describe spillway work as deferred.
- Restored upstream-compatible hard shutdown for aggressive powerstation
  tunnel actions and updated the unit expectation.

## Why

The upstream spillway case is useful, but this branch is intentionally waiting
to bring in the full spillway reference implementation. Keeping unsupported
fixture data in-tree would imply parity we cannot honestly claim yet.
