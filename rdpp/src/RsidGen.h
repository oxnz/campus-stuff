/*
 * File: RsidGen.h
 * Description: Road Segment UUID Generator interface
 * Author: Oxnz
 * Version: 1.1
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12 CST
 */

#pragma once

#include "RTypes.h"

#include <iostream>

namespace RsidGen {
    using namespace std;
    const roadseg_id MAX_RSID(1<<18);
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

	roadseg_id get_rsid(const gps_x& x, const gps_y& y) {
		if (x < RsidGen::GPS_X_MIN || x > RsidGen::GPS_X_MAX ||
			y < RsidGen::GPS_Y_MIN || y > RsidGen::GPS_Y_MAX) {
			std::cerr << "ERROR: invalid gps coord (" << x
					  << ", " << y << ")" << std::endl;
			return RsidGen::INVALID_RSID;
		}
#ifdef DEBUG
		cout << "DEBUG: ix = " << (((x-RsidGen::GPS_X_MIN) << 9)/
								   RsidGen::GPS_X_SCALE)
			 << " , iy = " << ((((y-RsidGen::GPS_Y_MIN) << 9)/
								RsidGen::GPS_Y_SCALE)) << endl;
#endif
		return roadseg_id(((((x - RsidGen::GPS_X_MIN) << 9)/
							RsidGen::GPS_X_SCALE) << 16) |
						  (((y - RsidGen::GPS_Y_MIN) << 9)/
						   RsidGen::GPS_Y_SCALE));
		}

		inline roadseg_id get_rsid2(const gps_x& x, const gps_y& y) {
			uint32_t xi(0), yi(0);
		while (xi < RsidGen::GPS_X_CNT && x > RsidGen::GPS_X_REGION[xi])
			++xi;
		if (!xi || xi == RsidGen::GPS_X_CNT)
			return RsidGen::INVALID_RSID;
		while (yi < RsidGen::GPS_Y_CNT && y > RsidGen::GPS_Y_REGION[yi])
			++yi;
		if (!yi || yi == RsidGen::GPS_Y_CNT)
			return RsidGen::INVALID_RSID;
		
		if (xi ==1 || xi == 6 || yi == 5) { // 2 km
			return roadseg_id((((((x-RsidGen::GPS_X_MIN)<<8)/
								 RsidGen::GPS_X_SCALE)<<16) |
							   (((y-RsidGen::GPS_Y_MIN)<<8)/
								RsidGen::GPS_Y_SCALE))|(1<<31));
		} else if (xi == 3 && yi == 2) { // 128 m
			return roadseg_id(((((x-RsidGen::GPS_X_MIN)<<11)/
								RsidGen::GPS_X_SCALE)<<16) |
							  (((y-RsidGen::GPS_Y_MIN)<<11)/
							   RsidGen::GPS_Y_SCALE)|(1<<15));
		} else { // 256 m
			return roadseg_id(((((x-RsidGen::GPS_X_MIN)<<10)/
								RsidGen::GPS_X_SCALE)<<16) |
							  (((y-RsidGen::GPS_Y_MIN)<<10)/
							   RsidGen::GPS_Y_SCALE)|(1<<31)|
							  (1<<15));
		}
		return RsidGen::INVALID_RSID;
	}

	inline roadseg_id get_rsid(const gps_coord& coord) {
		return get_rsid(coord.x, coord.y);
	}
    inline roadseg_id get_rsid2(const gps_coord& coord) {
		return get_rsid2(coord.x, coord.y);
	}
}
