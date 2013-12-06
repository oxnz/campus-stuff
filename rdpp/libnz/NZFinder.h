#pragma once

#include "NZExcept.h"

#include <stdexcept>

#include <dirent.h>

namespace NZ {
    class NZFinder {
    public:
	class NZFinderBadArg : public NZBadArg {};
    class NZDirExcept : public NZExcept
    {
    public:
    NZDirExcept(const char* s)
            : NZExcept(s) {}
    };
	class NZRegexExcept : public NZExcept
	{
	public:
	NZRegexExcept(const char* s)
		: NZExcept(s) {}
	};

	NZFinder(const char* path, const char* pattern = ".*");
	~NZFinder();
	dirent* find();
	private:
	struct __core* m_pcore;
    };
}
