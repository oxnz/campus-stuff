/*
 * File: RProcess.h
 * Description: main pre processor header file
 * Author: Oxnz
 * Version: 1.1
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12
 */
#pragma once

#include "RTypes.h"
#include "RDPool.h"

#include <iostream>
#include <list>
#include <map>
#include <set>

namespace R {
    class Processor {
    public:
        Processor(const char* indir,
                  const char* outdir,
                  size_t minPerTimeSlot = 3,
                  size_t bufsize = 2*1024*1024,
                  bool process = true);
        int process(uint32_t date, size_t len = 1, bool progbar = true);
        ~Processor();
    private:
        Processor(const Processor& p); // disable copy constructor
        Processor& operator= (const Processor& p); // disable copy-assign
    private:
        ssize_t readFileIntoMem(const char* fpath);
        int processFileBuffer();
        inline int processOrigRecord(const in_rec& rec, bool echo = false);
        int dumpRecords();
		/*
        inline ts_index getTSIndex(const gps_time& time) {
			return ((time%1000000/10000)*60+time%10000/100)/m_nMinPerTS;
		}*/

    private: // preprocessor member part
        std::string m_indir;
        std::string m_outdir;
        std::list<std::string> m_fileList;   // contains fpath
        std::set<orec_key>* m_pTSPool;
        
        const size_t m_nMinPerTS; // minute per time slot
        const size_t m_nTSCnt;
        
        gps_time m_tsp; // time slot pointer, indicate current time
        
        const size_t m_nBufSize;    // file buffer size
        const char* m_pFileBuffer;  // point to file buffer
        const char* m_pFileBufEnd;  // file buffer end pointer
        char*  m_pCurFBufPos;       // pointer remember position in file buffer

        const bool m_bProcess;
        RDP::RDPool* m_pRDPool;
    };
}
