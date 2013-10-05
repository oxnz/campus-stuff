#ifndef X_DB_R_H
#define X_DB_R_H

struct Database
{
	const void* class;
	char *name;
	unsigned long count;
	void* root;
};

#endif
