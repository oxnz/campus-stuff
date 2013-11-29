#pragma once

#ifdef NDEBUG
#define NZAssert(cond) ((void)0)
#else

#define NZAssert(cond) \
	((cond) ? (void)0 : NZUtils::assert(#cond, __FILE__, __LINE__))
#endif

#include <iostream>
namespace NZUtils {
	inline void assert(const char* cond, const char* file_, size_t line) {
		std::cout << "assert(" << cond << ") failed, file: " << file_
                  << ", line: " << line << std::endl;
		exit(1);
	}
	void printProgress(size_t percent, const char* hint = "");
	inline bool NZConfirm(const std::string& prompt) {
		std::cout << prompt << "? [Y/N]:";
		char c;
		while (std::cin >> c) {
			if (c == 'Y' || c == 'y')
				return true;
			else if (c == 'N' || c == 'n')
				return false;
			else {
				std::cout << "[Y/N]: ";
			}
		}
		return false;
	}
	/*
	 * @description: active like hexdump(1) & xxd(1)
	 * @params:
	 * 	-i p: source pointer
	 * 	-i len: source len
	 * 	-i col: column
	 * 	-i groupsz: group size in byte
	 * 	-i capital: if print hex in capital
	 * @returns:
	 * 	-o none
	 */
	void NZHexDump(const uint8_t* p,
                   size_t len,
                   size_t col = 16,
                   size_t groupsz = 2,
                   bool capital = true);
}
