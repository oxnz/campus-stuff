#include "RTime.h"


/*
RTime::RTime(void)
{
}
*/

// input sample: 20121101001636
RTime::RTime(gps_time time)
	: m_iYear(static_cast<uint16_t>(time/10000000000)),
	  m_iMonth(static_cast<uint8_t>(time%10000000000/100000000)),
	  m_iDay(static_cast<uint8_t>(time%100000000/1000000)),
	  m_iHour(static_cast<uint8_t>(time%1000000/10000)),
	  m_iMinute(static_cast<uint8_t>(time%10000/100)),
	  m_iSecond(static_cast<uint8_t>(time%100))
{
}

RTime::~RTime(void)
{
}
