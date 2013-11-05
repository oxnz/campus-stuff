/*
 *            File: processor.cpp
 *     Description: Main Pre Processor Source File
 *    Last-updated: 2013-10-25 22:54:04 CST
 *         Version: 0.1
 */


#include "RProcessor.h"
#include "RsidGen.h"
#include "RConstant.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;

Processor::Processor(const string& listfname, size_t minPerTS)
    : m_nMinPerTS(minPerTS), m_CurrentDate(-1), m_itsp(0xFF),
      m_nTransCount(10000), m_bEOF(true)
{
    ifstream listf(listfname.c_str());
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
inline size_t Processor::getTSIndex(const uint64_t time) {
    uint16_t h = static_cast<uint16_t>(time % 1000000/10000);
    uint16_t m = time % 10000/100;
    uint16_t s = time % 100;
    return (h*60*60+m*60+s)/(m_nMinPerTS*60);
}

inline int Processor::processOrigRecord(const in_rec& rec) {
#ifdef DEBUG
    printf("%u,%u,%u,%llu,%11.7lf,%10.7lf,%u,%u,%u\n",
           rec.car_id, rec.event, rec.status, rec.time,
           rec.x, rec.y, rec.speed, rec.direct, rec.valid);
#endif
    gps_coord coord = {static_cast<gps_x>(rec.x * 10000000), static_cast<gps_y>(rec.y * 10000000)};
    orec_key key = {get_rsid(coord), rec.cid};
    /*
     * @advice: skip the wrong road id
     */
    if (key.rsid == -1) { cerr << "invalid road id" << endl; return 0; }
    size_t ts = getTSIndex(rec.time);
    //if (m_itsp == -1) m_itsp = ts;
    map<orec_key, orec_value*> *pcrp = 0; // pointer -> current record pool
    if (ts == m_itsp) {
        pcrp = m_pmCTSRecordPool;
    }
    else if (ts == m_itsp+1 || (ts == 0 && m_itsp-1 == 24*60/m_nMinPerTS)) {
        pcrp = m_pmNTSRecordPool;
        //++m_nCurTransCnt;
    }
    else if (ts == m_itsp-1){
        //cerr << "Error: come up with an previous ts: " << ts << ", skipped"
          //   << endl;
        //return -1;
        return 0;
    } else {
        //cerr << "Error: come up with an unexpected ts: " << ts << endl;
        return 0;
    }
    orec_value *porecv = new orec_value; // pointer -> orec_value
    porecv->status = rec.status;
    porecv->time = rec.time;
    //pair<map<orec_key, orec_value*>::iterator, bool> ret =
        pcrp->insert(make_pair(key, porecv));
    //if (!ret.second)
        //++m_nRepeat;
#ifdef DEBUG
	cout << "roadseg_id: " << key.rsid << " car_id: " << key.cid << endl;
#endif
    
    return 0;
}

int Processor::processFileBuffer() {
    for (in_rec irec; m_pCurFBufPos < m_pFileBufEnd &&
             sscanf(m_pCurFBufPos, "%u,%hu,%hu,%llu,%lf,%lf,%hu,%hu,%hu\r\n",
                    &irec.cid, &irec.event, &irec.status,
                    &irec.time, &irec.x, &irec.y,
                    &irec.speed, &irec.direct, &irec.valid) == 9;
         m_pCurFBufPos = strchr(++m_pCurFBufPos, '\n')) {
        if (!irec.valid) continue;
        if (irec.status != NON_OCCUPIED) continue;
        if (m_itsp == 0xFF) m_itsp = getTSIndex(irec.time);
        if (processOrigRecord(irec) != 0) {
        //if (processRecord(irec) != 0) {
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
    cout << "one file finished, about to process next" << endl;
    return 0;
}

ssize_t Processor::readFileIntoMem(const char* fpath) {
    cout << "reading " << fpath;
    const char* p = strrchr(fpath, '/');
    if (p)
        ++p;
    else
        p = fpath;
    char buf[9] = {0};
    for (int i = 0; i < 8; ++i)
        buf[i] = p[i];
    m_CurrentDate = strtol(buf, NULL, 10);
    if (m_CurrentDate < 20121101 && m_CurrentDate > 20121131) {
        cerr << "error date in filename: " << m_CurrentDate << endl;
        return -1;
    }
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
        char fname[MAXPATHLEN];
        sprintf(fname, "%08d-%04d.dat", m_CurrentDate, m_itsp);
        ofstream outfile(fname, ios::out|ios::binary);
        for (map<orec_key, orec_value*>::iterator it = m_pmCTSRecordPool->begin();
             it != m_pmCTSRecordPool->end(); ++it, ++cnt) {
#ifdef DEBUG
            cout << "saved:(" << it->first.rsid << "," << it->first.cid << ")"
                 << "status:" << it->second->status << " time:"
                 << it->second->time << endl;
#endif
            outfile.write(reinterpret_cast<const char*>(&it->first.rsid),
                          sizeof(roadseg_id));
            /*
            outfile.write(reinterpret_cast<const char*>(&it->second->status),
                          sizeof(car_status));
            outfile.write(reinterpret_cast<const char*>(&it->second->time),
                          sizeof(gps_time));
                          */
        }
        outfile.close();
        cout << "write " << cnt << " records to file ["
             << fname << "]" << endl;
    }
    return 0;
}

int Processor::transferToNextTS() {
    if (dumpRecordsToFile() != 0) {
        cout << "dump file error" << endl;
        return -1;
    }
    if (++m_itsp == static_cast<int16_t>(24*60/m_nMinPerTS)) {
        cout << "current day processed done" << endl;
        m_itsp = 0;
    }
    swap(m_pmCTSRecordPool, m_pmNTSRecordPool);
	for (map<orec_key, orec_value*>::iterator it =
                 m_pmNTSRecordPool->begin(); it != m_pmNTSRecordPool->end();
             ++it) {
            delete it->second;
        }
    m_pmNTSRecordPool->clear();

    return 0;
}

int Processor::processTS(void) {
    while (m_pmNTSRecordPool->size() <= m_nTransCount) {
        cout << "next time slot size= " << m_pmNTSRecordPool->size() << endl;
        if (m_bEOF) { // test if current file processed done
            if (hasNextFile()) {
                cout << "========> processing [" << m_plFileList->front()
                     << "]" << endl;
                if(readFileIntoMem(m_plFileList->front().c_str()) <= 0)
                    return -1;
                m_plFileList->pop_front();
            } else {  // no more file exsits, dump to file & notify main by return 1
                //m_itsp = 24*60/m_nMinPerTS;
                m_itsp += 1000; // indicate that is the last dump which is incomplete
                if (dumpRecordsToFile() != 0) {
                    cout << "dump to file failed" << endl;
                    return -1;
                }
                cout << "dump to file succeed" << endl;
                return 1;
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
            cout << "Transfer to next TS ==> ";
            if (transferToNextTS() == -1) {
                cout << " failed" << endl;
                getchar();
            }
            cout << "succeed" << endl;
            return 0;
            break;
        default: // should never reach here
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
