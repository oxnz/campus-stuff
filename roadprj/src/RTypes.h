#pragma once

#include <stdint.h>
#include <sys/types.h>

#ifdef WIN32
typedef int _w64 ssize_t;
#endif

enum CAR_STATUS {
    NON_OCCUPIED = 0,
    OCCUPIED = 1,
    HALT = 2,
    STOPPED = 3,
    OTHER = 4
};

enum GPS_STATUS {
    INVALID = 0,
    VALID = 1
};

typedef uint32_t car_id; // 6 bits decimal
typedef uint8_t trigger_event;
typedef uint16_t car_status;
typedef uint64_t gps_time;
typedef uint64_t gps_x;
typedef uint64_t gps_y;
typedef struct {
    gps_x x;
    gps_y y;
} gps_coord;
typedef uint32_t gps_speed;
#define MAX_GPS_SPEED 255
#define MIN_GPS_SPEED 0
typedef uint32_t gps_direction;
#define MAX_GPS_DIRECTION 360
#define MIN_GPS_DIRECTION 0
typedef uint32_t gps_valid;
typedef struct {
    gps_time time;
    gps_coord coord;
    gps_speed speed;
    gps_direction direction;
    gps_valid valid;
} gps_info;
typedef struct {
    car_id id;
    trigger_event event;
    car_status status;
    gps_info gps;
} car_record;

typedef uint32_t roadseg_id;

typedef uint32_t rec_date;

typedef struct InRecord
{
    car_id cid;
    uint16_t event;
    uint16_t status;
    gps_time time;
    double x;
    double y;
    uint16_t speed;
    uint16_t direct;
    uint16_t valid;
} in_rec;

class OutRecord
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
