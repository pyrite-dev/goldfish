#include "include/dx11_resource.h"
#include <math.h>

ID3D11Buffer* g_pVertexBuffer = NULL;
ID3D11Buffer* g_pIndexBuffer = NULL;
ID3D11Buffer* g_pConstantBuffer = NULL;
ID3D11InputLayout* g_pInputLayout = NULL;
ID3D11VertexShader* g_pVertexShader = NULL;
ID3D11PixelShader* g_pPixelShader = NULL;

static AppState* g_appState = NULL;

// DEBUG
static float g_rotationX = 0.0f;
static float g_rotationY = 0.0f;
static const float ROTATION_SPEED = 0.0005f;

void PrintDebugInfo() {
    if (ISDEBUG) {
        char currentDir[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, currentDir);
        printf("Current working directory: %s\n", currentDir);
    }
}

HWND CreateAppWindow() {
    WNDCLASSEX wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WndProc,
        .hInstance = GetModuleHandle(NULL),
        .lpszClassName = "D3D11Window"
    };
    RegisterClassEx(&wc);

    return CreateWindow("D3D11Window", "DirectX11 Test",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, GetModuleHandle(NULL), NULL);
}

HRESULT CreateVertexShader(ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppInputLayout) {
    ID3DBlob* pVSBlob = NULL;
    HRESULT hr;
    
    hr = CompileShaderFromFile("../../shaders/VertexShader.hlsl", "main", "vs_4_0", &pVSBlob);
    if (FAILED(hr)) {
        MessageBoxA(NULL, "Vertex shader compilation failed", "Error", MB_OK);
        return hr;
    }

    hr = g_pd3dDevice->CreateVertexShader(
        pVSBlob->GetBufferPointer(), 
        pVSBlob->GetBufferSize(), NULL, ppVertexShader);
    if (FAILED(hr)) {
        pVSBlob->Release();
        MessageBoxA(NULL, "Could not create vertex shader", "Error", MB_OK);
        return hr;
    }
    
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), ppInputLayout);
    pVSBlob->Release();
    
    if (FAILED(hr)) {
        MessageBoxA(NULL, "Could not create input layout", "Error", MB_OK);
    }
    return hr;
}

HRESULT CreatePixelShader(ID3D11PixelShader** ppPixelShader) {
    ID3DBlob* pPSBlob = NULL;
    HRESULT hr;
    
    hr = CompileShaderFromFile("../../shaders/PixelShader.hlsl", "main", "ps_4_0", &pPSBlob);
    if (FAILED(hr)) {
        MessageBoxA(NULL, "Pixel shader compilation failed", "Error", MB_OK);
        return hr;
    }

    hr = g_pd3dDevice->CreatePixelShader(
        pPSBlob->GetBufferPointer(),
        pPSBlob->GetBufferSize(), NULL, ppPixelShader);
    pPSBlob->Release();
    
    if (FAILED(hr)) {
        MessageBoxA(NULL, "Could not create pixel shader", "Error", MB_OK);
    }
    return hr;
}

HRESULT CreateVertexBuffer(ID3D11Buffer** ppVertexBuffer, const struct Vertex* vertices, UINT vertexCount) {
    D3D11_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(struct Vertex) * vertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    
    D3D11_SUBRESOURCE_DATA initData = { 0 };
    initData.pSysMem = vertices;
    
    HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &initData, ppVertexBuffer);
    if (FAILED(hr)) {
        MessageBoxA(NULL, "Could not create vertex buffer", "Error", MB_OK);
    }
    return hr;
}

HRESULT CreateIndexBuffer(ID3D11Buffer** ppIndexBuffer, const unsigned int* indices, UINT indexCount) {
    D3D11_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned int) * indexCount;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    
    D3D11_SUBRESOURCE_DATA initData = { 0 };
    initData.pSysMem = indices;
    
    HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &initData, ppIndexBuffer);
    if (FAILED(hr)) {
        MessageBoxA(NULL, "Could not create index buffer", "Error", MB_OK);
    }
    return hr;
}

HRESULT CreateConstantBuffer(ID3D11Buffer** ppConstantBuffer) {
    D3D11_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(struct MatrixBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, NULL, ppConstantBuffer);
    if (FAILED(hr)) {
        MessageBoxA(NULL, "Could not create constant buffer", "Error", MB_OK);
    }
    return hr;
}

