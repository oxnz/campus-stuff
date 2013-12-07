/*
 * File: calc.cpp
 * Description: a simple calculator
 * Author: Oxnz <yunxinyi@gmail.com>
 * Last-updated: 2013-11-17 22:50:55 CST
 * Copying: Copyright (C) 2013, All rights reserved.
 */

#include <iostream>
#include <map>
#include <ctype.h>
#include <cmath>

enum class TokenType {
    BEGIN,
	HELP,
	NUM,
	IDENT,
	ASSIGN,
	LOG,
	EXP,
	SIN,
	COS,
	TAN,
	ABS,
	ADD,
	SUB,
	MUL,
	DIV,
	LPAREN,
	RPAREN,
	COMMA,
	UNKNOWN,
	EOL,
	END,
};

using namespace std;

struct Token {
	TokenType type;
	string ident;
	double value;
};

const double ZERO (1e-6);
Token NEXT_TOKEN;
map<string, double> IDENT_MAP;

void syntax_error(char c) {
	cerr << "unexpected character: " << c << endl;
	exit(1);
}

void syntax_error(const char* msg) {
    cerr << msg << endl;
    exit(1);
}

void lex() {
	Token t;
	char c;
	while ((c = cin.get()) != EOF && (c == ' ' || c == '\t'))
		;
    if (cin.eof()) {
        t.type = TokenType::END;
    } else if (isdigit(c)) {
        t.value = c - '0';
        while (isdigit(cin.peek()))
			t.value = t.value*10 + cin.get() - '0';
		if (cin.peek() == '.') {
			cin.get();
			int n;
			for (n = 0; isdigit(cin.peek()); ++n)
				t.value = t.value*10 + cin.get() - '0';
			t.value /= pow(10, n);
		}
		t.type = TokenType::NUM;
	} else if (isalpha(c)) {
        t.ident += c;
		while (isalnum(cin.peek()) || cin.peek() == '_')
            t.ident += cin.get();
		if (t.ident == "help") {
			t.type = TokenType::HELP;
		} else if (t.ident == "log") {
			t.type = TokenType::LOG;
        } else if (t.ident == "exp") {
            t.type = TokenType::EXP;
        } else if (t.ident == "sin") {
            t.type = TokenType::SIN;
        } else if (t.ident == "cos") {
            t.type = TokenType::COS;
        } else if (t.ident == "tan") {
            t.type = TokenType::TAN;
		} else if (t.ident == "abs") {
			t.type = TokenType::ABS;
		} else {
			t.type = TokenType::IDENT;
		}
	} else {
        switch (c) {
        case '=':
            t.type = TokenType::ASSIGN;
            break;
        case '+':
            t.type = TokenType::ADD;
            t.value = '+';
            break;
        case '-':
            t.type = TokenType::SUB;
            t.value = '-';
            break;
        case '*':
            t.type = TokenType::MUL;
            t.value = '*';
            break;
        case '/':
            t.type = TokenType::DIV;
            t.value = '/';
            break;
        case '(':
            t.type = TokenType::LPAREN;
            break;
        case ')':
            t.type = TokenType::RPAREN;
            break;
        case ',':
            t.type = TokenType::COMMA;
            break;
        case '\n':
            t.type = TokenType::EOL;
            break;
        default:
            syntax_error(c);
            break;
        }
    }
    NEXT_TOKEN = t;
    return;
}

double expr();
double term();

/*
 * <ident> ::= <id> | id = <expr>
 */
double ident() {
    double ret;
    string name = NEXT_TOKEN.ident;
    map<string, double>::iterator it = IDENT_MAP.find(name);
    lex();
    if (NEXT_TOKEN.type == TokenType::ASSIGN) {
        lex();
        ret = expr();
        if (it == IDENT_MAP.end()) { // not exist
            IDENT_MAP.insert(make_pair(name, ret));
        } else {
            it->second = ret;
        }
    } else {
        if (it == IDENT_MAP.end()) {
            cerr << "refer to an undeclared identifer" << endl;
            exit(1);
        } else
            ret = it->second;
    }
    return ret;
}

/*
 * <log> ::= log(<expr>,<expr>)
 */
double nzlog() {
    lex();
    if (NEXT_TOKEN.type != TokenType::LPAREN) {
        cerr << "missing '(' after log" << endl;
        exit(1);
    }
    lex();
    double base = expr();
    if (NEXT_TOKEN.type != TokenType::COMMA) {
        cerr << "missing ',' in log" << endl;
        cout << "[" << static_cast<int>(NEXT_TOKEN.type) << "]" << endl
             << "base = " << base << endl;
        exit(1);
    }
    lex();
    double exp = expr();
    if (NEXT_TOKEN.type != TokenType::RPAREN) {
        cerr << "missing ')' after log" << endl;
        exit(1);
    }
    lex();
    if ((base <= ZERO && (base - 1) < ZERO) || exp < ZERO || log(base) < ZERO) {
        cerr << "semantic error: log(base, exp) need:" << endl
             << "base > 0 && base != 1 && exp > 0 && log(base) != 0" << endl;
        exit(1);
    }
    return log(exp)/log(base);
}

/*
 * <exp> ::= exp(<expr>, <expr>)
 */
