#pragma once

#include "types.h"
#include <iostream>
#include <list>
#include <map>

class Processor {
 public:
    Processor(const std::string& listfname, size_t minPerTimeSlot);
    int processTS(void);
    bool hasNextFile(void) const {
        return m_plFileList->size() > 0;
    }
    int process(uint32_t nTimeSlot); // process n time slot length
    ~Processor();
 private:
    size_t readFileIntoMem(const std::string& fpath);
    int processFileBuffer(const char *buf, size_t len);
    size_t getTSIndex(uint64_t time);
 private:
    std::list<std::string> *m_plFileList;
    std::map<orec_key, orec_value*> *m_pmCTSRecordPool;
    std::map<orec_key, orec_value*> *m_pmNTSRecordPool;
    size_t m_nMinPerTS; // minute per time slot
    size_t m_nTimeTick; // current processing slot seconds left
    uint16_t m_itsp;   // time slot pointer, seperate CTS from NTS
    char *m_pFileBuffer;
    size_t m_nCRCount; // Current Record Count
    size_t m_nCRRepeat; // Repeat Record Count
    size_t m_nCRInvalid; // Invalid Record Count
    size_t m_nCRC; // Non-occupied Count
    size_t m_nCRO; // Occupied Count
    size_t m_nTransCount; // Transition records count from cur->next
    size_t m_nNRCount; // Next Record Count
    size_t m_nNRRepeat; // Repeat Record Count
    size_t m_nNRInvalid; // Invalid Record Count
    size_t m_nNRC; // Non-occupied Count
    size_t m_nNRO; // Occupied Count
    bool m_bEOF; // end of file
};
