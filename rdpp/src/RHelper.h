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
#include <string>
#include "RTypes.h"

namespace RHelper {
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

inline size_t get_envi(gps_time t) {
	return 1;
}

int find_files(const char* path, const char* pattern,
                    std::list<std::string>& outlist);
int find_files(const char* path, const char* pattern,
               std::vector<std::string>& outvector);
void print_progress(size_t percent, const char* hint = nullptr);
//void print_progress(const char* hint, size_t percent);
}
