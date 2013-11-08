#include "NZLogger.h"

NZ::LOG_LEVEL NZ::NZLogger::m_level = DEBUG;

void NZ::NZLogger::setLogLevel(LOG_LEVEL level) {
    NZ::NZLogger::m_level = level;
}

void NZ::NZLogger::log(const std::string& msg) {
    log(m_level, msg);
}

void NZ::NZLogger::log(LOG_LEVEL level, const std::string& msg) {
    if (level < m_level)
        return;
	std::clog << LOG_LEVEL_PREFIX[level] << msg << std::endl;
}
