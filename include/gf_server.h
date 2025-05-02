/**
 * @file gf_server.h
 * @~english
 * @brief Client interface
 */

#ifndef __GF_SERVER_H__
#define __GF_SERVER_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/server.h>

/* Engine */
#include <gf_type/core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Initialize server interface
 */
GF_EXPORT void gf_server_begin(void);

/**
 * @~english
 * @brief Cleanup server interface
 */
GF_EXPORT void gf_server_end(void);

/**
 * @~english
 * @brief Create server interface
 * @param engine Engine instance
 * @return Server interface
 */
GF_EXPORT gf_server_t* gf_server_create(gf_engine_t* engine);

/**
 * @~english
 * @brief Destroy server interface
 * @param server Server interface
 */
GF_EXPORT void gf_server_destroy(gf_server_t* server);

/**
 * @~english
 * @brief Start server shutdown
 * @param server Server interface
 */
GF_EXPORT void gf_server_shutdown(gf_server_t* server);

#ifdef __cplusplus
}
#endif

#endif
