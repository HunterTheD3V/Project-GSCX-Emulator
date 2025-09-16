#include "module_api.h"
#include "logger.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <string>
#include <unordered_map>

namespace gscx {

struct LoadedModule {
#ifdef _WIN32
    HMODULE handle{nullptr};
#endif
    ModuleInfo info{};
    FnInitialize init{nullptr};
    FnShutdown  shutdown{nullptr};
};

class ModuleHost {
public:
    bool load(const std::string& path, const HostServices& host) {
#ifdef _WIN32
        HMODULE h = ::LoadLibraryA(path.c_str());
        if (!h) { Logger::error("Falha ao carregar DLL: " + path); return false; }
        auto getInfo = reinterpret_cast<FnGetModuleInfo>(::GetProcAddress(h, kFnGetModuleInfo));
        auto init    = reinterpret_cast<FnInitialize>(::GetProcAddress(h, kFnInitialize));
        auto shut    = reinterpret_cast<FnShutdown>(::GetProcAddress(h, kFnShutdown));
        if (!getInfo || !init || !shut) { Logger::error("Entrypoints ausentes em " + path); ::FreeLibrary(h); return false; }
        LoadedModule lm; lm.handle=h; lm.info=getInfo(); lm.init=init; lm.shutdown=shut;
        if (!lm.init((void*)&host)) { Logger::error("Initialize falhou em " + path); ::FreeLibrary(h); return false; }
        modules_[lm.info.name] = lm;
        Logger::info("Módulo carregado: " + lm.info.name);
        return true;
#else
        (void)path; (void)host; return false;
#endif
    }

    void unload_all() {
#ifdef _WIN32
        for (auto& [name, m] : modules_) {
            if (m.shutdown) m.shutdown();
            if (m.handle) ::FreeLibrary(m.handle);
            Logger::info("Módulo descarregado: " + name);
        }
        modules_.clear();
#endif
    }

private:
    std::unordered_map<std::string, LoadedModule> modules_;
};

} // namespace gscx