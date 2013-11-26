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
    NZDirExcept(const std::string& s)
            : NZExcept(s) {}
    };
	class NZRegexExcept : public NZExcept
	{
	public:
	NZRegexExcept(const std::string& s)
		: NZExcept(s) {}
	};

	NZFinder(const char* path, const char* pattern = ".*");
	~NZFinder();
	dirent* find();
	private:
    private:
	struct __core* m_core;
    };
}
