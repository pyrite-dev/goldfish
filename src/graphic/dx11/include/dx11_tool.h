/**
 * @file dx11_tool.h
 * @brief DX11関連の便利ツールたち（？）
 * @author nekogakure
 */

#ifndef DX11_TOOL_H
#define DX11_TOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <d3dcompiler.h>
#include <windows.h>

char* LoadShaderFile(const char* filename);
HRESULT CompileShaderFromFile(const char* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** ppBlobOut);
#endif