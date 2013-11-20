#pragma once

#include <cstddef>
#include <list>

#include "token.h"

namespace MICROCC {
	class TokenTable;
	int lex(const char* buf, size_t len, TokenTable& toktbl);
};
