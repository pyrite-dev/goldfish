#define GF_EXPOSE_DRAW_PLATFORM
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CORE
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
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

/* why is this missing in msys 1.0? */
#ifndef MAPVK_VK_TO_VSC
#define MAPVK_VK_TO_VSC 0x00
#endif

typedef const char*(APIENTRY* PFNWGLGETEXTENSIONSSTRINGARB)(HDC);
#ifdef GF_DO_SWAP_INTERVAL
typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALPROC)(int);
#endif

typedef struct keymap {
	DWORD key;
	int   value;
} keymap_t;

static keymap_t* keymaps = NULL;

static void whmput(DWORD key, int value) { hmput(keymaps, key, value); }

void gf_draw_platform_begin(void) {
	int i;

	hmdefault(keymaps, -1);
	whmput(0x0001, GF_INPUT_KEY_ESCAPE);

	whmput(0x001c, GF_INPUT_KEY_ENTER);

	whmput(0x000e, GF_INPUT_KEY_BACKSPACE);
	whmput(0x0039, GF_INPUT_KEY_SPACE);

	whmput(0x000b, GF_INPUT_KEY_0);
	whmput(0x0002, GF_INPUT_KEY_1);
	whmput(0x0003, GF_INPUT_KEY_2);
	whmput(0x0004, GF_INPUT_KEY_3);
	whmput(0x0005, GF_INPUT_KEY_4);
	whmput(0x0006, GF_INPUT_KEY_5);
	whmput(0x0007, GF_INPUT_KEY_6);
	whmput(0x0008, GF_INPUT_KEY_7);
	whmput(0x0009, GF_INPUT_KEY_8);
	whmput(0x000a, GF_INPUT_KEY_9);

	whmput(0x001e, GF_INPUT_KEY_A);
	whmput(0x0030, GF_INPUT_KEY_B);
	whmput(0x002e, GF_INPUT_KEY_C);
	whmput(0x0020, GF_INPUT_KEY_D);
	whmput(0x0012, GF_INPUT_KEY_E);
	whmput(0x0021, GF_INPUT_KEY_F);
	whmput(0x0022, GF_INPUT_KEY_G);
	whmput(0x0023, GF_INPUT_KEY_H);
	whmput(0x0017, GF_INPUT_KEY_I);
	whmput(0x0024, GF_INPUT_KEY_J);
	whmput(0x0025, GF_INPUT_KEY_K);
	whmput(0x0026, GF_INPUT_KEY_L);
	whmput(0x0032, GF_INPUT_KEY_M);
	whmput(0x0031, GF_INPUT_KEY_N);
	whmput(0x0018, GF_INPUT_KEY_O);
	whmput(0x0019, GF_INPUT_KEY_P);
	whmput(0x0010, GF_INPUT_KEY_Q);
	whmput(0x0013, GF_INPUT_KEY_R);
	whmput(0x001f, GF_INPUT_KEY_S);
	whmput(0x0014, GF_INPUT_KEY_T);
	whmput(0x0016, GF_INPUT_KEY_U);
	whmput(0x002f, GF_INPUT_KEY_V);
	whmput(0x0011, GF_INPUT_KEY_W);
	whmput(0x002d, GF_INPUT_KEY_X);
	whmput(0x0015, GF_INPUT_KEY_Y);
	whmput(0x002c, GF_INPUT_KEY_Z);

	whmput(0x003b, GF_INPUT_KEY_F1);
	whmput(0x003c, GF_INPUT_KEY_F2);
	whmput(0x003d, GF_INPUT_KEY_F3);
	whmput(0x003e, GF_INPUT_KEY_F4);
	whmput(0x003f, GF_INPUT_KEY_F5);
	whmput(0x0040, GF_INPUT_KEY_F6);
	whmput(0x0041, GF_INPUT_KEY_F7);
	whmput(0x0042, GF_INPUT_KEY_F8);
	whmput(0x0043, GF_INPUT_KEY_F9);
	whmput(0x0044, GF_INPUT_KEY_F10);
	whmput(0x0057, GF_INPUT_KEY_F11);
	whmput(0x0058, GF_INPUT_KEY_F12);

	whmput(0xe048, GF_INPUT_KEY_UP);
	whmput(0xe050, GF_INPUT_KEY_DOWN);
	whmput(0xe04b, GF_INPUT_KEY_LEFT);
	whmput(0xe04d, GF_INPUT_KEY_RIGHT);

	whmput(0x002a, GF_INPUT_KEY_LEFT_SHIFT);
	whmput(0x0036, GF_INPUT_KEY_RIGHT_SHIFT);

	whmput(0x0038, GF_INPUT_KEY_LEFT_ALT);
	whmput(0xe038, GF_INPUT_KEY_RIGHT_ALT);

	whmput(0x001d, GF_INPUT_KEY_LEFT_CONTROL);
	whmput(0xe01d, GF_INPUT_KEY_RIGHT_CONTROL);

	whmput(0xe05b, GF_INPUT_KEY_LEFT_SUPER);
	whmput(0xe05c, GF_INPUT_KEY_RIGHT_SUPER);

	whmput(0x000f, GF_INPUT_KEY_TAB);
	whmput(0x003a, GF_INPUT_KEY_CAPSLOCK);

	whmput(0x0029, GF_INPUT_KEY_GRAVE);
	whmput(0x000c, GF_INPUT_KEY_MINUS);
	whmput(0x000d, GF_INPUT_KEY_EQUALS);
	whmput(0x001a, GF_INPUT_KEY_LEFT_BRACKET);
	whmput(0x001b, GF_INPUT_KEY_RIGHT_BRACKET);
	whmput(0x002b, GF_INPUT_KEY_BACKSLASH);
	whmput(0x0027, GF_INPUT_KEY_SEMICOLON);
	whmput(0x0028, GF_INPUT_KEY_QUOTE);
	whmput(0x0033, GF_INPUT_KEY_COMMA);
	whmput(0x0034, GF_INPUT_KEY_PERIOD);
	whmput(0x0035, GF_INPUT_KEY_SLASH);

	whmput(0xe052, GF_INPUT_KEY_INSERT);
	whmput(0xe053, GF_INPUT_KEY_DELETE);
	whmput(0xe047, GF_INPUT_KEY_HOME);
	whmput(0xe04f, GF_INPUT_KEY_END);
	whmput(0xe049, GF_INPUT_KEY_PAGE_UP);
	whmput(0xe051, GF_INPUT_KEY_PAGE_DOWN);

	whmput(0x0054, GF_INPUT_KEY_PRINT_SCREEN);
	whmput(0xe046, GF_INPUT_KEY_SCROLL_LOCK);
	whmput(0x0045, GF_INPUT_KEY_PAUSE_BREAK);
	whmput(0xe045, GF_INPUT_KEY_NUM_LOCK);
}