HRESULT SetupRenderStates() {
    HRESULT hr;
    
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    
    ID3D11RasterizerState* pRasterizerState = NULL;
    hr = g_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
    if (SUCCEEDED(hr)) {
        g_pImmediateContext->RSSetState(pRasterizerState);
        pRasterizerState->Release();
    } else {
        MessageBoxA(NULL, "Could not create rasterizer state", "Error", MB_OK);
        return hr;
    }
    
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { 0 };
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = FALSE;
    
    ID3D11DepthStencilState* pDepthStencilState = NULL;
    hr = g_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
    if (SUCCEEDED(hr)) {
        g_pImmediateContext->OMSetDepthStencilState(pDepthStencilState, 1);
        pDepthStencilState->Release();
    } else {
        MessageBoxA(NULL, "Could not create depth stencil state", "Error", MB_OK);
        return hr;
    }
    
    return S_OK;
}

void SetupPipeline(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, ID3D11InputLayout* pInputLayout, 
                  ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader, ID3D11Buffer* pConstantBuffer) {
    UINT stride = sizeof(struct Vertex);
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
    g_pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    g_pImmediateContext->IASetInputLayout(pInputLayout);
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_pImmediateContext->VSSetShader(pVertexShader, NULL, 0);
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
    g_pImmediateContext->PSSetShader(pPixelShader, NULL, 0);
}

void MultiplyMatrix(float result[16], const float a[16], const float b[16]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i * 4 + j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
            }
        }
    }
}

void CreateIdentityMatrix(float matrix[16]) {
    for (int i = 0; i < 16; i++) {
        matrix[i] = 0.0f;
    }
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

void CreateRotationY(float matrix[16], float angle) {
    CreateIdentityMatrix(matrix);
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    matrix[0] = cosA;   matrix[2] = sinA;
    matrix[8] = -sinA;  matrix[10] = cosA;
}

void CreateRotationX(float matrix[16], float angle) {
    CreateIdentityMatrix(matrix);
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    matrix[5] = cosA;   matrix[6] = -sinA;
    matrix[9] = sinA;   matrix[10] = cosA;
}

void CreateLookAtMatrix(float matrix[16], float eyeX, float eyeY, float eyeZ,
                       float atX, float atY, float atZ, float upX, float upY, float upZ) {

    float forwardX = atX - eyeX;
    float forwardY = atY - eyeY;
    float forwardZ = atZ - eyeZ;
    float forwardLen = sqrtf(forwardX*forwardX + forwardY*forwardY + forwardZ*forwardZ);
    forwardX /= forwardLen;
    forwardY /= forwardLen;
    forwardZ /= forwardLen;
    
    float rightX = forwardY*upZ - forwardZ*upY;
    float rightY = forwardZ*upX - forwardX*upZ;
    float rightZ = forwardX*upY - forwardY*upX;
    float rightLen = sqrtf(rightX*rightX + rightY*rightY + rightZ*rightZ);
    rightX /= rightLen;
    rightY /= rightLen;
    rightZ /= rightLen;
    
    float newUpX = rightY*forwardZ - rightZ*forwardY;
    float newUpY = rightZ*forwardX - rightX*forwardZ;
    float newUpZ = rightX*forwardY - rightY*forwardX;
    
    matrix[0] = rightX;   matrix[1] = newUpX;   matrix[2] = -forwardX;  matrix[3] = 0.0f;
    matrix[4] = rightY;   matrix[5] = newUpY;   matrix[6] = -forwardY;  matrix[7] = 0.0f;
    matrix[8] = rightZ;   matrix[9] = newUpZ;   matrix[10] = -forwardZ; matrix[11] = 0.0f;
    matrix[12] = -(rightX*eyeX + rightY*eyeY + rightZ*eyeZ);
    matrix[13] = -(newUpX*eyeX + newUpY*eyeY + newUpZ*eyeZ);
    matrix[14] = (forwardX*eyeX + forwardY*eyeY + forwardZ*eyeZ);
    matrix[15] = 1.0f;
}

void CreatePerspectiveMatrix(float matrix[16], float fov, float aspect, float nearZ, float farZ) {
    for (int i = 0; i < 16; i++) {
        matrix[i] = 0.0f;
    }
    
    float tanHalfFov = tanf(fov / 2.0f);
    matrix[0] = 1.0f / (aspect * tanHalfFov);
    matrix[5] = 1.0f / tanHalfFov;
    matrix[10] = -(farZ + nearZ) / (farZ - nearZ);
    matrix[11] = -1.0f;
    matrix[14] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
    matrix[15] = 0.0f;
}

void TransposeMatrix(float result[16], const float matrix[16]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[j * 4 + i] = matrix[i * 4 + j];
        }
    }
}

