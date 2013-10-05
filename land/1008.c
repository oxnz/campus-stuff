#include <stdio.h>

int main(void) {
	int i, j;
	int m, n;
	int k;
	int a[101][101];

	while (scanf("%d %d", &m, &n) != -1) {
		for (i=0; i<m; ++i) {
			a[i][100] = 0;
			for (j=0; j<n; ++j) {
				scanf("%d", &a[i][j]);
				a[i][100] += a[i][j];
			}
			a[100][j] += a[i][j];
		}
		scanf("%d", &k);
		if ((n+1) / m > k) {
			printf("No\n");
			continue;
		}
		for (i=0; i<m; ++i) {
			if (a[i][100] > k) {
				printf("No\n");
				break;
			}
		}
	}
	return 0;
}
