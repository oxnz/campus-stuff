#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>


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


#include "RHelper.h"
#include <iostream>
#include <algorithm>
using namespace std;

int find_files(const char *path, const char* pattern, list<string>& olist) {
	string fpath(path);
	if (find_file_init(path, pattern)) {
		cerr << "open directory init failed" << endl;
		return -1;
	}
	const char* fname;
	int cnt = 0;
	if (find_first_file(&fname)) {
		olist.push_back(fpath + "/" + fname);
		++cnt;
	}
	while (find_next_file(&fname)) {
		++cnt;
		olist.push_back(fpath + "/" + fname);
	}
	if (find_file_finish()) {
		cerr << "find file finish failed" << endl;
	}
    olist.sort();
    return cnt;
}

int find_files(const char* path, const char* pattern, vector<string>& ovec) {
    string fpath(path);
    if (find_file_init(path, pattern)) {
        cerr << "open directory failed!" << endl;
        return -1;
    }
    const char* fname;
    int cnt = 0;
    if (find_first_file(&fname)) {
        ovec.push_back(fpath + "/" + fname);
        ++cnt;
    }
    while (find_next_file(&fname)) {
        ++cnt;
        ovec.push_back(fpath + "/" + fname);
    }
    if (find_file_finish()) {
        cerr << "find file finish failed!" << endl;
    }
    sort(ovec.begin(), ovec.end());
    return cnt;
}
        
