#include "include/dx11_tool.h"
#include <stdio.h>
#include <stdlib.h>
#include <d3dcompiler.h>
#include <windows.h>

char* LoadShaderFile(const char* filename) {
    FILE* file = NULL;
    char* buffer = NULL;
    size_t fileSize, bytesRead;

    fopen_s(&file, filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open shader file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = (char*)malloc(fileSize + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    bytesRead = fread(buffer, 1, fileSize, file);
    buffer[bytesRead] = 0;

    fclose(file);
    return buffer;
}

HRESULT CompileShaderFromFile(const char* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** ppBlobOut) {
    HRESULT hr = S_OK;
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* pErrorBlob = NULL;
    char* shaderSource = NULL;
    
    shaderSource = LoadShaderFile(fileName);
    if (!shaderSource) {
        return E_FAIL;
    }
    
    hr = D3DCompile(shaderSource, strlen(shaderSource), fileName, NULL, NULL, entryPoint, 
                     shaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    
    if (FAILED(hr)) {
        if (pErrorBlob) {
            OutputDebugStringA((char*)pErrorBlob->lpVtbl->GetBufferPointer(pErrorBlob));
            pErrorBlob->lpVtbl->Release(pErrorBlob);
        }
        free(shaderSource);
        return hr;
    }
    
    if (pErrorBlob) pErrorBlob->lpVtbl->Release(pErrorBlob);
    free(shaderSource);
    
    return S_OK;
}