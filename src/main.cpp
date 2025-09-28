#include <print>

namespace color
{
static constexpr auto reset = "\033[0m";
static constexpr auto red   = "\033[38;5;196m";
static constexpr auto cyan  = "\033[38;5;51m";
static constexpr auto gold  = "\033[38;5;220m";
static constexpr auto green = "\033[38;5;46m";
} // namespace color

/// @brief Main entry point of the program
/// @return EXIT_SUCCESS on successful execution, EXIT_FAILURE on error
/// @details Demonstrates the usage of std::print with ANSI color formatting
/// @exception std::exception Catches and handles any standard library
/// exceptions
///
/// Example output:
/// @code
/// ⬤ C++ 2023.23.0 ISO/IEC P2693R5 (gcc-13.2.1)
///
/// ⬤ Hello, World!
/// @endcode
///
/// @note Requires C++23 compiler support for std::print
/// @warning Terminal must support ANSI escape sequences for color output
/// @see https://en.cppreference.com/w/cpp/io/print
int main()
{
    try
    {
        static constexpr auto cpp_ver = 23;
        static constexpr auto spec    = "P2693R5";

        std::print("{}⬤ {}C++{} {}.{}.{} {}ISO/IEC {} {}\n",
                   color::red,
                   color::cyan,
                   color::reset,
                   2023,
                   cpp_ver,
                   0,
                   color::gold,
                   spec,
                   color::green);

        std::print("\n{}⬤ {}Hello, {}World!{}\n",
                   color::green,
                   color::gold,
                   color::cyan,
                   color::reset);

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::print(stderr,
                   "{}* {}Error: {}{}\n",
                   color::red,
                   color::reset,
                   e.what(),
                   color::reset);
        return EXIT_FAILURE;
    }
}