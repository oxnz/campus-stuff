/*
 * File: RDPool.cpp
 * Description: Road Data Pool impl
 * Author: Oxnz
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12
 */

#include "RDPool.h"
#include "NZLogger.h"
#include "RHelper.h"

#include <fstream>

#ifdef DEBUG
#include <cassert>
#endif

using NZ::NZLogger;
using std::cout;
using std::endl;
using std::to_string;
using std::ofstream;
using std::ios;
using std::string;

RDP::RDPool::RDPool(size_t nrs, size_t nts)
    : m_nrs(nrs),
      m_nts(nts),
      m_pp(new car_count[nrs*nts])
{
    NZLogger::log(NZ::DEBUG, "creating RDP (nts = %u, nrs = %u) ...",
			m_nts, m_nrs);
    for (size_t i = 0; i < nrs*nts; ++i)
        *(m_pp + i) = 0;
    NZLogger::log(NZ::DEBUG, "RDP created");
}

car_count* RDP::RDPool::operator[](roadseg_id rsid) {
    if (rsid <= 0 || rsid > m_nrs) {
        NZLogger::log(NZ::FATAL, "invalid rsid: %u", rsid);
        return nullptr;
    }
    return m_pp + (rsid-1) * m_nts;
}

car_count& RDP::RDPool::operator()(roadseg_id rsid, ts_index tsi) {
    if (rsid <= 0 || rsid > m_nrs || tsi < 0 || tsi >= m_nts)
        NZLogger::log(NZ::FATAL,
				"invalid rsid or TS index: rsid(%u), tsi(%u)", rsid, tsi);
    return (m_pp + (rsid-1)*m_nts)[tsi];
}

int RDP::RDPool::process(const std::set<orec_key>* ptsm) {
    NZLogger::log(NZ::INFO, "RDP processing ...");
    for (ts_index i = 0; i < m_nts; ++i) {
        for (std::set<orec_key>::const_iterator it =
                 ptsm[i].begin(); it != ptsm[i].end(); ++it) {
            if (*it <= 0 || *it >= m_nrs) {
                NZLogger::log(NZ::DEBUG, "invalid rsid: %u", *it);
            } else
                ++((m_pp + ((*it)-1) * m_nts)[i]);
        }
    }
    NZLogger::log(NZ::INFO, "RDP process end");    
    return 0;
}

int RDP::RDPool::dump(const string& fpath) {
    NZLogger::log(NZ::INFO, "RDP dumping to file [%s] ...", fpath);
    ofstream outfile(fpath, ios::out|ios::binary);
    if (!outfile.is_open()) {
        NZLogger::log(NZ::FATAL, "cannot open file [%s]", fpath);
        return -1;
    }
    for (roadseg_id i = 0; i < m_nrs; ++i) {
        RHelper::print_progress(i*100/m_nrs);
        for (ts_index j = 0; j < m_nts; ++j) {
            outfile.write(reinterpret_cast<const char*>(m_pp + i*m_nts + j),
                          sizeof(car_count));
        }
    }
    RHelper::print_progress(100);
    outfile.close();
    NZLogger::log(NZ::INFO, "RDP dump success");
    return 0;
}

RDP::RDPool::~RDPool() {
    delete m_pp;
}
