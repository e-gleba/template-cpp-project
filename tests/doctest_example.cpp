#include <cstdint>

#include <doctest/doctest.h>

DOCTEST_TEST_SUITE("math playground")
{
    static constexpr std::int32_t factorial(std::int32_t n) noexcept
    {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    }

    DOCTEST_TEST_CASE("Factorial function" * doctest::test_suite("math"))
    {
        // Use LT (lightweight) macros: no expression logging, no templates,
        // faster compile
        DOCTEST_CHECK_EQ(factorial(0), 1);
        DOCTEST_CHECK_EQ(factorial(1), 1);
        DOCTEST_CHECK_EQ(factorial(2), 2);
        DOCTEST_CHECK_EQ(factorial(3), 6);
        DOCTEST_CHECK_EQ(factorial(4), 24);
        DOCTEST_CHECK_EQ(factorial(5), 120);
        DOCTEST_CHECK_EQ(factorial(10), 3'628'800);

        // Other useful LT macros:
        DOCTEST_CHECK_GT(factorial(5), factorial(4)); // 120 > 24
        DOCTEST_CHECK_LE(factorial(1), factorial(0)); // 1 <= 1
        DOCTEST_CHECK_NE(factorial(3), factorial(2)); // 6 != 2
    }
}
