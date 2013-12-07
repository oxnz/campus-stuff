#include "token.h"
#include "errproc.h"
#include "lexical.h"

using namespace std;
using namespace MICROCC;

int
MICROCC::lex(const char* buf, size_t len, TokenTable& toktbl) {
	Position pos = {0, 0};
	for (size_t i = 0; i < len;) {
		TokenType type;
		TokenValue val;
		if (isdigit(buf[i])) { // constant
			type = TokenType::INTVAL;
			while (isdigit(buf[i])) {
				val += buf[i];
				++i;
				++pos.col;
			}
			if (buf[i] == '.') {
				if (isdigit(buf[i+1])) {
					type = TokenType::DOUBLEVAL;
					val += buf[i];
					++i;
					++pos.col;
				} else {
					lexicalError(pos, "extra dot after integer");
					return -1;
				}
			}
			while (isdigit(buf[i])) {
				val += buf[i];
				++i;
				++pos.col;
			}
		} else if (isalpha(buf[i])) { // identifer
			while (isalnum(buf[i])) {
				val += buf[i];
				++i;
				++pos.col;
			}
			if (val == "int")
				type = TokenType::INT;
			else if (val == "double")
				type = TokenType::DOUBLE;
			else if (val == "char")
				type = TokenType::CHAR;
			else if (val == "str")
				type = TokenType::STR;
			else if (val == "true")
				type = TokenType::TRUE;
			else if (val == "false")
				type = TokenType::FALSE;
			else if (val == "if")
				type = TokenType::IF;
			else if (val == "elif")
				type = TokenType::ELIF;
			else if (val == "else")
				type = TokenType::ELSE;
			else if (val == "fi")
				type = TokenType::FI;
			else
				type = TokenType::IDENTIFIER;
		} else {
			switch (buf[i]) {
				case '=':
					type = TokenType::ASSIGN;
					break;
				case '+':
					type = TokenType::ADD;
					break;
				case '-':
					type = TokenType::SUB;
					break;
				case '*':
					type = TokenType::MUL;
					break;
				case '/':
					if (buf[i+1] == '/') { // line comment
						while (buf[++i] != '\n')
							++pos.col; // cout << buf[i];
						++i;
						pos.col += 2;
						continue;
					} else if (buf[i+1] == '*') { // block comment
						for (i += 2; (buf[i] != '*' || buf[i+1] != '/') &&
								buf[i+1] != -1;
								++i, ++pos.col)
							; // cout << buf[i];
						if (buf[i+1] == -1) {
							lexicalError(pos, "'*/' not found");
							return -1;
						} else {
							i += 2;
							pos.col += 2;
							continue;
						}
					}
					type = TokenType::DIV;
					break;

				case '<':
					type = TokenType::LT;
					break;
				case '>':
					type = TokenType::GT;
					break;
				case '!':
					type = TokenType::NOT;
					break;
				case ':':
					type = TokenType::COLON;
					break;
				case '&':
					type = TokenType::AMP;
					break;
				case '^':
					type = TokenType::CARET;
					break;
				case '_':
					type = TokenType::UNDERSCORE;
					break;
				case '\\':
					type = TokenType::BACKSLASH;
					break;
				case '.':
					type = TokenType::DOT;
					break;
				case '|':
					type = TokenType::PIPE;
					break;
				case '#':
					type = TokenType::POUND;
					while (buf[++i] != '\n') {
						val += buf[i];
						++pos.col;
					}
					++pos.col;
					continue;
					break;
				case '[':
					type = TokenType::LSUBSCRIPT;
					break;
				case ']':
					type = TokenType::RSUBSCRIPT;
					break;
				case '(':
					type = TokenType::LPAREN;
					break;
				case ')':
					type = TokenType::RPAREN;
					break;
				case '{':
					type = TokenType::LBRACKET;
					break;
				case '}':
					type = TokenType::RBRACKET;
					break;
				case '"':
					type = TokenType::QUOTE;
					break;
				case '~':
					type = TokenType::TILDE;
					break;
				case ' ':
					++pos.col;
					++i;
					continue;
				case '\t':
					pos.col += 8;
					++i;
					continue;
				case '\r': // windows \r\n
					pos.col = 0;
					++i;
					continue;
				case '\n':
					pos.col = 0;
					++pos.row;
					/* skip any white space or newline */
					++i;
					continue;
					break;
				case ',':
					type = TokenType::COMMA;
					break;
				case ';':
					type = TokenType::SEMICOLON;
					break;
				case -1:
					type = TokenType::EOF_;
					break;
				default:
					lexicalError(pos, "unexpected character: %c", buf[i]);
					return -1;
			}
			++i;
			++pos.col;
		}
		toktbl.push_back(Token(type, val, pos));
	}
	return 0;
}
