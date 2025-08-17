package com.egleba.app;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class AppActivityTest {

    private AppActivity app_activity;

    @Before
    public void setUp() throws Exception {
        app_activity = new AppActivity();
    }

    @After
    public void tearDown() throws Exception {
        app_activity = null;
    }

    @Test
    public void getLibraries() {
        String[] libraries = app_activity.getLibraries();

        assertNotNull(libraries);
        assertEquals(1, libraries.length);
        assertEquals("native_lib", libraries[0]);
    }

    @Test
    public void getLibraries_returns_immutable_reference() {
        String[] first_call = app_activity.getLibraries();
        String[] second_call = app_activity.getLibraries();

        assertArrayEquals(first_call, second_call);
    }

    @Test
    public void getLibraries_contains_expected_library() {
        String[] libraries = app_activity.getLibraries();

        boolean contains_native_lib = false;
        for (String lib : libraries) {
            if ("native_lib".equals(lib)) {
                contains_native_lib = true;
                break;
            }
        }

        assertTrue(contains_native_lib);
    }
}
