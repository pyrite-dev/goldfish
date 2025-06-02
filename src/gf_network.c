#define GF_EXPOSE_NETWORK

#include <gf_pre.h>

/* External library */
#include <gf_netdrv.h>

/* Interface */
#include <gf_network.h>

/* Engine */
#include <gf_type/compat.h>

/* Standard */

gf_uint32_t gf_network_id(const char* str) {
	gf_uint32_t id = *(gf_uint32_t*)&str[0];
	return htonl(id);
}
