#pragma once

#include "RType.h"


/* typedef struct RecordDate
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
} rec_date; */

typedef uint32_t rec_date;
//typedef uint32_t rec_time;

class RTime
{
public:
	//RTime(void);
	/*
	 * @description: construct from gps_time(uint64_t, example: 20121101001636)
	 */
	RTime(gps_time time);
	rec_date getRecordDate(void) { return m_iYear*10000+m_iMonth*100+m_iDay; }
	virtual ~RTime(void);
private:
	uint16_t m_iYear;
	uint8_t m_iMonth;
	uint8_t m_iDay;
	uint8_t m_iHour;
	uint8_t m_iMinute;
	uint8_t m_iSecond;
};

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} out_time;