#pragma once

#include <list>
#include <vector>
#include <string>
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
int find_files(const char* path, const char* pattern,
                    std::list<std::string>& outlist);
int find_files(const char* path, const char* pattern,
               std::vector<std::string>& outvector);
