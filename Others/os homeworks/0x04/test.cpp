#include "utils.h"
#include "block.h"
#include "diskmgt.h"

int main(int argc, char *argv[]) {
	int i = 22;
	Block *x = new Block(i);
	Block &y = *x;
	delete x;

	return 0;
}
