/*
 *            File: processor.cpp
 *     Description: Main Pre Processor Source File
 *    Last-updated: 2013-11-06 13:36:50 CST
 *          Author: Oxnz
 *         Version: 0.1
 */


#include "RProcessor.h"
#include "RsidGen.h"
#include "RConstant.h"
#include "RHelper.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

Processor::Processor(const char* indir, const char* outdir, size_t minPerTS)
    : m_outdir(outdir), m_nMinPerTS(minPerTS), m_CurrentDate(-1),
      m_itsp(0xFF), m_nTransCount(15000)
{
    int cnt = find_files(indir, "2012", m_fileList);
    if (cnt == 0) throw runtime_error("no file found");
    else if (cnt < 0) throw runtime_error("find_files error");

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
	cout << "(" << rec.cid << "," << rec.event << ","
		<< rec.status << "," << rec.time << ","
		<< rec.x << "," << rec.y << "," << rec.speed << ","
		<< rec.direct << "," << rec.valid << ")" << endl;
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
    else if (ts == m_itsp+1 || (ts == 0 && m_itsp == 24*60/m_nMinPerTS-1)) {
        pcrp = m_pmNTSRecordPool;
        //++m_nCurTransCnt;
    }
    else if (ts+1 == m_itsp){
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
	char* p;
	int i = 0;
	for (in_rec irec; m_pCurFBufPos < m_pFileBufEnd - 1; ++m_pCurFBufPos) {
		irec.cid = strtoul(m_pCurFBufPos, &p, 10);
		m_pCurFBufPos = p;
		irec.event = strtoul(++m_pCurFBufPos, &p, 10);
		m_pCurFBufPos = p;
		irec.status = strtoul(++m_pCurFBufPos, &p, 10);
		m_pCurFBufPos = p;
		irec.time = strtoull(++m_pCurFBufPos, &p, 10);
		m_pCurFBufPos = p;
		irec.x = strtod(++m_pCurFBufPos, &p);
		m_pCurFBufPos = p;
		irec.y = strtod(++m_pCurFBufPos, &p);
		m_pCurFBufPos = p;
		irec.speed = strtoul(++m_pCurFBufPos, &p, 10);
		m_pCurFBufPos = p;
		irec.direct = strtoul(++m_pCurFBufPos, &p, 10);
		m_pCurFBufPos = p;
		irec.valid = strtoul(++m_pCurFBufPos, &p, 10);
		m_pCurFBufPos = p;
#ifndef DEBUG
		if (++i == 1 || i == 20200)
			cout << "(" << irec.cid << "," << irec.event << ","
				<< irec.status << "," << irec.time << ","
				<< irec.x << "," << irec.y << "," << irec.speed << ","
				<< irec.direct << "," << irec.valid << ")" << endl;
#endif
		if (!irec.valid || irec.status != NON_OCCUPIED) continue;
        if (m_itsp == 0xFF) m_itsp = getTSIndex(irec.time);
        if (processOrigRecord(irec)) {
            cerr << "process record failed" << endl;
            return -1;
        }
	}
	//m_bEOF = true;
	if (m_pmNTSRecordPool->size() >= m_nTransCount) {
		cout << "reach the max transition count ("
			<< m_pmNTSRecordPool->size() << m_nTransCount << ")" << endl;
		return 1;
	}
	return 0;
}

int Processor::processFileBuffer2() {
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
    //m_bEOF = true;
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
        sprintf(fname, "%08d-%04lu.dat", m_CurrentDate, m_itsp);
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
    if (++m_itsp == 24*60/m_nMinPerTS) {
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
	while (hasNextFile()) {
		cout << "========> processing [" << m_fileList.front()
			 << "]" << endl;
		if(readFileIntoMem(m_fileList.front().c_str()) <= 0)
			return -1;
		m_fileList.pop_front();
		int ret = processFileBuffer();
		if (ret == 1) { // 1 indicate needs transfer to next TS
			if (transferToNextTS()) {
				cerr << "transfer to next TS failed!" << endl;
				return -1;
			}
		} else if (ret == -1) {
			cerr << "process file buffer failed!" << endl;
			return -1;
		} else
			return 0;
	}
	/* if reach here, no more files can be supplied,
	 * dump the incomplete file and return
	 */
	m_itsp += 1000; // indicate that is the last 2 files are incomplete
	if (transferToNextTS() == -1) {
		cout << "dump last incomplete part failed" << endl;
		return -1;
	}
	if (dumpRecordsToFile()) {
		cerr << "dump to file failed" << endl;
		return -1;
	}
	cout << "dump to file succeed" << endl;

	return 1; // indicate there's no more files to be processed
}
		
Processor::~Processor() {
    m_fileList.clear();
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
}
