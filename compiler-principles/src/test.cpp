//#include <microcc.h>
//#include <functional>
#include <list>
#include <iostream>

using namespace std;

struct Position {
	Position(int r, int c) : row(r), col(c) {}
	const Position& operator++() {
		++col;
		return *this;
	}
	int row;
	int col;
};

int main() {
	Position pos(0, 0);
	++pos;
	cout << pos.row << pos.col << endl;
	return 0;
}
