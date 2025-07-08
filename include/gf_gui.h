/**
 * @if SHOW_LICENSE
 **********************************************************************************
 *
 * Copyright (c) 2025 Pyrite Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgement:
 *    This product includes software developed by the Pyrite Development Team.
 *
 * 4. Neither the name of Pyrite Development Team nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************
 * @endif
 */

/**
 * @file gf_gui.h
 * @~english
 * @brief GUI
 */

#ifndef __GF_GUI_H__
#define __GF_GUI_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/gui.h>

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/core.h>
#include <gf_type/prop.h>
#include <gf_gui_component.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Draw border normally
 */
#define GF_GUI_NORMAL 1

/**
 * @~english
 * @brief Draw border inverted
 */
#define GF_GUI_INVERT -1

/**
 * @~english
 * @brief Default GUI font size
 */
#define GF_GUI_FONT_SIZE 24

/**
 * @~english
 * @brief Default GUI small font size
 */
#define GF_GUI_SMALL_FONT_SIZE 14

/**
 * @~english
 * @brief Create GUI
 * @param engine Engine instance
 * @param draw Drawing interface
 * @return GUI
 */
GF_EXPORT gf_gui_t* GFAPI gf_gui_create(gf_engine_t* engine, gf_draw_t* draw);

/**
 * @~english
 * @brief Destroy GUI
 * @param gui GUI
 */
GF_EXPORT void GFAPI gf_gui_destroy(gf_gui_t* gui);

/**
 * @~english
 * @brief Destroy GUI component by ID
 * @param gui GUI
 * @param id Component ID
 */
GF_EXPORT void GFAPI gf_gui_destroy_id(gf_gui_t* gui, gf_gui_id_t id);

/**
 * @~english
 * @brief Set default button sound
 * @param gui GUI
 * @param path Path to sound
 */
GF_EXPORT void GFAPI gf_gui_set_button_sound(gf_gui_t* gui, const char* path);

/**
 * @~english
 * @brief Get index of GUI component
 * @param gui GUI
 * @param id Component ID
 * @return Index
 */
GF_EXPORT int GFAPI gf_gui_get_index(gf_gui_t* gui, gf_gui_id_t id);

/**
 * @~english
 * @brief Render GUI
 * @param gui GUI
 */
GF_EXPORT void GFAPI gf_gui_render(gf_gui_t* gui);

/**
 * @~english
 * @brief Render box
 * @param gui GUI
 * @param mul GF_GUI_NORMAL or GF_GUI_INVERT
 * @param x X coord of box
 * @param y Y coord of box
 * @param w Width of box
 * @param h Height of box
 */
GF_EXPORT void GFAPI gf_gui_draw_box(gf_gui_t* gui, int mul, double x, double y, double w, double h);

/**
 * @~english
 * @brief Set callback
 * @param gui GUI
 * @param id Component ID
 * @param callback Callback
 */
GF_EXPORT void GFAPI gf_gui_set_callback(gf_gui_t* gui, gf_gui_id_t id, gf_gui_callback_t callback);

/**
 * @~english
 * @brief Set parent component
 * @param gui GUI
 * @param id Component ID
 * @param parent Parent component ID
 */
GF_EXPORT void GFAPI gf_gui_set_parent(gf_gui_t* gui, gf_gui_id_t id, gf_gui_id_t parent);

/**
 * @~english
 * @brief Get parent component
 * @param gui GUI
 * @param id Component ID
 * @return Parent component ID
 */
GF_EXPORT gf_gui_id_t GFAPI gf_gui_get_parent(gf_gui_t* gui, gf_gui_id_t id);

/**
 * @~english
 * @brief Set text
 * @param gui GUI
 * @param id Component ID
 * @param text Text
 */
GF_EXPORT void GFAPI gf_gui_set_text(gf_gui_t* gui, gf_gui_id_t id, const char* text);

/**
 * @~english
 * @brief Get text
 * @param gui GUI
 * @param id Component ID
 * @return Text
 */
GF_EXPORT const char* GFAPI gf_gui_get_text(gf_gui_t* gui, gf_gui_id_t id);

/**
 * @~english
 * @brief Set font color
 * @param gui GUI
 * @param id Component ID
 * @param color Font color
 */
