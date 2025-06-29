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
 * @file gf_input.h
 * @~english
 * @brief Input
 */

#ifndef __GF_INPUT_H__
#define __GF_INPUT_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/input.h>

/* Engine */
#include <gf_type/core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create input interface
 * @param engine Engine instance
 * @return Input interface
 */
GF_EXPORT gf_input_t* gf_input_create(gf_engine_t* engine);

/**
 * @~english
 * @brief Destroy input interface
 * @param input Input interface
 */
GF_EXPORT void gf_input_destroy(gf_input_t* input);

/**
 * @~english
 * @brief Handle key press event
 * @param input Input interface
 * @param key Key
 */
GF_EXPORT void gf_input_key_press(gf_input_t* input, int key);

/**
 * @~english
 * @brief Handle key release event
 * @param input Input interface
 * @param key Key
 */
GF_EXPORT void gf_input_key_release(gf_input_t* input, int key);

/**
 * @~english
 * @brief Check if key is down
 * @param input Input interface
 * @param key Key
 * @return `1` if key is down, `0` otherwise
 */
GF_EXPORT int gf_input_key_down(gf_input_t* input, int key);

/**
 * @~english
 * @brief Check if key got pressed
 * @param input Input interface
 * @param key Key
 * @return `1` if key got pressed, `0` otherwise
 */
GF_EXPORT int gf_input_key_pressed(gf_input_t* input, int key);

/**
 * @~english
 * @brief Check if key got released
 * @param input Input interface
 * @param key Key
 * @return `1` if key got released, `0` otherwise
 */
GF_EXPORT int gf_input_key_released(gf_input_t* input, int key);

/**
 * @~english
 * @brief Gets next pressed key
 * @param input Input interface
 * @return Key code of pressed key, otherwise -1
 */
GF_EXPORT int gf_input_get_pressed_key(gf_input_t* input);

/**
 * @~english
 * @brief Gets next released key
 * @param input Input interface
 * @return Key code of released key, otherwise -1
 */
GF_EXPORT int gf_input_get_released_key(gf_input_t* input);

/**
 * @~english
 * @brief Convert key to char
 * @param input Input interface
 * @return Non-`0` if key can be converted to ASCII and got pressed, `0` otherwise
 */
GF_EXPORT char gf_input_key_char(gf_input_t* input);

/**
 * @~english
 * @brief Convert key to char
 * @param input Input interface
 * @param key Key
 * @return Non-`0` if key can be converted to ASCII, `0` otherwise
 */
GF_EXPORT char gf_input_key_char_of(gf_input_t* input, int key);

/**
 * @~english
 * @brief Get name of key
 * @param key Key
 * @return String representation of key, if the key is valid, otherwise NULL
 */
GF_EXPORT const char* gf_input_key_name(int key);

/**
 * @~english
 * @brief Get key from name
 * @param name Key name
 * @return -1 if the key could not be found, otherwise GF_INPUT_KEY_*
 */
GF_EXPORT int gf_input_key_from_name(const char* name);

/**
 * @~english
 * @brief Binds a key to a command, or clears it
 * @param input Input interface
 * @param key Key
 * @param command Command to bind, if command is NULL, then the key is unbound
 */
GF_EXPORT void gf_input_bind_key(gf_input_t* input, int key, const char* command);

/**
 * @~english
 * @brief Returns the key id of the next bound key that has been mapped
 * @param input Input interface
 * @param last_key Pass last result of function to continue from that point, or -1 to search from start
 * @return -1 if there are no remaining bound keys, or non-`-1` for the next bound key
 */
GF_EXPORT int gf_input_next_bound_key(gf_input_t* input, int last_key);

/**
 * @~english
 * @brief Returns the command bound to the target key
 * @param input Input interface
 * @param key Key
 * @return Command that was bound to the key, or NULL if the key is unbound or invalid
 */
GF_EXPORT const char* gf_input_key_binding(gf_input_t* input, int key);

#ifdef __cplusplus
}
#endif

#endif
