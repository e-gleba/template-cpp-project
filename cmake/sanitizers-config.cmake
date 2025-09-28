add_library(sanitizers INTERFACE)

# Only enable in Debug builds
target_compile_options(sanitizers INTERFACE
    $<$<CONFIG:Debug>:
        $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-fsanitize=address,undefined -fno-omit-frame-pointer -g>
        $<$<COMPILE_LANG_AND_ID:C,GNU,Clang,AppleClang>:-fsanitize=address,undefined -fno-omit-frame-pointer -g>
    >
)

# Linker flags (required for sanitizers to work!)
target_link_options(sanitizers INTERFACE
    $<$<CONFIG:Debug>:
        $<$<PLATFORM_ID:Linux,Darwin>:-fsanitize=address,undefined>
    >
)
