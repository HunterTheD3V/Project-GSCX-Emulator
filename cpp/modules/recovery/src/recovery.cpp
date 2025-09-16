#include "module_api.h"
#include "logger.h"
#include <string>
#include "host_services_c.h"

using namespace gscx;

extern "C" __declspec(dllexport) ModuleInfo GSCX_GetModuleInfo() {
    return ModuleInfo{ .name = "recovery", .version_major = 0, .version_minor = 1 };
}

extern "C" void GSCX_RecoveryEntry();

static HostServicesC g_host{};

extern "C" __declspec(dllexport) bool GSCX_Initialize(void* host_ctx) {
    if (host_ctx) {
        g_host = *reinterpret_cast<HostServicesC*>(host_ctx);
        // Redirecionar Logger para callbacks do host
        Logger::set_info([](const char* m){ if (g_host.log_info) g_host.log_info(m); });
        Logger::set_warn([](const char* m){ if (g_host.log_warn) g_host.log_warn(m); });
        Logger::set_error([](const char* m){ if (g_host.log_error) g_host.log_error(m); });
    }
    Logger::info("recovery: inicializado (stub)");
    return true;
}

// Ponto principal do HLE de recovery: trata como BIOS + menu de recovery
extern "C" void GSCX_RecoveryMain() {
    if (g_host.log_info) g_host.log_info("[Recovery] HLE: inicialização básica, verificando NAND/flash...\n");
    // TODO: verificar integridade, inicializar tabelas, montar serviços
    if (g_host.log_info) g_host.log_info("[Recovery] HLE: exibindo menu (stub)\n");
}

extern "C" __declspec(dllexport) void GSCX_Shutdown() {
    Logger::info("recovery: finalizado (stub)");
}