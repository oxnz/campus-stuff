#include "process.h"
#include "types.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void* read_file_into_mem(const char *fname, ssize_t *size) {
    unsigned int fd;
    char *buf;
    struct stat sb;

    fd = open(fname, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }
    if (fstat(fd, &sb) == -1) {
        perror("stat");
        return NULL;
    }
    buf = malloc(sb.st_size);
    if (buf == NULL) {
        perror("malloc");
        return NULL;
    }
    *size = read(fd, buf, sb.st_size);
    if (close(fd) == -1) {
        perror("close");
    }
    return buf;
}

int write_mem_to_file(const char *fname, ssize_t size) {
    return 0;
}

void* map_file(const char *fname) {
    int fd;
    char *buf;
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

int process_records(const void *buf, size_t len) {
    const char *p = buf;
    car_record *record = (car_record *)malloc(sizeof(car_record));
    while (
           scanf("%u,%u,%u,%lu,%lu,%lu,%u,%u,%u\n", (uint8_t*)&record->id,
                 &record->event, &record->status,
                 &record->gps.time, &record->gps.coord.x,
                 &record->gps.coord.y, &record->gps.speed,
                 &record->gps.direction, &record->gps.valid) == 9) {
        buf = strchr(++buf, '\n');
    }
    return 0;
}

int process_record_file(const char *fname) {
    return 0;
}

