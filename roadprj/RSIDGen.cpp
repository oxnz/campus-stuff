#include "RType.h"
#include "RsIDGen.h"
#include "constant.h"

#include <iostream>

namespace GPS {
const static gps_x x0 = 1150000000;
const static gps_x x1 = 1180000000;
const static gps_y y0 = 370000000;
const static gps_y y1 = 420000000;
}

/* gps_coord must be checked cause there are some error statistics
 */
roadseg_id get_roadseg_id(const gps_coord& coord) {
    if (coord.x < GPS::x0 || coord.x > GPS::x1) {
        //fprintf(stderr, "invalid gps x\n");
        //getchar();
    } else if (coord.y < GPS::y0 || coord.y > GPS::y1) {
        //fprintf(stderr, "invalid gps y\n");
        //getchar();
    }
    return static_cast<roadseg_id>(((coord.x >> 9) << 32) | (coord.y >> 9));
}

#using "C:\Users\xinyi\Documents\Visual Studio 2012\Projects\TestCall\TestCall\bin\Debug\TestCall.dll"
using namespace TestCall;
#using "D:\map_match_CL1\map_match_CL1\bin\x86\Debug\map_match_CL1.dll"
using namespace map_match_CL1;

#include<time.h>
static roadseg_id __rsid[RSID_MAXCNT];
ssize_t get_roadseg_id(const gps_coord& coord, roadseg_id** pprsid) {
	ssize_t cnt = -1;
	*pprsid = __rsid;
	
	static time_t t0 = time(NULL);

	Match^ map_match = gcnew Match();
		
	bool ret = map_match->OpenMapFile("./network63.mxd");
	if (ret == false) {
		std::cerr << "Open map file failed!" << std::endl;
		getchar();
	}
	__rsid[0] = map_match->getRoadID(116.2884521,39.8673973);
	static time_t t1 = time(NULL);
		std::cout << "Time spend to create match: " << t1 - t0 << std::endl;
		//coord.x / 10000000.00, coord.y / 10000000.00);
	cnt = 1;
	return cnt;
}
