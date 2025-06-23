#include <gf_pre.h>

/* External library */
#include <lua.h>
#include <zlib.h>
#include <pcre.h>

/* Interface */
#include <gf_version.h>

/* Engine */

/* Standard */
#include <string.h>
#include <stdlib.h>

#define GF_VERSION "1.0.0"

void gf_version_get(gf_version_t* version) {
	char* cpstr = malloc(512);
	int   i;
	int   incr = 0;
	int   old  = 0;

	strcpy(version->date, __DATE__);

	strcpy(version->full, GF_VERSION);
#ifdef DEBUG
	strcat(version->full, "-debug");
#else
	strcat(version->full, "-release");
#endif

	strcpy(version->zlib, ZLIB_VERSION);

	strcpy(cpstr, LUA_RELEASE);
	strcpy(version->lua, cpstr + 4);

	strcpy(version->pcre, pcre_version());

#if defined(GF_DRV_OPENGL)
	strcpy(version->driver, "OpenGL");
#if defined(GF_TYPE_OSMESA)
	strcat(version->driver, " (OSMesa)");
#elif defined(GF_TYPE_NATIVE)
	strcat(version->driver, " (Native)");
#else
	strcat(version->driver, " (Unknown)");
#endif

#if defined(GF_USE_X11)
	strcpy(version->backend, "X11");
#elif defined(GF_USE_GDI)
	strcpy(version->backend, "GDI");
#elif defined(GF_USE_RGFW)
	strcpy(version->backend, "RGFW");
#elif defined(GF_USE_AGL)
	strcpy(version->backend, "AGL");
#endif
#endif

#if defined(GF_THREAD_WIN32)
	strcpy(version->thread, "Win32");
#elif defined(GF_THREAD_POSIX)
	strcpy(version->thread, "POSIX");
#endif

	strcpy(cpstr, GF_VERSION);
	for(i = 0;; i++) {
		if(cpstr[i] == '.' || cpstr[i] == 0) {
			int num;
			cpstr[i] = 0;
			num	 = atoi(cpstr + old);
			if(incr == 0) {
				version->major = num;
			} else if(incr == 1) {
				version->minor = num;
			} else if(incr == 2) {
				version->patch = num;
			}
			old = i + 1;
			incr++;
			if(incr == 3) break;
		}
	}
}
