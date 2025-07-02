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

void bindgen_init(gf_lua_t* lua) {
	lua_pushlightuserdata(LUA(lua), lua);
	lua_setglobal(LUA(lua), "_LUA_WRAP");
	bindgen_GoldFishGUIComponent_init(lua);
	bindgen_GoldFishAudio_init(lua);
	bindgen_GoldFishTexture_init(lua);
	bindgen_GoldFishFont_init(lua);

	lua_newtable(LUA(lua));
	bindgen_gui_init(lua);
	bindgen_audio_init(lua);
	bindgen_font_init(lua);
	bindgen_graphic_init(lua);
	lua_setglobal(LUA(lua), "goldfish");
}

/*** Generated at Wed Jul 2 2025 ***/
