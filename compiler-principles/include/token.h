#pragma once

#include <iostream>
#include <list>

namespace MICROCC {
	enum class TokenType : uint32_t {
		// keywords
		BOOL, INT, DOUBLE, CHAR, STR, TRUE,	FALSE,
		INTVAL, // int constant
		DOUBLEVAL, // double constant
		CHARVAL, // char constant
		STRVAL, // string constant

		IF, ELIF, ELSE, FI,

		CASE, IN, ESAC,

		// identifer
		IDENTIFER,

		// operators
		ASSIGN, // =
		ADD, // +
		SUB, // -
		MUL, // *
		DIV, // /
		LT,	 // <
		LE,	 // <=
		NE,	 // !=
		EQ,	 // ==
		GT,  // >
		GE,  // >=
		NOT, // !

		COLON, // :
		AMP, // &
		CARET, // ^
		UNDERSCORE, // _
		BACKSLASH, // "\"
		DOT, // .
		PIPE, // |
		POUND, // #

		LSUBSCRIPT, // [
		RSUBSCRIPT, // ]

		// punctions
		LPAREN, // (
		RPAREN, // )
		LBRACKET, // {
		RBRACKET, // }
		QUOTE, // "
		TILDE, // ~
		// separators
		COMMA,
		SEMICOLON,
		SPACE,
		TAB,
		EOL,
		EOF_,
	};

	enum class Scope {
		GLOBAL = 0x80,
		LOCAL = 0x00,
	};

	typedef std::string TokenValue;

	struct Token {
		Token(TokenType type, std::string val);
		friend std::ostream& operator<<(std::ostream& os, const Token& t);
		TokenType m_type;
		TokenValue m_value;
	};

	class TokenTable : public std::list<Token> {
		public:
			TokenTable();
			virtual ~TokenTable();
		private:
	};
}
