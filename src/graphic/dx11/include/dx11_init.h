/**
 * @file dx11_init.h
 * @brief DX11の初期化ヘッダファイル
 * @author nekogakure
 */

#ifndef DX11_INIT_H
#define DX11_INIT_H

#include <d3d11.h>
#include <dxgi.h>
#include <stddef.h>
#include <combaseapi.h>
#include <dxgi1_2.h>

extern IDXGISwapChain*           g_pSwapChain;
extern ID3D11Device*             g_pd3dDevice;
extern ID3D11DeviceContext*      g_pImmediateContext;
extern ID3D11RenderTargetView*   g_pRenderTargetView;
extern ID3D11Texture2D*          g_pDepthStencil;
extern ID3D11DepthStencilView*   g_pDepthStencilView;

typedef struct {
    UINT width;                 // ウィンドウ幅
    UINT height;                // ウィンドウ高さ
    UINT refreshRateNumerator;  // リフレッシュレート分子
    UINT refreshRateDenominator; // リフレッシュレート分母
    UINT sampleCount;           // サンプルカウント
    UINT sampleQuality;         // サンプルクオリティ
    BOOL windowed;              // ウィンドウモードフラグ
} D3DInitParams;

void SetDefaultD3DInitParams(D3DInitParams* params);
HRESULT InitD3D(HWND hWnd, D3DInitParams* params);
#endif