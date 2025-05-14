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
