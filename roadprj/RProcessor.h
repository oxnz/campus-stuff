#pragma once

#include "RTypes.h"
#include <iostream>
#include <list>
#include <map>


class Processor {
 public:
    /*
     * @description: initialization work, which open listfname and read
     *               fliename and store, malloc mem, setup some constant
     *               value, etc.
     * @parameter:
     *  listfname: a text which contains a list of filepath which is sorted
     *             by the record time sequence
     *  minPerTimeSlot: minute per time slot
     * @return: None
     * @exception: if malloc error, will throw out bad_alloc exception;
     *             if listfname open fails, will throw out runtime_error
     *             exception
     */
    Processor(const std::string& listfname, size_t minPerTimeSlot);
    /*
     * @description: process one time slot each time
     *
     * @return: 0 on success, -1 on failure
     */
    int processTS(void);
    bool hasNextFile(void) const {
        return m_plFileList->size() > 0;
    }
    /*
     * @description: process n time slot each time
     *
     * @notice: unimplemented yet
     */
    int process(uint32_t nTimeSlot);
    ~Processor();
private:
	 Processor(const Processor& p); // disable copy constructor
	 Processor& operator= (const Processor& p); // disable copy-assign
private:
    ssize_t readFileIntoMem(const char* fpath);
    int processFileBuffer(); // strto{ul,ull,d} version
    int processFileBuffer2(); // sscanf version, too slow on linux sys
    inline int processOrigRecord(const in_rec& rec);
    int transferToNextTS(void);
    int dumpRecordsToFile();
    inline size_t getTSIndex(const uint64_t time);
 private:
    std::list<std::string> *m_plFileList;   // contains fpath

    /*
     * Current Time Slot Record Pool &
     * Next Time Slot Record Pool
     */
    std::map<orec_key, orec_value*> *m_pmCTSRecordPool;
    std::map<orec_key, orec_value*> *m_pmNTSRecordPool;

    const size_t m_nMinPerTS; // minute per time slot

    rec_date m_CurrentDate;    // Current Date;
    size_t m_itsp;     // time slot pointer, seperate CTS from NTS

    const char* m_pFileBuffer;  // point to file buffer
    const char* m_pFileBufEnd;  // file buffer end pointer
    char*  m_pCurFBufPos;       // pointer remember position in file buffer

    const size_t m_nTransCount; // Transition records count from cur->next

    bool m_bEOF;        // end of file
};
