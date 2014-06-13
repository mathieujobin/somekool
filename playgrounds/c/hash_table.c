#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*************** File manipulation functions *********************/
int file_length(FILE *fp) {
	if (fseek(fp, 0L, SEEK_END)) { printf("error seeking to end...\n"); }
	int sz = ftell(fp);
	printf("DEBUG: file size is %d\n", sz);
	fseek(fp, 0L, SEEK_SET);
	return sz;
}

char* content_from_file(char *filename) {
	FILE* text_file = fopen(filename, "r");
	printf("DEBUG: pointer is %d\n", text_file);
	int file_len = file_length(text_file);
	char *content = (char*)malloc(file_len);
	int bytes_read = fread(content, 1, file_len, text_file);
	if (bytes_read != file_len) {
		printf("incorrect byte count %d versus %d\n", bytes_read, file_len);
	}
	fclose(text_file);
	return content;
}

/******************* String to Word splitter... **************************/
struct list_of_words {
	int word_count;
	char **list;
};

struct list_of_words words_from_content(char *content) {
	int i = 0, word_count = 0, word_start = 0, word_length = 0;
	char **bucket = malloc(sizeof(char*)*10000); // allocating memory for 10000 pointers
	char *word = NULL;
	int len = strlen(content);

	printf("DEBUG: string is >>>%s<<<\n", content);
	for (i = 0; i <= len; i++) {
		char c = *(content+i);
		// if we are on a delimitor char (comma, dot, space, tab, CR, LF)
		if (c == 44 || c == 46 || c == 32 || c == 9 || c == 10 || c == 13) {
			if (word_length > 0) { // we got a word...
				word = malloc(word_length+1); // allocating memory for word
				strncpy(word, content+word_start, word_length);
				word_length = 0; word_start = i+1;
				bucket[word_count++] = word;
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
	struct list_of_words lw;
	lw.word_count = word_count;
	lw.list = malloc(sizeof(char*)*word_count); // reallocate needed amount of pointers
	for(i = 0; i < word_count; i++) {
		lw.list[i] = bucket[i];
	}
	free(bucket);
	return lw;
}

/*************** Hash table definition **********************/
struct entry {
	char *key;
	void *val;
};

// simple function that returns an index key
// for a given string. Takes the ascii value of the first and last char
// adds them to the length of the string.
int key_to_index(char* key) {
	int len = strlen(key);
	return len + key[0] + key[len-1];
}

	// hash table
	struct entry table[10000];

void main(void) {
	int i;
	char *filename = "/home/mathieu/visa-worries.txt";
	struct list_of_words lw = words_from_content(content_from_file(filename));
	for (i = 0; i < lw.word_count; i++) {
		printf("index for word %s is %d\n", lw.list[i], key_to_index(lw.list[i]));
	}
}
