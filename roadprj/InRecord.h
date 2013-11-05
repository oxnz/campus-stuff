#pragma once

#include "RType.h"

struct InRecord
{
	car_id car_id;
	uint16_t event;
	uint16_t status;
	gps_time time;
	double x;
	double y;
	uint16_t speed;
	uint16_t direct;
	uint16_t valid;
};

typedef InRecord in_rec;