/*
 * File: RHelper.cpp
 * Description: helper source file, supplies services
 * Author: Oxnz
 * Version: 1.1
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12 CST
 */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

static DIR *dirp;
static const char *regexp;

int find_file_init(const char *path, const char *pattern) {
    dirp = opendir(path);
    if (dirp == NULL) return errno;
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
#include "NZLogger.h"

#include <iostream>
#include <algorithm>

using namespace std;
using NZ::NZLogger;

int RHelper::find_files(const char *path,
                        const char* pattern,
                        list<string>& olist) {
	string fpath(path);
	int ret = find_file_init(path, pattern);
	if (ret) {
		NZLogger::log(NZ::ERROR, "find_file_init(%s, %s): %s",
				path, pattern, strerror(ret));
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
	if ((ret = find_file_finish()))
		NZLogger::log(NZ::ERROR, "find_file_finish(): %s", strerror(ret));
    olist.sort();
    return cnt;
}

int RHelper::find_files(const char* path,
                        const char* pattern,
                        vector<string>& ovec) {
    string fpath(path);
    if (find_file_init(path, pattern)) {
		NZLogger::log(NZ::ERROR, "find_file_init(%s, %s): %s",
				path, pattern, strerror(errno));
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
    if (find_file_finish())
		NZLogger::log(NZ::ERROR, "find_file_finish(): %s", strerror(errno));
    sort(ovec.begin(), ovec.end());
    return cnt;
}

#include <fstream>
ssize_t readFileIntoMem(char* const pbuf,
                        const size_t capacity,
                        const char* fpath) {
    size_t size;
    NZLogger::log(NZ::INFO, "reading [%s] ...", fpath);
    ifstream infile(fpath);
    if (!infile.is_open()) {
        NZLogger::log(NZ::ERROR, "open file [%s] failed", fpath);
        return -1;
    }
    infile.seekg(0, ios::end);
    size = infile.tellg();
    NZLogger::log(NZ::INFO, "file size: %u", size);
    if (size > capacity) {
        NZLogger::log(NZ::ERROR, "file size is larger than the capacity");
        return -1;
    }
    infile.seekg(0, ios::beg);
    infile.read(pbuf, size);
    infile.close();
	return size;
}

ssize_t readFileIntoMem(char* const pbuf,
                        const size_t capacity,
                        list<string>& flist) {
    ssize_t size(0);
    int ret;
    char* const p = pbuf;
    while (flist.size()) {
        NZLogger::log(NZ::INFO, "reading [%s] ...", flist.front());
        ret = readFileIntoMem(p, capacity - size, flist.front().c_str());

        if (ret) {
            flist.pop_front();
            size += ret;
        } else if (ret == 0) {
            NZLogger::log(NZ::INFO, "capacity reached");
            return size;
        } else {
            NZLogger::log(NZ::ERROR,
                          "an error occured while reading file into memory");
            return -1;
        }
    }
    return size;
}

void RHelper::print_progress(size_t percent, const char* hint) {
	if (!hint || strlen(hint) > 20)
		hint = "Progress:";
	char buf[51] = "--------------------------------------------------";
	for (size_t i = 0; i < percent; ++i) {
		buf[i/2] = '=';
	}
	cout << '\r' << hint << " [" << buf << "] " << percent << "%";
	if (percent == 100)
		cout << endl;
}
