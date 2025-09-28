#include <cstdint>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <android/log.h>
#include <jni.h>

class android_logbuf final : public std::streambuf {
    static constexpr auto* TAG = "native_tests";
    static constexpr size_t BUF_SIZE = 512;
    char buf[BUF_SIZE];

public:
    android_logbuf() { setp(buf, buf + BUF_SIZE - 1); }
    ~android_logbuf() { flush_buffer(); }

protected:
    int_type overflow(int_type ch) override {
        if (ch == traits_type::eof()) return traits_type::not_eof(ch);
        *pptr() = char(ch);
        pbump(1);
        if (ch == '\n' || pptr() == epptr()) flush_buffer();
        return ch;
    }
    int sync() override {
        flush_buffer();
        return 0;
    }

private:
    void flush_buffer() {
        auto len = pptr() - pbase();
        if (len <= 0) return;
        // strip trailing newline so it’s not doubled
        size_t real_len = (buf[len - 1] == '\n') ? len - 1 : len;
        __android_log_write(ANDROID_LOG_INFO, TAG,
                            std::string(buf, real_len).c_str());
        setp(buf, buf + BUF_SIZE - 1);
    }
};

// redirect cout once
static android_logbuf g_logbuf;

static bool const redirect_cout = ([] {
    std::cout.rdbuf(&g_logbuf);
    return true;
})();

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_egleba_app_AppActivityTest_getTestNames(JNIEnv* env, jclass) {
    jclass strCls = env->FindClass("java/lang/String");
    jobjectArray arr = env->NewObjectArray(
            static_cast<int>(doctest::getRegisteredTests().size()), strCls, nullptr);

    int i = 0;
    for (auto& tc : doctest::getRegisteredTests()) {
        env->SetObjectArrayElement(arr, i, env->NewStringUTF(tc.m_name));
        i++;
    }
    return arr;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_egleba_app_AppActivityTest_runTest(JNIEnv* env, jclass,
                                              jstring jname) {
    const char* name = env->GetStringUTFChars(jname, nullptr);

    doctest::Context ctx;
    ctx.setOption("test-case", name);
    ctx.setOption("success", true);
    ctx.setOption("duration", true);
    ctx.setOption("reporters", "junit");

    const int res = ctx.run();
    env->ReleaseStringUTFChars(jname, name);
    return res == 0;
}

namespace {
constexpr std::int32_t factorial(std::int32_t n) noexcept {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

} // anonymous namespace

DOCTEST_TEST_CASE("Factorial function" * doctest::test_suite("math")) {
    // Use LT (lightweight) macros: no expression logging, no templates, faster compile
    DOCTEST_CHECK_EQ(factorial(0), 1);
    DOCTEST_CHECK_EQ(factorial(1), 1);
    DOCTEST_CHECK_EQ(factorial(2), 2);
    DOCTEST_CHECK_EQ(factorial(3), 6);
    DOCTEST_CHECK_EQ(factorial(4), 24);
    DOCTEST_CHECK_EQ(factorial(5), 120);
    DOCTEST_CHECK_EQ(factorial(10), 3'628'800);

    // Other useful LT macros:
    DOCTEST_CHECK_GT(factorial(5), factorial(4));   // 120 > 24
    DOCTEST_CHECK_LE(factorial(1), factorial(0));   // 1 <= 1
    DOCTEST_CHECK_NE(factorial(3), factorial(2));   // 6 != 2
}
