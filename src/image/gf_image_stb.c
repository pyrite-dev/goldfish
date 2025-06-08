#define STB_IMAGE_IMPLEMENTATION

#ifdef __GNUC__
#if __GNUC__ <= 3
#define STBI_NO_THREAD_LOCALS
#endif
#endif

#include <stb_image.h>
