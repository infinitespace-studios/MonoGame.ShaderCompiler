#include "dxc_wrapper.h"
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#define __EMULATE_UUID 0
#endif

#ifndef _WIN32
#include "dxc/WinAdapter.h"
#endif
// Include the necessary DXC headers
#include "dxc/dxcapi.h"

DxcResult dxc_compile_shader(const char* source, const char* entry, const char* target) {
    DxcResult result = { NULL, 0, NULL, 0 };

    // Check if source is valid
    if (!source) {
        result.error_message = strdup("Source code is NULL.");
        return result;
    }

    // Initialize the DXC compiler and utils
    IDxcCompiler3* compiler = NULL;
    IDxcUtils* utils = NULL;
    IDxcIncludeHandler* includeHandler = NULL;

    // Create the compiler and utils
    if (FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler))) ||
        FAILED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)))) {
        result.error_message = strdup("Failed to create DXC compiler or utils.");
        return result;
    }

    // Create an include handler
    if (FAILED(utils->CreateDefaultIncludeHandler(&includeHandler))) {
        result.error_message = strdup("Failed to create include handler.");
        compiler->Release();
        utils->Release();
        return result;
    }

    // Compile the shader
    DxcBuffer pBuffer = {source, strlen(source), DXC_CP_UTF8};
    const wchar_t* args[] = { L"-E", (const wchar_t*)entry, L"-T", (const wchar_t*)target };
    CComPtr<IDxcResult> pCompileResult;
    if (FAILED(compiler->Compile(&pBuffer, args, _countof(args), nullptr, IID_PPV_ARGS(&pCompileResult)))) {
        result.error_message = strdup("Failed to compile shader.");
        includeHandler->Release();
        compiler->Release();
        utils->Release();
        return result;
    }

    // Check the result for errors
    HRESULT status;
    pCompileResult->GetStatus(&status);
    if (FAILED(status)) {
        IDxcBlobEncoding* errorsBlob = NULL;
        pCompileResult->GetErrorBuffer(&errorsBlob);
        result.error_message = _strdup((char*)errorsBlob->GetBufferPointer());
        pCompileResult->Release();
        includeHandler->Release();
        compiler->Release();
        utils->Release();
        return result;
    }

    // Retrieve compiled bytecode
    IDxcBlob* bytecodeBlob = NULL;
    pCompileResult->GetResult(&bytecodeBlob);

    // Copy the compiled bytecode into a result structure
    size_t bytecodeSize = bytecodeBlob->GetBufferSize();
    result.compiled_code = (char*)malloc(bytecodeSize);
    if (result.compiled_code) {
        memcpy(result.compiled_code, bytecodeBlob->GetBufferPointer(), bytecodeSize);
        result.success = 1;
    } else {
        result.error_message = strdup("Failed to allocate memory for bytecode.");
    }

    // Cleanup
    pCompileResult->Release();
    includeHandler->Release();
    compiler->Release();
    utils->Release();

    return result;
}

void dxc_free_result(DxcResult* result) {
    if (result->compiled_code) {
        free(result->compiled_code);
    }
    if (result->error_message) {
        free(result->error_message);
    }
}
