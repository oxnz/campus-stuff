#pragma once

#include "common.h"

namespace MICROCC {
	void lexicalError(const char* fmt, ...);
	void syntaxError(const char* fmt, ...);
	void semanticError(const char* fmt, ...);
}
