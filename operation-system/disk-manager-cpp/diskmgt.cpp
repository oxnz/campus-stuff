#include <list>
#include <iostream>
#include <stdexcept>

#include "diskmgt.h"
#include "block.h"

struct SuperBlock {
	int totalSize = 0;
	int groupSize = 0;
	std::list< std::list<Block*>* > * pool;
};

DiskManager::DiskManager(int totalSize, int groupSize) {
	DebugLog("init with totalSize=%d and groupSize=%d", totalSize, groupSize);
	try {
		SuperBlock *sb = new SuperBlock;
		sb->totalSize = totalSize;
		sb->groupSize = groupSize;
		sb->pool = new std::list< std::list<Block*>* >;
		int total = totalSize;
		for (int i = 0; i < (totalSize + 1) / groupSize; ++i) {
			auto group = new std::list<Block*>;
			for (int j = 0; total > 0 && j < groupSize; ++j, --total) {
				Block *block = new Block(i*groupSize+j);
				group->push_back(block);
			}
			sb->pool->push_back(group);
		}
		data = sb;
	}
	catch (std::exception e) {
		//throw out exception
		DebugLog("exception:%s", e.what());
		throw std::runtime_error("init failed due to block create");
	}
}

DiskManager::~DiskManager() {
	SuperBlock *sb = static_cast<SuperBlock*>(data);
	if (sb) {
		if (sb->pool) {
			while (!sb->pool->empty()) {
				auto group = sb->pool->front();
				for(auto block: *group)
					delete block;
				group->clear();
				delete group;
				sb->pool->pop_front();
			}
		}
		sb->pool->clear();
		delete sb->pool;
		sb->pool = NULL;
		delete sb;
		sb = NULL;
	}
}

int DiskManager::status(bool echo) {
	SuperBlock *sb = static_cast<SuperBlock*>(data);
	if (sb) {
		if (sb->pool) {
			if (echo)
				for (const auto group: *sb->pool) {
					std::cout << "[status] group has member(s) of:" << std::endl;
					for (const auto &block: *group)
						std::cout << "\tuuid: " << block->uuid << std::endl;
				}
			return 0;	// ok
		}
		return -1;	// sb->pool non-exist
	}
	return -2;	// sb non-exist
}

int DiskManager::request(std::list<int> &list, int size) {
	SuperBlock *sb = static_cast<SuperBlock*>(data);
	if (sb->totalSize < size) {
		// no enough space
		return -1;
	}
	while (size && sb->pool->size()) {
		for (auto i = sb->pool->begin(); size && i != sb->pool->end(); ++i) {
			auto group = *i;
			while (size && !group->empty()) {
				auto blockp = group->begin();
				if (group->size() > 1)
					++blockp;
				--size;
				list.push_back((*blockp)->uuid);
				delete *blockp;
				group->erase(blockp);
				--sb->totalSize;
			}
			if (group->empty())
				sb->pool->pop_front();
		}
	}
	return 0;
}

int DiskManager::release(int uuid) {
	SuperBlock *sb = static_cast<SuperBlock*>(data);
	++sb->totalSize;
	Block *blockp = new Block(uuid);
	std::list< std::list<Block*>* >::iterator i;
	for (i = sb->pool->begin(); i != sb->pool->end(); ++i) {
		if ((*i)->size() < sb->groupSize) {
			(*i)->push_back(blockp);
			return 0;
		}
	}

	if (i == sb->pool->end()) {
		std::list<Block*> *groupp = new std::list<Block*>;
		groupp->push_back(blockp);
		sb->pool->push_back(groupp);
	}
	return 0;
}
