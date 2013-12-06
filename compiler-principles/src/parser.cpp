/*
 * File: syntax.cpp
 */

/*
 * <microc> ::= <program> // accpet sign
 * <program> ::= <func><program> | <stmt><program> | <main_func>
 * <if_stmt> ::= if <condition> <stmt> [else <stmt>] fi
 * <for_stmt> ::= for (<declaration>; <condition>; <stmt>) <block>
 * <while_stmt> ::= while (<condition>) <block>
 * <block> ::= '{' <stmts> '}'
 * <do_stmt> ::= do stmt while (<condition>);
 * <condition> ::= <expresstion> {>|>=|<|<=|!= <expr>}
 * <expr> ::= <expr>+<term> | <term>-<term> | -<term> | <term>
 * <term> ::= <term>*<factor> | <term>/<factor> | <term>%<factor> | <factor>
 * <factor> ::= (<expr>) | <id>
 * <stmt> ::= <decl> | <assign> | <do_stmt> | <for_stmt>
 * 				| <while_stmt> | <if_stmt> | <block>
 * <decl> ::= <type><assign> | <type><id>';'
 * <assign> ::= <id> = {<id>|<num>|<expr>};
 * <stmts> ::= <stmt>*
 * <main_func> ::= int main'('')' <block>
 * <function> ::= <type> <id>(<arg>(, <arg>)) <block>
 * <type> ::= int | double | bool
 * <id> ::= [a-zA-Z][a-zA-Z0-9_]*
 * <arg> ::= <type><id>
 * <args> ::= <arg>'(', <arg>')'*
 */
#include "parser.h"
#include "grammer.h"
#include "token.h"
#include "codegen.h"

using namespace std;

bool main_func();
bool program() {
	return main_func();
	return false;
}

bool
MICROCC::Parser::parse(const TokenTable& ttb, MCodeTable& mctbl) {
	cout << endl << "parsing" << endl;
	MidCode mc;
	mc.op = MICROCC::MidCode::OP::add;
	mc.operand1 = "a";
	mc.operand2 = "b";
	mc.result = "tmp1";
	mctbl.push_back(mc);
	mc.op = MICROCC::MidCode::OP::mul;
	mctbl.push_back(mc);
	return program();
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
