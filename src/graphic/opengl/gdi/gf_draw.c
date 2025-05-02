#define GF_EXPOSE_DRAW_PLATFORM
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <gf_opengl.h>

/* Interface */
#include <gf_draw_platform.h>

/* Engine */
#include <gf_draw_driver.h>
#include <gf_log.h>
#include <gf_draw.h>
#include <gf_input.h>

/* Standard */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef const char*(APIENTRY* PFNWGLGETEXTENSIONSSTRINGARB)(HDC);
#ifdef DO_SWAP_INTERVAL
typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALPROC)(int);
#endif

void gf_draw_platform_begin(void) {}
void gf_draw_platform_end(void) {}

LRESULT CALLBACK gf_draw_platform_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	PAINTSTRUCT ps;
	RECT	    rect;
	gf_draw_t*  draw = (gf_draw_t*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch(msg) {
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		if(draw->platform != NULL) {
			GetClientRect(hWnd, &rect);
			draw->x	     = rect.left;
			draw->y	     = rect.top;
			draw->width  = rect.right - rect.left;
			draw->height = rect.bottom - rect.top;
#if defined(TYPE_NATIVE)
			wglMakeCurrent(draw->platform->dc, draw->platform->glrc);
#elif defined(TYPE_OSMESA)
			DeleteObject(draw->platform->bitmap);
			DeleteDC(draw->platform->bitmapdc);
			/**
			 * Do NOT:
			 * free(draw->platform->buffer);
			 */
			draw->platform->buffer		 = malloc(draw->width * draw->height * 4);
			draw->platform->header.bV5Width	 = draw->width;
			draw->platform->header.bV5Height = -((LONG)draw->height);
			draw->platform->bitmap		 = CreateDIBSection(draw->platform->dc, (BITMAPINFO*)&draw->platform->header, DIB_RGB_COLORS, (void**)&draw->platform->buffer, NULL, (DWORD)0);
			draw->platform->bitmapdc	 = CreateCompatibleDC(draw->platform->dc);
			SelectObject(draw->platform->bitmapdc, draw->platform->bitmap);
			OSMesaMakeCurrent(draw->platform->context, draw->platform->buffer, GL_UNSIGNED_BYTE, draw->width, draw->height);
#endif
			gf_draw_reshape(draw);
		}
		break;
	case WM_MOUSEMOVE:
		if(draw->input != NULL) {
			POINTS p	     = MAKEPOINTS(lp);
			draw->input->mouse_x = p.x;
			draw->input->mouse_y = p.y;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		if(draw->input != NULL) {
			if(msg == WM_LBUTTONDOWN) {
				SetCapture(hWnd);
				draw->input->mouse_flag |= GF_INPUT_MOUSE_LEFT_MASK;
			} else {
				draw->input->mouse_flag ^= GF_INPUT_MOUSE_LEFT_MASK;
				ReleaseCapture();
			}
		}
		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		if(draw->input != NULL) {
			if(msg == WM_MBUTTONDOWN) {
				SetCapture(hWnd);
				draw->input->mouse_flag |= GF_INPUT_MOUSE_MIDDLE_MASK;
			} else {
				draw->input->mouse_flag ^= GF_INPUT_MOUSE_MIDDLE_MASK;
				ReleaseCapture();
			}
		}
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		if(draw->input != NULL) {
			if(msg == WM_RBUTTONDOWN) {
				SetCapture(hWnd);
				draw->input->mouse_flag |= GF_INPUT_MOUSE_RIGHT_MASK;
			} else {
				draw->input->mouse_flag ^= GF_INPUT_MOUSE_RIGHT_MASK;
				ReleaseCapture();
			}
		}
		break;
	case WM_CLOSE:
		draw->close = 1;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query) {
#if defined(TYPE_NATIVE)
	const char*		     ext = NULL;
	const char*		     ptr;
	PFNWGLGETEXTENSIONSSTRINGARB proc;
#endif
	const int len = strlen(query);
#if defined(TYPE_NATIVE)
	wglMakeCurrent(draw->platform->dc, draw->platform->glrc);

	proc = (PFNWGLGETEXTENSIONSSTRINGARB)wglGetProcAddress("wglGetExtensionsStringARB");

	if(proc != NULL) {
		ext = proc(draw->platform->dc);
		ptr = strstr(ext, query);
		return ((ptr != NULL) && ((ptr[len] == ' ') || (ptr[len] == '\0')));
	}
#elif defined(TYPE_OSMESA)
#endif
	return 0;
}

int gf_draw_platform_step(gf_draw_t* draw) {
	MSG msg;
	int ret = 0;
#if defined(TYPE_NATIVE)
	wglMakeCurrent(draw->platform->dc, draw->platform->glrc);
#elif defined(TYPE_OSMESA)
	OSMesaMakeCurrent(draw->platform->context, draw->platform->buffer, GL_UNSIGNED_BYTE, draw->width, draw->height);
#endif
	while(PeekMessage(&msg, draw->platform->window, 0, 0, PM_NOREMOVE)) {
		if(GetMessage(&msg, draw->platform->window, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			ret = 1;
			break;
		}
	}
	if(ret == 0) {
		gf_draw_driver_before(draw);
		gf_draw_frame(draw);
		gf_draw_driver_after(draw);

#if defined(TYPE_NATIVE)
		SwapBuffers(draw->platform->dc);
#elif defined(TYPE_OSMESA)
		BitBlt(draw->platform->dc, 0, 0, draw->width, draw->height, draw->platform->bitmapdc, 0, 0, SRCCOPY);
#endif
	}
	return ret;
}

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	WNDCLASSEX wc;
#ifdef DO_SWAP_INTERVAL
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT;
#endif
#if defined(TYPE_NATIVE)
	PIXELFORMATDESCRIPTOR desc;
	int		      fmt;
#endif
	RECT		    rect;
	DWORD		    style;
	gf_draw_platform_t* platform = malloc(sizeof(*platform));
	memset(platform, 0, sizeof(*platform));
	platform->engine = engine;

	platform->instance = (HINSTANCE)GetModuleHandle(NULL);
	if(platform->instance == NULL) {
		gf_log_function(engine, "Failed to get instance", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}

	wc.cbSize	 = sizeof(wc);
	wc.style	 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc	 = gf_draw_platform_proc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = platform->instance;
	wc.hIcon	 = LoadIcon(platform->instance, "GAME");
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = "goldfish";
	wc.hIconSm	 = LoadIcon(platform->instance, "GAME");
	if(!RegisterClassEx(&wc)) {
		gf_log_function(engine, "Failed to register class", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}

	platform->window = CreateWindow("goldfish", draw->title, (WS_OVERLAPPEDWINDOW), draw->x, draw->y, draw->width, draw->height, NULL, 0, platform->instance, NULL);
	if(platform->window == NULL) {
		gf_log_function(engine, "Failed to create window", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}

	SetWindowLongPtr(platform->window, GWLP_USERDATA, (LONG_PTR)draw);

	platform->dc = GetDC(platform->window);

#if defined(TYPE_NATIVE)
	memset(&desc, 0, sizeof(desc));
	desc.nSize	= sizeof(desc);
	desc.nVersion	= 1;
	desc.dwFlags	= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	desc.iPixelType = PFD_TYPE_RGBA;
	desc.cColorBits = 24;
	desc.cAlphaBits = 8;
	desc.cDepthBits = 32;

	fmt = ChoosePixelFormat(platform->dc, &desc);
	SetPixelFormat(platform->dc, fmt, &desc);

	platform->glrc = wglCreateContext(platform->dc);
	if(platform->glrc == NULL) {
		gf_log_function(engine, "Failed to create OpenGL context", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}
	wglMakeCurrent(platform->dc, platform->glrc);
#elif defined(TYPE_OSMESA)
	platform->context = OSMesaCreateContext(OSMESA_BGRA, NULL);
	if(platform->context == NULL) {
		gf_log_function(engine, "Failed to get OpenGL context", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}
	platform->buffer = malloc(draw->width * draw->height * 4);
	OSMesaMakeCurrent(platform->context, platform->buffer, GL_UNSIGNED_BYTE, draw->width, draw->height);
	OSMesaPixelStore(OSMESA_Y_UP, 0);

	memset(&platform->header, 0, sizeof(platform->header));
	platform->header.bV5Size	= sizeof(platform->header);
	platform->header.bV5Width	= draw->width;
	platform->header.bV5Height	= -((LONG)draw->height);
	platform->header.bV5Planes	= 1;
	platform->header.bV5BitCount	= 32;
	platform->header.bV5Compression = BI_RGB;

	platform->bitmap   = CreateDIBSection(platform->dc, (BITMAPINFO*)&platform->header, DIB_RGB_COLORS, (void**)&platform->buffer, NULL, (DWORD)0);
	platform->bitmapdc = CreateCompatibleDC(platform->dc);
	SelectObject(platform->bitmapdc, platform->bitmap);
#endif

#if defined(DO_SWAP_INTERVAL) && defined(TYPE_NATIVE)
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if(wglSwapIntervalEXT != NULL) {
		gf_log_function(engine, "Enabled VSync", "");
		wglSwapIntervalEXT(1);
	}
#endif

	SetRect(&rect, 0, 0, draw->width, draw->height);
	style = (DWORD)GetWindowLongPtr(platform->window, GWL_STYLE);
	AdjustWindowRect(&rect, style, FALSE);
	SetWindowPos(platform->window, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);

	ShowWindow(platform->window, SW_NORMAL);
	UpdateWindow(platform->window);

	return platform;
}

void gf_draw_platform_destroy(gf_draw_platform_t* platform) {
#if defined(TYPE_NATIVE)
	if(platform->glrc != NULL) {
		wglMakeCurrent(NULL, NULL);
	}
#endif
	if(platform->dc != NULL) {
		ReleaseDC(platform->window, platform->dc);
	}
#if defined(TYPE_NATIVE)
	if(platform->glrc != NULL) {
		wglDeleteContext(platform->glrc);
	}
#elif defined(TYPE_OSMESA)
	if(platform->context != NULL) {
		OSMesaDestroyContext(platform->context);
	}
	if(platform->bitmapdc != NULL) {
		DeleteDC(platform->bitmapdc);
	}
	if(platform->bitmap != NULL) {
		DeleteObject(platform->bitmap);
	}
#endif
	if(platform->window != NULL) {
		DestroyWindow(platform->window);
	}
	gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	free(platform);
}
