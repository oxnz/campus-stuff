#pragma once

#include <exception>
class RExcept : public std::exception {
};

class RProcessorException : public RExcept {
};

class RDPoolException : public RExcept {
};
