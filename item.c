#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "errors.h"
struct item create_item(char *owner, int priority, int execution_time, int *error){
	struct item process;
	static int pid = 1;
	*error = SUCCESS;
	errno = SUCCESS;
	process.data.user = NULL;
	process.data.user = strdup(owner);
	free(owner);
	if(process.data.user == NULL) {
		*error = errno;
		return process;
	}
	process.data.p = priority;
	process.data.C = execution_time;
	process.data.which = READY;
	process.pid = pid++;
	return process;
}
void free_item(struct item process) { //As user is the only data stored in dynamic memory, it is the only one we need to free
	free(process.data.user);
}
void print_item(struct item process, int index) {
	printf("%5d\t%5d\t%5d\t%5d\t", index + 1, process.pid, process.data.p, process.data.C);
	if(process.data.which == READY)
		printf("%10s\t", "READY");
	else if(process.data.which == RUNNING)
		printf("%10s\t", "RUNNING");
	else
		printf("%10s\t", "TERMINATED");
	printf("%21s", process.data.user);
}
int compare_pid(const void *pa, const void *pb) {
	struct item *a = (struct item *) pa;
	struct item *b = (struct item *) pb;
	if(a->pid == b->pid)
		return 0;
	if(a->pid < b->pid)
		return -1;
	return 1;
}
int compare_P(const void *pa, const void *pb) {
	struct item *a = (struct item *) pa;
	struct item *b = (struct item *) pb;
	if(a->data.p == b->data.p)
		return 0;
	if(a->data.p < b->data.p)
		return -1;
	return 1;
}
int compare_C(const void *pa, const void *pb) {
	struct item *a = (struct item *) pa;
	struct item *b = (struct item *) pb;
	if(a->data.C == b->data.C)
		return 0;
	if(a->data.C < b->data.C)
		return -1;
	return 1;
}
int compare_user(const void *pa, const void *pb) {
	struct item *a = (struct item *) pa;
	struct item *b = (struct item *) pb;
	return (strcmp(a->data.user, b->data.user));
}
