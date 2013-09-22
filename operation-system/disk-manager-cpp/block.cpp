#include "block.h"

int Block::count = 0;

Block::Block(int uuid)
	: uuid(uuid)
{
	Block::count ++;
	DebugLog("construct block >> count=%d with uuid:%d", Block::count, uuid);
}

Block::~Block(void) {
	Block::count --;
	DebugLog("destruct block >> count=%d with uuid:%d", Block::count, uuid);
}

void* Block::read(int size) {
	return data;
}

void Block::write(void *data, int size) {
	this->data = data;
}
