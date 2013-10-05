#include <stdio.h>
#include <string.h>

int main(void) {
	int a[12][31];
	int i, j, n;
	int x;
	char c;
	i = j = 0;
	memset(a, 0, 12*31*sizeof(int));

	while (scanf("%d:%d", &i, &j) != -1) {
		--i;
		--j;
		n = 0;
		while ((c = getchar()) != EOF) {
			if (isdigit(c)) {
				ungetc(c, stdin);
				break;
			}
			else if (isalpha(c) || c=='-')
			{
				if (x)
				{
					++n;
					x = 0;
				}
			}
			else
				x = 1;
		}
		a[i][j] = n;
	}
	for (i=0; i<12; ++i)
		for (j=0; j<31; ++j) {
			if (a[i][j] == 0)
				return 0;
			else
				printf("%d:%d %d\n", i+1, j+1, a[i][j]);
		}
	return 0;
}
