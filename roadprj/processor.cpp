#include "processor.h"
#include "types.h"
#include "dummy.h"
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

Processor::Processor(const string& listfname, size_t minPerTS)
    : m_nMinPerTS(minPerTS), 
      m_nCRCount(0), m_nCRRepeat(0), m_nCRInvalid(0), m_nCRC(0), m_nCRO(0),
      m_nNRCount(0), m_nNRRepeat(0), m_nNRInvalid(0), m_nNRC(0), m_nNRO(0),
      m_itsp(0), m_nTransCount(1000);
{
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
    m_pmCTSRecordPool = new std::map<orec_key, orec_value*>;
    m_pmNTSRecordPool = new std::map<orec_key, orec_value*>;
    m_pFileBuffer = new char[2*1024*1024];
}

size_t getTSIndex(uint64_t time) {
    uint16_t h = time % 1000000/10000;
    uint16_t m = time % 10000/100;
    uint16_t s = time % 100;
    return (h*60*60+m*60+s)/(m_nMinPerTS*60);
}

int Processor::process(const char *buf, size_t len) {
    cout << "reading ..." << endl;
    size_t tc = 0; // transition count
    for (struct {const char *p; in_rec irec; } tmp = 
        { (const char *)m_pFileBuffer, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
         tmp.p < m_pFileBuffer+len+1 &&
             sscanf(tmp.p, "%u,%hu,%hu,%llu,%lf,%lf,%hu,%hu,%hu\r\n",
                    &tmp.irec.car_id, &tmp.irec.event, &tmp.irec.status,
                    &tmp.irec.time, &tmp.irec.x, &tmp.irec.y,
                    &tmp.irec.speed, &tmp.irec.direct, &tmp.irec.valid)
             == 9;
         tmp.p = strchr(++tmp.p, '\n')) {
        printf("%u,%u,%u,%llu,%11.7lf,%10.7lf,%u,%u,%u\n",
               tmp.irec.car_id, tmp.irec.event, tmp.irec.status,
               tmp.irec.time,
               tmp.irec.x, tmp.irec.y, tmp.irec.speed, tmp.irec.direct,
               tmp.irec.valid);
        ++m_nRCount;
        if (!tmp.irec.valid) { ++m_nRInvalid; continue; }
        gps_coord coord = {tmp.irec.x * 10000000, tmp.irec.y * 10000000};
        orec_key key = {get_road_id(coord), tmp.irec.car_id};
        if (key.rid == -1) { cerr << "invalid road id" << endl; continue; }
        size_t ts = getTSIndex(tmp.irec.time);
        if (m_itsp == -1) m_itsp = ts;
        map<orec_key, orec_value*> *pcrp; // pointer -> current record pool
        if (ts == m_itsp) pcrp = m_pmCTSRecordPool;
        else if (ts == m_itsp+1) pcrp = m_pmNTSRecordPool;
        if (m_itsp == ts) {
        printf("its=%u:%u:%u, its=%u\n", h, m, s, its);
        getchar();
        orec_value *porecv = new orec_value;
        porecv->status = tmp.irec.status;
        porecv->time = tmp.irec.time;
        pair<map<orec_key, orec_value*>::iterator, bool> ret =
            m_pmCTSRecordPool->insert(make_pair(key, porecv));
        if (!ret.second) ++m_nRRepeat;
        printf("road_id: %u, car_id = %u\n", key.rid, key.cid);
    }
    return 0;
}


int Processor::processTS(void) {
    if (m_bEOF) {
        if (hasNextFile()) {
            cout << "processing " << m_plFileList->front();
            ifstream infile(m_plFileList->front().c_str());
            m_plFileList->pop_front();
            if (!infile.is_open())
                return -1;
            infile.seekg(0, ios::end);
            ssize_t size = infile.tellg();
            cout << " size: " << size;
            infile.seekg(0, ios::beg);
            infile.read(m_pFileBuffer, size);
            infile.close();
            processFileBuffer(m_pFileBuffer, size);
        }
    } else {
        processFileBuffer();
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
