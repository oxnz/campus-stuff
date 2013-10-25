#include "dummy.h"
#include "types.h"

#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int test_get_roadseg_id(void) {
    for (unsigned long i = 1160000000; i < 1170000000; ++i) {
        for (unsigned long j = 392600000; j < 410300000; j+=100) {
            gps_coord coord = {i, j};
            printf("x = %lu, y = %lu, roadseg_id = %u\n", i, j, get_roadseg_id(coord));
            //getchar();
        }
    }
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

int main(int argc, char *argv[]) {
    printf("testing get_roadseg_id:\n");
    //test_get_roadseg_id();
    if (test_read_preped_file("outfile") == -1) {
        cerr << "test read preprocessed file failed" << endl;
    }

    return -1;
}
