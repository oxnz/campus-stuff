#ifndef X_STU_R_H
#define X_STU_R_H

#include "person.h"
#include "person.r.h"

struct Student
{
	const struct Person _;
	int ID;
};

#endif