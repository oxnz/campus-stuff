#pragma once

#include <vector>
#include <string>
/*
 * @description: find file specified path that matches pattern
 *
 * @parameters:
 *  -in		path		specify the path to look up
 *  -in		pattern		a string the file name contains
 *  -out	ovec		vector to store file paths, elements
 *  					are in ascending order
 * @returns:
 *  int		stands for found file count, 0 for end, and -1 for error
 */
int find_files(const char *path, const char* pattern,
                    std::vector<std::string>& out);
