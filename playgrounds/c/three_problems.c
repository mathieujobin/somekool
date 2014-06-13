#include <stdio.h>


int problem1(int A[], int N) {
	int i = 0;

	struct entry {
		int val;
		int first_index;
		int count; // no need
	};
	struct entry *map;
	for(i = 0; i < N; i++) {
		
	}
}
	
int problem2(int A[], int N) {

}

int problem3(int A[], int N) {
	int i = 0, x = 0, y = 0, orientation = 0;
	int direction = 1;

	for (i = 0; i < N; i++) {
		//printf("DEBUG: (o,d) (%d, %d)\n", orientation, direction);
		printf("N %d, (x,y) => (%d, %d) -> ", i, x, y);
		if (orientation == 0) {
			y += A[i] * direction;
			orientation = 1;
		} else {
			x += A[i] * direction;
			orientation = 0;
			direction = -1 * direction;
		}
		printf("(%d, %d)\n", x, y);
	}
}

void main(void) {
	//int A[] = [ 1, 4, 3, 3, 1, 2]; // 4
	//int A[] = [6, 4, 4, 6]; // -1
	//problem1();

	int A[] = {1, 3, 2, 5, 4, 4, 6, 3, 2};
	problem3(A, 9);

}

