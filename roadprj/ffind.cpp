/*
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "ffind.h"

static DIR *dirp;
static const char *regexp;

int find_file_init(const char *path, const char *pattern) {
    dirp = opendir(path);
    if (dirp == NULL) return -1;
    regexp = pattern;
    return 0;
}

int find_first_file(const char **pfname) {
    struct dirent *dp;
    errno = 0;
    while ((dp = readdir(dirp)) != NULL) {
        if (!strstr(dp->d_name, regexp)) continue;
        *pfname = dp->d_name;
        return 1;
    }
    return errno == 0 ? 0 : -1;
}

int find_next_file(const char **pfname) {
    if (dirp == NULL) {
        fprintf(stderr, "please call get_first_file first to open the directory\n");
        return -1;
    }
    struct dirent *dp;
    errno = 0;
    while ((dp = readdir(dirp)) != NULL) {
        if (!strstr(dp->d_name, regexp)) continue;
        *pfname = dp->d_name;
        return 1;
    }
    return errno == 0 ? 0 : -1;
}

int find_file_finish(void) {
    if (dirp) closedir(dirp);
    dirp = NULL;
    regexp = NULL;
    return 0;
}

*/