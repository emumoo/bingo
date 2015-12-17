// Author Lawrence Hook

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_WORD_SIZE 100

int init_terms();
void choose_terms();
int chosen(char *term);

FILE *terms_file;
char **terms;
char *choices[24];

int num_terms;


int main(int argc, char *argv[]) {
	FILE *write_file;
	FILE *template_file;

	char c;
	char write_name[20];
	int random, i, j;

	int param1 = strtol(argv[1], NULL, 0);
	
	// Seed the PRNG
	srand(time(NULL));

	// Initialize the terms from terms.txt
	num_terms = init_terms();

	for (j = 0; j < param1; j++) {

		sprintf(write_name, "bingo%i.tex", j+1);

		write_file = fopen(write_name, "w");


		// Open template
		template_file = fopen("bingo_template", "r");

		
		// Choose the ordering (and subset) of terms
		choose_terms();

		/*************************************************************************************************/
		/*                                                                                               */
		/*                                             BODY                                              */
		/*                                                                                               */
		/*************************************************************************************************/
		
		i = 0;
		c = fgetc(template_file);
		while (c != EOF) {
			if (c == '~') {
				fprintf(write_file, "%s", choices[i]);
				// printf("%s\n", choices[i]);
				i++;
			}
			else {
				fputc(c, write_file);
				// printf("%c", c);
			}
			c = fgetc(template_file);
		}

		/*************************************************************************************************/
		/*                                                                                               */
		/*                                          EPILOGUE                                             */
		/*                                                                                               */
		/*************************************************************************************************/



		fclose(write_file);
		// fclose(terms_file);
		// fclose(template_file);
	}
	

	// // deallocate terms
	// for (i = 0; i < num_terms; i++) {
	// 	free(terms[i]);
	// }
	// free(terms);

	return 0;
}

void choose_terms() {
	int random, i;

	// Clear choices
	for (i = 0; i < 24; i++) {
		free(choices[i]);
	}
	memset(choices, 0, sizeof(choices));

	
	random = rand() % num_terms;

	// Select ordering (and subset) of terms
	for (i = 0; i < 24; i++) {

		while (chosen(terms[random])) {
			random = rand() % num_terms;
		}
		
		choices[i] = malloc(strlen(terms[random]) + 1);
		memset(choices[i], 0, strlen(terms[random]) + 1);

		strcpy(choices[i], terms[random]);
	}

}

// return 1 if term has been chosen, 0 otherwise
int chosen(char *term) {
	int i;
	for (i = 0; i < 24; i++) {
		if (choices[i] == 0) {
			return 0;
		}
		if (strcmp(choices[i], term) == 0) {
			return 1;
		}
	}
	return 0;
}

// Return the number of terms
int init_terms() {
	char word[MAX_WORD_SIZE];
	int i = 0;

	terms_file = fopen("terms.txt", "r");
	
	// Find how many terms there are
	while (fgets(word, MAX_WORD_SIZE, terms_file) != NULL) {
		// Skip newlines
		if (strlen(word) > 0  && word[0] != 0x0a) {
			i++;
		}
	}

	// Clear word
	memset(word, 0, sizeof(word));
	
	// Allocate space for the terms
	terms = malloc(i * sizeof(char*));
	memset(terms, 0, i * sizeof(char*));

	// Reset the file pointer and variable i
	fseek(terms_file, 0, SEEK_SET);
	i = 0;
	
	// Store each term in terms
	while (fgets(word, MAX_WORD_SIZE, terms_file) != NULL) {
		// Skip newlines
		if (strlen(word) > 0 && word[0] != 0x0a) {
			
			terms[i] = malloc( strlen(word) + 1);
			memset(terms[i], 0, strlen(word) + 1);
			
			// Remove trailing newline
			if (word[strlen(word)-1] == 0x0a) {
				word[strlen(word)-1] = 0x00;
			}

			strcpy(terms[i], word);

			i++;
		}
		// Clear word
		memset(word, 0, sizeof(word));
	}

	// Return the number of terms
	return i;
}

