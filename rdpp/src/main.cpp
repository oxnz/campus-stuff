/*
 * File: main.cpp
 * Description: this file is part of the RDPP Project, main func entry point
 * Author: Oxnz
 * Mail: yunxinyi@gmail.com
 * This file is part of the @PACKAGE@ project
 * Copying: Copyright (C) 2013, All rights reserved.
 *
 * Version: @VERSION@
 * Revision: -*-
 * Last-update: 2013-11-07 22:10:12
 */

#include "RProcessor.h"
#include "RDPool.h"
#include "NZLogger.h"

#include <unistd.h>
#include <signal.h>
#include <getopt.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <list>

using std::cout;
using std::cerr;
using std::endl;
using NZ::NZLogger;
/*
 * @descritpion: control processing loop, true will cause return after
 * finish current day
 */
static int loop = true;

void signal_handler(int signo, siginfo_t *info, void *ptr) {
    switch (signo) {
    case SIGINT:
        loop = false;
        NZLogger::log(NZ::WARNING, "catch sigint, please wait a moment");
        break;
    default:
        NZLogger::log(NZ::ERROR, "unknown signal, signo = %d, skipped", signo);
        break;
    }
}

int reg_signal_handler() {
    struct sigaction action;
    action.sa_sigaction = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) == -1) {
		NZLogger::log(NZ::ERROR, "sigaction error: %s", strerror(errno));
        return -1;
    }
	return 0;
}

int help(int ecode = 0) {
    std::ostream* os = &cout;
    if (ecode)
        os = &cerr;
    *os << "Usage: " << "rdpp" << " [options] indir [outdir]" << endl
		<< "  Process original data and output report data & final data" << endl
		<< "  The options are: " << endl
        << "\t-h\tshow this help message and exit" << endl
        << "\t-b\tspecify buffer length in megabytes" << endl
        << "\t-d\tspecify the start date to process" << endl
        << "\t-i\tspecify input directory" << endl
        << "\t-l\tspecify how many days to process" << endl
        << "\t-o\tspecify output directory" << endl
        << "\t-p\tprocess date while preprocessing" << endl
		<< "\t-q\tquery" << endl
        << "\t-t\tspecify time slot grannularity in minute" << endl
		<< "\t-v[+]\tshow verbose message" << endl
        << "\t-P\tdiable progress bar" << endl
		<< "\t-V\tshow version info" << endl
		/*
        << "  Unavailable Option:" << endl
        << "\t-j\toutput a js array file contains car count info" << endl
		*/
        << "  Default Value:" << endl
        << "\t-b:\t2" << endl // \tbuffer size: 2 Megabytes" << endl
        << "\t-l:\t1" << endl // \tprocess 1 single day" << endl
		<< "\t-p:\tfalse" << endl
        << "\t-t:\t3" << endl //\ttime slot length: 3 minutes" << endl;
		<< "\t-P:\tfalse" << endl;
    return ecode;
}

int main(int argc, char *argv[]) {
	NZLogger::setLogLevel(NZ::WARNING);
    int ch;
    size_t bufsize(2*1024*1024), date(0), dcnt(1), mpts(3);
    const char* pIndir(0);
    const char* pOutdir(0);
	bool process(false);
	bool query(false);
	bool progbar(true);
    if (argc == 1)
        return help(0);
    while ((ch = getopt(argc, argv, "b:d:hi:l:o:qt:vV")) != -1) {
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
		case 'p':
			process = true;
			break;
		case 'q':
			query = true;
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
		case 'v':
			NZLogger::setLogLevel(NZ::WARNING);
			break;
		case 'P':
			progbar = false;
			break;
		case 'V':
			std::cout << "Road Data (Pre) Processor 1.6" << std::endl
				<< "Copyright (C) 2013 Oxnz,"
				<< " All rights reserved" << std::endl
			   	<< "Bug-report: <yunxinyi@gmail.com>" << std::endl;
			return 0;
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
	if (query) {
		if (!pIndir) {
			NZLogger::log(NZ::ERROR, "data directory not specified");
			return -1;
		}
		return RDP::RDPool::query(mpts, pIndir);
	}
    if (!pIndir || !pOutdir) {
        NZLogger::log(NZ::ERROR, "indir or outdir is null");
        return -1;
    }
    if (!date) {
        NZLogger::log(NZ::ERROR, "start date not specified");
        return -1;
    }
    NZLogger::log(NZ::DEBUG, "input dir: %s, output dir: %s\n",
			pIndir, pOutdir);
	if (reg_signal_handler()) {
		NZLogger::log(NZ::FATAL, "failed to register signal handler");
		return -1;
	}

    R::Processor *rdpp;
    try {
        rdpp = new R::Processor(pIndir, pOutdir, mpts, bufsize, process);
    } catch (std::logic_error& e) {
        NZLogger::log(NZ::ERROR, "logic error -> %s", e.what());
        return -1;
    } catch (std::bad_alloc& e) {
        NZLogger::log(NZ::ERROR, "alloc error -> %s", e.what());
        return -1;
    } catch (std::runtime_error& e) {
        NZLogger::log(NZ::ERROR, "runtime error -> %s", e.what());
        return -1;
    } catch (std::exception& e) {
        NZLogger::log(NZ::ERROR, "unknown error -> ", e.what());
        return -1;
    } catch (...) {
        NZLogger::log(NZ::ERROR, "Unknown error happend, construct failed");
        return -1;
    }

    int ret(-1);
#ifdef SINGLE_DAY_MODE
    ++dcnt;
    --date;
    while (loop && --dcnt) {
#endif
        try {
#ifdef SINGLE_DAY_MODE
            ret = rdpp->process(++date, 1);
#else
			/* uncomment following 3 lines to train specified days
			 * in day list
			 */
			std::list<uint32_t> bad_sunday = {20121104};
			std::list<uint32_t> good_sunday = {20121111, 20121118, 20121125};
			std::list<uint32_t> good_saturday = {20121103, 20121110, 20121117,
				20121124};
			std::list<uint32_t> good_weekday = {20121101, 20121102, 20121105,
			20121106, 20121107, 20121108,
			20121109, 20121112, 20121113, 20121114, 20121115, 20121116,
			20121119, 20121120, 20121121, 20121122, 20121123,
			20121126, 20121127, 20121128, 20121129, 20121130,};
			std::list<uint32_t> tlist[4] = {
				bad_sunday,
				good_sunday,
				good_saturday,
				good_weekday,
			};
			for (size_t i = 0; i < 4; ++i) {
				NZLogger::log(NZ::NOTICE, "process index: %d", i);
				ret = rdpp->process(tlist[i], true);
				if (ret) {
					NZLogger::log(NZ::ERROR, "process failed, start by %d",
							tlist[i].front());
					return ret;
				}
			}
			return 0;
            ret = rdpp->process(date, dcnt, true);
#endif
        } catch (std::exception& e) {
            NZLogger::log(NZ::FATAL, "RDPP process failed");
            ret = -1;
        } catch (...) {
            NZLogger::log(NZ::FATAL, "unknown exception occured");
            ret = -1;
        }
        if (ret == -1) {
            loop = false;
            NZLogger::log(NZ::ERROR, "RDPP process failed, error code: %d",
					ret);
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
