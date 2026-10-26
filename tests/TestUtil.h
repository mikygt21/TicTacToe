#pragma once
// Minimal dependency-free test harness. Swap for Catch2/GoogleTest later if desired.
#include <iostream>

namespace ttt_test {
inline int failures = 0;
inline int report()
{
    if (failures == 0) std::cout << "All tests passed\n";
    else std::cout << failures << " check(s) FAILED\n";
    return failures == 0 ? 0 : 1;
}
} // namespace ttt_test

// Variadic so that commas inside braces, e.g. Move{0, 2}, don't split the argument.
#define CHECK(...) do { if (!(__VA_ARGS__)) { ++ttt_test::failures; \
    std::cerr << __FILE__ << ":" << __LINE__ << "  FAILED: " #__VA_ARGS__ "\n"; } } while (0)
