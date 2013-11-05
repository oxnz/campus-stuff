#include "RProcessor.h"
#include <unistd.h>
#include <signal.h>

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <exception>

using std::cout;
using std::cerr;
using std::endl;
/*
 * @descritpion: control processing loop, 0 will cause return after
 * finish current time slot
 */
static int loop = 1;

void signal_handler(int signo, siginfo_t *info, void *ptr) {
    switch (signo) {
    case SIGINT:
        loop = 0;
        cout << "catch INT signal, please wait a moment" << endl;
        break;
    default:
	cout << "unkonwn signal: " << signo << endl;
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
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    Processor *p;
    try {
        p = new Processor(argv[2], 3); // 3 min
/*
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
*/
    } catch (std::exception& e) {
	cout << e.what() << endl;
	return -1;
    }
    while (loop) {
        int ret = p->processTS();
        if (ret != 0) {
            if (p->hasNextFile()) {
            cerr << "Error occured while processing, aborted with error code: "
                 << ret << endl;
            return -1;
            } else {
                cout << "All files are processed successfully" << endl;
                loop = 0;
            }
        }
    }
    delete p;
    
    return 0;
}
