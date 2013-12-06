/*
 * File: NZFinder.cpp
 * Description: Finder implementation file
 * Author: Oxnz <yunxinyi@gmail.com>
 * Copying: Copyright (C) 2013, All rights reserved.
 */
#include "NZFinder.h"
#include "NZUtils.h"

#include <regex.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
namespace NZ {
	struct __core {
		regex_t regexp;
		DIR* dirp;
	};
    NZFinder::NZFinder(const char* path, const char* pattern)
		: m_pcore(new struct __core)
    {
		if (regcomp(&m_pcore->regexp, pattern, REG_EXTENDED|REG_NEWLINE))
			throw NZRegexExcept("regulare expression compile failed");
		if ((m_pcore->dirp = opendir(path)) == NULL)
			throw NZDirExcept((string("cannot open dir: ") + path
					+ strerror(errno)).c_str());
	}
	NZFinder::~NZFinder() {
		closedir(m_pcore->dirp);
		delete m_pcore;
	}
	dirent* NZFinder::find() {
		struct dirent* dp(nullptr);
		while ((dp = readdir(m_pcore->dirp)) != NULL) {
			regmatch_t pmatch;
			int ret = regexec(&m_pcore->regexp, dp->d_name, 1, &pmatch, 0);
			if (!ret)
				break;
			else if (ret == REG_NOMATCH)
				continue;
			else // if (ret)
				throw NZRegexExcept("regexec failed");
		}
		return dp;
	}
}