double nzexp() {
    lex();
    if (NEXT_TOKEN.type != TokenType::LPAREN) {
        cerr << "missing '(' after exp" << endl;
        exit(1);
    }
    lex();
    double base = expr();
    if (NEXT_TOKEN.type != TokenType::COMMA) {
        cerr << "missing ',' in exp" << endl;
        exit(1);
    }
    lex();
    double exp = expr();
    if (NEXT_TOKEN.type != TokenType::RPAREN) {
        cerr << "missing ')' after exp" << endl;
        exit(1);
    }
    lex();
    return pow(base, exp);
}

/*
 * <sin> ::= sin(<expr>)
 */
double nzsin() {
    lex();
    if (NEXT_TOKEN.type != TokenType::LPAREN) {
        cerr << "missing '(' after sin" << endl;
        exit(1);
    }
    lex();
    double ret = expr();
    if (NEXT_TOKEN.type != TokenType::RPAREN) {
        cerr << "missing ')' after sin" << endl;
        exit(1);
    }
    lex();
    return sin(ret);
}

/*
 * <cos> ::= cos(<expr>)
 */
double nzcos() {
    lex();
    if (NEXT_TOKEN.type != TokenType::LPAREN) {
        cerr << "missing '(' after cos" << endl;
        exit(1);
    }
    lex();
    double ret = expr();
    if (NEXT_TOKEN.type != TokenType::RPAREN) {
        cerr << "missing ')' after cos" << endl;
        exit(1);
    }
    lex();
    return cos(ret);
}

/*
 * <tan> ::= tan(<expr>)
 */
double nztan() {
    lex();
    if (NEXT_TOKEN.type != TokenType::LPAREN) {
        cerr << "missing '(' after tan" << endl;
        exit(1);
    }
    lex();
    double ret = expr();
    if (NEXT_TOKEN.type != TokenType::RPAREN) {
        cerr << "missing ')' after tan" << endl;
        exit(1);
    }
    lex();
    return tan(ret);
}

/*
 * <factor> ::= (<expr>) | num | <ident> | <log> | <exp> | <sin> |
 *              <cos> | <tan> | abs(<expr>)
 */
double factor() {
    double ret;
    switch (NEXT_TOKEN.type) {
    case TokenType::EOL:
        ret = 0;
        break;
    case TokenType::LPAREN:
        lex();
        ret = expr();
        if (NEXT_TOKEN.type == TokenType::RPAREN) {
            lex();
        } else {
            cerr << "missing ')'" << endl;
            exit(1);
        }
        break;
    case TokenType::NUM:        
        ret = NEXT_TOKEN.value;
        lex();
        break;
    case TokenType::IDENT:
        ret = ident();
        break;
    case TokenType::LOG:
        ret = nzlog();
        break;
    case TokenType::EXP:
        ret = nzexp();
        break;
    case TokenType::SIN:
        ret = nzsin();
        break;
    case TokenType::COS:
        ret = nzcos();
        break;
    case TokenType::TAN:
        ret = nztan();
        break;
    case TokenType::ABS:
        lex();
        if (NEXT_TOKEN.type != TokenType::LPAREN) {
            cerr << "missing '(' after abs" << endl;
            exit(1);
        }
        lex();
        ret = fabs(expr());
        if (NEXT_TOKEN.type != TokenType::RPAREN) {
            cerr << "missing ')' after abs" << endl;
            exit(1);
        }
        lex();
        break;
    default:
        cerr << "(" << __FUNCTION__ << ") unexpected token: "
             << "type = " << static_cast<int>(NEXT_TOKEN.type)
             << " ident = " << NEXT_TOKEN.ident
             << " value = " << NEXT_TOKEN.value << endl;
        exit(1);
        break;
    }
    return ret;
}

/*
 * <term> ::= <factor> { (*|/) <factor> }
 */
double term() {
	double ret = factor();
    while (true) {
        if (NEXT_TOKEN.type == TokenType::MUL) {
            lex();
            ret *= factor();
        } else if (NEXT_TOKEN.type == TokenType::DIV) {
            lex();
            double tmp = factor();
            if (fabs(tmp) < ZERO) {
                cerr << "divide by 0 error" << endl;
                exit(1);
            } else
                ret /= tmp;
        } else
            return fabs(ret) < ZERO ? 0 : ret;
    }
}
/*
 * <expr> ::= <term> {(+|-) term}
 */
double expr() {
    double ret = term();
    while (true) {
        if (NEXT_TOKEN.type == TokenType::ADD) {
            lex();
            ret += term();
        } else if (NEXT_TOKEN.type == TokenType::SUB) {
            lex();
            ret -= term();
        } else
            return ret;
    }
}

int help(int ecode) {
    (ecode == 0 ? cout : cerr)
        << "calc: a small calculator" << endl
        << "Usage: [options]" << endl
        << "  Options:" << endl
        << "    -h\tshow this help message and exit" << endl
        << "Copyright (C) 2013 Oxnz, All rights reserved"
        << endl;
    return ecode;
}

int main(int argc, char* argv[]) {
    if (argc > 1 && !strncmp(argv[1], "-h", 2))
        return help(0);
	cout << "calc 1.01" << endl
		<< "copyright (C) 2013 Oxnz, All rights reserved" << endl
		<< "type 'help' for more information" << endl;
    lex();
    do {
		if (NEXT_TOKEN.type == TokenType::HELP) {
			help(0);
			lex();
		}
        cout << expr() << endl;
        lex();
    } while (NEXT_TOKEN.type != TokenType::END);
	return 0;
}
