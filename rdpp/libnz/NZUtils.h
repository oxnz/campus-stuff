#pragma once

#ifdef NDEBUG
#define assert(cond) ((void)0)
#else

#define assert(cond) \
	((cond) ? (void)0 : NZUtils::assertImpl(#cond, __FILE__, __LINE__))
#endif

#include <iostream>
namespace NZUtils {
	inline void assertImpl(const char* cond, const char* file_, int line) {
		std::cout << "assert(" << cond << ") failed, file: " << file_
			<< ", line: " << line << std::endl;
		exit(1);
	}
	inline void printProgress(size_t percent, const char* hint = "") {
		if (!hint || strlen(hint) > 20)
			hint = "Progress:";
		char buf[51] = "--------------------------------------------------";
		/*
		if (percent%2)
			buf[(percent+1)>>1] = '>';
		*/
		for (size_t i = 0; i < percent; ++i) {
			buf[i/2] = '=';
		}
		std::cout << '\r' << hint << ": [" << buf << "] " << percent << "%";
		if (percent == 100)
			std::cout << std::endl;
	}
}
