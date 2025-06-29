#define GF_EXPOSE_DRAW_DRIVER
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_TEXTURE
#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#include <gf_dx11.h>

/* Interface */
#include <gf_draw_driver.h>

/* Engine */
#include <gf_draw_platform.h>
#include <gf_texture.h>
#include <gf_draw.h>
#include <gf_log.h>
#include <gf_math.h>
#include <gf_graphic.h>
#include <gf_prop.h>

/* DX11 specific includes */
#include "include/dx11_init.h"
#include "include/dx11_tool.h"
#include "include/dx11_resource.h"

/* Standard */
#include <stdlib.h>
#include <string.h>

gf_draw_driver_texture_t* gf_draw_driver_register_texture(gf_draw_t* draw, int width, int height, int* iwidth, int* iheight, unsigned char* data) {
	gf_draw_driver_texture_t* r = (gf_draw_driver_texture_t*)malloc(sizeof(*r));
	int w = width;
	int h = height;

	if(w > draw->driver->max_texture_size) w = draw->driver->max_texture_size;
	if(h > draw->driver->max_texture_size) h = draw->driver->max_texture_size;

	*iwidth = w;
	*iheight = h;

	r->engine = draw->engine;
	r->width = w;
	r->height = h;

	// Create DX11 texture
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = w;
	desc.Height = h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = data;
	initData.SysMemPitch = w * 4;

	HRESULT hr = draw->driver->device->CreateTexture2D(
		&desc, &initData, &r->texture);

	if(FAILED(hr)) {
		gf_log(draw->engine, "Failed to create DX11 texture");
		free(r);
		return NULL;
	}

	// Create shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = draw->driver->device->CreateShaderResourceView(
		(ID3D11Resource*)r->texture, &srvDesc, &r->resourceView);

	if(FAILED(hr)) {
		r->texture->Release();
		free(r);
		return NULL;
	}

	return r;
}

void gf_draw_driver_free_texture(gf_draw_driver_texture_t* driver_tex) {
	if(driver_tex) {
		if(driver_tex->resourceView) {
			driver_tex->resourceView->Release();
		}
		if(driver_tex->texture) {
			driver_tex->texture->Release();
		}
		free(driver_tex);
	}
}

void gf_draw_driver_viewport(gf_draw_t* draw, int x, int y, int width, int height) {
	D3D11_VIEWPORT vp;
	vp.TopLeftX = (float)x;
	vp.TopLeftY = (float)y;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	draw->driver->deviceContext->RSSetViewports(1, &vp);
}

void gf_draw_driver_clear(gf_draw_t* draw, gf_graphic_color_t color) {
	float clearColor[4] = {
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 255.0f
	};

	draw->driver->deviceContext->ClearRenderTargetView(
		draw->driver->renderTargetView, clearColor);

	draw->driver->deviceContext->ClearDepthStencilView(
		draw->driver->depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void gf_draw_driver_swap_buffers(gf_draw_t* draw) {
	draw->driver->swapChain->Present(1, 0);
}

void gf_draw_driver_enable_depth(gf_draw_t* draw) {
	// Set depth stencil state
	draw->driver->deviceContext->OMSetDepthStencilState(
		draw->driver->depthStencilState, 1);
}

void gf_draw_driver_disable_depth(gf_draw_t* draw) {
	// Set depth stencil state to disable depth testing
	draw->driver->deviceContext->OMSetDepthStencilState(NULL, 1);
}

void gf_draw_driver_enable_blend(gf_draw_t* draw) {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;

	draw->driver->deviceContext->OMSetBlendState(
		draw->driver->blendState, blendFactor, sampleMask);
}

void gf_draw_driver_disable_blend(gf_draw_t* draw) {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;

	draw->driver->deviceContext->OMSetBlendState(NULL, blendFactor, sampleMask);
}

int gf_draw_driver_has_extension(gf_draw_t* draw, const char* query) {
	// DX11 doesn't use extensions in the same way OpenGL does
	// Return appropriate values for common queries
	if(strcmp(query, "GL_ARB_texture_non_power_of_two") == 0) {
		return 1; // DX11 supports NPOT textures by default
	}
	return 0;
}

gf_draw_driver_t* gf_draw_driver_create(gf_engine_t* engine, gf_draw_t* draw) {
	gf_draw_driver_t* driver = (gf_draw_driver_t*)malloc(sizeof(*driver));
	memset(driver, 0, sizeof(*driver));

	driver->engine = engine;
	driver->npot = 1; // DX11 supports NPOT textures
	driver->max_texture_size = 8192; // Default max texture size for DX11

	// Initialize DX11 context
	HWND hWnd = (HWND)gf_draw_platform_get_window_handle(draw);
	if(!hWnd) {
		gf_log(engine, "Failed to get window handle for DX11 initialization");
		free(driver);
		return NULL;
	}

	D3DInitParams params;
	SetDefaultD3DInitParams(&params);
	params.width = draw->width;
	params.height = draw->height;

	HRESULT hr = InitD3D(hWnd, &params);
	if(FAILED(hr)) {
		gf_log(engine, "Failed to initialize DX11");
		free(driver);
		return NULL;
	}

	// Store DX11 objects in driver
	driver->device = g_pd3dDevice;
	driver->deviceContext = g_pImmediateContext;
	driver->swapChain = g_pSwapChain;
	driver->renderTargetView = g_pRenderTargetView;
	driver->depthStencilView = g_pDepthStencilView;

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = TRUE;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	hr = driver->device->CreateDepthStencilState(&dsDesc, &driver->depthStencilState);

	// Create blend state
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = driver->device->CreateBlendState(&blendDesc, &driver->blendState);

	// Create sampler state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = driver->device->CreateSamplerState(&sampDesc, &driver->samplerState);

	// Set initial viewport
	driver->viewport.TopLeftX = 0;
	driver->viewport.TopLeftY = 0;
	driver->viewport.Width = (float)draw->width;
	driver->viewport.Height = (float)draw->height;
	driver->viewport.MinDepth = 0.0f;
	driver->viewport.MaxDepth = 1.0f;

	driver->deviceContext->RSSetViewports(1, &driver->viewport);

	gf_log(engine, "DX11 driver initialized successfully");
	return driver;
}

void gf_draw_driver_destroy(gf_draw_driver_t* driver) {
	if(driver) {
		if(driver->samplerState) {
			driver->samplerState->Release();
		}
		if(driver->blendState) {
			driver->blendState->Release();
		}
		if(driver->depthStencilState) {
			driver->depthStencilState->Release();
		}
		
		// Global DX11 objects are cleaned up elsewhere
		free(driver);
	}
}

void gf_draw_driver_reshape(gf_draw_t* draw) {
	if(!draw->driver) return;

	// Update viewport
	draw->driver->viewport.Width = (float)draw->width;
	draw->driver->viewport.Height = (float)draw->height;

	draw->driver->deviceContext->RSSetViewports(1, &draw->driver->viewport);

	// Resize swap chain buffers
	if(draw->driver->swapChain) {
		draw->driver->swapChain->ResizeBuffers(
			0, draw->width, draw->height, DXGI_FORMAT_UNKNOWN, 0);
	}
}
