#include "processor.h"
#include "types.h"
#include "dummy.h"

 #include <set>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Processor::Processor(const string& listfname, size_t minPerTS)
    : m_nMinPerTS(minPerTS), 
      m_nCRCount(0), m_nCRRepeat(0), m_nCRInvalid(0), m_nCRC(0), m_nCRO(0),
      m_nNRCount(0), m_nNRRepeat(0), m_nNRInvalid(0), m_nNRC(0), m_nNRO(0),
      m_itsp(-1), m_nTransCount(1000), m_nCurTransCnt(0), m_bEOF(true)
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
    if (!m_plFileList->size()) {
        cerr << "File list contains no file" << endl;
        return; // FIX: change to throw
    }
    m_pmCTSRecordPool = new std::map<orec_key, orec_value*>;
    m_pmNTSRecordPool = new std::map<orec_key, orec_value*>;
    m_pFileBuffer = new char[2*1024*1024];
}

size_t Processor::getTSIndex(uint64_t time) {
    uint16_t h = time % 1000000/10000;
    uint16_t m = time % 10000/100;
    uint16_t s = time % 100;
    return (h*60*60+m*60+s)/(m_nMinPerTS*60);
}

int Processor::processOrigRecord(in_rec& rec) {
    printf("%u,%u,%u,%llu,%11.7lf,%10.7lf,%u,%u,%u\n",
           rec.car_id, rec.event, rec.status, rec.time,
           rec.x, rec.y, rec.speed, rec.direct, rec.valid);
        gps_coord coord = {rec.x * 10000000, rec.y * 10000000};
        orec_key key = {get_road_id(coord), rec.car_id};
        /*
         * @advice: skip the wrong road id
         */
        if (key.rid == -1) { cerr << "invalid road id" << endl; return -1; }
        size_t ts = getTSIndex(rec.time);
        if (m_itsp == -1) m_itsp = ts;
        map<orec_key, orec_value*> *pcrp = 0; // pointer -> current record pool
        if (ts == m_itsp) pcrp = m_pmCTSRecordPool;
        else if (ts == m_itsp+1) {
            pcrp = m_pmNTSRecordPool;
            ++m_nCurTransCnt;
        }
        else if (ts == m_itsp-1){
            cerr << "Error: come up with an previous ts: " << ts << ", skipped"
                    << endl;
            //return -1;
            return 0;
        } else {
            cerr << "Error: come up with an unexpected ts: " << ts << endl;
            return 0;
        }
        orec_value *porecv = new orec_value;
        porecv->status = rec.status;
        porecv->time = rec.time;
        pair<map<orec_key, orec_value*>::iterator, bool> ret =
            pcrp->insert(make_pair(key, porecv));
        if (!ret.second) ++m_nCRRepeat;
        printf("road_id: %u, car_id = %u\n", key.rid, key.cid);
    
    return 0;
}

int Processor::processFileBuffer() {
    for (in_rec irec; m_pCurFBufPos < m_pFileBufEnd &&
             sscanf(m_pCurFBufPos, "%u,%hu,%hu,%llu,%lf,%lf,%hu,%hu,%hu\r\n",
                    &irec.car_id, &irec.event, &irec.status,
                    &irec.time, &irec.x, &irec.y,
                    &irec.speed, &irec.direct, &irec.valid) == 9;
         m_pCurFBufPos = strchr(++m_pCurFBufPos, '\n')) {
        ++m_nCRCount;
        if (!irec.valid) { ++m_nCRInvalid; continue; }
        if (processOrigRecord(irec) != 0) {
            cerr << "process record failed" << endl;
            return -1;
        }
    }
    m_bEOF = true;
    getchar();
    return 0;
}

ssize_t Processor::readFileIntoMem(const char* fpath) {
    cout << "reading " << fpath;
    ifstream infile(fpath);
    if (!infile.is_open())
        return -1;
    infile.seekg(0, ios::end);
    ssize_t fsize = infile.tellg();
    m_pFileBufEnd = m_pFileBuffer + fsize;
    m_pCurFBufPos = m_pFileBuffer;
    cout << " size: " << fsize;
    infile.seekg(0, ios::beg);
    infile.read(m_pFileBuffer, fsize);
    infile.close();
    cout << endl;
    return fsize;
}

int Processor::processTS(void) {
    m_nCurTransCnt = 0;
    while (m_pmNTSRecordPool->size() < m_nTransCount) {
        cout << "cur trans cnt= " << m_nCurTransCnt << endl;
        if (m_bEOF) {
            if (hasNextFile()) {
                cout << "processing " << m_plFileList->front();
                if(readFileIntoMem(m_plFileList->front().c_str()) <= 0)
                    return -1;
            } else {
                cout << "all files are processed" << endl;
                getchar();
                return 0;
            }
        }
        if (processFileBuffer() != 0)
            return -1;
    }
    if (m_pmNTSRecordPool->size() == m_nTransCount) {
        cout << "need transfer" << endl;
        getchar();
    } else {
        cout << "is this reasonable?" << endl;
        getchar();
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
