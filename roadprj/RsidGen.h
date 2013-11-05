#pragma once
#include "RTypes.h"
#include "RConstant.h"

#include <iostream>

namespace GPS {
const static gps_x x0 = 1150000000;
const static gps_x x1 = 1180000000;
const static gps_y y0 = 370000000;
const static gps_y y1 = 420000000;
}

/* gps_coord must be checked cause there are some error statistics
 */
inline roadseg_id get_rsid(const gps_coord& coord) {
    if (coord.x < GPS::x0 || coord.x > GPS::x1 ||
            coord.y < GPS::y0 || coord.y > GPS::y1) {
        std::cerr << "invalid gps coord (" << coord.x << ", " << coord.y << ")"
                     << std::endl;
     }
    return static_cast<roadseg_id>(((coord.x >> 9) << 32) | (coord.y >> 9));
}
