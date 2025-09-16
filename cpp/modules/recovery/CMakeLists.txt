# Enable MASM for x64 assembly
enable_language(ASM_MASM)

add_library(gscx_recovery SHARED 
    src/recovery.cpp
    src/recovery_entry.asm
)

target_include_directories(gscx_recovery PUBLIC ../../core/include)

target_link_libraries(gscx_recovery PRIVATE gscx_core)

# Export the ASM entry symbol from the DLL on MSVC
if (MSVC)
    target_link_options(gscx_recovery PRIVATE "/EXPORT:GSCX_RecoveryEntry")
endif()

set_target_properties(gscx_recovery PROPERTIES OUTPUT_NAME "gscx_recovery")