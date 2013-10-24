#include "dummy.h"
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int test_get_road_id(void) {
    for (unsigned long i = 1160000000; i < 1170000000; ++i) {
        for (unsigned long j = 392600000; j < 410300000; j+=100) {
            gps_coord coord = {i, j};
            printf("x = %lu, y = %lu, road_id = %u\n", i, j, get_road_id(coord));
            getchar();
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    printf("testing get_road_id:\n");
    test_get_road_id();

    return -1;
}
