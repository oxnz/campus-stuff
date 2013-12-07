#include "errproc.h"

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

void MICROCC::lexicalError(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MICROCC::errproc(ErrType::LexicalErr, fmt, args);
	va_end(args);
}


void MICROCC::syntaxError(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MICROCC::errproc(ErrType::SyntaxErr, fmt, args);
	va_end(args);
}

void MICROCC::semanticError(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MICROCC::errproc(ErrType::SemanticErr, fmt, args);
	va_end(args);
}
