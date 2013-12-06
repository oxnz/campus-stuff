/*
 * File: NZExcept.h
 * Description: Inherit from std except
 * Author: Oxnz <yunxinyi@gmail.com>
 * Copying: Copyright (C) 2013, All rights reserved.
 */
#ifndef NZEXCEPT_H
#define NZEXCEPT_H

#include <stdexcept>

namespace NZ {
    class NZBadArg : public std::logic_error
    {
    public:
    NZBadArg(const char* s)
            : std::logic_error(s) {}
    };
    class NZExcept : public std::runtime_error
    {
    public:
    NZExcept(const char* s)
            : std::runtime_error(s) {}
    };
}

#endif
