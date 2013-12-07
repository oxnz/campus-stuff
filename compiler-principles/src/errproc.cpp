#include "errproc.h"
#include "token.h"
#include "codegen.h"

#include <cstdarg>
#include <cstdio>

namespace MICROCC {
	int ecode = 0;

	const char* errmsg[] = {
		"success",
		"invalid argument(s)",
		"invalid ident",
		"invalid expression",
		"invalid factor",
		"invalid term",
	};

	enum class ErrType {
		LexicalErr,
		SyntaxErr,
		SemanticErr,
	};

	int errproc(ErrType etype, const char* fmt, ...) {
		switch (etype) {
			case ErrType::LexicalErr:
				std::fprintf(stderr, "*** lexical error: ");
				break;
			case ErrType::SyntaxErr:
				std::fprintf(stderr, "*** syntax error: ");
				break;
			case ErrType::SemanticErr:
				std::fprintf(stderr, "*** semantic error: ");
				break;
		}
		va_list args;
		va_start(args, fmt);
		int ret = std::vfprintf(stderr, fmt, args);
		va_end(args);
		std::fprintf(stderr, "\n");
		return ret;
	}
}


void
MICROCC::lexicalError(const Position& pos, const char* fmt, ...) {
	std::fprintf(stderr, "*** lexical error (line: %d, column: %d): ",
			pos.row, pos.col);
	va_list args;
	va_start(args, fmt);
	std::vfprintf(stderr, fmt, args);
	va_end(args);
	std::fprintf(stderr, "\n");
}

void
MICROCC::syntaxError(const Token& tok, const char* fmt, ...) {
	std::fprintf(stderr, "*** syntax error (line: %d, column: %d, token: ",
			tok.m_pos.row, tok.m_pos.col);
	std::cerr << tok << "): ";
	va_list args;
	va_start(args, fmt);
	std::vfprintf(stderr, fmt, args);
	va_end(args);
	std::fprintf(stderr, "\n");
}

void
MICROCC::syntaxError(const StackNode& stknode, const char* fmt, ...) {
	std::fprintf(stderr, "*** syntax error (line: %d, column: %d, stat: %d): ",
			stknode.m_pos.row, stknode.m_pos.col, stknode.stat);
	std::cerr << "token: " << stknode;
	va_list args;
	va_start(args, fmt);
	std::vfprintf(stderr, fmt, args);
	va_end(args);
	std::cerr << std::endl;
}

/*
void MICROCC::semanticError(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MICROCC::errproc(ErrType::SemanticErr, fmt, args);
	va_end(args);
}
*/
