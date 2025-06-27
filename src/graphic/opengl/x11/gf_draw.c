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
#include <gf_input.h>
#include <gf_log.h>
#include <gf_draw.h>

/* Standard */
#include <string.h>
#include <stdlib.h>

#ifdef GF_DO_SWAP_INTERVAL
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

typedef struct keymap {
	KeySym key;
	int    value;
} keymap_t;

static keymap_t* keymaps = NULL;

static void whmput(KeySym key, int value) { hmput(keymaps, key, value); }

void gf_draw_platform_begin(void) {
	int i;

	hmdefault(keymaps, -1);
	whmput(XK_Escape, GF_INPUT_KEY_ESCAPE);

	whmput(XK_Return, GF_INPUT_KEY_ENTER);

	whmput(XK_BackSpace, GF_INPUT_KEY_BACKSPACE);
	whmput(XK_space, GF_INPUT_KEY_SPACE);

	for(i = 0; i < 10; i++) whmput(XK_0 + i, GF_INPUT_KEY_0 + i);

	for(i = 0; i < 26; i++) {
		whmput(XK_A + i, GF_INPUT_KEY_A + i);
		whmput(XK_a + i, GF_INPUT_KEY_A + i);
	}

	for(i = 0; i < 12; i++) {
		whmput(XK_F1 + i, GF_INPUT_KEY_F1 + i);
	}

	whmput(XK_Up, GF_INPUT_KEY_UP);
	whmput(XK_Down, GF_INPUT_KEY_DOWN);
	whmput(XK_Left, GF_INPUT_KEY_LEFT);
	whmput(XK_Right, GF_INPUT_KEY_RIGHT);

	whmput(XK_Shift_L, GF_INPUT_KEY_LEFT_SHIFT);
	whmput(XK_Shift_R, GF_INPUT_KEY_RIGHT_SHIFT);

	whmput(XK_Alt_L, GF_INPUT_KEY_LEFT_ALT);
	whmput(XK_Alt_R, GF_INPUT_KEY_RIGHT_ALT);

	whmput(XK_Control_L, GF_INPUT_KEY_LEFT_CONTROL);
	whmput(XK_Control_R, GF_INPUT_KEY_RIGHT_CONTROL);

	whmput(XK_Super_L, GF_INPUT_KEY_LEFT_SUPER);
	whmput(XK_Super_R, GF_INPUT_KEY_RIGHT_SUPER);

	whmput(XK_Tab, GF_INPUT_KEY_TAB);
	whmput(XK_Caps_Lock, GF_INPUT_KEY_CAPSLOCK);

	whmput(XK_grave, GF_INPUT_KEY_GRAVE);
	whmput(XK_minus, GF_INPUT_KEY_MINUS);
	whmput(XK_equal, GF_INPUT_KEY_EQUALS);
	whmput(XK_bracketleft, GF_INPUT_KEY_LEFT_BRACKET);
	whmput(XK_bracketright, GF_INPUT_KEY_RIGHT_BRACKET);
	whmput(XK_backslash, GF_INPUT_KEY_BACKSLASH);
	whmput(XK_semicolon, GF_INPUT_KEY_SEMICOLON);
	whmput(XK_apostrophe, GF_INPUT_KEY_QUOTE);
	whmput(XK_comma, GF_INPUT_KEY_COMMA);
	whmput(XK_period, GF_INPUT_KEY_PERIOD);
	whmput(XK_slash, GF_INPUT_KEY_SLASH);

	whmput(XK_Insert, GF_INPUT_KEY_INSERT);
	whmput(XK_Delete, GF_INPUT_KEY_DELETE);
	whmput(XK_Home, GF_INPUT_KEY_HOME);
	whmput(XK_End, GF_INPUT_KEY_END);
	whmput(XK_Page_Up, GF_INPUT_KEY_PAGE_UP);
	whmput(XK_Page_Down, GF_INPUT_KEY_PAGE_DOWN);

	whmput(XK_Print, GF_INPUT_KEY_PRINT_SCREEN);
	whmput(XK_Scroll_Lock, GF_INPUT_KEY_SCROLL_LOCK);
	whmput(XK_Pause, GF_INPUT_KEY_PAUSE_BREAK);
	whmput(XK_Num_Lock, GF_INPUT_KEY_NUM_LOCK);
}

void gf_draw_platform_end(void) {
	hmfree(keymaps);
	keymaps = NULL;
}

