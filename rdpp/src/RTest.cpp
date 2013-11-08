/*
 * File: RTest.h
 * Description: all test cases are here
 * Author: Oxnz
 * Version: 1.1
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12 CST
 */

#include "RsidGen.h"
#include "RTypes.h"
#include "RHelper.h"

#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;
using namespace RHelper;

#include <unistd.h>
#include "NZLogger.h"
using NZ::NZLogger;
int test_logger() {
    NZLogger::log(NZ::ERROR, "hello"
                  "world"
                  "123");
    return 0;
}

int output_html(void) {
    ofstream outfile("./out/test.html", ios::out);
    if (!outfile.is_open()) {
        cerr << "ERROR: cannot write to file test.html" << endl;
        return -1;
    }
    outfile << "<html>" << endl;
    outfile << "\bx" << endl;
    outfile.close();
    return 0;
}

int test_output_html() {
    return output_html();
}

int test_print_progress(void) {
    for (int i = 2; i < 480; ++i)
        print_progress(i*100/480+1);
    return 0;
}

int test_get_roadseg_id(void) {
	uint32_t x(1150000000), y(394000000);
    for (int i = 0; i < 14; ++i) {
        for (int j = 0; j < 10; ++j) {
            gps_coord c = {x, y};
            cout << "rsid (" << x << "," << y << ") = "
                 << RsidGen::get_rsid2(c) << endl;
            y += 2000000;
        }
        y -= 20000000;
        x += 2000000;
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

int test_find_files(void) {
	list<string> files;
	int ret = find_files("/Users/oxnz/Desktop/input", "2012", files);
	for (list<string>::iterator it = files.begin(); it != files.end(); ++it)
		cout << "file: " << *it << endl;
	cout << "found " << ret << " files" << endl;
	return 0;
}

int get_ts_index(const gps_time& time) {
    uint16_t h = static_cast<uint16_t>(time%1000000/10000);
    uint16_t m = time % 10000/100;
    uint16_t s = time % 100;
    return (h*60*60+m*60+s)/(3*60);
}

int test_get_ts_index(void) {
    uint64_t t = 20121101001122;
    for (int i = 0; i < 10; ++i) {
        t += 20400;
        cout << "ts index of " << t << " = " << get_ts_index(t) << endl;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    return test_logger();
    return test_output_html();
    return test_print_progress();
    return test_get_ts_index();
	// return test_find_files();
    printf("testing get_roadseg_id:\n");
    test_get_roadseg_id();
	return 0;
    if (test_read_preped_file("outfile") == -1) {
        cerr << "test read preprocessed file failed" << endl;
    }

    return -1;
}
