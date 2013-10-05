#include <stdio.h>

int main(void) {
	int n;
	int a[500] = {1, 2, 4, 7, 13};

	for (n=5; n < 500; ++n)
		a[n] = a[n-1] + a[n-2] + a[n-3];
	do {
		scanf("%d", &n);
		if (n == 0)
			break;
		else
			printf("%d\n", a[n-1]);
	} while (1);
	return 0;
}
