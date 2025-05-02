/* Engine */
#include <gf_version.h>

/* External library */

/* Standard */
#include <stdio.h>

int main(int argc, char** argv) {
	gf_version_t ver;
	gf_version_get(&ver);

	printf("GoldFish Engine %s\n", ver.full);
	printf("Build Date   : %s\n", ver.date);
	printf("Thread model : %s\n", ver.thread);
	printf("Renderer     : %s on %s\n", ver.driver, ver.backend);

	return 0;
}
