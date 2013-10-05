#include <stdio.h>
#include <math.h>

int main(void) {

	int l, w, h;
	int l1, w1, h1;
	int l2, w2, h2;
	int L1, L2;

	do {
		scanf("%d %d %d", &l, &w, &h);
		if (l == -1)
			break;
		else {
			scanf("%d %d %d", &l1, &w1, &h1);
			scanf("%d %d %d", &l2, &w2, &h2);
			if (l1 == 0 && l2 == l) {
				printf("front<->back\n");
				L1 = (h1+h2+l)*(h1+h2+l)+(w1-w2)*(w1-w2);
				L2 = (w1+w2+l)*(w1+w2+l)+(h1-h2)*(h1-h2);
				printf("%0.2f\n", sqrt(L1<l2?L1:L2));
			}
			else if (w1 == 0 && w2 == w) {
				printf("left<->right\n");
				L1 = (h1+h2+w)*(h1+h2+w)+(l1-l2)*(l1-l2);
				L2 = (l1+l2+w)*(l1+l2+w)+(h1-h2)*(h1-h2);
				printf("%0.2f\n", sqrt(L1<l2?L1:L2));
			}
			else if (h1 == 0 && h2 == h) {
				printf("bottom<->top\n");
				L1 = (l1+l2+h)*(l1+l2+h)+(w1-w2)*(w1-w2);
				L2 = (w1+w2+h)*(w1+w2+h)+(l1-l2)*(l1-l2);
				printf("%0.2f\n", sqrt(L1<l2?L1:L2));
			}
		}
	} while (1);

	return 0;
}
