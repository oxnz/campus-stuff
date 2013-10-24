#include "types.h"
#include "dummy.h"
#include <stdio.h>

const static gps_x x0 = 1150000000;
const static gps_x x1 = 1180000000;
const static gps_y y0 = 370000000;
const static gps_y y1 = 420000000;
const static gps_x coord_wx = x1-x0;
const static gps_y coord_wy = y1-y0;
const static unsigned long count = 500;
// 500 -> 250000 25w

road_id get_road_id(gps_coord coord) {
    if (coord.x < x0 || coord.x > x1) {
        fprintf(stderr, "invalid gps x\n");
        getchar();
    } else if (coord.y < y0 || coord.y > y1) {
        fprintf(stderr, "invalid gps y\n");
        getchar();
    }
    return ((coord.x >> 9) << 32) | (coord.y >> 9);
}
