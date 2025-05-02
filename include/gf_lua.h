/**
 * @file gf_lua.h
 * @~english
 * @brief Lua
 */

#ifndef __GF_LUA_H__
#define __GF_LUA_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/lua.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/gui.h>
#include <gf_type/font.h>
#include <gf_type/audio.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create GoldFish GUI Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_create_goldfish_gui(gf_lua_t* lua);

/**
 * @~english
 * @brief Create GoldFish Graphic Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_create_goldfish_graphic(gf_lua_t* lua);

/**
 * @~english
 * @brief Create GoldFish Font Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_create_goldfish_font(gf_lua_t* lua);

/**
 * @~english
 * @brief Create GoldFish Audio Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_create_goldfish_audio(gf_lua_t* lua);

/**
 * @~english
 * @brief Initialize GUI metatables
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_meta_init_gui(gf_lua_t* lua);

/**
 * @~english
 * @brief Initialize Font metatables
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_meta_init_font(gf_lua_t* lua);

/**
 * @~english
 * @brief Initialize Audio metatables
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_meta_init_audio(gf_lua_t* lua);

/**
 * @~english
 * @brief Create GUI userdata
 * @param lua Lua interface
 */
GF_EXPORT gf_gui_id_t* gf_lua_create_gui(gf_lua_t* lua);

/**
 * @~english
 * @brief Create Font userdata
 * @param lua Lua interface
 */
GF_EXPORT gf_font_t** gf_lua_create_font(gf_lua_t* lua);

/**
 * @~english
 * @brief Create Audio userdata
 * @param lua Lua interface
 */
GF_EXPORT gf_audio_id_t* gf_lua_create_audio(gf_lua_t* lua);

/**
 * @~english
 * @brief Initialize metatables
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_meta_init(gf_lua_t* lua);

/**
 * @~english
 * @brief Create Lua interface
 * @param engine Engine instance
 * @return Lua interface
 */
GF_EXPORT gf_lua_t* gf_lua_create(gf_engine_t* engine);

/**
 * @~english
 * @brief Run Lua program
 * @param lua Lua interface
 * @param path Path
 * @return `0` if successful, otherwise `-1`
 */
GF_EXPORT int gf_lua_run(gf_lua_t* lua, const char* path);

/**
 * @~english
 * @brief Destroy Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_destroy(gf_lua_t* lua);

/**
 * @~english
 * @brief Run a single step for Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_step(gf_lua_t* lua);

/**
 * @~english
 * @brief Run close hook for Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_close(gf_lua_t* lua);

#ifdef __cplusplus
}
#endif

#endif
