package com.egleba.app;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import java.util.Collection;
import java.util.ArrayList;

import static org.junit.Assert.assertTrue;

@RunWith(Parameterized.class)
public class AppActivityTest {

    static {
        System.loadLibrary("tests");
    }

    public static native String[] getTestNames();
    public static native boolean runTest(String name);

    @Parameterized.Parameter
    public String testName;

    @Parameters(name = "{0}")
    public static Collection<Object[]> data() {
        try {
            String[] testNames = getTestNames();

            Collection<Object[]> result = new ArrayList<>();
            for (String name : testNames) {
                result.add(new Object[]{name});
            }
            return result;
        } catch (Exception e) {
            Collection<Object[]> fallback = new ArrayList<>();
            fallback.add(new Object[]{"mock_test_error"});
            return fallback;
        }
    }

    @Test
    public void runSingle() {
        assertTrue("❌ " + testName + " failed", runTest(testName));
    }
}
