#ifdef __GNUC__
#if __GNUC__ <= 3
#define NO_SVG
#endif
#endif

#ifndef NO_SVG
#define NANOSVG_IMPLEMENTATION
#define NANOSVGRAST_IMPLEMENTATION
#include <nanosvg.h>
#include <nanosvgrast.h>
#endif
