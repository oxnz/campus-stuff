#include <stdio.h>
#include <string.h>

int main(void) {

	char name[2][20];

	struct Node{
		char		name[20];
		unsigned int	age;
		int		generation;
	}Adam[14] = {
		{"God",		1000,	0},
		{"Adam",	930,	1},
		{"Seth",	912,	2},
		{"Enosh",	905,	3},
		{"Kenan",	910,	4},
		{"Mahalalel",	895,	5},
		{"Jared",	962,	6},
		{"Enoch",	365,	7},
		{"Methuselah",	969,	8},
		{"Lamech",	777,	9},
		{"Noah",	0,	10},
		{"Shem",	0,	11},
		{"Ham",		0,	11},
		{"Japheth",	0,	11},
	};

	int i;

	while (scanf("%s %s", name[0], name[1]) != -1) {
		struct Node *a = NULL;
		struct Node *b = NULL;
		struct Node *tmp = Adam;
		for (i=0; i<14; ++i) {
			if (!strcmp(name[0], tmp->name)) {
				a = tmp;
			}
			else if (!strcmp(name[1], tmp->name)) {
				b = tmp;
			}
			if (a != NULL && b != NULL)
				break;
			tmp ++;
		}
		if (a == NULL || b == NULL)
			printf("No enough information\n");
		else if (a->generation < b->generation)
			printf("Yes\n");
		else if (a->generation == b->generation || a->generation > b->generation)
			printf ("No\n");
		else
			printf("No enough information\n");
		if (a == NULL || b == NULL)
			printf("No enough information\n");
		else if (a->age == 0 || b->age == 0)
			printf("No enough information\n");
		else if (a->age > b->age)
			printf("Yes\n");
		else
			printf("No\n");
	}
	return 0;
}
