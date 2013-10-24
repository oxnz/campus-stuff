#include "processor.h"
#include "types.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include <set>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Processor::Processor(const string& listfname)
    : m_nRCount(0), m_nRRepeat(0), m_nRInvalid(0) {
    ifstream listf;
    listf.open(listfname);
    if (!listf) {
        cerr << "error: unable to open file: " << listfname << endl;
    }
    m_plFileList = new list<string>;
    string fpath;
    while (listf >> fpath) {
        cout << "file: " << fpath << endl;
        m_plFileList->push_back(fpath);
    }
    m_pmCTSRecordPool = new std::map<road_id, car_record*>;
    m_pmNTSRecordPool = new std::map<road_id, car_record*>;
    m_pFileBuffer = new char[2*1024*1024];
 }

int Processor::process(gps_time time) {
    if (m_plFileList->size()) {
        cout << "processing " << m_plFileList->front() << endl;
        ifstream infile(m_plFileList->front().c_str());
        m_plFileList->pop_front();
        if (!infile.is_open())
            return -1;
        infile.seekg(0, ios::end);
        ssize_t size = infile.tellg();
        cout << "File size: " << size << endl;
        infile.readsome(m_pFileBuffer, size);
        infile.close();
        car_record *precord = new car_record;
        cout << "reading ..." << endl;
        printf("x=%cx=%cx=%c\n", *m_pFileBuffer, *(m_pFileBuffer+1), *(m_pFileBuffer+2));
        /*
        for(const char *p = (const char *)m_pFileBuffer, int i = 0, 
                uint32_t id = 0, uint32_t event = 0, uint32_t direction = 0,
                uint32_t speed, uint32_t valid;
            p < m_pFileBuffer+size+1; p = strchr(++p, '\n')) {
            printf("%p, %p\n", m_pFileBuffer, p);
            sscanf(p, "%u,%u,%u,%llu,%lf,%lf,%u,%u,%u\r\n",
        while (sscanf(p, 
        read_file_into_mem(
        while (infile >> record.id >> record.event >> record.status >>
               record.gps.time >> record.gps.coord.x >> record.gps.coord.y >>
               record.gps.speed >> record.gps.direction >> record.gps.valid) {
            cout << "car_id:" << record.id << endl;
          
            }*/
    }
    return 0;
}

Processor::~Processor() {
    while (m_plFileList->size())
        m_plFileList->pop_front();
    delete m_plFileList;
    m_pmCTSRecordPool->clear();
    m_pmNTSRecordPool->clear();
    delete m_pmCTSRecordPool;
    delete m_pmNTSRecordPool;
    delete m_pFileBuffer;
}

/*
static char *pfbuf = 0;
static set<uint32_t, uint32_t> *pcset;
static const char *dstdir = 0;
static int logfd  = 0;

int process_file_init(const char *outdir,
                      uint32_t max_recf_size, uint32_t max_roadseg_cnt) {
    printf("pfbuf size=%u, roadseg cnt=%u, roadseg tbl size=%lu\n",
           max_recf_size, max_roadseg_cnt, max_roadseg_cnt*sizeof(car_id));
    dstdir = outdir;
    char logfname[256] = "";
    sprintf(logfname, "%s/rdpp.log", dstdir);
    printf("logfile=%s\n", logfname);
    if ((logfd = open(logfname, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR)) == -1) {
        perror("open logfile");
        return -1;
    }
    pfbuf = (char *)malloc(max_recf_size);
    try {
        pcset = new set<uint32_t, uint32_t>;
    } catch (bad_alloc& exception) {
        cerr << "new set error" << endl;
        return -1;
    }
    return 0;
}

int process_file_finish() {
    if (pfbuf)
        free(pfbuf);
    if (pcset)
        free(pcset);
    if (logfd)
        if (close(logfd) == -1) {
            perror("close logfile");
            return -1;
        }
    return 0;
}


int write_mem_to_file(const char *fname, ssize_t size) {
    return 0;
}


    struct stat sb;

    if ((fd = open(fname, O_RDONLY)) == -1) {
        perror("open");
        return NULL;
    }
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        return NULL;
    }
    buf = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    if (close(fd) == -1) {
        perror("close");
        return NULL;
    }
    return buf;
    
}


int process_records(const char *buf, size_t len) {
    car_record *record = (car_record *)malloc(sizeof(car_record));
    const char *p = buf;
    double gx,gy; // gps x and y
    uint32_t cnt_all = 0;
    uint32_t cnt_invalid = 0;
    uint32_t cnt_non_repeat = 0;
    while (sscanf(p, "%u,%u,%u,%llu,%lf,%lf,%u,%u,%u\r\n", &record->id,
                  &record->event, &record->status, &record->gps.time,
                  &gx, &gy, &record->gps.speed,
                  &record->gps.direction, &record->gps.valid) == 9
           && p < buf+len+1) {
        ++cnt_all;
        p = strchr(++p, '\n');
        record->gps.coord.x = gx * 10000000;
        record->gps.coord.y = gy * 10000000;
        printf("record:%06u,%u,%u,%llu,%llu,%llu,%u,%u,%u\n", record->id,
               record->event, record->status, record->gps.time,
               record->gps.coord.x, record->gps.coord.y, record->gps.speed,
               record->gps.direction, record->gps.valid);
        if (record->gps.valid == 0) { // invalid gps
            ++cnt_invalid;
            continue;
        }

    }
    if (p != buf+len-1) {
        fprintf(stderr, "error end: buf+len+1=%p, p=%p\n", buf+len+1, p);
        return -1;
    }
    printf("done: %u/%u/%u\n", cnt_non_repeat, cnt_invalid, cnt_all);
    return 0;
}

int process_record_file(const char *fpath) {
    unsigned int fd;
    size_t size;
    struct stat sb;

    printf("processing file: [%s]\n", fpath);
    if ((fd = open(fpath, O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }
    if (fstat(fd, &sb) == -1) {
        perror("stat");
        close(fd);
        return -1;
    }
    if ((size = read(fd, pfbuf, sb.st_size)) < sb.st_size) {
        fprintf(stderr, "read: read size < file size\n");
        close(fd);
        return -1;
    } else {
        printf("read into memory\n");
    }
    process_records(pfbuf, size);
    if (close(fd) == -1) {
        perror("close");
    }
    return 0;
}
*/
