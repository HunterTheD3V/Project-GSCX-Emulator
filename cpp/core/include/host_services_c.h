#pragma once

#ifdef _WIN32
#define GSCX_CALL __stdcall
#else
#define GSCX_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (GSCX_CALL *gscx_log_fn)(const char*);

    typedef struct HostServicesC {
        gscx_log_fn log_info;
        gscx_log_fn log_warn;
        gscx_log_fn log_error;
    } HostServicesC;

#ifdef __cplusplus
}
#endif