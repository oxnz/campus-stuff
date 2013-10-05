#ifndef X_NEW_H
#define X_NEW_H

#include <stdio.h>

typedef const void* Type;
typedef void* Object;

Object new(const Type, ...);
void delete(Object);
size_t sizeOf(const Object);
int differ(const Object, const Object);
int puto(const Object, FILE *fp);

#endif