cmake_minimum_required(VERSION 3.28)

cpmaddpackage(
    NAME
    doctest
    GITHUB_REPOSITORY
    doctest/doctest
    VERSION
    2.4.12
    SYSTEM
    YES
    OPTIONS
    "DOCTEST_WITH_TESTS OFF"
    "DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF"
    "DOCTEST_NO_INSTALL ON"
    "DOCTEST_USE_STD_HEADERS ON")

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
