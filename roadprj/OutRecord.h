#pragma once

#include "Record.h"
#include "RType.h"

class OutRecord : public Record
{
public:
	OutRecord(void);
	virtual ~OutRecord(void);
};

typedef struct OutRecordKey
{
	roadseg_id rsid;
    car_id cid;
	/*
	 * This operator must be implemented, or std::map will complain about it
	 */
    inline bool operator< (const struct OutRecordKey& rhs) const {
        return rsid < rhs.rsid || (rsid == rhs.rsid && cid < rhs.cid);
    }
    inline bool operator == (const struct OutRecordKey& rhs) const {
        return rsid == rhs.rsid && cid == rhs.cid;
    }
} orec_key;

typedef struct {
    uint16_t status;
    uint64_t time;
} orec_value;


typedef struct ArchiveRecord {
    roadseg_id rsid;
    orec_value orecv;
} arch_rec;// archive record struct for serialization
