#pragma once
#include "RTypes.h"
#include "RConstant.h"

#include <iostream>

namespace GPS {
    const  gps_x GPS_X_MIN(1153748770);
    const  gps_x GPS_X_MAX(1175000130);
    const  gps_x GPS_X_SCALE(GPS_X_MAX-GPS_X_MIN);
    const  gps_y GPS_Y_MIN(394166530);
    const  gps_y GPS_Y_MAX(410832710);
    const  gps_y GPS_Y_SCALE(GPS_Y_MAX-GPS_Y_MIN);
    const  size_t GPS_X_CNT(7);
    const  gps_x GPS_X_REGION[GPS_X_CNT] = {
        GPS_X_MIN, 1157277820, 1161560850, 1166281140, 1168806310, 1171340430,
        GPS_X_MAX };
    const static size_t GPS_Y_CNT(6);
    const static gps_y GPS_Y_REGION[GPS_Y_CNT] = {
        GPS_Y_MIN, 397479450, 400570710, 402664640, 405814250, GPS_Y_MAX };
    const static roadseg_id INVALID_RSID(0);
}

using namespace GPS;
using namespace std;
/* gps_coord must be checked cause there are some error statistics
 */
inline roadseg_id get_rsid(const gps_coord& coord) {
    /*
    if (coord.x < GPS::x0 || coord.x > GPS::x1 ||
            coord.y < GPS::y0 || coord.y > GPS::y1) {
    */
    if (coord.x < GPS_X_MIN || coord.x > GPS_X_MAX ||
        coord.y < GPS_Y_MIN || coord.y > GPS_Y_MAX) {
        std::cerr << "invalid gps coord (" << coord.x << ", " << coord.y << ")"
                  << std::endl;
        return INVALID_RSID;
    }
#ifdef DEBUG
    cout << "ix = " << (((coord.x-GPS_X_MIN) << 9)/GPS_X_SCALE)
         << " , iy = " << ((((coord.y-GPS_Y_MIN) << 9)/GPS_Y_SCALE)) << endl;
#endif
    return roadseg_id(((((coord.x - GPS_X_MIN) << 9)/GPS_X_SCALE) << 16) |
                      (((coord.y - GPS_Y_MIN) << 9)/GPS_Y_SCALE));
}


inline roadseg_id get_rsid2(const gps_coord& coord) {
    uint32_t xi(0), yi(0);
    while (xi < GPS_X_CNT && coord.x > GPS_X_REGION[xi])
        ++xi;
    if (!xi || xi == GPS_X_CNT)
        return INVALID_RSID;
    while (yi < GPS_Y_CNT && coord.y > GPS_Y_REGION[yi])
        ++yi;
    if (!yi || yi == GPS_Y_CNT)
        return INVALID_RSID;

    if (xi ==1 || xi == 6 || yi == 5) { // 2 km
        return roadseg_id((((((coord.x-GPS_X_MIN)<<8)/GPS_X_SCALE)<<16) |
                           (((coord.y-GPS_Y_MIN)<<8)/GPS_Y_SCALE))|(1<<31));
    } else if (xi == 3 && yi == 2) { // 128 m
        return roadseg_id(((((coord.x-GPS_X_MIN)<<11)/GPS_X_SCALE)<<16) |
                          (((coord.y-GPS_Y_MIN)<<11)/GPS_Y_SCALE)|(1<<15));
    } else { // 256 m
        return roadseg_id(((((coord.x-GPS_X_MIN)<<10)/GPS_X_SCALE)<<16) |
                          (((coord.y-GPS_Y_MIN)<<10)/GPS_Y_SCALE)|(1<<31)|
                          (1<<15));
    }
    return INVALID_RSID;
}
