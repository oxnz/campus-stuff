#include "process.h"
#include "ffind.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>

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
    case SIGKILL:
        fprintf(stderr, "Can't handle kill\n");
        break;
    default:
        printf("uknonw signal: %d\n", signo);
        break;
    }
}

#define LOG_FILE "/var/log/rdpp.log"
int setup_log(const char *logfile) {
    setlogmask(LOG_UPTO (LOG_INFO));
    openlog(LOGFILE, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_INFO, "Hello from pid: %d\n", getpid());
    closelog();
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stdout, "Usage:\n\t%s [srcdir] [dstdir]\n", argv[0]);
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
    if (find_file_init(argv[1], "201211") != 0) {
        fprintf(stderr, "error init find file\n");
        return -1;
    }
    const char *fname;
    int ret = find_first_file(&fname);
    while (ret==1 && loop) {
        process_record_file(fname);
        ret = find_next_file(&fname);
    }
    if (ret == -1) {
        fprintf(stderr, "error while finding files\n");
        return -1;
    }
    if (ret == 0) {
        fprintf(stderr, "job finished\n");
    }
    if (ret == 1) { // loop == 0
        fprintf(stderr, "stopped while still jobs there\n");
    }

    return 0;
}
