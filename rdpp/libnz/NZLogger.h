/*
 * File: NZLogger.h
 * Description: A colorful logger
 * Author: Oxnz <yunxinyi@gmail.com>
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-20 22:11:00
 */
#pragma once

#include <iostream>

#ifdef NDEBUG
#define NZLog(...) ((void)0)
#else
#define NZLog(level, ...) \
	NZ::NZLogger::log(level, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#endif

namespace NZ {
	class NZLogger {
	public:
		enum class LogLevel { DEBUG, INFO, WARNING, ERROR, FATAL };
		static void setLogLevel(LogLevel level) {
			m_level = level;
		}
		static int log(LogLevel level, const char* file_,
				const char* func, int line, const char* fmt, ...);
	public:
		static bool m_color; // default is true
	private:
		static LogLevel m_level; // default is debug
	};
}