void UpdateRotationFromInput() {
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        g_rotationY -= ROTATION_SPEED;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        g_rotationY += ROTATION_SPEED;
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        g_rotationX -= ROTATION_SPEED;
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        g_rotationX += ROTATION_SPEED;
    }
}

void RenderScene(float fovDegrees, float aspect) {
    UpdateRotationFromInput();
    
    float clearColor[4] = { 0.0f, 0.1f, 0.3f, 1.0f };
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, clearColor);
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    
    struct MatrixBuffer matrices;
    float tempMatrix[16];
    float rotationY[16], rotationX[16];
    float worldMatrix[16], viewMatrix[16], projectionMatrix[16];
    
    CreateRotationY(rotationY, g_rotationY);
    CreateRotationX(rotationX, g_rotationX);
    MultiplyMatrix(worldMatrix, rotationX, rotationY);
    
    CreateLookAtMatrix(viewMatrix, 0.0f, 0.0f, -5.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);

    float fov = (fovDegrees * 3.14159f) / 180.0f;
    float nearZ = 0.1f;
    float farZ = 100.0f;
    CreatePerspectiveMatrix(projectionMatrix, fov, aspect, nearZ, farZ);
    
    TransposeMatrix(matrices.world, worldMatrix);
    TransposeMatrix(matrices.view, viewMatrix);
    TransposeMatrix(matrices.projection, projectionMatrix);
    
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = g_pImmediateContext->Map((ID3D11Resource*)g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr)) {
        memcpy(mappedResource.pData, &matrices, sizeof(struct MatrixBuffer));
        g_pImmediateContext->Unmap((ID3D11Resource*)g_pConstantBuffer, 0);
    }
    
    unsigned int indexCount = 36; //cube
    if (g_appState && g_appState->meshCount > 0) {
        indexCount = g_appState->meshes[g_appState->currentMeshIndex].indexCount;
    }
    
    g_pImmediateContext->DrawIndexed(indexCount, 0, 0);
    
    g_pSwapChain->Present(0, 0);
}

void CleanupGraphicsResources() {
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pIndexBuffer) g_pIndexBuffer->Release();
    if (g_pConstantBuffer) g_pConstantBuffer->Release();
    if (g_pInputLayout) g_pInputLayout->Release();
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pPixelShader) g_pPixelShader->Release();
    if (g_pDepthStencilView) g_pDepthStencilView->Release();
    if (g_pDepthStencil) g_pDepthStencil->Release();
    if (g_pImmediateContext) g_pImmediateContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
}

HRESULT InitializeAllResources(AppState* appState) {
    if (!appState) return E_INVALIDARG;
    
    g_appState = appState;
    
    appState->windowHandle = CreateAppWindow();
    if (!appState->windowHandle) {
        MessageBoxA(NULL, "Window creation failed", "Error", MB_OK);
        return E_FAIL;
    }
    
    D3DInitParams params;
    SetDefaultD3DInitParams(&params);
    params.width = appState->config.windowWidth;
    params.height = appState->config.windowHeight;
    
    HRESULT hr = InitD3D(appState->windowHandle, &params);
    if (FAILED(hr)) {
        MessageBoxA(NULL, "Direct3D initialization failed", "Error", MB_OK);
        return hr;
    }
    
    // shader
    hr = CreateVertexShader(&g_pVertexShader, &g_pInputLayout);
    if (FAILED(hr)) return hr;
    
    hr = CreatePixelShader(&g_pPixelShader);
    if (FAILED(hr)) return hr;
    
    // buffer
    hr = CreateConstantBuffer(&g_pConstantBuffer);
    if (FAILED(hr)) return hr;
    
    // render states
    hr = SetupRenderStates();
    if (FAILED(hr)) return hr;
    
    if (appState->meshCount > 0 && appState->meshes) {
        hr = CreateVertexBuffer(&g_pVertexBuffer, 
                               appState->meshes[appState->currentMeshIndex].vertices,
                               appState->meshes[appState->currentMeshIndex].vertexCount);
        if (FAILED(hr)) return hr;
        
        hr = CreateIndexBuffer(&g_pIndexBuffer,
                              appState->meshes[appState->currentMeshIndex].indices,
                              appState->meshes[appState->currentMeshIndex].indexCount);
        if (FAILED(hr)) return hr;
        
        SetupPipeline(g_pVertexBuffer, g_pIndexBuffer, g_pInputLayout, 
                     g_pVertexShader, g_pPixelShader, g_pConstantBuffer);
    }
    
    appState->isRunning = true;
    ShowWindow(appState->windowHandle, SW_SHOW);
    
    return S_OK;
}

