#ifndef RTYPES_H
#define RTYPES_H
#include <stdint.h>
typedef uint32_t car_id;
typedef uint32_t trigger_event;
typedef uint32_t car_status;
typedef uint64_t gps_time;
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
typedef uint32_t road_id;

#endif
