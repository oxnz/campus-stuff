/*
 * File: RHelper.h
 * This file is part of the @PACKAGE@ project
 * Description: helper interface file, supplies services
 * Author: Oxnz
 * Version: @VERSION@
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12 CST
 */

#pragma once

#include <list>
#include <vector>
#include <cstring>
#include <string>
#include <ctime>
#include <cerrno>
#include "RTypes.h"

#include "NZLogger.h"

namespace RHelper {
	const size_t MAX_ENVC_CNT(4);	// environ conditions count
	const size_t DCNT_OF_ENV[MAX_ENVC_CNT] = {
		22,		// weekdays good condition
		4,		// saturday
		3,		// sunday
		1		// sunday: bad weather
	};
	const std::string FNAME_OF_ENV[MAX_ENVC_CNT] = {
		"0.rsd",
		"1.rsd",
		"2.rsd",
		"3.rsd",
	};
	/*
	 * @description: find file specified path that matches pattern
	 *
	 * @parameters:
	 *  -in		path		specify the path to look up
	 *  -in		pattern		a string the file name contains
	 *  -out	outlist		list to store file paths, elements
	 *  					are in ascending order
	 * @returns:
	 *  int		stands for found file count, 0 for end, and -1 for error
	 */
	inline ts_index getTSIndex(size_t mpts, gps_time t) {
		return ((t%1000000/10000)*60+t%10000/100)/mpts;
	}

	/* NOTICE: This function ignore any bad weather, cause the test cases
	 * didn't contain really bad weather days
	 */
	inline size_t get_envi(const gps_time& t) {
		time_t rawtime;
		if (time(&rawtime) == -1) {
			NZ::NZLogger::log(NZ::ERROR, "%s: %s", __FUNCTION__,
					strerror(errno));
			return -1;
		}
		struct tm* timeinfo = localtime(&rawtime);
		timeinfo->tm_year = t/10000000000 - 1900;
		timeinfo->tm_mon = t/100000000%100 - 1;
		timeinfo->tm_mday = t/1000000%100;
		mktime(timeinfo);
		NZ::NZLogger::log(NZ::DEBUG, "%d-%d-%d: %d(%d %d %d)", t/10000000000,
			t/100000000%100, t/1000000%100, timeinfo->tm_wday,
			timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday);
		if (!timeinfo->tm_wday) { // sunday good condition
			return 3; // bad condition
			return 2;
		}
		else if (timeinfo->tm_wday == 6) // saturday good condition
			return 1;
		else if (timeinfo->tm_wday) // 1-5: weekdays good condition
			return 0;
		return 0;
	}

	int find_files(const char* path, const char* pattern,
						std::list<std::string>& outlist);
	int find_files(const char* path, const char* pattern,
				   std::vector<std::string>& outvector);
	void print_progress(size_t percent, const char* hint = nullptr);
}
