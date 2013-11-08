/*
 * File: RProcess.h
 * Description: main pre processor source file
 * Author: Oxnz
 * Version: 1.1
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12
 */

#include "RProcessor.h"
#include "RsidGen.h"
#include "RConstant.h"
#include "RHelper.h"

#include "NZLogger.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;
using NZ::NZLogger;

R::Processor::Processor(const char* indir, const char* outdir,
                        size_t minPerTS, size_t bufsize, bool process)
    : m_indir(indir),
      m_outdir(outdir),
      m_nMinPerTS(minPerTS),
      m_nTSCnt(24*60/minPerTS),
      m_tsp(0),
      m_nBufSize(bufsize),
      m_bProcess(process),
      m_pRDPool(process ? new RDP::RDPool(RsidGen::MAX_RSID, 24*60/minPerTS)
                : 0)
{
    if (m_indir.length() * m_outdir.length() == 0)
        throw logic_error("invalid parameter: indir or outdir is null");
    if (m_nMinPerTS < 2)
        throw logic_error("invalid parameter: MinPerTS too small");
    else if (24*60%m_nMinPerTS)
        throw logic_error("invalid paramter: 24*60 mod MinPerTS != 0");
    if (m_nBufSize <= 1024*1024)
        throw logic_error("invalid paremter: buf size is too small");
    if (*m_indir.rbegin() != '/')
        m_indir.append("/");
    if (*m_outdir.rbegin() != '/')
        m_outdir.append("/");
    m_pFileBuffer = new char[m_nBufSize];
    m_pTSPool = new map<const orec_key, void*>[24*60/minPerTS];
}

/*
 * @description: get time slot index
 */
inline size_t R::Processor::getTSIndex(const gps_time& time) {
    uint16_t h = static_cast<uint16_t>(time % 1000000/10000);
    uint16_t m = time % 10000/100;
    uint16_t s = time % 100;
    return (h*60*60+m*60+s)/(m_nMinPerTS*60);
}

inline int R::Processor::processOrigRecord(const in_rec& rec) {
    NZLogger::log(NZ::DEBUG, to_string(rec.cid) + "," + to_string(rec.event)
                  + "," + to_string(rec.status) + "," + to_string(rec.time)
                  + "," + to_string(rec.x) + "," + to_string(rec.y)
                  + "," + to_string(rec.speed) + "," + to_string(rec.direct)
                  + "," + to_string(rec.valid) + ")");
    gps_coord coord = {static_cast<gps_x>(rec.x * 10000000),
                       static_cast<gps_y>(rec.y * 10000000)};
    orec_key key = {RsidGen::get_rsid2(coord), rec.cid};
    /*
     * @advice: skip the wrong road id
     */
    if (key.rsid == 0) {
        NZLogger::log(NZ::WARNING, "invalid road segment ID");
        return 0;
    }
    if (abs(static_cast<int64_t>(rec.time - m_tsp)/10000)) {
        NZLogger::log(NZ::DEBUG, "invalid timestamp: " + to_string(rec.time)
                      + " - current time: " + to_string(m_tsp) + " = "
                      + to_string(abs(static_cast<int64_t>(rec.time - m_tsp)
                                      /10000)));
        return 0;
    } else {
        m_tsp = rec.time;
    }
     
    m_pTSPool[getTSIndex(rec.time)
              ].insert(make_pair(key, static_cast<void*>(0)));
    NZLogger::log(NZ::DEBUG, "roadseg_id: " + to_string(key.rsid) + " car_id: "
                  + to_string(key.cid));
    return 0;
}

int R::Processor::processFileBuffer() {
	char* p;
#ifdef DEBUG
	int i = 0;
#endif
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
#ifdef DEBUG
		if (++i == 1 || i == 20200)
			cout << "DEBUG: (" << irec.cid << "," << irec.event << ","
				<< irec.status << "," << irec.time << ","
				<< irec.x << "," << irec.y << "," << irec.speed << ","
				<< irec.direct << "," << irec.valid << ")" << endl;
#endif
		if (!irec.valid || irec.status != NON_OCCUPIED) continue;
        if(processOrigRecord(irec)) {
            cerr << "ERROR: process record failed, record:"
                 << "(" << irec.cid << "," << irec.event << ","
                 << irec.status << "," << irec.time << ","
                 << irec.x << "," << irec.y << "," << irec.speed << ","
                 << irec.direct << "," << irec.valid << ")" << endl;

            return -1;
        }
	}

	return 0;
}

ssize_t R::Processor::readFileIntoMem(const char* fpath) {
    NZLogger::log(NZ::INFO, string("reading [") + fpath + "] ...");
    ifstream infile(fpath);
    if (!infile.is_open()) {
        NZLogger::log(NZ::ERROR, string("open file [") +
                      fpath + "] failed");
        return -1;
    }
    infile.seekg(0, ios::end);
    size_t fsize = infile.tellg();
    if (fsize > m_nBufSize) {
        NZLogger::log(NZ::ERROR, "file size is larger than the buffer size");
        return -1;
    }
    m_pFileBufEnd = m_pFileBuffer + fsize;
    m_pCurFBufPos = (char *)m_pFileBuffer;
    NZLogger::log(NZ::INFO, "file size: " + to_string(fsize));
    infile.seekg(0, ios::beg);
    infile.read((char *)m_pFileBuffer, fsize);
    infile.close();
    return fsize;
}

