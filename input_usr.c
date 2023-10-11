#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "input_usr.h"
#include "errors.h"
int ERROR_CTRLD = 0;
void handler_2(int signal) {
	ERROR_CTRLD = 1;
}
char* get_input(int *error) {
	ssize_t bytes_read;
	size_t number_bytes = 0;
	char* string = NULL;
	*error = SUCCESS;
	errno = SUCCESS;
	signal(SIGINT, handler_2);
	bytes_read = getline(&string, &number_bytes, stdin);
	if(errno != 0) {
		*error = errno;
		free(string);
		return NULL;
	}
	if(bytes_read == -1 || ERROR_CTRLD == 1) {
		clearerr(stdin);
		*error = ERR_CTRL_D;
		free(string);
		return NULL;
	}
	if(strlen(string) - 1 == 0) {
		*error = ERR_EMPTY_STRING;
		free(string);
		return NULL;
	}
	return string;
}
int get_integer(int* error) {
	*error = SUCCESS;
	errno = SUCCESS;
	char* endptr;
	char* string = get_input(error);
	if(*error != SUCCESS) {
		free(string);
		return *error;
	}
	int val = (int) strtol(string, &endptr, 10);
	if(strcmp(endptr, "\n") != 0)  {
		*error = ERR_CONTAIN_LETTER;
		free(string);
		return *error;
	}
	free(string);
	return val;
}
