/**
 * @file gf_type/draw_driver.h
 * @~english
 * @brief Type definitions related to drawing driver
 */

#ifndef __GF_TYPE_DRAW_DRIVER_H__
#define __GF_TYPE_DRAW_DRIVER_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_DRAW_DRIVER
#define gf_draw_driver_t struct gf_draw_driver
#define gf_draw_driver_texture_t struct gf_draw_driver_texture

gf_draw_driver_t;
gf_draw_driver_texture_t;

/* External library */
#if defined(GF_DRV_OPENGL)
#include <gf_opengl.h>
#elif defined(GF_DRV_DX11)
#include <gf_dx11.h>
#endif

/* Engine */
#include <gf_type/core.h>

/* Standard */

#if defined(GF_DRV_OPENGL)
GF_DECLARE_TYPE(draw_driver, {
	gf_engine_t* engine;
	int	     npot;
	int	     max_texture_size;
});
GF_DECLARE_TYPE(draw_driver_texture, {
	gf_engine_t* engine;
	GLuint	     id;
	int	     width;
	int	     height;
});
#elif defined(GF_DRV_DX11)
GF_DECLARE_TYPE(draw_driver, {
	gf_engine_t*          engine;
	int                   npot;
	int                   max_texture_size;
	ID3D11Device*         device;
	ID3D11DeviceContext*  deviceContext;
	IDXGISwapChain*       swapChain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11RasterizerState*   rasterizerState;
	ID3D11BlendState*        blendState;
	ID3D11SamplerState*      samplerState;
	D3D11_VIEWPORT           viewport;
});
GF_DECLARE_TYPE(draw_driver_texture, {
	gf_engine_t*          engine;
	ID3D11Texture2D*      texture;
	ID3D11ShaderResourceView* resourceView;
	int                   width;
	int                   height;
});
#else
/**
 * @struct gf_draw_driver
 * @~english
 * @brief Drawing driver
 * @note Definition depends on the drawing driver, read the header file for definition
 */
GF_DECLARE_TYPE(draw_driver, {});

/**
 * @struct gf_draw_driver_texture
 * @~english
 * @brief Driver-dependent texture
 * @note Definition depends on the drawing driver, read the header file for definition
 */
GF_DECLARE_TYPE(draw_driver_texture, {});
#endif
#else
typedef void gf_draw_driver_t;
typedef void gf_draw_driver_texture_t;
#endif

#endif
