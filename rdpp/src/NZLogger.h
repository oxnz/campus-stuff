/*
 * File: NZLogger.h
 * Description:  A simple Logger class
 * Author: Oxnz
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013 Oxnz All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-10 22:10:12
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

	static const char* LOG_LEVEL_PREFIX[] = {
		"DEBUG: ",
		"INFO: ",
		"WARNING: ",
		"ERROR: ",
		"FATAL ERROR: ",
		"NOTICE: ",
	};

    class NZLogger {
    public:
        static void setLogLevel(LOG_LEVEL level) {
			NZ::NZLogger::m_level = level;
		}
		/*
		static void setLogStyle(const char* prefix, bool time,
				const char* suffix) {
			m_prefix = string(prefix);
			m_bTime = time;
		}
		*/
		static inline void log(const char* s) {
			log(m_level, s);
		}
		static inline void log(LOG_LEVEL level, const char* s) {
			if (level < m_level)
				return;
			std::clog << LOG_LEVEL_PREFIX[level];
			printf(s);
			std::clog << std::endl;
		}
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
			std::clog << LOG_LEVEL_PREFIX[level];
			printf(fmt, args...);
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
    };
}
