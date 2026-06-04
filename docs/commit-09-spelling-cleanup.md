# Commit 09: Spelling Cleanup

This change fixes spelling mistakes in comments and test documentation.

The cleanup intentionally avoids parser-visible keywords and legacy input spellings. That keeps existing topology, action, inflow, and global configuration files interoperable with the current in-memory model and file formats.

The test README was also refreshed to match the current CTest suite size after the build, interop, and module smoke tests were added.
