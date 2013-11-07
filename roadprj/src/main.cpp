#include "RProcessor.h"

#include <unistd.h>
#include <signal.h>
#include <getopt.h>

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
static int loop = true;

void signal_handler(int signo, siginfo_t *info, void *ptr) {
    switch (signo) {
    case SIGINT:
        loop = false;
        cout << "WARNING: catch INT signal, please wait a moment" << endl;
        break;
    default:
        cerr << "ERROR: unkonwn signal(signo=" << signo
             << "), skipped" <<  endl;
        break;
    }
}

int help(int ecode = 0) {
    std::ostream* os = &cout;
    if (ecode)
        os = &cerr;
    *os << "Usage: " << "rdpp" << " [options]" << endl
        << "  Options:" << endl
        << "\t-h\tshow this help message and exit" << endl
        << "\t-b\tspecify buffer length in megabytes" << endl
        << "\t-d\tspecify the start date to process" << endl
        << "\t-i\tspecify input directory" << endl
        << "\t-l\tspecify how many days to process" << endl
        << "\t-o\tspecify output directory" << endl
        << "\t-t\tspecify time slot grannularity in minute" << endl
        << "  Unavailable Option:" << endl
        << "\t-p\tshow progress bar" << endl
        << "\t-j\toutput a js array file contains car count info" << endl
        << "  Default Value:" << endl
        << "\t-b: 2\tbuffer size: 2 Megabytes" << endl
        << "\t-l: 1\tprocess 1 single day" << endl
        << "\t-t: 3\ttime slot length: 3 minutes" << endl;

    return ecode;
}

int main(int argc, char *argv[]) {
    int ch;
    size_t bufsize(2*1024*1024), date(0), dcnt(1), mpts(3);
    const char* pIndir(0);
    const char* pOutdir(0);
    if (argc == 1)
        return help(0);
    while ((ch = getopt(argc, argv, "b:d:hi:l:o:t:")) != -1) {
        switch (ch) {
        case 'b':
            bufsize = strtoul(optarg, NULL, 10);
            if (bufsize >= 2 && bufsize < 20)
                bufsize *= 1024*1024;
            else {
                cerr << "ERROR: bad buffer size, must be in 2 ~ 20" << endl;
                return -1;
            }
            break;
        case 'd':
            date = strtoul(optarg, NULL, 10);
            if (date > 18000000 && date < 99999999)
                break;
            else {
                cerr << "ERROR: bad date, must be in 18000000~99999999" << endl;
                return -1;
            }
        case 'i':
            pIndir = optarg;
            break;
        case 'l':
            dcnt = strtoul(optarg, NULL, 10);
            if (dcnt >= 1 && dcnt <= 30)
                break;
            else {
                cerr << "ERROR: bad day count, must be in 1~30" << endl;
                return -1;
            }
        case 'o':
            pOutdir = optarg;
            break;
        case 't':
            mpts = strtoul(optarg, NULL, 10);
            if (mpts >= 2 && mpts <= 59 && 24*60 % mpts == 0)
                break;
            else {
                cerr << "ERROR: bad minute per time slot, must be in 2~59"
                     << " && 24*60 mod minPerTS == 0" << endl;
                return -1;
            }
        case 'h':
            ch = 0;
        case '?':
        default:
            ch = -1;
            return help(ch);
        }
    }
    argc -= optind;
    argv += optind;
    if (!pIndir || !pOutdir) {
        cerr << "ERROR: indir or outdir is null" << endl;
        return -1;
    }
    if (!date) {
        cerr << "ERROR: start date not specified" << endl;
        return -1;
    }
#ifdef DEBUG
    cout << "DEBUG: IN: " << pIndir << " Out: " << pOutdir << endl;
#endif
    struct sigaction action;
    action.sa_sigaction = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    Processor *rdpp;
    try {
        rdpp = new Processor(pIndir, pOutdir, mpts, bufsize);
    } catch (std::logic_error& e) {
        cerr << "logic error:" << e.what() << endl;
        return -1;
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

#ifdef SINGLE_DAY_MODE
    int ret;
    ++dcnt;
    --date;
    while (loop && --dcnt) {
        ret = rdpp->process(++date);
#else
        int ret = rdpp->process(date, dcnt);
#endif
        if (ret == -1) {
            loop = false;
            cerr << "ERROR: RDPP process failed, error code: " << ret << endl;
        } else if (ret == 1) {
            cout << "INFO: no more files to be processed" << endl;
            loop = false;
            ret = 0;
        }
#ifdef SINGLE_DAY_MODE
    }
#endif
    delete rdpp;
    
    return ret;
}
