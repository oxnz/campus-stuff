#pragma once

#include "types.h"
#include <iostream>
#include <list>
#include <map>

class Processor {
 public:
    Processor(const std::string& listfname);
    int process(gps_time time);
    ~Processor();
 private:

 private:
    std::list<std::string> *m_plFileList;
    std::map<road_id, car_record*> *m_pmCTSRecordPool;
    std::map<road_id, car_record*> *m_pmNTSRecordPool;
    char *m_pFileBuffer;
    size_t m_nRCount;
    size_t m_nRRepeat;
    size_t m_nRInvalid;
};

