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
 * File Name:    gf_prop.c
 * Author:       Nishi
 *
 * Functionality:
 * TODO: Write here
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_PROP

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_prop.h>

/* Engine */

/* Standard */
#include <stdlib.h>
#include <string.h>

void gf_prop_set_text(gf_prop_t** prop, const char* key, const char* value) {
	void* ptr = malloc(strlen(value) + 1);
	strcpy(ptr, value);
	gf_prop_set_ptr(prop, key, ptr);
}

const char* gf_prop_get_text(gf_prop_t** prop, const char* key) { return (const char*)gf_prop_get_ptr(prop, key); }

void gf_prop_set_integer(gf_prop_t** prop, const char* key, gf_prop_integer_t value) {
	void* ptr = malloc(sizeof(value));
	memcpy(ptr, &value, sizeof(value));
	gf_prop_set_ptr(prop, key, ptr);
}

gf_prop_integer_t gf_prop_get_integer(gf_prop_t** prop, const char* key) {
	gf_prop_integer_t* r = gf_prop_get_ptr(prop, key);
	if(r == NULL) return GF_PROP_NO_SUCH;
	return *r;
}

void gf_prop_set_floating(gf_prop_t** prop, const char* key, gf_prop_floating_t value) {
	void* ptr = malloc(sizeof(value));
	memcpy(ptr, &value, sizeof(value));
	gf_prop_set_ptr(prop, key, ptr);
}

gf_prop_floating_t gf_prop_get_floating(gf_prop_t** prop, const char* key) {
	gf_prop_floating_t* r = gf_prop_get_ptr(prop, key);
	if(r == NULL) return GF_PROP_NO_SUCH;
	return *r;
}

void gf_prop_set_ptr(gf_prop_t** prop, const char* key, void* value) {
	int ind;

	gf_prop_delete(prop, key);
	shput(*prop, (char*)key, value);
	ind		  = shgeti(*prop, (char*)key);
	(*prop)[ind].keep = NULL;
}

void* gf_prop_get_ptr(gf_prop_t** prop, const char* key) {
	int ind = shgeti(*prop, (char*)key);
	if(ind == -1) return NULL;
	return (*prop)[ind].value;
}

void gf_prop_set_ptr_keep(gf_prop_t** prop, const char* key, void* value) {
	int ind;

	gf_prop_delete(prop, key);
	shput(*prop, (char*)key, NULL);
	ind		  = shgeti(*prop, (char*)key);
	(*prop)[ind].keep = value;
}

void* gf_prop_get_ptr_keep(gf_prop_t** prop, const char* key) {
	int ind = shgeti(*prop, (char*)key);
	if(ind == -1) return NULL;
	return (*prop)[ind].keep;
}

void gf_prop_delete(gf_prop_t** prop, const char* key) {
	int ind;
	ind = shgeti(*prop, (char*)key);
	if(ind == -1) return;

	if((*prop)[ind].value != NULL) {
		free((*prop)[ind].value);
	}

	shdel(*prop, key);
}

void gf_prop_destroy(gf_prop_t** prop) {
	int i;
	for(i = 0; i < shlen(*prop); i++) {
		if((*prop)[i].value != NULL) {
			free((*prop)[i].value);
		}
	}
	*prop = NULL;
}

void gf_prop_create(gf_prop_t** prop) {
	*prop = NULL;
	sh_new_strdup(*prop);
}
