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
 * @file gf_audio.h
 * @~english
 * @brief Audio interface
 */

#ifndef __GF_AUDIO_H__
#define __GF_AUDIO_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/audio.h>

/* Engine */
#include <gf_type/core.h>

/* Standard */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Audio callback
 * @param audio Audio interface
 * @param output Output buffer
 * @param frame Frame sound driver wants
 */
GF_EXPORT void gf_audio_callback(gf_audio_t* audio, void* output, int frame);

/**
 * @~english
 * @brief Create audio interface
 * @param engine Engine instance
 * @return Audio interface
 */
GF_EXPORT gf_audio_t* gf_audio_create(gf_engine_t* engine);

/**
 * @~english
 * @brief Destroy audio interface
 * @param audio Audio interface
 */
GF_EXPORT void gf_audio_destroy(gf_audio_t* audio);

/**
 * @~english
 * @brief Destroy audio decoder
 * @param decoder Audio decoder
 */
GF_EXPORT void gf_audio_decoder_destroy(gf_audio_decoder_t* decoder);

/**
 * @~english
 * @brief Enable auto-destroy
 * @param audio Audio interface
 * @param id Audio ID
 */
GF_EXPORT void gf_audio_auto_destroy(gf_audio_t* audio, gf_audio_id_t id);

/**
 * @~english
 * @brief Load and play file
 * @param audio Audio interface
 * @param path Path
 * @return ID if successful, otherwise `-1`
 */
GF_EXPORT gf_audio_id_t gf_audio_load_file(gf_audio_t* audio, const char* path);

/**
 * @~english
 * @brief Load and play data
 * @param audio Audio interface
 * @param data Data
 * @param size Data size
 * @return ID if successful, otherwise `-1`
 */
GF_EXPORT gf_audio_id_t gf_audio_load(gf_audio_t* audio, const void* data, size_t size);

/**
 * @~english
 * @brief Pause audio
 * @param audio Audio interface
 * @param id Audio ID
 */
GF_EXPORT void gf_audio_pause(gf_audio_t* audio, gf_audio_id_t id);

/**
 * @~english
 * @brief Resume audio
 * @param audio Audio interface
 * @param id Audio ID
 */
GF_EXPORT void gf_audio_resume(gf_audio_t* audio, gf_audio_id_t id);

/**
 * @~english
 * @brief Stop audio
 * @param audio Audio interface
 * @param id Audio ID
 * @note This frees the decoder
 */
GF_EXPORT void gf_audio_stop(gf_audio_t* audio, gf_audio_id_t id);

/**
 * @~english
 * @brief Set volume
 * @param audio Audio interface
 * @param volume Volume
 */
GF_EXPORT void gf_audio_set_volume(gf_audio_t* audio, double volume);

/**
 * @~english
 * @brief Get volume
 * @param audio Audio interface
 * @return Volume
 */
GF_EXPORT double gf_audio_get_volume(gf_audio_t* audio);

/**
 * @~english
 * @brief Check if audio is over
 * @param audio Audio interface
 * @param id Audio ID
 * @return `1` if it is over, otherwise `0`
 */
GF_EXPORT int gf_audio_is_over(gf_audio_t* audio, gf_audio_id_t id);

#ifdef __cplusplus
}
#endif

#endif
