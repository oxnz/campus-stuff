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
 * {fg}:
 *  30 Black
 *  31 Red
 *  32 Green
 *  33 Yellow
 *  34 Blue
 *  35 Magenta
 *  36 Cyan
 *  37 White
 * {bg}
 *  40 Black
 *  41 Red
 *  42 Green
 *  43 Yellow
 *  44 Blue
 *  45 Magenta
 *  46 Cyan
 *  47 White
 * format: "\x1B[{attr};{fg};{bg}m\x1B[0m"
 */
#include "NZLogger.h"

//#include <stdarg.h>
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
	int NZLogger::logImpl(NZLogger::LogLevel level, const char* file_,
				const char* func, int line, const char* fmt, ...) {
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
