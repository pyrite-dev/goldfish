#include "include/dx11_init.h"

IDXGISwapChain*           g_pSwapChain = NULL;
ID3D11Device*             g_pd3dDevice = NULL;
ID3D11DeviceContext*      g_pImmediateContext = NULL;
ID3D11RenderTargetView*   g_pRenderTargetView = NULL;
ID3D11Texture2D*          g_pDepthStencil = NULL;
ID3D11DepthStencilView*   g_pDepthStencilView = NULL;

void SetDefaultD3DInitParams(D3DInitParams* params)
{
    if (params) {
        params->width = 800;
        params->height = 600;
        params->refreshRateNumerator = 60;
        params->refreshRateDenominator = 1;
        params->sampleCount = 1;
        params->sampleQuality = 0;
        params->windowed = TRUE;
    }
}

HRESULT InitD3D(HWND hWnd, D3DInitParams* params)
{
    HRESULT hr;
    UINT width;
    UINT height;
    D3DInitParams defaultParams;
    
    if (params == NULL) {
        SetDefaultD3DInitParams(&defaultParams);
        params = &defaultParams;
    }
    
    if (params->width == 0 || params->height == 0) {
        RECT rc;
        GetClientRect(hWnd, &rc);
        width = rc.right - rc.left;
        height = rc.bottom - rc.top;
    } else {
        width = params->width;
        height = params->height;
    }

    DXGI_SWAP_CHAIN_DESC sd = { 0 };
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = params->refreshRateNumerator;
    sd.BufferDesc.RefreshRate.Denominator = params->refreshRateDenominator;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = params->sampleCount;
    sd.SampleDesc.Quality = params->sampleQuality;
    sd.Windowed = params->windowed;
    
    D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL FeatureLevel;

    hr = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, FeatureLevels, 1,
        D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice,
        &FeatureLevel, &g_pImmediateContext);
    if (FAILED(hr)) return hr;

    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return hr;

    hr = g_pd3dDevice->CreateRenderTargetView((ID3D11Resource*)pBackBuffer, NULL, &g_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) return hr;

    D3D11_TEXTURE2D_DESC descDepth = { 0 };
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
    if (FAILED(hr)) return hr;

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView((ID3D11Resource*)g_pDepthStencil, &descDSV, &g_pDepthStencilView);
    if (FAILED(hr)) return hr;

    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

    D3D11_VIEWPORT vp;
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, &vp);

    return S_OK;
}