/*
 *            File: processor.cpp
 *     Description: Main Pre Processor Source File
 *    Last-updated: 2013-10-25 22:54:04 CST
 *         Version: 0.1
 */

#include "processor.h"
#include "types.h"
#include "dummy.h"
#include "constant.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Processor::Processor(const string& listfname, size_t minPerTS)
    : m_nMinPerTS(minPerTS), 
      m_nCRCount(0), m_nCRRepeat(0), m_nCRInvalid(0), m_nCRC(0), m_nCRO(0),
      m_nNRCount(0), m_nNRRepeat(0), m_nNRInvalid(0), m_nNRC(0), m_nNRO(0),
      m_itsp(-1), m_nTransCount(1000), m_bEOF(true)
{
    ifstream listf(listfname);
    if (!listf.is_open()) {
        cerr << "error: unable to open file: " << listfname << endl;
        throw runtime_error("can't open file: " + listfname);
    }
    m_plFileList = new list<string>;
    string fpath;
    while (listf >> fpath) {
#ifdef DEBUG
        cout << "file: " << fpath << endl;
#endif
        m_plFileList->push_back(fpath);
    }
    if (!m_plFileList->size()) {
        cerr << "File list contains no file" << endl;
        return; // FIX: change to throw
    }
    m_pmCTSRecordPool = new std::map<orec_key, orec_value*>;
    m_pmNTSRecordPool = new std::map<orec_key, orec_value*>;
    m_pFileBuffer = new char[RFBUF_MAXLEN];
}

/*
 * @description: get time slot index
 */
size_t Processor::getTSIndex(const uint64_t time) {
    uint16_t h = static_cast<uint16_t>(time % 1000000/10000);
    uint16_t m = time % 10000/100;
    uint16_t s = time % 100;
    return (h*60*60+m*60+s)/(m_nMinPerTS*60);
}

int Processor::processOrigRecord(const in_rec &rec) {
#ifdef DEBUG
    printf("%u,%u,%u,%llu,%11.7lf,%10.7lf,%u,%u,%u\n",
           rec.car_id, rec.event, rec.status, rec.time,
           rec.x, rec.y, rec.speed, rec.direct, rec.valid);
#endif
    gps_coord coord = {static_cast<gps_x>(rec.x * 10000000), static_cast<gps_y>(rec.y * 10000000)};
    orec_key key = {get_roadseg_id(coord), rec.car_id};
    /*
     * @advice: skip the wrong road id
     */
    if (key.rsid == -1) { cerr << "invalid road id" << endl; return -1; }
    size_t ts = getTSIndex(rec.time);
    if (m_itsp == -1) m_itsp = ts;
    map<orec_key, orec_value*> *pcrp = 0; // pointer -> current record pool
    if (ts == m_itsp) {
        pcrp = m_pmCTSRecordPool;
    }
    else if (ts == m_itsp+1) {
        pcrp = m_pmNTSRecordPool;
        //++m_nCurTransCnt;
    }
    else if (ts == m_itsp-1){
        cerr << "Error: come up with an previous ts: " << ts << ", skipped"
             << endl;
        //return -1;
        return 0;
    } else {
        cerr << "Error: come up with an unexpected ts: " << ts << endl;
        return 0;
    }
    orec_value *porecv = new orec_value; // pointer -> orec_value
    porecv->status = rec.status;
    porecv->time = rec.time;
    pair<map<orec_key, orec_value*>::iterator, bool> ret =
        pcrp->insert(make_pair(key, porecv));
    if (!ret.second) ++m_nCRRepeat;
#ifdef DEBUG
    printf("roadseg_id: %u, car_id = %u\n", key.rsid, key.cid);
#endif
    
    return 0;
}

