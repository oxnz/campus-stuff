#include "RProcessor.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <signal.h>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
/*
 * @descritpion: control processing loop, 0 will cause return after
 * finish current file
 */
static int loop = 1;

#ifdef WIN32
BOOL CtrlHandler( DWORD fdwCtrlType ) {
    switch (fdwCtrlType) {
        /* Handle the CTRL-C signal. */
    case CTRL_C_EVENT:
        printf("CTRL_C_EVENT \n");
        break;
    case CTRL_CLOSE_EVENT:
        printf("CTRL_CLOSE_EVENT \n");
        break;
    case CTRL_BREAK_EVENT:
        printf("CTRL_BREAK_EVENT \n");
        break;
    case CTRL_LOGOFF_EVENT:
        printf("CTRL_LOGOFF_EVENT \n");
        break;
    case CTRL_SHUTDOWN_EVENT:
        printf("CTRL_SHUTDOWN_EVENT \n");
        break;
    default:
        return FALSE;
    }
    loop = 0;
    return (TRUE);
}
#else
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
#endif

#include "RSIDGen.h"
int test(void) {
	gps_coord c = {0, 1};
	roadseg_id* prsid = 0;
	ssize_t cnt = get_roadseg_id(c, &prsid);
	cout << "rsid cnt=" << cnt << endl;
	return 0;
}

int main(int argc, const char *argv[]) {
    test();
    if (argc != 3) {
        cerr <<  "Usage:" << endl << argv[0] << " -c <listfname>" << endl;
        return -1;
    }
#ifdef WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
#else
    struct sigaction action;
    action.sa_sigaction = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    // action.sa_flags = SA_SIGINFO;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        return -1;
    }
#endif
    Processor *p;
    try {
        p = new Processor(argv[2], 3); // 3 min
    } catch (std::bad_alloc& e) {
        cerr << "alloc error:" << e.what() << endl;
        return -1;
    } catch (std::runtime_error& e) {
        cerr << "runtime error:" << e.what() << endl;
        return -1;
    } catch (std::exception& e) {
        cerr << "unknown error:" << e.what() << endl;
        return -1;
    } catch (...) {
        cerr << "Unknown error happened, exit" << endl;
        return -1;
    }
    
    while (loop) {
        int ret = p->processTS();
        if (ret != 0) {
            cerr << "Error while processing, error code: " << ret << endl;
            return -1;
        }
        if (p->hasNextFile()) {
            //cerr << "Error while processing, unknonwn stopping" << endl;
            cout << "has next file" << endl;
            continue;
        } else {
            cout << "No more files, process ended" << endl;
            break;
        }
        getchar();
    }
    
    return 0;
}
