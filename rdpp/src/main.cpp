/*
 * File: main.cpp
 * Description: this file is part of the RDPP Project, main func entry point
 * Author: Oxnz
 * Mail: yunxinyi@gmail.com
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12
 */
#include "RProcessor.h"
#include "NZLogger.h"

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
using NZ::NZLogger;
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
    NZLogger::setLogLevel(NZ::ERROR);
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
                NZLogger::log(NZ::ERROR, "bad buffer size must be in 2~20");
                return -1;
            }
            break;
        case 'd':
            date = strtoul(optarg, NULL, 10);
            if (date > 18000000 && date < 99999999)
                break;
            else {
                NZLogger::log(NZ::ERROR,
                              "bad date, should be in 18000000~99999999");
                return -1;
            }
        case 'i':
            pIndir = optarg;
            break;
        case 'l':
            dcnt = strtoul(optarg, NULL, 10);
            if (dcnt >= 1 && dcnt <= 31)
                break;
            else {
                NZLogger::log(NZ::ERROR, "bad day count, must be in 1~31");
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
                NZLogger::log(NZ::ERROR,
                              "bad minute per time slot, must be 2~59"
                              " && 24*60 mod minPerTS == 0");
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
        NZLogger::log(NZ::ERROR, "indir or outdir is null");
        return -1;
    }
    if (!date) {
        NZLogger::log(NZ::ERROR, "start date not specified");
        return -1;
    }
    NZLogger::log(NZ::DEBUG, std::string("input dir: ") + pIndir
                  + "output dir: " + pOutdir);
    struct sigaction action;
    action.sa_sigaction = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    R::Processor *rdpp;
    try {
        rdpp = new R::Processor(pIndir, pOutdir, mpts, bufsize);
    } catch (std::logic_error& e) {
        NZLogger::log(NZ::ERROR, std::string("logic error -> ") + e.what());
        return -1;
    } catch (std::bad_alloc& e) {
        NZLogger::log(NZ::ERROR, std::string("alloc error -> ") + e.what());
        return -1;
    } catch (std::runtime_error& e) {
        NZLogger::log(NZ::ERROR, std::string("runtime error -> ") + e.what());
        return -1;
    } catch (std::exception& e) {
        NZLogger::log(NZ::ERROR, std::string("unknown error -> ") + e.what());
        return -1;
    } catch (...) {
        NZLogger::log(NZ::ERROR, "Unknown error happend, construct failed");
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
            NZLogger::log(NZ::ERROR, "RDPP process failed, error code: "
                          + std::to_string(ret));
        } else if (ret == 1) {
            NZLogger::log(NZ::INFO, "no more files to be processed");
            loop = false;
            ret = 0;
        }
#ifdef SINGLE_DAY_MODE
    }
#endif
    delete rdpp;
    
    return ret;
}
