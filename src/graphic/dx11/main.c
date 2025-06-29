#include "include/dx11_resource.h"
#include "include/dx11_data.h"
#include <stdio.h>
#include <d3dcompiler.h>
#include <windows.h>

#define ISDEBUG 1

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

const struct Vertex cubeVertices[] = {
    // 前面
    { { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 左下前
    { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 左上前
    { {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 右上前
    { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 右下前
    
    // 背面
    { { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } }, // 左下後
    { { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, // 左上後
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 右上後
    { {  0.5f, -0.5f,  0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }  // 右下後
};

const unsigned int cubeIndices[] = {
    // 前面
    0, 1, 2,  0, 2, 3,
    // 背面
    4, 6, 5,  4, 7, 6,
    // 左面
    4, 5, 1,  4, 1, 0,
    // 右面
    3, 2, 6,  3, 6, 7,
    // 上面
    1, 5, 6,  1, 6, 2,
    // 下面
    4, 0, 3,  4, 3, 7
};

const struct Vertex triangleVertices[] = {
    { {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 上
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 左下
    { {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }  // 右下
};

const unsigned int triangleIndices[] = {
    0, 1, 2
};
/*
const struct Vertex vertices[] = {
    // 前面
    { { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 左下前
    { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 左上前
    { {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 右上前
    { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 右下前
    
    // 背面
    { { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } }, // 左下後
    { { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, // 左上後
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 右上後
    { {  0.5f, -0.5f,  0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }  // 右下後
};

const unsigned int indices[] = {
    // 前面
    0, 1, 2,  0, 2, 3,
    // 背面
    4, 6, 5,  4, 7, 6,
    // 左面
    4, 5, 1,  4, 1, 0,
    // 右面
    3, 2, 6,  3, 6, 7,
    // 上面
    1, 5, 6,  1, 6, 2,
    // 下面
    4, 0, 3,  4, 3, 7
};
*/

int main() {
    PrintDebugInfo();
    
    AppState appState = {0};
    
    appState.config.fov = 90.0f;
    appState.config.aspectRatio = 800.0f / 600.0f;
    appState.config.windowWidth = 800;
    appState.config.windowHeight = 600;
    appState.config.debugMode = ISDEBUG;
    
    // add meshes
    MeshData* cubeMesh = CreateMeshData(cubeVertices, 8, cubeIndices, 36, "Cube");
    AddMeshToApp(&appState, cubeMesh);
    
    MeshData* triangleMesh = CreateMeshData(triangleVertices, 3, triangleIndices, 3, "Triangle");
    AddMeshToApp(&appState, triangleMesh);
    
    // init
    HRESULT hr = InitializeAllResources(&appState);
    if (FAILED(hr)) {
        printf("Failed to initialize application resources\n");
        return 1;
    }
    
    printf("Application initialized successfully!\n");
    printf("Controls:\n");
    printf("  Arrow keys: Rotate object\n");
    printf("  1-2 keys: Switch between meshes\n");
    printf("  ESC: Exit\n");
    
    // main
    while (appState.isRunning) {
        if (!ProcessAppEvents(&appState)) {
            break;
        }
        
        RenderCurrentMesh(&appState);
    }
    
    CleanupAllResources();
    
    return 0;
}

