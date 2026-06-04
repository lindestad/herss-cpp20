# Commit 11: Test Fixture RAII

This change removes manual `new` and `delete` usage from the test suite.

Tests that need to exercise legacy raw-pointer fields still do so. The difference is that ownership now lives in stack objects or `std::unique_ptr`, while raw fields such as `Node::S` remain compatibility views into owned test objects.

This keeps the tests aligned with the production ownership direction. It also makes fixture cleanup deterministic and reduces the chance that a failing assertion skips a matching delete path.
