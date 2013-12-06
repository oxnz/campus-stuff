/*
 * File: syntax.cpp
 */
#include "parser.h"
#include "token.h"

using namespace std;

bool MICROCC::Parser::parse(const TokenTable& ttb) {
	cout << "parsing" << endl;
	return false;
}
/*
 * <program> ::= <main_func>
 * <if_stmt> ::= if <condition> <stmt> (else <stmt> fi)
 * <for_stmt> ::= for (<declaration>; <condition>; <stmt>) <stmt>
 * <while_stmt> ::= while (<condition>) <stmt>
 * <do_stmt> ::= do stmt while (<condition>);
 * <condition> ::= <expresstion> (>|>=|<|<=|!= <expression>)
 * <expression> ::= <term>(+|-)<term>
 * <term> ::= <factor>(*|/|%)<factor>
 * <factor> ::= <num>|(<expression>)
 * <stmt> ::= <expression>; | <do_stmt> | <for_stmt>
 * 				| <while_stmt> | <if_stmt> | { <stmts> }
 * <stmts> ::= <stmt>*
 * <main_func> ::= int main() { <stmt>* }
 * <function> ::= <type> <ident>(<arg>(, <arg>)) { <stmt>* }
 * <type> ::= int | double | bool
 * <ident> ::= [a-zA-Z][a-zA-Z0-9_]*
 * <arg> ::= <type><ident>
 * <args> ::= <arg>(, <arg>)*
 */

bool main_func();
bool program() {
	return main_func();
	return false;
}

bool if_stmt() {
	return false;
}

bool for_stmt() {
	return false;
}

bool while_stmt() {
	return false;
}

bool do_stmt() {
	return false;
}

bool func() {
	return false;
}

bool main_func() {
	if ("main")
		return func();
	else
		return false;
}
