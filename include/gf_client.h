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
