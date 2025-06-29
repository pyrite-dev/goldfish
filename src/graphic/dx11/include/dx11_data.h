/**
 * @file dx11_data.h
 * @brief DX11のデータ定義ヘッダファイル
 * @author nekogakure
 */

#ifndef DX11_DATA_H
#define DX11_DATA_H

#include <stdbool.h>
#include <windows.h>

struct Vertex {
    float pos[3];
    float color[4];
};

struct MatrixBuffer {
    float world[16];
    float view[16];
    float projection[16];
};
typedef struct {
    float fov;
    float aspectRatio;
    int windowWidth;
    int windowHeight;
    bool debugMode;
} AppConfig;

typedef struct {
    const struct Vertex* vertices;
    const unsigned int* indices;
    unsigned int vertexCount;
    unsigned int indexCount;
    const char* name;
} MeshData;

typedef struct {
    AppConfig config;
    MeshData* meshes;
    unsigned int meshCount;
    unsigned int currentMeshIndex;
    HWND windowHandle;
    bool isRunning;
} AppState;

extern const struct Vertex vertices[];
extern const unsigned int indices[];

#endif