#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "collection.h"
#include "errors.h"
struct collection initialize_collection(int *error) {
	struct collection col;
	*error = SUCCESS;
	errno = SUCCESS;
	col.schedule = (struct item *) malloc(INI_SIZE * sizeof(struct item));
	if(col.schedule == NULL) {
		*error = errno;
		return col;
	}
	col.total_capacity = INI_SIZE;
	col.number_occupied = 0;
	return col;
}
struct collection clean_collection(struct collection col) {
	if(col.schedule != NULL) {
		for(int i = 0; i < col.number_occupied; i++)
			free_item(col.schedule[i]);
	}
	free(col.schedule);
	memset(&col, 0, sizeof(struct collection));
	return col;
}
int find_item_pid(struct collection col, int pid, int *error) {
	*error = SUCCESS;
	if(col.number_occupied == 0) {
		*error = ERR_NO_ITEMS;
		return *error;
	}
	for(int i = 0; i < col.number_occupied; i++) {
		if(col.schedule[i].pid == pid)
			return i;
	}
	*error = ERR_ELEMENT_NOT_FOUND;
	return *error;
}
int find_item(struct collection col, char *owner, int priority, int execution_time, int* error) {
	*error = SUCCESS;
	if(col.number_occupied == 0) {
		*error = ERR_NO_ITEMS;
		return *error;
	}
	for(int i = 0; i < col.number_occupied; i++) {
		if(strcmp(col.schedule[i].data.user, owner) == 0 && col.schedule[i].data.p == priority && col.schedule[i].data.C == execution_time){
			*error = ERR_ELEMENT_ALREADY_COL;
			return *error;
		}
	}
	*error = ERR_ELEMENT_NOT_FOUND;
	return *error;
}
struct collection add_item(struct collection col, char *owner, int priority, int execution_time, int *error) {
	*error = SUCCESS;
	errno = SUCCESS;
	if(col.number_occupied == col.total_capacity) {
		if(col.total_capacity == 0) { //Not initialized
			col = initialize_collection(error);
			if(*error != SUCCESS)
				return col;
		}
		else { //reached total capacity
			int new_capacity = col.total_capacity * 2;
			size_t ask = new_capacity * sizeof(struct item);
			struct item* tmp;
			tmp = (struct item*) realloc(col.schedule, ask);
			if(tmp == NULL) {
				*error = errno;
				return col;
			}
			col.schedule = tmp;
			col.total_capacity = new_capacity;
		}
	}
	col.schedule[col.number_occupied] = create_item(owner, priority, execution_time, error);
	if (*error != SUCCESS)
		return col;
	col.number_occupied++;
	return col;
}
struct collection delete_item(struct collection col, int index, int *error) {
	free_item(col.schedule[index]);
	col.number_occupied--;
	if(col.number_occupied != 0 && index != col.number_occupied) {
		memcpy(&col.schedule[index], &col.schedule[col.number_occupied], sizeof(struct item));
		col.schedule[index].data.user = strdup (col.schedule[col.number_occupied].data.user);
		free_item(col.schedule[col.number_occupied]);
	}
	return col;
}
void print_header() {
	printf("%5s\t%5s\t%5s\t%5s\t%10s\t%20s\n", "POS", "PID", "P", "C", "STAT", "OWNER");
}
void print_all(struct collection col, int *error) {
	*error = SUCCESS;
	if (col.number_occupied == 0){
		*error = ERR_NO_ITEMS;
		return;
	}
	print_header();
	for (int i = 0; i < col.number_occupied; i++) {
		print_item(col.schedule[i], i);
	}
}
int check_yes_no(char* string, int* error) {
	if(*error != SUCCESS)
		return -1;
	if(strcmp(string, "Yes\n") == 0 || strcmp(string, "YES\n") == 0 || strcmp(string, "Y\n") == 0 || strcmp(string, "y\n") == 0 || strcmp(string, "yes\n") == 0)
		return 0;
	else if(strcmp(string, "No\n") == 0 || strcmp(string, "NO\n") == 0 || strcmp(string, "N\n") == 0 || strcmp(string, "n\n") == 0 || strcmp(string, "no\n") == 0) {
		return 1;
	}
	printf("I did not understand you.\n");
	*error = ERR_INVALID_INPUT;
	return -1;
}
void check_sorting_option(int sorting_option, int* error) {
	if(*error != SUCCESS)
		return;
	if(sorting_option != 1 && sorting_option != 2 && sorting_option != 3 && sorting_option != 4)
		*error = ERR_INVALID_INPUT;
}
struct collection sort_collection(struct collection col, int (*compare)(const void *, const void *), int *error) {
	*error = SUCCESS;
	if (col.number_occupied != 0)
		qsort(col.schedule, col.number_occupied, sizeof(struct item), compare);
	else
		*error = ERR_NO_ITEMS;
	return col;
}
