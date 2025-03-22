#ifndef DXC_WRAPPER_H
#define DXC_WRAPPER_H

#ifdef _WIN32
    #define DXC_API __declspec(dllexport)
#else
    #define DXC_API __attribute__((visibility("default")))
#endif

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char* compiled_code;
    size_t compiled_size;
    char* error_message;
    bool success;
} DxcResult;

DXC_API bool dxc_initialize();
DXC_API DxcResult dxc_compile_shader(const char* source, const char* entry, const char* target);
DXC_API void dxc_free_result(DxcResult* result);
DXC_API void dxc_shutdown();

#ifdef __cplusplus
}
#endif

#endif // DXC_WRAPPER_H