int Processor::processFileBuffer() {
    for (in_rec irec; m_pCurFBufPos < m_pFileBufEnd &&
#ifdef WIN32
             sscanf_s(m_pCurFBufPos, "%u,%hu,%hu,%llu,%lf,%lf,%hu,%hu,%hu\r\n",
#else
             sscanf(m_pCurFBufPos, "%u,%hu,%hu,%llu,%lf,%lf,%hu,%hu,%hu\r\n",
#endif
                    &irec.car_id, &irec.event, &irec.status,
                    &irec.time, &irec.x, &irec.y,
                    &irec.speed, &irec.direct, &irec.valid) == 9;
         m_pCurFBufPos = strchr(++m_pCurFBufPos, '\n')) {
        ++m_nCRCount;
        if (!irec.valid) { ++m_nCRInvalid; continue; }
        if (processOrigRecord(irec) != 0) {
            cerr << "process record failed" << endl;
            return -1;
        }
        if (m_pmNTSRecordPool->size() == m_nTransCount) {
            cout << "reach the max transition count" << endl;
            return 1;
        } else if (m_pmNTSRecordPool->size() > m_nTransCount) {
            cout << "greater than trans count, how ?!" << endl;
            return 0;
        }
    }
    m_bEOF = true;
    cout << "one file finished, press enter to process next" << endl;
    getchar();
    return 0;
}

ssize_t Processor::readFileIntoMem(const char* fpath) {
    cout << "reading " << fpath;
    ifstream infile(fpath);
    if (!infile.is_open()) {
        cerr << "open file failed" << endl;
        return -1;
    }
    infile.seekg(0, ios::end);
    ssize_t fsize = static_cast<ssize_t>(infile.tellg());
    if (fsize > RFBUF_MAXLEN) {
        cerr << "file size too large" << endl;
        return -1;
    }
    m_pFileBufEnd = m_pFileBuffer + fsize;
    m_pCurFBufPos = (char *)m_pFileBuffer;
    cout << " size: " << fsize;
    infile.seekg(0, ios::beg);
    infile.read((char *)m_pFileBuffer, fsize);
    infile.close();
    cout << endl;
    return fsize;
}

int Processor::dumpRecordsToFile() {
    if (m_pmCTSRecordPool) {
        size_t cnt = 0;
        // TODO
        ofstream outfile("outfile", ios::out|ios::app|ios::binary);
        for (map<orec_key, orec_value*>::iterator it = m_pmCTSRecordPool->begin();
             it != m_pmCTSRecordPool->end(); ++it, ++cnt) {
#ifdef DEBUG
            cout << "saved:(" << it->first.rsid << "," << it->first.cid << ")"
                 << "status:" << it->second->status << " time:"
                 << it->second->time << endl;
#endif
            outfile.write(reinterpret_cast<const char*>(&it->first.rsid),
                          sizeof(roadseg_id));
            outfile.write(reinterpret_cast<const char*>(&it->second->status),
                          sizeof(car_status));
            outfile.write(reinterpret_cast<const char*>(&it->second->time),
                          sizeof(gps_time));
        }
        outfile.close();
        cout << "write " << cnt << " records out" << endl;
    }
    return 0;
}

int Processor::transferToNextTS() {
    if (dumpRecordsToFile() != 0) {
        cout << "dump file error" << endl;
        return -1;
    }
    if (++m_itsp == 24*60/m_nMinPerTS) {
        cout << "current day processed done" << endl;
        return -1;
    }
    swap(m_pmCTSRecordPool, m_pmNTSRecordPool);
    m_pmNTSRecordPool->clear();
    m_nCRCount = m_nNRCount;
    m_nNRCount = 0;
    m_nCRRepeat = m_nNRRepeat;
    m_nNRRepeat = 0;
    m_nCRInvalid = m_nNRInvalid;
    m_nNRInvalid = 0;
    m_nCRC = m_nNRC;
    m_nNRC = 0;
    m_nCRO = m_nNRO;
    m_nNRO = 0;

    cout << "transfered to next time slot" << endl;
    return 0;
}

int Processor::processTS(void) {
    while (m_pmNTSRecordPool->size() <= m_nTransCount) {
        cout << "next time slot size= " << m_pmNTSRecordPool->size() << endl;
        if (m_bEOF) { // test if current file processed done
            if (hasNextFile()) {
                cout << "processing " << m_plFileList->front();
                if(readFileIntoMem(m_plFileList->front().c_str()) <= 0)
                    return -1;
                m_plFileList->pop_front();
            } else {  // no more file exsits
                cout << "all files are processed" << endl;
                if (dumpRecordsToFile() != 0) {
                    cout << "dump error" << endl;
                    return -1;
                }
                cout << "dumped" << endl;
                getchar();
                return 0;
            }
        }
        switch (processFileBuffer()) {
        case 0:
            cout << "return 0 from pro file buf" << endl;
            continue;
            break;
        case -1:
            cout << "process file buffer failed" << endl;
            return -1;
            break;
        case 1:
            cout << "need transfer to next TS" << endl;
            getchar();
            if (transferToNextTS() == -1) {
                cout << "transfer to next TS failed" << endl;
                getchar();
            }
            cout << "after transfer" << endl;
            getchar();
            cout << "next sz=" << m_pmNTSRecordPool->size() << endl;
            return 0;
            break;
        default:
            cerr << "unexpected return value" << endl;
            return -1;
        }

    }

    return 0;
}

Processor::~Processor() {
    while (m_plFileList->size())
        m_plFileList->pop_front();
    delete m_plFileList;
    if (m_pmCTSRecordPool) {
        for (map<orec_key, orec_value*>::iterator it =
                 m_pmCTSRecordPool->begin(); it != m_pmCTSRecordPool->end();
             ++it) {
            delete it->second;
        }
        m_pmCTSRecordPool->clear();
        delete m_pmCTSRecordPool;
    }
    if (m_pmNTSRecordPool) {
        for (map<orec_key, orec_value*>::iterator it =
                 m_pmNTSRecordPool->begin(); it != m_pmNTSRecordPool->end();
             ++it) {
            delete it->second;
        }
        m_pmNTSRecordPool->clear();
        delete m_pmNTSRecordPool;
    }
    delete m_pFileBuffer;
}

