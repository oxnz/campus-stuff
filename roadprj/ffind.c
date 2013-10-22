#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <regex.h>

#include "ffind.h"

static DIR *dirp;
static regex_t regex;

int find_file_init(const char *path, const char *regexp) {
    dirp = opendir(path);
    if (dirp == NULL) return -1;
 
    return 0;
}

const char* find_first_file(void) {
    struct dirent *dp;
    errno = 0;
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_namlen < 8) {
            fprintf(stderr, "too short name : %s, skipped\n", dp->d_name);
            continue;
        }
        if (dp->d_type != DT_REG) {
            fprintf(stderr, "non regular file dected: %s, skipped\n", dp->d_name);
            continue;
        }
        printf("file: %s\n", dp->d_name);
        return dp->d_name;
    }
    if (errno != 0) {
        perror("error reading directory");
    }
    (void) closedir(dirp);
    dirp = 0;
    dp = 0;
    return NULL;
}

const char *find_next_file() {
    if (dirp == NULL) {
        fprintf(stderr, "please call get_first_file first to open the directory\n");
        return NULL;
    }
    struct dirent *dp;
    errno = 0;
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_namlen < 8) {
            fprintf(stderr, "too short name: %s, skipped\n", dp->d_name);
            continue;
        }
        if (dp->d_type != DT_REG) {
            fprintf(stderr, "non regular file: %s, skipped\n", dp->d_name);
            continue;
        }
        printf("file: %s\n", dp->d_name);
        return dp->d_name;
    }
    if (errno != 0) {
        perror("error reading directory");
    }
    closedir(dirp);
    dp = 0;
    dirp = 0;
    return NULL;
}
