#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int n;
	int *a;
	int i;
	int j;
	int tmp;

	while (scanf("%d", &n) != -1) {
		a = malloc(sizeof(int)*n);
		for (i=0; i<n; ++i) {
			scanf("%d", &a[i]);
		}
		for (j=0; j<7; ++j) {
			for (i=0; i<n; ++i) {
				scanf("%d", &tmp);
				if (a[i] > tmp)
					a[i] = tmp;
			}
		}
		tmp = 0;
		for (i=0; i<n; ++i)
			tmp += a[i];
		printf("%d\n", tmp);
	}
	return 0;
}
