#pragma once

#include "token.h"
#include "common.h"

namespace MICROCC {
	/*
	 *               ACTION-GOTO TABLE
	 * --------------------------------------------------;
	 * STATUS|	ACTION   	| 		GOTO 	         	 |
	 * --------------------------------------------------;
	 *  	 | <id> | ( | ) | <expr> | <term> | <factor> |
	 * --------------------------------------------------;
	 *   0	 |  s1  | s1| A |   4    |   4    |    5     |
	 * --------------------------------------------------;
	 *   1   |  r1  |   |   |   5    |   3    |    1     |
	 * --------------------------------------------------;
	 */
	const int STATCNT(12); // statuc count
	const int TOKTYPCNT(9); // token type count

	enum class AGOP : int {
		A, // accept
		S, // shift
		R, // reduce
		G, // goto
		E, // error
	};

	typedef int Status;
	struct ActGoItem {
		AGOP op;
		Status stat;
	};

	ActGoItem ActGoTable[STATCNT][TOKTYPCNT] {
		{ // 0
			{AGOP::S, 5}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::S, 4}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::G, 1}, {AGOP::G, 2}, {AGOP::G, 3},
		},
		{ // 1
			{AGOP::E, 0}, {AGOP::S, 6}, {AGOP::E, 0},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::A, 0},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 2
			{AGOP::E, 0}, {AGOP::R, 2}, {AGOP::S, 7},
			{AGOP::E, 0}, {AGOP::R, 2}, {AGOP::R, 2},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 3
			{AGOP::E, 0}, {AGOP::R, 4}, {AGOP::R, 4},
			{AGOP::E, 0}, {AGOP::R, 4}, {AGOP::R, 4},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 4
			{AGOP::S, 5}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::S, 4}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::G, 8}, {AGOP::G, 2}, {AGOP::G, 3},
		},
		{ // 5
			{AGOP::E, 0}, {AGOP::R, 6}, {AGOP::R, 6},
			{AGOP::E, 0}, {AGOP::R, 6}, {AGOP::R, 6},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 6
			{AGOP::S, 5}, {AGOP::R, 4}, {AGOP::R, 4},
			{AGOP::S, 4}, {AGOP::R, 4}, {AGOP::R, 4},
			{AGOP::E, 0}, {AGOP::G, 9}, {AGOP::G, 3},
		},
		{ // 7
			{AGOP::S, 5}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::S, 4}, {AGOP::E, 0}, {AGOP::E, 0},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::G, 10},
		},
		{ // 8
			{AGOP::E, 0}, {AGOP::S, 6}, {AGOP::E, 0},
			{AGOP::E, 0}, {AGOP::S, 11}, {AGOP::E, 0},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 9
			{AGOP::E, 0}, {AGOP::R, 1}, {AGOP::S, 7},
			{AGOP::E, 0}, {AGOP::R, 1}, {AGOP::R, 1},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 10
			{AGOP::E, 0}, {AGOP::R, 3}, {AGOP::R, 3},
			{AGOP::E, 0}, {AGOP::R, 3}, {AGOP::R, 3},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
		{ // 11
			{AGOP::E, 0}, {AGOP::R, 5}, {AGOP::R, 5},
			{AGOP::E, 0}, {AGOP::R, 5}, {AGOP::R, 5},
			{AGOP::E, 0}, {AGOP::E, 0}, {AGOP::E, 0},
		},
	};
}
