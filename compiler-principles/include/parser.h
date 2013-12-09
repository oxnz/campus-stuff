/*
 * File: parser.h
 * Description: This file is part of the microcc project.
 * Copying: Copyright (C) 2013 Oxnz, All rights reserved.
 */

#pragma once

#include "common.h"

namespace MICROCC {
	class TokenTable;
	class CodeGen;
	class Parser {
	public:
		bool parse(TokenTable&, CodeGen&);
	private:
	};
}
