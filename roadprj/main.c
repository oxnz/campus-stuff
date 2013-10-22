#include "process.h"
#include "ffind.h"
#include <stdio.h>
#include <signal.h>

/*
 * @descritpion: control processing loop, 0 will cause return after
 * finish current file
 */
static int loop = 1;

void signal_handler(int signo) {
    switch (signo) {
    case SIGINT:
        loop = 0;
        signal(signo, signal_handler);
        break;
    case SIGKILL:
        fprintf(stderr, "Can't handle kill\n");
        break;
    default:
        printf("uknonw signal: %d\n", signo);
        break;
    }
}
    
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stdout, "Usage:\n\t%s [srcdir] [dstdir]\n", argv[0]);
        return -1;
    }
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "errror catch signint\n");
        return -1;
    }
    if (find_file_init(argv[1], NULL) != 0) {
        fprintf(stderr, "error init find file\n");
    }
    const char *fname = find_first_file();
    while (fname) {
        process_record_file(fname);
        if (loop) {
            fname = find_next_file();
        }
    }
    return 0;
}
