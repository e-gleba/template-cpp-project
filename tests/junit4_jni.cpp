#include "main.h"

#include <android/log.h>
#include <jni.h>

#include <iostream>

#include <doctest/doctest.h>

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

static android_logbuf g_logbuf;

static bool const redirect_cout = ([] {
    std::cout.rdbuf(&g_logbuf);
    return true;
})();

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_egleba_app_AppActivityTest_getTestNames(JNIEnv* env, jclass) {
    jclass strCls = env->FindClass("java/lang/String");

    const std::set<std::string> tests = get_all_tests();
    jobjectArray arr = env->NewObjectArray(
        static_cast<int>(tests.size()), strCls, nullptr);

    int i = 0;
    for (auto& tc : tests) {
        env->SetObjectArrayElement(arr, i, env->NewStringUTF(tc.data()));
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
    ctx.setOption("no-exitcode", true); // todo: make a comment why critical
    ctx.setOption("duration", true);

    const int res = ctx.run();
    env->ReleaseStringUTFChars(jname, name);
    return res == 0;
}
