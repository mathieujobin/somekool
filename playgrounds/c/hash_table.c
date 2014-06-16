#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

void* malloc_and_set(size_t size) {
	void *ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

/*************** Simple Print functions *************************/
void print_string_array(char **list, int size) {
	printf("[ ");
	int i; for (i = 0; i < size; i++) {
		printf("%s, ", *(list+i));
	}
	printf("\x8\x8"); // erasing last delimitor
	printf(" ]\n");
}

void print_num_array(int *list, int size) {
	printf("[ ");
	int i; for (i = 0; i < size; i++) {
		printf("%d, ", *(list+i));
	}
	printf("\x8\x8"); // erasing last delimitor
	printf(" ]\n");
}

/*************** File manipulation functions *********************/
int file_length(FILE *fp) {
	if (fseek(fp, 0L, SEEK_END)) { printf("error seeking to end...\n"); }
	int sz = ftell(fp);
	printf("DEBUG: file size is %d\n", (int)sz);
	fseek(fp, 0L, SEEK_SET);
	return sz;
}

void content_from_file(char **content, char *filename) {
	FILE* text_file = fopen(filename, "r");
	if (text_file == 0) {
		printf("could not open file at %s\n", filename);
	}
	// fseek segfaults with file could not be open, text_file == 0
	int file_len = file_length(text_file);
	*content = (char*)malloc_and_set(file_len+1);
	int bytes_read = fread(*content, 1, file_len, text_file);
	if (bytes_read != file_len) {
		printf("incorrect byte count %d versus %d\n", bytes_read, file_len);
	}
	fclose(text_file);
}

/******************* String to Word splitter... **************************/
struct list_of_words {
	int word_count;
	char **list;
};

void free_list_of_words(struct list_of_words lw) {
	int i;
	for (i = lw.word_count - 1; i >= 0; i--) {
		//printf("freeing word \"%s\" at address %d\n", lw.list[i], *(lw.list+i));
		free(*(lw.list+i));
	}
	free(lw.list);
}

void print_list_of_words(struct list_of_words lw) {
	int i;
	char *format = "%s, ";

	printf("[ ");
	for (i = 0; i < lw.word_count; i++) {
		printf(format, lw.list[i]);
	}
	printf("\x8\x8"); // erasing last delimitor
	printf(" ]\n");
}

struct list_of_words words_from_content(char *content) {
	int i = 0, word_count = 0, word_start = 0, word_length = 0;
	char **bucket = malloc_and_set(sizeof(char*)*10000); // allocating memory for 10000 pointers
	int len = strlen(content);

	//printf("DEBUG: string is >>>%s<<<\n", content);
	for (i = 0; i <= len; i++) {
		char c = *(content+i);
		// if we are on a delimitor char (comma, dot, space, tab, CR, LF)
		if (c == 44 || c == 46 || c == 32 || c == 9 || c == 10 || c == 13) {
			if (word_length > 0) { // we got a word...
				bucket[word_count] = malloc_and_set(word_length+1); // allocating memory for word
				strncpy(bucket[word_count], content+word_start, word_length);
				word_length = 0; word_start = i+1;
				word_count++;
				//printf("DEBUG: word >%s< copied into bucket\n", bucket[word_count-1]);
			} else {
				// hit a delimitor on a empty word, most likely two delimitor following each other
				word_start++;
			}
		} else {
			// lets increase word length
			word_length++;
			//printf("DEBUG: Char %d is %c (%d)\n", i, c, c);
		}
	}
	printf("DEBUG: creating list of words\n");
	struct list_of_words lw;
	lw.word_count = word_count;
	lw.list = malloc_and_set(sizeof(char*)*word_count); // reallocate needed amount of pointers
	for(i = 0; i < word_count; i++) {
		lw.list[i] = bucket[i];
	}
	printf("DEBUG: done copying, now freeing bucket\n");
	free(bucket);
	return lw;
}

/*************** lw_from_file *****************/

// Load up a text file from disk and return a list_of_words struct
struct list_of_words lw_from_file(char *filename) {
	char *content = NULL;
	content_from_file(&content, filename);
	struct list_of_words lw = words_from_content(content);
	free(content);
	return lw;
}

/*************** mergeSort *****************/

int* array_slice(int* array, int from, int end) {
	int size = (end - from) + 1;
	int *out = malloc_and_set(sizeof(int)*size);
	int i; for (i = 0; i < size; i++) {
		out[i] = array[from+i];
	}
	return out;
}

int* mergesort(int* array, int nsize) {
	if (nsize <= 1)
		return; // array of size 1 or less is always sorted, recursion ends;

	int mid = (nsize / 2);
	int *part_a = mergesort(array_slice(array, 0, mid), mid);
	int *part_b = mergesort(array_slice(array, mid, nsize), nsize-mid);

	int offset_a = 0, offset_b = 0;
	while (offset_a <= mid || offset_b <= (nsize-mid)) {
		if (part_a[offset_a] <= part_b[offset_b])
			array[offset_a+offset_b] = part_a[offset_a++];
		else
			array[offset_a+offset_b] = part_b[offset_b++];
	}
	while (offset_a <= mid) {
			array[offset_a+offset_b] = part_a[offset_a++];
	}
	while (offset_b <= (nsize-mid)) {
			array[offset_a+offset_b] = part_b[offset_b++];
	}
	free(part_a); free(part_b);
	return array;
}

/*************** QuickSort *****************/

void quicksort(int* array, int from, int end) {
	if (from >= end || end == 1)
		return;

	int min = from, max = end;
	int free_spot = from;
	int pivot = array[from];
	while (max > min) {
		if (free_spot == min) {
			if (array[max] > pivot) {
				max--;
			} else {
				array[free_spot] = array[max];
				free_spot = max;
				min++;
			}
		} else if (free_spot == max) {
			if (array[min] < pivot) {
				min++;
			} else {
				array[free_spot] = array[min];
				free_spot = min;
				max--;
			}
		}
	}

	array[free_spot] = pivot;

	quicksort(array, from, free_spot-1);
	quicksort(array, free_spot+1, end);
}

/*************** Hash table definition **********************/
struct entry {
	char *key;
	void *val;
};

// simple hash function that returns an index key for a value
// for a given string. Takes the ascii value of the first and last char
// adds them to the length of the string.
int key_to_index(char* key) {
	int len = strlen(key);
	int base_key;
	if (len <= 0)
		base_key = 0;
	else if (len == 1)
		base_key = key[0];
	else
		base_key = len * (key[0] + 2*key[1] + 4*key[len-1]);

	if (base_key < 0)
		return -1 * base_key;
	else
		return base_key;
}

	// hash table
	struct entry table[10000];

void main(void) {
	int i;
	struct list_of_words lw = lw_from_file("/home/mathieu/visa-worries.txt");
	print_list_of_words(lw);
	int* key_list = malloc(sizeof(int) * lw.word_count);
	for (i = 0; i < lw.word_count; i++) {
		key_list[i] = key_to_index(lw.list[i]);
		printf("index for word %s is %d\n", lw.list[i], key_list[i]);
	}
	int *copy = array_slice(key_list, 0, lw.word_count);
	print_num_array(mergesort(copy, lw.word_count), lw.word_count);
	free(copy);
	print_num_array(key_list, lw.word_count);
	quicksort(key_list, 0, lw.word_count);
	print_num_array(key_list, lw.word_count);
	print_string_array(lw.list, lw.word_count);
	free(key_list);
	free_list_of_words(lw);
}
