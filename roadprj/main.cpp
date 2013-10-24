#include "processor.h"
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <iostream>

using namespace std;
/*
 * @descritpion: control processing loop, 0 will cause return after
 * finish current file
 */
static int loop = 1;

void signal_handler(int signo, siginfo_t *info, void *ptr) {
    printf("signal handling\n");
    switch (signo) {
    case SIGINT:
        loop = 0;
        printf("INT\n");
        break;
    default:
        printf("uknonw signal: %d\n", signo);
        break;
    }
}

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        cerr <<  "Usage:" << endl << argv[0] << " -c <listfname>" << endl;
        return -1;
    }
    struct sigaction action;
    action.sa_sigaction = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    // action.sa_flags = SA_SIGINFO;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    Processor *p;
    try {
        p = new Processor(argv[2], 3); // 3 min
    } catch (bad_alloc& exc) {
        cerr << "alloc error" << endl;
        return -1;
    }
    while (loop) {
        int ret = p->processTS();
        if (ret != 0) {
            cerr << "Error while processing, error code: " << ret << endl;
            return -1;
        }
        if (p->hasNextFile()) {
            cerr << "Error while processing, unknonwn stopping" << endl;
        } else
            break;
    }

    return 0;
}
