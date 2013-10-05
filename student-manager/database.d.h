#ifndef X_DB_D_H
#define X_DB_D_H

struct Node
{
	void* data;
	struct Node* prev;
	struct Node* next;
};

//#define Node(a, b, c, d, e) struct Node = {a, b, c, d}

#endif