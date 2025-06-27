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
	int key;
	int value;
} keymap_t;

static keymap_t* keymaps = NULL;

static void whmput(int key, int value) { hmput(keymaps, key, value); }

static void gf_draw_platform_init_keymap(gf_draw_platform_t* platform) {
	XkbComponentNamesRec rec;
	XkbDescPtr	     desc = XkbGetMap(platform->display, 0, XkbUseCoreKbd);
	XkbDescPtr	     evdesc;

	XkbGetNames(platform->display, XkbKeyNamesMask, desc);

	memset(&rec, 0, sizeof(rec));
	rec.keycodes = (char*)"evdev";
	evdesc	     = XkbGetKeyboardByName(platform->display, XkbUseCoreKbd, &rec, XkbGBN_KeyNamesMask, XkbGBN_KeyNamesMask, False);
	if(evdesc != NULL && desc != NULL) {
		int i;
		int j;
		for(i = evdesc->min_key_code; i <= evdesc->max_key_code; i++) {
			for(j = desc->min_key_code; j <= desc->max_key_code; j++) {
				if(strncmp(evdesc->names->keys[i].name, desc->names->keys[j].name, XkbKeyNameLength) == 0) {
					int k = hmget(keymaps, i);
					hmput(platform->keymap, j, k);
					break;
				}
			}
		}
		XkbFreeKeyboard(desc, 0, True);
		XkbFreeKeyboard(evdesc, 0, True);
	} else {
		int i;
		for(i = 0; i < hmlen(keymaps); i++) {
			hmput(platform->keymap, keymaps[i].key, keymaps[i].value);
		}
	}
}

void gf_draw_platform_begin(void) {
	int i;

	hmdefault(keymaps, -1);
	whmput(9, GF_INPUT_KEY_ESCAPE);

	whmput(36, GF_INPUT_KEY_ENTER);

	whmput(22, GF_INPUT_KEY_BACKSPACE);
	whmput(65, GF_INPUT_KEY_SPACE);

	whmput(10, GF_INPUT_KEY_1);
	whmput(11, GF_INPUT_KEY_2);
	whmput(12, GF_INPUT_KEY_3);
	whmput(13, GF_INPUT_KEY_4);
	whmput(14, GF_INPUT_KEY_5);
	whmput(15, GF_INPUT_KEY_6);
	whmput(16, GF_INPUT_KEY_7);
	whmput(17, GF_INPUT_KEY_8);
	whmput(18, GF_INPUT_KEY_9);
	whmput(19, GF_INPUT_KEY_0);

	whmput(38, GF_INPUT_KEY_A);
	whmput(56, GF_INPUT_KEY_B);
	whmput(54, GF_INPUT_KEY_C);
	whmput(40, GF_INPUT_KEY_D);
	whmput(26, GF_INPUT_KEY_E);
	whmput(41, GF_INPUT_KEY_F);
	whmput(42, GF_INPUT_KEY_G);
	whmput(43, GF_INPUT_KEY_H);
	whmput(31, GF_INPUT_KEY_I);
	whmput(44, GF_INPUT_KEY_J);
	whmput(45, GF_INPUT_KEY_K);
	whmput(46, GF_INPUT_KEY_L);
	whmput(58, GF_INPUT_KEY_M);
	whmput(57, GF_INPUT_KEY_N);
	whmput(32, GF_INPUT_KEY_O);
	whmput(33, GF_INPUT_KEY_P);
	whmput(24, GF_INPUT_KEY_Q);
	whmput(27, GF_INPUT_KEY_R);
	whmput(39, GF_INPUT_KEY_S);
	whmput(28, GF_INPUT_KEY_T);
	whmput(30, GF_INPUT_KEY_U);
	whmput(55, GF_INPUT_KEY_V);
	whmput(25, GF_INPUT_KEY_W);
	whmput(53, GF_INPUT_KEY_X);
	whmput(29, GF_INPUT_KEY_Y);
	whmput(52, GF_INPUT_KEY_Z);

	whmput(67, GF_INPUT_KEY_F1);
	whmput(68, GF_INPUT_KEY_F2);
	whmput(69, GF_INPUT_KEY_F3);
	whmput(70, GF_INPUT_KEY_F4);
	whmput(71, GF_INPUT_KEY_F5);
	whmput(72, GF_INPUT_KEY_F6);
	whmput(73, GF_INPUT_KEY_F7);
	whmput(74, GF_INPUT_KEY_F8);
	whmput(75, GF_INPUT_KEY_F9);
	whmput(76, GF_INPUT_KEY_F10);
	whmput(95, GF_INPUT_KEY_F11);
	whmput(96, GF_INPUT_KEY_F12);

	whmput(111, GF_INPUT_KEY_UP);
	whmput(116, GF_INPUT_KEY_DOWN);
	whmput(113, GF_INPUT_KEY_LEFT);
	whmput(114, GF_INPUT_KEY_RIGHT);

	whmput(50, GF_INPUT_KEY_LEFT_SHIFT);
	whmput(62, GF_INPUT_KEY_RIGHT_SHIFT);

	whmput(64, GF_INPUT_KEY_LEFT_ALT);
	whmput(108, GF_INPUT_KEY_RIGHT_ALT);

	whmput(37, GF_INPUT_KEY_LEFT_CONTROL);
	whmput(105, GF_INPUT_KEY_RIGHT_CONTROL);

	whmput(133, GF_INPUT_KEY_LEFT_SUPER);
	whmput(134, GF_INPUT_KEY_RIGHT_SUPER);

	whmput(23, GF_INPUT_KEY_TAB);
	whmput(66, GF_INPUT_KEY_CAPSLOCK);

	whmput(49, GF_INPUT_KEY_GRAVE);
	whmput(20, GF_INPUT_KEY_MINUS);
	whmput(21, GF_INPUT_KEY_EQUALS);
	whmput(34, GF_INPUT_KEY_LEFT_BRACKET);
	whmput(35, GF_INPUT_KEY_RIGHT_BRACKET);
	whmput(51, GF_INPUT_KEY_BACKSLASH);
	whmput(47, GF_INPUT_KEY_SEMICOLON);
	whmput(48, GF_INPUT_KEY_QUOTE);
	whmput(59, GF_INPUT_KEY_COMMA);
	whmput(60, GF_INPUT_KEY_PERIOD);
	whmput(61, GF_INPUT_KEY_SLASH);

	whmput(118, GF_INPUT_KEY_INSERT);
	whmput(119, GF_INPUT_KEY_DELETE);
	whmput(110, GF_INPUT_KEY_HOME);
	whmput(115, GF_INPUT_KEY_END);
	whmput(112, GF_INPUT_KEY_PAGE_UP);
	whmput(117, GF_INPUT_KEY_PAGE_DOWN);

	whmput(107, GF_INPUT_KEY_PRINT_SCREEN);
	whmput(78, GF_INPUT_KEY_SCROLL_LOCK);
	whmput(128, GF_INPUT_KEY_PAUSE_BREAK);
	whmput(77, GF_INPUT_KEY_NUM_LOCK);
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

	gf_draw_platform_init_keymap(platform);

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
			int key = hmget(draw->platform->keymap, event.xkey.keycode);
			gf_input_key_press(draw->input, key);
		} else if(event.type == KeyRelease) {
			int key	   = hmget(draw->platform->keymap, event.xkey.keycode);
			int retrig = 0;
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
		hmfree(platform->keymap);
	}
	/* TODO: How do I even free cursor? Do I even have to? */
	gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	free(platform);
}
