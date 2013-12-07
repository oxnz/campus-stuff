#pragma once

#include <iostream>
#include <list>

#include "common.h"

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
		IDENTIFIER,

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
		BEGIN,
		END,

		// non-terminals
		FUNC,

		EXPR,
		TERM,
		FACTOR,

	};

	typedef std::string TokenValue;

	struct Position {
		size_t row;
		size_t col;
		friend std::ostream& operator<<(std::ostream& os, const Position& pos);
	};

	struct Token {
		Token(TokenType type, const TokenValue& val = "nonset",
				const Position& p = {static_cast<size_t>(-1),
				static_cast<size_t>(-1)});
		friend std::ostream& operator<<(std::ostream& os, const Token& t);
		TokenType m_type;
		TokenValue m_value;
		Position m_pos;
	};

	class TokenTable : public std::list<Token> {
		public:
			TokenTable();
			virtual ~TokenTable();
			friend std::ostream& operator<<(std::ostream& os,
					const TokenTable& toktbl);
		private:
	};
}