void gf_draw_platform_end(void) {
	hmfree(keymaps);
	keymaps = NULL;
}

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
#if defined(GF_TYPE_NATIVE)
			wglMakeCurrent(draw->platform->dc, draw->platform->glrc);
#elif defined(GF_TYPE_OSMESA)
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
	case WM_KEYDOWN:
		if(draw->input != NULL) {
			DWORD sc = HIWORD(lp) & 0xff;
			if(sc == 0) {
				sc = MapVirtualKey(wp, MAPVK_VK_TO_VSC);
			} else if((HIWORD(lp) & KF_EXTENDED) == KF_EXTENDED) {
				sc = MAKEWORD(sc, 0xe0);
			}
			int key = hmget(keymaps, sc);
			gf_input_key_press(draw->input, key);
		}
		break;
	case WM_KEYUP:
		if(draw->input != NULL) {
			DWORD sc = HIWORD(lp) & 0xff;
			if(sc == 0) {
				sc = MapVirtualKey(wp, MAPVK_VK_TO_VSC);
			} else if((HIWORD(lp) & KF_EXTENDED) == KF_EXTENDED) {
				sc = MAKEWORD(sc, 0xe0);
			}
			int key = hmget(keymaps, sc);
			gf_input_key_release(draw->input, key);
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
#if defined(GF_TYPE_NATIVE)
	const char*		     ext = NULL;
	const char*		     ptr;
	PFNWGLGETEXTENSIONSSTRINGARB proc;
#endif
	const int len = strlen(query);
#if defined(GF_TYPE_NATIVE)
	wglMakeCurrent(draw->platform->dc, draw->platform->glrc);

	proc = (PFNWGLGETEXTENSIONSSTRINGARB)wglGetProcAddress("wglGetExtensionsStringARB");

	if(proc != NULL) {
		ext = proc(draw->platform->dc);
		ptr = strstr(ext, query);
		return ((ptr != NULL) && ((ptr[len] == ' ') || (ptr[len] == '\0')));
	}
#elif defined(GF_TYPE_OSMESA)
#endif
	return 0;
}

