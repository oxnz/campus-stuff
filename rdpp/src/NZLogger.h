/*
 * File: NZLogger.h
 * Description:  A Colorful Logger
 * Author: Oxnz
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013 Oxnz All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-10 22:10:12
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
#pragma once

#include <iostream>

namespace NZ {
    enum LOG_LEVEL {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
		NOTICE, // notice always the highest privor
    };

    class NZLogger {
    public:
        static void setLogLevel(LOG_LEVEL level) {
			NZ::NZLogger::m_level = level;
		}
		static void setLogStyle(const char* prefix, bool time = false,
				const char* suffix = nullptr, bool color = true) {
			m_sPrefix = std::string(prefix);
			m_sSuffix = std::string(suffix);
			m_bTime = time;
			m_bColor = color;
		}
		/*
		static inline void log(const char* s) {
			log(m_level, s);
		}
		static inline void log(LOG_LEVEL level, const char* s) {
			if (level < m_level)
				return;
			if (level >= ERROR)
				printf("%s", "\x1B[31m");
			std::clog << LOG_LEVEL_PREFIX[level];
			printf(s);
			if (level >= ERROR)
				printf("%s", "\x1B[0m");
			std::clog << std::endl;
		}
		*/
		template<typename... Args>
        static inline void log(const char* fmt, const Args&... args) {
			log(m_level, fmt, args...);
		}
		template<typename... Args>
		static inline void log(LOG_LEVEL level,
				const char* fmt,
				const Args&... args) {
			if (level < m_level)
				return;
			switch (level) {
				case DEBUG:	// Magenta
					printf("%s", "\x1B[35mDEBUG: ");
					break;
				case INFO:	// Blue
					printf("%s", "\x1B[34mINFO: ");
					break;
				case WARNING: // red
					printf("%s", "\x1B[31mWARNING: ");
					break;
				case ERROR:	// bright red
					printf("%s", "\x1B[1;31mERROR: ");
					break;
				case FATAL:	// bright white on red
					printf("%s", "\x1B[1;37;41mFATAL ERROR: ");
					break;
				case NOTICE:	// bright Green
					printf("%s", "\x1B[1;32mNOTICE: ");
					break;
				default:	// Blue
					printf("%s", "\x1B[34mLOG: ");
					break;
			}
			//std::clog << LOG_LEVEL_PREFIX[level];
			printf(fmt, args...);
			printf("%s", "\x1B[0m");
			std::clog << std::endl;
		}
		static inline void printf(const char* s) {
			while (*s) {
				if (*s == '%' && *++s != '%')
					std::clog << "*** log error: invalid format string,"
					   << "	missing arguments" << std::endl;
				std::clog << *s++;
			}
		}
		template<typename T, typename... Args>
			static void printf(const char* fmt, const T& value,
					const Args&... args) {
			while (*fmt) {
				if (*fmt == '%' && *++fmt != '%') {
					std::clog << value;
					return printf(++fmt, args...);
				} else
					std::clog << *fmt++;
			}
			std::clog << "*** log error: extra arguments provided to"
				<< std::endl;
			//throw std::runtime_error("extra arguments provided to log");
		}
    private:
        static LOG_LEVEL m_level;
		static std::string m_sPrefix;
		static std::string m_sSuffix;
		static bool m_bTime;
		static bool m_bColor;
    };
}
