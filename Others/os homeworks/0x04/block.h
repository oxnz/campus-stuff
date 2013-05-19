#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "utils.h"

class Block {
public:
	Block(int uuid);
	Block(const Block &block) = delete;
	Block& operator = (const Block &) = delete;
	~Block(void);
	void* read(int size);
	void write(void *data, int size);
public:
	const int uuid;
private:
	static int count;
	void *data;
};

#endif//__BLOCK_H__
