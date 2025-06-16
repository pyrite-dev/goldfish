/**
 * @file gf_type/command.h
 * @~english
 * @brief Type definitions related to commands
 */

#ifndef __GF_TYPE_COMMAND_H__
#define __GF_TYPE_COMMAND_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_COMMAND

#define gf_command_alias_t struct gf_command_alias
gf_command_alias_t;

/**
 * @struct gf_command_alias
 * @~english
 * @brief Command alias
 *
 * @var gf_command_alias::key
 * @brief Command alias
 *
 * @var gf_command_alias::value
 * @brief Command to execute
 */
GF_DECLARE_TYPE(command_alias, {
	char* key;
	char* value;
});

#else
typedef void gf_command_alias_t;
#endif

#endif
