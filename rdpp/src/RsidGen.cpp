/*
 * File: RsidGen.cpp
 * Description: Road Segment UUID Generator impl
 * Author: Oxnz
 * Version: 1.1
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12 CST
 */

#include "RsidGen.h"

/* gps_coord must be checked cause there are some error statistics
 */
roadseg_id RsidGen::get_rsid(const gps_coord& coord) {
    if (coord.x < RsidGen::GPS_X_MIN || coord.x > RsidGen::GPS_X_MAX ||
        coord.y < RsidGen::GPS_Y_MIN || coord.y > RsidGen::GPS_Y_MAX) {
        std::cerr << "ERROR: invalid gps coord (" << coord.x
                  << ", " << coord.y << ")" << std::endl;
        return RsidGen::INVALID_RSID;
    }
#ifdef DEBUG
    cout << "DEBUG: ix = " << (((coord.x-RsidGen::GPS_X_MIN) << 9)/
                               RsidGen::GPS_X_SCALE)
         << " , iy = " << ((((coord.y-RsidGen::GPS_Y_MIN) << 9)/
                            RsidGen::GPS_Y_SCALE)) << endl;
#endif
    return roadseg_id(((((coord.x - RsidGen::GPS_X_MIN) << 9)/
                        RsidGen::GPS_X_SCALE) << 16) |
                      (((coord.y - RsidGen::GPS_Y_MIN) << 9)/
                       RsidGen::GPS_Y_SCALE));
}


roadseg_id RsidGen::get_rsid2(const gps_coord& coord) {
    uint32_t xi(0), yi(0);
    while (xi < RsidGen::GPS_X_CNT && coord.x > RsidGen::GPS_X_REGION[xi])
        ++xi;
    if (!xi || xi == RsidGen::GPS_X_CNT)
        return RsidGen::INVALID_RSID;
    while (yi < RsidGen::GPS_Y_CNT && coord.y > RsidGen::GPS_Y_REGION[yi])
        ++yi;
    if (!yi || yi == RsidGen::GPS_Y_CNT)
        return RsidGen::INVALID_RSID;
    
    if (xi ==1 || xi == 6 || yi == 5) { // 2 km
        return roadseg_id((((((coord.x-RsidGen::GPS_X_MIN)<<8)/
                             RsidGen::GPS_X_SCALE)<<16) |
                           (((coord.y-RsidGen::GPS_Y_MIN)<<8)/
                            RsidGen::GPS_Y_SCALE))|(1<<31));
    } else if (xi == 3 && yi == 2) { // 128 m
        return roadseg_id(((((coord.x-RsidGen::GPS_X_MIN)<<11)/
                            RsidGen::GPS_X_SCALE)<<16) |
                          (((coord.y-RsidGen::GPS_Y_MIN)<<11)/
                           RsidGen::GPS_Y_SCALE)|(1<<15));
    } else { // 256 m
        return roadseg_id(((((coord.x-RsidGen::GPS_X_MIN)<<10)/
                            RsidGen::GPS_X_SCALE)<<16) |
                          (((coord.y-RsidGen::GPS_Y_MIN)<<10)/
                           RsidGen::GPS_Y_SCALE)|(1<<31)|
                          (1<<15));
    }
    return RsidGen::INVALID_RSID;
}
