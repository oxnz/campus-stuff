/*
 * File: RDPool.h
 * Description: Road Data Pool interface
 * Author: Oxnz
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12
 */

#pragma once

#include "RTypes.h"

#include <iostream>
#include <set>

namespace RDP {
    class RDPool {
    public:
        RDPool(size_t nrs, size_t nts);
        car_count* operator[](roadseg_id);
        car_count& operator()(roadseg_id, ts_index);
        int process(const std::set<orec_key>* m_pTSPool);
        int dump(const std::string& fpath);
        virtual ~RDPool();
    private:
        RDPool(const RDPool&);
        RDPool& operator=(const RDPool&);
    private:
        size_t m_nrs;
        size_t m_nts;
        car_count* const m_pp;
    };
}
