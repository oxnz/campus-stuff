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
		: m_core(new struct __core)
    {
		if (regcomp(&m_core->regexp, pattern, REG_EXTENDED|REG_NEWLINE))
			throw NZRegexExcept("regulare expression compile failed");
		if ((m_core->dirp = opendir(path)) == NULL)
			throw NZDirExcept(string("cannot open dir: ") + path
					+ strerror(errno));
	}
	NZFinder::~NZFinder() {
		closedir(m_core->dirp);
		delete m_core;
	}
	dirent* NZFinder::find() {
		struct dirent* dp(nullptr);
		while ((dp = readdir(m_core->dirp)) != NULL) {
			regmatch_t pmatch;
			int ret = regexec(&m_core->regexp, dp->d_name, 1, &pmatch, 0);
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
