/**
 * @file gf_client.h
 * @~english
 * @brief Client interface
 */

#ifndef __GF_CLIENT_H__
#define __GF_CLIENT_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/client.h>

/* Engine */
#include <gf_type/core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Initialize client interface
 */
GF_EXPORT void gf_client_begin(void);

/**
 * @~english
 * @brief Cleanup client interface
 */
GF_EXPORT void gf_client_end(void);

/**
 * @~english
 * @brief Create client interface
 * @param engine Engine instance
 * @param title Window title
 * @return Client interface
 */
GF_EXPORT gf_client_t* gf_client_create(gf_engine_t* engine, const char* title);

/**
 * @~english
 * @brief Destroy client interface
 * @param client Client interface
 */
GF_EXPORT void gf_client_destroy(gf_client_t* client);

/**
 * @~english
 * @brief Do client single step
 * @param client Client interface
 * @return `0` if successful, otherwise if failed
 */
GF_EXPORT int gf_client_step(gf_client_t* client);

/**
 * @~english
 * @brief Start client shutdown
 * @param client Client interface
 */
GF_EXPORT void gf_client_shutdown(gf_client_t* client);

#ifdef __cplusplus
}
#endif

#endif
