#include "module_api.h"
#include "logger.h"
#include <string>
+#include "host_services_c.h"
 
 using namespace gscx;
 
 extern "C" __declspec(dllexport) ModuleInfo GSCX_GetModuleInfo() {
     return ModuleInfo{ .name = "gpu_rsx", .version_major = 0, .version_minor = 1 };
 }
 
 extern "C" __declspec(dllexport) bool GSCX_Initialize(void* host_ctx) {
-    (void)host_ctx;
+    if (host_ctx) {
+        auto* hs = reinterpret_cast<HostServicesC*>(host_ctx);
+        Logger::set_info([hs](const char* m){ if (hs->log_info) hs->log_info(m); });
+        Logger::set_warn([hs](const char* m){ if (hs->log_warn) hs->log_warn(m); });
+        Logger::set_error([hs](const char* m){ if (hs->log_error) hs->log_error(m); });
+    }
     Logger::info("gpu_rsx: inicializado (stub)");
     return true;
 }
 
 extern "C" __declspec(dllexport) void GSCX_Shutdown() {
     Logger::info("gpu_rsx: finalizado (stub)");
 }