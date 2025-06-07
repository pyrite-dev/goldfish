/**
 * @file gf_command.h
 * @~english
 * @brief Command
 */

#ifndef __GF_COMMAND_H__
#define __GF_COMMAND_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Run command
 * @param engine Engine instance
 * @param path Path
 */
GF_EXPORT void gf_command_file(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Run command
 * @param engine Engine instance
 * @param list Command list
 * @param listc Command list length
 */
GF_EXPORT void gf_command_run(gf_engine_t* engine, char** list, int listc);

#ifdef __cplusplus
}
#endif

#endif
