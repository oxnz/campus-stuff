#include "process.h"

#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stdout, "Usage:\n\t%s [srcdir] [dstdir]\n", argv[0]);
        return -1;
    }
    const char *fname = get_first_file(argv[1]);
    sleep(4);
    while (fname) {
        printf("got file: %s\n", fname);
        fname = get_next_file();
    }
    /*
    fname = get_first_file(argv[2]);
    while (fname) {
        printf("got file: %s\n",fname);
        fname = get_next_file();
        }*/
    return 0;
}