int R::Processor::dumpRecords() {
    string fpath = m_outdir + to_string(m_tsp/1000000) + ".dat";
    ofstream outfile(fpath.c_str(), ios::out|ios::binary);
    if (!outfile.is_open()) {
        NZLogger::log(NZ::FATAL, "cannot open file [" + string(fpath) + "]");
        return -1;
    }
    ofstream outjson(fpath.append(".js").c_str(), ios::out|ios::app);
    if (!outjson.is_open()) {
        NZLogger::log(NZ::ERROR, "cannot open file [" + string(fpath)
                      + ".js]");
        return -1;
    }
    outjson << "var data = new Array(";
    NZLogger::log(NZ::INFO, "dumping to file [" + string(fpath) + "] ...");

    size_t cnt;
    roadseg_id x;
    for (size_t i = 0; i < m_nTSCnt; ++i) {
        cnt = 0;
        x = roadseg_id(i);
        outfile.write(reinterpret_cast<const char*>(&x),
                      sizeof(roadseg_id));
        for (map<const orec_key, void*>::iterator it = m_pTSPool[i].begin();
             it != m_pTSPool[i].end(); ++it) {
            ++cnt;
            outfile.write(reinterpret_cast<const char*>(&it->first.rsid),
                          sizeof(roadseg_id));
        }
        m_pTSPool[i].clear();
        x = RsidGen::INVALID_RSID;
        outfile.write(reinterpret_cast<const char*>(&x),
                      sizeof(roadseg_id));
        x = roadseg_id(cnt);
        outfile.write(reinterpret_cast<const char*>(&x),
                      sizeof(roadseg_id));
        if (i % 10 == 0) {
            cout << endl << setw(6) << left << i << "  ";
            outjson << endl;
        }
        cout << setw(6) << setfill(' ') << left << cnt << " ";
        outjson << cnt << ",";
    }
    cout << endl;
    NZLogger::log(NZ::INFO, "dump to file [" + string(fpath)
                  + "] successfully");
    outfile.close();
    // erase last comma
    outjson << "\b);" << endl;
    outjson.close();
    return 0;
}

int R::Processor::process(uint32_t date, size_t len, bool progbar) {
    string indir;
    int ret(0);
    int fcnt(0);
    for (size_t i = 0; i < len; ++i, ++date) {
        m_tsp = date;
        m_tsp *= 1000000;
        indir = m_indir + to_string(date);
        ret = RHelper::find_files(indir.c_str(),
                         to_string(date/100).c_str(),
                         m_fileList);
        if (ret == -1) {
            NZLogger::log(NZ::FATAL, "find files error");
            return -1;
        } else if (!ret) {
            NZLogger::log(NZ::WARNING, "no file was found");
            return 1;
        } else {
            NZLogger::log(NZ::INFO, "processing day " + to_string(date) + ", "
                          + to_string(ret) + " files, m_tsp = "
                          + to_string(m_tsp));
        }

        fcnt = m_fileList.size();
        while (m_fileList.size()) {
            if (progbar) {
                RHelper::print_progress((fcnt - m_fileList.size())*100/fcnt);
                cout << endl;
            }
            NZLogger::log(NZ::INFO, "processing " + m_fileList.front());
            if (readFileIntoMem(m_fileList.front().c_str()) <= 0) {
                NZLogger::log(NZ::ERROR, "read file into memory failed");
                return -1;
            }
            m_fileList.pop_front();
            ret = processFileBuffer();
            if (ret == -1) {
                NZLogger::log(NZ::ERROR, "process file buffer failed");
                return -1;
            }
        }
        if (progbar)
            RHelper::print_progress(100);
        if (m_bProcess && m_pRDPool->process(m_pTSPool)) {
            NZLogger::log(NZ::FATAL, "RDP process failed, skipped");
        }
        if (dumpRecords()) {
            NZLogger::log(NZ::FATAL, "dump to file failed");
            return -1;
        }
	}
    if (m_bProcess && m_pRDPool->dump(m_outdir + to_string(m_tsp/1000000)
                                      + ".rsd")) {
        NZLogger::log(NZ::FATAL, "RDP dump failed");
        return ret;
    }
    
	return ret;
}

R::Processor::~Processor() {
    if (m_fileList.size())
        m_fileList.clear();
    for (int i = 0; i < 480; ++i) {
        if (m_pTSPool[i].size())
            m_pTSPool[i].clear();
    }
    delete[] m_pTSPool;
    delete[] m_pFileBuffer;
    delete m_pRDPool;
}
