#pragma once

#include <list>

#include "token.h"
#include "common.h"

namespace MICROCC {
	class TokenTable;
	int lex(const char* buf, size_t len, TokenTable& toktbl);
};
