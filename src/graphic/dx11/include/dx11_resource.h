/**
 * @file dx11_resource.h
 * @brief DX11のリソース管理ヘッダファイル
 * @author nekogakure
 */
#ifndef DX11_RESOURCE_H
#define DX11_RESOURCE_H

#include "dx11_init.h"
#include "dx11_tool.h"
#include "dx11_data.h"
#include "window_mng.h"
#include <stdio.h>
#include <stdlib.h>
#include <d3dcompiler.h>
#include <windows.h>

#define ISDEBUG 1

extern ID3D11Buffer* g_pVertexBuffer;
extern ID3D11Buffer* g_pIndexBuffer;
extern ID3D11Buffer* g_pConstantBuffer;
extern ID3D11InputLayout* g_pInputLayout;
extern ID3D11VertexShader* g_pVertexShader;
extern ID3D11PixelShader* g_pPixelShader;

void PrintDebugInfo();
HWND CreateAppWindow();
HRESULT CreateVertexShader(ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppInputLayout);
HRESULT CreatePixelShader(ID3D11PixelShader** ppPixelShader);
HRESULT CreateVertexBuffer(ID3D11Buffer** ppVertexBuffer, const struct Vertex* vertices, UINT vertexCount);
HRESULT CreateIndexBuffer(ID3D11Buffer** ppIndexBuffer, const unsigned int* indices, UINT indexCount);
HRESULT CreateConstantBuffer(ID3D11Buffer** ppConstantBuffer);
HRESULT SetupRenderStates();
void SetupPipeline(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, ID3D11InputLayout* pInputLayout, 
                  ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader, ID3D11Buffer* pConstantBuffer);
void UpdateRotationFromInput();
void RenderScene(float fovDegrees, float aspect);
void CleanupGraphicsResources();
void RunMainLoop(HWND hWnd, float fovDegrees, float aspectRatio);

HRESULT InitializeAllResources(AppState* appState);
void CleanupAllResources();
void SetCurrentMesh(AppState* appState, unsigned int meshIndex);
MeshData* CreateMeshData(const struct Vertex* vertices, unsigned int vertexCount, 
                        const unsigned int* indices, unsigned int indexCount, const char* name);
void AddMeshToApp(AppState* appState, MeshData* mesh);
void UpdateAppConfig(AppState* appState, float fov, float aspectRatio, int width, int height);
void RenderCurrentMesh(AppState* appState);
bool ProcessAppEvents(AppState* appState);
#endif