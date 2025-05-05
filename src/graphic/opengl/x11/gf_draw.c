#define GF_EXPOSE_DRAW_PLATFORM
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CORE
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <gf_opengl.h>

/* Interface */
#include <gf_draw_platform.h>

/* Engine */
#include <gf_draw_driver.h>
#include <gf_input.h>
#include <gf_log.h>
#include <gf_draw.h>

/* Standard */
#include <string.h>
#include <stdlib.h>

#ifdef DO_SWAP_INTERVAL
#ifndef GLX_MESA_swap_control
#define GLX_MESA_swap_control 1
typedef int (*PFNGLXGETSWAPINTERVALMESAPROC)(void);
typedef void (*PFNGLXSWAPINTERVALMESAPROC)(int);
#endif

#ifndef GLX_EXT_swap_control
#define GLX_EXT_swap_control 1
typedef void (*PFNGLXSWAPINTERVALEXTPROC)(Display*, GLXDrawable, int);
#endif

#ifndef GLX_SGI_swap_control
#define GLX_SGI_swap_control 1
typedef void (*PFNGLXSWAPINTERVALSGIPROC)(int);
#endif
#endif

void gf_draw_platform_begin(void) {}
void gf_draw_platform_end(void) {}

int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query) {
	const char* ext = NULL;
	const char* ptr;
	const int   len = strlen(query);

#if defined(TYPE_NATIVE)
	glXMakeCurrent(draw->platform->display, draw->platform->window, draw->platform->context);
#elif defined(TYPE_OSMESA)
	OSMesaMakeCurrent(draw->platform->context, draw->platform->buffer, GL_UNSIGNED_BYTE, draw->width, draw->height);
#endif

#if defined(TYPE_NATIVE)
	ext = glXQueryExtensionsString(draw->platform->display, DefaultScreen(draw->platform->display));
	ptr = strstr(ext, query);
	return ((ptr != NULL) && ((ptr[len] == ' ') || (ptr[len] == '\0')));
#elif defined(TYPE_OSMESA)
	return 0;
#endif
}

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	int    i = 0;
	int    screen;
	Window root;
#if defined(TYPE_NATIVE)
	int	     attribs[64];
	XVisualInfo* visual;
#endif
	XSetWindowAttributes attr;
	XSizeHints	     hints;
	int		     interval = 0;
	gf_draw_platform_t*  platform = malloc(sizeof(*platform));
	memset(platform, 0, sizeof(*platform));
	platform->engine = engine;

#if defined(TYPE_OSMESA)
	platform->buffer = NULL;
	platform->image	 = NULL;
