#include "spirv_wrapper.h"
#include "spirv_cross_c.h"
#include <stdlib.h>
#include <string.h>

SpirvResult spirv_to_glsl(const void* spirv_data, size_t size, unsigned int glslversion) {
    SpirvResult result = { NULL, 0, NULL, 0 };

    if (!spirv_data || size == 0) {
        result.error_message = strdup("Invalid SPIR-V data.");
        return result;
    }

    // Create compiler
    spvc_context context;
    spvc_parsed_ir ir;
    spvc_compiler compiler;
    spvc_compiler_options options;
    const char* glsl;

    if (spvc_context_create(&context) != SPVC_SUCCESS) {
        result.error_message = strdup("Failed to create SPIRV-Cross context.");
        return result;
    }

    if (spvc_context_parse_spirv(context, spirv_data, size / sizeof(uint32_t), &ir) != SPVC_SUCCESS ||
        spvc_context_create_compiler(context, SPVC_BACKEND_GLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler) != SPVC_SUCCESS) {
        result.error_message = strdup("Failed to create SPIRV-Cross compiler.");
        spvc_context_destroy(context);
        return result;
    }

    // Set options
    spvc_compiler_create_compiler_options(compiler, &options);
    spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, glslversion);
    spvc_compiler_install_compiler_options(compiler, options);

    // Compile SPIR-V to GLSL
    if (spvc_compiler_compile(compiler, &glsl) != SPVC_SUCCESS) {
        result.error_message = strdup("Failed to compile SPIR-V to GLSL.");
        spvc_context_destroy(context);
        return result;
    }

    result.compiled_code = strdup(glsl);
    result.success = 1;

    // Clean up
    spvc_context_destroy(context);
    return result;
}

void spirv_free_result(SpirvResult* result) {
    if (result->compiled_code) free(result->compiled_code);
    if (result->error_message) free(result->error_message);
}
