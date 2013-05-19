#include <iostream>
#include <list>

#include "diskmgt.h"

int main(int argc, char *argv[]) {
	DiskManager *dm(NULL);
	std::list<int> results;
	int ret(0);

	dm = new DiskManager(20, 5);
	if (!dm) {
		std::cout << "Disk Manger init failed" << std::endl;
		return 1;
	}
	ret = dm->request(results, 3);
	if (!ret)
		dm->status(true);
	else if (ret == -1)
		std::cout << "not enough space" << std::endl;
	else
		goto errOpt;
	for (int uuid: results)
		std::cout << "get block with uuid:" << uuid << std::endl;
	for (int uuid: results) {
		std::cout << "release block with uuid:" << uuid << std::endl;
		dm->release(uuid);
		results.pop_front();
	}
	ret = dm->request(results, 8);
	if (!ret)
		dm->status(true);
	else if (ret == -1)
		std::cout << "not enough space" << std::endl;
	else
		goto errOpt;
	for (int uuid: results) {
		std::cout << "release block with uuid:" << uuid << std::endl;
		dm->release(uuid);
		results.pop_front();
	}

	ret = dm->request(results, 33);
	if (!ret)
		dm->status();
	else if (ret == -1)
		std::cout << "not enough space" << std::endl;
	else
		goto errOpt;

	delete dm;

	return 0;
errOpt:
	std::cout << "*** an error encountered!" << std::endl;
	if (dm)
		ret = dm->status(true);
	delete dm;
	return ret;
}
