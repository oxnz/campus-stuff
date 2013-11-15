#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <regex.h>

#include "ffind.h"

static DIR *dirp = NULL;
static struct dirent *dp = NULL;
static regex_t *regexp = NULL;

#ifdef DEBUG
#define pregerr(fname) do {                     \
    if (ret) {                                  \
        char __1992[101] = {0};                       \
        regerror(ret, regexp, __1992, 100);                        \
        fprintf(stderr, "*** Error: [%s] in [%s]\n", __1992, fname);    \
    } } while (0);
#endif

int find_file_init(const char *path, const char *pattern) {
    if (dirp || regexp) return -1;
    if (pattern == NULL) pattern = ".*";
    dirp = opendir(path);
    if (dirp == NULL) return errno;
    regexp = malloc(sizeof(regex_t));
    if (regexp == NULL) return errno;
    int ret = regcomp(regexp, pattern, REG_EXTENDED|REG_NEWLINE);
#ifdef DEBUG
    pregerr(__FUNCTION__);
#endif
    return ret;
}

const char* find_first_file(void) {
    errno = 0;
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_type != DT_REG) continue; // skip non regular file
        regmatch_t pmatch[1];
        int ret = regexec(regexp, dp->d_name, 1, pmatch, 0);
        if (ret == REG_NOMATCH) { ret = 0; continue; }
#ifdef DEBUG
        pregerr(__FUNCTION__);
#endif
        if (ret) { errno = ret; return NULL; }
        return dp->d_name;
    }
    return NULL;
}

const char *find_next_file() {
    if (dirp == NULL || dp == NULL || regexp == NULL) return NULL;
    errno = 0;
    while ((dp = readdir(dirp)) != NULL) {
        printf(">>%s\n", dp->d_name);
        if (dp->d_type != DT_REG) continue;

        regmatch_t pmatch[1];
        int ret = regexec(regexp, dp->d_name, 1, pmatch, 0);
        if (ret == REG_NOMATCH) { ret = 0; continue; }
        printf("ret=%d\n", ret);
#ifdef DEBUG
        pregerr(__FUNCTION__);
#endif
        if (ret) { errno = ret; return NULL; }
        return dp->d_name;
    }
    if (dp == NULL) printf("END\n");
    return NULL;
}

int find_file_finish(void) {
    if (dirp)
        closedir(dirp);
    dirp = NULL;
    dp = NULL;
    if (regexp)
        regfree(regexp);
    regexp = NULL;
    return 0;
}