int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query) {
	const char* ext = NULL;
	const char* ptr;
	const int   len = strlen(query);

#if defined(GF_TYPE_NATIVE)
	glXMakeCurrent(draw->platform->display, draw->platform->window, draw->platform->context);
#endif

#if defined(GF_TYPE_NATIVE)
	ext = glXQueryExtensionsString(draw->platform->display, DefaultScreen(draw->platform->display));
	ptr = strstr(ext, query);
	return ((ptr != NULL) && ((ptr[len] == ' ') || (ptr[len] == '\0')));
#endif
}

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	int    i = 0;
	int    screen;
	Window root;
#if defined(GF_TYPE_NATIVE)
	int	     attribs[64];
	XVisualInfo* visual;
#endif
	Pixmap		     blank;
	XColor		     dummy;
	XSetWindowAttributes attr;
	XSizeHints	     hints;
	unsigned char	     b;
	int		     interval = 0;
	gf_draw_platform_t*  platform = malloc(sizeof(*platform));
	memset(platform, 0, sizeof(*platform));
	platform->engine = engine;

	draw->platform = platform;

	platform->display = XOpenDisplay(NULL);
	if(platform->display == NULL) {
		gf_log_function(engine, "Failed to open display", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}

	screen = DefaultScreen(platform->display);
	root   = RootWindow(platform->display, screen);

#if defined(GF_TYPE_NATIVE)
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
#endif

	attr.event_mask = StructureNotifyMask | ExposureMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
#if defined(GF_TYPE_NATIVE)
	attr.colormap	 = XCreateColormap(platform->display, root, visual->visual, AllocNone);
	platform->window = XCreateWindow(platform->display, root, draw->width, draw->height, draw->width, draw->height, 0, visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask, &attr);
#endif

	b		 = 0;
	blank		 = XCreateBitmapFromData(platform->display, platform->window, &b, 1, 1);
	platform->cursor = XCreatePixmapCursor(platform->display, blank, blank, &dummy, &dummy, 0, 0);
	XFreePixmap(platform->display, blank);

	XDefineCursor(platform->display, platform->window, platform->cursor);

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

#if defined(GF_TYPE_NATIVE)
	platform->context = glXCreateContext(platform->display, visual, NULL, True);
	if(platform->context == NULL) {
		XFree(visual);
		gf_log_function(engine, "Failed to get OpenGL context", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}

	XFree(visual);
#endif

	XMapWindow(platform->display, platform->window);
#if defined(GF_TYPE_NATIVE)
	glXMakeCurrent(platform->display, platform->window, platform->context);
#endif

#if defined(GF_DO_SWAP_INTERVAL) && defined(GF_TYPE_NATIVE)
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
#if defined(GF_TYPE_NATIVE)
	glXMakeCurrent(draw->platform->display, draw->platform->window, draw->platform->context);
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
#if defined(GF_TYPE_NATIVE)
			glXMakeCurrent(draw->platform->display, draw->platform->window, draw->platform->context);
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
		} else if(event.type == KeyPress) {
			KeySym ks  = XLookupKeysym(&event.xkey, 0);
			int    key = hmget(keymaps, ks);
			gf_input_key_press(draw->input, key);
		} else if(event.type == KeyRelease) {
			KeySym ks     = XLookupKeysym(&event.xkey, 0);
			int    key    = hmget(keymaps, ks);
			int    retrig = 0;
			if(XEventsQueued(draw->platform->display, QueuedAfterReading)) {
				XEvent nev;
				XPeekEvent(draw->platform->display, &nev);
				if(nev.type == KeyPress && nev.xkey.time == event.xkey.time && nev.xkey.keycode == event.xkey.keycode) {
					retrig = 1;
				}
			}
			if(!retrig) {
				gf_input_key_release(draw->input, key);
			}
		}
	}
	if(ret == 0) {
		gf_draw_driver_before(draw);
		gf_draw_frame(draw);
		gf_draw_driver_after(draw);

#if defined(GF_TYPE_NATIVE)
		glXSwapBuffers(draw->platform->display, draw->platform->window);
#endif
	}
	return ret;
}

void gf_draw_platform_destroy(gf_draw_platform_t* platform) {
	if(platform->context != NULL) {
#if defined(GF_TYPE_NATIVE)
		glXMakeCurrent(platform->display, None, NULL);
		glXDestroyContext(platform->display, platform->context);
#endif
	}
	if(platform->display != NULL) {
		XDestroyWindow(platform->display, platform->window);
		XCloseDisplay(platform->display);
	}
	/* TODO: How do I even free cursor? Do I even have to? */
	gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	free(platform);
}