GF_EXPORT void GFAPI gf_gui_set_font_color(gf_gui_t* gui, gf_gui_id_t id, gf_graphic_color_t color);

/**
 * @~english
 * @brief Get font color
 * @param gui GUI
 * @param id Component ID
 * @return Font color
 */
GF_EXPORT gf_graphic_color_t GFAPI gf_gui_get_font_color(gf_gui_t* gui, gf_gui_id_t id);

/**
 * @~english
 * @brief Set hover font color
 * @param gui GUI
 * @param id Component ID
 * @param color Hover font color
 */
GF_EXPORT void GFAPI gf_gui_set_hover_font_color(gf_gui_t* gui, gf_gui_id_t id, gf_graphic_color_t color);

/**
 * @~english
 * @brief Get hover font color
 * @param gui GUI
 * @param id Component ID
 * @return Hover font color
 */
GF_EXPORT gf_graphic_color_t GFAPI gf_gui_get_hover_font_color(gf_gui_t* gui, gf_gui_id_t id);

/**
 * @~english
 * @brief Sort components
 * @param gui GUI
 */
GF_EXPORT void GFAPI gf_gui_sort_component(gf_gui_t* gui);

/**
 * @~english
 * @brief Move component topmost
 * @param gui GUI
 * @param id Component ID
 */
GF_EXPORT void GFAPI gf_gui_move_topmost(gf_gui_t* gui, gf_gui_id_t id);

/**
 * @~english
 * @brief Initialize component
 * @param gui GUI
 * @param c Component
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 */
GF_EXPORT void GFAPI gf_gui_create_component(gf_gui_t* gui, gf_gui_component_t* c, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create component specified by name
 * @param gui GUI
 * @param name Component name
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 */
GF_EXPORT gf_gui_id_t GFAPI gf_gui_create_common(gf_gui_t* gui, const char* name, double x, double y, double w, double h);

/**
 * @~english
 * @brief Calculate component X/Y/width/height
 * @param gui GUI
 * @param c Component
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 */
GF_EXPORT void GFAPI gf_gui_calc_xywh(gf_gui_t* gui, gf_gui_component_t* c, double* x, double* y, double* w, double* h);

/**
 * @~english
 * @brief Set component ID to property
 * @param gui GUI
 * @param id Component ID
 * @param key Key
 * @param id2 Component ID
 */
GF_EXPORT void GFAPI gf_gui_set_prop_id(gf_gui_t* gui, gf_gui_id_t id, const char* key, gf_gui_id_t id2);

/**
 * @~english
 * @brief Get component ID from property
 * @param gui GUI
 * @param id Component ID
 * @param key Key
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t GFAPI gf_gui_get_prop_id(gf_gui_t* gui, gf_gui_id_t id, const char* key);

/**
 * @~english
 * @brief Get property container
 * @param gui GUI
 * @param id Component ID
 * @return Property
 */
GF_EXPORT gf_prop_t** GFAPI gf_gui_get_prop(gf_gui_t* gui, gf_gui_id_t id);

/**
 * @~english
 * @brief Set width and height
 * @param gui GUI
 * @param id Component ID
 * @param w Width
 * @param h Height
 */
GF_EXPORT void GFAPI gf_gui_set_wh(gf_gui_t* gui, gf_gui_id_t id, double w, double h);

/**
 * @~english
 * @brief Get width and height
 * @param gui GUI
 * @param id Component ID
 * @param w Width
 * @param h Height
 */
GF_EXPORT void GFAPI gf_gui_get_wh(gf_gui_t* gui, gf_gui_id_t id, double* w, double* h);

/**
 * @~english
 * @brief Set X coord and Y coord
 * @param gui GUI
 * @param id Component ID
 * @param x X coord
 * @param y Y coord
 */
GF_EXPORT void GFAPI gf_gui_set_xy(gf_gui_t* gui, gf_gui_id_t id, double x, double y);

/**
 * @~english
 * @brief Get X coord and Y coord
 * @param gui GUI
 * @param id Component ID
 * @param x X coord
 * @param y Y coord
 */
GF_EXPORT void GFAPI gf_gui_get_xy(gf_gui_t* gui, gf_gui_id_t id, double* x, double* y);

#ifdef __cplusplus
}
#endif

#endif