int gf_draw_platform_step(gf_draw_t* draw) {
	MSG msg;
	int ret = 0;
#if defined(GF_TYPE_NATIVE)
	wglMakeCurrent(draw->platform->dc, draw->platform->glrc);
#elif defined(GF_TYPE_OSMESA)
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

#if defined(GF_TYPE_NATIVE)
		SwapBuffers(draw->platform->dc);
#elif defined(GF_TYPE_OSMESA)
		BitBlt(draw->platform->dc, 0, 0, draw->width, draw->height, draw->platform->bitmapdc, 0, 0, SRCCOPY);
#endif
	}
	return ret;
}

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	WNDCLASSEX wc;
#ifdef GF_DO_SWAP_INTERVAL
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT;
#endif
#if defined(GF_TYPE_NATIVE)
	PIXELFORMATDESCRIPTOR desc;
	int		      fmt;
#endif
	RECT		    rect;
	DWORD		    style;
	gf_draw_platform_t* platform = malloc(sizeof(*platform));
	memset(platform, 0, sizeof(*platform));
	platform->engine = engine;

	draw->platform = platform;

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
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = "goldfish";
	wc.hIcon	 = LoadIcon(platform->instance, "GAME");
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

#if WINVER >= 0x0400
	{
		BITMAPV4HEADER bi;
		HBITMAP	       color;
		HBITMAP	       mask;
		unsigned char* target = NULL;
		ICONINFO       ii;
		HICON	       handle;
		HDC	       dc = GetDC(NULL);

		memset(&bi, 0, sizeof(bi));
		bi.bV4Size	    = sizeof(bi);
		bi.bV4Width	    = 1;
		bi.bV4Height	    = -((LONG)1);
		bi.bV4Planes	    = 1;
		bi.bV4BitCount	    = 32;
		bi.bV4V4Compression = BI_RGB;
		bi.bV4RedMask	    = 0xff << 16;
		bi.bV4GreenMask	    = 0xff << 8;
		bi.bV4BlueMask	    = 0xff << 0;
		bi.bV4AlphaMask	    = 0xff << 24;

		color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&target, NULL, (DWORD)0);
		ReleaseDC(NULL, dc);

		mask = CreateBitmap(1, 1, 1, 1, NULL);

		memset(&target[0], 0, 4);

		memset(&ii, 0, sizeof(ii));
		ii.fIcon    = TRUE;
		ii.xHotspot = 0;
		ii.yHotspot = 0;
		ii.hbmMask  = mask;
		ii.hbmColor = color;

		handle = CreateIconIndirect(&ii);

		DeleteObject(color);
		DeleteObject(mask);

		SetClassLongPtr(platform->window, GCLP_HCURSOR, (LPARAM)handle);
		SetCursor((HCURSOR)handle);
		platform->cursor = (HCURSOR)handle;
	}

	if(engine->icon != NULL) {
		BITMAPV4HEADER bi;
		HBITMAP	       color;
		HBITMAP	       mask;
		unsigned char* target = NULL;
		ICONINFO       ii;
		HICON	       handle;
		int	       i;
		HDC	       dc = GetDC(NULL);

		memset(&bi, 0, sizeof(bi));
		bi.bV4Size	    = sizeof(bi);
		bi.bV4Width	    = engine->icon_width;
		bi.bV4Height	    = -((LONG)engine->icon_height);
		bi.bV4Planes	    = 1;
		bi.bV4BitCount	    = 32;
		bi.bV4V4Compression = BI_RGB;
		bi.bV4RedMask	    = 0xff << 16;
		bi.bV4GreenMask	    = 0xff << 8;
		bi.bV4BlueMask	    = 0xff << 0;
		bi.bV4AlphaMask	    = 0xff << 24;

		color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&target, NULL, (DWORD)0);
		ReleaseDC(NULL, dc);

		mask = CreateBitmap(engine->icon_width, engine->icon_height, 1, 1, NULL);

		for(i = 0; i < engine->icon_width * engine->icon_height; i++) {
			target[i * 4 + 0] = engine->icon[i * 4 + 2];
			target[i * 4 + 1] = engine->icon[i * 4 + 1];
			target[i * 4 + 2] = engine->icon[i * 4 + 0];
			target[i * 4 + 3] = engine->icon[i * 4 + 3];
		}

		memset(&ii, 0, sizeof(ii));
		ii.fIcon    = TRUE;
		ii.xHotspot = 0;
		ii.yHotspot = 0;
		ii.hbmMask  = mask;
		ii.hbmColor = color;

		handle = CreateIconIndirect(&ii);

		DeleteObject(color);
		DeleteObject(mask);

		SetClassLongPtr(platform->window, GCLP_HICON, (LPARAM)handle);
		SetClassLongPtr(platform->window, GCLP_HICONSM, (LPARAM)handle);
		DestroyIcon(handle);
	}
