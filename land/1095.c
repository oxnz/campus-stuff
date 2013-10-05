#include <stdio.h>

int main(void) {

	char a[3][3];
	int i, j;
	int n;

	do
	{
		n = 0;
		for (i=0; i<3; ++i) {
			for (j=0; j<3; ++j)
				scanf("%c", &a[i][j]);
			getchar();
		}
	} while (getchar() != EOF);
	return 0;
}
