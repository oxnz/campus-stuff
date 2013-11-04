#ifndef FFIND_H
#define FFIND_H

int find_file_init(const char *path, const char *pattern);
/*
 * @description: find file specified path that matches pattern
 *
 * @parameters:
 * ppfname: pointer point to const char *
 * @returns:
 * 1 stands for found, 0 for end, and negtive for error
 */
int find_first_file(const char **ppfname);
int find_next_file(const char **ppfname);

#endif
