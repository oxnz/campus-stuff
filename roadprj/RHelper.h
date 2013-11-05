#pragma once

#include <vector>
#include <string>
/*
 * @description: find file specified path that matches pattern
 *
 * @parameters:
 *  -in
 * @returns:
 * 1 stands for found, 0 for end, and negtive for error
 */
int find_first_file(const char *path, const char* pattern,
                    std::vector<std::string>& out);


