#pragma once

#include <iostream>

namespace NZ {
    enum LOG_LEVEL {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
    };

	static const char* LOG_LEVEL_PREFIX[] = {
		"DEBUG: ",
		"INFO: ",
		"WARNING: ",
		"ERROR: ",
		"FATAL ERROR: ",
	};

    class NZLogger {
    public:
        static void setLogLevel(LOG_LEVEL level);
        static void log(const std::string& msg);
        static void log(LOG_LEVEL level, const std::string& msg);
		static inline std::ostream& log(LOG_LEVEL level = INFO) {
			return std::clog << LOG_LEVEL_PREFIX[level];
		}
    private:
		static std::ostream os;
        static LOG_LEVEL m_level;
    };
}
