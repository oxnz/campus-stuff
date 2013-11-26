#pragma once

#ifdef __cpluscplus
extern "C" {
	int find_file_init(const char *path, const char *pattern);
	const char *find_first_file(void);
	const char *find_next_file(void);
	int find_file_finish(void);
	const char *find_file(const char *path, const char *pattern);
#define DEBUG

#ifdef __cplusplus
}
#endif
