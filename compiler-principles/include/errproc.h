#pragma once

#include "common.h"

namespace MICROCC {
	struct Position;
	void lexicalError(const Position& pos, const char* fmt, ...);
	struct Token;
	void syntaxError(const Token& tok, const char* fmt, ...);
	struct StackNode;
	void syntaxError(const StackNode& stknode, const char* fmt, ...);
	void semanticError(const char* fmt, ...);
	int errproc(const char* fmt, ...);
}
