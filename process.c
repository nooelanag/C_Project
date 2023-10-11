#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
	char* non_numeric;
	char* string = (char*) calloc(1, sizeof(char));
	char* aux_string;
	char character;
	int first_argument, second_argument, counter = 0;
	if(argc < 3) {
		fprintf(stderr, "Not enough arguments.\n");
		exit(EXIT_FAILURE);
	}
	else if(argc > 3) {
		fprintf(stderr, "Too many arguments.\n");
		exit(EXIT_FAILURE);
	}
	first_argument = strtol(argv[1], &non_numeric, 10);
	second_argument = strtol(argv[2], &non_numeric, 10);
	if(strlen(non_numeric) != 0 || first_argument <= 0 || second_argument <= 0) {
		fprintf(stderr, "Argument not integer or less than 0.\n");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < second_argument; i++) {
		character = '*';
		aux_string = (char*) calloc(i + 1, sizeof(char));
		strcpy(aux_string, string);
		free(string);
		string = aux_string;
		string[i] = character;
	}
	while(1) {
		printf("[%d] Process %d [%s] iter %d\n", getpid(), first_argument, string, ++counter);
	}
	free(string);
	return 0;
}

