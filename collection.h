#include <stdio.h>
#include "item.h"
#include "input_usr.h"
#ifndef COLLECTION_H
#define COLLECTION_H
#define INI_SIZE 5
struct collection {
	int total_capacity;
	int number_occupied;
	struct item* schedule;
};
struct collection initialize_collection();	//copied from slides
struct collection clean_collection();		//copied from slides
int find_item_pid();
int find_item();
struct collection add_item();				//copied from slides
struct collection delete_item();			//inspired from slides
void print_header();
void print_all();
int check_yes_no();
void check_sorting_option();
struct collection sort_collection();		//copied from slides
#endif
