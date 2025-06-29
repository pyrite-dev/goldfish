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
 * @file gf_gui_component.h
 * @~english
 * @brief GUI component stuff
 * @warning This file is auto-generated using ./tool/gengui.sh at `Sat Jun  7 22:11:55 JST 2025`
 */

#ifndef __GF_GUI_COMPONENT_H__
#define __GF_GUI_COMPONENT_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/gui.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create button component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_button(gf_gui_t* gui, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create entry component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_entry(gf_gui_t* gui, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create frame component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_frame(gf_gui_t* gui, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create progress component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_progress(gf_gui_t* gui, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create range component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_range(gf_gui_t* gui, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create scrollbar component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_scrollbar(gf_gui_t* gui, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create tab component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_tab(gf_gui_t* gui, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create text component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_text(gf_gui_t* gui, double x, double y, double w, double h);

/**
 * @~english
 * @brief Create window component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
GF_EXPORT gf_gui_id_t gf_gui_create_window(gf_gui_t* gui, double x, double y, double w, double h);

GF_EXPORT gf_gui_call_t gf_gui_calls[9];

#ifdef __cplusplus
}
#endif

#endif
