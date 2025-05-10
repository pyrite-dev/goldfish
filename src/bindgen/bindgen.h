#ifndef __BINDGEN_H__
#define __BINDGEN_H__
#ifdef __cplusplus
extern "C" {
#endif
/**
 * Auto-generated using bindgen (https://github.com/pyrite-dev/bindgen)
 * Do not modify this directly! Might get overwritten.
 */

#include <bindgen.h>
#include <lua.h>
#include <lauxlib.h>

#ifndef LUA
#define LUA(wrap) (wrap->lua)
#endif

/*** <Pre> ***/

#ifndef GF_EXPOSE_ALL
#define GF_EXPOSE_ALL
#endif
#include <gf_type/lua.h>

/*** </Pre> ***/

void bindgen_GoldFishGUIComponent_init(gf_lua_t* lua);
void bindgen_GoldFishAudio_init(gf_lua_t* lua);
void bindgen_GoldFishFont_init(gf_lua_t* lua);
void bindgen_gui_init(gf_lua_t* lua);
void bindgen_audio_init(gf_lua_t* lua);
void bindgen_font_init(gf_lua_t* lua);
void bindgen_graphic_init(gf_lua_t* lua);
void bindgen_init(gf_lua_t* lua);

#ifdef __cplusplus
}
#endif

/*** Generated at Sat May 10 2025 ***/
#endif
