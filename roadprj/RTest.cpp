#include "RsidGen.h"
#include "RTypes.h"
#include "RHelper.h"

#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int test_get_roadseg_id(void) {
	uint32_t x,y;
	x=1163284302;
	y=399685555;

    gps_coord c = {x, y};
    roadseg_id rsid = get_rsid(c);
    cout <<rsid << endl;

    return 0;
}

int test_read_preped_file(const char *fpath) {
    ifstream infile(fpath, ios::in|ios::binary);
    if (!infile.is_open()) {
        cerr << "can't open infile" << endl;
        return -1;
    }
    arch_rec rec;
    for (int i = 0; i < 10; ++i) {
        infile.read(reinterpret_cast<char*>(&rec.rsid), sizeof(roadseg_id));
        infile.read(reinterpret_cast<char*>(&rec.orecv.status), sizeof(car_status));
        infile.read(reinterpret_cast<char*>(&rec.orecv.time), sizeof(gps_time));
        cout << "rsid: " << rec.rsid << " orecv status: " << rec.orecv.status
             << "time: " << rec.orecv.time << endl;
    }
    infile.close();
    return 0;
}

int test_find_files(void) {
	list<string> files;
	int ret = find_files("/Users/oxnz/Desktop/input", "2012", files);
	for (list<string>::iterator it = files.begin(); it != files.end(); ++it)
		cout << "file: " << *it << endl;
	cout << "found " << ret << " files" << endl;
	return 0;
}


int main(int argc, char *argv[]) {
	return test_find_files();
    printf("testing get_roadseg_id:\n");
    test_get_roadseg_id();
	return 0;
    if (test_read_preped_file("outfile") == -1) {
        cerr << "test read preprocessed file failed" << endl;
    }

    return -1;
}

