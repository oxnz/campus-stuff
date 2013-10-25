#include "types.h"
#include "dummy.h"
#include "constant.h"
#include <stdio.h>

const static gps_x x0 = 1150000000;
const static gps_x x1 = 1180000000;
const static gps_y y0 = 370000000;
const static gps_y y1 = 420000000;

roadseg_id get_roadseg_id(gps_coord coord) {
    if (coord.x < x0 || coord.x > x1) {
        fprintf(stderr, "invalid gps x\n");
        //getchar();
    } else if (coord.y < y0 || coord.y > y1) {
        //fprintf(stderr, "invalid gps y\n");
        getchar();
    }
    return static_cast<roadseg_id>(((coord.x >> 9) << 32) | (coord.y >> 9));
}

#using "C:\Users\xinyi\Documents\Visual Studio 2012\Projects\TestCall\TestCall\bin\Debug\TestCall.dll"
using namespace TestCall;
static roadseg_id __rsid[RSID_MAXCNT];
ssize_t get_roadseg_id(gps_coord coord, roadseg_id* rsid) {
	ssize_t cnt = -1;
	try {
        CR ^r = gcnew CR();
        r->Name = "zzj"; 
        printf("%s\n", r->Name); 
        r->showDialog();
        cli::array<System::UInt32, 1>^ %b = r->getRsID(123, 123);
	
        if (b->Length <= 0)
            return -1;
        cnt = b->Length > RSID_MAXCNT ? RSID_MAXCNT : b->Length;
        for (ssize_t i = 0; i < cnt; ++i) {
            __rsid[i] = b[i];
        }
	}  catch(...) {
		return -1;
	}
	return cnt;
}
