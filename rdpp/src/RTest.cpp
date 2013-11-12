/*
 * File: RTest.h
 * This file is part of the @PACKAGE@ project
 * Description: all test cases are here
 * Author: Oxnz
 * Version: @VERSION@
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

class T {
	friend ostream& operator<<(ostream& os, const T& t) {
		os << "HELLO" << 123 << 2.3 << 1.2121212121 << endl;
		return os;
	}
};

int test_logger() {
	T t;
	NZLogger::log("T: %z", t);
	NZ::NZLogger::setLogLevel(NZ::INFO);
	for (int i = 0; i < 1000000; ++i) {
		NZLogger::log(NZ::DEBUG, "This is a %s log%d%f", "debug", 123, 12.12);
	}
	NZLogger::log("IN LOGGER");
	cout << "log end" << endl;
	int i = 0;
	NZLogger::log("abc %s %d %p end", "hello", 123, &i);
	cout << "log2 end" << endl;
    return 0;
}

#include "RDPool.h"
using namespace RDP;
int test_pp() {
    RDPool *p = new RDPool(10, 20);
    for (roadseg_id i = 1; i <= 10; ++i) {
        for (ts_index j = 0; j < 20; ++j)
            cout << (*p)(i, j) << " ";
        cout << endl;
    }
    for (roadseg_id i = 1; i <= 10; ++i) {
        for (ts_index j = 0; j < 20; ++j) {
            cout << (*p)[i][j];
        }
        cout << endl;
    }
    delete p;
    
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
    for (int i = 0; i < 1000; ++i) {
        print_progress(i*100/1000, "Testxx: ");
		usleep(1000);
	}
	print_progress(100, "Test :XX");
    return 0;
}

using namespace RsidGen;
int test_get_roadseg_id(void) {
	cout << "X_STEP: " << GPS_X_STEP << " Y_STEP: " << GPS_Y_STEP
		<< endl << "X_INNER_STEP: " << GPS_X_INNER_STEP
		<< " Y_INNER_STEP: " << GPS_Y_INNER_STEP
		<< endl << "GPS_L_CNT: " << GPS_L_CNT
		<< " GPS_ML_CNT: " << GPS_ML_CNT << endl
		<< "GPS_MM_CNT: " << GPS_MM_CNT << endl
		<< "GPS_MR_CNT: " << GPS_MR_CNT << endl
		<< "GPS_H_CNT: " << GPS_H_CNT << endl
		<< "MAX_RSID: " << MAX_RSID << endl;
	std::set<roadseg_id> s;
	roadseg_id rsid = RsidGen::get_rsid2(1163748016, 399757500);
	cout << "rsid of 1163748016, 399757500 = " << rsid << endl;
	/*
	for (gps_x x = 1153000000; x < 1176000000; x += 10)
		for (gps_y y = 394000000; y < 411000000; y += 10) {
			rsid = RsidGen::get_rsid2(x, y);
			if (rsid) {
		//	cout << "rsid(" << x << "," << y << ") = " << rsid << endl;
			s.insert(rsid);
			}
		}
	cout << "set size = " << s.size() << endl;
	getchar();
	for (std::set<roadseg_id>::iterator it = s.begin();
			it != s.end(); ++it)
		cout << *it << endl;
		*/

    return 0;
}

int test_read_preped_file(const char *fpath) {
    ifstream infile(fpath, ios::in|ios::binary);
    if (!infile.is_open()) {
        cerr << "can't open infile" << endl;
        return -1;
    }
	roadseg_id rsid;
	roadseg_id tsi;
	roadseg_id origcnt;
	roadseg_id cnt;
	for (ts_index i = 0; i < 480; ++i) {
		cnt = 0;
		infile.read(reinterpret_cast<char*>(&tsi), sizeof(roadseg_id));
		cout << "reading TS Index: " << i << " orig index: " << tsi << endl;
		do {
			infile.read(reinterpret_cast<char*>(&rsid), sizeof(roadseg_id));
			//cout << "rsid: " << rsid << endl;
			++cnt;
		} while (rsid != RsidGen::INVALID_RSID);
		infile.read(reinterpret_cast<char*>(&origcnt), sizeof(roadseg_id));
		if (--cnt != origcnt) {
			cerr << "*** error: count didn't match" << endl;
			getchar();
		}
		cout << "read cnt: " << cnt << " orig cnt = " << origcnt << endl;
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

int test_set(void) {
	std::set<roadseg_id> s;
	gps_x x0 = 1154000000;
	gps_x x1 = 1176000000;
	gps_y y0 = 394100000;
	gps_y y1 = 410900000;
	gps_x dx = (x1-x0) >> 9;
	gps_y dy = (y1-y0) >> 9;
	dx = 901;
	dy = 904;
	roadseg_id rsid(0);
	cout << "insert start, count: " << s.size() << endl;
	cout << "dx: " << dx << ", dy: " << dy << endl;
	for (gps_x x = x0; x < x1; x += dx)
		for (gps_y y = y0; y < y1; y += dy) {
			rsid = RsidGen::get_rsid(x, y);
			if (rsid == RsidGen::INVALID_RSID) continue;
			s.insert(rsid);
		//	cout << "(" << x << "," << y << "), rsid: " << rsid << endl;
		}
	cout << "insert finished, count: " << s.size() << endl;
	getchar();
	return 0;
}

#include "RDPool.h"
using namespace RDP;
int test_query(void) {
	/*
	cout << rdp->query(110488, 5) << endl;
	cout << rdp->query(110488, 6) << endl;
	cout << rdp->query(110488, 7) << endl;
	cout << rdp->query(110488, 8) << endl;
	delete rdp;
	*/
	RDPool::query(480);
	return 0;
}

int main(int argc, char *argv[]) {
	return test_query();
    return test_get_roadseg_id();
    return test_read_preped_file("out/20121101.dat");
    return test_print_progress();
    return test_logger();
	return test_set();
//    return test_pp();
    return test_output_html();
    return test_get_ts_index();
	// return test_find_files();
    printf("testing get_roadseg_id:\n");
    test_get_roadseg_id();
	return 0;

    return -1;
}
