#ifndef X_DB_H
#define X_DB_H

extern const void* Database;

void* add(void*, const void*);
void* find(const void*, const void*);
void* drop(void*, const void*);

void save(void*);
void load(void*);

#define X_BUF_SIZE 1024

#endif
