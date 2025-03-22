#ifndef SPIRV_WRAPPER_H
#define SPIRV_WRAPPER_H

#ifdef _WIN32
    #define SPIRV_API __declspec(dllexport)
#else
    #define SPIRV_API __attribute__((visibility("default")))
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
} SpirvResult;

SPIRV_API SpirvResult spirv_to_glsl(const void* spirv_data, size_t size, unsigned int glslversion);
SPIRV_API void spirv_free_result(SpirvResult* result);

#ifdef __cplusplus
}
#endif

#endif // SPIRV_WRAPPER_H
