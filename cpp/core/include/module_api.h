#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace gscx {

struct ModuleInfo {
    std::string name;
    uint32_t    version_major{0};
    uint32_t    version_minor{1};
};

// Common interface each DLL must export to be discoverable.
using FnGetModuleInfo = ModuleInfo(*)();
using FnInitialize    = bool(*)(void* host_ctx);
using FnShutdown      = void(*)();

// Host services provided to modules
struct HostServices {
    std::function<void(const char*)> log_info;
    std::function<void(const char*)> log_warn;
    std::function<void(const char*)> log_error;
    // TODO: memory map, IO, DMA services, scheduler hooks
};

// Entry points names (to be exported by modules)
inline constexpr const char* kFnGetModuleInfo = "GSCX_GetModuleInfo";
inline constexpr const char* kFnInitialize    = "GSCX_Initialize";
inline constexpr const char* kFnShutdown      = "GSCX_Shutdown";

} // namespace gscx