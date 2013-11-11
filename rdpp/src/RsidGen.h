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
    const roadseg_id INVALID_RSID(0);
    const roadseg_id MAX_RSID(1<<18);
    const gps_x GPS_X_MIN(1153745330);
    const gps_x GPS_X_MAX(1175003570);
    const gps_y GPS_Y_MIN(394164260);
    const gps_y GPS_Y_MAX(410834980);
    const gps_x GPS_X_SCALE(GPS_X_MAX-GPS_X_MIN);
    const gps_y GPS_Y_SCALE(GPS_Y_MAX-GPS_Y_MIN);
	const roadseg_id GPS_X_STEP(GPS_X_SCALE >> 9);
	const roadseg_id GPS_Y_STEP(GPS_Y_SCALE >> 9);
	const gps_x GPS_X_INNER_MIN(1161509570);
	const gps_x GPS_X_INNER_MAX(1166824130);
	const gps_y GPS_Y_INNER_MIN(397452820);
	const gps_y GPS_Y_INNER_MAX(400578580);
	const gps_x GPS_X_INNER_SCALE(GPS_X_INNER_MAX-GPS_X_INNER_MIN);
	const gps_y GPS_Y_INNER_SCALE(GPS_Y_INNER_MAX-GPS_Y_INNER_MIN);
	const roadseg_id GPS_X_INNER_STEP(GPS_X_INNER_SCALE>>9);
	const roadseg_id GPS_Y_INNER_STEP(GPS_Y_INNER_SCALE>>8);

	const roadseg_id GPS_L_CNT(((GPS_X_INNER_MIN-GPS_X_MIN)/GPS_X_STEP)
			*((GPS_Y_INNER_MIN-GPS_Y_MIN)/GPS_Y_STEP));
	const roadseg_id GPS_ML_CNT(GPS_L_CNT
			+(((GPS_X_INNER_MIN-GPS_X_MIN)/GPS_X_STEP)
				*((GPS_Y_INNER_MAX-GPS_Y_INNER_MIN)/GPS_Y_STEP)));
	const roadseg_id GPS_MM_CNT(GPS_ML_CNT
			+(((GPS_X_INNER_MAX-GPS_X_INNER_MIN)/GPS_X_INNER_STEP)
				*((GPS_Y_INNER_MAX-GPS_Y_INNER_MIN)/GPS_Y_INNER_STEP)));
	const roadseg_id GPS_MR_CNT(GPS_MM_CNT
			+(((GPS_X_MAX-GPS_X_INNER_MAX)/GPS_X_STEP)
				*((GPS_Y_INNER_MAX-GPS_Y_INNER_MIN)/GPS_Y_STEP)));

	inline roadseg_id get_rsid(const gps_x& x, const gps_y& y) {
		if (x < GPS_X_MIN || x > GPS_X_MAX || y < GPS_Y_MIN || y > GPS_Y_MAX)
			return INVALID_RSID;
		return roadseg_id((x-GPS_X_MIN)/GPS_X_STEP
				+((y-GPS_Y_MIN)/GPS_Y_STEP));
	}

	inline roadseg_id get_rsid2(const gps_x& x, const gps_y& y) {
		if (x < GPS_X_MIN || x > GPS_X_MAX || y < GPS_Y_MIN || y > GPS_Y_MAX)
			return INVALID_RSID;
		else if (y < GPS_Y_INNER_MIN)
			return roadseg_id(((x-GPS_X_MIN)/GPS_X_STEP+1)
					*((y-GPS_Y_MIN)/GPS_Y_STEP+1));
		else if (y >= GPS_Y_INNER_MAX)
			return roadseg_id(GPS_MR_CNT
							+((x-GPS_X_MIN)/GPS_X_STEP+1)
							*((y-GPS_Y_INNER_MAX)/GPS_Y_STEP+1));
		else if (x < GPS_X_INNER_MIN)
			return roadseg_id(GPS_L_CNT
					+((x-GPS_X_MIN)/GPS_X_STEP+1)
					*((y-GPS_Y_INNER_MIN)/GPS_Y_STEP+1));
		else if (x >= GPS_X_INNER_MAX)
			return roadseg_id(GPS_MM_CNT
				+((x-GPS_X_INNER_MAX)/GPS_X_STEP+1)
				*((y-GPS_Y_INNER_MIN)/GPS_Y_STEP+1));
		else
			return roadseg_id(GPS_ML_CNT
					+((x-GPS_X_INNER_MIN)/GPS_X_INNER_STEP+1)
					*((y-GPS_Y_INNER_MIN)/GPS_Y_INNER_STEP+1));
	}

	/*
    const  size_t GPS_X_CNT(7);
    const  gps_x GPS_X_REGION[GPS_X_CNT] = {
        GPS_X_MIN, 1157277820, 1161560850, 1166281140, 1168806310, 1171340430,
        GPS_X_MAX };
    const size_t GPS_Y_CNT(6);
    const gps_y GPS_Y_REGION[GPS_Y_CNT] = {
        GPS_Y_MIN, 397479450, 400570710, 402664640, 405814250, GPS_Y_MAX };

	roadseg_id get_rsid_deprecated(const gps_x& x, const gps_y& y) {
		if (x < RsidGen::GPS_X_MIN || x > RsidGen::GPS_X_MAX ||
			y < RsidGen::GPS_Y_MIN || y > RsidGen::GPS_Y_MAX) {
			//std::cerr << "ERROR: invalid gps coord (" << x
			//		  << ", " << y << ")" << std::endl;
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

	const roadseg_id INNER_MAX(1 << 14); // 2^7 * 2^7 ~= 10000+
	const gps_x INNER_XSTEP = (GPS_X_REGION[4] - GPS_X_REGION[3]) >> 7;
	const gps_y INNER_YSTEP = (GPS_Y_REGION[4] - GPS_Y_REGION[3]) >> 7;
	const roadseg_id MIDDLE_MAX(INNER_MAX + (1 << 14)); // 10000+
	const gps_x MIDDLE_XSTEP = (GPS_X_REGION[5] - GPS_X_REGION[1]) >> 7;
	const gps_y MIDDLE_YSTEP = (GPS_Y_REGION[4] - GPS_Y_REGION[0]) >> 7;
	const roadseg_id OUTTER_MAX(MIDDLE_MAX + (1 << 14)); // 10000+
	const gps_x OUTTER_XSTEP = GPS_X_SCALE >> 7;
	const gps_y OUTTER_YSTEP = GPS_Y_SCALE >> 7;

	inline roadseg_id get_rsid4(const gps_x& x, const gps_y& y) {
		uint32_t xi(0), yi(0);
		while (xi < GPS_X_CNT && x > GPS_X_REGION[xi])
			++xi;
		if (!xi || xi == GPS_X_CNT)
			return INVALID_RSID;
		while (yi < GPS_Y_CNT && y > GPS_Y_REGION[yi])
			++yi;
		if (!yi || yi == GPS_Y_CNT)
			return INVALID_RSID;
		if (xi == 4 && yi == 4) // inner area
			return ((x - GPS_X_REGION[3])/INNER_XSTEP + 1)
				* ((y - GPS_Y_REGION[3])/INNER_YSTEP + 1);
		if (xi == 1 || xi == 6 || yi == 5) // outter area
			return MIDDLE_MAX
					+ ((x - GPS_X_MIN) / OUTTER_XSTEP + 1)
				* ((y - GPS_Y_MIN) / OUTTER_YSTEP + 1);
		else // middle
			return INNER_MAX
				+ ((x - GPS_X_REGION[1]) / MIDDLE_XSTEP + 1)
				* ((y - GPS_Y_REGION[0]) / MIDDLE_YSTEP + 1);
	}

		inline roadseg_id get_rsid2(const gps_x& x, const gps_y& y) {
			//std::cout << "(" << x << "," << y << ")" << std::endl;
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


	*/
	inline roadseg_id get_rsid(const gps_coord& coord) {
		return get_rsid(coord.x, coord.y);
	}
    inline roadseg_id get_rsid2(const gps_coord& coord) {
		return get_rsid2(coord.x, coord.y);
	}
}
