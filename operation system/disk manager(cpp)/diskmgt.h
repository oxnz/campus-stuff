#ifndef __DMG_H__
#define __DMG_H__

#include <list>

class DiskManager {
public:
	DiskManager(int totalSize, int groupSize);
	~DiskManager();
	int status(const bool echo=false);
	int request(std::list<int> &result, int size);
	int release(int uuid);
private:
	void * data;
};

#endif//__DMG_H__
