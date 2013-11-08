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
    std::string label;
    std::ostream* os = &std::cout;
    switch (level) {
    case DEBUG:
        label = "DEBUG: ";
        break;
    case INFO:
        label = "INFO: ";
        break;
    case WARNING:
        label = "WARNING: ";
        break;
    case ERROR:
        label = "ERROR: ";
        break;
    case FATAL:
        label = "FATAL ERROR: ";
        break;
    default:
        label = "INFO: ";
        break;
    }
    if (level > DEBUG)
        os = &std::cerr;
    *os << label << msg << std::endl;
}



