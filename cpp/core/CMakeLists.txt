add_library(gscx_core STATIC
    src/decoder.cpp
    src/translator.cpp
    src/jit_runtime.cpp
    src/module_host.cpp
    src/gscore_loader.cpp
)

target_include_directories(gscx_core PUBLIC include)

if (MSVC)
    target_compile_options(gscx_core PRIVATE /W4)
else()
    target_compile_options(gscx_core PRIVATE -Wall -Wextra -Wpedantic)
endif()