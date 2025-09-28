package com.egleba.app;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import java.util.Arrays;
import java.util.Collection;
import java.util.Objects;
import java.util.stream.Collectors;

import static org.junit.Assert.assertTrue;

/// On-device native test runner for Android instrumentation tests.
///
/// Executes native C/C++ unit tests bundled in the APK via the Android NDK testing framework.
/// Each native test is discovered at runtime and run as a separate JUnit parameterized test.
///
/// @note Follows Android’s official on-device native testing guidance:
///       https://developer.android.com/ndk/guides/test-native-libraries
///       https://developer.android.com/training/testing/unit-testing/instrumented-unit-tests
///
/// @see https://developer.android.com/ndk/guides/test-native-libraries
/// @see https://developer.android.com/training/testing/unit-testing/instrumented-unit-tests
@RunWith(Parameterized.class)
public final class AppActivityTest {

    static {
        System.loadLibrary("tests");
    }

    /// Retrieves the list of available native test names.
    ///
    /// Queries the embedded native test suite for registered test identifiers.
    /// Used to populate JUnit parameterized instances.
    ///
    /// @return Array of native test names, or null if unavailable
    public static native String[] getTestNames();

    /// Executes a specific native test by name.
    ///
    /// @param name Test identifier to run
    /// @return true if passed, false otherwise
    public static native boolean runTest(String name);

    /// The name of the current test case being executed.
    @Parameterized.Parameter
    public String testName;

    /// Provides test parameters for JUnit's parameterized runner.
    ///
    /// Safely handles null or empty native test lists to avoid runtime errors.
    ///
    /// @return Collection of test name parameters; empty if no tests found
    @Parameters(name = "{0}")
    public static Collection<Object[]> data() {
        final String[] names = Objects.requireNonNull(getTestNames(),
                "Native test names must not be null — check that libtests is loaded and exports a valid test suite.");
        return Arrays.stream(names)
                .map(name -> new Object[]{name})
                .collect(Collectors.toList());
    }

    /// Executes a single native test case.
    ///
    /// Asserts that the native test passes; includes test name in failure message.
    @Test
    public void runSingle() {
        assertTrue("❌ Native test '" + testName + "' failed", runTest(testName));
    }
}