#endif

	platform->display = XOpenDisplay(NULL);
	if(platform->display == NULL) {
		gf_log_function(engine, "Failed to open display", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}

	screen = DefaultScreen(platform->display);
	root   = RootWindow(platform->display, screen);

#if defined(TYPE_NATIVE)
	attribs[i++] = GLX_RGBA;
	attribs[i++] = GLX_DOUBLEBUFFER;

	attribs[i++] = GLX_RED_SIZE;
	attribs[i++] = 1;
	attribs[i++] = GLX_GREEN_SIZE;
	attribs[i++] = 1;
	attribs[i++] = GLX_BLUE_SIZE;
	attribs[i++] = 1;
	attribs[i++] = GLX_DEPTH_SIZE;
	attribs[i++] = 1;

	attribs[i++] = None;

	visual = glXChooseVisual(platform->display, screen, attribs);
	if(visual == NULL) {
		gf_log_function(engine, "Failed to get visual", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}
#elif defined(TYPE_OSMESA)
	platform->visual.visual = DefaultVisual(platform->display, screen);
	platform->visual.depth	= DefaultDepth(platform->display, screen);
#endif

	attr.event_mask = StructureNotifyMask | ExposureMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
#if defined(TYPE_NATIVE)
	attr.colormap	 = XCreateColormap(platform->display, root, visual->visual, AllocNone);
	platform->window = XCreateWindow(platform->display, root, draw->width, draw->height, draw->width, draw->height, 0, visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask, &attr);
#elif defined(TYPE_OSMESA)
	attr.colormap	 = XCreateColormap(platform->display, root, platform->visual.visual, AllocNone);
	platform->window = XCreateWindow(platform->display, root, draw->width, draw->height, draw->width, draw->height, 0, platform->visual.depth, InputOutput, platform->visual.visual, CWColormap | CWEventMask, &attr);
#endif

	if(engine->icon != NULL) {
		unsigned long* icon	 = malloc((2 + engine->icon_width * engine->icon_height) * sizeof(*icon));
		Atom	       icon_atom = XInternAtom(platform->display, "_NET_WM_ICON", False);

		icon[0] = engine->icon_width;
		icon[1] = engine->icon_height;

		for(i = 0; i < engine->icon_width * engine->icon_height; i++) {
			icon[2 + i] = 0;

			icon[2 + i] = icon[2 + i] << 8;
			icon[2 + i] |= engine->icon[i * 4 + 3];

			icon[2 + i] = icon[2 + i] << 8;
			icon[2 + i] |= engine->icon[i * 4 + 0];

			icon[2 + i] = icon[2 + i] << 8;
			icon[2 + i] |= engine->icon[i * 4 + 1];

			icon[2 + i] = icon[2 + i] << 8;
			icon[2 + i] |= engine->icon[i * 4 + 2];
		}

		XChangeProperty(platform->display, platform->window, icon_atom, 6, 32, PropModeReplace, (unsigned char*)icon, 2 + engine->icon_width * engine->icon_height);

		free(icon);
	}

	hints.x	     = draw->x;
	hints.y	     = draw->y;
	hints.width  = draw->width;
	hints.height = draw->height;
	hints.flags  = USSize | USPosition;
	XSetNormalHints(platform->display, platform->window, &hints);
	XSetStandardProperties(platform->display, platform->window, draw->title, "GoldFish", None, (char**)NULL, 0, &hints);

	platform->wm_delete_window = XInternAtom(platform->display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(platform->display, platform->window, &platform->wm_delete_window, 1);

#if defined(TYPE_NATIVE)
	platform->context = glXCreateContext(platform->display, visual, NULL, True);
	if(platform->context == NULL) {
		XFree(visual);
		gf_log_function(engine, "Failed to get OpenGL context", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}

	XFree(visual);
#elif defined(TYPE_OSMESA)
	platform->context = OSMesaCreateContext(OSMESA_BGRA, NULL);
	if(platform->context == NULL) {
		gf_log_function(engine, "Failed to get OpenGL context", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}
#endif

	XMapWindow(platform->display, platform->window);
#if defined(TYPE_NATIVE)
	glXMakeCurrent(platform->display, platform->window, platform->context);
#elif defined(TYPE_OSMESA)
	platform->buffer = malloc(draw->width * draw->height * 4);
	platform->gc	 = XCreateGC(platform->display, platform->window, 0, NULL);
	platform->image	 = XCreateImage(platform->display, platform->visual.visual, platform->visual.depth, ZPixmap, 0, NULL, draw->width, draw->height, 32, 0);
	OSMesaMakeCurrent(platform->context, platform->buffer, GL_UNSIGNED_BYTE, draw->width, draw->height);
	OSMesaPixelStore(OSMESA_Y_UP, 0);
#endif

#if defined(DO_SWAP_INTERVAL) && defined(TYPE_NATIVE)
	if(gf_draw_platform_has_extension(draw, "GLX_EXT_swap_control")) {
		unsigned int		  tmp  = -1;
		PFNGLXSWAPINTERVALEXTPROC proc = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddressARB("glXSwapIntervalEXT");
		if(proc != NULL) {
			proc(platform->display, platform->window, 1);
		}
		glXQueryDrawable(platform->display, platform->window, GLX_SWAP_INTERVAL_EXT, &tmp);
		interval = tmp;
	} else if(gf_draw_platform_has_extension(draw, "GLX_MESA_swap_control")) {
		PFNGLXGETSWAPINTERVALMESAPROC proc  = (PFNGLXGETSWAPINTERVALMESAPROC)glXGetProcAddressARB("glXGetSwapIntervalMESA");
		PFNGLXSWAPINTERVALMESAPROC    proc2 = (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddressARB("glXSwapIntervalMESA");
		if(proc2 != NULL) {
			proc2(1);
		}
		interval = proc();
	} else if(gf_draw_platform_has_extension(draw, "GLX_SGI_swap_control")) {
		PFNGLXSWAPINTERVALSGIPROC proc = (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddressARB("glXSwapIntervalSGI");
		proc(1);
		interval = 1;
	}
	if(interval > 0) {
		gf_log_function(engine, "Enabled VSync", "");
	}
#endif
	return platform;
}

int gf_draw_platform_step(gf_draw_t* draw) {
	int ret = 0;
#if defined(TYPE_NATIVE)
	glXMakeCurrent(draw->platform->display, draw->platform->window, draw->platform->context);
#elif defined(TYPE_OSMESA)
	OSMesaMakeCurrent(draw->platform->context, draw->platform->buffer, GL_UNSIGNED_BYTE, draw->width, draw->height);
#endif
	while(XPending(draw->platform->display) > 0) {
		XEvent event;
		XNextEvent(draw->platform->display, &event);
		if(event.type == Expose) {
			break;
		} else if(event.type == MotionNotify) {
			if(draw->input != NULL) {
				draw->input->mouse_x = event.xmotion.x;
				draw->input->mouse_y = event.xmotion.y;
			}
		} else if(event.type == ConfigureNotify) {
			draw->x	     = event.xconfigure.x;
			draw->y	     = event.xconfigure.y;
			draw->width  = event.xconfigure.width;
			draw->height = event.xconfigure.height;
#if defined(TYPE_NATIVE)
			glXMakeCurrent(draw->platform->display, draw->platform->window, draw->platform->context);
#elif defined(TYPE_OSMESA)
			free(draw->platform->buffer);
			XDestroyImage(draw->platform->image);
			draw->platform->buffer = malloc(draw->width * draw->height * 4);
			draw->platform->image  = XCreateImage(draw->platform->display, draw->platform->visual.visual, draw->platform->visual.depth, ZPixmap, 0, NULL, draw->width, draw->height, 32, 0);
			OSMesaMakeCurrent(draw->platform->context, draw->platform->buffer, GL_UNSIGNED_BYTE, draw->width, draw->height);
#endif
			gf_draw_reshape(draw);
		} else if(event.type == ButtonPress) {
			if(draw->input != NULL) {
				if(event.xbutton.button == Button1) draw->input->mouse_flag |= GF_INPUT_MOUSE_LEFT_MASK;
				if(event.xbutton.button == Button2) draw->input->mouse_flag |= GF_INPUT_MOUSE_MIDDLE_MASK;
				if(event.xbutton.button == Button3) draw->input->mouse_flag |= GF_INPUT_MOUSE_RIGHT_MASK;
			}
		} else if(event.type == ButtonRelease) {
			if(draw->input != NULL) {
				if(event.xbutton.button == Button1) draw->input->mouse_flag ^= GF_INPUT_MOUSE_LEFT_MASK;
				if(event.xbutton.button == Button2) draw->input->mouse_flag ^= GF_INPUT_MOUSE_MIDDLE_MASK;
				if(event.xbutton.button == Button3) draw->input->mouse_flag ^= GF_INPUT_MOUSE_RIGHT_MASK;
			}
		} else if(event.type == ClientMessage) {
			if(event.xclient.data.l[0] == draw->platform->wm_delete_window) {
				draw->close = 1;
				break;
			}
		}
	}
	if(ret == 0) {
		gf_draw_driver_before(draw);
		gf_draw_frame(draw);
		gf_draw_driver_after(draw);

#if defined(TYPE_NATIVE)
		glXSwapBuffers(draw->platform->display, draw->platform->window);
#elif defined(TYPE_OSMESA)
		draw->platform->image->data = (char*)draw->platform->buffer;
		XPutImage(draw->platform->display, draw->platform->window, draw->platform->gc, draw->platform->image, 0, 0, 0, 0, draw->width, draw->height);
		draw->platform->image->data = NULL;
#endif
	}
	return ret;
}

void gf_draw_platform_destroy(gf_draw_platform_t* platform) {
	if(platform->context != NULL) {
#if defined(TYPE_NATIVE)
		glXMakeCurrent(platform->display, None, NULL);
		glXDestroyContext(platform->display, platform->context);
#elif defined(TYPE_OSMESA)
		OSMesaDestroyContext(platform->context);
#endif
	}
#if defined(TYPE_OSMESA)
	if(platform->image != NULL) {
		XDestroyImage(platform->image);
		XFreeGC(platform->display, platform->gc);
	}
#endif
	if(platform->display != NULL) {
		XDestroyWindow(platform->display, platform->window);
		XCloseDisplay(platform->display);
	}
#if defined(TYPE_OSMESA)
	if(platform->buffer != NULL) {
		free(platform->buffer);
	}
#endif
	gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	free(platform);
}
