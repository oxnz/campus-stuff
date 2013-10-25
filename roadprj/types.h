#ifndef RTYPES_H
#define RTYPES_H

#include <stdint.h>

typedef uint32_t car_id; // 6 bits decimal
typedef uint8_t trigger_event;
typedef uint32_t car_status;
typedef uint64_t gps_time;
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    } out_time;
typedef uint64_t gps_x;
typedef uint64_t gps_y;
typedef struct {
    gps_x x;
    gps_y y;
} gps_coord;
typedef uint32_t gps_speed;
typedef uint32_t gps_direction;
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

typedef struct {
    car_id car_id;
    uint16_t event;
    uint16_t status;
    gps_time time;
    double x;
    double y;
    uint16_t speed;
    uint16_t direct;
    uint16_t valid;
} in_rec;

struct __orec_key {
    roadseg_id rid;
    car_id cid;
    bool operator< (const struct __orec_key& k) const {
        return rid < k.rid || (rid == k.rid && cid < k.cid);
    }
    bool operator == (const struct __orec_key& k) const {
        return rid == k.rid && cid == k.cid;
    }
};
typedef struct __orec_key orec_key;

typedef struct {
    uint16_t status;
    uint64_t time;
} orec_value;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
} rec_date;

struct arch_rec {
    roadseg_id rid;
    orec_value orecv;
};// archive record struct for serialization

typedef arch_rec arch_rec;
#endif
