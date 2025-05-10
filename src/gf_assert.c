#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#ifdef _WIN32
#include <windows.h>
#endif

/* Interface */
#include <gf_assert.h>

/* Engine */

/* Standard */
#include <stdlib.h>
#include <string.h>

int gf_assert_body(gf_engine_t* engine, const char* expr, const char* filename, int line, const char* funcname) {
	char* msg = malloc(4096);
	msg[0]	  = 0;

	sprintf(msg + strlen(msg), "GoldFish Assertion Error!\n");
	sprintf(msg + strlen(msg), "Expression: %s\n", expr);
	sprintf(msg + strlen(msg), "File: %s:%d\n", filename, line);
	sprintf(msg + strlen(msg), "Function Name: %s\n", funcname);
	sprintf(msg + strlen(msg), "\n");
	sprintf(msg + strlen(msg), "If you are the game developer, you fucked up something.\n");
	sprintf(msg + strlen(msg), "If you are a player, simply report this to the game developer.\n");
#ifdef _WIN32
	MessageBox(NULL, msg, "GoldFish Assertion Error", MB_OK | MB_ICONERROR);
#else
	fprintf(stderr, "----- Begin GoldFish Assertion Error -----\n");
	fprintf(stderr, "%s", msg);
	fprintf(stderr, "-----  End  GoldFish Assertion Error -----\n");
#endif
	free(msg);
	if(engine) engine->error = 1;

	return 1;
}
