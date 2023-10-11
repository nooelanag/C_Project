#ifndef ITEM_H
#define ITEM_H
enum state{READY, RUNNING, TERMINATED};
struct info {
	char *user;
	int p;
	int C;
	enum state which;
};
struct item {
	int pid;
	struct info data;
};
struct item create_item();	//copied from slides
void free_item();			//copied from slides
void print_item();
int compare_pid();			//copied from slides
int compare_P();
int compare_C();			//copied from slides
int compare_user();			//copied from slides
#endif