MeshData* CreateMeshData(const struct Vertex* vertices, unsigned int vertexCount, 
                        const unsigned int* indices, unsigned int indexCount, const char* name) {
    MeshData* mesh = (MeshData*)malloc(sizeof(MeshData));
    if (!mesh) return NULL;
    
    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->vertexCount = vertexCount;
    mesh->indexCount = indexCount;
    mesh->name = name;
    
    return mesh;
}

void AddMeshToApp(AppState* appState, MeshData* mesh) {
    if (!appState || !mesh) return;
    
    appState->meshes = (MeshData*)realloc(appState->meshes, 
                                         sizeof(MeshData) * (appState->meshCount + 1));
    if (appState->meshes) {
        appState->meshes[appState->meshCount] = *mesh;
        appState->meshCount++;
        
        if (appState->config.debugMode) {
            printf("Added mesh '%s' (%u vertices, %u indices)\n", 
                   mesh->name, mesh->vertexCount, mesh->indexCount);
        }
    }
    
    free(mesh);
}

void SetCurrentMesh(AppState* appState, unsigned int meshIndex) {
    if (!appState || meshIndex >= appState->meshCount) return;
    
    appState->currentMeshIndex = meshIndex;
    
    if (g_pVertexBuffer) {
        g_pVertexBuffer->Release();
        g_pVertexBuffer = NULL;
    }
    if (g_pIndexBuffer) {
        g_pIndexBuffer->Release();
        g_pIndexBuffer = NULL;
    }
    
    const MeshData* currentMesh = &appState->meshes[meshIndex];
    
    CreateVertexBuffer(&g_pVertexBuffer, currentMesh->vertices, currentMesh->vertexCount);
    CreateIndexBuffer(&g_pIndexBuffer, currentMesh->indices, currentMesh->indexCount);
    
    SetupPipeline(g_pVertexBuffer, g_pIndexBuffer, g_pInputLayout, 
                 g_pVertexShader, g_pPixelShader, g_pConstantBuffer);
    
    if (appState->config.debugMode) {
        printf("Switched to mesh '%s'\n", currentMesh->name);
    }
}

void UpdateAppConfig(AppState* appState, float fov, float aspectRatio, int width, int height) {
    if (!appState) return;
    
    appState->config.fov = fov;
    appState->config.aspectRatio = aspectRatio;
    appState->config.windowWidth = width;
    appState->config.windowHeight = height;
    
    if (appState->config.debugMode) {
        printf("Config updated: FOV=%.1f, Aspect=%.3f, Size=%dx%d\n", 
               fov, aspectRatio, width, height);
    }
}

void RenderCurrentMesh(AppState* appState) {
    if (!appState || appState->meshCount == 0) return;
    
    const MeshData* currentMesh = &appState->meshes[appState->currentMeshIndex];
    
    RenderScene(appState->config.fov, appState->config.aspectRatio);
}

bool ProcessAppEvents(AppState* appState) {
    if (!appState) return false;
    
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            appState->isRunning = false;
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        appState->isRunning = false;
        return false;
    }
    
    static bool keyPressed[3] = {false, false, false};
    
    bool key1 = (GetAsyncKeyState('1') & 0x8000) != 0;
    if (key1 && !keyPressed[0] && appState->meshCount > 0) {
        SetCurrentMesh(appState, 0);
    }
    keyPressed[0] = key1;
    
    bool key2 = (GetAsyncKeyState('2') & 0x8000) != 0;
    if (key2 && !keyPressed[1] && appState->meshCount > 1) {
        SetCurrentMesh(appState, 1);
    }
    keyPressed[1] = key2;
    
    bool key3 = (GetAsyncKeyState('3') & 0x8000) != 0;
    if (key3 && !keyPressed[2] && appState->meshCount > 2) {
        SetCurrentMesh(appState, 2);
    }
    keyPressed[2] = key3;
    
    return true;
}

void CleanupAllResources() {
    CleanupGraphicsResources();
    
    if (g_appState && g_appState->meshes) {
        free(g_appState->meshes);
        g_appState->meshes = NULL;
        g_appState->meshCount = 0;
    }
    
    g_appState = NULL;
}
