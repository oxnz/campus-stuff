/*
 * File: syntax.h
 * Description: This file is part of the microcc project.
 * Copying: Copyright (C) 2013 Oxnz, All rights reserved.
 */

/*
 * program ::= {otherfunc|var}|mainfunc EOF
 * declare ::= vardeclare | funcdeclare
 * vardeclare ::= type identifer { = constant | expression } + ;
 * funcdelcare ::= type identifer ( type* ) ;
 * funcdefine ::= type identifer ( type* ) { statement* }
 * identifer ::= char {char | digit}*
 */
#pragma once

namespace MICROCC {
	class TokenTable;
	class Parser {
	public:
		bool parse(const TokenTable&);
	private:
	};
}
