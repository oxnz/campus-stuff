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

    class NZLogger {
    public:
        static void setLogLevel(LOG_LEVEL level);
        static void log(const std::string& msg);
        static void log(LOG_LEVEL level, const std::string& msg);
    private:
        static LOG_LEVEL m_level;
    };
 
}
