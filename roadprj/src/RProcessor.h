#pragma once

#include "RTypes.h"

#include <iostream>
#include <list>
#include <map>

class Processor {
 public:
    Processor(const char* indir, const char* outdir,
              size_t minPerTimeSlot = 3,
              size_t bufsize = 2*1024*1024);
    int process(uint32_t date, size_t len = 1);
    ~Processor();
private:
	 Processor(const Processor& p); // disable copy constructor
	 Processor& operator= (const Processor& p); // disable copy-assign
private:
    ssize_t readFileIntoMem(const char* fpath);
    int processFileBuffer(); // strto{ul,ull,d} version
    inline int processOrigRecord(const in_rec& rec);
    int dumpRecords();
    inline size_t getTSIndex(const gps_time& time);
 private:
    std::string m_indir;
    std::string m_outdir;
    std::list<std::string> m_fileList;   // contains fpath
    std::map<const orec_key, void*>* m_pTSPool;
    
    const size_t m_nMinPerTS; // minute per time slot
    const size_t m_nTSCnt;
    
    gps_time m_tsp; // time slot pointer, indicate current time

    const size_t m_nBufSize;    // file buffer size
    const char* m_pFileBuffer;  // point to file buffer
    const char* m_pFileBufEnd;  // file buffer end pointer
    char*  m_pCurFBufPos;       // pointer remember position in file buffer

};
