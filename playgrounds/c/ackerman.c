#include <stdio.h>

/************* Stack overflow warning ****************/
/************* increase with $ ulimit -s 99999 *******/

void* malloc_and_set(size_t size) {
	void *ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

/************ ACKERMAN **************/
#define enable_global_cache
#ifdef enable_global_cache
int *global_ack_cache;
int line_length = 3000000;
#endif

int ack(int m, int n) {
#ifdef enable_global_cache
	int *addr = 0; addr = global_ack_cache+(m*line_length)+n;
	if (n > line_length) {
		printf("Alert: n is greater than %d (%d)\n", line_length, n);
		printf("address is %lu and value is %d for %d, %d\n", addr, *addr, m, n);
	} else if (*addr != 0) {
		//printf("picking up value of ack(%d, %d) == %d; from cache...\n", m, n, *addr);
		return *addr;
	}
#endif
	//printf("computing ack(%d, %d);\n", m, n);
	int ans;
	if (m <= 0) {
		ans = n+1;
	} else if (n <= 0) {
		ans = ack(m-1, 1);
	} else {
		ans = ack(m-1, ack(m, n-1));
	}
#ifdef enable_global_cache
	// prevent overflow, over line_length size, we need to compute as if there was no cache.
	if (n <= line_length) {
		*addr = ans;
	}
#endif
	return ans;
}

void main(void) {
	int m_max = 5, n_max = 20;
#ifdef enable_global_cache
	global_ack_cache = malloc_and_set(sizeof(int)*line_length*m_max);
#endif
	int i, j;
	for (i = 0; i <= m_max; i++) {
		for(j = 0; j <= n_max; j++) {
			printf("result of ackerman(%d, %d) => %d.", i, j, ack(i, j));
			printf("\n");
		}
	}
	free(global_ack_cache);
	exit(0);
}

