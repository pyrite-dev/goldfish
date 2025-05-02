/* Engine */
#include <gf_version.h>
#include <gf_resource.h>
#include <gf_log.h>

/* External library */

/* Standard */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#endif

char* out  = NULL;
char* base = ".";

int count = 0;

int add_all(gf_resource_t* resource, char* path) {
#ifdef _WIN32
	WIN32_FIND_DATA ffd;
	HANDLE		hfind;
#else
	DIR*	       dir;
	struct dirent* d;
#endif
	int   st = 0;
	char* op = malloc(strlen(base) + 1 + strlen(path) + 32 + 1);
	op[0]	 = 0;
	strcat(op, base);
	strcat(op, "/");
	strcat(op, path);

#ifdef _WIN32
	strcat(op, "/*");
	hfind = FindFirstFile(op, &ffd);
	if(hfind == INVALID_HANDLE_VALUE) {
		free(op);
		fprintf(stderr, "Could not open directory\n");
		return -1;
	}
	do {
		if(strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0) {
			char* p	 = malloc(strlen(base) + 1 + strlen(path) + strlen(ffd.cFileName) + 1);
			char* np = malloc(strlen(path) + strlen(ffd.cFileName) + 1 + 1);
			p[0]	 = 0;
			strcat(p, base);
			strcat(p, "/");
			strcat(p, path);
			strcat(p, ffd.cFileName);

			np[0] = 0;
			strcat(np, path);
			strcat(np, ffd.cFileName);

			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				strcat(np, "/");
				if((st = add_all(resource, np)) != 0) {
					free(np);
					break;
				}
			} else {
				FILE* f = fopen(p, "rb");
				if(f != NULL) {
					unsigned int   sz;
					unsigned char* data;
					fseek(f, 0, SEEK_END);
					sz = ftell(f);
					fseek(f, 0, SEEK_SET);

					data = malloc(sz);
					fread(data, sz, 1, f);
					fclose(f);

					gf_resource_add(resource, np, data, sz);

					free(data);
					count++;
				}
			}
			free(np);

			free(p);
		}
	} while(FindNextFile(hfind, &ffd) != 0);
	FindClose(hfind);
#else
	dir = opendir(op);
	if(dir == NULL) {
		free(op);
		fprintf(stderr, "Could not open directory\n");
		return -1;
	}
	while((d = readdir(dir)) != NULL) {
		if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
			char*	    p  = malloc(strlen(base) + 1 + strlen(path) + strlen(d->d_name) + 1);
			char*	    np = malloc(strlen(path) + strlen(d->d_name) + 1 + 1);
			struct stat s;
			p[0] = 0;
			strcat(p, base);
			strcat(p, "/");
			strcat(p, path);
			strcat(p, d->d_name);

			np[0] = 0;
			strcat(np, path);
			strcat(np, d->d_name);

			stat(p, &s);

			if(S_ISDIR(s.st_mode)) {
				strcat(np, "/");
				if((st = add_all(resource, np)) != 0) {
					free(np);
					break;
				}
			} else {
				FILE* f = fopen(p, "rb");
				if(f != NULL) {
					unsigned int   sz;
					unsigned char* data;
					fseek(f, 0, SEEK_END);
					sz = ftell(f);
					fseek(f, 0, SEEK_SET);

					data = malloc(sz);
					fread(data, sz, 1, f);
					fclose(f);

					gf_resource_add(resource, np, data, sz);

					free(data);

					count++;
				}
			}
			free(np);

			free(p);
		}
	}
	closedir(dir);
#endif
	free(op);
	return st;
}

int main(int argc, char** argv) {
	gf_version_t   version;
	gf_resource_t* resource;
	int	       i;
	int	       st    = 0;
	char**	       start = NULL;

	gf_log_default = NULL;

	gf_version_get(&version);
	printf("GoldFish Engine Resource Packer %s\n", version.full);
	for(i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "-d") == 0) {
				base = argv[++i];
			} else {
				fprintf(stderr, "Invalid flag: %s\n", argv[i]);
				return 1;
			}
		} else {
			if(out != NULL) {
				start = &argv[i];
				break;
			} else {
				out = argv[i];
			}
		}
	}

	if(out == NULL || base == NULL) {
		fprintf(stderr, "Usage: %s [-d basedir] output\n", argv[0]);
		return 1;
	}

	resource = gf_resource_create(NULL, NULL);
	if(resource != NULL) {
		if(start == NULL) {
			if(add_all(resource, "") != 0) {
				st = 1;
			}
		} else {
			for(i = 0; start[i] != NULL; i++) {
				char* cp = malloc(strlen(start[i]) + 1 + 1);
				cp[0]	 = 0;
				strcat(cp, start[i]);
				strcat(cp, "/");
				if(add_all(resource, cp) != 0) {
					free(cp);
					st = 1;
					break;
				}
				free(cp);
			}
		}
		printf("Found %d files\n", count);
		gf_resource_write(resource, out, 1);
		gf_resource_destroy(resource);
	}

	return st;
}
