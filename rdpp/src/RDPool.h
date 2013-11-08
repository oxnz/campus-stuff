#pragma once

#include "RTypes.h"

#include <map>

namespace RDP {
    class RDPool {
    public:
        RDPool(size_t nrs, size_t nts);
        car_count* operator[](roadseg_id);
        car_count& operator()(roadseg_id, ts_index);
        int process(const std::map<const orec_key, void*>* m_pTSPool);
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
