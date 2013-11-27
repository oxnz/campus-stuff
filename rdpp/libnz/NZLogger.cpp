/*
 * File: NZLogger.cpp
 * Description: NZLogger impl file
 * Author: Oxnz <yunxinyi@gmail.com>
 * Copying: Copyright (C) 2013, All rights reserved.
 */

/*
 * ref:
 *
 * {attr}:
 * 	0 Reset all attributes (return to normal mode)
 * 	1 Bright (Usually turns on BOLD)
 * 	2 Dim
 * 	3 Underline
 * 	4 Blink
 * 	5 Reverse
 * 	6 Hidden
 * {color	fg	bg}:
 *  Black	30	40
 *  Red		31	41
 *  Green	32	42
 *  Yellow	33	43
 *  Blue	34	44
 *  Magenta	35	45
 *  Cyan	36	46
 *  White	37	47
 * format: "\x1B[{attr};{fg};{bg}m\x1B[0m"
 */
#include "NZLogger.h"

#include <cstdarg>
#include <cstdio>
#include <ctime>

namespace NZ {
	/*
	 * init class variables
	 */
	NZLogger::LogLevel NZLogger::m_level = NZLogger::LogLevel::DEBUG;
	bool NZLogger::m_color = true;

	/*
	 * member functions
	 */
	int NZLogger::log(NZLogger::LogLevel level, const char* file_,
				const char* func, int line, const char* fmt, ...) {
		if (level < m_level)
			return 0;
		time_t t = time(NULL);
		struct tm* pt = localtime(&t);
		const char* lvlprefix;
		switch (level) {
			case NZLogger::LogLevel::DEBUG: // Magenta
				lvlprefix = m_color ? "\x1B[35mDebug" : "Debug";
				break;
			case NZLogger::LogLevel::INFO: // Blue
				lvlprefix = m_color ? "\x1B[34mInfo" : "Info";
				break;
			case NZLogger::LogLevel::WARNING: // Red
				lvlprefix = m_color ? "\x1B[31mWarning" : "Warning";
				break;
			case NZLogger::LogLevel::ERROR: // bright red
				lvlprefix = m_color ? "\x1B[1;31mError" : "Error";
				break;
			case NZLogger::LogLevel::FATAL: // bright white on red background
				lvlprefix = m_color ? "\x1B[1;37;41mFatal Error" :
					"Fatal Error";
				break;
		}

		std::fprintf(stderr, "%04d-%02d-%02d %02d:%02d:%02d %s[%s:%d] %s: ",
				pt->tm_year + 1900, pt->tm_mon + 1, pt->tm_mday,
				pt->tm_hour, pt->tm_min, pt->tm_sec,
				func, file_, line,
				lvlprefix
				);
		va_list args;
		va_start(args, fmt);
		int ret = std::vfprintf(stderr, fmt, args);
		va_end(args);
		std::fprintf(stderr, "%s", m_color ? "\x1B[0m\n" : "\n");
		return ret;
	}
}