#endif

	SetWindowLongPtr(platform->window, GWLP_USERDATA, (LONG_PTR)draw);

	platform->dc = GetDC(platform->window);

#if defined(GF_TYPE_NATIVE)
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
#elif defined(GF_TYPE_OSMESA)
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
	platform->header.bV4Size	  = sizeof(platform->header);
	platform->header.bV4Width	  = draw->width;
	platform->header.bV4Height	  = -((LONG)draw->height);
	platform->header.bV4Planes	  = 1;
	platform->header.bV4BitCount	  = 32;
	platform->header.bV4V4Compression = BI_RGB;

	platform->bitmap   = CreateDIBSection(platform->dc, (BITMAPINFO*)&platform->header, DIB_RGB_COLORS, (void**)&platform->buffer, NULL, (DWORD)0);
	platform->bitmapdc = CreateCompatibleDC(platform->dc);
	SelectObject(platform->bitmapdc, platform->bitmap);
#endif

#if defined(GF_DO_SWAP_INTERVAL) && defined(GF_TYPE_NATIVE)
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
#if defined(GF_TYPE_NATIVE)
	if(platform->glrc != NULL) {
		wglMakeCurrent(NULL, NULL);
	}
#endif
	if(platform->dc != NULL) {
		ReleaseDC(platform->window, platform->dc);
	}
#if defined(GF_TYPE_NATIVE)
	if(platform->glrc != NULL) {
		wglDeleteContext(platform->glrc);
	}
#elif defined(GF_TYPE_OSMESA)
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
#if WINVER >= 0x0400
	if(platform->cursor != NULL) {
		DestroyCursor((HCURSOR)platform->cursor);
	}
#endif
	gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	free(platform);
}
