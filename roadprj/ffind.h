#ifndef FFIND_H
#define FFIND_H

int find_file_init(const char *path, const char *regexp);
const char *find_first_file(void);
const char *find_next_file(void);
const char *find_file(const char *path, const char *regexp);

#endif
