#ifndef DUMMY_H
#define DUMMY_H

#include "types.h"

roadseg_id get_roadseg_id(gps_coord coord);
/*
 * @description: a wrapper interface between C++ <-> C#
 * @parameters:
 *	 -in	gps_coord: cooradinates of the point
 *	 -out	rsid	 : an pointer used for take out the results
 * @return:
 *		count roadseg ids, or -1 on fails
 * @notice: the array is static allocated, don't try to delete it
 */
ssize_t get_roadseg_id(gps_coord coord, roadseg_id* rsid);

#endif
