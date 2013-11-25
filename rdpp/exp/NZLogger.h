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
#define log(...) ((void)0)
#else
#define log(level, ...) \
	NZ::NZLogger::logImpl(level, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#endif

namespace NZ {
	class NZLogger {
	public:
		enum class LogLevel { DEBUG, INFO, WARNING, ERROR, FATAL };
		static void setLogLevel(LogLevel level) {
			m_level = level;
		}
		static int logImpl(LogLevel level, const char* file_,
				const char* func, int line, const char* fmt, ...);
	public:
		static bool m_color;
	private:
		static LogLevel m_level;
	};
}
